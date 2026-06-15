# Each out-of-line `boost::functionN::operator()` call SITE reserves 0x12C of dead caller frame
tags: cpp:template cpp:member | asm:sub-esp asm:call | topic:convention
symptoms: sub esp 388h, frame absurdly large next to the visible locals, +0x12C per operator() call
confidence: 8/10
variants: boost-function-operator-call-per-instantiation.md

Under /Od+/GL each out-of-line `boost::functionN::operator()` call site adds exactly
0x12C bytes of dead caller frame (machinery temps allocated, never written). Frame
reads: 4 (`this` spill) + named locals/temps + 0x12C * (operator() call sites). The base
build reproduces it from plain source - do NOT hunt for a missing 300-byte local when
`sub esp` looks absurdly large next to the visible locals.

```cpp
m_functor2( m_string0, m_string1, m_string2 );   // a plain functor invocation
```
```asm
sub  esp, 388h                            ; string_response::execute: this + 3 call sites
call boost::function3<...>::operator()    ; x3
```
Evidence: string_response::execute 3 calls = 0x388; receive_response::execute 1 call + 0xC reader locals = 0x13C; receive_udp_response::execute 1 call + extra stats stmts = 0x16C.
