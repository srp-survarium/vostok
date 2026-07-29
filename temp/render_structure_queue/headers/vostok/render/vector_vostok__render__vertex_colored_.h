////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::render::vertex_colored,vostok::render::std_allocator<vostok::render::vertex_colored> >
	stlp_std::vector<vostok::render::vertex_colored >;
class vostok::render::vertex_colored;

namespace vostok {
namespace render {

class vector< vertex_colored > : public std::vector< vertex_colored > {
public:
	inline								vector<vostok::render::vertex_colored>( ) { /* no source */ }
	inline								vector<vostok::render::vertex_colored>( u32 arg_0, vertex_colored const& arg_1 ) { /* no source */ }
	inline	explicit					vector<vostok::render::vertex_colored>( u32 arg_0 ) { /* no source */ }

	inline	vertex_colored&				operator[]	( u32 arg_0 ) { /* no source */ }
	inline	vertex_colored const&		operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32							size		( ) const { /* no source */ }

	inline								~vector<vostok::render::vertex_colored>( ) { /* no source */ }
}; // class vector< vertex_colored >

STATIC_SIZE_ASSERT(vector< vertex_colored >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
