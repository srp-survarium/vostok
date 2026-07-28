# `boost::bind(&C::m, this)` functor build (sret) = pack mf code-ptr at +0 and the bound `this` at +4
tags: cpp:template | asm:mov asm:lea | topic:codegen-idiom
symptoms: sret bind_t in [ebp+8], mov [ecx],edx (mf ptr from [ebp+10h]) then mov [edx+4],ecx (bound this from list1<value<C*>>)
confidence: 8/10

`boost::bind` builds the `bind_t` functor by **sret** (result ptr in `[ebp+8]`): it stores the
member-fn-ptr value into the functor head (`mov [ecx],edx`, edx = the pmf arg at `[ebp+10h]`) and
the bound `this` into the next field (`mov [edx+4],ecx`, from the `list1<value<C*>>` arg at
`[ebp+0Ch]`). The shuffles through `[ebp-4]/-Ch/-14h` are the `mfN`/`value` member-init temporaries
under /Od. This is the construction-time field LAYOUT (distinct from cref-vs-value which is about
WHICH args become wrappers, boost-bind-cref.md).

```cpp
boost::bind( &player_logic_sprint_state::initialize, this )   // -> bind_t<void,mf0<...>,list1<value<sprint_state*>>>
```
```asm
mov   edx, [ebp+10h]      ; the pmf arg
mov   [ecx], edx          ; -> bind_t field 0 (mf0 code ptr)
mov   edx, [ebp+8]        ; sret bind_t*
mov   [edx+4], ecx        ; -> bound this (list1<value<C*>>)
mov   eax, [ebp+8]        ; return the bind_t by ptr
```
Steerable: the field order follows from the bound member-fn + arg list. cite: target boost::bind<void,player_logic_sprint_state,player_logic_sprint_state*>(...) rva 0x113470.
