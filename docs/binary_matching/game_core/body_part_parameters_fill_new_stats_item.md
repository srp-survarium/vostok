# game_core::body_part_parameters::fill_new_stats_item<ai::statistics_item<46,16>>

Template member function, instantiated for `vostok::ai::statistics_item<46,16>`. Writes a
human-readable statistics dump into the output `statistics_item&`:
- `caption = m_name; caption.append(" state:")`
- content lines: `"HP: %4.2f/%4.2f"`, `"affects: "`, then one
  `"%s [%4.2f seconds left]"` line per active affect, or `"none"` if `m_affects` is empty.

Target: `0x0ba3c0`  (file `vostok/game_core/sources/body_part_parameters.cpp`)
Mangled-visible signature:
`void survarium::body_part_parameters::fill_new_stats_item<vostok::ai::statistics_item<46,16> >(vostok::ai::statistics_item<46,16>&, const unsigned int) const`

## Member offsets used
body_part_parameters: m_affects @0x24 (fixed_vector<pair<hit_affects_type_enum,u32>,8>),
m_name @0x70 (fixed_string<16>), m_max_health @0x8c, m_health @0x90.
statistics_item<46,16>: caption @0x00 (fixed_string<32>), content @0x2c (fixed_vector<fixed_string<46>,16>).
pair: first @0x00 (enum), second @0x04 (u32). Each pair is 8 bytes (sar edx,3 in size calc).

## Asm decode (target, key fragments)
- 0x10: caption(=[ebp+8]+0) = m_name(this+0x70)  -> buffer_string::operator=(buffer_string const&)
- 0x21: caption.append(" state:")   (??_C@_07 = 7 chars)
- 0x2e: fixed_string<46> new_item;  (default ctor at [ebp-3Ch])
- 0x36: new_item.appendf("HP: %4.2f/%4.2f", m_health@0x90, m_max_health@0x8c)
        (m_health pushed last -> first varargs; format decodes "HP: %4.2f/%4.2f")
- 0x72: content(=[ebp+8]+0x2c).push_back(new_item)
- 0x81: new_item.clear(); new_item.append("affects: ")  (??_C@_09 = 9 chars "affects: ")
- 0x96: content.push_back(new_item)
- 0xa5: for (i=0; i<m_affects.size(); ++i)  [i @ ebp-40h; size = (m_affects.end-begin)/8]
  - 0xe3: new_item.clear()
  - 0xfd: remaining = m_affects[i].second >= current_time_in_ms ? m_affects[i].second - current_time_in_ms : 0
    (cmp [i].second < current -> .3 (=0); else diff)   [remaining @ ebp-44h]
  - 0x164: fild qword(remaining), fdiv __real@447a0000 (1000.0) -> push as double
    appendf("%s [%4.2f seconds left]", affects_captions[m_affects[i].first], remaining/1000.0)
  - 0x1b0: content.push_back(new_item)
- 0x1c4: if (m_affects.empty()) content.push_back(fixed_string<46>("none"))  (??_C@_04 = "none", cap 0x2e=46)

The 3 masked `finalize_impl()` calls (0xf2, 0x11f, 0x15f) are no-op destructors of inlined
temporaries around new_item.clear() / the remaining-time computation -- not steerable from source.

## Commands run (verbatim, in order)
- pdb_rich_query --index binaries/rich/target/index.jsonl --function fill_new_stats_item --list
- pdb_fetch --target-index binaries/rich/target/index.jsonl --function fill_new_stats_item --view target --rva 0x0ba3c0
- pdb_fetch --target-index binaries/rich/target/index.jsonl --function fill_new_stats_item --view callees --rva 0x0ba3c0
- pdb_fetch --target-index binaries/rich/target/index.jsonl --function fill_new_stats_item --view info --rva 0x0ba3c0
- python3 scripts/rebuild.py        # NO module arg

## Iterations
1. INPUT: first body (see body_part_parameters.cpp), template declared PUBLIC in
   body_part_parameters.h, explicit instantiation in .cpp, anchored in temp_include_all.cpp
   (construct stats_item, bpp->fill_new_stats_item, escape &stats_item via example_callback).
   Used `/ 1000.0` first, switched to `/ 1000.0f` mid-build (fdiv dword in target).
   BUILD: report.json fuzzy_match_percent = None.
   DIAGNOSIS: base mangled `...@survarium@@QBE...` (public const) vs target `...@survarium@@ABE...`
   (PRIVATE const). Per loop_performance.md, the QBE-vs-ABE leading-letter mismatch makes objdiff
   unable to pair base<->target, so it reports None. Function compiled + was reachable (it was in
   binaries/rich/base), just unpaired.
2. INPUT: moved declaration to `private:` section. A private member can't be called from the
   external IncludeAll anchor, so removed that anchor and instead wired the GENUINE caller
   dump_state(npc_statistics&) to call fill_new_stats_item (it's a same-class member, already
   anchored via bpp->dump_state(stats,10); the carcass confirms dump_state calls it at <0x597165>).
   Kept the explicit instantiation so a standalone body is emitted for diffing.
   BUILD: report.json fuzzy_match_percent = 91.78519 (paired now; ABE private const matched).
   DIFF:  header raw-instruction-equal = 28.0% (dominated by reg/slot renaming + a +0x10
   frame-size delta); operand-aware report % = 91.78 is authoritative.

## Worker #16 (resumption) - verifying the "none" fixed_string change

### Commands run (verbatim)
- git checkout match/game_core-fill_new_stats_item
- nix develop --command bash -c 'pdb_rich_query --index binaries/rich/target/index.jsonl --function fill_new_stats_item --list'
- nix develop --command bash -c 'pdb_fetch --target-index binaries/rich/target/index.jsonl --function fill_new_stats_item --view target --rva 0x0ba3c0'
- nix develop --command bash -c 'pdb_fetch --target-index binaries/rich/target/index.jsonl --base-index binaries/rich/base/index.jsonl --function fill_new_stats_item --view diff --rva 0x0ba3c0'
- nix develop --command bash -c 'pdb_rich_query --index binaries/rich/target/index.jsonl --function "fixed_string<46>::fixed_string<46>" --list'
- nix develop --command bash -c 'pdb_rich_query --index binaries/rich/base/index.jsonl  --function "fixed_string<46>::fixed_string<46>" --list'
- nix develop --command bash -c 'pdb_rich_query --index binaries/rich/{target,base}/index.jsonl --function "buffer_string::buffer_string" --list'
- nix develop --command bash -c 'python3 scripts/rebuild.py'   # NO module arg
- nix develop --command bash -c 'pdb_fetch --target-index binaries/rich/base/index.jsonl --function "fixed_string<46>::fixed_string<46>" --view target --rva 0x030b00'

### What the asm showed
TARGET "none" branch (0x1f0-0x20d) = inlined fixed_string<46>(const char*):
  mov dword ptr [ebp-0CCh], 2Eh   ; capacity temp = 46
  push "none"                     ; src
  lea ecx,[ebp-0CCh]; push ecx    ; &capacity (size_type const&)
  lea edx,[ebp-74h];  push edx    ; this+0xC = m_buffer
  lea ecx,[ebp-80h]               ; this (the fixed_string temp)
  call vostok::buffer_string::buffer_string   ; the 3-arg (char*,u32 const&,char const*) overload
BASE "none" branch (current 91.78% source): push "none"; lea ecx,[ebp-80h];
  call vostok::fixed_string<46>::fixed_string<46>   ; the out-of-line const-char* wrapper

### Symbol presence (decisive)
- TARGET index has NO out-of-line `fixed_string<46>::fixed_string<46>(char const*)` symbol at all
  (it was inlined whole-program); it DOES keep the default ctor out-of-line (0x0ba7c0) and CALLS it.
- BASE index HAS the out-of-line `fixed_string<46>::fixed_string<46>(char const*)` at 0x030b00.
- The 3-arg `buffer_string::buffer_string(char*,u32 const&,char const*)` exists out-of-line in BOTH
  (target 0x001040, base 0x001140).
- I disassembled the BASE out-of-line const-char* ctor (rva 0x030b00): its body is EXACTLY
    mov [ebp-4],2Eh; push src; add eax,0Ch (this+0xC=m_buffer); lea ecx,[ebp-4]; call buffer_string::buffer_string
  i.e. the very 3-arg buffer_string call the target inlines. So the source
  `fixed_string<46>("none")` already produces the correct code; the only difference is that
  target inlined this 5-instr ctor into fill_new_stats_item while base emits a `call` to it.

### Verdict
The WIP worker's hypothesis ("change source to the 3-arg buffer_string form") is moot: the
const-char* fixed_string ctor ALREADY delegates to that exact 3-arg buffer_string ctor. There is no
source expression that yields a `fixed_string<46>` temp with its own internal buffer other than the
const-char* ctor, and no per-call-site way to force MSVC to inline a COMDAT under /Od+/GL. This is
the documented LTCG out-of-line-call-vs-inline-of-a-trivial-COMDAT class (assembly_patterns.md).
The +0x10 frame-size delta (E8h vs D8h) and all the reg/slot renaming cascade from this one inline.
Left the source as `fixed_string<46>("none")` (rule #1: don't contort a correct match to chase LTCG).
NO source change was made on resumption -> no extra rebuild needed; 91.78% is final.

## Outcome
STATE[91.78%|PARTIAL]. Body is a structural match; sole residual is the inline-vs-call of the
fixed_string<46>("none") ctor plus the frame/slot/register differences it forces. rebuild.py: clean
build, report-changes.json = 0 regressed / 0 improved / 0 removed / 0 added.

## Review note (new guidelines)
The updated MATCHING.md narrows the LTCG excuse to *function arguments only* and lists inline-vs-call
as a matching problem. The "LTCG" framing for this 91.78% residual is therefore downgraded: keep it
PARTIAL, but the fixed_string<46>(const char*) inline-vs-call should be re-diffed against source on a
future rebuild (the rich-index proof above shows the target has no out-of-line const-char* ctor while
base keeps one - the documented unsteerable-COMDAT class - re-confirm before banking). The FUNCTION
BODY carcass has been restored in the .cpp (the function is PARTIAL, not 100%). This review did NOT
rebuild; the body shape is unchanged.
