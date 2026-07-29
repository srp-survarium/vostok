////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_XS_DESCRIPTOR_H_INCLUDED
#define RENDER_XS_DESCRIPTOR_H_INCLUDED

/* INCLUDES */
struct vostok::render::ps_data;
typedef vostok::intrusive_ptr<vostok::render::res_xs_hw<vostok::render::ps_data>,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_xs_hw<vostok::render::ps_data>_ptr;
class vostok::render::res_xs_hw<vostok::render::ps_data>;

/* FORWARD REFS */
class vostok::render::res_texture;

namespace vostok {
namespace render {

class xs_descriptor< ps_data > {
public:
	inline										xs_descriptor<vostok::render::ps_data>( ) { /* no source */ }

	inline	void								reset			( res_xs_hw< ps_data >* arg_0 ) { /* no source */ }

	inline	bool								set_sampler		( pcstr arg_0, ID3D11SamplerState* arg_1 ) { /* no source */ }
	inline	bool								set_texture		( pcstr arg_0, res_texture* arg_1 ) { /* no source */ }
	inline	bool								use_texture		( pcstr arg_0 ) { /* no source */ }
	inline	bool								use_sampler		( pcstr arg_0 ) { /* no source */ }

	inline	ps_data&							data			( ) { /* no source */ }
	inline	ps_data const&						data			( ) const { /* no source */ }

	inline	res_xs_hw< ps_data >_ptr const&		hardware_shader	( ) const { /* no source */ }

	inline										~xs_descriptor<vostok::render::ps_data>( ) { /* no source */ }

private:
	/* 0x0000 */	res_xs_hw< ps_data >_ptr	m_hardware_shader;
	/* 0x0004 */	ps_data						m_shader_data;
}; // class xs_descriptor< ps_data >

STATIC_SIZE_ASSERT(xs_descriptor< ps_data >, 0x2F7C);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_XS_DESCRIPTOR_H_INCLUDED
