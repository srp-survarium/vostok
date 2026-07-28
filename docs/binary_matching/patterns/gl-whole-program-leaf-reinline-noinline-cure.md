# /GL re-inlines an out-of-line VOSTOK_*_API leaf into ~all callers: noinline+dllexport+Oy-off cure
tags: cpp:inline cpp:call cpp:intrinsic | asm:call asm:lock asm:push-ebp | topic:inline-vs-call topic:whole-program topic:hack topic:frame-pointer
symptoms: a small `VOSTOK_CORE_API` (etc.) leaf — `interlocked_or`/`interlocked_and` (a `lock cmpxchg` retry loop) — that the TARGET out-lines at ~all call sites (`call ?interlocked_or@...`) is RE-INLINED by our `/GL` whole-program build into `lock or [m],r` / `lock and [m],r` at every consumer; the standalone symbol stays UNPAIRED at a tiny custom-calling-convention size (15B, args in edx/ecx, no frame) vs the target's 25B standard frame (`push ebp; mov ebp,esp; push esi; ... [ebp+8]/[ebp+0Ch]`)
confidence: 8/10

## What you see
- A target consumer emits `call vostok::threading::interlocked_or` (a reloc to `?interlocked_or@threading@vostok@@YAJACJJ@Z`); the base consumer emits an inline `lock or [ecx], eax` (or `lock and`). Whole-engine counts: target keeps a HANDFUL inline (here 5 `or` + 2 `and`) and out-lines the rest (56 `or` + 47 `and` call sites); base inlines ~all (~55 `or` + ~42 `and`, 0 calls).
- The standalone out-of-line definition exists in BOTH indexes but is reported unpaired (`fuzzy None`): base is ~15B with a `/GL` custom calling convention (args arrive in `edx`/`ecx`, no `push ebp`), target is 25B standard `__cdecl` with a real frame.
- `interlocked_increment`/`interlocked_decrement` are NOT affected — they lower to `lock xadd` (a real x86 intrinsic) inline on BOTH sides, correctly.

## Cause
The leaf is declared `VOSTOK_CORE_API` (which, in a `VOSTOK_STATIC_LIBRARIES` build, expands to nothing) and defined out-of-line in a `.cpp`. Under `WholeProgramOptimization="1"` (`/GL` + LTCG) MSVC is free to (a) re-inline the body into every caller and (b) give the surviving standalone a custom register-passing convention and omit its frame (the TU is also `OmitFramePointers="true"` = `/Oy`). The target was built so the leaf stayed out-of-line with the standard ABI — i.e. it carried an anti-`/GL` attribute and kept its frame pointer.

## Cure (3 levers on BOTH the declaration and the definition)
1. **`__declspec(noinline)`** — forces every caller to emit `call` instead of inlining the body. This is the orchestrator-sanctioned hack (packet_reader::r<T> precedent, +25 fns); it recovers the target CODEGEN, not necessarily the original source spelling — mark `// sushi@TODO: remove ... revisit if a faithful cause is found` and add a `review_todos.md` row.
2. **`DISABLE_LINKER_GL`** (= `__declspec(dllexport)`, the engine's OWN anti-`/GL` idiom — see `math::mul4x3` in `math_float4x4.{h,cpp}`, `float3_pod::normalize`, `float4x4::identity`) — restores the standard `__cdecl` (args from `[esp+...]`), bringing the standalone 15B -> 23B. FAITHFUL: this device is already used across the math headers, so prefer it over a second hack.
3. **`#pragma optimize( "y", off )`** around the definitions — restores the frame pointer that the TU's global `OmitFramePointers="true"` (`/Oy`) strips, bringing 23B -> 25B (== target: `push ebp; mov ebp,esp; ... [ebp+8]/[ebp+0Ch]; pop ebp; ret`). Needed because you must NOT touch the `.vcproj`; the pragma is the per-function expression of `/Oy-`.

Apply on the declaration too (cross-TU `/GL` must see `noinline`/`dllexport` at the call site to honour them).

## Result / residual
- ~100 caller sites flip from inline `lock or`/`lock and` to `call` — matching the target's out-of-line strategy. Big net-positive fuzzy across the consumers.
- The standalone body is byte-correct (the `lock cmpxchg` loop) but may not fully PAIR: (a) a `push esi` SCHEDULE diff (target pushes the callee-saved reg right after the frame, base pushes it after loading the args — optimizer scheduling, not source-steerable; ~85.7%); and (b) a COMDAT-PLACEMENT mismatch — the target attributes the COMDAT to the consuming HEADER (here `threading_policies.h:128/131`, the `operator_or`/`operator_and` instantiation point), implying the original emitted these as header-instantiated COMDATs, while the base emits them from the `.cpp` (its own obj). objdiff pairs within a unit, so base+target land in different units and report `None`. To make them PAIR, move the definitions into a header included by the consumers (deferred — ODR/dllexport risk).
- The handful of sites the target KEEPS inline (5+2 here) the base now over-out-lines — a minor residual at those specific consumers.

## When to reach for this
Only when ONE inline leaf caps 30+ functions across the engine (orchestrator-driven; not in the per-function matcher loop). Confirm the wall is a single shared leaf (count `IMAGE_REL_I386_REL32 ?<leaf>@` relocs vs inline occurrences via `llvm-objdump -d -r --triple=i386-pc-win32` over `binaries/objdiff/{base,target}`). Leave `lock xadd` intrinsics (increment/decrement) untouched.
