"""vostok.core.tsv - the one tracked-table convention.

A tracked table is a tab-separated text file under `docs/binary_matching/`:
`#`-prefixed comment lines, blank lines ignored, one record per line, fields
never containing a tab. It is committed, so it must diff and merge line by line
- that is the whole reason these are text and `match.db` is not.

The reviewed override tables (`exact_fold_aliases.tsv`,
`module_ownership_overrides.tsv`, `folded_symbol_overrides.tsv`) and the
delinker's generated symbol map all use this shape. `read` owns the shape;
each caller owns its own arity check and error wording, because a malformed
override table must name the tool that will misbehave.

The matching ledger (`ledger.store`) is deliberately NOT read through here: it
carries a `# [units]` legend and a header row, and its format is settled.
"""

from __future__ import annotations

from pathlib import Path
from typing import Iterator


def read(path: Path | str, *, strip: bool = True) -> Iterator[tuple[int, list[str]]]:
    """Yield `(line number, fields)` for every record line.

    `strip=False` keeps surrounding whitespace inside the first/last field -
    which is what a table whose records may legitimately end in a space wants.
    """
    for number, raw in enumerate(Path(path).read_text(encoding="utf-8").splitlines(), 1):
        line = raw.strip() if strip else raw
        if not line or line.startswith("#"):
            continue
        yield number, line.split("\t")


def write_if_changed(path: Path | str, text: str) -> bool:
    """Write `text`, and report whether the file's content actually moved.

    Generated tables are regenerated on every build; rewriting an unchanged one
    costs a spurious mtime bump (and, for anything ninja watches, a spurious
    rebuild of everything downstream). Returning the bool also lets a caller say
    "unchanged" instead of implying work happened.
    """
    path = Path(path)
    if path.is_file() and path.read_text(encoding="utf-8") == text:
        return False
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(text, encoding="utf-8")
    return True
