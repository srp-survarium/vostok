////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_BASE_OUTPUT_WINDOW_H_INCLUDED
#define RENDER_BASE_OUTPUT_WINDOW_H_INCLUDED

/* INCLUDES */
class vostok::math::uint2;
class vostok::resources::unmanaged_resource;

namespace vostok {
namespace render {

struct base_output_window : public resources::unmanaged_resource {
	inline	math::uint2 const&		current_size		( ) const { /* no source */ }

	inline							base_output_window	( ) { /* no source */ }
	virtual							~base_output_window	( ) { /* no source */ }

	/* 0x0000 */	/* resources::unmanaged_resource */
protected:
	/* 0x0108 */	math::uint2		m_current_size;
}; // struct base_output_window

STATIC_SIZE_ASSERT(base_output_window, 0x110);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_BASE_OUTPUT_WINDOW_H_INCLUDED
