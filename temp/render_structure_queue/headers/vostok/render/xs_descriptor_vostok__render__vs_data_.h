////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_XS_DESCRIPTOR_H_INCLUDED
#define RENDER_XS_DESCRIPTOR_H_INCLUDED

/* INCLUDES */
struct vostok::render::vs_data;
typedef vostok::intrusive_ptr<vostok::render::res_xs_hw<vostok::render::vs_data>,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_xs_hw<vostok::render::vs_data>_ptr;
class vostok::render::res_xs_hw<vostok::render::vs_data>;

/* FORWARD REFS */
class vostok::render::res_texture;

namespace vostok {
namespace render {

class xs_descriptor< vs_data > {
public:
	inline										xs_descriptor<vostok::render::vs_data>( ) { /* no source */ }

			void								reset			( res_xs_hw< vs_data >* xs_hw );

	inline	bool								set_sampler		( pcstr arg_0, ID3D11SamplerState* arg_1 ) { /* no source */ }
			bool								set_texture		( pcstr name, res_texture* texture );
			bool								use_texture		( pcstr name );
	inline	bool								use_sampler		( pcstr arg_0 ) { /* no source */ }

	inline	vs_data&							data			( ) { /* no source */ }
	inline	vs_data const&						data			( ) const { /* no source */ }

	inline	res_xs_hw< vs_data >_ptr const&		hardware_shader	( ) const { /* no source */ }

	inline										~xs_descriptor<vostok::render::vs_data>( ) { /* no source */ }

private:
	/* 0x0000 */	res_xs_hw< vs_data >_ptr	m_hardware_shader;
	/* 0x0004 */	vs_data						m_shader_data;
}; // class xs_descriptor< vs_data >

STATIC_SIZE_ASSERT(xs_descriptor< vs_data >, 0x2F80);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_XS_DESCRIPTOR_H_INCLUDED
