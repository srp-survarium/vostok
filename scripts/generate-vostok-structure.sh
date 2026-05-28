#!/usr/bin/env bash
# Linux equivalent of generate-vostok-structure.bat.
# Runs pdb-parser on the original survarium.pdb → vostok-structure/ stubs.
#
# Env vars (set automatically by flake.nix devShell):
#   SURVARIUM_BIN — directory containing survarium.pdb
#   ROOT_DIR      — parent of vostok/, vostok-structure/

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
: "${ROOT_DIR:="$(cd "$SCRIPT_DIR/../.." && pwd)"}"
: "${VOSTOK_DIR:="$ROOT_DIR/vostok"}"
: "${SURVARIUM_BIN:="$VOSTOK_DIR/binaries/game"}"
: "${VOSTOK_STUB_DIR:="$ROOT_DIR/vostok-structure"}"

PDB_PARSER_DIR="$VOSTOK_DIR/scripts/pdb-parser"

if [[ ! -f "$SURVARIUM_BIN/survarium.pdb" ]]; then
  echo "[pdb-parser] ERROR: survarium.pdb not found at $SURVARIUM_BIN"
  echo "  Run: bash scripts/setup-toolchain.sh (requires result-survarium-game)"
  exit 1
fi

echo "[pdb-parser] Generating vostok-structure from survarium.pdb ..."
cargo run --manifest-path "$PDB_PARSER_DIR/Cargo.toml" --release -- \
  --output-path "$VOSTOK_STUB_DIR" \
  --pdb-path    "$SURVARIUM_BIN/survarium.pdb" \
  --engine-path "$VOSTOK_DIR/sources/vostok"

echo "[pdb-parser] Done. Stubs at: $VOSTOK_STUB_DIR"
