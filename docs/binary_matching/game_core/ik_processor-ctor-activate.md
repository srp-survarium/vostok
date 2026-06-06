# game_core::ik_processor {ctor, activate}

Unit: `survarium::ik_processor::ik_processor()` (target rva 0x57eba0) and
`void survarium::ik_processor::activate(vostok::animation::skeleton const&)`
(target rva 0x57eb80). Same class, both tiny; matched as one unit/PR/rebuild.

## Target asm

### ctor (0x57eba0), mangled `??0ik_processor@survarium@@IAE@XZ` (IAE = protected)
```
0x00: push  ebp
0x01: mov   ebp, esp
0x03: push  ecx
0x04: mov   [ebp-4], ecx
0x07: mov   eax, [ebp-4]
0x0a: call  ...fixed_size_allocator<...>::finalize_impl   ; delinker misname (empty/folded)
0x0f: mov   eax, [ebp-4]
0x12: mov   dword ptr [eax], 0      ; m_skeleton = 0        (offset 0x0)
0x18: mov   ecx, [ebp-4]
0x1b: mov   dword ptr [ecx+4], 0    ; m_last_time_in_ms = 0 (offset 0x4)
0x22: mov   eax, [ebp-4]            ; return this
0x25: mov   esp, ebp
0x27: pop   ebp
0x28: ret
```
Carcass: whole body at line 16 (`{`), close at line 17 (`}`). The pre-store
`call ...finalize_impl` at 0x0a is NOT an ASSERT: an ASSERT emits
`mov byte[ebp-N],0; lea eax,[ebp-N]; call finalize_impl` (~0x0c bytes, consumes a byte
slot - see booby_trap_core::load 0x09..0x44), but here it is `mov eax,[ebp-4]; call`
with NO byte-local prefix. The tell: `pdb_rich_query --function noncopyable::noncopyable`
gives `vostok::core::noncopyable::noncopyable()` at rva **0x03f210** and
`fixed_size_allocator<...>::finalize_impl()` is ALSO at **0x03f210** - they are
COMDAT-folded to the same address, and the delinker just printed the finalize_impl name.
So this `call` IS the `core::noncopyable` BASE CTOR, emitted out-of-line because
noncopyable has its own ctor symbol (folded). Same class as the assembly_patterns
"base-ctor call is a folded out-of-line symbol both binaries call" corollary.
=> source is a plain ctor deriving from noncopyable with a member-init list
`: m_skeleton( NULL ), m_last_time_in_ms( 0 )` (base call then the two zero stores).

### activate (0x57eb80), mangled `?activate@ik_processor@survarium@@IAEXABVskeleton@animation@vostok@@@Z` (IAE = protected)
```
0x00: push  ebp
0x01: mov   ebp, esp
0x03: push  ecx
0x04: mov   [ebp-4], ecx
0x07: mov   eax, [ebp-4]
0x0a: mov   ecx, [ebp+8]   ; &skeleton (the reference arg)
0x0d: mov   [eax], ecx     ; m_skeleton = &skeleton  (offset 0x0)
0x0f: mov   esp, ebp
0x11: pop   ebp
0x12: ret   4
```
=> `m_skeleton = &skeleton;`. Single statement, line 21.

## Commands run (verbatim, in order)
- pdb_rich_query --index binaries/rich/target/index.jsonl --function "ik_processor::ik_processor" --list
- pdb_rich_query --index binaries/rich/target/index.jsonl --function "ik_processor::activate" --list
- pdb_fetch --target-index ... --function "ik_processor::ik_processor" --rva 0x57eba0 --view target/info/callees
- pdb_fetch --target-index ... --function "ik_processor::activate" --rva 0x57eb80 --view target/info/callees
- COFF symbol parse of binaries/objdiff/target/.../ik_processor.cpp.obj -> both IAE (protected)
- python3 scripts/rebuild.py   # NO module arg

## SHAPE decisions before build
- Visibility: both `IAE` = protected. Header changed from `public:` to `protected:`.
- ctor: ASSERT( UNKNOWN_EXPRESSION ) first (the pre-store no-arg call), then member
  assignments in body order (m_skeleton, m_last_time_in_ms).
- activate: `m_skeleton = &skeleton;`.
- Anchor: ctor+activate are protected -> a `concrete_ik_processor : ik_processor`
  subclass in temp_include_all's `use_game_core_ik_processor`; construct it (keeps
  the protected ctor), call activate via a derived `do_activate`, escape `&proc`.

## Iterations
1. INPUT: ctor = ASSERT( UNKNOWN_EXPRESSION ) + 2 body assignments; activate =
   m_skeleton=&skeleton.  ABANDONED before build: the pre-store call is the folded
   noncopyable base ctor (rva 0x03f210), not an ASSERT (wrong byte shape).
2. INPUT: ctor = member-init list `: m_skeleton(NULL), m_last_time_in_ms(0)` empty body
   (base ctor auto-emitted by deriving noncopyable); activate = `m_skeleton = &skeleton;`.
   Header: both methods moved to `protected:`.
   Anchor: `concrete_ik_processor : ik_processor` in use_game_core_ik_processor;
   construct it (keeps protected ctor), call activate via derived do_activate, escape &proc.
   BUILD (first attempt): BLOCKED - `python3 scripts/rebuild.py` failed with
   `error: writing to file: No space left on device`. `/nix` store was 100% full.
   So this could not be built or scored on the first attempt.
3. INPUT: unchanged from iter 2 (the source/header/anchor were already correct).
   BUILD: /nix freed (83G avail). `python3 scripts/rebuild.py` (NO module arg) ran to
   completion (exit 0, full EXE relink + base structure/COFF/rich refresh).
   SCORE (binaries/objdiff/report.json, fuzzy_match_percent):
     - `??0ik_processor@survarium@@IAE@XZ`                                   -> 100.0
     - `?activate@ik_processor@survarium@@IAEXABVskeleton@animation@vostok@@@Z` -> 100.0
   report-changes.json: BOTH symbols appear in `improved` (0.0 -> 100.0); neither in
   `regressed`. The ctor's folded `core::noncopyable` base call reproduced exactly - no
   ASSERT needed, confirming the iter-1 analysis.

## Outcome
STATE[100%|DONE] for BOTH ctor and activate. Verified by a full relink:
report.json fuzzy_match_percent = 100.0 for each, and both show as improved
(0.0 -> 100.0) in report-changes.json.
Regressions caused: NONE on any matched unit. The 53 `regressed` entries are entirely
the known symmetric delinker re-slice churn that flips COMDAT-folded helpers 100<->0
when the EXE is relinked (28 scalar-deleting-dtors, 6 thunk/vcall, 4 resource_ptr,
3 dtors, bullet/boost/empty_stub, plus intrusive_ptr/interlocked/speedtree_data ctor/dtor
and an unmatched buffer_string::operator=); they are balanced by 65 improved and none is
an ik_processor / get_bone_matrix / legs_ik / player_logic / player_stealth function.
Inlining: none. Cluster: ctor + activate matched together (same class, trivial).
Carcass deleted (clean 100% DONE on both); source kept only the two `// STATE[100%|DONE]`
lines + member-init list / single store.
