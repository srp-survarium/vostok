////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::render::streamable_texture_info,vostok::render::std_allocator<vostok::render::streamable_texture_info> >
	stlp_std::vector<vostok::render::streamable_texture_info >;
class vostok::render::streamable_texture_info;

namespace vostok {
namespace render {

class vector< streamable_texture_info > : public std::vector< streamable_texture_info > {
public:
	inline										vector<vostok::render::streamable_texture_info>( ) { /* no source */ }
	inline										vector<vostok::render::streamable_texture_info>( u32 arg_0, streamable_texture_info const& arg_1 ) { /* no source */ }
	inline	explicit							vector<vostok::render::streamable_texture_info>( u32 arg_0 ) { /* no source */ }

	inline	streamable_texture_info&			operator[]	( u32 arg_0 ) { /* no source */ }
	inline	streamable_texture_info const&		operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32									size		( ) const { /* no source */ }

	inline										~vector<vostok::render::streamable_texture_info>( ) { /* no source */ }
}; // class vector< streamable_texture_info >

STATIC_SIZE_ASSERT(vector< streamable_texture_info >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
