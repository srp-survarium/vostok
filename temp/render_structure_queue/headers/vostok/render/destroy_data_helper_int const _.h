////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_DESTROY_DATA_HELPER_H_INCLUDED
#define RENDER_DESTROY_DATA_HELPER_H_INCLUDED

/* INCLUDES */
struct vostok::render::destroy_data_helper_base;

namespace vostok {
namespace render {

struct destroy_data_helper< int const > : public destroy_data_helper_base {
	virtual	void	destroy							( pcvoid arg_0 ) override { /* no source */ }

	inline			destroy_data_helper<int const >	( ) { /* no source */ }
}; // struct destroy_data_helper< int const >

STATIC_SIZE_ASSERT(destroy_data_helper< int const >, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_DESTROY_DATA_HELPER_H_INCLUDED
