////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_TERRAIN_BUFFER_FRAGMENT_H_INCLUDED
#define RENDER_TERRAIN_BUFFER_FRAGMENT_H_INCLUDED

/* INCLUDES */
class vostok::math::rectangle<vostok::math::uint2>;
class vostok::vectora<vostok::render::terrain_data>;

/* FORWARD REFS */
class vostok::memory::base_allocator;

namespace vostok {
namespace render {

struct terrain_buffer_fragment {
	inline	explicit	terrain_buffer_fragment	( memory::base_allocator* arg_0 ) { /* no source */ }
	inline				~terrain_buffer_fragment( ) { /* no source */ }

	/* 0x0000 */	vectora< terrain_data >				buffer;
	/* 0x0010 */	math::rectangle< math::uint2 >		rect;
}; // struct terrain_buffer_fragment

STATIC_SIZE_ASSERT(terrain_buffer_fragment, 0x20);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_TERRAIN_BUFFER_FRAGMENT_H_INCLUDED
