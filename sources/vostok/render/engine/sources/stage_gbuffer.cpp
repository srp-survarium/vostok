#include "pch.h"
#include "stage_gbuffer.h"

#include "render_target.h"
#include "renderer_context.h"
#include "vertex_formats.h"

#include <vostok/render/core/backend.h>
#include <vostok/render/core/options.h>

namespace vostok {
namespace render {

void effect_copy_depth_rt::compile(
	effect_compiler&,
	custom_config_value const&
)
{
	// claude@NOTE: no legacy ancestor - effect_copy_depth_rt postdates the legacy corpus; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x72410]
}

// TODO: Make as a general functions.
void fill_surface( render_target_ptr surf, renderer_context* context )
{
	// FUNCTION BODY[0x62cec0]
	// claude@NOTE: legacy (renderer.cpp) was static + #ifndef MASTER_GOLD; the target keeps it
	// unguarded in this TU
	float w = float(surf->width());
	float h = float(surf->height());
	float z = 0.0f;

	backend::ref().set_render_targets( &*surf, 0, 0, 0);
	backend::ref().reset_depth_stencil_target();
	backend::ref().clear_render_targets( vostok::math::color( 1.0f, 1.0f, 1.0f, 0.0f));

	float2	p0( 0,0);
	float2	p1( 1,1);

	u32		offset;

	float3 const* eye_rays = context->get_eye_rays();

	// Fill vertex buffer
	vertex_formats::Tquad* pv = backend::ref().vertex.lock<vertex_formats::Tquad>( 4, offset);
	pv->set( 0, h, z, 1.0, eye_rays[1].x, eye_rays[1].y, eye_rays[1].z, p0.x, p1.y); pv++;
	pv->set( 0, 0, z, 1.0, eye_rays[0].x, eye_rays[0].y, eye_rays[0].z, p0.x, p0.y); pv++;
	pv->set( w, h, z, 1.0, eye_rays[3].x, eye_rays[3].y, eye_rays[3].z, p1.x, p1.y); pv++;
	pv->set( w, 0, z, 1.0, eye_rays[2].x, eye_rays[2].y, eye_rays[2].z, p1.x, p0.y); pv++;
	backend::ref().vertex.unlock();

	context->m_g_quad_eye_ray->apply();

	backend::ref().render_indexed( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 2*3, 0, offset);
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

	// FUNCTION BODY[0x62d800]
	// claude@NOTE: legacy is_pre_pass ctor argument became the m_is_pre_pass member (init false);
	// the new sun/shadow/translucency/wind constant hosts have no legacy ancestor - matcher-phase
	m_object_transparency_scale_parameter	= backend::ref().register_constant_host("object_transparency_scale", rc_float);
	m_c_start_corner						= backend::ref().register_constant_host("start_corner", rc_float);
	m_far_fog_color_and_distance			= backend::ref().register_constant_host("far_fog_color_and_distance", rc_float);
	m_near_fog_distance						= backend::ref().register_constant_host("near_fog_distance", rc_float);
	m_ambient_color							= backend::ref().register_constant_host("ambient_color", rc_float );
	m_c_environment_skylight_upper_color	= backend::ref().register_constant_host("environment_skylight_upper_color", rc_float);
	m_c_environment_skylight_lower_color	= backend::ref().register_constant_host("environment_skylight_lower_color", rc_float);
	m_c_environment_skylight_parameters		= backend::ref().register_constant_host("environment_skylight_parameters", rc_float);
	m_c_gs_test_constant					= backend::ref().register_constant_host("gs_test_constant", rc_float);

	m_enabled								= options::ref().current.m_enabled_g_stage;

	if (m_enabled)
	{
		if (m_is_pre_pass)
			m_enabled						= options::ref().current.m_enabled_g_stage_pre_pass;
		else
			m_enabled						= options::ref().current.m_enabled_g_stage_material_pass;
	}
}

stage_gbuffer::~stage_gbuffer( )
{
	// FUNCTION BODY[0x62ce50]
}

bool stage_gbuffer::is_effects_ready( ) const
{
	// claude@NOTE: no legacy ancestor - absent from the legacy stage_gbuffer remainder (only execute survives there); matcher-phase work.
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
	// claude@NOTE: no legacy ancestor - absent from the legacy stage_gbuffer remainder; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x62d1d0]
}

bool remove_model_if_not_lod_predicate::operator()( render_surface_instance* )
{
	// claude@NOTE: no legacy ancestor - remove/sort predicates postdate the legacy corpus; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x72510]
	return false;
}

bool remove_model_if_not_static_predicate::operator()( render_surface_instance* )
{
	// claude@NOTE: no legacy ancestor - remove/sort predicates postdate the legacy corpus; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x72580]
	return false;
}

bool remove_model_if_not_skeletal_predicate::operator()( render_surface_instance* )
{
	// claude@NOTE: no legacy ancestor - remove/sort predicates postdate the legacy corpus; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x725e0]
	return false;
}

bool remove_model_if_not_translucency_predicate::operator()( render_surface_instance* )
{
	// claude@NOTE: no legacy ancestor - remove/sort predicates postdate the legacy corpus; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x72630]
	return false;
}

bool sort_by_ps_predicate::operator()(
	render_surface_instance const*,
	render_surface_instance const*
) const
{
	// claude@NOTE: no legacy ancestor - remove/sort predicates postdate the legacy corpus; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x64d60]
	return false;
}

void stage_gbuffer::z_only_pass( )
{
	// claude@NOTE: no legacy ancestor - absent from the legacy stage_gbuffer remainder; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x62e060]
}

void stage_gbuffer::execute( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x62e300]
	// claude@NOTE: legacy monolithic execute blocked - dropped m_dynamic_visuals member,
	// select_models signature drift, and the shipped split into render_models/render_grass/
	// render_particles/render_speedtree + predicates; legacy loop kept in temp/render_legacy remainder
}

void stage_gbuffer::render_grass( bool )
{
	// STATE[STUB]
	// FUNCTION BODY[0x62e010]
}

void stage_gbuffer::render_particles( bool )
{
	// claude@NOTE: no legacy ancestor - absent from the legacy stage_gbuffer remainder; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x62d460]
}

void stage_gbuffer::render_speedtree( )
{
	// claude@NOTE: no legacy ancestor - absent from the legacy stage_gbuffer remainder; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x62ce20]
}

} // namespace render
} // namespace vostok
