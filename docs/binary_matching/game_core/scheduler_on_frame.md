# game_core::scheduler::on_frame(u32 frame_delta, u32 current_time)

Two-arg `on_frame` overload (the public per-frame entry point that iterates the
records). Target RVA `0x77de80`. The private `on_frame(record&, u32, u32)`
overload (RVA `0x0a8dd0`) is already `DONE` at 82.17% and is *not* in scope here.

## Target asm (rva 0x77de80)

```
0x09:  mov   [ebp-274h], ecx              ; this
0x0f:  mov   eax, [ebp-274h]              ; L49
0x15:  mov   dword ptr [eax+28h], 0       ; m_current_index = 0
0x1c:  jmp   short .2
.1:
0x1e:  mov   ecx, [ebp-274h]
0x24:  mov   edx, [ecx+28h]
0x27:  add   edx, 1                       ; ++m_current_index
0x2a:  mov   eax, [ebp-274h]
0x30:  mov   [eax+28h], edx
.2:
0x33:  mov   eax, [ebp-274h]
0x39:  add   eax, 10h                     ; &m_active_objects  (offset 0x10!)
0x3c:  call  vectora<record>::size        ; REAL CALL (not inlined)
0x41:  mov   ecx, [ebp-274h]
0x47:  cmp   [ecx+28h], eax               ; m_current_index < size
0x4a:  jae   short .3
0x4c:  mov   edx, [ebp+0Ch]               ; current_time  (L50)
0x4f:  push  edx
0x50:  mov   eax, [ebp+8]                 ; frame_delta
0x53:  push  eax
0x54:  mov   edx, [ebp-274h]
0x5a:  add   edx, 10h                     ; &m_active_objects
0x5d:  mov   ecx, [ebp-274h]
0x63:  mov   eax, [ecx+28h]               ; m_current_index
0x66:  call  vector<record>::operator[]   ; REAL CALL (not inlined)
0x6b:  push  eax
0x6c:  mov   ecx, [ebp-274h]
0x72:  call  scheduler::on_frame          ; on_frame(record&, frame_delta, current_time)
0x77:  jmp   short .1
.3:
0x79:  mov   edx, [ebp-274h]              ; L52
0x7f:  mov   dword ptr [edx+28h], 0FFFFFFFFh   ; m_current_index = u32(-1)
0x86:  mov   esp, ebp                     ; L53
0x89:  ret   8
```

Structure: L45 (prologue), L49 (for-init), L50 (body call), L52 (m_current_index=-1), L53 (close).

Note the frame is `sub esp, 274h` in target (huge) — the target inlined the
`vectora::size` return / temporary differently, but the *body* logic is the loop.

## Commands run (verbatim, in order)
- `pdb_rich_query --index binaries/rich/target/index.jsonl --function on_frame --list`
- `pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x77de80 --view target`
- `pdb_fetch --target-index ... --rva 0x77de80 --view structure,callees,info`
- `pdb_fetch --base-index binaries/rich/base/index.jsonl --rva 0x565440 --view base`
  (base = current STUB, RVA 0x565440)
- `report.json`: two-arg on_frame at **52.33%** before changes.

## Analysis of the 52% STUB
1. **Container bug (steerable):** STUB loop condition used
   `m_inactive_objects.size()` (offset 0x00, base reads `[this+4]-[this+0]`) but
   target reads `&m_active_objects` (offset 0x10) for the size call. Same for the
   index — target indexes `m_active_objects`. The STUB already indexed
   `m_active_objects` for the body, so only the *condition* used the wrong
   container. Fix: `m_active_objects.size()`.
2. **Inlining diff (NOT steerable, LTCG):** target makes real `call`s to
   `vectora<record>::size` and `vector<record>::operator[]`; base (under /Od)
   inlines both (`idiv 0x38` for size, `imul 0x38` for operator[]). This is an
   LTCG/inlining artifact between the two builds — cannot be forced from this
   function's source. sushi@NOTE already flags "size didn't inline in target".

## Iterations
1. INPUT: changed loop condition `m_inactive_objects.size()` -> `m_active_objects.size()`.
   BUILD #1: ran `rebuild.py game_core` (WRONG - module arg only builds the lib,
   did NOT relink the EXE; finished without relinking, report-changes showed +0.00/0
   changed; EXE timestamp unchanged at the previous day's link). Score stayed
   52.33% and base asm still read the old (m_inactive) code. See loop_performance.md.
   BUILD #2: re-ran `rebuild.py` (no arg, full EXE relink).
   SCORE: 52.33% -> **46.39%**.
   DIFF: base now reads `add ecx,10h` then size `[edx+4]-[eax]` of offset 0x10
   (correct member, was offset 0x00 before). But base still *inlines* size()
   (`cdq; idiv 0x38`) and operator[] (`imul 0x38; add`), whereas target makes
   real `call`s to `vectora<record>::size` (0x083010) and
   `vector<record>::operator[]` (0x082ed0). Confirmed via pdb_rich_query:
   `vectora<record>::size` exists out-of-line in TARGET but **does not exist** in
   BASE (fully inlined). That is the entire residual diff.

   (Note: `--view diff` for this overload can't be used - target sig is
   `on_frame(const unsigned int, const unsigned int)`, base is
   `on_frame(unsigned int, unsigned int)`; no common substring distinguishes it
   from the `record&` overload, so diff resolution fails. Used side-by-side
   `--view target` / `--view base` instead.)

## report-changes (full relink)
50 regressed / 50 improved, but 48 of each are `100.0<->0.0` COMDAT-folding churn
(destructors/ctors re-attributed by the delinker on any full relink, unrelated to
this one-line source change). Only real entry attributable here:
`scheduler::on_frame(u32,u32)` 52.33 -> 46.39 (plus one tiny render dtor 99.92,
also folding-adjacent). No genuine regression caused by the source change.

## Outcome
STATE[46.39%|PARTIAL]. The body is source-correct: correct member
(`m_active_objects`, offset 0x10), correct loop (`m_current_index` 0..size,
terminator `u32(-1)`), correct call `on_frame(record, frame_delta, current_time)`
with the right cdecl push order. The 52->46 drop is NOT a regression in
correctness - the old 52% matched a *wrong* member (m_inactive, offset 0x00) whose
shorter inlined form happened to score higher. Kept the correct member rather than
contort source to game the metric.
Blocker (LTCG, unsteerable): target out-of-lines and *calls*
`vectora<record>::size` + `vector<record>::operator[]`; base inlines both. The
emit-out-of-line-vs-inline choice for a trivial COMDAT template method is an LTCG
decision not controllable from this function's source.
Regressions caused: none (folding churn only).
Inlining: size()/operator[] inlined in base, out-of-line in target - the residual.

---

# game_core::scheduler::on_frame(record&, u32, u32) -- private overload

Structure-verifier pass (v2). Target RVA `0xa8dd0`, base RVA `0x574120`,
report.json fuzzy 82.17%.

## structure-diff (condensed)
```
target 25 / base 25 stmts
0x0df <0x11> | 0x0de <0x3f> | count = math::min( count, (s32)record.m_max_update_count );   SIZE
0x104 <0xe>  | 0x131 <0x17> | scheduler::callback callback = record.m_callback;   SIZE
; aligned 22, size-diffs 2, quantity-diffs 2
```
Plus two EMPTY-only (one base, one target) source-line gaps -- collapsed line
attribution, not control-structure divergence.

## Cause of the two SIZE diffs (--view diff, fuzzy 88.50%)
1. `count = math::min( ... )` -- target emits a `call` to an out-of-line
   `math::min<s32>`; our base INLINES the branchless min
   (`xor edx,edx; cmp; setl dl; neg edx; sub; and; add`). Pure LTCG inline-vs-call
   of a trivial template, same as the already-noted `math::floor`.
2. `scheduler::callback callback = record.m_callback;` -- target out-of-lines the
   `boost::function` copy-construct (`lea; call`), base does the longer inline
   `lea; call; push; lea; call` form. Again inline-vs-call at the COMDAT boundary.

These extra inlined temporaries also explain the frame delta (`push esi`,
`sub esp, 2D0h` base vs `2C0h` target) -- a downstream slot artifact, not a source
shape change. `[ebp-NN]` slot numbers throughout are allocation noise.

## Verdict
STRUCTURE MATCH. 25/25 statements aligned, all control structure (the two early
returns, the per-frame branch, the ASSERTs, the count loop) reproduced. Residual
is LTCG inline-vs-call only; non-steerable from this function's source. STATE
stays `82.17%|DONE`. No source logic changed; carcass replaced with the condensed
structure-diff embed.

---

# Assessment: landing register_for_update / unregister out-of-line (orchestrator note)

These two members are declared OUT-OF-LINE in `scheduler.h` (lines 57-66) but
currently DEFINED `inline` in `scheduler_inline.h` with `STATE[BLOCKED]`
(register_for_update at 0.0%, unregister at 0.0%/notes that everything inlined
differently). The orchestrator asked whether landing them out-of-line (moving the
definitions from `scheduler_inline.h` into `scheduler.cpp`) is doable here, since
that is what the target did and would also help `oxygen_tank::set_active`.

Decision: LEFT BLOCKED. Reasons:
- These are NOT matched bodies -- they are unverified skeletons that still carry
  carcasses and 0% match. Producing the out-of-line definitions is a genuine
  MATCHING task (write+verify each body), not a structure-verification.
- Both call `register_object()` and `objects()`, themselves `STATE[INLINED]`/
  `STATE[BLOCKED]` inline helpers in the same header. Pulling
  register_for_update/unregister out-of-line without first settling those helpers'
  inline-vs-out-of-line story risks changing how they inline at the OTHER call
  sites (register_on_frame, and the oxygen_tank path the note mentions) -- exactly
  the "header restructure that risks other units" the orchestrator said to avoid.
- This verifier pass is comment/marker-only by mandate; a header move + rebuild of
  the dependent TUs is a separate matcher unit.

Recommended next step (for a matcher, not this pass): take register_object/objects
first (settle their target inline/out-of-line form), then move
register_for_update + unregister definitions into scheduler.cpp and verify against
target rvas 0x92da0 (register_for_update) and 0x92de0 (unregister).
