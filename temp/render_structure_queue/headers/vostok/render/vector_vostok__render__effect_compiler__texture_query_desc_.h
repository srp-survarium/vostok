////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::render::effect_compiler::texture_query_desc,vostok::render::std_allocator<vostok::render::effect_compiler::texture_query_desc> >
	stlp_std::vector<vostok::render::effect_compiler::texture_query_desc >;
class vostok::render::effect_compiler::texture_query_desc;

namespace vostok {
namespace render {

class vector< effect_compiler::texture_query_desc > : public std::vector< effect_compiler::texture_query_desc > {
public:
	inline				vector<vostok::render::effect_compiler::texture_query_desc>( ) { /* no source */ }
	inline				vector<vostok::render::effect_compiler::texture_query_desc>( u32 arg_0, effect_compiler::texture_query_desc const& arg_1 ) { /* no source */ }
	inline	explicit	vector<vostok::render::effect_compiler::texture_query_desc>( u32 arg_0 ) { /* no source */ }

	inline	effect_compiler::texture_query_desc&	operator[]	( u32 arg_0 ) { /* no source */ }
	inline	effect_compiler::texture_query_desc const&	operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32			size		( ) const { /* no source */ }

	inline				~vector<vostok::render::effect_compiler::texture_query_desc>( ) { /* no source */ }
}; // class vector< effect_compiler::texture_query_desc >

STATIC_SIZE_ASSERT(vector< effect_compiler::texture_query_desc >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
