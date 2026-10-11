# Target vs. base: the whole-image report

How our linked `survarium-dx11-win32-gold.exe` compares with the retail
`survarium.exe`, region by region. This page used to be a static synthesis of
one-off tools (PE/Rich header, link order, RTTI); those comparisons are now one
live report:

    python3 -m vostok diff image                      # counts against config/image_diff.tsv
    python3 -m vostok diff image --detail identity    # headline fields side by side
    python3 -m vostok diff image --detail rich --detail imports --detail order
    python3 -m vostok diff image --update             # bank counts that moved toward retail

It reads the two EXEs and PDBs, the evidence databases and the data lane's
linked-image report of the last build, so run it after `vostok build`. It is not
part of the build. Compiler flags per project stay in [build-flags.md](build-flags.md);
the layout analysis behind the `order-*` regions is
[binary_matching/link-layout.md](binary_matching/link-layout.md).

## Regions

| region | what it counts | lower is |
|---|---|---|
| `headers` | DOS and PE header bytes, PE headers aligned at each image's signature, without layout fields, timestamp and checksum | better |
| `header-layout` | the excluded layout and clock bytes | informational |
| `rich` | \|count difference\| over the Rich header `@comp.id`s (product and build) | better |
| `imports` | import and delay-import tables aligned by name: DLLs and thunks missing, extra or moved, differing hints | better |
| `sections` | section-table rows whose name, order or characteristics differ | better |
| `rsrc` | resource bytes after rebasing data RVAs; all of retail's when ours has none | better |
| `code-exact` | retail code bytes reproduced: functions paired by PDB name whose bytes agree once every relocated field and rel32 branch is translated through the PDB symbols of each side | **higher** is better |
| `code-differing` | paired function bytes that differ (the ledger's work) | informational |
| `code-untranslated` | bytes that agree but reach a field one side cannot name | informational |
| `code-misplaced` | exact bytes outside the longest run that keeps retail order | better |
| `code-order` | places where our code order steps backwards in retail order | better |
| `code-unplaced` | our function bytes with no retail name | better |
| `code-absent` | retail function bytes with no counterpart in our image | better |
| `data-inexact`, `data-unclaimed`, `data-absent`, `data-unplaced` | the data lane's linked-image audit (`binaries/gen/data_image_report.json`) | better |
| `order-code-comdat`, `order-code-plain`, `order-rdata`, `order-data`, `order-bss` | pairwise inversions of compiland order per section kind (`vostok tool link-layout order`) | better |
| `vftables` | retail `??_7` vftables our image does not define | better |

Pairing uses every name the PDB gives an address: publics, procedures and
data. Local-scope ordinals and anonymous-namespace hashes are blanked, and a
name several compilands define pairs only with its compiland. An ICF-folded
address therefore matches through any of its names.

`config/image_diff.tsv` banks every non-informational region (`code-exact` as a
floor, the rest as ceilings). A run fails when a count moves past its bank, and
`--update` only tightens one. Lower a ceiling whenever a change moves the image
closer to retail; never raise one to admit a regression.

## First banked reading

Measured on the link-layout PR's full build (#595's tree plus the
`weapon_core` accessor fix):

- **Identity.** Machine `0x14c`, linker 9.0, subsystem 2, image base
  `0x10000` and characteristics `0x122` agree. Entry point, image size and
  section sizes follow the code that is still different. The CodeView path is
  the Wine build path.
- **Rich header.** 13 objects apart. Every C/C++ object on both sides is VS2008
  SP1 (build 30729): `0x83` 725/724, `0x84` 374/370, `0x8a` 1181/1174. The old
  report of a large VS2008 RTM (21022) share in our build no longer holds;
  the one RTM object left is retail's `0x94`.
- **Imports.** 87 findings, all order: no thunk is missing or extra. KERNEL32
  thunks sit in a different order, and the delay-import DLLs are emitted in a
  different order.
- **Sections and resources.** Ours has no `.rsrc` (562,176 bytes), which also
  shifts `.reloc` into its slot.
- **Code.** 5.13 MB of retail code reproduced exactly; 0.98 MB of it sits outside
  retail order, with 2,050 order breaks. 349 KB of ours has no retail name,
  and 145 KB of retail has no counterpart.
- **Order.** 8.05% of compiland pairs inverted for COMDAT code, 2.72% for
  non-COMDAT LTCG code, 4.84% `.rdata`, 3.31% `.data`, 7.44% `.bss`.
- **Polymorphic types.** 7 retail vftables are missing: six Scaleform
  allocator and video classes from the GFx archives, and
  `weapon_core_shotgun_reload_state_cook` (the old report counted 272).
