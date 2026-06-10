# Serial-number (RFC1982) operator< / operator<= / operator-
tags: cpp:operator | asm:cmp asm:jcc | topic:codegen-idiom
symptoms: two-clause OR with +0x8000, jge vs jg one-byte diff, +0x10000 & 0x8000FFFF
confidence: 8/10

Wrap-around sequence-number compare is a two-clause OR; `operator<=` differs by exactly
one byte (first clause jge -> jg). The `+0x10000 & 0x8000FFFF` + sign-extend in
operator- IS the compiler's `(s16)` cast, not extra logic.

```cpp
// a < b:
( a < b && u32( a ) + 0x8000 > b ) || ( b < a && u32( b ) + 0x8000 <= a )
// operator-(left,right):
right <= left ? s16( left.m - right.m ) : -( right - left )
```
Evidence: network_core sequence_number unit.
