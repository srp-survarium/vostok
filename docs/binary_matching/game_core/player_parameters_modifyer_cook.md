# player_parameters_cook.cpp - structure verification (chain/game_core-sv-player_parameters_cook)

Verified the four matched-but-<100% functions. All four are STRUCTURE MATCHES whose
residual is whole-program LTCG inline-vs-call or ICF fold - none had a fixable
source-structure divergence. No source-logic edits were made (comments + STATE only),
so the percentages are unchanged.

Diagnosis command (per function):
```
nix develop -c pdb_fetch --target-index binaries/rich/target/index.jsonl \
  --base-index binaries/rich/base/index.jsonl --function <name> --rva 0x<rva> \
  --view structure-diff --condensed \
  --objdiff-base-dir binaries/objdiff/base --objdiff-target-dir binaries/objdiff/target
```
(Default name lookup picked the wrong unit - `delete_resource` matched
`ai::behaviour_cook::delete_resource`; always pass `--rva`. RVAs from
`pdb_rich_query --index binaries/rich/target/index.jsonl --function <qualified> --list`.)

## delete_resource (target 0x59b330) - 31.00% PARTIAL, WALL
Structure: 1 stmt / 1 stmt (matches). The lone statement `VOSTOK_DELETE_IMPL(g_allocator,
resource)` diverges:
- TARGET: `lea eax,[ebp+8]; push eax; mov eax,[g_allocator]; call <finalize_impl-misname>;
  push eax; call delete_helper; add esp,8`  (strip_pointer kept out-of-line, two pushed args)
- BASE:   `mov eax,[g_allocator]; call <Release-misname>; push eax; lea edi,[ebp+8];
  call delete_helper; add esp,4`            (strip_pointer inlined, one arg)
The macro is `delete_helper(strip_pointer(g_allocator), resource)` (Master Gold no-debug
form, memory_macros.h:41). Pure whole-program inline-vs-call. CONFIRMED non-steerable:
`items_cook::delete_resource` has a byte-identical diff and is already banked at 31% DONE.
The ~31% is just how few operands survive when nearly every instr's operand shifts.

## player_parameters_modifyer_cook() ctor (target 0x59b2e0) - 100% DONE
The STATE marker was STALE (said 66.69% PARTIAL); report.json scores the sole ctor symbol
`??0player_parameters_modifyer_cook@@QAE@XZ` at 100.0%. Structure: 1 stmt / 1 stmt,
`aligned 1, size-diffs 0, quantity-diffs 0`. The `--view diff` instruction view showed the
`call` at 0x0e resolving to different fold names (base->random32::seed, target->boost::_bi::
list1) for the translate_query_cook 4th DEFAULT arg `enum_flags<flags_enum> flags = 0`
(resources_cook_classes.h:144) ctor - but that ICF fold does NOT break objdiff's reloc
pairing here, so the function scores a clean 100%. Embed deleted, reduced to bare
`// STATE[100%|DONE]`.

## translate_query (target 0x59b360) - 94.92% PARTIAL, WALL  (STATE was stale at 89.62)
Structure: 95 stmts / 95 stmts, `aligned 90, size-diffs 4, quantity-diffs 2`. The size diffs:
- `if value_exists("additional_slots")` and the two `additional_{artefact,devices}_slots +=
  (u8)cfg["additional_slots"][...]` statements.
- the documented map_assign `body_part_parameters_modifyers[name] = modifyer` (sushi@MATCH).
Root cause for the `(u8)` lines: the conversion chain `operator u8() -> cast_unsigned_number<u8>()
-> cast_number<u8,u64,u32>()` is cut at a different inline level in each binary:
- TARGET calls `binary_config_value::operator unsigned char` out-of-line (rva 0x52160) and
  inlines cast_number INTO it.
- BASE inlines the operator away and calls `cast_number<unsigned char,unsigned __int64,
  unsigned int>` directly (base rva 0x79dd0).
Verified via both rich indexes: operator-u8 standalone ONLY in target, cast_number<u8> ONLY in
base. Source `(u8)cfg[...]` is already correct. Non-steerable inline-vs-call (same class as
is_aimed/get_user). map_assign is the same wall.

## apply (target 0x59bc10) - 85.01% PARTIAL, WALL  (STATE was stale at 84.21)
Structure: 86 stmts target / 76 base, `aligned 61, size-diffs 15, quantity-diffs 10` - the
quantity-diffs are `EMPTY only target` (inlined-call boundaries the target keeps as separate
no-address statements) and one `ONLY target` L141 `}` at 0x6d7 (the anomaly for-loop body's
closing-brace jmp, present only because the body is non-fall-through in target).
Drivers (all whole-program LTCG inline-vs-call):
1. First statement `damage_model_ptr damage_model = player->damage_model()`: TARGET copy-ctor
   call `intrusive_ptr<...>::intrusive_ptr` (one call); BASE `mov [ebp-4],0; ...; call ::set`
   (default-init + set). The resource_ptr copy-ctor is out-of-line in target, inlined in base.
2. `player->damage_model()->add_damage_protector(...)` in the anomaly loop (sushi's "biggest
   diff, order different"): TARGET re-calls the virtual `damage_model()` then `intrusive_ptr::
   operator*` (out-of-line) each iteration; BASE inlines `operator*` to a direct `mov eax,[edx]`
   (m_object load). Same inline-vs-call wall.
These bump the frame by 8 (`sub esp,22Ch` vs `224h`) and rename every subsequent `[ebp-XXX]`
slot/register - a cascade from the two inline decisions. Source is already correct; non-steerable.
The `call random32::seed`/`list1` pairs throughout are ICF-fold misnames of fixed_string<16>/
iterator helpers (identical bytes).

## Result (numbers from report.json; STATE markers corrected where stale; edits are comment-only so codegen unchanged)
- delete_resource    31.00% -> 31.00% PARTIAL (WALL: strip_pointer inline-vs-call)
- ctor               66.69% -> 100%   DONE    (STATE was stale; clean match, embed deleted)
- translate_query    89.62% -> 94.92% PARTIAL (STATE was stale; WALL: operator u8 vs cast_number inline-vs-call + map_assign)
- apply              84.21% -> 85.01% PARTIAL (STATE was stale; WALL: resource_ptr copy-ctor + intrusive_ptr::operator* inline-vs-call, +8 frame cascade)
