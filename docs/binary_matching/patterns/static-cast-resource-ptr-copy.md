# `static_cast_resource_ptr<P>( x.get_unmanaged_resource() )`: extra copy-construct + 8B frame
tags: cpp:cast cpp:template | asm:call asm:lea asm:sub-esp | topic:inline-vs-call
symptoms: sub esp 0BCh vs 0B4h, extra resource_ptr copy-construct call into a second slot
confidence: 7/10
variants: static-cast-checked.md, template-param-byvalue-vs-constref.md

TARGET builds the `get_unmanaged_resource()` result as a direct stack prvalue into the
by-value param (one temp slot); BASE materializes a named slot then an EXTRA resource_ptr
copy-construct into a second slot (the `+ call` and recurring +8B frame). Whether the
by-value param is constructed in place or via a copy is a header-template inline/RVO
decision under LTCG - the nested-call source is already maximally direct. Mark DONE
(structure matches).

```cpp
P p = static_cast_resource_ptr< P >( data[ i ].get_unmanaged_resource( ) );
```
Evidence: booby_trap_set_core_cook on_subresources_loaded (89.02%), on_config_ready (82.01%); static_cast_resource_ptr takes `const src_ptr` by value (resources_resource_ptr_inline.h:55).
