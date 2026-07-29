////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::render::lpv_vertex,vostok::render::std_allocator<vostok::render::lpv_vertex> >
	stlp_std::vector<vostok::render::lpv_vertex >;
class vostok::render::lpv_vertex;

namespace vostok {
namespace render {

class vector< lpv_vertex > : public std::vector< lpv_vertex > {
public:
	inline							vector<vostok::render::lpv_vertex>	( ) { /* no source */ }
	inline							vector<vostok::render::lpv_vertex>	( u32 arg_0, lpv_vertex const& arg_1 ) { /* no source */ }
	inline	explicit				vector<vostok::render::lpv_vertex>	( u32 arg_0 ) { /* no source */ }

	inline	lpv_vertex&				operator[]							( u32 arg_0 ) { /* no source */ }
	inline	lpv_vertex const&		operator[]							( u32 arg_0 ) const { /* no source */ }

	inline	u32						size								( ) const { /* no source */ }

	inline							~vector<vostok::render::lpv_vertex>	( ) { /* no source */ }
}; // class vector< lpv_vertex >

STATIC_SIZE_ASSERT(vector< lpv_vertex >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
