# `member = T( args )`: construct temp, memberwise-copy data, destroy temp
tags: cpp:operator cpp:ctor | asm:lea asm:call asm:fld asm:fstp | topic:codegen-idiom topic:fold-icf
symptoms: lea call T::T then fld/fstp copies skipping +0 vtable then lea ecx call dtor
confidence: 8/10

Assigning a freshly-built temporary into a class-type member emits under /Od: construct
the temp at `[ebp-N]`, memberwise-COPY only the DATA members into the member (vtable at
+0 untouched - the implicit operator=), destroy the temp. The trivial dtor is
COMDAT-folded so its name differs base vs target (same folded empty fn).

```cpp
m_heel_interpolator = fermi_interpolator( time );
```
```asm
lea eax,[ebp-N]; call T::T            ; temp
fld [temp+4]; fstp [m_x+4]            ; data only - +0 vtable skipped
fld [temp+8]; fstp [m_x+8]
lea ecx,[ebp-N]; call <folded dtor>   ; misnamed (finalize_impl / dummy::nonnull)
```
Residual: a `lea ecx` for the folded dtor's `this` on one side only = permitted ICF/LTCG arg-passing (mark DONE ~98-99%). To even compile, the member class needs NON-const data + accessible operator= - check the PDB ground-truth structure header (binaries/structure/target/headers) for real const-ness/access.
Evidence: legs_ik_processor::set_{heel,toe}_on_ground 98.84/98.59% (working fermi_interpolator.h wrongly had const members + private op=).
