# vm-toolchain - handoff / parked state

**Status: parked (2026-05-30).** The Wine + `overlay_sp1_crt` path (branch
`fix/toolchain-sp1-crt`) already produces a verified SP1-CRT toolchain
(`libcmt.lib` = build 30729 confirmed inside the packaged tarball), and the
RTM-vs-SP1 drift was established to be *entirely* the static CRT - so this VM
approach buys cleanliness, not correctness. PR #100 was opened then closed; this
branch (`explore/qemu-toolchain`) keeps the work for later.

This doc captures everything needed to resume without re-deriving it.

## Why this exists

Real Windows `msiexec` applies VS2008 SP1 to the static CRT, which Wine's does
not - so a VM build would need no `overlay_sp1_crt` hack and would dodge
wine-staging/C1902/xvfb. The cost is a full (if tiny) Windows install per build
plus the in-guest plumbing here. We decided that cost is not worth it *right now*
because the overlay already fixes the whole known problem.

## What is DONE and VALIDATED (host side, no Windows ISO needed)

- **KVM**: `/dev/kvm` is present and mode 0666 (usable by this non-root user);
  CPU has `vmx`. `qemu-system-i386 -accel kvm -cpu host` initialises fine.
- **Rootless disk plumbing** (the part that took the most thought):
  - `sfdisk` writes an MBR with one FAT32 partition to a plain raw file
    (`printf 'label: dos\n,,c\n' | sfdisk data.img`); partition 1 lands at byte
    offset 1048576 (sector 2048).
  - `mtools` formats/reads/writes that partition at its offset via the
    `image@@OFFSET` syntax (`mformat -i data.img@@1048576 -F ::`, then
    `mcopy -i data.img@@1048576 ...`). No root, no loopback, no libguestfs.
  - **Avoid `libguestfs`**: `libguestfs-with-appliance` *builds from source* on
    this nixpkgs pin (slow). The sfdisk+mtools route replaced it.
  - Long filenames survive (VFAT LFN), so guest paths like
    `DXSDK_Jun10.exe` / `VS90sp1-KB945140-X86-ENU.msp` resolve.
- **All three input disks build from the real cached inputs**: 1.44M FAT12
  floppy (`winnt.sif`, key substituted, CRLF), 4G FAT32 data disk (real 159M SP1
  MSP + 599M DXSDK + `orchestrate.cmd`), and the qcow2 system disk.
- **Exact QEMU invocation smoke-tested headless** - the `pc` machine takes all
  four IDE devices (hda/hdb/hdc/hdd) + the floppy under KVM with no errors.
- **`package()`** emits a reproducible `tar.xz` with the same
  `--sort/--mtime/--owner` normalisation and `vostok-toolchain-v0.100b/`
  transform as the Wine path (verified against a synthetic output tree).
- **`verify_libcmt_sp1()`** both branches unit-checked (`30729` -> OK;
  `21022` -> "not SP1, full install likely needed").
- `ruff` clean.

## What is NOT done (gated on a local Windows XP ISO + product key)

1. The actual unattended boot (XP install via `winnt.sif`, `[GuiRunOnce]`).
2. In-guest orchestration really running (`orchestrate.cmd`):
   - **Open question (the crux):** does real-Windows
     `msiexec /a vs_setup.msi PATCH=<sp1.msp>` patch the static CRT to 30729? If
     yes, the cheap admin-install path works; if not, a *full* VS2008 install is
     needed.
   - **XP SP1 vs SP2:** VS2008's stated minimum is XP SP2. Admin install has
     looser gating; a full install may refuse on SP1. The user planned to supply
     the ISO and a matching product key.
   - DXSDK `/U` silent install; guest drive-letter assignment (handled
     defensively by searching D:-I:).
3. **WinSDK 6.0A**: `orchestrate.cmd` has only a best-effort copy from the
   admin-install tree. The Wine path instead extracts it from the VS2008 ISO's
   `WCU\WinSDK\WinSDK_Build.exe`; the VM may need to run that explicitly.
4. Swap `WINDOWS_ISO` (env, local) for a pinned archive.org `fetchurl` once a
   stable URL is known.

## Key fact reused from the Wine work

The SP1 static CRT ships as *whole files* (not deltas) in
**`VC90sp1-KB947888-x86-enu.msp`** (the VC patch, NOT the umbrella
`VS90sp1-KB945140` IDE patch), keyed like
`FL_libcmt_lib_7051_x86_ln.<GUID>` -> `libcmt.lib`. The VM avoids all of this by
letting Windows apply the patch, but if the admin-install route fails to patch
the CRT, this is where the SP1 `libcmt.lib` lives.

## How to resume

```bash
cd vostok-wt-qemu-toolchain   # branch explore/qemu-toolchain
WINDOWS_ISO=/path/winxp.iso WINDOWS_PRODUCT_KEY=XXXXX-XXXXX-XXXXX-XXXXX-XXXXX \
  VM_VNC=:2 nix-shell scripts/vm-toolchain/create-toolchain-vm.nix
```

First run: watch via a VNC client on `127.0.0.1:5902`. Everything the guest does
is logged to `build.log` on the data disk, so a failed in-guest run is debuggable
from the host:

```bash
OFF=$(sfdisk -d binaries/.vm-work/data.img | sed -n 's/.*start=[ ]*\([0-9]*\).*/\1/p' | head -1)
mcopy -i binaries/.vm-work/data.img@@$((OFF*512)) ::/build.log -   # print the guest log
```

IDE drive layout: hda=system.qcow2, hdb=data.img (FAT32: installers + output),
hdc=Windows install ISO, hdd=VS2008 DVD, fda=unattend floppy. See README.md.

## Files

`create-toolchain-vm.nix`, `create-toolchain-vm.py`, `winnt.sif`,
`orchestrate.cmd`, `README.md` (architecture), this `HANDOFF.md`.
