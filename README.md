# vostok - offline

Survarium (v0.100b) playable offline, without servers or an account. This branch is
[`v0.10b`](https://github.com/srp-survarium/vostok/tree/v0.10b) - the decompiled
Vostok Engine - plus the changes below, without the binary-matching tooling
(ledger, objdiff, PDB evidence, matching docs), which lives on `v0.10b`.

## Changes on top of v0.10b

- **Offline client:** the game starts in the lobby without a login or lobby server
  and runs matches locally.
- **Map picker in the chat:** the lobby opens with the chat unfolded and a
  numbered list of maps; type a number or a map name and press Enter.
- **Loading screen on Play;** a match does not start without a weapon equipped.
- **Free camera:** F6 (or the console command `demo_record`) toggles a free-flying
  camera; the crosshair is hidden while it is on.
- **Position report:** F7 (or `where`) prints the map, camera position and direction
  to the chat and console and appends it to `where.txt`.
- **Minimap hidden** in matches.
- **Wine input:** letter keys type their Latin letter in the chat and console
  whatever the keyboard layout; Russian text renders under an English Wine locale.
- **Crash guards:** static collision objects without a cooked shape are skipped;
  stats texts are released on quit.
- **Release script:** `scripts/release/make-release.sh` assembles a shareable
  folder with `Play.bat` (Windows) and `play.sh` (Linux: private Wine prefix with
  DXVK).

## Build and play

```sh
nix develop
python3 -m vostok build
scripts/release/make-release.sh --resources-db <resources.db> --out <dir>
```

`scripts/rebase-offline.sh` rebases this branch onto `origin/v0.10b` and re-applies
the strip.
