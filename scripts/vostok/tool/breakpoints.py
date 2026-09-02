# SPDX-License-Identifier: GPL-3.0-or-later

"""
vostok.tool.breakpoints
"""

import argparse
import subprocess

from vostok.core.paths import TARGET_SOURCES as BREAKPOINTS_DIR
from vostok.core import log as _log


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("address", nargs="?")
    parser.add_argument("--whole-file", action="store_true")
    args = parser.parse_args()

    result = subprocess.run(
        ["rg", "-l", args.address, BREAKPOINTS_DIR],
        capture_output=True,
        text=True,
        check=True
    )
    files = [file for file in result.stdout.splitlines() if file.strip()]

    if len(files) != 1:
        raise ValueError(f"Expected exactly one file match, got {len(files)}")

    breakpoints = []
    with open(files[0], "r") as file:
        if args.whole_file:
            inside_function = False
            for line in file:
                if inside_function and "******" in line:
                    inside_function = False
                elif inside_function:
                    start = line.find('0x')
                    if start == -1:
                        continue
                    end = line.find('>', start)
                    breakpoints.append(line[start:end])
                else:
                    inside_function = "FUNCTION BODY" in line

        else:
            found_function = False
            for line in file:
                if found_function and '}' in line:
                    break
                elif found_function:
                    start = line.find('0x')
                    if start == -1:
                        continue
                    end = line.find('>', start)
                    breakpoints.append(line[start:end])
                else:
                    found_function = args.address in line

    print(len(breakpoints))
    for bp in breakpoints:
        print(bp)


if __name__ == "__main__":
    raise SystemExit(_log.run("vostok.tool.breakpoints", main))
