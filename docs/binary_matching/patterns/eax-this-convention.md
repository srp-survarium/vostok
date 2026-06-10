# LTCG this-in-EAX frameless callee = the target kept it out-of-line; do NOT escape its address
tags: cpp:member | asm:call asm:mov | topic:inline-vs-call topic:convention
symptoms: add eax 120h call reset, callee no push ebp reads [eax]/[eax+4], xorps frameless body
confidence: 4/10
variants: inline-vs-call-trivial-getter.md, optimized-comdat-in-od-unit.md

A member call appearing as `add eax,<off>; call <callee>` with `this` in EAX and a
frameless callee is an LTCG convention applied ONLY to functions kept out-of-line
whole-program - seeing EAX-this proves the target kept the callee standalone while our
build inlines the tiny body (frame grows, slots shift). VERIFIED INEFFECTIVE (PR #124,
weapon_core_aimed_state_base::finalize -> animation_playback_state::reset): decl/def
split into the class's own header-TU; __declspec(noinline); multiple real same-module
callers. CRITICAL: do not escape `&callee` through a sink - a member-fn-ptr forces
__thiscall ECX + a full frame (diverges MORE) and still gets inlined.

```asm
add eax,120h; call reset            ; this in EAX, no lea ecx
; callee: xorps xmm0,xmm0; mov dword[eax],0; movss [eax+4],xmm0; ret   ; frameless
```
The closest reproducible shape is an EMPTY-stub callee (lets /Od elide the call cleanly); genuine inline-vs-call residual, caller PARTIAL.
evidence-basis: negative result; all steering attempts verified ineffective (PR #124)
