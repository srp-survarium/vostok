////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED
#define RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED

namespace vostok {
namespace render {

struct std_allocator< effect_compiler::texture_query_desc >::rebind< effect_compiler::texture_query_desc > {
}; // struct std_allocator< effect_compiler::texture_query_desc >::rebind< effect_compiler::texture_query_desc >

STATIC_SIZE_ASSERT(std_allocator< effect_compiler::texture_query_desc >::rebind< effect_compiler::texture_query_desc >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED
