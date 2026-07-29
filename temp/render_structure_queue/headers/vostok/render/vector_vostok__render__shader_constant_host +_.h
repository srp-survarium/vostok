////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::render::shader_constant_host *,vostok::render::std_allocator<vostok::render::shader_constant_host *> >
	stlp_std::vector<vostok::render::shader_constant_host * >;
class vostok::render::shader_constant_host *;

/* FORWARD REFS */
class vostok::render::shader_constant_host;
class vostok::render::shader_constant_host*;

namespace vostok {
namespace render {

class vector< shader_constant_host* > : public std::vector< shader_constant_host* > {
public:
	inline									vector<vostok::render::shader_constant_host *>( ) { /* no source */ }
	inline									vector<vostok::render::shader_constant_host *>( u32 arg_0, shader_constant_host* const& arg_1 ) { /* no source */ }
	inline	explicit						vector<vostok::render::shader_constant_host *>( u32 arg_0 ) { /* no source */ }

	inline	shader_constant_host*&			operator[]	( u32 arg_0 ) { /* no source */ }
	inline	shader_constant_host* const&	operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32								size		( ) const { /* no source */ }

	inline									~vector<vostok::render::shader_constant_host *>( ) { /* no source */ }
}; // class vector< shader_constant_host* >

STATIC_SIZE_ASSERT(vector< shader_constant_host* >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
