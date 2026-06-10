# Allocator `new_X` / `delete_X` free-function helpers: the exact statement shapes
tags: cpp:new-delete | asm:call | topic:allocator topic:structure-shape
symptoms: cast-into-local + separate placement new, void* buffer temp, ptr = NULL
confidence: 9/10
variants: explicit-dtor-call-not-delete.md, free-vs-delete-impl.md

`new_X( allocator )` is THREE statements (do NOT fuse to `new (allocator.allocate()) T`);
`delete_X( allocator, ptr )` is FOUR - including the real `void* buffer = ptr;` temp (NOT
`deallocate( reinterpret_cast<pvoid&>(ptr) )`) and the explicit NULL.

```cpp
// new_X:
T* const result = ( T* )allocator.allocate( );
new ( result ) T( );
return result;
// delete_X:
X::helper::call_destructor( *ptr );
void* buffer = ptr;
allocator.deallocate( buffer );
ptr = NULL;
```
Evidence: new_udp_match_packet 13.7 -> 99.7 (3/3 stmts); delete_udp_match_packet 0 -> 61.3 (4/4; residuals LTCG frame/inline).
