////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SAMPLER_STATE_DESCRIPTOR_H_INCLUDED
#define RENDER_SAMPLER_STATE_DESCRIPTOR_H_INCLUDED

/* INCLUDES */
struct D3D11_SAMPLER_DESC;
class vostok::render::effect_compiler;

namespace vostok {
namespace render {

class sampler_state_descriptor {
	inline	explicit						sampler_state_descriptor( effect_compiler& arg_0 ) { /* no source */ }
public:
	inline									sampler_state_descriptor( ) { /* no source */ }

	inline	sampler_state_descriptor&		reset					( ) { /* no source */ }

	inline	sampler_state_descriptor&		set						( D3D11_FILTER arg_0, D3D11_TEXTURE_ADDRESS_MODE arg_1 ) { /* no source */ }

	inline	sampler_state_descriptor&		set_filter				( D3D11_FILTER arg_0 ) { /* no source */ }
	inline	sampler_state_descriptor&		set_address_mode		(
												D3D11_TEXTURE_ADDRESS_MODE		arg_0,
												D3D11_TEXTURE_ADDRESS_MODE		arg_1,
												D3D11_TEXTURE_ADDRESS_MODE		arg_2
											) { /* no source */ }
	inline	sampler_state_descriptor&		set_mip					( float arg_0, float arg_1, float arg_2 ) { /* no source */ }
	inline	sampler_state_descriptor&		set_max_anisotropy		( u32 arg_0 ) { /* no source */ }
	inline	sampler_state_descriptor&		set_comparison_function	( D3D11_COMPARISON_FUNC arg_0 ) { /* no source */ }
	inline	sampler_state_descriptor&		set_border_color		( float4 arg_0 ) { /* no source */ }

	inline	effect_compiler&				end_sampler				( ) { /* no source */ }

private:
	/* 0x0000 */	effect_compiler*		m_effect_compiler;
	/* 0x0004 */	D3D11_SAMPLER_DESC		m_desc;
	/* 0x0038 */	bool					m_updated;
}; // class sampler_state_descriptor

STATIC_SIZE_ASSERT(sampler_state_descriptor, 0x3C);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SAMPLER_STATE_DESCRIPTOR_H_INCLUDED
