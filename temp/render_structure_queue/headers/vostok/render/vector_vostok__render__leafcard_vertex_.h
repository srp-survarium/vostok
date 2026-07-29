////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::render::leafcard_vertex,vostok::render::std_allocator<vostok::render::leafcard_vertex> >
	stlp_std::vector<vostok::render::leafcard_vertex >;
class vostok::render::leafcard_vertex;

namespace vostok {
namespace render {

class vector< leafcard_vertex > : public std::vector< leafcard_vertex > {
public:
	inline								vector<vostok::render::leafcard_vertex>( ) { /* no source */ }
	inline								vector<vostok::render::leafcard_vertex>( u32 arg_0, leafcard_vertex const& arg_1 ) { /* no source */ }
	inline	explicit					vector<vostok::render::leafcard_vertex>( u32 arg_0 ) { /* no source */ }

	inline	leafcard_vertex&			operator[]	( u32 arg_0 ) { /* no source */ }
	inline	leafcard_vertex const&		operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32							size		( ) const { /* no source */ }

	inline								~vector<vostok::render::leafcard_vertex>( ) { /* no source */ }
}; // class vector< leafcard_vertex >

STATIC_SIZE_ASSERT(vector< leafcard_vertex >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
