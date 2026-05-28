#!/usr/bin/env bash
#
# setup-toolchain.sh
#
# Extracts the MSVC 8.0 (VS2008) compiler, Windows SDK v6.0A, DirectX SDK
# June 2010, and ninja.exe into a self-contained portable toolchain at:
#   binaries/toolchain/msvc/   — cl.exe, lib.exe, link.exe, headers, libs
#   binaries/toolchain/winsdk/ — Windows SDK headers + x86 libs
#   binaries/toolchain/dxsdk/  — DirectX SDK headers + x86 libs
#   binaries/toolchain/ninja/  — ninja.exe
#
# Then configures the local Wine prefix so cl.exe finds everything.
#
# Run once after `nix develop` and after building the installer downloads:
#   nix build .#vs2008-iso        --out-link binaries/result-vs2008-iso
#   nix build .#vs2008-sp1-iso    --out-link binaries/result-vs2008-sp1-iso
#   nix build .#dxsdk             --out-link binaries/result-dxsdk
#   nix build .#ninja-win         --out-link binaries/result-ninja-win
#
# For game binaries (survarium.exe + .pdb) — handled separately by Nix:
#   nix build .#survarium-game    --out-link binaries/result-survarium-game
#
# Required env vars (set automatically by flake.nix devShell):
#   VS2008_ISO      — path to the VS2008 Pro DVD ISO in the Nix store
#   VS2008_SP1_ISO  — path to the VS2008 SP1 ISO in the Nix store
#   DXSDK_EXE           — path to DXSDK_Jun10.exe in the Nix store
#   NINJA_WIN_ZIP       — path to ninja-win.zip in the Nix store
#   MSVC_DIR        — destination for the portable MSVC toolchain
#   WINSDK_DIR      — destination for the Windows SDK
#   DXSDK_DIR       — destination for the extracted DirectX SDK
#   WINEPREFIX      — local Wine prefix directory (vostok/.wineprefix)
#
# Optional env vars:
#   VOSTOK_LIBS_DIR — where vostok-libs is/will be (default: ../vostok-libs)
#   SKIP_LIBS       — set to 1 to skip the vostok-libs download step

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
VOSTOK_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"

: "${VS2008_ISO:?VS2008_ISO not set — run this from inside nix develop}"
: "${VS2008_SP1_ISO:?VS2008_SP1_ISO not set}"
: "${DXSDK_EXE:?DXSDK_EXE not set}"
: "${NINJA_WIN_ZIP:?NINJA_WIN_ZIP not set}"
: "${MSVC_DIR:="$VOSTOK_DIR/binaries/toolchain/msvc"}"
: "${WINSDK_DIR:="$VOSTOK_DIR/binaries/toolchain/winsdk"}"
: "${DXSDK_DIR:="$VOSTOK_DIR/binaries/toolchain/dxsdk"}"
: "${NINJA_DIR:="$VOSTOK_DIR/binaries/toolchain/ninja"}"
: "${WINEPREFIX:="$VOSTOK_DIR/binaries/.wineprefix"}"
: "${VOSTOK_LIBS_DIR:="$(cd "$VOSTOK_DIR/.." && pwd)/vostok-libs"}"
: "${SKIP_LIBS:=0}"

CLEAN=0
for arg in "$@"; do
  [[ "$arg" == "--clean" ]] && CLEAN=1
done

if [[ $CLEAN -eq 1 ]]; then
  echo "[setup] --clean: removing $MSVC_DIR, $WINSDK_DIR, $DXSDK_DIR, $NINJA_DIR"
  rm -rf "$MSVC_DIR" "$WINSDK_DIR" "$DXSDK_DIR" "$NINJA_DIR"
fi

# ---------------------------------------------------------------------------
# 0a. Download vostok-libs (proprietary binaries: DLLs, LIBs)
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

    # The archive may extract to a subdirectory; detect and flatten if needed.
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

# Work dir on /home (not tmpfs /) — extraction needs several GB of space.
mkdir -p "$VOSTOK_DIR/binaries"
WORK_DIR="$(mktemp -d "$VOSTOK_DIR/binaries/.setup-work-XXXXXX")"
trap 'rm -rf "$WORK_DIR"' EXIT

export WINEPREFIX
export WINEDEBUG=-all
# Suppress Wine Mono / Gecko installation pop-up dialogs.
export WINEDLLOVERRIDES="mscoree,mshtml="

# Find msiextract: in PATH (inside nix develop) or search nix store.
_find_msiextract() {
  command -v msiextract 2>/dev/null || \
    find /nix/store -maxdepth 4 -name "msiextract" 2>/dev/null | head -1
}

# ---------------------------------------------------------------------------
# 0. Initialise Wine prefix
# ---------------------------------------------------------------------------
if [[ ! -d "$WINEPREFIX/drive_c" ]]; then
  echo "[setup] Initialising Wine prefix at $WINEPREFIX ..."
  wineboot --init
  echo "[setup] Wine prefix ready."
fi

# ---------------------------------------------------------------------------
# 1. Extract VS2008 VC compiler via msiextract (no Wine installer needed)
# ---------------------------------------------------------------------------
if [[ ! -d "$MSVC_DIR/VC/bin" ]]; then
  echo "[setup] Extracting VS2008 ISO ..."
  mkdir -p "$WORK_DIR/vs2008"
  # Exit code 2 = non-fatal UDF sector warnings; files extract fine.
  7z x "$VS2008_ISO" -tUDF -o"$WORK_DIR/vs2008" -y > /dev/null || true

  _vs_msi="$WORK_DIR/vs2008/Setup/vs_setup.msi"
  if [[ ! -f "$_vs_msi" ]]; then
    _vs_msi="$WORK_DIR/vs2008/vs_setup.msi"
  fi
  if [[ ! -f "$_vs_msi" ]]; then
    echo "[setup] ERROR: vs_setup.msi not found in ISO."
    find "$WORK_DIR/vs2008" -maxdepth 3 -name "*.msi" | head -10
    exit 1
  fi

  _msiextract="$(_find_msiextract)"
  if [[ -z "$_msiextract" ]]; then
    echo "[setup] ERROR: msiextract not found. Run inside nix develop."
    exit 1
  fi

  echo "[setup] Extracting VC compiler with msiextract ..."
  mkdir -p "$WORK_DIR/vs2008-extracted"
  (cd "$(dirname "$_vs_msi")" && "$_msiextract" -C "$WORK_DIR/vs2008-extracted" "$(basename "$_vs_msi")")

  # cl.exe lives in a dir named "VC:Vc7" (MSI internal component separator).
  _vc_bin="$(find "$WORK_DIR/vs2008-extracted" -name "cl.exe" -path "*/bin/cl.exe" \
    ! -path "*/amd64/*" ! -path "*/x86_amd64/*" ! -path "*/ce/*" \
    -printf "%h\n" | head -1)"

  if [[ -z "$_vc_bin" ]]; then
    echo "[setup] ERROR: cl.exe not found after msiextract."
    find "$WORK_DIR/vs2008-extracted" -maxdepth 4 -type d | head -20
    exit 1
  fi

  _vc_dir="$(dirname "$_vc_bin")"
  _vs9_root="$(dirname "$(dirname "$_vc_dir")")"
  _ide_dir="$_vs9_root/Common7/IDE"

  echo "[setup] Copying MSVC toolchain → $MSVC_DIR ..."
  mkdir -p "$MSVC_DIR"
  cp -r "$_vc_dir" "$MSVC_DIR/VC"

  # PDB DLLs must live next to cl.exe for PDB writing to work.
  for f in mspdb80.dll mspdbcore.dll mspdbsrv.exe msobj80.dll; do
    [[ -f "$_ide_dir/$f" ]] && cp "$_ide_dir/$f" "$MSVC_DIR/VC/bin/"
  done

  echo "[setup] Base VS2008 VC compiler extracted."
else
  echo "[setup] MSVC compiler already present, skipping."
fi

# ---------------------------------------------------------------------------
# 2. Apply VS2008 SP1
# ---------------------------------------------------------------------------
SP1_STAMP="$MSVC_DIR/.sp1-applied"
if [[ ! -f "$SP1_STAMP" ]]; then
  echo "[setup] Extracting VS2008 SP1 ISO ..."
  mkdir -p "$WORK_DIR/sp1"
  7z x "$VS2008_SP1_ISO" -tUDF -o"$WORK_DIR/sp1" -y > /dev/null || true

  _sp1_msp=""
  for candidate in \
      "$WORK_DIR/sp1/vs90sp1/VS90sp1-KB945140-X86-ENU.msp" \
      "$WORK_DIR/sp1/VS90sp1-KB945140-X86-ENU.msp"; do
    [[ -f "$candidate" ]] && _sp1_msp="$candidate" && break
  done

  if [[ -z "$_sp1_msp" ]]; then
    echo "[setup] WARNING: VS90 SP1 MSP not found — skipping SP1."
    touch "$SP1_STAMP"
  else
    echo "[setup] Applying VS2008 SP1 (wine msiexec /p ...) ..."
    wine msiexec /p "$(winepath -w "$_sp1_msp")" /qn || {
      echo "[setup] WARNING: SP1 msiexec exited non-zero."
    }
    wineserver --wait 2>/dev/null || true
    sleep 5

    _vc_bin_sp1=""
    for candidate in \
        "$WINEPREFIX/drive_c/Program Files/Microsoft Visual Studio 9.0/VC/bin" \
        "$WINEPREFIX/drive_c/Program Files (x86)/Microsoft Visual Studio 9.0/VC/bin"; do
      [[ -d "$candidate" && -f "$candidate/cl.exe" ]] && _vc_bin_sp1="$candidate" && break
    done

    if [[ -n "$_vc_bin_sp1" ]]; then
      echo "[setup] Overlaying SP1 binaries ..."
      _vc_root="$(dirname "$(dirname "$_vc_bin_sp1")")"
      cp -r "$_vc_root/VC/bin/." "$MSVC_DIR/VC/bin/"
      cp -r "$_vc_root/VC/include/." "$MSVC_DIR/VC/include/" 2>/dev/null || true
      cp -r "$_vc_root/VC/lib/." "$MSVC_DIR/VC/lib/" 2>/dev/null || true
      _ide_dir="$_vc_root/Common7/IDE"
      for f in mspdb80.dll mspdbcore.dll mspdbsrv.exe msobj80.dll; do
        [[ -f "$_ide_dir/$f" ]] && cp "$_ide_dir/$f" "$MSVC_DIR/VC/bin/"
      done
      echo "[setup] SP1 applied."
    else
      echo "[setup] WARNING: cl.exe not found in Wine prefix after SP1 — SP1 may not have applied."
    fi

    touch "$SP1_STAMP"
  fi
else
  echo "[setup] SP1 already applied, skipping."
fi

# ---------------------------------------------------------------------------
# 3. Extract Windows SDK v6.0A headers + x86 libs from VS2008 ISO
# ---------------------------------------------------------------------------
if [[ ! -d "$WINSDK_DIR/Include" ]]; then
  echo "[setup] Extracting Windows SDK v6.0A from VS2008 ISO ..."

  # The SDK is bundled as WCU/WinSDK/WinSDK_Build.exe — a sfxcab containing
  # VistaClientHeadersLibs-x86.msi (headers + x86 libs).
  mkdir -p "$WORK_DIR/vs2008-sdk"
  7z x "$VS2008_ISO" -tUDF "WCU/WinSDK/WinSDK_Build.exe" -o"$WORK_DIR/vs2008-sdk" -y > /dev/null || true

  _sdk_build_exe="$WORK_DIR/vs2008-sdk/WCU/WinSDK/WinSDK_Build.exe"
  if [[ ! -f "$_sdk_build_exe" ]]; then
    echo "[setup] ERROR: WinSDK_Build.exe not found in ISO."
    exit 1
  fi

  # sfxcab — extract with 7z to get the MSI + CABs
  mkdir -p "$WORK_DIR/sdk-sfx"
  7z x "$_sdk_build_exe" -o"$WORK_DIR/sdk-sfx" -y > /dev/null

  _sdk_msi="$WORK_DIR/sdk-sfx/VistaClientHeadersLibs-x86.msi"
  if [[ ! -f "$_sdk_msi" ]]; then
    echo "[setup] ERROR: VistaClientHeadersLibs-x86.msi not found after sfxcab extraction."
    ls "$WORK_DIR/sdk-sfx/"
    exit 1
  fi

  _msiextract="$(_find_msiextract)"
  mkdir -p "$WORK_DIR/sdk-extracted"
  (cd "$WORK_DIR/sdk-sfx" && "$_msiextract" -C "$WORK_DIR/sdk-extracted" VistaClientHeadersLibs-x86.msi)

  # msiextract uses MSI component separators in dir names (colons).
  # Find the Include and Lib directories under the extracted tree.
  _sdk_include="$(find "$WORK_DIR/sdk-extracted" -maxdepth 6 -type d -name "Include" | head -1)"
  _sdk_lib="$(find "$WORK_DIR/sdk-extracted" -maxdepth 6 -type d -name "Lib" ! -path "*/x64" | head -1)"

  if [[ -z "$_sdk_include" ]] || [[ -z "$_sdk_lib" ]]; then
    echo "[setup] ERROR: Could not find Include/ or Lib/ in extracted Windows SDK."
    find "$WORK_DIR/sdk-extracted" -maxdepth 6 -type d | head -20
    exit 1
  fi

  echo "[setup] Copying Windows SDK → $WINSDK_DIR ..."
  mkdir -p "$WINSDK_DIR"
  cp -r "$_sdk_include" "$WINSDK_DIR/Include"
  cp -r "$_sdk_lib"     "$WINSDK_DIR/Lib"

  echo "[setup] Windows SDK extracted ($(ls "$WINSDK_DIR/Include" | wc -l) headers, $(ls "$WINSDK_DIR/Lib" | wc -l) libs)."
else
  echo "[setup] Windows SDK already present, skipping."
fi

# ---------------------------------------------------------------------------
# 4. Extract DirectX SDK June 2010
# ---------------------------------------------------------------------------
if [[ ! -d "$DXSDK_DIR/Include" ]]; then
  echo "[setup] Extracting DirectX SDK → $DXSDK_DIR ..."
  mkdir -p "$WORK_DIR/dxsdk"
  7z x "$DXSDK_EXE" -o"$WORK_DIR/dxsdk" -y > /dev/null

  mkdir -p "$DXSDK_DIR"
  for candidate in "$WORK_DIR/dxsdk/DXSDK" "$WORK_DIR/dxsdk"; do
    if [[ -d "$candidate/Include" ]]; then
      cp -r "$candidate/Include" "$DXSDK_DIR/"
      cp -r "$candidate/Lib"     "$DXSDK_DIR/"
      break
    fi
  done

  if [[ ! -d "$DXSDK_DIR/Include" ]]; then
    echo "[setup] WARNING: Could not find Include/ in extracted DX SDK."
    find "$WORK_DIR/dxsdk" -maxdepth 3 -type d | head -20
  else
    echo "[setup] DirectX SDK extracted."
  fi
else
  echo "[setup] DirectX SDK already present, skipping."
fi

# ---------------------------------------------------------------------------
# 5. Extract ninja.exe
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
# 6. Configure Wine environment (PATH, INCLUDE, LIB)
# ---------------------------------------------------------------------------
ENV_STAMP="$MSVC_DIR/.wine-env-set"
# Regenerate if the stamp is older than MSVC or WinSDK dirs (handles re-runs).
if [[ ! -f "$ENV_STAMP" ]] || [[ "$MSVC_DIR/VC/bin/cl.exe" -nt "$ENV_STAMP" ]]; then
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

  touch "$ENV_STAMP"
  echo "[setup] Wine environment configured."
  echo "  PATH+=   $WINE_VC_BIN"
  echo "  INCLUDE= $WINE_INCLUDE_VAL"
  echo "  LIB=     $WINE_LIB_VAL"
else
  echo "[setup] Wine environment already configured."
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
