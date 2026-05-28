#!/usr/bin/env bash
#
# setup-toolchain.sh
#
# Configures the Wine environment (PATH, INCLUDE, LIB registry entries) for
# the Nix-built toolchain, and downloads vostok-libs from Google Drive.
#
# Build the Nix packages first (all fetched and extracted automatically):
#   nix build .#vostok-toolchain  --out-link binaries/result-vostok-toolchain  # 3.3 GB
#
# Required env vars (set automatically by flake.nix devShell):
#   MSVC_DIR     — path to the VC compiler directory (has VC/bin/cl.exe)
#   WINSDK_DIR   — path to the Windows SDK directory (has Include/ and Lib/)
#   DXSDK_DIR    — path to the DirectX SDK directory (has Include/ and Lib/)
#   NINJA_DIR    — path to the directory containing ninja.exe
#   WINEPREFIX   — Wine prefix directory
#
# Optional env vars:
#   VOSTOK_LIBS_DIR — where vostok-libs is/will be (default: ../vostok-libs)
#   SKIP_LIBS       — set to 1 to skip the vostok-libs download step

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
VOSTOK_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"

: "${MSVC_DIR:?MSVC_DIR not set — run this from inside nix develop}"
: "${WINSDK_DIR:?WINSDK_DIR not set}"
: "${DXSDK_DIR:?DXSDK_DIR not set}"
: "${NINJA_DIR:?NINJA_DIR not set}"
: "${WINEPREFIX:="$VOSTOK_DIR/binaries/.wineprefix"}"
: "${VOSTOK_LIBS_DIR:="$(cd "$VOSTOK_DIR/.." && pwd)/vostok-libs"}"
: "${SKIP_LIBS:=0}"

export WINEPREFIX
export WINEDEBUG=-all
# Suppress Wine Mono / Gecko installation pop-up dialogs.
export WINEDLLOVERRIDES="mscoree,mshtml="

if [[ ! -d "$MSVC_DIR/VC/bin" ]]; then
  echo "[setup] ERROR: cl.exe not found under $MSVC_DIR/VC/bin"
  echo "  Build the Nix toolchain: nix build .#vostok-toolchain --out-link binaries/result-vostok-toolchain"
  exit 1
fi

# ---------------------------------------------------------------------------
# 0. Download vostok-libs (proprietary binaries: DLLs, LIBs)
# ---------------------------------------------------------------------------
# These are third-party pre-built libraries required by the game build.
# Source: https://drive.google.com/file/d/1ShmDiJArDcOiuPA0DyswEzhXHeRa0Psx
_GDRIVE_LIBS_ID="1ShmDiJArDcOiuPA0DyswEzhXHeRa0Psx"
_LIBS_ARCHIVE="$VOSTOK_DIR/.setup-vostok-libs-download"

if [[ "$SKIP_LIBS" == "1" ]]; then
  echo "[setup] SKIP_LIBS=1: skipping vostok-libs download."
elif [[ -d "$VOSTOK_LIBS_DIR/sources" ]]; then
  echo "[setup] vostok-libs already present at $VOSTOK_LIBS_DIR, skipping download."
else
  echo "[setup] Downloading vostok-libs from Google Drive ..."

  _gdown="$(command -v gdown 2>/dev/null || true)"
  if [[ -z "$_gdown" ]]; then
    echo "[setup] WARNING: gdown not found. Run inside nix develop, or set SKIP_LIBS=1 to skip."
    echo "  Manual download: https://drive.google.com/file/d/$_GDRIVE_LIBS_ID"
  else
    "$_gdown" "$_GDRIVE_LIBS_ID" -O "$_LIBS_ARCHIVE" --fuzzy

    echo "[setup] Extracting vostok-libs → $VOSTOK_LIBS_DIR ..."
    mkdir -p "$VOSTOK_LIBS_DIR"
    7z x "$_LIBS_ARCHIVE" -o"$VOSTOK_LIBS_DIR" -y > /dev/null

    _nested="$(find "$VOSTOK_LIBS_DIR" -maxdepth 1 -mindepth 1 -type d | head -1)"
    if [[ -d "$_nested/sources" ]] && [[ ! -d "$VOSTOK_LIBS_DIR/sources" ]]; then
      echo "[setup] Flattening nested archive directory ..."
      mv "$_nested"/* "$VOSTOK_LIBS_DIR/" && rmdir "$_nested"
    fi

    rm -f "$_LIBS_ARCHIVE"

    if [[ ! -d "$VOSTOK_LIBS_DIR/sources" ]]; then
      echo "[setup] WARNING: Expected $VOSTOK_LIBS_DIR/sources not found after extraction."
      echo "  Contents: $(ls "$VOSTOK_LIBS_DIR" | head -5)"
    else
      echo "[setup] vostok-libs extracted. Running copy_lib_files.py ..."
      python3 "$VOSTOK_DIR/scripts/copy_lib_files.py" \
        "$VOSTOK_LIBS_DIR/sources" "$VOSTOK_DIR/sources"
      echo "[setup] Library files copied into sources/."
    fi
  fi
fi

# ---------------------------------------------------------------------------
# 1. Initialise Wine prefix
# ---------------------------------------------------------------------------
if [[ ! -d "$WINEPREFIX/drive_c" ]]; then
  echo "[setup] Initialising Wine prefix at $WINEPREFIX ..."
  mkdir -p "$WINEPREFIX"
  wineboot --init
  wineserver --wait 2>/dev/null || true
  echo "[setup] Wine prefix ready."
fi

# ---------------------------------------------------------------------------
# 2. Configure Wine environment (PATH, INCLUDE, LIB)
# ---------------------------------------------------------------------------
# Stamp stores the resolved MSVC path so we re-run if the toolchain changes
# (e.g., after a Nix rebuild producing a different store path).
ENV_STAMP="$WINEPREFIX/.vostok-env-set"
_msvc_real="$(readlink -f "$MSVC_DIR")"

if [[ -f "$ENV_STAMP" ]] && [[ "$(cat "$ENV_STAMP")" == "$_msvc_real" ]]; then
  echo "[setup] Wine environment already configured."
else
  echo "[setup] Configuring Wine environment (PATH, INCLUDE, LIB) ..."

  WINE_VC_BIN="$(winepath -w "$MSVC_DIR/VC/bin")"
  WINE_VC_INC="$(winepath -w "$MSVC_DIR/VC/include")"
  WINE_VC_LIB="$(winepath -w "$MSVC_DIR/VC/lib")"
  WINE_WINSDK_INC="$(winepath -w "$WINSDK_DIR/Include")"
  WINE_WINSDK_LIB="$(winepath -w "$WINSDK_DIR/Lib")"
  WINE_DXSDK_INC="$(winepath -w "$DXSDK_DIR/Include")"
  WINE_DXSDK_LIB="$(winepath -w "$DXSDK_DIR/Lib/x86")"

  # DXSDK must precede WinSDK: d3d9types.h in WinSDK 6.0A is an older version
  # without DX9Ex types (D3DAUTHENTICATEDCHANNELTYPE etc.); same include guard
  # would shadow the DXSDK version if WinSDK comes first.
  WINE_INCLUDE_VAL="$WINE_VC_INC;$WINE_DXSDK_INC;$WINE_WINSDK_INC"
  WINE_LIB_VAL="$WINE_VC_LIB;$WINE_DXSDK_LIB;$WINE_WINSDK_LIB"

  _REG_ENV="HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment"

  _CURRENT_PATH="$(wine reg query "$_REG_ENV" /v PATH 2>/dev/null \
    | grep -i "REG_" | awk '{print $NF}')"
  if [[ -n "$_CURRENT_PATH" && "$_CURRENT_PATH" != *"$WINE_VC_BIN"* ]]; then
    wine reg add "$_REG_ENV" /v PATH /t REG_EXPAND_SZ \
      /d "$WINE_VC_BIN;$_CURRENT_PATH" /f > /dev/null 2>&1
  elif [[ -z "$_CURRENT_PATH" ]]; then
    wine reg add "$_REG_ENV" /v PATH /t REG_EXPAND_SZ \
      /d "$WINE_VC_BIN;%SystemRoot%\\system32;%SystemRoot%" /f > /dev/null 2>&1
  fi

  wine reg add "$_REG_ENV" /v INCLUDE /t REG_SZ /d "$WINE_INCLUDE_VAL" /f > /dev/null 2>&1
  wine reg add "$_REG_ENV" /v LIB     /t REG_SZ /d "$WINE_LIB_VAL"     /f > /dev/null 2>&1

  echo "$_msvc_real" > "$ENV_STAMP"
  echo "[setup] Wine environment configured."
  echo "  PATH+=   $WINE_VC_BIN"
  echo "  INCLUDE= $WINE_INCLUDE_VAL"
  echo "  LIB=     $WINE_LIB_VAL"
fi

echo ""
echo "[setup] Done. Toolchain layout:"
echo "  MSVC:    $MSVC_DIR/VC/"
echo "  WinSDK:  $WINSDK_DIR/"
echo "  DXSDK:   $DXSDK_DIR/"
echo "  Ninja:   $NINJA_DIR/ninja.exe"
echo "  Wine:    $WINEPREFIX/"
echo ""
echo "  Verify: WINEPREFIX=$WINEPREFIX wine $MSVC_DIR/VC/bin/cl.exe"
echo "  Verify: WINEPREFIX=$WINEPREFIX wine $NINJA_DIR/ninja.exe --version"
