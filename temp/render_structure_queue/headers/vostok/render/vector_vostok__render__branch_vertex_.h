////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::render::branch_vertex,vostok::render::std_allocator<vostok::render::branch_vertex> >
	stlp_std::vector<vostok::render::branch_vertex >;
class vostok::render::branch_vertex;

namespace vostok {
namespace render {

class vector< branch_vertex > : public std::vector< branch_vertex > {
public:
	inline							vector<vostok::render::branch_vertex>( ) { /* no source */ }
	inline							vector<vostok::render::branch_vertex>( u32 arg_0, branch_vertex const& arg_1 ) { /* no source */ }
	inline	explicit				vector<vostok::render::branch_vertex>( u32 arg_0 ) { /* no source */ }

	inline	branch_vertex&			operator[]	( u32 arg_0 ) { /* no source */ }
	inline	branch_vertex const&	operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32						size		( ) const { /* no source */ }

	inline							~vector<vostok::render::branch_vertex>( ) { /* no source */ }
}; // class vector< branch_vertex >

STATIC_SIZE_ASSERT(vector< branch_vertex >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
