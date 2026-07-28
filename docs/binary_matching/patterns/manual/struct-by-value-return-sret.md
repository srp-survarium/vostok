# Returning a struct by value: >8 bytes = hidden return-pointer (sret); <=8-byte POD = `edx:eax`
tags: cpp:return cpp:member | asm:lea asm:push asm:call asm:mov | topic:convention
symptoms: lea <slot>/push it as an extra arg before the call + result read through eax (sret), or mov eax/mov edx after the call for an 8-byte POD, add esp counts the hidden arg
confidence: 8/10
variants: call-convention-cdecl-vs-thiscall.md

How a by-value struct return is passed. A struct **larger than 8 bytes** uses **sret**: the
caller allocates the result slot and passes its **address as a hidden extra argument**
(pushed last, so it is the first parameter); the callee fills it and returns the pointer in
`eax`. A **POD of 8 bytes or less** is returned in the **`edx:eax`** register pair (just
`eax` for <=4). The hidden-pointer push + reading the result through `eax` is the sret tell;
the `add esp,N` after the call counts the hidden arg. Probe-verified:

```cpp
struct Big { float x,y,z; };  Big   makeBig( float );    // 12B -> sret
struct Small { int a,b; };    Small makeSmall( int );    // 8B  -> edx:eax
void useBig  () { Big   r = makeBig( 1 );   gB = r.x; }
void useSmall() { Small r = makeSmall( 2 ); gS = r.a; }
```
```asm
?useBig@@YAXXZ PROC                          ?useSmall@@YAXXZ PROC
  ... push the float arg ...                   6a 02 push 2
  8d 45 d8 lea eax,$T2566   ; result slot      e8 .. call ?makeSmall@@YA?AUSmall@@H@Z
  50       push eax         ; HIDDEN ret ptr    83 c4 04 add esp,4
  e8 .. call ?makeBig@@YA?AUBig@@M@Z            89 45 f0 mov $T,eax    ; low  dword
  83 c4 08 add esp,8        ; arg + hidden ptr  89 55 f4 mov $T+4,edx  ; high dword
  8b 08 mov ecx,[eax] ...   ; copy 12B via eax  ; (no hidden pointer)
```
Steerable: a `lea <slot> / push` feeding the call with the result then read through `eax` = an sret struct return (>8B); a `mov $T,eax / mov $T+4,edx` pair after the call = an 8-byte POD return. Mangling carries the return type (`?AUBig@@`).
