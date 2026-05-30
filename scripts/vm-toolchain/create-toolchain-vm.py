#!/usr/bin/env python3
"""
create-toolchain-vm.py - build the vostok-toolchain inside a real Windows XP VM
(QEMU/KVM) instead of Wine, so Windows' own msiexec applies VS2008 SP1 to the
static CRT. One unattended boot: XP installs itself, runs the SDK installers, and
drops the extracted trees onto a host-readable FAT disk; the host then packages
them exactly like the Wine path.

Run via the nix-shell wrapper:
  WINDOWS_ISO=/path/to/winxp.iso nix-shell scripts/vm-toolchain/create-toolchain-vm.nix

Env (the .nix wrapper sets all but the first two):
  WINDOWS_ISO          path to the Windows XP install ISO            (required)
  WINDOWS_PRODUCT_KEY  XXXXX-XXXXX-XXXXX-XXXXX-XXXXX for winnt.sif   (required)
  VS2008_ISO           VS2008 Pro DVD ISO
  VS2008_SP1_ISO       VS2008 SP1 ISO (we pull the one MSP out of it)
  DXSDK_EXE            DXSDK_Jun10.exe
  NINJA_WIN_ZIP        ninja-win.zip
  VM_VNC               e.g. ":2" to watch/debug the install (default: headless)
  VM_TIMEOUT_MIN       hard cap on the VM run (default: 90)
"""

import os
import shutil
import subprocess
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
VOSTOK_DIR = Path(os.environ.get("VOSTOK_DIR", HERE.parents[1]))
WORK = VOSTOK_DIR / "binaries" / ".vm-work"
OUT_TARBALL = VOSTOK_DIR / "binaries" / "vostok-toolchain-v0.100b.tar.xz"

# Survarium v0.100b release date - stamped on every tar entry for reproducibility.
RELEASE_EPOCH = 1368100800

SP1_MSP_NAME = "VS90sp1-KB945140-X86-ENU.msp"


def log(msg: str) -> None:
    print(f"[vm] {msg}", flush=True)


def run(cmd: list, **kw) -> subprocess.CompletedProcess:
    kw.setdefault("check", True)
    return subprocess.run([str(c) for c in cmd], **kw)


def need(var: str) -> str:
    val = os.environ.get(var, "").strip()
    if not val:
        sys.exit(f"ERROR: {var} is not set (see the header of {HERE.name}).")
    return val


def to_crlf(text: str) -> bytes:
    return text.replace("\r\n", "\n").replace("\n", "\r\n").encode("ascii")


# ---------------------------------------------------------------------------
# Host-side input disks
# ---------------------------------------------------------------------------

def build_unattend_floppy(work: Path, product_key: str) -> Path:
    """1.44 MB FAT12 floppy holding A:\\WINNT.SIF (mtools, rootless)."""
    sif = (HERE / "winnt.sif").read_text().replace("@PRODUCT_KEY@", product_key)
    sif_file = work / "winnt.sif"
    sif_file.write_bytes(to_crlf(sif))

    flp = work / "unattend.flp"
    run(["mformat", "-C", "-f", "1440", "-v", "VOSTOK", "-i", flp, "::"])
    run(["mcopy", "-i", flp, sif_file, "::WINNT.SIF"])
    log(f"unattend floppy: {flp}")
    return flp


# All mtools calls target partition 1 of a raw disk image at its byte offset via
# the `image@@offset` syntax, so we never need root, loopback, or a partition
# mount. MTOOLS_SKIP_CHECK quiets benign geometry mismatches on the big FAT32.
MTOOLS_ENV = {**os.environ, "MTOOLS_SKIP_CHECK": "1"}


def _part1_offset(img: Path) -> int:
    """Byte offset of partition 1 in a DOS-partitioned image (via sfdisk -d)."""
    out = run(["sfdisk", "-d", img], capture_output=True, text=True).stdout
    for line in out.splitlines():
        if "start=" in line:
            return int(line.split("start=")[1].split(",")[0]) * 512
    sys.exit(f"ERROR: no partition found in {img}")


def build_data_disk(work: Path, sp1_iso: Path, dxsdk_exe: Path) -> Path:
    """Partitioned FAT32 disk the guest reads installers from and writes output to.

    Read side: sp1\\<msp>, DXSDK_Jun10.exe, vostok\\orchestrate.cmd.
    Write side: an empty out\\ the guest fills.
    Rootless: sfdisk writes the MBR, mtools formats/populates partition 1 at its
    offset (image@@offset). XP mounts it as an ordinary FAT32 hard disk.
    """
    stage = work / "data"
    if stage.exists():
        shutil.rmtree(stage)
    (stage / "sp1").mkdir(parents=True)
    (stage / "vostok").mkdir(parents=True)
    (stage / "out").mkdir(parents=True)

    # Pull the single SP1 MSP out of the SP1 ISO (7z); the guest applies it.
    sp1_extract = work / "sp1-iso"
    if sp1_extract.exists():
        shutil.rmtree(sp1_extract)
    run(["7z", "x", sp1_iso, "-tUDF", f"-o{sp1_extract}", "-y"],
        stdout=subprocess.DEVNULL, check=False)
    msp = next(sp1_extract.rglob(SP1_MSP_NAME), None)
    if not msp:
        sys.exit(f"ERROR: {SP1_MSP_NAME} not found in {sp1_iso}")
    shutil.copy2(msp, stage / "sp1" / SP1_MSP_NAME)

    shutil.copy2(dxsdk_exe, stage / "DXSDK_Jun10.exe")
    orch = (HERE / "orchestrate.cmd").read_text()
    (stage / "vostok" / "orchestrate.cmd").write_bytes(to_crlf(orch))
    (stage / "out" / ".keep").write_text("")

    data = work / "data.img"
    if data.exists():
        data.unlink()
    run(["truncate", "-s", "4G", data])
    run(["sfdisk", data], input="label: dos\n,,c\n", text=True,
        stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    img = f"{data}@@{_part1_offset(data)}"
    run(["mformat", "-i", img, "-F", "-v", "VOSTOKDATA", "::"], env=MTOOLS_ENV)
    for top in ("sp1", "vostok", "out"):
        run(["mcopy", "-i", img, "-s", stage / top, "::"], env=MTOOLS_ENV)
    run(["mcopy", "-i", img, stage / "DXSDK_Jun10.exe", "::"], env=MTOOLS_ENV)
    log(f"data disk: {data}")
    return data


def build_system_disk(work: Path) -> Path:
    sysd = work / "system.qcow2"
    if sysd.exists():
        sysd.unlink()
    run(["qemu-img", "create", "-f", "qcow2", sysd, "12G"],
        stdout=subprocess.DEVNULL)
    log(f"system disk: {sysd}")
    return sysd


# ---------------------------------------------------------------------------
# Run the VM
# ---------------------------------------------------------------------------

def run_vm(system: Path, data: Path, win_iso: Path, vs_iso: Path) -> None:
    timeout_min = int(os.environ.get("VM_TIMEOUT_MIN", "90"))
    vnc = os.environ.get("VM_VNC", "").strip()
    display = ["-vnc", f"127.0.0.1{vnc}"] if vnc else ["-display", "none"]

    cmd = [
        "qemu-system-i386",
        "-machine", "pc", "-accel", "kvm", "-cpu", "host",
        "-m", "2048", "-smp", "2",
        # IDE: hda system, hdb data, hdc Windows install CD, hdd VS2008 DVD
        "-drive", f"file={system},if=ide,index=0,media=disk,format=qcow2",
        "-drive", f"file={data},if=ide,index=1,media=disk,format=raw",
        "-drive", f"file={win_iso},if=ide,index=2,media=cdrom",
        "-drive", f"file={vs_iso},if=ide,index=3,media=cdrom",
        "-fda", str(WORK / "unattend.flp"),
        # boot CD first; on the post-install reboots the "press any key" prompt
        # times out and control falls through to the now-installed HDD. (No
        # -no-reboot: XP reboots between text-mode and GUI setup.)
        "-boot", "order=d",
        "-rtc", "base=localtime",
        "-net", "nic,model=rtl8139", "-net", "user",
        "-name", "vostok-xp-build",
        *display,
    ]
    log(f"booting VM (timeout {timeout_min} min, "
        f"{'vnc '+vnc if vnc else 'headless'}) ...")
    log("  " + " ".join(cmd))
    try:
        run(cmd, check=False, timeout=timeout_min * 60)
    except subprocess.TimeoutExpired:
        sys.exit(f"ERROR: VM exceeded {timeout_min} min - inspect with VM_VNC.")
    log("VM powered off.")


# ---------------------------------------------------------------------------
# Extract + package
# ---------------------------------------------------------------------------

def extract_output(data: Path, work: Path) -> Path:
    """Copy /out off the data disk's FAT partition with mtools (rootless)."""
    dest = work / "out"
    if dest.exists():
        shutil.rmtree(dest)
    dest.mkdir(parents=True)
    img = f"{data}@@{_part1_offset(data)}"
    run(["mcopy", "-i", img, "-s", "-n", "-Q", "::/out", dest], env=MTOOLS_ENV)
    got = dest / "out"
    if not (got / "DONE").exists():
        log("WARNING: no DONE marker - the in-guest build likely failed. "
            f"Check {got}/build.log")
    return got


def main() -> None:
    win_iso = Path(need("WINDOWS_ISO"))
    product_key = need("WINDOWS_PRODUCT_KEY")
    vs_iso = Path(need("VS2008_ISO"))
    sp1_iso = Path(need("VS2008_SP1_ISO"))
    dxsdk = Path(need("DXSDK_EXE"))
    if not win_iso.is_file():
        sys.exit(f"ERROR: WINDOWS_ISO not found: {win_iso}")

    WORK.mkdir(parents=True, exist_ok=True)
    log(f"work dir: {WORK}")

    build_unattend_floppy(WORK, product_key)
    data = build_data_disk(WORK, sp1_iso, dxsdk)
    system = build_system_disk(WORK)

    run_vm(system, data, win_iso, vs_iso)

    out = extract_output(data, WORK)
    log(f"guest output staged at {out}")
    # TODO(package): assemble msvc/winsdk/dxsdk/ninja from `out`, normalise mtimes
    # to RELEASE_EPOCH, tar.xz -> OUT_TARBALL, print sha256 + verify libcmt SP1.
    log("Next: package + verify libcmt.lib is SP1 30729 (not yet implemented).")


if __name__ == "__main__":
    main()
