////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_DESTROY_DATA_HELPER_H_INCLUDED
#define RENDER_DESTROY_DATA_HELPER_H_INCLUDED

/* INCLUDES */
struct vostok::render::destroy_data_helper_base;

namespace vostok {
namespace render {

struct destroy_data_helper< enum enum_vertex_input_type const > : public destroy_data_helper_base {
	virtual	void	destroy	( pcvoid data ) override { /* no source */ }

	inline			destroy_data_helper<enum vostok::render::enum_vertex_input_type const >( ) { /* no source */ }
}; // struct destroy_data_helper< enum enum_vertex_input_type const >

STATIC_SIZE_ASSERT(destroy_data_helper< enum enum_vertex_input_type const >, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_DESTROY_DATA_HELPER_H_INCLUDED
