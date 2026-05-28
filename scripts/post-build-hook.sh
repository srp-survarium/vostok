#!/usr/bin/env bash
# Linux equivalent of scripts/vs/post-build-hook.bat.
# Run after a Wine+Ninja build to regenerate COFF base files and xray-structure stubs.
#
# Env vars (set automatically by flake.nix devShell):
#   ROOT_DIR — parent of vostok/, vostok-delinker/

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
: "${ROOT_DIR:="$(cd "$SCRIPT_DIR/../.." && pwd)"}"
: "${VOSTOK_DIR:="$ROOT_DIR/vostok"}"
: "${XRAY_STUB_DIR:="$ROOT_DIR/xray-structure"}"
: "${VOSTOK_DELINKER_DIR:="$ROOT_DIR/vostok-delinker"}"

ENGINE_DIR="$VOSTOK_DIR/sources/vostok"
PDB_FILE="$VOSTOK_DIR/binaries/Win32/survarium-dx11-win32-gold.pdb"
EXE_FILE="$VOSTOK_DIR/binaries/Win32/survarium-dx11-win32-gold.exe"
PDB_PARSER_DIR="$VOSTOK_DIR/scripts/pdb-parser"

if [[ ! -f "$EXE_FILE" ]]; then
  echo "[post-build] ERROR: $EXE_FILE not found. Build the project first."
  echo "  Build with: wine binaries/toolchain/ninja/ninja.exe -C \"\$(winepath -w binaries/ninja)\" -j8"
  exit 1
fi

echo "[post-build] Regenerating COFF base object files ..."
bash "$VOSTOK_DELINKER_DIR/build_base.sh"

echo "[post-build] Regenerating xray-structure stubs ..."
cargo run --manifest-path "$PDB_PARSER_DIR/Cargo.toml" --release -- \
  --output-path "$XRAY_STUB_DIR" \
  --pdb-path    "$PDB_FILE" \
  --engine-path "$ENGINE_DIR" \
  --as-base \
  --skip-non-engine-headers

echo "[post-build] Done."
