////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED
#define RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED

namespace vostok {
namespace render {

struct std_allocator< std::pair< u32, volume_fog_parameters > >::rebind< std::pair< u32, volume_fog_parameters > > {
}; // struct std_allocator< std::pair< u32, volume_fog_parameters > >::rebind< std::pair< u32, volume_fog_parameters > >

STATIC_SIZE_ASSERT(std_allocator< std::pair< u32, volume_fog_parameters > >::rebind< std::pair< u32, volume_fog_parameters > >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED
