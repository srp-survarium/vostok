# `inline` free fns and class-body members DO inline under /Od /Ob2 — only out-of-line defs emit a `call`
tags: cpp:inline cpp:member | asm:call asm:imul asm:mov | topic:inline-vs-call topic:codegen-idiom
symptoms: a small accessor/helper has NO call site (body spliced in), only separately-defined functions show call ?fn@@..; /Ob2 overrides /Od for inline-keyword and in-class definitions
confidence: 9/10
variants: two-accessors-one-and-split.md, address-taken-anchor-emits-header-comdat.md

`/Od` alone disables inlining, but the engine modules pair it with `/Ob2`
(`InlineFunctionExpansion="2"`), which DOES expand functions that are either `inline`-keyword
or defined in the class body. So a trivial getter/helper is spliced into its caller with NO
`call`; only a function DEFINED OUT OF LINE (declared, body elsewhere) emits a real `call`.
Probe-verified — `sq` and a class-body `get()` inlined, the extern `freefn` did not:

```cpp
inline int sq( int x ) { return x*x; }
struct C { int v; int get() const { return v; } };
void call_inline ( int x ) { g = sq( x ); }    // inlined
void call_member ( C* c )  { g = c->get(); }    // inlined
void call_outline( int x ) { g = freefn( x ); } // out-of-line -> real call
```
```asm
?call_inline@@YAXH@Z PROC          ?call_member@@YAXPAUC@@@Z PROC     ?call_outline@@YAXH@Z PROC
  8b 45 08 mov eax,[x]               8b 45 08 mov eax,[c]              8b 45 08 mov eax,[x]
  0f af 45 08 imul eax,[x] ; x*x     8b 08    mov ecx,[eax] ; c->v     50 push eax
  a3 .. mov g,eax  ; NO call         89 .. (temp) ; NO call            e8 .. call ?freefn@@YAHH@Z
                                                                       83 c4 04 add esp,4
```
Wall (mostly): a small helper with no call = inlined; do NOT try to match it as a standalone function (it has no body in the consumer's obj). **LTCG (`/GL`) inlines even MORE** — across TUs, including non-`inline` functions — so the shipped build hides yet more callees; this `/Ob2` set is the compile-time lower bound (see the topic:inline-vs-call family for the LTCG-driven cases).
