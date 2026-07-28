# STLport `begin()`/`end()` for a tree/list = an ICF-folded iterator copy-ctor CALL into a slot; the name is noise
tags: cpp:iterator cpp:for | asm:call asm:cmp | topic:fold-icf topic:stlport
symptoms: lea eax,[ebp-4]; call <ICF-fold name e.g. boost::_bi::list1<...>>, mov [end],eax, then cmp it,end / setne, _Rb_tree_iterator copy-ctor folded
confidence: 8/10

A tree/list `begin()`/`end()` returns an iterator OBJECT, so /Od materializes each as a small
copy-ctor **CALL** into a frame slot. ICF folds that trivial iterator ctor onto an unrelated
same-shape symbol (often a `boost::_bi::list1<...>`), so the call NAME is misleading — match the
shape, not the name. The `it != end` test is then `cmp it,[end]; setne; movzx; test; je`.

```cpp
map<u16, game_material const*>::iterator it  = m_materials.begin( );
map<u16, game_material const*>::iterator end = m_materials.end( );
```
```asm
lea   eax, [ebp-4]
call  boost::_bi::list1<...>::list1<...>   ; ICF-folded begin() iterator ctor
add   ecx, 108h
call  boost::_bi::list1<...>::list1<...>   ; folded end() iterator ctor
...
mov   ecx, [ebp-4]
xor   edx, edx
cmp   ecx, [ebp-18h]
setne dl
```
Steerable: store `begin()`/`end()` in named locals (as the source does) to get the two ctor-call rows; the fold name is unrecoverable noise (icf-fold-names-noise.md). cite: delete_materials game_material_manager.cpp:47-48.
