////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::render::requested_streamable_texture,vostok::render::std_allocator<vostok::render::requested_streamable_texture> >
	stlp_std::vector<vostok::render::requested_streamable_texture >;
class vostok::render::requested_streamable_texture;

namespace vostok {
namespace render {

class vector< requested_streamable_texture > : public std::vector< requested_streamable_texture > {
public:
	inline											vector<vostok::render::requested_streamable_texture>( ) { /* no source */ }
	inline											vector<vostok::render::requested_streamable_texture>( u32 arg_0, requested_streamable_texture const& arg_1 ) { /* no source */ }
	inline	explicit								vector<vostok::render::requested_streamable_texture>( u32 arg_0 ) { /* no source */ }

	inline	requested_streamable_texture&			operator[]	( u32 arg_0 ) { /* no source */ }
	inline	requested_streamable_texture const&		operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32										size		( ) const { /* no source */ }

	inline											~vector<vostok::render::requested_streamable_texture>( ) { /* no source */ }
}; // class vector< requested_streamable_texture >

STATIC_SIZE_ASSERT(vector< requested_streamable_texture >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
