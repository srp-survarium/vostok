////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::render::shadow_vertex,vostok::render::std_allocator<vostok::render::shadow_vertex> >
	stlp_std::vector<vostok::render::shadow_vertex >;
class vostok::render::shadow_vertex;

namespace vostok {
namespace render {

class vector< shadow_vertex > : public std::vector< shadow_vertex > {
public:
	inline							vector<vostok::render::shadow_vertex>( ) { /* no source */ }
	inline							vector<vostok::render::shadow_vertex>( u32 arg_0, shadow_vertex const& arg_1 ) { /* no source */ }
	inline	explicit				vector<vostok::render::shadow_vertex>( u32 arg_0 ) { /* no source */ }

	inline	shadow_vertex&			operator[]	( u32 arg_0 ) { /* no source */ }
	inline	shadow_vertex const&	operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32						size		( ) const { /* no source */ }

	inline							~vector<vostok::render::shadow_vertex>( ) { /* no source */ }
}; // class vector< shadow_vertex >

STATIC_SIZE_ASSERT(vector< shadow_vertex >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
