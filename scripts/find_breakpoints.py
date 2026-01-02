"""
find_breakpoints.py
"""

import argparse
import subprocess
from pathlib import Path

SCRIPT_DIR = Path(__file__).resolve().parent
ROOT_DIR   = (SCRIPT_DIR / ".." / "..").resolve()

BREAKPOINTS_DIR = ROOT_DIR / "vostok-structure" / "sources"


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("address", nargs="?")
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
        found_function = False
        for line in file:
            if found_function and '}' in line:
                break;
            elif found_function:
                start = line.find('0x')
                if start == -1:
                    continue;
                end = line.find('>', start)
                breakpoints.append(line[start:end])
            else:
                found_function = args.address in line

    print(len(breakpoints))
    for bp in breakpoints:
        print(bp)


if __name__ == "__main__":
    main()
