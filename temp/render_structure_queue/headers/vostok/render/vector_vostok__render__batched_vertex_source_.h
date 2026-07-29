////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::render::batched_vertex_source,vostok::render::std_allocator<vostok::render::batched_vertex_source> >
	stlp_std::vector<vostok::render::batched_vertex_source >;
class vostok::render::batched_vertex_source;

namespace vostok {
namespace render {

class vector< batched_vertex_source > : public std::vector< batched_vertex_source > {
public:
	inline									vector<vostok::render::batched_vertex_source>( ) { /* no source */ }
	inline									vector<vostok::render::batched_vertex_source>( u32 arg_0, batched_vertex_source const& arg_1 ) { /* no source */ }
	inline	explicit						vector<vostok::render::batched_vertex_source>( u32 arg_0 ) { /* no source */ }

	inline	batched_vertex_source&			operator[]	( u32 arg_0 ) { /* no source */ }
	inline	batched_vertex_source const&	operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32								size		( ) const { /* no source */ }

	inline									~vector<vostok::render::batched_vertex_source>( ) { /* no source */ }
}; // class vector< batched_vertex_source >

STATIC_SIZE_ASSERT(vector< batched_vertex_source >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
