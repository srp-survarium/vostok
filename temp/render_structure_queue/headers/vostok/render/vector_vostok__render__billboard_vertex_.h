////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::render::billboard_vertex,vostok::render::std_allocator<vostok::render::billboard_vertex> >
	stlp_std::vector<vostok::render::billboard_vertex >;
class vostok::render::billboard_vertex;

namespace vostok {
namespace render {

class vector< billboard_vertex > : public std::vector< billboard_vertex > {
public:
	inline								vector<vostok::render::billboard_vertex>( ) { /* no source */ }
	inline								vector<vostok::render::billboard_vertex>( u32 arg_0, billboard_vertex const& arg_1 ) { /* no source */ }
	inline	explicit					vector<vostok::render::billboard_vertex>( u32 arg_0 ) { /* no source */ }

	inline	billboard_vertex&			operator[]	( u32 arg_0 ) { /* no source */ }
	inline	billboard_vertex const&		operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32							size		( ) const { /* no source */ }

	inline								~vector<vostok::render::billboard_vertex>( ) { /* no source */ }
}; // class vector< billboard_vertex >

STATIC_SIZE_ASSERT(vector< billboard_vertex >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
