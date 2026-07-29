////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::render::streaming_ready_texture,vostok::render::std_allocator<vostok::render::streaming_ready_texture> >
	stlp_std::vector<vostok::render::streaming_ready_texture >;
class vostok::render::streaming_ready_texture;

namespace vostok {
namespace render {

class vector< streaming_ready_texture > : public std::vector< streaming_ready_texture > {
public:
	inline										vector<vostok::render::streaming_ready_texture>( ) { /* no source */ }
	inline										vector<vostok::render::streaming_ready_texture>( u32 arg_0, streaming_ready_texture const& arg_1 ) { /* no source */ }
	inline	explicit							vector<vostok::render::streaming_ready_texture>( u32 arg_0 ) { /* no source */ }

	inline	streaming_ready_texture&			operator[]	( u32 arg_0 ) { /* no source */ }
	inline	streaming_ready_texture const&		operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32									size		( ) const { /* no source */ }

	inline										~vector<vostok::render::streaming_ready_texture>( ) { /* no source */ }
}; // class vector< streaming_ready_texture >

STATIC_SIZE_ASSERT(vector< streaming_ready_texture >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
