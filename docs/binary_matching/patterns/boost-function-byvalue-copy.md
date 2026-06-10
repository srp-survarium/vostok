# By-value boost::function argument: target copy-ctor (1 call) vs base default+assign_to_own (2)
tags: cpp:template | asm:call asm:sub-esp | topic:inline-vs-call topic:convention
symptoms: call function<>::function<> then call assign_to_own, sub esp 20h vs 14h, extra [ebp-10h] slot
confidence: 8/10
variants: boost-function-assign-inline.md

Passing a boost::function BY VALUE constructs a temporary copy: target copy-constructs in
ONE call; base default-constructs then calls `assign_to_own` (extra call + slot enlarge
the frame and shift slot numbers). The choice lives inside boost::function's own header
inlining; by-value is the only source shape. Statement count matches; the lone SIZE row
is this call. Non-steerable; mark PARTIAL.

```cpp
body_part->dump_state( callback, index++ );   // dump_state takes boost::function<...> by value
```
```asm
; target: sub esp,20h; mov eax,esp; lea ecx,[ebp+8]; call function<>::function<>   ; copy ctor
; base:   ... call function<>::function<> (default); lea ecx,[ebp+8]; push ecx; call assign_to_own
```
Evidence: damage_model::dump_stats (79.26%, 7/7 stmts).
