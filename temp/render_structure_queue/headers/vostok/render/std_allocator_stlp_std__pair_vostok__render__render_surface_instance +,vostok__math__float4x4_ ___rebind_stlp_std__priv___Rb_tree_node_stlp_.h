////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED
#define RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED

namespace vostok {
namespace render {

struct std_allocator< std::pair< render_surface_instance*, float4x4 > >::rebind< std::priv::_Rb_tree_node< std::pair< render_surface_instance* const, float4x4 > > > {
}; // struct std_allocator< std::pair< render_surface_instance*, float4x4 > >::rebind< std::priv::_Rb_tree_node< std::pair< render_surface_instance* const, float4x4 > > >

STATIC_SIZE_ASSERT(std_allocator< std::pair< render_surface_instance*, float4x4 > >::rebind< std::priv::_Rb_tree_node< std::pair< render_surface_instance* const, float4x4 > > >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED
