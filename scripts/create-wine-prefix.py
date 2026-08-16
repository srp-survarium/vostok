#!/usr/bin/env python3
"""create-wine-prefix.py - the local Survarium PLAY environment.

Deliberately flat, not part of the `vostok` package: this provisions a machine
to RUN the game, which needs neither the matching toolchain nor the dev shell.
It must work from a bare clone with nothing on PYTHONPATH.

    python3 scripts/create-wine-prefix.py [--target ~/survarium-wine]
                                          [--game <dir>] [--resources <dir>]

It assembles a prefix SEPARATE from the build prefix (binaries/.wineprefix).
Never share them: the build prefix carries VS2008, mspdbsrv and a Windows
version pinned for the compiler, none of which the game wants.

    <target>/prefix/   the wine prefix (ru_RU locale, a virtual desktop)
    <target>/game/     survarium.exe + its DLLs, copied from --game
                       (SURVARIUM_BIN by default)

WHAT THIS IS FOR
----------------
Survarium's official servers are gone - the shipped client dials
188.93.23.27:65001, which has not answered for years. Playing means running
the Survarium Restoration Project (~/Projects/surv/srp) locally and pointing
the client at it:

    cargo run --bin login-server      127.0.0.1:1234
    cargo run --bin lobby-server      127.0.0.1:1235
    cargo run --bin match-server      127.0.0.1:1236
    cargo run --bin browser-server    127.0.0.1:80    (privileged port)

    survarium.exe -no_splash_screen -client=127.0.0.1:1234

THE CONNECTION FIX (srp c67196e8a, "bind vs advertised")
--------------------------------------------------------
Connecting from Wine did not work for a long time, and the reason is that
there are TWO addresses, not one. Each srp server now separates them:

    bind_host    default 0.0.0.0     where the server listens
    public_host  default 127.0.0.1   what the client is TOLD to dial next

The client is handed onward addresses (login -> lobby -> match), so a single
hardcoded 127.0.0.1 is simultaneously wrong for binding and right for
advertising. Fixing one breaks the other and the symptom is identical either
way - the connection simply fails. Override with SRP_PUBLIC_HOST and
SRP_<SERVER>_{BIND,PUBLIC_HOST,PORT}.

STATUS: UNVERIFIED END TO END
-----------------------------
The prefix settings below are inferred, not measured: we have no runnable
build of our own yet, which is the whole point of the matching campaign. The
srp side (ports, the -client= flag, bind-vs-advertised) IS grounded - it comes
from srp's dev-bootstrap and that commit. Treat every Wine setting here as a
starting point and record what actually worked, the way gruntz's prefix
doctrine does.
"""

from __future__ import annotations

import argparse
import os
import shutil
import subprocess
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent

# The game is Russian; its resources and fonts assume a Cyrillic locale.
LOCALE = "ru_RU.UTF-8"

SRP_SERVERS = (
    ("login-server", 1234),
    ("lobby-server", 1235),
    ("match-server", 1236),
    ("browser-server", 80),
)


def log(message: str) -> None:
    print(f"[wine-prefix] {message}")


def run(argv: list[str], env: dict[str, str]) -> int:
    proc = subprocess.run(argv, env=env)
    return proc.returncode


def wine_env(prefix: Path) -> dict[str, str]:
    env = dict(os.environ)
    env["WINEPREFIX"] = str(prefix)
    env["LANG"] = LOCALE
    env["LC_ALL"] = LOCALE
    # The game ships its own splash/video path; Wine's mono and gecko prompts
    # only get in the way of an unattended provision.
    env["WINEDLLOVERRIDES"] = "mscoree,mshtml="
    env.setdefault("WINEDEBUG", "fixme-all")
    return env


def resolve_game_dir(explicit: str | None) -> Path | None:
    for candidate in (explicit, os.environ.get("SURVARIUM_BIN")):
        if candidate and Path(candidate).is_dir():
            return Path(candidate)
    return None


def provision(target: Path, game: Path | None, resources: Path | None) -> int:
    prefix = target / "prefix"
    game_dir = target / "game"
    prefix.mkdir(parents=True, exist_ok=True)
    game_dir.mkdir(parents=True, exist_ok=True)

    env = wine_env(prefix)
    if not shutil.which("wine"):
        log("wine is not on PATH - enter `nix develop` or install wine")
        return 1

    log(f"initialising prefix at {prefix} (locale {LOCALE})")
    if run(["wineboot", "--init"], env) != 0:
        log("wineboot failed")
        return 1

    # A virtual desktop keeps the game from switching the host video mode; a
    # crash then cannot leave the desktop at 1024x768.
    run(["wine", "reg", "add", r"HKCU\Software\Wine\Explorer",
         "/v", "Desktop", "/d", "Default", "/f"], env)
    run(["wine", "reg", "add", r"HKCU\Software\Wine\Explorer\Desktops",
         "/v", "Default", "/d", "1600x900", "/f"], env)

    if game is None:
        log("no game directory (pass --game or set SURVARIUM_BIN);"
            " prefix is ready but empty")
    else:
        copied = 0
        for entry in sorted(game.iterdir()):
            if entry.is_file() and entry.suffix.lower() in (".exe", ".dll"):
                destination = game_dir / entry.name
                if not destination.exists():          # idempotent: never clobber
                    shutil.copy2(entry, destination)
                    copied += 1
        log(f"game files: {copied} copied into {game_dir}"
            f" ({len(list(game_dir.iterdir()))} present)")

    if resources is not None and resources.is_dir():
        link = game_dir / "resources"
        if not link.exists():
            link.symlink_to(resources)
            log(f"resources linked: {link} -> {resources}")
    else:
        log("no resources directory (pass --resources or use"
            " `nix develop .#with-resources`)")

    print()
    log("to play, start the srp servers, then:")
    for name, port in SRP_SERVERS:
        print(f"    (cd ~/Projects/surv/srp && cargo run --bin {name})"
              f"   # 127.0.0.1:{port}")
    print()
    print(f"    WINEPREFIX={prefix} LANG={LOCALE} \\")
    print(f"      wine {game_dir}/survarium.exe"
          f" -no_splash_screen -client=127.0.0.1:1234")
    print()
    log("browser-server wants port 80: run it with CAP_NET_BIND_SERVICE or"
        " override SRP_BROWSER_SERVER_PORT")
    return 0


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(
        description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter,
    )
    parser.add_argument("--target", default=str(Path.home() / "survarium-wine"),
                        help="where to build the play environment"
                             " (default ~/survarium-wine)")
    parser.add_argument("--game", help="directory holding survarium.exe"
                                       " (default $SURVARIUM_BIN)")
    parser.add_argument("--resources", help="game resources directory"
                                            " (default $VOSTOK_RESOURCES_UNPACKED)")
    args = parser.parse_args(argv)

    resources = args.resources or os.environ.get("VOSTOK_RESOURCES_UNPACKED")
    return provision(
        Path(args.target).expanduser(),
        resolve_game_dir(args.game),
        Path(resources).expanduser() if resources else None,
    )


if __name__ == "__main__":
    sys.exit(main())
