////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::render::leafmesh_vertex,vostok::render::std_allocator<vostok::render::leafmesh_vertex> >
	stlp_std::vector<vostok::render::leafmesh_vertex >;
class vostok::render::leafmesh_vertex;

namespace vostok {
namespace render {

class vector< leafmesh_vertex > : public std::vector< leafmesh_vertex > {
public:
	inline								vector<vostok::render::leafmesh_vertex>( ) { /* no source */ }
	inline								vector<vostok::render::leafmesh_vertex>( u32 arg_0, leafmesh_vertex const& arg_1 ) { /* no source */ }
	inline	explicit					vector<vostok::render::leafmesh_vertex>( u32 arg_0 ) { /* no source */ }

	inline	leafmesh_vertex&			operator[]	( u32 arg_0 ) { /* no source */ }
	inline	leafmesh_vertex const&		operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32							size		( ) const { /* no source */ }

	inline								~vector<vostok::render::leafmesh_vertex>( ) { /* no source */ }
}; // class vector< leafmesh_vertex >

STATIC_SIZE_ASSERT(vector< leafmesh_vertex >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
