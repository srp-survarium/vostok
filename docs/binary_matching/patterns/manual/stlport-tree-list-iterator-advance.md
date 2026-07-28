# STLport `++it`: map/set = `call _Rb_global::_M_increment` (tree walk); list = `mov reg,[node]` (next link)
tags: cpp:for cpp:iterator | asm:call asm:mov asm:push | topic:codegen-idiom topic:stlport
symptoms: stlp_std::priv::_Rb_global<bool>::_M_increment, _Rb_tree_iterator, _List_iterator, mov edx,[ecx] then mov [it],edx
confidence: 9/10

A node-based container (`map`/`set`/`list`) advances its iterator by following a node link, NOT
by adding `sizeof(T)`. Under /Od a `map`/`set` `++it` becomes a **CALL** to
`_Rb_global<bool>::_M_increment` (the tree successor walk); a `list` `++it` inlines to a single
load of the node's `next` pointer.

```cpp
for ( ; it != end; ++it )    VOSTOK_DELETE_IMPL( g_allocator, it->second );   // map
for ( const_iterator iter = m_filtered_ids.begin(); iter != m_filtered_ids.end(); ++iter )  // list
```
```asm
; map ++it (/Od)                         ; list ++iter (/Od)
mov   edx, [ebp-4]                          mov   ecx, [ebp-4]
push  edx                                   mov   edx, [ecx]      ; node->next
call  stlp_std::priv::_Rb_global<bool>::_M_increment    mov [ebp-4], edx
add   esp, 4
mov   [ebp-4], eax
```
Wall-ish: the `_M_increment` call is unavoidable structure for tree loops — matching it means the source iterates the tree (no manual index). cite: survarium::game_material_manager::delete_materials game_material_manager.cpp:49; cover_filter::contains_object_id cover_filter.cpp:31.
