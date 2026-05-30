# vostok-toolchain via a real Windows VM (QEMU/KVM)

An alternative to the Wine-based `scripts/create-toolchain-release.py`. Instead of
coaxing Wine's `msiexec` into an admin install (which leaves the static CRT at RTM
and needs the `overlay_sp1_crt` patch), we run a real Windows guest under QEMU/KVM
and let Windows' own installers do the work. A single command boots the VM,
installs VS2008 + SP1 + the DirectX SDK fully unattended, and drops the extracted
`msvc/` + `winsdk/` + `dxsdk/` trees onto a host-readable disk.

Why bother:

- Real `msiexec` applies SP1 to the **static CRT** (`libcmt.lib` = build 30729),
  so no overlay hack and no silent RTM-CRT drift.
- No wine-staging dependency, no `/Zi`->C1902 saga, no xvfb display flakiness.
- The guest is the same OS the toolchain targets, so byte-for-byte behaviour is
  the reference, not an emulation of it.

Cost: a full (if tiny) Windows install per build, an extra ISO to source, and
more moving parts (unattended answer file, in-guest orchestration, disk plumbing).

## Guest: Windows XP (chosen for size/speed)

XP gives a ~1.5 GB install, BIOS boot (SeaBIOS - no UEFI/TPM/secure-boot), and no
activation drama in a throwaway VM. Caveats to resolve with the real ISO:

- **VS2008's stated minimum is XP SP2.** XP SP1 may refuse the *full* VS2008
  installer. Mitigation: we use the **admin install** (`msiexec /a vs_setup.msi
  PATCH=<sp1.msp>`), which only unpacks files and has looser OS gating; on real
  Windows the PATCH is applied to the CRT correctly. If admin-install+PATCH still
  leaves the CRT RTM, fall back to a full install on an XP SP2/SP3 image.
- **Product key.** Unattended XP setup needs a key in `winnt.sif` that matches the
  ISO (VLK vs retail). Supplied out-of-band with the ISO.

## One-boot, fully-unattended flow

```
nix-shell create-toolchain-vm.nix      (WINDOWS_ISO=/path/to/winxp.iso)
  -> create-toolchain-vm.py:
     1. build inputs on the host (rootless):
        - unattend.flp : FAT12 floppy holding winnt.sif        (mtools)
        - data.img     : FAT32 disk: SP1 MSP + DXSDK_Jun10.exe + orchestrate.cmd,
                         and the guest also writes its output back here (mtools)
        - system.qcow2 : empty system disk XP installs onto      (qemu-img)
        The big VS2008 DVD is attached directly (no re-authoring).
     2. boot QEMU once (KVM, SeaBIOS) with all of the above attached:
        XP installs itself from the install CD via winnt.sif, then [GuiRunOnce]
        runs orchestrate.cmd from the data disk at first logon.
     3. orchestrate.cmd (in guest): admin-install VS2008 +PATCH=SP1 off the DVD,
        install DXSDK /U, copy VC/ + WinSDK + DXSDK back onto data.img, write
        DONE, shutdown -s.
     4. host waits for QEMU to exit, reads data.img with mtools (mcopy),
        assembles msvc/winsdk/dxsdk/ninja, normalises mtimes, tars vostok-
        toolchain-v0.100b.tar.xz, prints the sha256.
```

### IDE drive layout (XP gets 4 IDE devices out-of-box, no F6 driver disk)

| Drive | Contents                                                        |
|-------|-----------------------------------------------------------------|
| hda   | system.qcow2  (XP installs here)                                |
| hdb   | data.img      (FAT32: SP1 MSP + DXSDK + orchestrate.cmd; output)|
| hdc   | Windows XP install ISO (boot)                                   |
| hdd   | VS2008 DVD ISO (read vs_setup.msi + cabs directly)              |
| fda   | unattend.flp  (A:\winnt.sif)                                    |

Everything is IDE/PIIX3 so XP needs no extra storage drivers; virtio is avoided to
keep the install hands-free. The data disk doubles as installer source and output
sink, which keeps us within XP's four IDE devices without authoring a data DVD.
XP reads the VS2008 UDF DVD natively.

## Files

| File                    | Role                                                       |
|-------------------------|------------------------------------------------------------|
| `create-toolchain-vm.nix` | nix-shell: qemu/xorriso/mtools/dosfstools/p7zip + ISO env |
| `create-toolchain-vm.py`  | host driver: build disks, run QEMU, extract, package     |
| `winnt.sif`               | XP fully-unattended answer file (template)               |
| `orchestrate.cmd`         | in-guest: run installers, copy out, shut down            |

## Host-side reproducibility

The Windows *install* is not bit-reproducible (timestamps, SIDs), but we never
ship the image - only the extracted `VC/`/SDK files, which come straight from the
MSIs/installer payloads and are deterministic. Packaging normalises mtimes exactly
like the Wine path, so the tarball should be reproducible regardless of build host.

## Open questions / things to validate with the real ISO

1. Does `msiexec /a ... PATCH=<sp1.msp>` on real XP patch the static CRT to 30729?
   (If yes, no full install needed - this is the cheap, clean path.)
2. XP SP1 vs SP2 for whichever installer ends up being required.
3. DXSDK Jun2010 `/U` silent install on XP (the well-known S1023 error needs a
   newer VC++ redist present, which a clean XP will not have - so it should pass).
4. Drive-letter assignment for the data DVD inside the guest (orchestrate.cmd finds
   itself by `%~d0`, and finds the output disk by volume label, to stay robust).
