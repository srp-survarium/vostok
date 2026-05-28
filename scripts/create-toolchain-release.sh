#!/usr/bin/env bash
#
# create-toolchain-release.sh
#
# Produces a self-contained vostok-toolchain tarball with VS2008 SP1 correctly
# applied, ready to publish as a GitHub release. Once published, update
# flake.nix to fetch that release instead of building from the ISOs.
#
# How this differs from setup-toolchain.sh:
#   setup-toolchain.sh uses msitools (no Wine) to extract the base VC compiler,
#   which cannot apply SP1 (msiexec /p requires the product to be registered in
#   the Wine MSI database). This script uses wine msiexec /a (administrative
#   install) instead — it creates a flat file image of the VS2008 installation
#   without registering anything, and then wine msiexec /p /a patches that image
#   to SP1 level. The result is the correct SP1 compiler.
#
# Requirements:
#   - Run from inside `nix develop` (provides wine, 7z, msitools, python3)
#   - VS2008_ISO, VS2008_SP1_ISO, DXSDK_EXE, NINJA_WIN_ZIP env vars set
#     (the flake shellHook sets these from result symlinks under binaries/)
#   - ~10 GB of free disk space in WORK_DIR (default: binaries/.release-work)
#   - Wine may need a display for the admin install. If running headlessly,
#     try: Xvfb :99 -screen 0 1024x768x24 & export DISPLAY=:99
#
# Output:
#   binaries/vostok-toolchain-v0.100b.tar.xz
#     msvc/VC/   — VS2008 SP1 compiler (cl.exe, headers, x86 libs)
#     winsdk/    — Windows SDK 6.0A headers + x86 libs
#     dxsdk/     — DirectX SDK June 2010 headers + libs
#     ninja/     — ninja.exe v1.12.1
#
# After running, create a GitHub release and upload the tarball, then update
# flake.nix: replace vs2008-toolchain with a fetchurl pointing to the release.

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
VOSTOK_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"

# --- Input files ---
: "${VS2008_ISO:?VS2008_ISO not set — run from inside nix develop}"
: "${VS2008_SP1_ISO:?VS2008_SP1_ISO not set}"
: "${DXSDK_EXE:?DXSDK_EXE not set}"
: "${NINJA_WIN_ZIP:?NINJA_WIN_ZIP not set}"

# --- Output + work dirs ---
OUTPUT="$VOSTOK_DIR/binaries/vostok-toolchain-v0.100b.tar.xz"
WORK_DIR="${WORK_DIR:-$VOSTOK_DIR/binaries/.release-work}"
STAGE_DIR="$WORK_DIR/stage"      # final layout staged here before tar

mkdir -p "$WORK_DIR" "$STAGE_DIR"
trap 'echo "[release] Cleaning up work dir ..."; rm -rf "$WORK_DIR"' EXIT

export WINEPREFIX="$WORK_DIR/.wineprefix"
export WINEDEBUG=-all
export WINEDLLOVERRIDES="mscoree,mshtml="

echo "[release] Work dir: $WORK_DIR"
echo "[release] Output:   $OUTPUT"
echo ""

# ---------------------------------------------------------------------------
# 1. VS2008 administrative install + SP1 patch
# ---------------------------------------------------------------------------
echo "[release] Extracting VS2008 ISO ..."
mkdir -p "$WORK_DIR/vs2008-iso"
7z x "$VS2008_ISO" -tUDF -o"$WORK_DIR/vs2008-iso" -y > /dev/null || true

_vs_msi=""
for candidate in \
    "$WORK_DIR/vs2008-iso/Setup/vs_setup.msi" \
    "$WORK_DIR/vs2008-iso/vs_setup.msi"; do
  [ -f "$candidate" ] && _vs_msi="$candidate" && break
done
[ -z "$_vs_msi" ] && echo "[release] ERROR: vs_setup.msi not found in VS2008 ISO" && exit 1

echo "[release] Initialising Wine prefix ..."
mkdir -p "$WINEPREFIX"
wineboot --init
wineserver --wait 2>/dev/null || true

# Administrative install: extracts all VS2008 files to TARGETDIR as a flat
# source image, without registering the product. /qn = no UI.
# Note: if this fails with a display error, set DISPLAY=:99 (Xvfb) first.
echo "[release] Running VS2008 administrative install (this may take a while) ..."
mkdir -p "$WORK_DIR/vs-admin"
wine msiexec /a "$(winepath -w "$_vs_msi")" \
  TARGETDIR="$(winepath -w "$WORK_DIR/vs-admin")" \
  /qn 2>/dev/null || {
    echo "[release] WARNING: msiexec /a exited non-zero. Checking for files ..."
  }
wineserver --wait 2>/dev/null || true

# Find cl.exe in the administrative image.
_vc_bin=$(find "$WORK_DIR/vs-admin" -name "cl.exe" -path "*/bin/cl.exe" \
  ! -path "*/amd64/*" ! -path "*/x86_amd64/*" ! -path "*/ce/*" \
  -printf "%h\n" 2>/dev/null | head -1)

if [ -z "$_vc_bin" ]; then
  echo "[release] ERROR: cl.exe not found after administrative install."
  echo "  Possible causes:"
  echo "    - Wine msiexec /a requires a display. Try: Xvfb :99 & DISPLAY=:99 $0"
  echo "    - The ISO extraction was incomplete."
  find "$WORK_DIR/vs-admin" -maxdepth 4 -type d | head -20
  exit 1
fi
echo "[release] Found VC at: $(_vc_dir=$(dirname "$_vc_bin"); dirname "$_vc_dir")"

# Extract the SP1 ISO and find the MSP.
echo "[release] Extracting VS2008 SP1 ISO ..."
mkdir -p "$WORK_DIR/sp1-iso"
7z x "$VS2008_SP1_ISO" -tUDF -o"$WORK_DIR/sp1-iso" -y > /dev/null || true

_sp1_msp=""
for candidate in \
    "$WORK_DIR/sp1-iso/vs90sp1/VS90sp1-KB945140-X86-ENU.msp" \
    "$WORK_DIR/sp1-iso/VS90sp1-KB945140-X86-ENU.msp"; do
  [ -f "$candidate" ] && _sp1_msp="$candidate" && break
done

if [ -z "$_sp1_msp" ]; then
  echo "[release] WARNING: SP1 MSP not found — packaging base VS2008 without SP1."
else
  # Find the admin image MSI in TARGETDIR (msiexec /a copies it there).
  _admin_msi=$(find "$WORK_DIR/vs-admin" -maxdepth 2 -name "vs_setup.msi" | head -1)

  if [ -z "$_admin_msi" ]; then
    echo "[release] WARNING: Admin image vs_setup.msi not found — skipping SP1 patch."
  else
    echo "[release] Applying VS2008 SP1 to administrative image ..."
    # msiexec /p <patch> /a <admin-msi>: patches the admin image in-place.
    # Updated files will appear in the same directory tree as the admin install.
    wine msiexec /p "$(winepath -w "$_sp1_msp")" \
      /a "$(winepath -w "$_admin_msi")" \
      /qn 2>/dev/null || {
        echo "[release] WARNING: SP1 msiexec /p /a exited non-zero — SP1 may not be applied."
      }
    wineserver --wait 2>/dev/null || true

    # Re-locate cl.exe (SP1 may have moved or updated it).
    _vc_bin=$(find "$WORK_DIR/vs-admin" -name "cl.exe" -path "*/bin/cl.exe" \
      ! -path "*/amd64/*" ! -path "*/x86_amd64/*" ! -path "*/ce/*" \
      -printf "%h\n" 2>/dev/null | head -1)
    echo "[release] SP1 applied. cl.exe at: $( dirname "$_vc_bin" )"
  fi
fi

# Copy VC tree to stage.
_vc_dir=$(dirname "$_vc_bin")
_vs9_root=$(dirname "$(dirname "$_vc_dir")")
_ide_dir="$_vs9_root/Common7/IDE"

mkdir -p "$STAGE_DIR/msvc/VC"
cp -r "$_vc_dir"/. "$STAGE_DIR/msvc/VC/"
for f in mspdb80.dll mspdbcore.dll mspdbsrv.exe msobj80.dll; do
  [ -f "$_ide_dir/$f" ] && cp "$_ide_dir/$f" "$STAGE_DIR/msvc/VC/bin/"
done

# ---------------------------------------------------------------------------
# 2. Windows SDK 6.0A (msitools — no Wine needed)
# ---------------------------------------------------------------------------
echo "[release] Extracting Windows SDK ..."
# Reuse already-extracted ISO if present; else re-extract just WinSDK_Build.exe.
_sdk_build="$WORK_DIR/vs2008-iso/WCU/WinSDK/WinSDK_Build.exe"
if [ ! -f "$_sdk_build" ]; then
  7z x "$VS2008_ISO" -tUDF "WCU/WinSDK/WinSDK_Build.exe" \
    -o"$WORK_DIR/vs2008-iso" -y > /dev/null || true
fi
[ ! -f "$_sdk_build" ] && echo "[release] ERROR: WinSDK_Build.exe not found" && exit 1

mkdir -p "$WORK_DIR/sdk-sfx" "$WORK_DIR/sdk-extracted"
7z x "$_sdk_build" -o"$WORK_DIR/sdk-sfx" -y > /dev/null
[ ! -f "$WORK_DIR/sdk-sfx/VistaClientHeadersLibs-x86.msi" ] && \
  echo "[release] ERROR: VistaClientHeadersLibs-x86.msi not found" && exit 1
(cd "$WORK_DIR/sdk-sfx" && \
  msiextract -C "$WORK_DIR/sdk-extracted" VistaClientHeadersLibs-x86.msi)

_sdk_inc=$(find "$WORK_DIR/sdk-extracted" -maxdepth 6 -type d -name "Include" | head -1)
_sdk_lib=$(find "$WORK_DIR/sdk-extracted" -maxdepth 6 -type d -name "Lib" \
  ! -path "*/x64" | head -1)
[ -z "$_sdk_inc" ] && echo "[release] ERROR: WinSDK Include not found" && exit 1
[ -z "$_sdk_lib" ] && echo "[release] ERROR: WinSDK Lib not found" && exit 1

mkdir -p "$STAGE_DIR/winsdk"
cp -r "$_sdk_inc" "$STAGE_DIR/winsdk/Include"
cp -r "$_sdk_lib" "$STAGE_DIR/winsdk/Lib"
echo "[release] WinSDK: $(ls "$STAGE_DIR/winsdk/Include" | wc -l) headers."

# ---------------------------------------------------------------------------
# 3. DirectX SDK
# ---------------------------------------------------------------------------
echo "[release] Extracting DirectX SDK ..."
mkdir -p "$WORK_DIR/dxsdk"
7z x "$DXSDK_EXE" -o"$WORK_DIR/dxsdk" -y > /dev/null
mkdir -p "$STAGE_DIR/dxsdk"
for candidate in "$WORK_DIR/dxsdk/DXSDK" "$WORK_DIR/dxsdk"; do
  if [ -d "$candidate/Include" ]; then
    cp -r "$candidate/Include" "$STAGE_DIR/dxsdk/"
    cp -r "$candidate/Lib"     "$STAGE_DIR/dxsdk/"
    break
  fi
done
[ ! -d "$STAGE_DIR/dxsdk/Include" ] && echo "[release] ERROR: DXSDK Include not found" && exit 1
echo "[release] DXSDK: $(ls "$STAGE_DIR/dxsdk/Include" | wc -l) headers."

# ---------------------------------------------------------------------------
# 4. ninja.exe
# ---------------------------------------------------------------------------
echo "[release] Extracting ninja.exe ..."
mkdir -p "$STAGE_DIR/ninja"
7z e "$NINJA_WIN_ZIP" ninja.exe -o"$STAGE_DIR/ninja" -y > /dev/null
[ ! -f "$STAGE_DIR/ninja/ninja.exe" ] && echo "[release] ERROR: ninja.exe not found" && exit 1

# ---------------------------------------------------------------------------
# 5. Package
# ---------------------------------------------------------------------------
echo "[release] Packaging → $OUTPUT ..."
mkdir -p "$(dirname "$OUTPUT")"
# Strip STAGE_DIR prefix so the tarball extracts to vostok-toolchain/.
tar -C "$WORK_DIR/stage" \
  --transform "s|^\.|vostok-toolchain-v0.100b|" \
  -cJf "$OUTPUT" .

SHA256=$(sha256sum "$OUTPUT" | cut -d' ' -f1)
SIZE=$(du -sh "$OUTPUT" | cut -f1)

echo ""
echo "[release] Done!"
echo "  Output: $OUTPUT  ($SIZE)"
echo "  SHA256: $SHA256"
echo ""
echo "Next steps:"
echo "  1. Create a GitHub release in this repo (e.g., tag toolchain-v0.100b)."
echo "  2. Upload $OUTPUT as a release asset."
echo "  3. In flake.nix, replace the vs2008-toolchain derivation with:"
echo ""
echo "     vs2008-toolchain = pkgs.runCommandNoCC \"vs2008-toolchain\" {"
echo "       src = pkgs.fetchurl {"
echo "         name = \"vostok-toolchain-v0.100b.tar.xz\";"
echo "         url  = \"https://github.com/<org>/vostok/releases/download/toolchain-v0.100b/vostok-toolchain-v0.100b.tar.xz\";"
echo "         sha256 = \"$SHA256\";"
echo "       };"
echo "       nativeBuildInputs = [ pkgs.gnutar pkgs.xz ];"
echo "     } ''"
echo "       mkdir -p \"\$out\""
echo "       tar xf \"\$src\" -C \"\$out\" --strip-components=1"
echo "     '';"
echo ""
echo "  4. Remove the _vs2008-iso-src and _vs2008-sp1-iso-src internal fetches."
