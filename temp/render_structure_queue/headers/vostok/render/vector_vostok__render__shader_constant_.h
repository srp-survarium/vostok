////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::render::shader_constant,vostok::render::std_allocator<vostok::render::shader_constant> >
	stlp_std::vector<vostok::render::shader_constant >;
class vostok::render::shader_constant;

namespace vostok {
namespace render {

class vector< shader_constant > : public std::vector< shader_constant > {
public:
	inline								vector<vostok::render::shader_constant>( ) { /* no source */ }
	inline								vector<vostok::render::shader_constant>( u32 arg_0, shader_constant const& arg_1 ) { /* no source */ }
	inline	explicit					vector<vostok::render::shader_constant>( u32 arg_0 ) { /* no source */ }

	inline	shader_constant&			operator[]	( u32 arg_0 ) { /* no source */ }
	inline	shader_constant const&		operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32							size		( ) const { /* no source */ }

	inline								~vector<vostok::render::shader_constant>( ) { /* no source */ }
}; // class vector< shader_constant >

STATIC_SIZE_ASSERT(vector< shader_constant >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
