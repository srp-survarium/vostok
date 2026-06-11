# `if ( m_fn )` on boost::function: safe-bool call vs inlined sbb/not/and is per-INSTANTIATION (the function0 wall)
tags: cpp:template cpp:if cpp:bool | asm:call asm:sbb asm:test | topic:inline-vs-call
symptoms: call operator! fold then movzx neg sbb not and eax &dummy::nonnull reloc, function0 sites only, 75.26%
confidence: 8/10
variants: boost-function-operator-call-per-instantiation.md, inline-vs-call-template-comdat.md

`if ( m_fn )` lowers either to ONE call of the out-of-line safe-bool COMDAT
(`operator void (dummy::*)()`, flag in eax; `test eax,eax`) or to the INLINED safe-bool
body: `call <operator! fold>; movzx; neg; sbb; not; and eax, <&dummy::nonnull reloc>`.
Which form an INSTANTIATION gets is a whole-program LTCG choice: in the same target TU
function1/function2 sites use the call form (matched 100% from plain `if ( m_fn )`)
while function0 sites are inlined - and the base made the opposite choice for function0
only. Don't respell the positive test; bank the function0 residual.

```cpp
if ( m_on_connected )         // positive test: not steerable per instantiation
	m_on_connected( );

if ( !m_fn )                  // DISTINCT and steerable: operator! COMDAT + test/je
	return;                   // (no and-with-constant)
```
Evidence: tcp on_connected_impl/on_disconnected_impl stuck 75.26% (function0); `if ( !m_fn ) return;` matched 100% across all on_X forwarders.
