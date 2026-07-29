////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::render::texture_named_instance,vostok::render::std_allocator<vostok::render::texture_named_instance> >
	stlp_std::vector<vostok::render::texture_named_instance >;
class vostok::render::texture_named_instance;

namespace vostok {
namespace render {

class vector< texture_named_instance > : public std::vector< texture_named_instance > {
public:
	inline										vector<vostok::render::texture_named_instance>( ) { /* no source */ }
	inline										vector<vostok::render::texture_named_instance>( u32 arg_0, texture_named_instance const& arg_1 ) { /* no source */ }
	inline	explicit							vector<vostok::render::texture_named_instance>( u32 arg_0 ) { /* no source */ }

	inline	texture_named_instance&				operator[]	( u32 arg_0 ) { /* no source */ }
	inline	texture_named_instance const&		operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32									size		( ) const { /* no source */ }

	inline										~vector<vostok::render::texture_named_instance>( ) { /* no source */ }
}; // class vector< texture_named_instance >

STATIC_SIZE_ASSERT(vector< texture_named_instance >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
