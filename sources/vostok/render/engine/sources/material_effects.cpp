#include "pch.h"
#include "material_effects.h"

namespace vostok {
namespace render {

material_effects::material_effects( ) :
	m_vertex_input_type	( null_vertex_input_type ),
	m_cull_mode			( D3D11_CULL_NONE )
{
	// FUNCTION BODY[0x62ff80]
	// This is correct until the class is not polymorphic !
	//vostok::memory::zero( this , sizeof(*this));
	vostok::memory::zero		(stage_enable);
	organic_clear_color		= math::float4(1.0f, 1.0f, 1.0f, 1.0f);
	is_cast_shadow			= true;
	is_organic				= false;
	organic_clear_color		= vostok::math::float4(1.0f, 1.0f, 1.0f, 1.0f);
	// claude@NOTE: the canonical flags added since the legacy ancestor (is_emissive,
	// use_subsurface_scattering, alpha/wind/sky/reflection/translucency/blend_mode, ...)
	// are not initialized by the legacy seed - matcher-phase.
}

enum_vertex_input_type material_effects::get_vertex_input_type( ) const
{
	// FUNCTION BODY[0x62feb0]
	return m_vertex_input_type;
}

void material_effects::get_used_textures(
	vector< texture_named_instance >&
) const
{
	// claude@NOTE: no legacy ancestor - legacy material_effects had no get_used_textures; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x62fec0]
}

} // namespace render
} // namespace vostok
