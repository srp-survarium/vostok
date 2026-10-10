#!/usr/bin/env bash
# SPDX-License-Identifier: GPL-3.0-or-later
# Assemble a shareable, single-click offline release of the reconstructed Survarium client.
#
#   scripts/release/make-release.sh --resources-db PATH [--maps-file PATH] [--out DIR] [--exe PATH] [--zip]
#   --maps-file: the resource-porter package README (its map list is copied into MAPS.txt)
#
# Layout of the result (the engine opens ../../resources.db relative to the exe and
# requires a resources/ folder beside it):
#   <out>/Play.bat  play.sh  README.txt
#   <out>/resources.db  resources/
#   <out>/binaries/Win32/survarium-dx11-win32-gold.exe  bugtrap.dll
#   <out>/dxvk/  (32-bit DXVK; play.sh installs it into its own Wine prefix, never next to the exe)
set -euo pipefail

repo=$(cd "$(dirname "$0")/../.." && pwd)
exe="$repo/binaries/Win32/survarium-dx11-win32-gold.exe"
bugtrap="$repo/binaries/Win32/bugtrap.dll"
resources_db=""
out="$HOME/survarium-offline-$(date +%Y%m%d)"
make_zip=0
maps_file=""

while [ $# -gt 0 ]; do
	case "$1" in
		--resources-db) resources_db=$2; shift 2 ;;
		--out) out=$2; shift 2 ;;
		--exe) exe=$2; shift 2 ;;
		--zip) make_zip=1; shift ;;
		--maps-file) maps_file=$2; shift 2 ;;
		*) echo "unknown argument: $1" >&2; exit 2 ;;
	esac
done

[ -f "$exe" ] || { echo "missing exe: $exe" >&2; exit 1; }
[ -n "$resources_db" ] && [ -f "$resources_db" ] || { echo "pass --resources-db PATH (e.g. the resource-porter all-maps release)" >&2; exit 1; }
[ -e "$out" ] && { echo "output exists, refusing to overwrite: $out" >&2; exit 1; }

# 32-bit DXVK DLLs from the repo's pinned nixpkgs (same attribute as the flake's dxvk-win32).
dxvk_paths=$(nix build --no-link --print-out-paths --inputs-from "$repo" nixpkgs#pkgsCross.mingw32.dxvk_2)
dxvk=$(dirname "$(find $dxvk_paths -name d3d11.dll | head -1)")
[ -f "$dxvk/d3d11.dll" ] || { echo "could not locate 32-bit DXVK" >&2; exit 1; }

mkdir -p "$out/binaries/Win32" "$out/resources" "$out/dxvk"
cp "$exe" "$out/binaries/Win32/"
[ -f "$bugtrap" ] && cp "$bugtrap" "$out/binaries/Win32/"
cp --reflink=auto "$resources_db" "$out/resources.db"
cp "$dxvk"/d3d11.dll "$dxvk"/dxgi.dll "$dxvk"/d3d9.dll "$out/dxvk/"
chmod u+w "$out"/dxvk/*.dll
touch "$out/resources/.keep"

cat > "$out/Play.bat" <<'BAT'
@echo off
rem Survarium offline build: start the game from its own folder.
cd /d "%~dp0binaries\Win32"
start "" survarium-dx11-win32-gold.exe -no_splash_screen -max_resources_size=2048
BAT

cat > "$out/play.sh" <<'SH'
#!/usr/bin/env bash
# Survarium offline build for Linux: runs the Windows exe under Wine in a private prefix.
set -euo pipefail
dir=$(cd "$(dirname "$0")" && pwd)
command -v wine >/dev/null || { echo "Wine is required (wine 9+; staging recommended)." >&2; exit 1; }
# Always a private prefix: an inherited WINEPREFIX may belong to something else.
export WINEPREFIX="${SURVARIUM_WINEPREFIX:-$dir/wineprefix}"
export WINEDEBUG="${WINEDEBUG:--all}"
# An English locale keeps Wine's keyboard layout sane; the game converts its Russian text itself.
export LANG=en_US.UTF-8 LC_ALL=en_US.UTF-8
if [ ! -f "$WINEPREFIX/system.reg" ]; then
	echo "First run: creating the Wine prefix (one minute)..."
	wineboot -i >/dev/null 2>&1 || true
	wineserver -w || true
fi
# Install DXVK into the prefix (Wine's built-in Direct3D crashes on this game).
sys="$WINEPREFIX/drive_c/windows/syswow64"; [ -d "$sys" ] || sys="$WINEPREFIX/drive_c/windows/system32"
for dll in d3d11 dxgi d3d9; do cmp -s "$dir/dxvk/$dll.dll" "$sys/$dll.dll" || cp -f "$dir/dxvk/$dll.dll" "$sys/$dll.dll"; done
export WINEDLLOVERRIDES="d3d9,d3d11,dxgi=n,b"
# Prefer Wine's Wayland driver when available: the X11 driver mis-maps letter keys
# on desktops with several keyboard layouts.
if [ -n "${WAYLAND_DISPLAY:-}" ] && [ -z "${SURVARIUM_X11:-}" ]; then unset DISPLAY; fi
cd "$dir/binaries/Win32"
exec wine survarium-dx11-win32-gold.exe -no_splash_screen -max_resources_size=2048 "$@"
SH
chmod +x "$out/play.sh"

[ -n "$maps_file" ] && cp "$maps_file" "$out/MAPS.txt"
cat > "$out/README.txt" <<TXT
Survarium (v0.100b) - offline build
====================================

A single-player build of Survarium's client, compiled from the Vostok engine
reconstruction, with extra maps ported from other Survarium versions,
S.T.A.L.K.E.R. and F.E.A.R.
No servers or account are needed.

Start
  Windows: double-click Play.bat
  Linux:   run ./play.sh   (needs Wine; first start creates ./wineprefix)

Windows needs the "DirectX End-User Runtimes (June 2010)" from Microsoft
(d3dx9_43, d3dx11_43, D3DCompiler_43, XInput1_3, XAudio2). Most PCs that have
played older games already have them.

Playing
  The game opens in the lobby. Press Play to start on the default map, or open
  the chat, type 0 and press Enter to see the numbered map list, then type the
  map's number (or its name) and press Enter.
  Equip a weapon before starting a match.

The maps in this package and their chat names are listed in MAPS.txt.

Files
  binaries/Win32/survarium-dx11-win32-gold.exe   the game
  binaries/Win32/bugtrap.dll                     crash reporter (optional)
  resources.db                                   game data + ported maps
  resources/                                     must exist (empty)
  dxvk/                                          DXVK 2.7.1 (zlib licence), used by play.sh only

Settings and crash reports go to Documents/survarium (on Linux, Wine maps this to
your home folder's Documents).
TXT

( cd "$out" && sha256sum binaries/Win32/* resources.db dxvk/* Play.bat play.sh > SHA256SUMS )
echo "release: $out ($(du -sh "$out" | cut -f1))"

if [ "$make_zip" = 1 ]; then
	zipfile="$out.zip"
	( cd "$(dirname "$out")" && nix shell --inputs-from "$repo" nixpkgs#zip -c zip -r -1 -q "$zipfile" "$(basename "$out")" )
	echo "zip: $zipfile ($(du -sh "$zipfile" | cut -f1))"
fi
