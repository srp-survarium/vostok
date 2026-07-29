////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::render::streaming_texture_instance,vostok::render::std_allocator<vostok::render::streaming_texture_instance> >
	stlp_std::vector<vostok::render::streaming_texture_instance >;
class vostok::render::streaming_texture_instance;

namespace vostok {
namespace render {

class vector< streaming_texture_instance > : public std::vector< streaming_texture_instance > {
public:
	inline											vector<vostok::render::streaming_texture_instance>( ) { /* no source */ }
	inline											vector<vostok::render::streaming_texture_instance>( u32 arg_0, streaming_texture_instance const& arg_1 ) { /* no source */ }
	inline	explicit								vector<vostok::render::streaming_texture_instance>( u32 arg_0 ) { /* no source */ }

	inline	streaming_texture_instance&				operator[]	( u32 arg_0 ) { /* no source */ }
	inline	streaming_texture_instance const&		operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32										size		( ) const { /* no source */ }

	inline											~vector<vostok::render::streaming_texture_instance>( ) { /* no source */ }
}; // class vector< streaming_texture_instance >

STATIC_SIZE_ASSERT(vector< streaming_texture_instance >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
