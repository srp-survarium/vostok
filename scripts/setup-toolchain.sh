#!/usr/bin/env bash
#
# setup-toolchain.sh
#
# Extracts a portable MSVC 8.0 (VS2008) toolchain and the DirectX SDK June 2010
# from the installer images downloaded into the Nix store, then initialises a
# local Wine prefix that can run cl.exe / lib.exe / link.exe natively on Linux.
#
# Run once after `nix develop`. Re-running is safe — existing directories are
# not clobbered unless you pass --clean.
#
# Required env vars (set automatically by flake.nix devShell):
#   VS2008_ISO      — path to the VS2008 Pro DVD ISO in the Nix store
#   VS2008_SP1_ISO  — path to the VS2008 SP1 ISO in the Nix store
#   DXSDK_EXE       — path to DXSDK_Jun10.exe in the Nix store
#   MSVC_DIR        — destination for the portable MSVC toolchain
#   DXSDK_DIR       — destination for the extracted DirectX SDK
#   WINEPREFIX      — local Wine prefix directory (vostok/.wineprefix)
#   WINEARCH        — should be win32

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
VOSTOK_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"

: "${VS2008_ISO:?VS2008_ISO not set — run this from inside nix develop}"
: "${VS2008_SP1_ISO:?VS2008_SP1_ISO not set}"
: "${DXSDK_EXE:?DXSDK_EXE not set}"
: "${NINJA_WIN_ZIP:?NINJA_WIN_ZIP not set}"
: "${MSVC_DIR:="$VOSTOK_DIR/toolchain/msvc"}"
: "${DXSDK_DIR:="$VOSTOK_DIR/toolchain/dxsdk"}"
: "${NINJA_DIR:="$VOSTOK_DIR/toolchain/ninja"}"
: "${WINEPREFIX:="$VOSTOK_DIR/.wineprefix"}"
: "${WINEARCH:=win32}"

CLEAN=0
for arg in "$@"; do
  [[ "$arg" == "--clean" ]] && CLEAN=1
done

if [[ $CLEAN -eq 1 ]]; then
  echo "[setup] --clean: removing $MSVC_DIR, $DXSDK_DIR, $NINJA_DIR, $WINEPREFIX"
  rm -rf "$MSVC_DIR" "$DXSDK_DIR" "$NINJA_DIR" "$WINEPREFIX"
fi

WORK_DIR="$(mktemp -d)"
trap 'rm -rf "$WORK_DIR"' EXIT

# ---------------------------------------------------------------------------
# 1. Extract portable MSVC 8.0 toolchain from the VS2008 ISO
# ---------------------------------------------------------------------------
if [[ ! -d "$MSVC_DIR/VC/bin" ]]; then
  echo "[setup] Extracting VS2008 ISO → $MSVC_DIR ..."
  mkdir -p "$WORK_DIR/vs2008"

  # 7z can read ISO 9660 images directly.
  7z x "$VS2008_ISO" -o"$WORK_DIR/vs2008" -y > /dev/null

  # The VC compiler files live under VC/ inside the ISO.
  # Common7/IDE contains the PDB DLLs that cl.exe depends on.
  mkdir -p \
    "$MSVC_DIR/VC/bin" \
    "$MSVC_DIR/VC/include" \
    "$MSVC_DIR/VC/lib" \
    "$MSVC_DIR/VC/atlmfc/include" \
    "$MSVC_DIR/VC/atlmfc/lib"

  _vc="$WORK_DIR/vs2008/VC"
  _ide="$WORK_DIR/vs2008/Common7/IDE"

  # Compiler front-end + back-end + linker + librarian
  for f in cl.exe c1.dll c1xx.dll c2.dll link.exe lib.exe ml.exe \
            mspft80.dll pgort90.dll; do
    [[ -f "$_vc/bin/$f" ]] && cp "$_vc/bin/$f" "$MSVC_DIR/VC/bin/"
  done

  # PDB support DLLs live in Common7/IDE (must sit next to cl.exe)
  for f in mspdb80.dll mspdbcore.dll mspdbsrv.exe msobj80.dll; do
    [[ -f "$_ide/$f" ]] && cp "$_ide/$f" "$MSVC_DIR/VC/bin/"
  done

  # CRT and platform headers / libs
  cp -r "$_vc/include/." "$MSVC_DIR/VC/include/"
  cp -r "$_vc/lib/."     "$MSVC_DIR/VC/lib/"

  if [[ -d "$_vc/atlmfc" ]]; then
    cp -r "$_vc/atlmfc/include/." "$MSVC_DIR/VC/atlmfc/include/" 2>/dev/null || true
    cp -r "$_vc/atlmfc/lib/."     "$MSVC_DIR/VC/atlmfc/lib/"     2>/dev/null || true
  fi

  echo "[setup] Base VS2008 toolchain extracted."
else
  echo "[setup] MSVC toolchain already present, skipping VS2008 ISO extraction."
fi

# ---------------------------------------------------------------------------
# 2. Apply SP1 — overlay updated compiler binaries
# ---------------------------------------------------------------------------
SP1_STAMP="$MSVC_DIR/.sp1-applied"
if [[ ! -f "$SP1_STAMP" ]]; then
  echo "[setup] Applying VS2008 SP1 ..."
  mkdir -p "$WORK_DIR/sp1"
  7z x "$VS2008_SP1_ISO" -o"$WORK_DIR/sp1" -y > /dev/null

  # SP1 packages its updates in patch cabinet files.
  # The layout inside the SP1 ISO varies; common locations:
  #   packages/vcrtl/  or  vc/   or root-level cab files.
  # We search for updated cl.exe and overlay it if found.
  for updated_cl in \
      "$WORK_DIR/sp1/packages/vcrtl/vc/bin/cl.exe" \
      "$WORK_DIR/sp1/vc/bin/cl.exe"; do
    if [[ -f "$updated_cl" ]]; then
      echo "[setup] SP1: overlaying updated VC binaries"
      cp -r "$(dirname "$updated_cl")/." "$MSVC_DIR/VC/bin/" 2>/dev/null || true
      break
    fi
  done

  # Also overlay updated headers/libs if present
  for sp1_inc in \
      "$WORK_DIR/sp1/packages/vcrtl/vc/include" \
      "$WORK_DIR/sp1/vc/include"; do
    if [[ -d "$sp1_inc" ]]; then
      cp -r "$sp1_inc/." "$MSVC_DIR/VC/include/" 2>/dev/null || true
      break
    fi
  done

  touch "$SP1_STAMP"
  echo "[setup] SP1 applied (or no matching files found — investigate $WORK_DIR/sp1 if needed)."
else
  echo "[setup] SP1 already applied, skipping."
fi

# ---------------------------------------------------------------------------
# 3. Extract DirectX SDK June 2010
# ---------------------------------------------------------------------------
if [[ ! -d "$DXSDK_DIR/Include" ]]; then
  echo "[setup] Extracting DirectX SDK → $DXSDK_DIR ..."
  mkdir -p "$WORK_DIR/dxsdk"
  # DXSDK_Jun10.exe is a self-extracting archive; 7z handles it.
  7z x "$DXSDK_EXE" -o"$WORK_DIR/dxsdk" -y > /dev/null

  mkdir -p "$DXSDK_DIR"
  # The SDK may have a nested directory structure after extraction.
  for candidate in \
      "$WORK_DIR/dxsdk" \
      "$WORK_DIR/dxsdk/DXSDK"; do
    if [[ -d "$candidate/Include" ]]; then
      cp -r "$candidate/Include" "$DXSDK_DIR/"
      cp -r "$candidate/Lib"     "$DXSDK_DIR/"
      break
    fi
  done

  if [[ ! -d "$DXSDK_DIR/Include" ]]; then
    echo "[setup] WARNING: Could not find Include/ inside extracted DX SDK."
    echo "  Extracted layout:"
    find "$WORK_DIR/dxsdk" -maxdepth 3 -type d | head -30
  else
    echo "[setup] DirectX SDK extracted."
  fi
else
  echo "[setup] DirectX SDK already present, skipping."
fi

# ---------------------------------------------------------------------------
# 4. Extract ninja.exe (Windows binary, run under Wine)
# ---------------------------------------------------------------------------
if [[ ! -f "$NINJA_DIR/ninja.exe" ]]; then
  echo "[setup] Extracting ninja.exe → $NINJA_DIR ..."
  mkdir -p "$NINJA_DIR"
  7z e "$NINJA_WIN_ZIP" -o"$NINJA_DIR" ninja.exe -y > /dev/null
  echo "[setup] ninja.exe ready."
else
  echo "[setup] ninja.exe already present, skipping."
fi

# ---------------------------------------------------------------------------
# 5. Write vcvars32.sh — sets up the toolchain environment for a shell session
# ---------------------------------------------------------------------------
VCVARS="$MSVC_DIR/vcvars32.sh"
cat > "$VCVARS" <<EOF
# vcvars32.sh — portable MSVC 8.0 environment for Wine
# Source this file: . "$VCVARS"

export MSVC_DIR="$MSVC_DIR"
export DXSDK_DIR="$DXSDK_DIR"
export NINJA_DIR="$NINJA_DIR"
export WINEPREFIX="$WINEPREFIX"
export WINEARCH=win32

# Wine paths (Z: = Linux root under Wine)
WINE_MSVC_BIN="\$(winepath -w "\$MSVC_DIR/VC/bin")"
WINE_MSVC_INC="\$(winepath -w "\$MSVC_DIR/VC/include")"
WINE_MSVC_LIB="\$(winepath -w "\$MSVC_DIR/VC/lib")"
WINE_DXSDK_INC="\$(winepath -w "\$DXSDK_DIR/Include")"
WINE_DXSDK_LIB="\$(winepath -w "\$DXSDK_DIR/Lib/x86")"
WINE_NINJA="\$(winepath -w "\$NINJA_DIR/ninja.exe")"

export WINE_CL="\$WINE_MSVC_BIN\\\\cl.exe"
export WINE_LIB="\$WINE_MSVC_BIN\\\\lib.exe"
export WINE_LINK="\$WINE_MSVC_BIN\\\\link.exe"
export WINE_NINJA="\$WINE_NINJA"

# INCLUDE and LIB env vars passed into the Wine environment for cl/link
export WINE_INCLUDE="\$WINE_MSVC_INC;\$WINE_DXSDK_INC"
export WINE_LIB_PATH="\$WINE_MSVC_LIB;\$WINE_DXSDK_LIB"
EOF
chmod +x "$VCVARS"

# ---------------------------------------------------------------------------
# 6. Initialise the Wine prefix
# ---------------------------------------------------------------------------
if [[ ! -d "$WINEPREFIX/drive_c" ]]; then
  echo "[setup] Initialising Wine prefix at $WINEPREFIX ..."
  WINEPREFIX="$WINEPREFIX" WINEARCH="$WINEARCH" wineboot --init 2>/dev/null
  echo "[setup] Wine prefix ready."
else
  echo "[setup] Wine prefix already initialised."
fi

echo ""
echo "[setup] Done. Toolchain layout:"
echo "  MSVC:   $MSVC_DIR/VC/"
echo "  DXSDK:  $DXSDK_DIR/"
echo "  Ninja:  $NINJA_DIR/ninja.exe"
echo "  Wine:   $WINEPREFIX/"
echo ""
echo "  Test cl.exe:    WINEPREFIX=$WINEPREFIX wine $MSVC_DIR/VC/bin/cl.exe"
echo "  Test ninja.exe: WINEPREFIX=$WINEPREFIX wine $NINJA_DIR/ninja.exe --version"
