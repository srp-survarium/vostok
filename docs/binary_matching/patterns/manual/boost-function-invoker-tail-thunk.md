# `void_function_obj_invoker0<bind_t<...>>::invoke` = a frameless 3-insn tail thunk `mov eax,[esp+4]; mov ecx,[eax]; jmp ecx`
tags: cpp:template | asm:mov asm:jmp | topic:fold-icf
symptoms: mov eax,[esp+4]; mov ecx,[eax]; jmp ecx, void_function_obj_invoker0, static invoke(function_buffer&), no frame
confidence: 9/10

The static invoker that `boost::function::operator()` reaches through `[eax+4]`
(boost-function-operator-call-body.md) is a frameless 3-instruction tail thunk: read the
`function_buffer&` arg, load the stored functor's first field (the `mfN` code ptr / fn ptr), and
`jmp` to it. It is parameterized on `bind_t<...>` so there are dozens; most ICF-fold, so the
delinker prints whichever fold rep it picked — a name mismatch here is noise, not a bug.

```asm
mov   eax, [esp+4]    ; function_buffer&
mov   ecx, [eax]      ; stored functor field 0
jmp   ecx             ; tail-call into the bound mf thunk
```
Read the `bind_t<...,mfN<...>,listN<...>>` in the symbol to know which callback this dispatches; ICF folds collapse equal-shaped ones. cite: target boost::detail::function::void_function_obj_invoker0<bind_t<void,...,list0>,void>::invoke (97 fold matches).
