#include "pch.h"
#include "stage_gbuffer.h"

#include "render_target.h"

namespace vostok {
namespace render {

void effect_copy_depth_rt::compile(
	effect_compiler&,
	custom_config_value const&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x72410]
}

void fill_surface( render_target_ptr, renderer_context* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x62cec0]
}

stage_gbuffer::stage_gbuffer(
	renderer*			in_renderer,
	renderer_context*	context
) :
	stage( in_renderer, context ),
	m_sun_direction( 0 ),
	m_c_start_corner( 0 ),
	m_object_transparency_scale_parameter( 0 ),
	m_far_fog_color_and_distance( 0 ),
	m_c_sun_near_aabb_point( 0 ),
	m_c_bound_box_min( 0 ),
	m_c_bound_box_max( 0 ),
	m_near_fog_distance( 0 ),
	m_fog_alpha( 0 ),
	m_ambient_color( 0 ),
	m_c_environment_skylight_upper_color( 0 ),
	m_c_environment_skylight_lower_color( 0 ),
	m_c_environment_skylight_parameters( 0 ),
	m_c_gs_test_constant( 0 ),
	m_c_sun_direction( 0 ),
	m_c_translucency_max_scatter( 0 ),
	m_c_sun_color( 0 ),
	m_wind_info_parameters( 0 ),
	m_smoothness_multiplier( 0 ),
	m_is_pre_pass( false ),
	m_fill_view_space_depth( false )
{
	for ( u32 i = 0; i < 4; ++i )
		m_shadow[i] = 0;

	// STATE[STUB]
	// FUNCTION BODY[0x62d800]
}

stage_gbuffer::~stage_gbuffer( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x62ce50]
}

bool stage_gbuffer::is_effects_ready( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x62ce30]
	return false;
}

void stage_gbuffer::render_models(
	vector<render_surface_instance*>&,
	u32,
	u32&,
	bool
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x62d1d0]
}

bool remove_model_if_not_lod_predicate::operator()( render_surface_instance* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x72510]
	return false;
}

bool remove_model_if_not_static_predicate::operator()( render_surface_instance* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x72580]
	return false;
}

bool remove_model_if_not_skeletal_predicate::operator()( render_surface_instance* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x725e0]
	return false;
}

bool remove_model_if_not_translucency_predicate::operator()( render_surface_instance* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x72630]
	return false;
}

bool sort_by_ps_predicate::operator()(
	render_surface_instance const*,
	render_surface_instance const*
) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x64d60]
	return false;
}

void stage_gbuffer::z_only_pass( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x62e060]
}

void stage_gbuffer::execute( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x62e300]
}

void stage_gbuffer::render_grass( bool )
{
	// STATE[STUB]
	// FUNCTION BODY[0x62e010]
}

void stage_gbuffer::render_particles( bool )
{
	// STATE[STUB]
	// FUNCTION BODY[0x62d460]
}

void stage_gbuffer::render_speedtree( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x62ce20]
}

} // namespace render
} // namespace vostok
