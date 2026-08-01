# The entry guard vanished from the callee - LTCG partial-inlined it into the caller

tags: cpp:if asm:cmp asm:je | topic:inlining topic:ltcg topic:diagnosis

## Symptom
The target opens with a file-scope debug-switch guard

```
0x06:  cmp   byte ptr [s_draw_grass_debug_value], 0
0x0d:  push  ebx / ebp / esi / edi
0x11:  je    .exit
```

your source has exactly that `if ( !s_x ) return;`, the function still builds its
whole body - and yet the base has **no `cmp` at all**, is ~0xd bytes short, and
its prologue interleaves the register pushes with the first real statement.

## Diagnosis
Disassemble the **caller**, not the callee. MSVC /GL performs *partial inlining*:
it hoists a cheap entry test out of a single-call-site callee and leaves the
callee body guard-free.

```
; base renderer::draw_debug
0x386:  mov   ecx, [edi+3BCh]
0x38c:  test  ecx, ecx
0x38e:  je    short .17
0x390:  cmp   byte ptr [s_draw_grass_debug_value], 0   <- the callee's guard, hoisted
0x397:  je    short .17
0x399:  call  vostok::render::grass_world::render_debug
```

The target's caller has only `mov/test/je; call` - it did not partial-inline.
Confirm by fetching the caller on both sides:

```
pdb_fetch --target-index ... --function <caller> --view target | grep -B4 <callee>
pdb_fetch --base-index   ... --function <caller> --view base   | grep -B4 <callee>
```

## What it is NOT
It is not a missing guard, a DCE'd `cc_bool`, or a wrong polarity. Do **not**
"fix" it by deleting the guard, inverting it, or wrapping the body in
`if ( s_x ) { ... }` - the callee source is already faithful and the CFG of both
spellings is identical, so the pass fires either way.

## Cost and disposition
Two-sided: the callee loses the `cmp`/`je` (and gets a different prologue and
frame layout, which shows up as register-allocation drift throughout), and the
caller's guarded statement grows by the hoisted test. Bank it as an LTCG
inlining wall with a `claude@NOTE` on BOTH sides; it may dissolve on its own once
the caller grows (more of its callees get bodied) and the inliner's budget
changes.

## Related
- `entry-guard-is-file-scope-debug-switch.md` - recovering the guard in the first
  place.
- `dced-static-helper-anchor.md` - the other direction, where the callee really
  is gone.
