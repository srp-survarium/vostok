# Two trivial accessors in ONE `&&` can split inline-vs-call (one matches, one residual)
tags: cpp:inline cpp:if cpp:bool | asm:call asm:mov | topic:inline-vs-call
symptoms: one getter call only on target side inside a && chain, [ebp-N] slot shift
confidence: 8/10
variants: inline-vs-call-trivial-getter.md

In `if ( a() && getter_x() && getter_y() )` with both getters one-line header accessors,
the target's linker may keep ONE out-of-line (standalone in target index) while inlining
the other; our /GL inlines BOTH. The inlined-on-both-sides getter matches byte-for-byte;
the target-kept-standalone one is the lone residual. Per-method whole-program decision -
mark the caller PARTIAL.

```asm
; matching:  mov al,[this+48Fh]            ; inlined both sides
; residual:  call ...::round_is_chambered  ; target standalone @0x09b360, base inlines mov cl,[+48Eh]
```
Confirm which is the residual by querying BOTH rich indexes per accessor (target standalone + base absent = the diverger).
Evidence: weapon_core_reload_state_base::initialize 92% (!deserializing() && chamber_a_round_on_reload() && round_is_chambered()).
