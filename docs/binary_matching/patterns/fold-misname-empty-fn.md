# rva 0x03f210 is the universal empty-function fold target: a call there is whatever empty fn the source needs
tags: cpp:ctor cpp:assert | asm:call asm:mov | topic:fold-icf
symptoms: call finalize_impl, call empty_stub, several unrelated empty functions at rva 0x03f210
confidence: 9/10
variants: assert-eater-recover.md, icf-fold-names-noise.md, float4x4-identity-temp.md, xyz-fold-misname.md

/OPT:ICF folds every byte-identical empty `{}` body to one address (0x03f210 holds at
least core::noncopyable::noncopyable(), fixed_size_allocator<...>::finalize_impl(),
ai::fsm_state::~fsm_state()); the delinker prints whichever name it picked. Never take
the name literally - identify the empty function the SOURCE needs (usually an out-of-line
base ctor or base dtor). Read the byte PREFIX to tell ASSERT vs base-ctor fold:

```asm
mov byte[ebp-N],0; lea eax,[ebp-N]; call <fold>   ; ASSERT eater (~0xc, consumes a byte slot)
mov eax,[ebp-4]; call <fold>                       ; bare base-ctor fold (this, no byte local)
```
Evidence: ik_processor::ik_processor() - pre-store `call ...finalize_impl` @0x0a was the folded core::noncopyable base ctor (plain member-init-list ctor deriving noncopyable); player_logic_base_state folded-base-ctor corollary.
