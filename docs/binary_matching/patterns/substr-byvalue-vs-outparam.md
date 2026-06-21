# `dest = s.substr(pos,n)` (by-value temp + memcpy) vs `s.substr(pos,n,&dest)` (in-place)
tags: cpp:string cpp:template cpp:call | asm:lea asm:call asm:memcpy | topic:inline-vs-call topic:structure-shape
symptoms: extra N-byte stack temp, call buffer_string::substr then a separate memcpy into dest, clear-then-copy (mov byte[dest],0)

buffer_string/fixed_string expose TWO substr overloads and they compile to very
different shapes - pick by the asm, not by which reads cleaner:

- `void substr(pos, n, buffer_string* out_dest) const` (out-param): ONE `call
  buffer_string::substr` writing straight into `out_dest`. No extra buffer, no
  memcpy. Small statement.
- `fixed_string substr(pos, n = npos) const` (by-value return): builds a FULL
  `fixed_string<N>` temp on the stack (frame grows by ~N), calls
  `buffer_string::substr` into the temp, then the trailing `dest = temp` runs
  `fixed_string::operator=` inlined = clear dest (`mov byte ptr [dest],0`) +
  `memcpy(dest.begin, temp.begin, len)` + null-terminate.

TELL: a `call buffer_string::substr` IMMEDIATELY followed (same source statement)
by `mov byte ptr [dest],0` and a `call memcpy`, plus a 2nd N-byte region in the
frame, is the BY-VALUE form. Writing the out-param form (`substr(0,n,&dest)`)
leaves the substr statement ~0x70 bytes too small and drops the % hard
(create_network_client: out-param 58% -> by-value 86%).

```cpp
fixed_string< 512 > host;
host = m_network_client_options.substr( 0, offset );   // by-value: temp + memcpy assign
```
```asm
lea  eax,[esp+234h]                 ; temp fixed_string<512>
... push 0; push &options; mov edx,offset; call vostok::buffer_string::substr
mov  eax,[&host]; mov byte ptr [eax],0      ; host.clear (operator=)
mov  eax,[temp.end]; sub eax,temp.begin     ; len
push len; push temp.begin; push host.begin; call memcpy
mov  byte ptr [host.begin+len],0            ; null-terminate
```
Evidence: survarium::game::create_network_client (game.cpp, 0x5e6f70).
