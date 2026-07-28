# A `{ }` block around a non-trivial-dtor local moves its `call ~T` to the block's `}`
tags: cpp:dtor cpp:local | asm:call asm:lea | topic:structure-shape
symptoms: call ~T emitted BEFORE a following statement (not at function end), dtor-call position shifts with the brace
confidence: 9/10
variants: braces-single-statement-invisible.md, sibling-braced-scopes.md

The case where braces DO show. A local with a non-trivial dtor is destroyed at the end of
its enclosing scope, so an inner `{ }` makes `call ~T` fire at the inner `}` — BEFORE the
statements after the block — instead of at function end. The dtor-call POSITION relative to
the surrounding statements is the tell. Probe-verified:

```cpp
void noblock( ) {   Lock l;   g = 1; }   // ~Lock at function end -> AFTER  g=1
void block  ( ) { { Lock l; } g = 1; }   // ~Lock at the inner }  -> BEFORE g=1
```
```asm
?noblock@@YAXXZ PROC                      ?block@@YAXXZ PROC
  8d 4d ff  lea ecx,[l]                     8d 4d ff  lea ecx,[l]
  e8 ..     call ??0Lock@@QAE@XZ            e8 ..     call ??0Lock@@QAE@XZ   ; ctor
  c7 05 .. 01.. mov g,1                     8d 4d ff  lea ecx,[l]
  8d 4d ff  lea ecx,[l]                     e8 ..     call ??1Lock@@QAE@XZ   ; ~Lock at the }
  e8 ..     call ??1Lock@@QAE@XZ            c7 05 .. 01.. mov g,1            ;   ...then g=1
;          ^ ~Lock AFTER g=1               ^ ~Lock BEFORE g=1
```
Steerable: an early `call ~T` sitting between two body statements = an inner lexical block closing there; add/remove the `{ }` to move it. (Trivial-dtor types show nothing — braces-single-statement-invisible.md.)
