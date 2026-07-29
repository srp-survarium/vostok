////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_DESTROY_DATA_HELPER_BASE_H_INCLUDED
#define RENDER_DESTROY_DATA_HELPER_BASE_H_INCLUDED

namespace vostok {
namespace render {

struct destroy_data_helper_base {
	virtual	void	destroy					( pcvoid arg_0 ) = 0;

	inline			destroy_data_helper_base( ) { /* no source */ }
}; // struct destroy_data_helper_base

STATIC_SIZE_ASSERT(destroy_data_helper_base, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_DESTROY_DATA_HELPER_BASE_H_INCLUDED
