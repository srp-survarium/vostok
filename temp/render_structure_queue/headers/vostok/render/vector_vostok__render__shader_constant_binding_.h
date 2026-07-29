////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::render::shader_constant_binding,vostok::render::std_allocator<vostok::render::shader_constant_binding> >
	stlp_std::vector<vostok::render::shader_constant_binding >;
class vostok::render::shader_constant_binding;

namespace vostok {
namespace render {

class vector< shader_constant_binding > : public std::vector< shader_constant_binding > {
public:
	inline										vector<vostok::render::shader_constant_binding>( ) { /* no source */ }
	inline										vector<vostok::render::shader_constant_binding>( u32 arg_0, shader_constant_binding const& arg_1 ) { /* no source */ }
	inline	explicit							vector<vostok::render::shader_constant_binding>( u32 arg_0 ) { /* no source */ }

	inline	shader_constant_binding&			operator[]	( u32 arg_0 ) { /* no source */ }
	inline	shader_constant_binding const&		operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32									size		( ) const { /* no source */ }

	inline										~vector<vostok::render::shader_constant_binding>( ) { /* no source */ }
}; // class vector< shader_constant_binding >

STATIC_SIZE_ASSERT(vector< shader_constant_binding >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
