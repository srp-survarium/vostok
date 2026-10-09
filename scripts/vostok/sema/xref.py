# SPDX-License-Identifier: GPL-3.0-or-later

"""vostok.sema.xref - the direct caller/callee graph, per side.

`--callees` reads one function's call sites; the default direction scans the
whole side for functions that call it. Both work off the rendered instruction
text, so an operand is matched against a mangled name OR the demangled
qualified name, whichever the disassembly used.
"""

from __future__ import annotations

import bisect
import contextlib
import hashlib
import json
import re
import sqlite3

from vostok.sema.index import _records, _side_record


RE_CALL = re.compile(r"^call\s+(.+?)\s*$", re.IGNORECASE)


def _qualified_name(name):
    """Drop a demangled return type and argument list, preserving scoped names."""
    head = name.split("(", 1)[0].strip()
    marker = head.find("operator ")
    if marker >= 0:
        scope = head[:marker].rsplit(" ", 1)[-1]
        return scope + head[marker:]
    return head.rsplit(" ", 1)[-1]


def _call_operand(text):
    match = RE_CALL.match(text.strip())
    return match.group(1).strip() if match else None


def _callee_rows(rec):
    rows = []
    for insn in rec.get("instructions", []):
        operand = _call_operand(insn.get("text", ""))
        if operand:
            rows.append((insn.get("off", 0), operand))
    return rows


def _matches_operand(rec, operand):
    return operand in (rec["mangled"], _qualified_name(rec["name"]))


def _physical_span(image, selected):
    start, size = selected["rva"], selected.get("size", 0)
    section = image.section_at(start)
    if size <= 0 or section is None or section.name != ".text" or not section.contains(start, size):
        raise ValueError("selected PDB procedure has no complete physical .text span")
    raw = image.read_rva(start, size)
    return {
        "rva": start, "size": size, "sha256": hashlib.sha256(raw).hexdigest(),
        "head_hex": raw[:16].hex(),
        "nonpadding_bytes": any(value not in {0, 0x90, 0xCC} for value in raw),
    }


def _physical_pointer_rows(image, selected, symbols=()):
    """Actual HIGHLOW words only; labels annotate candidates, never callers."""
    value = image.image_base + selected["rva"]
    ordered = sorted(symbols, key=lambda symbol: symbol.rva)
    starts = [symbol.rva for symbol in ordered]
    rows = []
    for site in image.base_relocations():
        section = image.section_at(site)
        if section is None or not section.contains(site, 4) or image.u32_rva(site) != value:
            continue
        row = {"site_rva": site, "section": section.name,
               "value_va": value, "destination_rva": selected["rva"],
               "label": "", "label_rva": None, "label_offset": None,
               "vtable_candidate": False, "owner_extent_known": False}
        index = bisect.bisect_right(starts, site) - 1
        if index >= 0:
            owner = ordered[index]
            owner_section = image.section_at(owner.rva)
            same_section = owner_section is not None and owner_section.name == section.name
            known = same_section and owner.end is not None and site + 4 <= owner.end
            vtable = owner.display_name.startswith("??_7")
            if same_section and (known or owner.rva == site or vtable):
                row.update(label=owner.display_name, label_rva=owner.rva,
                           label_offset=site - owner.rva, vtable_candidate=vtable,
                           owner_extent_known=known)
        rows.append(row)
    return rows


def _stale_stripping_note(span, note):
    if not span["nonpadding_bytes"]:
        return False
    # A repaired historical note is not a fresh claim that this body is absent.
    for sentence in re.split(r"[.;]", note.lower()):
        if any(word in sentence for word in ("disproved", "prior stripped", "not stripped", "previously stripped")):
            continue
        if re.search(r"\b(?:link[- ]stripped|stripped(?:[- ]body)?|not emitted|no standalone body)\b", sentence):
            return True
    return False


def _verified_pointer_labels(side, image_hash, index, report):
    """Only the exact index bytes bound to this PE can annotate pointer sites."""
    from vostok.data.inventory import load

    try:
        inputs = json.loads(report.read_text(encoding="utf-8"))["inputs"][side]
        index_hash = hashlib.sha256(index.read_bytes()).hexdigest()
        if inputs.get("exe_sha256") != image_hash or inputs.get("index_sha256") != index_hash:
            return [], False
        symbols = load(index)
        # Detect an index replaced between validation and parsing.
        if hashlib.sha256(index.read_bytes()).hexdigest() != index_hash:
            return [], False
        return symbols, True
    except (OSError, ValueError, KeyError, TypeError, AttributeError):
        return [], False


def _cmd_physical_pointers(side, selected):
    from vostok.core import paths
    from vostok.data.pe import PEImage
    from vostok.data.pipeline import image_paths
    from vostok.sema.index import _index_path
    from vostok.sema.pairing import ledger_row

    image = PEImage(image_paths(side)[0])
    with contextlib.closing(sqlite3.connect(f"file:{_index_path(side)}?mode=ro", uri=True)) as connection:
        row = connection.execute("SELECT value FROM metadata WHERE key='exe_sha256'").fetchone()
    image_hash = hashlib.sha256(image.data).hexdigest()
    if row is None or row[0] != image_hash:
        raise ValueError("physical pointer view requires a PE matching its PDB evidence hash")
    index = paths.DATA_TARGET_INDEX if side == "target" else paths.DATA_BASE_INDEX
    symbols, labels_verified = _verified_pointer_labels(side, image_hash, index, paths.DATA_REPORT)
    span = _physical_span(image, selected)
    rows = _physical_pointer_rows(image, selected, symbols)
    print(f"{side} physical span rva={span['rva']:#x} size={span['size']} "
          f"sha256={span['sha256']} head={span['head_hex']}")
    if not labels_verified:
        print("Data/vtable labels omitted: no data report binds this PE and the current index bytes.")
    print(f"HIGHLOW code-pointer words to {selected['name']}: {len(rows)}")
    for row in rows:
        label = ""
        if row["label"]:
            label = f" label={row['label']}+{row['label_offset']:#x}"
            if not row["owner_extent_known"]:
                label += " (extent unknown)"
        role = " vtable candidate" if row["vtable_candidate"] else ""
        print(f"  site_rva={row['site_rva']:#x} section={row['section']} "
              f"value_va={row['value_va']:#x} destination_rva={row['destination_rva']:#x}{role}{label}")
    ledger = ledger_row(selected)
    if side == "base" and ledger and _stale_stripping_note(span, ledger.get("note") or ""):
        print("WARNING: ledger stripping claim conflicts with a nonpadding physical procedure span; "
              "recheck the exact side/owner and saved note evidence.")
        print(f"  note={ledger['note']}")
    print("Pointer words and vtable labels are reference candidates, not runtime reachability. "
          "Confirm constructor storage, slot and actual indirect caller separately. "
          "No pointers found does not prove absence of virtual/address-taken calls.")
    return 0


def cmd_xref(args):
    side, selected = _side_record(args)
    if getattr(args, "pointers", False):
        return _cmd_physical_pointers(side, selected)
    if args.callees:
        rows = _callee_rows(selected)
        if not args.raw:
            counts = {}
            first = {}
            for off, operand in rows:
                counts[operand] = counts.get(operand, 0) + 1
                first.setdefault(operand, off)
            rows = [(first[name], name, counts[name]) for name in sorted(counts)]
        else:
            rows = [(off, name, 1) for off, name in rows]
        print(f"{side} callees of {selected['name']} (rva={selected['rva']:#x})")
        for off, operand, count in rows:
            suffix = f" x{count}" if count > 1 else ""
            print(f"  +0x{off:04x}  {operand}{suffix}")
        return 0

    hits = []
    for rec in _records(side):
        sites = [(off, operand) for off, operand in _callee_rows(rec)
                 if _matches_operand(selected, operand)]
        if sites:
            hits.append((rec, sites))
    print(f"{side} callers of {selected['name']} (rva={selected['rva']:#x})")
    if not hits:
        print("  No name-matched direct calls; inspect --pointers for virtual/address-taken reference candidates.")
    for rec, sites in hits:
        if args.raw:
            for off, _ in sites:
                print(f"  rva={rec['rva']:#010x}+0x{off:04x}  {rec['file']}  {rec['name']}")
        else:
            print(f"  rva={rec['rva']:#010x}  calls={len(sites):<3}  {rec['file']}  {rec['name']}")
    return 0
