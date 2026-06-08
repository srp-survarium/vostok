# game_core one-liner batch 1 (18 trivial functions, 10 headers, one rebuild)

Branch: `match/game_core-oneliners-batch1` off `match/game_core-breath_vibration_calculator`
(tip `f6b29db8`). One `rebuild.py` (706s wall, exit 0, no post-build stall).

## Result table

| fn | STATE | cause |
|----|-------|-------|
| `damage_protector::damage_protector()` | 100% DONE | already matched (marker was STUB) |
| `damage_protector::~damage_protector()` | 100% DONE | already matched (marker was STUB) |
| `artefact_container_core::use_info` | None PARTIAL | frameless-target vs /Od-framed-base |
| `booby_trap_core::get_speed` | None PARTIAL | frameless-target vs /Od-framed-base |
| `inventory_holder::inventory()` (non-const) | None PARTIAL | trivial-accessor LTCG inline-vs-call |
| `inventory_holder::inventory() const` | None DONE | ICF-folds with non-const |
| `inventory_holder::scheduler()` | None PARTIAL | trivial-accessor LTCG inline-vs-call |
| `weapon_user_animations_selector::set_animations` | None PARTIAL | trivial-setter LTCG inline-vs-call |
| `base_project::register_named_object` | None PARTIAL | LTCG inline-vs-call |
| `base_project::register_object_to_resolve` | None PARTIAL | LTCG inline-vs-call |
| `collision_geometry::cast_to_collision_geometry` | None DONE | ICF-fold `return this` |
| `player_logic_base_state::is_ready_for_transition` | None DONE | ICF-fold `return true` |
| `collision_sensor::on_inside` | None DONE | ICF-fold empty (moved to `protected:`/`MAE`) |
| `collision_sensor::on_leave` | None DONE | ICF-fold empty (`MAE`) |
| `collision_sensor::on_enter` | None DONE | ICF-fold empty (`MAE`) |
| `collision_sensor::on_objetcs_loosed` | None DONE | ICF-fold empty (`MAE`) |
| `interactive_object::assign_game_ui` | None DONE | ICF-fold empty `ret 4` |
| `interactive_object::cast_weapon_core` (const + non-const) | None DONE | ICF-fold `xor eax,eax;ret` |

## Commands run

```
# target symbol/rva discovery
nix develop --command python3 -c "<parse binaries/rich/target/index.jsonl by substring>"
# target asm per rva
nix develop --command pdb_fetch --target-index binaries/rich/target/index.jsonl --rva <rva> --view target
# access chars from delinked target obj symbol tables
strings binaries/objdiff/target/vostok/game_core/<unit>.h.obj | grep <fn>
# per-unit current scores
nix develop --command python3 -c "<read binaries/objdiff/report.json units/functions>"
# rebuild (ONE)
nix develop --command python3 scripts/rebuild.py     # 706s wall, exit 0
# grade + reachability
nix develop --command python3 -c "<re-read report.json>"
grep -c '<fn>' binaries/rich/base/index.jsonl
# COFF byte-compare target vs base obj .text
nix develop --command python3 <<'EOF' ... walk COFF, slice .text by symbol value ... EOF
```

## Key findings (asm evidence)

### use_info / get_speed: TARGET is FRAMELESS, our /Od is framed (build-flag, not source)
COFF `.text` byte-dump of the delinked TARGET obj:
- `?get_speed@booby_trap_core...EBEMXZ` = `d9 ee c3` = `fldz; ret` (3 bytes, NO ebp frame).
- `?use_info@artefact_container_core...UAE...` = `b8 <reloc> c2 04 00` = `mov eax,lit; ret 4` (frameless).

Our BASE obj (same source):
- `get_speed` = `55 8bec 51 894dfc d9ee 8be5 5d c3` (full `/Od` frame + `mov [ebp-4],ecx`).
- `use_info` = `55 8bec 51 894dfc b8 <reloc> 8be5 5d c2 0400`.

The original build OMITTED the ebp frame for these `this`-unused leaf members (frame-pointer
omission), which `/Od` never does. The size/byte gap (3-5 vs 11+ bytes) is too large for objdiff
to pair -> None. Not steerable from source under our flags. The semantic body (fldz / the literal /
ret 4) IS correct. (Contrast `damage_protector` ctor/dtor, which USE `this` and match 100% WITH
frames - the original keeps frames whenever `this` is referenced.) These two reached the EXE
standalone (qualified-call anchors), so they are present-but-divergent PARTIAL, not dead-stripped.

### getters/setters/registrars: trivial-accessor LTCG inline-vs-call
`inventory()`/`scheduler()`/`set_animations`/`register_named_object`/`register_object_to_resolve`
were each INLINED whole-program into their synthetic anchor (`concrete_*::touch()`), verified by
disassembling the anchor:
- `inventory()` -> `mov [this+8]` (raw ptr inside the smart-ptr member @ +8) - correct offset.
- `scheduler()` -> `mov [this+4]` (m_scheduler reference @ +4) - correct offset.
- `register_named_object` -> `add ecx,4; call map<fixed_string<260>,..>::operator[]<char const*>;
  mov [eax],obj` - exactly the target's `add eax,4; ...; mov [eax],ecx; ret 8`.
No standalone body reaches the EXE (not even in the delinked obj), so report.json scores None. This
is the documented trivial-accessor inline-vs-call class: anchored only by a synthetic caller, /GL
LTCG inlines it whole-program. They are capped at None until a REAL game consumer (reader/caller of
the same members) keeps them standalone. Bodies are byte-correct (offsets verified).

### ICF-fold trivial virtual overrides: byte-correct, unscorable None
`cast_to_collision_geometry` (`return this`, @0x17600 empty-frame fold), `is_ready_for_transition`
(`return true`, @0x12700 `mov al,1;ret`), the four `collision_sensor::on_*` (empty, @0x12c50 `ret 4`
/ @0xd2070), `assign_game_ui` (`ret 4`), `cast_weapon_core` x2 (`xor eax,eax;ret` @0x327c0). /OPT:ICF
folds each into a representative whose symbol differs build-to-build, so neither delinker re-attaches
the body to our function -> None. PROVED the bytes are emitted by counting the fold families in
`binaries/rich/base/index.jsonl`: `mov al,1;ret`=25, `ret 4`=68, `xor eax,eax;ret`=16, empty
this-frame=100. Marked None|DONE (do not chase the None).

### access fix: collision_sensor's four on_* overrides are `MAE` (protected), not `UAE`
The delinked TARGET obj symbols are `?on_*@collision_sensor@survarium@@MAEX...` (M = protected
virtual). The header had them under `public:` -> would mangle `U` -> unpairable. Moved them to a
`protected:` section. (They ICF-fold anyway so still None, but the access now matches the target.)

## Anchors added in temp_include_all.cpp
- `use_game_core_inventory_holder`: `concrete_inventory_holder` (overrides all 7 pure virtuals) ->
  `touch()` calls inventory()/scheduler() and escapes the results.
- `use_game_core_weapon_user_animations_selector`: null-ref `sel.set_animations(...)`.
- `use_game_core_base_project`: `concrete_base_project::touch()` calls both registrars.
- `use_game_core_booby_trap_core_get_speed`: PRIVATE virtual -> befriended free anchor (friend decl
  added to booby_trap_core.h, emits no bytes) + qualified devirtualized call on a null ref.
- `use_artefact_container_core`: changed `core.use_info(NULL)` (virtual) to the qualified
  `core.artefact_container_core::use_info(NULL)` to keep the standalone body.
All four new anchors dispatched from the aggregator (verified reachable: each in base rich index).
The ICF-fold overrides are NOT anchored (a folded body can't be made scorable); proven by family scan.

## Regressions
`report-changes.json`: 15 "regressed" / 21 "improved". ALL 15 regressions are ICF fold-representative
churn (boost `_bi::storage*`, particle/btHashMap/btPairSet dtors, game_console vcall thunks,
`empty_stub`) - none touch this batch's 10 units (verified by substring scan). The 21 improved include
base_project ctor/dtor/resolve_links/operator[] etc. that my anchor pulled into the EXE.
