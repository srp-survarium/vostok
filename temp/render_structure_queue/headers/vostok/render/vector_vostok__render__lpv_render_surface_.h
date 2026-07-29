////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::render::lpv_render_surface,vostok::render::std_allocator<vostok::render::lpv_render_surface> >
	stlp_std::vector<vostok::render::lpv_render_surface >;
class vostok::render::lpv_render_surface;

namespace vostok {
namespace render {

class vector< lpv_render_surface > : public std::vector< lpv_render_surface > {
public:
	inline									vector<vostok::render::lpv_render_surface>( ) { /* no source */ }
	inline									vector<vostok::render::lpv_render_surface>( u32 arg_0, lpv_render_surface const& arg_1 ) { /* no source */ }
	inline	explicit						vector<vostok::render::lpv_render_surface>( u32 arg_0 ) { /* no source */ }

	inline	lpv_render_surface&				operator[]	( u32 arg_0 ) { /* no source */ }
	inline	lpv_render_surface const&		operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32								size		( ) const { /* no source */ }

	inline									~vector<vostok::render::lpv_render_surface>( ) { /* no source */ }
}; // class vector< lpv_render_surface >

STATIC_SIZE_ASSERT(vector< lpv_render_surface >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
