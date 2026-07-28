# `boost::functionN::operator()` body = empty-guard + bad_function_call throw + manager indirection (`and eax,~1; call [eax+4]`)
tags: cpp:template cpp:operator | asm:cmp asm:and asm:call | topic:codegen-idiom
symptoms: cmp dword[esi],0 / jne, call boost::bad_function_call, throw_exception, mov eax,[esi]; and eax,0FFFFFFFEh; call [eax+4]; add esi,8; ret 4*N
confidence: 9/10

The out-of-line `operator()` first checks the empty functor (`cmp [esi],0; jne`) and on empty calls
`bad_function_call` ctor + `boost::throw_exception` — this throw path is emitted **even with
exceptions off** (the STLport/MSVC8 build). On the live path it loads the manager pointer
(`mov eax,[esi]; and eax,0FFFFFFFEh` strips the tag bit), reads the invoker at `[eax+4]`, then
`add esi,8; push esi` (the functor buffer = arg0 after the N user args) and `call`. Body is `ret 4*N`.

```cpp
m_callback( *notification );   // boost::function1<void, ...>::operator()
```
```asm
cmp   dword ptr [esi], 0          ; empty-functor guard
jne   short ..
call  boost::bad_function_call::bad_function_call   ; thrown even w/ EH off
mov   eax, [esi] / and eax, 0FFFFFFFEh   ; strip tag bit
mov   edx, [eax+4]               ; -> the invoker
push  ecx                        ; user arg (function1)
add   esi, 8 / push esi          ; -> functor buffer
call  edx
ret   4
```
Steerable from a plain `m_cb(args)` member invoke (boost-function-member-invoke.md). cite: target boost::function1<void,survarium::game_object_&>::operator(); call site watcher.cpp:58.
