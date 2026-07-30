#include "pch.h"
#include "stage_light_propagation_volumes.h"

#include "remove_model_if_in_frustum_predicate.h"

namespace vostok {
namespace render {

stage_light_propagation_volumes::stage_light_propagation_volumes(
	renderer*			in_renderer,
	renderer_context*	context
) :
	stage						( in_renderer, context ),
	start_render_eye_position	( 0.0f, 0.0f, 0.0f ),
	m_radiance_volume			( 0 ),
	m_num_cascades				( 0 ),
	m_rsm_source_size			( 0 ),
	m_rsm_downsampled_size		( 0 ),
	m_grid_size					( 0 ),
	m_has_indirect_lighting		( false )
{
	// STATE[STUB]
	// FUNCTION BODY[0x619bf0]
}

stage_light_propagation_volumes::~stage_light_propagation_volumes( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x615980]
}

bool stage_light_propagation_volumes::is_effects_ready( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x6146e0]
	return false;
}

void stage_light_propagation_volumes::set_rsm_contants( float3 const&, float3 const&, float )
{
	// STATE[STUB]
	// FUNCTION BODY[0x614cd0]
}

void stage_light_propagation_volumes::register_rsm_constans( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x615510]
}

void stage_light_propagation_volumes::register_light_constans( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x614de0]
}

void stage_light_propagation_volumes::pre_lpv_batch_render(
	float3 const&,
	float,
	geometry_batch const&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x6168f0]
}

void stage_light_propagation_volumes::post_lpv_batch_render( geometry_batch const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x614750]
}

void stage_light_propagation_volumes::render_to_rms(
	float3 const&,
	float,
	float4x4 const&,
	float4x4 const&,
	vector<float4x4>,
	u32
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x616c80]
}

bool remove_model_if_in_frustum_predicate::operator()( lpv_render_surface const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x7d230]
	return false;
}

void stage_light_propagation_volumes::render_to_rms_smoothed2(
	float3 const&,
	float,
	float4x4 const&,
	float4x4 const&,
	vector<float4x4>,
	u32,
	u32,
	u32
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x615f70]
}

static float3 compute_aligment( float3 const&, float4x4 const&, float )
{
	// STATE[STUB]
	// FUNCTION BODY[0x614760]
	return float3( 1.0f, 1.0f, 1.0f );
}

void stage_light_propagation_volumes::render_to_sun_rms(
	light*,
	u32,
	vector<float4x4>
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x6188a0]
}

void stage_light_propagation_volumes::render_to_sun_rms_smoothed(
	light*,
	u32,
	vector<float4x4>,
	u32,
	u32
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x6169b0]
}

void stage_light_propagation_volumes::render_to_point_rms(
	light*,
	u32,
	vector<float4x4>
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x618750]
}

void stage_light_propagation_volumes::render_to_sky_rms(
	light*,
	u32,
	u32,
	vector<float4x4>
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x618200]
}

void stage_light_propagation_volumes::render_to_spot_rms( light*, vector<float4x4> )
{
	// STATE[STUB]
	// FUNCTION BODY[0x618010]
}

void stage_light_propagation_volumes::propagate_lighting( u32 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x615f50]
}

void stage_light_propagation_volumes::propagate_lighting_smoothed( u32, u32 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x615f30]
}

void stage_light_propagation_volumes::render_quad( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x615760]
}

void stage_light_propagation_volumes::downsample_rsm(
	float3 const&,
	float3 const&,
	float,
	u32
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x615d50]
}

void stage_light_propagation_volumes::inject_lighting(
	u32,
	float3 const&,
	float3 const&,
	float
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x615d20]
}

void stage_light_propagation_volumes::inject_occluders(
	u32,
	float3 const&,
	float3 const&,
	vector<float4x4>
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x615cc0]
}

void stage_light_propagation_volumes::execute_smoothed_impl( u32, u32, u32, u32, u32 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6173c0]
}

void stage_light_propagation_volumes::execute_impl( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x618b60]
}

void stage_light_propagation_volumes::execute_disabled( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x615690]
}

void stage_light_propagation_volumes::execute( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x619b30]
}

void stage_light_propagation_volumes::draw_debug( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6146d0]
}

} // namespace render
} // namespace vostok
