#!/usr/bin/env bash
#
# setup-toolchain.sh
#
# Installs VS2008 + SP1 under a local Wine prefix, then copies the resulting
# VC compiler files into a self-contained portable toolchain at toolchain/msvc/.
# Also extracts the DirectX SDK and ninja.exe.
#
# Run once after `nix develop` and after building the installer downloads:
#   nix build .#vs2008-iso    --out-link result-vs2008-iso
#   nix build .#vs2008-sp1-iso --out-link result-vs2008-sp1-iso
#   nix build .#dxsdk          --out-link result-dxsdk
#   nix build .#ninja-win       --out-link result-ninja-win
#
# Required env vars (set automatically by flake.nix devShell):
#   VS2008_ISO      — path to the VS2008 Pro DVD ISO in the Nix store
#   VS2008_SP1_ISO  — path to the VS2008 SP1 ISO in the Nix store
#   DXSDK_EXE       — path to DXSDK_Jun10.exe in the Nix store
#   NINJA_WIN_ZIP   — path to ninja-win.zip in the Nix store
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
  echo "[setup] --clean: removing $MSVC_DIR, $DXSDK_DIR, $NINJA_DIR"
  rm -rf "$MSVC_DIR" "$DXSDK_DIR" "$NINJA_DIR"
fi

WORK_DIR="$(mktemp -d)"
trap 'rm -rf "$WORK_DIR"' EXIT

export WINEPREFIX WINEARCH
export WINEDEBUG=-all  # suppress Wine spam

# ---------------------------------------------------------------------------
# 0. Initialise Wine prefix if not yet done
# ---------------------------------------------------------------------------
if [[ ! -d "$WINEPREFIX/drive_c" ]]; then
  echo "[setup] Initialising Wine prefix at $WINEPREFIX ..."
  wineboot --init
  echo "[setup] Wine prefix ready."
fi

# ---------------------------------------------------------------------------
# 1. Install VS2008 under Wine, then copy out the VC toolchain
# ---------------------------------------------------------------------------
if [[ ! -d "$MSVC_DIR/VC/bin" ]]; then
  echo "[setup] Extracting VS2008 ISO ..."
  mkdir -p "$WORK_DIR/vs2008"

  # VS2008 Pro DVD uses UDF filesystem (-tUDF required for 7z)
  7z x "$VS2008_ISO" -tUDF -o"$WORK_DIR/vs2008" -y > /dev/null

  echo "[setup] Running VS2008 installer under Wine (this takes a few minutes) ..."
  # Run vcsetup.exe (VC-only installer) silently.
  # /q = quiet, /ADDLOCAL=ALL = install all VC features
  # vcsetup.exe lives at the root or under VC/ in the ISO.
  _vs_setup=""
  for candidate in \
      "$WORK_DIR/vs2008/vc/vcsetup.exe" \
      "$WORK_DIR/vs2008/VC/vcsetup.exe" \
      "$WORK_DIR/vs2008/vcsetup.exe" \
      "$WORK_DIR/vs2008/setup.exe"; do
    if [[ -f "$candidate" ]]; then
      _vs_setup="$candidate"
      break
    fi
  done

  if [[ -z "$_vs_setup" ]]; then
    echo "[setup] ERROR: Could not find VS2008 setup executable in ISO."
    echo "  Extracted layout:"
    find "$WORK_DIR/vs2008" -maxdepth 2 -name "*.exe" | head -20
    exit 1
  fi

  echo "[setup] Running: wine $(basename "$_vs_setup") /q ADDLOCAL=ALL"
  wine "$_vs_setup" /q ADDLOCAL=ALL || {
    echo "[setup] WARNING: Installer exited non-zero. Checking if files were installed..."
  }

  # Wait a moment for Wine processes to settle
  sleep 5
  wineserver --wait 2>/dev/null || true

  # Find where VS2008 was installed in the Wine prefix
  _vc_bin=""
  for candidate in \
      "$WINEPREFIX/drive_c/Program Files/Microsoft Visual Studio 9.0/VC/bin" \
      "$WINEPREFIX/drive_c/Program Files (x86)/Microsoft Visual Studio 9.0/VC/bin"; do
    if [[ -d "$candidate" && -f "$candidate/cl.exe" ]]; then
      _vc_bin="$candidate"
      break
    fi
  done

  if [[ -z "$_vc_bin" ]]; then
    echo "[setup] ERROR: cl.exe not found in Wine prefix after installation."
    echo "  Expected at: $WINEPREFIX/drive_c/Program Files/Microsoft Visual Studio 9.0/VC/bin"
    echo "  Wine prefix drive_c contents:"
    ls "$WINEPREFIX/drive_c/Program Files/" 2>/dev/null | head -20
    exit 1
  fi

  _vc_root="$(dirname "$(dirname "$_vc_bin")")"  # .../Microsoft Visual Studio 9.0
  _vc_dir="$_vc_root/VC"
  _ide_dir="$_vc_root/Common7/IDE"

  echo "[setup] Copying portable MSVC toolchain from Wine prefix → $MSVC_DIR ..."
  mkdir -p "$MSVC_DIR"

  # Copy VC directory (bin/, include/, lib/, atlmfc/, crt/ etc.)
  cp -r "$_vc_dir" "$MSVC_DIR/VC"

  # Copy PDB DLLs from Common7/IDE into VC/bin so cl.exe finds them
  for f in mspdb80.dll mspdbcore.dll mspdbsrv.exe msobj80.dll; do
    [[ -f "$_ide_dir/$f" ]] && cp "$_ide_dir/$f" "$MSVC_DIR/VC/bin/"
  done

  echo "[setup] Base VS2008 toolchain extracted."
else
  echo "[setup] MSVC toolchain already present, skipping VS2008 installation."
fi

# ---------------------------------------------------------------------------
# 2. Apply SP1 via Wine msiexec, then re-copy updated binaries
# ---------------------------------------------------------------------------
SP1_STAMP="$MSVC_DIR/.sp1-applied"
if [[ ! -f "$SP1_STAMP" ]]; then
  echo "[setup] Extracting VS2008 SP1 ISO ..."
  mkdir -p "$WORK_DIR/sp1"
  7z x "$VS2008_SP1_ISO" -tUDF -o"$WORK_DIR/sp1" -y > /dev/null

  # The main VS90 SP1 patch for x86
  _sp1_msp=""
  for candidate in \
      "$WORK_DIR/sp1/vs90sp1/VS90sp1-KB945140-X86-ENU.msp" \
      "$WORK_DIR/sp1/VS90sp1-KB945140-X86-ENU.msp"; do
    [[ -f "$candidate" ]] && _sp1_msp="$candidate" && break
  done

  if [[ -z "$_sp1_msp" ]]; then
    echo "[setup] WARNING: VS90 SP1 MSP not found in SP1 ISO — skipping SP1."
    touch "$SP1_STAMP"
  else
    echo "[setup] Applying VS2008 SP1 (wine msiexec /p ...) ..."
    wine msiexec /p "$(winepath -w "$_sp1_msp")" /qn || {
      echo "[setup] WARNING: SP1 msiexec exited non-zero."
    }
    wineserver --wait 2>/dev/null || true
    sleep 5

    # Re-copy updated VC/bin binaries from Wine prefix
    _vc_bin=""
    for candidate in \
        "$WINEPREFIX/drive_c/Program Files/Microsoft Visual Studio 9.0/VC/bin" \
        "$WINEPREFIX/drive_c/Program Files (x86)/Microsoft Visual Studio 9.0/VC/bin"; do
      [[ -d "$candidate" && -f "$candidate/cl.exe" ]] && _vc_bin="$candidate" && break
    done

    if [[ -n "$_vc_bin" ]]; then
      echo "[setup] Overlaying SP1 binaries ..."
      _vc_root="$(dirname "$(dirname "$_vc_bin")")"
      cp -r "$_vc_root/VC/bin/." "$MSVC_DIR/VC/bin/"
      cp -r "$_vc_root/VC/include/." "$MSVC_DIR/VC/include/" 2>/dev/null || true
      cp -r "$_vc_root/VC/lib/." "$MSVC_DIR/VC/lib/" 2>/dev/null || true
      # PDB DLLs
      _ide_dir="$_vc_root/Common7/IDE"
      for f in mspdb80.dll mspdbcore.dll mspdbsrv.exe msobj80.dll; do
        [[ -f "$_ide_dir/$f" ]] && cp "$_ide_dir/$f" "$MSVC_DIR/VC/bin/"
      done
      echo "[setup] SP1 applied."
    else
      echo "[setup] WARNING: cl.exe not found after SP1 — SP1 may not have applied."
    fi

    touch "$SP1_STAMP"
  fi
else
  echo "[setup] SP1 already applied, skipping."
fi

# ---------------------------------------------------------------------------
# 3. Extract DirectX SDK June 2010
# ---------------------------------------------------------------------------
if [[ ! -d "$DXSDK_DIR/Include" ]]; then
  echo "[setup] Extracting DirectX SDK → $DXSDK_DIR ..."
  mkdir -p "$WORK_DIR/dxsdk"
  7z x "$DXSDK_EXE" -o"$WORK_DIR/dxsdk" -y > /dev/null

  mkdir -p "$DXSDK_DIR"
  # The SDK extracts with a DXSDK/ prefix
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
# 5. Write vcvars32.sh
# ---------------------------------------------------------------------------
VCVARS="$MSVC_DIR/vcvars32.sh"
# Paths are baked in at generation time (the toolchain is fixed-location).
# winepath -w converts Linux absolute paths to Wine Z:\ paths at source time.
cat > "$VCVARS" <<EOF
# vcvars32.sh — portable MSVC 8.0 environment for Wine
# Source this file: . $VCVARS

export WINEPREFIX="$WINEPREFIX"
export WINEARCH="$WINEARCH"
export WINEDEBUG=-all

WINE_VC_BIN="\$(winepath -w "$MSVC_DIR/VC/bin")"
WINE_VC_INC="\$(winepath -w "$MSVC_DIR/VC/include")"
WINE_VC_LIB="\$(winepath -w "$MSVC_DIR/VC/lib")"
WINE_DXSDK_INC="\$(winepath -w "$DXSDK_DIR/Include")"
WINE_DXSDK_LIB="\$(winepath -w "$DXSDK_DIR/Lib/x86")"

export WINE_CL="\$WINE_VC_BIN\\\\cl.exe"
export WINE_LIB="\$WINE_VC_BIN\\\\lib.exe"
export WINE_LINK="\$WINE_VC_BIN\\\\link.exe"
export WINE_NINJA="\$(winepath -w "$NINJA_DIR/ninja.exe")"

export WINE_INCLUDE="\$WINE_VC_INC;\$WINE_DXSDK_INC"
export WINE_LIB_PATH="\$WINE_VC_LIB;\$WINE_DXSDK_LIB"
EOF

# ---------------------------------------------------------------------------
# 6. Set INCLUDE, LIB, PATH in the Wine environment registry
#    so that cl.exe finds headers/libs when invoked via cmd.exe from ninja.exe.
# ---------------------------------------------------------------------------
ENV_STAMP="$MSVC_DIR/.wine-env-set"
if [[ ! -f "$ENV_STAMP" ]]; then
  echo "[setup] Configuring Wine environment (INCLUDE, LIB, PATH) ..."

  WINE_VC_BIN="$(winepath -w "$MSVC_DIR/VC/bin")"
  WINE_VC_INC="$(winepath -w "$MSVC_DIR/VC/include")"
  WINE_VC_LIB="$(winepath -w "$MSVC_DIR/VC/lib")"
  WINE_DXSDK_INC="$(winepath -w "$DXSDK_DIR/Include")"
  WINE_DXSDK_LIB="$(winepath -w "$DXSDK_DIR/Lib/x86")"

  WINE_INCLUDE_VAL="$WINE_VC_INC;$WINE_DXSDK_INC"
  WINE_LIB_VAL="$WINE_VC_LIB;$WINE_DXSDK_LIB"

  _REG_ENV="HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment"

  # Add VC/bin to the system PATH
  _CURRENT_PATH="$(wine reg query "$_REG_ENV" /v PATH 2>/dev/null | grep -i "REG_" | awk '{print $NF}')"
  if [[ -n "$_CURRENT_PATH" && "$_CURRENT_PATH" != *"$WINE_VC_BIN"* ]]; then
    wine reg add "$_REG_ENV" /v PATH /t REG_EXPAND_SZ /d "$WINE_VC_BIN;$_CURRENT_PATH" /f > /dev/null 2>&1
  fi

  wine reg add "$_REG_ENV" /v INCLUDE /t REG_SZ /d "$WINE_INCLUDE_VAL" /f > /dev/null 2>&1
  wine reg add "$_REG_ENV" /v LIB /t REG_SZ /d "$WINE_LIB_VAL" /f > /dev/null 2>&1

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
echo "  MSVC:   $MSVC_DIR/VC/"
echo "  DXSDK:  $DXSDK_DIR/"
echo "  Ninja:  $NINJA_DIR/ninja.exe"
echo "  Wine:   $WINEPREFIX/"
echo ""
echo "  Verify: WINEPREFIX=$WINEPREFIX wine $MSVC_DIR/VC/bin/cl.exe"
echo "  Verify: WINEPREFIX=$WINEPREFIX wine $NINJA_DIR/ninja.exe --version"
