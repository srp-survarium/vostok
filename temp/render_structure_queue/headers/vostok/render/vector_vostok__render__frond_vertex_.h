////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::render::frond_vertex,vostok::render::std_allocator<vostok::render::frond_vertex> >
	stlp_std::vector<vostok::render::frond_vertex >;
class vostok::render::frond_vertex;

namespace vostok {
namespace render {

class vector< frond_vertex > : public std::vector< frond_vertex > {
public:
	inline							vector<vostok::render::frond_vertex>( ) { /* no source */ }
	inline							vector<vostok::render::frond_vertex>( u32 arg_0, frond_vertex const& arg_1 ) { /* no source */ }
	inline	explicit				vector<vostok::render::frond_vertex>( u32 arg_0 ) { /* no source */ }

	inline	frond_vertex&			operator[]	( u32 arg_0 ) { /* no source */ }
	inline	frond_vertex const&		operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32						size		( ) const { /* no source */ }

	inline							~vector<vostok::render::frond_vertex>( ) { /* no source */ }
}; // class vector< frond_vertex >

STATIC_SIZE_ASSERT(vector< frond_vertex >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
