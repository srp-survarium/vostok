# SPDX-License-Identifier: GPL-3.0-or-later

# Offline map playtest bug ledger

Keep one entry per observed failure. Record the map, weapon, exact action, EXE
hash, result, and raw log before another run overwrites `survarium_sheep.log`.
An absence of a crash dump or a useful stack trace is part of the record; it
does not identify a cause. The archived artifacts for the first sessions are in
[`offline-startup-2026-10-09/`](offline-startup-2026-10-09/).

## Test setup

- Wine prefix: `/tmp/vostok-start-map/play/prefix`; game directory:
  `/tmp/vostok-start-map/play/game`; working directory:
  `/tmp/vostok-start-map/play/binaries/Win32`.
- Isolated user config: `/tmp/vostok-start-map/play/documents/survarium/user.cfg`.
- Resources: the unpacked v0.100b assets linked under `play/resources`.
- All runtime failures below used the **earlier** play-copy EXE, SHA-256
  `8f9452a35fc5af6b44f335f20166f5fba51ace65ef911ef0238c91626ba2e467`.
  The later full-build EXE, SHA-256
  `bc8306f6b765179b25be9e5f344a3456f1ecbf88891272613c433ad23e16956b`,
  includes the offline `m_local_player` assignment. It was subsequently tested
  with the TT-33: shooting did not crash, but the camera jumped vertically
  (OS-005). A Vityaz run on this build also crashed before any shot (OS-006).
  A Remington 870 run on this build crashed on a later shot (OS-007).
  The other weapons have not yet been retested on this build.
- All archived Wine stderr files contain the same `libEGL`/DRI2 graphics
  warnings. They do not themselves report the game crash. The game logs also
  report missing textures and failed resource cooks; their relationship to the
  crashes is unknown.

## OS-001: Remington 870 run crashed on the rainy map

**Setup:** `level_03_rain`, `rem_870` in the alternate-map run. **Reported:**
The user said it crashed. The game error report at 2026-10-08 23:52:18
records `ACCESS_VIOLATION` and two unsymbolized EXE addresses, `0x000cb7ad`
and `0x001916a4`. It does not establish what player action triggered the
failure. **Evidence:**
[`game error log`](offline-startup-2026-10-09/rain-rem870-crash.log),
[`minidump`](offline-startup-2026-10-09/rain-rem870-crash.dmp),
[`Wine stderr`](offline-startup-2026-10-09/alt-map.stderr).
The minidump SHA-256 is
`4bfb95b4f01f31ebd9e07f21a08eb94cdebf74df252357ca2756e7a0bed8c7c0`.
**Status:** Unresolved.

## OS-002: Remington 870 crashed when fired on the default map

**Setup:** `level_03_evn`, `rem_870`. **Reported:** The user said it crashed
when they shot, then clarified that the weapon itself loaded successfully.
The run's `evening-rem.stdout` was empty; its surviving
[`Wine stderr`](offline-startup-2026-10-09/evening-rem.stderr) contains only
graphics warnings. The in-game log was overwritten by the next run and no
dedicated dump was retained. **Status:** Unresolved; user report is the only
evidence tying the failure to firing.

## OS-003: Vityaz crashed on reload

**Setup:** `level_03_evn`, `vityaz`. **Reported:** The user said the game
crashed when reloading. The game error log at 2026-10-09 00:01:05 ends with
`8: (null)` and `cannot load dbghelp library`; it contains no useful stack
trace or dump. The log does not itself confirm the reload action.
**Evidence:**
[`game error log`](offline-startup-2026-10-09/vityaz-reload-crash.log),
[`Wine stderr`](offline-startup-2026-10-09/vityaz.stderr).
**Status:** Unresolved.

## OS-004: TT-33 crashed when fired

**Setup:** `level_03_evn`, `tt_33`. **Reported:** The user said the game
crashed when shooting. At 2026-10-09 00:06:11, the game log records repeated
`ACCESS_VIOLATION` messages after a `cannot load dbghelp library` message.
No new error-report file or dump was produced. **Evidence:**
[`game log`](offline-startup-2026-10-09/tt33-shooting-crash.log),
[`Wine stderr`](offline-startup-2026-10-09/tt33.stderr).
**Status:** Not reproduced while shooting with the later full-build EXE;
camera jumping was observed instead (OS-005). More runs are needed before
calling the firing crash fixed.

## OS-005: Camera jumps vertically during TT-33 play

**Setup:** `level_03_evn`, `tt_33`, full-build EXE SHA-256
`bc8306f6b765179b25be9e5f344a3456f1ecbf88891272613c433ad23e16956b`.
**Reported:** Shooting no longer crashed, but the camera position changed
unpredictably along the Y axis, jumping up and down during play. There is no
coordinate trace or crash in the game log, so the size, timing and trigger of
each jump are not captured. **Evidence:**
[`game log`](offline-startup-2026-10-09/final-tt33-camera-jump.log),
[`Wine stderr`](offline-startup-2026-10-09/final-tt33-camera-jump.stderr),
and the player's observation. **Status:** Open.

## OS-006: Vityaz crashed before shooting on the final build

**Setup:** `level_03_evn`, `vityaz`, full-build EXE SHA-256
`bc8306f6b765179b25be9e5f344a3456f1ecbf88891272613c433ad23e16956b`.
**Reported:** The game crashed without the player firing. No other triggering
action was reported. The game error report at 2026-10-09 00:12:08 ends with
`8: (null)` and `cannot load dbghelp library`, the same ending as the earlier
Vityaz report (OS-003); no dump or useful stack trace was produced.
**Evidence:**
[`game error log`](offline-startup-2026-10-09/final-vityaz-spontaneous-crash.log),
[`Wine stderr`](offline-startup-2026-10-09/final-vityaz-spontaneous-crash.stderr),
and the player's observation. **Status:** Open; repeat occurrence of the
Vityaz crash, now without shooting.

## OS-007: Remington 870 crashed on a later shot on the final build

**Setup:** `level_03_evn`, `rem_870`, full-build EXE SHA-256
`bc8306f6b765179b25be9e5f344a3456f1ecbf88891272613c433ad23e16956b`.
**Reported:** The first shot worked; the game crashed on a later shot that
appeared unpredictable to the player. The shot count was not captured. The
game error report at 2026-10-09 00:13:53 ends with `cannot load dbghelp
library` and gives no useful stack trace or dump. This is a separate run from
the earlier Remington 870 firing report (OS-002).
**Evidence:**
[`game error log`](offline-startup-2026-10-09/final-rem870-later-shot-crash.log),
[`Wine stderr`](offline-startup-2026-10-09/final-rem870-later-shot-crash.stderr),
and the player's observation. The repeat run with the same map, weapon and
EXE also crashed on a later, apparently unpredictable shot, not the first.
Its error report at
2026-10-09 00:15:50 again ended with `cannot load dbghelp library`, without a
useful stack trace or dump. **Repeat evidence:**
[`game error log`](offline-startup-2026-10-09/final-rem870-repeat-later-shot-crash.log),
[`Wine stderr`](offline-startup-2026-10-09/final-rem870-repeat-later-shot-crash.stderr).
**Status:** Reproduced twice on the final build; exact triggering shot unknown.

## OS-008: `level_03` crashed immediately after loading

**Setup:** `level_03`, `rem_870`, full-build EXE SHA-256
`bc8306f6b765179b25be9e5f344a3456f1ecbf88891272613c433ad23e16956b`.
**Reported:** The game crashed immediately; no shot was fired. The error
report at 2026-10-09 00:16:54 ends with `8: (null)` and `cannot load dbghelp
library`. No dump or useful stack trace was produced, so the log does not
identify whether the map or weapon caused the failure. **Evidence:**
[`game error log`](offline-startup-2026-10-09/final-level03-rem870-startup-crash.log),
[`Wine stderr`](offline-startup-2026-10-09/final-level03-rem870-startup-crash.stderr),
and the player's observation. **Status:** Open; not reproduced on the same
setup's repeat run, which loaded and later crashed during play (OS-009).

## OS-009: `level_03` crashed during play with Remington 870

**Setup:** `level_03`, `rem_870`, full-build EXE SHA-256
`bc8306f6b765179b25be9e5f344a3456f1ecbf88891272613c433ad23e16956b`.
**Reported:** The map loaded, then the game crashed while the player was
shooting. The player could not tell whether a shot triggered it. The error
report at 2026-10-09 00:18:27 ends with `cannot load dbghelp library` and
contains no useful stack trace or dump. **Evidence:**
[`game error log`](offline-startup-2026-10-09/final-level03-rem870-play-crash.log),
[`Wine stderr`](offline-startup-2026-10-09/final-level03-rem870-play-crash.stderr),
and the player's observation. **Status:** Open; trigger unknown.

## OS-010: `level_03` crashed during play with default AK-74U

**Setup:** `level_03`, `ak_74u`, full-build EXE SHA-256
`bc8306f6b765179b25be9e5f344a3456f1ecbf88891272613c433ad23e16956b`.
**Reported:** The map loaded, then the game crashed while the player was
shooting. The log does not establish whether a shot triggered the crash. The
error report at 2026-10-09 00:20:59 ends with `cannot load dbghelp library`;
there is no useful stack trace or dump. **Evidence:**
[`game error log`](offline-startup-2026-10-09/final-level03-ak74u-play-crash.log),
[`Wine stderr`](offline-startup-2026-10-09/final-level03-ak74u-play-crash.stderr),
and the player's observation. **Status:** Open.

## OS-011: Ground lighting appears broken on the rain map

**Setup:** `level_03_rain`, `ak_74u`, full-build EXE SHA-256
`bc8306f6b765179b25be9e5f344a3456f1ecbf88891272613c433ad23e16956b`.
**Reported:** The player described the ground lighting as broken with AK-74U,
then observed strange ground flickering in a later Vityaz run. The flicker was
not necessarily ugly, but looked wrong. No visual capture was taken. The game
logs report missing textures and resource cooks, but do not establish that
these caused the visible problem. **Evidence:** the player's observations and
[`AK-74U game log`](offline-startup-2026-10-09/final-rain-ak74u-shoot-reload-crash.log),
[`Vityaz game log`](offline-startup-2026-10-09/final-rain-vityaz-visual.log).
**Status:** Open; appearance and trigger need a visual record.

## OS-012: Rain-map crash while shooting and pressing reload

**Setup:** `level_03_rain`, `ak_74u`, full-build EXE SHA-256
`bc8306f6b765179b25be9e5f344a3456f1ecbf88891272613c433ad23e16956b`.
**Reported:** The game crashed while the player was shooting and pressed
reload at the same time. The player could not tell whether either input caused
the crash. The error report at 2026-10-09 00:22:59 ends with `cannot load
dbghelp library`; there is no useful stack trace or dump. **Evidence:**
[`game error log`](offline-startup-2026-10-09/final-rain-ak74u-shoot-reload-crash.log),
[`Wine stderr`](offline-startup-2026-10-09/final-rain-ak74u-shoot-reload-crash.stderr),
and the player's observation. **Status:** Open; trigger unknown.

## OS-013: Water effect on weapons and hands looks overly glossy

**Setup:** `level_03_rain`, `vityaz`, full-build EXE SHA-256
`bc8306f6b765179b25be9e5f344a3456f1ecbf88891272613c433ad23e16956b`.
**Reported:** Outdoors, water on the weapon and hands looked almost glossy and
visually wrong. The effect disappeared from both hands and weapon inside
buildings. No visual capture was taken. The game log cannot verify the
appearance or establish its cause. **Evidence:** the player's observation,
[`game log`](offline-startup-2026-10-09/final-rain-vityaz-visual.log), and
[`Wine stderr`](offline-startup-2026-10-09/final-rain-vityaz-visual.stderr).
**Status:** Open. This Vityaz run did not crash during the observed playtime;
that does not resolve the earlier Vityaz crashes on `level_03_evn`.

## Development build failure (resolved)

The first worktree `--code-only` build exited 1 because `offline_client.cpp`
used the unqualified `match_options` type at line 117 (`C2065`/`C3867`, followed
by errors using `options`). The full generated log is
`binaries/build-jobs/vostok-build-9a48cf00f4f34b20a2c1588d0b22a80b.log`.
The source was corrected; the next `--code-only` build and final full build
both exited 0. This compiler failure is closed and unrelated to the runtime
crashes above.

## OS-014: Latest-build AK-74U crash while shooting on the default map

**Setup:** `level_03_evn`, `ak_74u`, offline local player, Wine prefix
`/tmp/vostok-start-map/play/prefix`. The full build from commit `84e25625f`
plus the worktree's offline-startup changes succeeded as job
`vostok-build-252da27465df456bb1e4f88fa03f7c6f`. The EXE SHA-256 is
`b23486236c349d60ac45874187ce3a91c64fff13c0233668c2a0d45fd0948c22`.

**Reported:** The player said the game crashed while shooting. The game wrote
an error report at 2026-10-09 15:58:31 CEST, and the Wine command exited 255.
The report ends with `cannot load dbghelp library`; it contains no useful
stack trace and does not establish that firing caused the crash. No new dump
was found. The game log also lists missing textures and failed resource cooks,
but does not connect them to the crash. This is a new-build observation of a
shooting-time crash, distinct from earlier runs with other EXE hashes.

**Evidence:** [`game log`](offline-startup-2026-10-09/latest-build-default-ak74u-shooting-crash.log),
[`error report`](offline-startup-2026-10-09/latest-build-default-ak74u-shooting-error-report.log)
(byte-identical to the game log),
[`startup config`](offline-startup-2026-10-09/latest-build-default-ak74u-user.cfg),
and the player's observation. Wine stderr was not captured to a file for this
run. **Status:** Open; the cause is unknown.

**Memory/texture check (2026-10-09):** This EXE is 32-bit and marked
large-address-aware, running in a 64-bit Wine prefix. Its startup log reserves
3,652,321,276 bytes (3.40 GiB) across engine arenas, including 1.80 GiB for
managed resources, 0.90 GiB for unmanaged resources, and 0.25 GiB for render.
The logged 108,600 bytes used is from startup, not the crash. There is no
out-of-memory message or memory snapshot at the crash, so exhaustion is not
ruled out. The logged `vehicles_spec.dds` and `metal_column_nmap.dds` do not
exist in the unpacked v0.100b asset set. The logged
`electrical_cabinet_02_spec.dds` does exist, but its requested path contains
an extra space after `electric/`. Those texture failures have direct path
explanations and do not, by themselves, indicate insufficient memory. The
RTX 3060 Ti has 8 GiB VRAM; `nvidia-smi` showed 5.4 GiB free at the time of
this later check, not during the crash.

## OS-015: Sound-fix build crashes around opening the console

**Setup:** `level_03_evn`, `ak_74u`, offline local player, Wine prefix
`/tmp/vostok-start-map/play/prefix`. Full build
`vostok-build-546ab50510fe45318290fee51af1c6a8` used commit `9225e3f34`,
the worktree's offline-startup changes, and the walking-sound change in
`weapon.cpp`. EXE SHA-256:
`0f4b2220728e084ee2a6ae5d2e2bc85a9579331e4956b734ae47280917aac428`.

**Observed:** The player confirmed walking sound worked. On the first run the
player opened the console and the game crashed; they considered that the
likely trigger. Wine exited 255 and the game wrote an error report at
2026-10-09 17:08:58 CEST. A same-build rerun also crashed, with an error
report at 17:10:14 CEST and Wine exit 255. The player subsequently confirmed that both crashes followed pressing
the tilde key to open the console. Both game reports end
with `cannot load dbghelp library`, without a useful stack trace or new dump.
The second run's Wine SEH trace records an access violation at `0x7a6e2388`
while reading address `0x000000b4`; that identifies a fault, not its cause.

**Memory:** GNU `time -v` measured peak resident sets of 124,548 KiB and
125,176 KiB for the two Wine commands. These are not total game memory,
virtual-address usage, or GPU memory; they cannot establish or exclude
memory pressure in another Wine process. Neither game report says
out-of-memory.

**Evidence:** First run:
[`game log`](offline-startup-2026-10-09/soundfix-console-crash.log),
[`error report`](offline-startup-2026-10-09/soundfix-console-error-report.log),
[`memory measurement`](offline-startup-2026-10-09/soundfix-console-memory.txt),
[`startup config`](offline-startup-2026-10-09/soundfix-console-user.cfg).
Rerun:
[`game log`](offline-startup-2026-10-09/soundfix-console-repeat.log),
[`error report`](offline-startup-2026-10-09/soundfix-console-repeat-error-report.log),
[`Wine stderr/SEH trace`](offline-startup-2026-10-09/soundfix-console-repeat.stderr),
[`memory measurement`](offline-startup-2026-10-09/soundfix-console-repeat-memory.txt).
**Status:** Reproduced twice after pressing tilde, as confirmed by the player.
The failing code path and root cause remain unproved.

**Retail comparison:** The shipped v0.100b `survarium.exe` (SHA-256
`6cde21fac0fc508c140814c83d8a9d0158c0bff8f99c322cd7454c3d3103f737`)
was started with the same asset set in a separate Wine prefix. The player
reported that `~` opened the console in the retail login menu without a
crash. No working server connection was available in that run, so it did not
provide an in-map retail comparison.
This supports a difference between the tested contexts, but does not identify
the failing code path. Evidence: [`retail game log snapshot`](offline-startup-2026-10-09/retail-menu-console-open.log),
[`retail Wine stderr snapshot`](offline-startup-2026-10-09/retail-menu-console-open.stderr),
and the player's observation.

## OS-016: Retail crash during attempted lobby entry with local SRP

**Setup:** Shipped v0.100b `survarium.exe` SHA-256
`6cde21fac0fc508c140814c83d8a9d0158c0bff8f99c322cd7454c3d3103f737`,
isolated prefix `/tmp/vostok-start-map/retail-play/prefix`, launched with
`-no_splash_screen -client=127.0.0.1:1234`. SRP login, lobby, match and
browser services were listening locally. This first run used the unpacked
resource tree, which omitted `resources/ssl` and therefore the client trust
certificates.

**Reported:** The player thought the game crashed while entering the lobby.
At 2026-10-09 18:56:56 CEST the game log reports `[LOGIN] error during
handshaking: certificate verify failed` and `game: SSL certificate verification
failed`. Lobby-related UI fields appeared in the log just before the 18:57:02
crash report. Wine's SEH trace shows an access violation at `0x7a60b750`
while writing address `0x00000b84`. Wine exited 255. The generated dump is
zero bytes, so it provides no stack. The certificate failure is a concrete
test-setup defect; the evidence does not prove it caused the later crash.

**Evidence:** [`game log`](offline-startup-2026-10-09/retail-local-server-lobby-crash.log),
[`error report`](offline-startup-2026-10-09/retail-local-server-lobby-error-report.log),
[`Wine stderr/SEH trace`](offline-startup-2026-10-09/retail-local-server-lobby.stderr),
[`memory measurement`](offline-startup-2026-10-09/retail-local-server-lobby.memory.txt),
[`empty dump`](offline-startup-2026-10-09/retail-local-server-lobby-empty.dmp),
and the player's observation. **Status:** Open. A new run with the pinned
v0.100b login/lobby certificates linked into the isolated resource tree is
underway.

## OS-017: Offline lobby startup crashes in Wine's Direct3D renderer

**Setup:** Code-only build `vostok-build-823eedc456fc4e9dba601665fad647f4`
from the experimental lobby worktree, EXE SHA-256
`b7c9709c467894f36b604e1d61f3f88278b27d4381277c18fabe9513c58964b4`.
The game used the unpacked v0.100b assets and isolated Wine prefix
`/tmp/vostok-start-map/play/prefix`, with the offline lobby and local
level-100 profile enabled. No server was involved.

**Observed:** Two launches on 2026-10-09 at 19:47 and 19:48 CEST exited
during lobby startup, before the inventory or map selection could be tested.
Both Wine SEH traces report an access violation at `0x7a60b750`, writing to
`0x00000b84`. Wine's module-load trace places that address in its built-in
`wined3d.dll` loaded at `0x7a530000`. The game wrote an error report and an
empty dump on each launch. The game log records UI font glyph warnings and a
missing `grass_l01.dds` texture shortly before the crash, but neither is a
proved cause.

**Comparison:** OS-016's retail lobby run faulted at the same instruction and
null-object offset. This supports a shared Wine rendering problem rather than
an offline-profile-specific failure, but the caller and root cause remain
unknown. A Wine debugger attempt failed before the game started with
`server_init_process_done: Assertion '!status' failed`; it supplied no stack.

**Evidence:** First launch:
[`error report`](offline-startup-2026-10-09/offline-lobby-first-crash.log),
[`Wine SEH trace`](offline-startup-2026-10-09/offline-lobby-first-crash.stderr).
Repeat with module loading enabled:
[`error report`](offline-startup-2026-10-09/offline-lobby-repeat-crash.log),
[`Wine module and SEH trace`](offline-startup-2026-10-09/offline-lobby-repeat-crash.stderr),
[`startup config`](offline-startup-2026-10-09/offline-lobby-user.cfg).
**Workaround tested:** Replacing the isolated play copy's `d3d11.dll` and
`dxgi.dll` with DXVK 2.7.1 initially failed because the engine reserved
roughly 2.7 GiB for resources in the 32-bit address space; DXVK reported
`VK_ERROR_OUT_OF_HOST_MEMORY` while mapping 16 MiB Vulkan allocations.
Starting the same EXE with its existing `-max_resources_size=2048` option
reduced that reservation. With DXVK's D3D11/DXGI DLLs, the offline lobby
rendered and the default character, weapon, and Play button were visible.
This does not identify the original `wined3d.dll` crash's cause. DXVK's
uncapped [`Wine trace`](offline-startup-2026-10-09/offline-lobby-dxvk-uncapped.stderr)
records the allocation failure; the capped run's
[`Wine trace`](offline-startup-2026-10-09/offline-lobby-dxvk-2g.stderr),
[`D3D11 log`](offline-startup-2026-10-09/offline-lobby-dxvk-2g-d3d11.log),
and [`DXGI log`](offline-startup-2026-10-09/offline-lobby-dxvk-2g-dxgi.log)
record the working renderer setup. **Status:** Original renderer crash remains
open; the lobby is accessible with this runtime workaround. Match and
inventory behavior still need interactive verification.

## OS-018: Code-only offline match later exited with an access violation

**Setup:** `level_03_evn`, default AK74U, code-only EXE SHA-256
`b7c9709c467894f36b604e1d61f3f88278b27d4381277c18fabe9513c58964b4`.
DXVK 2.7.1 provided D3D11/DXGI, with `-max_resources_size=2048`. The lobby
rendered, Play loaded the map, and the weapon was visible. Mouse clicks were
used to inspect the inventory and start the match; one click in the match
fired a shot. Automated keyboard input did not reach the game reliably.

**Observed:** At 2026-10-09 20:14:18 CEST, approximately 19 minutes after
launch, the game exited with `ACCESS_VIOLATION`. Its report has four
unsymbolized EXE addresses (`0xf4141513`, `0x478b0689`, `0x00750703`,
`0x0008a36d`), which do not identify the failing function. The last UI log
entries concern settings-window font glyphs. The precise player action at
crash time is unknown; this is not evidence that firing or keyboard input
caused it. The dump is 133 KiB.

**Evidence:** [`error report`](offline-startup-2026-10-09/offline-codeonly-late-crash.log),
[`minidump`](offline-startup-2026-10-09/offline-codeonly-late-crash.dmp),
[`Wine stderr`](offline-startup-2026-10-09/offline-codeonly-late-crash.stderr).
Dump SHA-256: `cb9133aaacdde46ba9578501d6686c17a018c909a68f6f0440955695bdbff82b`.
**Status:** Open; trigger unknown.

## OS-019: Full-build offline lobby crashed during startup

**Setup:** Full build `vostok-build-5fd4d43b2b7b4110b0a99ce1b46f1ab0`, EXE
SHA-256 `6e323d1c7bf07c85b10804c854697879a4c7a6686d614a1588ee56a088883eb6`.
The same isolated Wine prefix, bundled v0.100b resources, DXVK 2.7.1
D3D11/DXGI, and `-max_resources_size=2048` were used. No server was running.

**Observed:** On 2026-10-09 at 20:18:50 CEST, the game exited 255 before a
lobby screenshot or input. The report records `ACCESS_VIOLATION` and four
unsymbolized EXE addresses (`0xffffff68`, `0x0021eae0`, `0x00000004`,
`0x00403c74`). Wine logged a failed 2.7 GiB virtual allocation but continued
through Vulkan and UI initialization; the log does not show that allocation
caused the later crash. The last game-log entries include UI font glyph
warnings and a missing `grass_l01.dds` texture. Their relationship to the
fault is unknown. The dump is 211 KiB.

**Evidence:** [`error report`](offline-startup-2026-10-09/offline-full-lobby-crash.log),
[`minidump`](offline-startup-2026-10-09/offline-full-lobby-crash.dmp),
[`Wine service trace`](offline-startup-2026-10-09/offline-full-lobby-crash.stderr).
Dump SHA-256: `ef884d09e86a530e21a9d8ea3939caf0a495693bf35698ac21fc9f03818039a9`.
**Status:** Open; the 20:22 retry reached the offline lobby with the same EXE
and setup, so this startup crash is intermittent.

## OS-020: Opening the in-game menu crashed in `scene_view::remove_movie`

**Setup:** `level_03_evn`, default weapon, code-only build with the lobby
camera and texture-streaming fixes, EXE SHA-256 `69080278f41e4ebf04aa257c29266eb9719e6146cfd004931b17954f4e3360cd`. Run directory
`/tmp/vostok-start-map/play/playtest-20261009-211901` keeps the EXE, its PDB, the dump and the Wine SEH trace.

**Observed:** Shooting did not crash. Pressing Escape to leave the map crashed
at 21:33:00 with a write access violation at `eip=0x0008a36d` (address
`0xE3`), the same EXE address as OS-018.

**Cause (symbolized):** image base 0x10000, RVA 0x7a36d is
`vector<resource_ptr<…>>::_M_erase` (ICF-shared with
`flash_movie_resource`), called from `render::engine::world::hide_movie` on
the render command queue. `scene_view::remove_movie` erases `find()` without
checking it (retail-identical), so it erased `end()`. `game::activate_main_menu`
→ `game_world::show_ui(false)` hides the chat when `chat_handler::is_active()`.
The offline lobby leaves the chat active, but `game_world::show_ui(true)` only
adds it to the match view when `has_bandwidth()`, which the offline client
reports false. **Fix:** offline `game_world::show_ui(true)` always shows the
chat in the match view, pairing every hide with a show. **Status:** verified; the
next playtest opened and closed the menu and left the map without this crash.

## OS-021: Quitting from a match crashed in `~network_client`

**Setup:** `level_03_evn`, build with the OS-020 chat fix and the bullet
orientation fix, EXE SHA-256 `58467e62022e145fccba3058a775c44306ed9623fc2291de7944e41643612e2a`; run directory `/tmp/vostok-start-map/play/playtest-20261009-214707`.

**Observed:** The player left the game from the match; they did not notice a
crash. Wine exit 255 and an error report at 21:56:08: write access violation at
`eip=0x001b4dd0` (address `0x4`, `ecx=0`).

**Cause (symbolized):** the stack runs `game::clear_resources` → offline client
scalar-deleting destructor → `network_client::~network_client` +0x29 →
`Scaleform::RefCountNTSImpl::Release` (prebuilt, unsymbolized). The retail-identical
destructor destroys the four `draw_stats` texts (`m_unacknowledged_packets_*`,
`m_max_local_sequence_difference_*`) whenever the game world still has a text
manager. Only `draw_stats` creates them, so quitting straight from a match
without it releases a null `DrawText`. **Fix:** `~offline_client` creates hidden
texts for any of the four that were never created, so the shared destructor
releases real objects. **Status:** fix built; the next run exited cleanly (exit 0,
no error report), but whether it quit from inside a match was not recorded.

## OS-022: Starting an offline match without a weapon crashed in `player::insert`

**Setup:** `level_03_evn`, both weapon slots emptied in the lobby, build with
the offline loading screen and OS-021 fix, EXE SHA-256 `7d828d6f26330eb5f4a640ffc7fc97f5bde798c80fce5d9abec746c353b7d7fc`; run directory
`/tmp/vostok-start-map/play/playtest-20261009-220954`.

**Observed:** Pressing Play with no weapon equipped crashed at 22:11:15 while
the map finished loading: read access violation at `eip=0x005d3761`.

**Cause (symbolized):** `player::insert` +0x1d1, called from
`offline_client::on_player_loaded`. With both weapon slots empty, `insert`
falls back to `m_empty_hands` and calls `activate` on it, but `player_cook`
(retail-identical) only loads empty hands for demo players, so a real player
gets NULL. Online, Play is only a request (`set_status_ready_for_match`); the
client never blocks a weaponless start, so the original server presumably refused it. SRP and
the offline glue start unconditionally. **Fix:** `offline_client::start_match`
refuses to start without an item in `weapon1_slot` or `weapon2_slot` and says
so in chat. **Status:** fix built; awaiting playtest.

## OS-023: Typing in the offline chat triggered lobby key bindings

**Reported:** After clicking into the chat and typing (for example a map name),
keys still acted as game bindings and opened menus, so map selection through
chat was unusable.

**Cause:** Flash reports `chat_enter_start`, and `chat_handler::callback` calls
`focus( true )`, which registers the chat as an input handler. Its
`on_keyboard_action` consumes every key. Retail `focus` first returns when the
network client has no bandwidth (virtual call at vtable +0x14, offset 0x17), so
the chat is inert until connected. The offline client always reports no
bandwidth, so the chat never took the keyboard. **Fix:** the offline branch's
`chat_handler::focus` drops that early return. **Status:** verified; typed chat
text reached the chat (an unknown map name got the offline map-error reply).

## OS-024: The lobby chat panel stayed visible after the map loaded

**Reported:** After starting a match from the lobby, the lobby-style chat panel
remained on screen in the match.

**Cause:** `game_world::on_activate` switches the chat to in-match mode
(`chat_handler::set_mode( true )`: team/all tabs, hidden until the chat key)
only when the network client has bandwidth. The offline client never does, so
the chat stayed in lobby mode. **Fix:** the offline branch's `on_activate`
switches modes unconditionally; `set_mode` only reads the messaging client's
stored team for the tab label. **Status:** included in EXE `051058cf…` (the
map-list build compiled it); the user reported no panel in the next match.

## OS-025: Only digits could be typed into the chat under Wine

**Reported:** In Russian and default locales alike, the chat accepted digits but
not letters.

**Cause:** `swf_input_translator` registers letters with no fallback character
(`register_char_bind( key_a, 0x41, true )`), so their character comes only from
`keyboard::get_dik_unicode`, which reads `DIPROP_KEYNAME` from DirectInput
(both 100% retail). A DirectInput probe under the play prefix (Wine staging 11.8,
X11 or Wayland driver, any LANG) shows `GetProperty( DIPROP_KEYNAME )` fails
with `0x80070002` for DIK_A/Q/Z but returns "1" and "space" for DIK_1/SPACE;
`MapVirtualKeyW( DIK_A, MAPVK_VSC_TO_VK )` is also 0. **Fix (offline branch,
Wine workaround):** when the key-name lookup fails and the bind has no
character, `translate_key_action` uses the bind's Flash scan code, which for
letters is the uppercase Latin virtual-key code. **Status:** superseded by OS-032;
the fallback alone did not help because letters never reached the game under
Wine's X11 driver.

## OS-026: Movement blocked after starting a match with the chat focused

**Reported:** With the chat focused in the lobby, entering the match left the
player unable to move.

**Cause:** a focused `chat_handler` is an input handler that consumes every key;
only its own Enter/Escape/cancel paths release focus, never a scene change.
**Fix:** `offline_client::start_match` calls `chat_handler::focus( false )`
before loading. **Status:** fix built; awaiting playtest.

## OS-027: Intermittent startup crash in the lobby Flash movie (retail too)

**Observed:** EXE `051058cf…` exited at startup (run `playtest-20261009-225748`):
read of address `0x14`. The user reports retail crashes the same way at startup
on large resolutions.

**Symbolized** (index with `--engine-path 'c:\survarium'` to include the
Scaleform SDK sources): `Scaleform::GFx::AS3::AvmDisplayObj::OnEvent` +0xb7 ←
`AS3::EventChains::QueueEvents` +0x116 ← `AS3::MovieRoot::OnNextFrame` ←
`MovieImpl::AdvanceFrame` ← `MovieImpl::Advance` ← `lobby_menu::update_ui` +0xf6
(`m_lobby_menu_ui->movie->Advance`). A display object in the lobby movie's
enter-frame event chain is NULL. **Status:** open; resolution dependence not yet
reproduced.

## OS-028: WASD dead (Space works) under the Russian Wine locale

**Reported:** With `LANG/LC_ALL=ru_RU.UTF-8` the player could jump but not move.
**Cause:** Wine selects the Russian keyboard layout (`HKL 04190419`) for that
locale; `HKCU\Keyboard Layout\Preload` does not override it. Letter keys break
while Space/digits are layout-independent. **Fix:** run Wine with
`en_US.UTF-8` and pin the CRT code page instead:
`core_entry_point.cpp` `setlocale( LC_CTYPE, ".1251" )` (retail `""` meant 1251
on Russian Windows), so the cp1251 UI text still converts. **Status:** fix
building.

## OS-029: Weapon jumps up when leaving aim right after a shot

**Reported:** Shooting (e.g. Remington 700) and immediately leaving aim made the
weapon jump up. **Cause:** a reconstruction divergence hidden behind a 99.9%
STRUCTURE MATCH: retail `weapon_recoil_calculator::tick` jumps from the
additive-timer test (0xd5 `jnp .5`, 0xfe `jmp .5`) to the koef interpolation at
0x29c, so recoil decay and `process_compensation` run every tick. Ours nested
them inside `if ( m_additive_recoil_timer != 0.0f )`; after the 1 ms additive
timer the kick froze and the hip animation applied it on unaim. **Fix:** close
the timer block before the interpolation (also queued for the behaviour-audit
branch). **Status:** fix building. The separate "weapon disappears before
scope-in ends" report is not explained by the scope-swap code (same thresholds as
retail); next step is the `leupold` scope `change_scope_factor` at runtime.

## OS-030: Equipping a trap crashed in `booby_trap_set_core::load`

**Observed:** EXE `11f1d250…`, run `playtest-20261009-231436`: write access
violation to `0x10` inside the CRT string copy, called from `strings::copy` ←
`booby_trap_set_core::load` +0x522 ← `booby_trap_set_core_cook::on_config_ready`.

**Cause (reconstruction):** ours re-initialised the two `buffer_vector` members
by assignment (`m_traps = buffer_vector<…>( … )`,
`m_damage_parameters = buffer_vector<…>( … )`). `buffer_vector::operator=`
compiles to `assign( begin, end )`, copying zero elements into the member's own
null storage, so the member never adopts the new buffer and the following
`push_back`/`back()` write through garbage. Retail placement-constructs both
(`push this+118h; push 8; call operator new` at 0xa1-0xb0, same at this+120h
near 0x3d1). **Fix:** `new ( &m_traps ) booby_traps_type( … )` and
`new ( &m_damage_parameters ) damage_parameters_type( … )` (also queued for the
behaviour-audit branch). These were the only two such assignments in the engine.
**Status:** fix building.

## OS-031: `level_04_011e` from the all-maps package crashed while loading

**Observed:** run `playtest-20261009-233121` (all-maps package
`573c5d1c…`): cooks failed (class 32) for
`models/flora/bush_s_002_sux.model/collision` and
`models/props/cover/tyres_s1/s2.model/walker_collision`; then
`static_collision::insert` → `physics::create_static_rigid_body` +0xb2 read
through a NULL shape (address 0x108). Retail would fail the same way on this
data (no null check). **Cause:** resource-port data, not engine code.
**Decision:** drop the map from the package for now (resource-porter
`package/all-maps`); fix the port later.

## OS-032: Keyboard under Wine with a US+PL+RU desktop keymap

**Observed:** Under Wine's X11 driver, letter keys (WASD, chat letters) never
reached the game; Space, Esc and digits did. The desktop keymap is
`pc_pl_ru-by_2 … group(alt_shift_toggle)`; the X11 driver guesses the layout
from keysyms and mis-maps letter keys when a Cyrillic group is present. Under the
Wayland driver (`DISPLAY` unset, `WAYLAND_DISPLAY` set) WASD and digits work, but
every chat letter typed "c": Wine names letter keys by their XKB keysym in the
Russian group (`Cyrillic_ef`, …) and the game lowercases the first character.
**Fix:** launch on the Wayland driver; `translate_key_action` (offline branch)
trusts a DirectInput key name only when it is one character and otherwise uses
the letter's scan code (its Latin virtual-key code). **Status:** WASD verified
on Wayland; letter fix building.

## OS-033: Intermittent render-arena exhaustion (parked)

**Observed:** EXE `e29c2fe9…` (00:45, 00:46) and `cdf75b2d…` (00:35): within
seconds of pressing Escape in the lobby, or on leaving a match, the 256 MB
`render` arena (`engine_world.cpp:80`) goes from 0% to 99.97%, "out of memory
in arena render", then a NULL read at 0x24 in
`collision::loose_oct_tree::update_bounds` ← `insert_impl` ← `move` ←
`render::scene::modify_model` ← `render::engine::world::update_model`. The same
EXE did not crash at 00:25, 00:47 or 00:48. Not seen before build 10 that night.
**Ruled out:** the all-maps package (byte-identical retail files, crash in the
lobby before anything ported loads); the "no surface with id[5/6/7]" warnings
(present in earlier healthy runs).
**Hypothesis (unverified):** a model with NaN/huge bounds makes the loose
octree subdivide until the arena is exhausted. Candidate: the recoil fix
(OS-029) now runs recoil interpolation/compensation every tick, also for the
lobby character's never-fired weapon; if that weapon's `weapon_recoil_params`
were never loaded, uninitialised floats could produce NaN koefs → NaN pose →
NaN bounds, which would explain the intermittency. **Status:** parked by the
user; next step is logging the bounds/transform of the model inside
`modify_model` when the octree grows.
**Update (01:00):** reproduced on `soc_mp_military_1` (run
`playtest-20261010-005834`), 18 s after screenshots showing vegetation instances
swapping models/positions between frames. All 38 `loose_oct_tree` functions are
100% retail matches, so the octree only explodes when fed NaN/huge model bounds;
the recoil hypothesis is weakened (`weapon_core_cook` always sets recoil params
from the weapon config, default ctor zeroes them). Leading suspect: models
receiving garbage transforms — on military_1 the ported vegetation (being fixed
in resource-porter v3), in the lobby still unknown.
