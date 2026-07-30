#include "pch.h"
#include "stage_shadow_direct.h"

#include "remove_inappropriate_models.h"
#include "remove_model_if_in_frustum_predicate.h"

namespace vostok {
namespace render {

stage_shadow_direct::stage_shadow_direct(
	renderer*			in_renderer,
	renderer_context*	context
) :
	stage							( in_renderer, context ),
	m_c_light_direction				( 0 ),
	m_c_light_position				( 0 ),
	m_c_light_attenuation_power		( 0 ),
	m_c_start_corner				( 0 ),
	m_wind_info_parameters			( 0 ),
	m_shadow_cascade_index			( 0 ),
	m_cascade_shadow_map_size		( 0 ),
	m_invalid_shadow				( true )
{
	// STATE[STUB]
	// FUNCTION BODY[0x627de0]
}

bool stage_shadow_direct::is_effects_ready( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x6272e0]
	return false;
}

stage_shadow_direct::~stage_shadow_direct( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6278b0]
}

void stage_shadow_direct::execute_disabled( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x627cd0]
}

void stage_shadow_direct::execute( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6297a0]
}

void stage_shadow_direct::draw_debug( u32 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x627890]
}

bool remove_model_if_in_frustum_predicate::operator()(
	render_surface_instance*
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x781d0]
	return false;
}

bool remove_inappropriate_models::operator()( render_surface_instance* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x78250]
	return false;
}

void stage_shadow_direct::prepare_models(
	vector<render_surface_instance*>&,
	float4x4 const&,
	u32,
	u32,
	float3 const&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x627920]
}

void stage_shadow_direct::render_models(
	vector<render_surface_instance*>&,
	float4x4 const&,
	u32,
	u32,
	float3 const&,
	u32,
	u32
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x6282e0]
}

void stage_shadow_direct::render_dynamic_models(
	u32,
	u32,
	float3 const&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x6272d0]
}

void stage_shadow_direct::execute_cascade( u32, u32, u32 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6287b0]
}

float3 stage_shadow_direct::compute_aligment(
	float3 const&,
	float4x4 const&,
	float,
	float,
	float3&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x6272f0]
	return float3( 0.0f, 0.0f, 0.0f );
}

} // namespace render
} // namespace vostok
