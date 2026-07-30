#include "pch.h"
#include "organic_lighting_passes.h"
#include "stage_lights.h"

namespace vostok {
namespace render {

void stage_lights::new_sphere_geometry( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x61bcd0]
}

void stage_lights::create_pyramid_geometry( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x61bac0]
}

void stage_lights::create_obb_geometry( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x61b9a0]
}

stage_lights::stage_lights(
	renderer*			in_renderer,
	renderer_context*	context,
	bool				is_forward_lighting_pass
) :
	stage						( in_renderer, context ),
	m_enable_env_probes			( false ),
	m_num_instanced_lights		( 0 ),
	m_instance_data_array		( 0 ),
	m_light_instances			( 0 ),
	m_shadow_z_bias				( 0.0f ),
	m_shadow_map_size			( 0.0f ),
	m_is_forward_lighting_pass	( is_forward_lighting_pass )
{
	// STATE[STUB]
	// FUNCTION BODY[0x624e70]
}

bool stage_lights::is_effects_ready( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x61a8f0]
	return false;
}

void stage_lights::make_spot_light_shadowmap( u32, light* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6206d0]
}

void stage_lights::make_plane_spot_light_shadowmap( u32, light* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x620430]
}

void stage_lights::render_to_hw_shadowmap(
	light*,
	u32,
	float,
	u32,
	u32,
	float4x4 const&,
	float4x4 const&,
	u32
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x61fb80]
}

stage_lights::~stage_lights( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x61b090]
}

void stage_lights::render_particle_lighting(
	render_particle_emitter_instance*,
	light*,
	u32
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x61ec00]
}

float compute_gaussian_value( float, float, float )
{
	// STATE[STUB]
	// FUNCTION BODY[0x61a9a0]
	return 0.0f;
}

void get_gaussain_weights_offsets(
	float*,
	float*,
	u32,
	float,
	float,
	u32
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x61aa00]
}

void stage_lights::fill_surface( render_target_ptr )
{
	// STATE[STUB]
	// FUNCTION BODY[0x61add0]
}

void stage_lights::make_skin_scattering_texture(
	render_surface_instance*,
	light*
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x61d2d0]
}

void stage_lights::render_particle_probe_lighting(
	render_particle_emitter_instance*,
	environment_probe*,
	u32
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x61cec0]
}

void stage_lights::render_model_probe_lighting(
	render_surface_instance*,
	environment_probe*,
	float
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x61cc70]
}

void stage_lights::render_model_lighting(
	render_surface_instance*,
	light*
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x621850]
}

void stage_lights::render_speedtree_lighting(
	lod_entry const*,
	SpeedTree::CInstance const*,
	SpeedTree::SInstanceLod const*,
	speedtree_tree_component*,
	light*
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x61bde0]
}

void stage_lights::execute_disabled( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x61aae0]
}

void stage_lights::execute( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6240c0]
}

u32 stage_lights::index_to_shadow_size( u32 ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x61a8b0]
	return 0;
}

void stage_lights::render_shadowed_light( light* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6208b0]
}

void stage_lights::render_light( light*, bool )
{
	// STATE[STUB]
	// FUNCTION BODY[0x622c20]
}

void stage_lights::draw_geometry( light* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x61ac40]
}

void stage_lights::debug_render( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6225f0]
}

} // namespace render
} // namespace vostok
