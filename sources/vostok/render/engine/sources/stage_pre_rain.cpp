#include "pch.h"
#include "stage_pre_rain.h"

namespace vostok {
namespace render {

stage_pre_rain::stage_pre_rain(
	renderer*			in_renderer,
	renderer_context*	context
) :
	stage( in_renderer, context ),
	m_shadow_map_size( 0 ),
	m_view_to_shadow_parameter( 0 ),
	m_eye_ray_corner_parameter( 0 ),
	m_rain_offset_parameter( 0 ),
	m_rain_density_parameter( 0 ),
	m_rain_offset( 0.0f ),
	m_rain_offset_counter( 0.0f )
{
	// STATE[STUB]
	// FUNCTION BODY[0x62c190]
}

bool stage_pre_rain::is_effects_ready( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x62b0d0]
	return false;
}

stage_pre_rain::~stage_pre_rain( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x62b660]
}

float3 stage_pre_rain::compute_aligment(
	float3 const&,
	float4x4 const&,
	float
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x62b0f0]
	return float3( 0.0f, 0.0f, 0.0f );
}

float4x4 stage_pre_rain::render_rain_shadow_map( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x62b6f0]
	return float4x4( );
}

void stage_pre_rain::execute( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x62beb0]
}

} // namespace render
} // namespace vostok
