////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::render::res_render_output *,vostok::render::std_allocator<vostok::render::res_render_output *> >
	stlp_std::vector<vostok::render::res_render_output * >;
class vostok::render::res_render_output *;

/* FORWARD REFS */
class vostok::render::res_render_output;
class vostok::render::res_render_output*;

namespace vostok {
namespace render {

class vector< res_render_output* > : public std::vector< res_render_output* > {
public:
	inline									vector<vostok::render::res_render_output *>( ) { /* no source */ }
	inline									vector<vostok::render::res_render_output *>( u32 arg_0, res_render_output* const& arg_1 ) { /* no source */ }
	inline	explicit						vector<vostok::render::res_render_output *>( u32 arg_0 ) { /* no source */ }

	inline	res_render_output*&				operator[]	( u32 arg_0 ) { /* no source */ }
	inline	res_render_output* const&		operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32								size		( ) const { /* no source */ }

	inline									~vector<vostok::render::res_render_output *>( ) { /* no source */ }
}; // class vector< res_render_output* >

STATIC_SIZE_ASSERT(vector< res_render_output* >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
