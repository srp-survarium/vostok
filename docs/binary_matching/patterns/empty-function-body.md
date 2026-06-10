# Empty function body: the bare 5-byte /Od frame
tags: cpp:return | asm:push asm:ret | topic:codegen-idiom
symptoms: push ebp mov ebp esp pop ebp ret, 5 bytes
confidence: 9/10

An empty `void f( ) { }` is the bare /Od frame, byte-identical for every empty __cdecl
function. It DOES get its own standalone symbol and its own unit row in objdiff/report;
/OPT:ICF may fold byte-identical bodies but the symbol still resolves - do not assume an
empty function is unscorable.

```cpp
void survarium::game_core_initialize( ) { }
```
```asm
push ebp
mov  ebp, esp
pop  ebp
ret
```
Evidence: game_core/sources/entry_point.cpp, symbol ?game_core_initialize@survarium@@YAXXZ.
