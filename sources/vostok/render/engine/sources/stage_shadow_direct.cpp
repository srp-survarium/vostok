#include "pch.h"
#include "stage_shadow_direct.h"

#include "remove_inappropriate_models.h"
#include "remove_model_if_in_frustum_predicate.h"
#include "shadow_cascade_volume.h"

#include <vostok/render/core/backend.h>
#include <vostok/render/core/effect_manager.h>
#include <vostok/render/core/options.h>
#include <vostok/render/core/res_effect.h>
#include "effect_shadow_map.h"
#include "renderer_context.h"

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
	// FUNCTION BODY[0x627de0]
	// claude@NOTE: legacy create_cascaded_shadow_map_buffers(1024) dropped - the canonical class
	// keeps a SINGLE m_rt_shadow_map/m_t_shadow_map (per-cascade re-render) instead of 4 buffers
	effect_manager::ref().create_effect<effect_shadow_map>(&m_effect_shadow_direct);

	m_c_light_direction		= backend::ref().register_constant_host( "light_direction", rc_float );
	m_c_light_position		= backend::ref().register_constant_host( "light_position", rc_float );
//	m_sunmask				= backend::ref().register_constant_host( "sunmask");
	m_c_light_attenuation_power	= backend::ref().register_constant_host( "light_attenuation", rc_float );
	m_c_start_corner		= backend::ref().register_constant_host("start_corner", rc_float);
	m_enabled				= options::ref().current.m_enabled_sun_shadows_stage;
}

bool stage_shadow_direct::is_effects_ready( ) const
{
	// FUNCTION BODY[0x6272e0]
	return m_effect_shadow_direct.c_ptr() != NULL;
}

stage_shadow_direct::~stage_shadow_direct( )
{
	// FUNCTION BODY[0x6278b0]
}

void stage_shadow_direct::execute_disabled( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x627cd0]
	// claude@NOTE: legacy cleared the 4 per-cascade depth targets; canonical keeps one
	// m_rt_shadow_map - matcher-phase
}

void stage_shadow_direct::execute( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6297a0]
}

#if 0
static void draw_line(float3 const& start, float3 const& end, vostok::math::color clr)
{
	float3 line[2];
	line[0] = start;
	line[1] = end;
	system_renderer::ref().draw_screen_lines(line, 2, clr, 1, 0xffffffff, true, false);
}
#endif // #if 0

void stage_shadow_direct::draw_debug( u32 in_cascade_id )
{
	// FUNCTION BODY[0x627890]
	(void)&in_cascade_id;

#if 0

	enum_sun_cascade cascade_id = (enum_sun_cascade)in_cascade_id;

	light*	sun = &*m_context->scene()->lights().get_sun().c_ptr();
	if (!sun)
		return;

	float const size						= m_context->m_sun_cascades[cascade_id].size;

	// TODO: global variable?
	float const sun_distance				= 300.f;

	float3 sun_position						=	m_context->get_view_pos() - sun->direction*sun_distance;
	float3 tmp_vec							=	m_context->get_view_pos() - sun_position;

	float4x4 light_view_transform			=	create_camera_direction( sun_position, sun->direction, float3( 1, 0, 0) /*this need to be north direction*/);
	float4x4 light_projection_transform		=	math::create_orthographic_projection( size, size, 0.1f, sun_distance +/*sqrt(2)*/1.41421f*size );

	shadow_cascade_volume::cascade_volume_planes cull_planes;
	float3	light_shift_xz( 0.f, 0.f, 0.f);

	shadow_cascade_volume	cascade_volume;

	//initialize shadow_cascade_volume
	//////////////////////////////////////////////////////////////////////////

	if ( cascade_id == 0  || m_context->m_sun_cascades[cascade_id].reset_chain )
		for( int i = 0; i < 4; i++)
		{
			float3 eye_ray = m_context->get_eye_rays()[i];
			eye_ray.normalize();
			eye_ray = m_context->get_v_inverted().transform_direction(eye_ray);
			cascade_volume.view_frustum_rays.push_back	( ray( m_context->get_view_pos() + m_context->get_near()*eye_ray, eye_ray) );
		}
	else
		cascade_volume.view_frustum_rays	= m_context->m_sun_cascades[cascade_id].rays;

	cascade_volume.view_ray.origin			= m_context->get_view_pos();
	cascade_volume.view_ray.direction		= m_context->get_view_dir();
	cascade_volume.light_ray.origin			= sun_position;
	cascade_volume.light_ray.direction		= sun->direction;

	float4x4 light_full_transform = light_view_transform * light_projection_transform;

	float4x4 light_full_transform_invert = invert4x3( light_full_transform);

	for	( int p = 0; p < 8; p++)	{
		float4	xf	= light_full_transform_invert.transform( float4( corners[p], 1.f));
		cascade_volume.light_cuboid_points[p] = xf.xyz();
	}

	{
		static math::color colors_x[12] =
		{
			math::color(1.0f, 0.0f, 0.0f, 1.f),
			math::color(0.0f, 1.0f, 0.0f, 1.f),
			math::color(0.0f, 0.0f, 1.0f, 1.f),
			math::color(1.0f, 0.0f, 1.0f, 1.f),
			math::color(0.5f, 0.0f, 0.0f, 1.f),
			math::color(0.0f, 0.5f, 0.0f, 1.f),
			math::color(0.0f, 0.0f, 0.5f, 1.f),
			math::color(0.5f, 0.0f, 0.5f, 1.f),
			math::color(0.5f, 0.5f, 0.5f, 1.f),
			math::color(0.5f, 0.5f, 0.0f, 1.f),
			math::color(1.0f, 1.0f, 0.0f, 1.f),
			math::color(1.0f, 1.0f, 1.0f, 1.f),
		};

		draw_line(cascade_volume.light_cuboid_points[0], cascade_volume.light_cuboid_points[1], colors_x[0]);
		draw_line(cascade_volume.light_cuboid_points[0], cascade_volume.light_cuboid_points[3], colors_x[1]);
		draw_line(cascade_volume.light_cuboid_points[0], cascade_volume.light_cuboid_points[7], colors_x[2]);
		draw_line(cascade_volume.light_cuboid_points[1], cascade_volume.light_cuboid_points[2], colors_x[3]);
		draw_line(cascade_volume.light_cuboid_points[1], cascade_volume.light_cuboid_points[6], colors_x[4]);
		draw_line(cascade_volume.light_cuboid_points[2], cascade_volume.light_cuboid_points[3], colors_x[5]);
		draw_line(cascade_volume.light_cuboid_points[2], cascade_volume.light_cuboid_points[4], colors_x[6]);
		draw_line(cascade_volume.light_cuboid_points[3], cascade_volume.light_cuboid_points[5], colors_x[7]);
		draw_line(cascade_volume.light_cuboid_points[4], cascade_volume.light_cuboid_points[5], colors_x[8]);
		draw_line(cascade_volume.light_cuboid_points[4], cascade_volume.light_cuboid_points[6], colors_x[9]);
		draw_line(cascade_volume.light_cuboid_points[5], cascade_volume.light_cuboid_points[7], colors_x[10]);
		draw_line(cascade_volume.light_cuboid_points[6], cascade_volume.light_cuboid_points[7], colors_x[11]);
	}

	// only side planes
	for (int plane=0; plane < 4; plane++)
		for (int pt=0; pt < 4; pt++)
		{
			int asd = facetable[plane][pt];
			cascade_volume.light_cuboid_polys[plane].points[pt] = asd;
		}

	cascade_volume.compute_caster_model_fixed( cull_planes, light_shift_xz, size, true, true, true, true);
	//light_shift_xz *= 0.0f;

	float3 adjastment		= compute_aligment( light_shift_xz, light_view_transform * light_projection_transform, 1024);

	for (u32 it=0; it< LIGHT_CUBOIDSIDEPOLYS_COUNT; it++)
	{
		shadow_cascade_volume::polygon&	poly	=	cascade_volume.light_cuboid_polys[it];
		float3 line[2];
		line[0] = float3(0.0f, 0.0f, 0.0f);
		line[1] = poly.plane.normal * poly.plane.d;
		static math::color colors_x[4] =
		{
			math::color(1.0f, 0.0f, 0.0f, 1.f),
			math::color(0.0f, 1.0f, 0.0f, 1.f),
			math::color(0.0f, 0.0f, 1.0f, 1.f),
			math::color(1.0f, 0.0f, 1.0f, 1.f),
		};
		system_renderer::ref().draw_screen_lines(line, 2, colors_x[it], 1, 0xffffffff, true, false);
	}

	float3 line[2];
	line[0] = float3(0.0f, 0.0f, 0.0f);
	line[1] = cascade_volume.view_ray.direction * 3.0f;
	system_renderer::ref().draw_screen_lines(line, 2, math::color(1.0f, 1.0f, 1.0f, 1.0f), 1, 0xffffffff, true, false);

	{
		draw_line(light_shift_xz + cascade_volume.light_cuboid_points[0], light_shift_xz + cascade_volume.light_cuboid_points[1], vostok::math::color(1.0f, 1.0f, 0.0f, 1.0f));
		draw_line(light_shift_xz + cascade_volume.light_cuboid_points[0], light_shift_xz + cascade_volume.light_cuboid_points[3], vostok::math::color(1.0f, 1.0f, 0.0f, 1.0f));
		draw_line(light_shift_xz + cascade_volume.light_cuboid_points[0], light_shift_xz + cascade_volume.light_cuboid_points[7], vostok::math::color(1.0f, 1.0f, 0.0f, 1.0f));
		draw_line(light_shift_xz + cascade_volume.light_cuboid_points[1], light_shift_xz + cascade_volume.light_cuboid_points[2], vostok::math::color(1.0f, 1.0f, 0.0f, 1.0f));
		draw_line(light_shift_xz + cascade_volume.light_cuboid_points[1], light_shift_xz + cascade_volume.light_cuboid_points[6], vostok::math::color(1.0f, 1.0f, 0.0f, 1.0f));
		draw_line(light_shift_xz + cascade_volume.light_cuboid_points[2], light_shift_xz + cascade_volume.light_cuboid_points[3], vostok::math::color(1.0f, 1.0f, 0.0f, 1.0f));
		draw_line(light_shift_xz + cascade_volume.light_cuboid_points[2], light_shift_xz + cascade_volume.light_cuboid_points[4], vostok::math::color(1.0f, 1.0f, 0.0f, 1.0f));
		draw_line(light_shift_xz + cascade_volume.light_cuboid_points[3], light_shift_xz + cascade_volume.light_cuboid_points[5], vostok::math::color(1.0f, 1.0f, 0.0f, 1.0f));
		draw_line(light_shift_xz + cascade_volume.light_cuboid_points[4], light_shift_xz + cascade_volume.light_cuboid_points[5], vostok::math::color(1.0f, 1.0f, 0.0f, 1.0f));
		draw_line(light_shift_xz + cascade_volume.light_cuboid_points[4], light_shift_xz + cascade_volume.light_cuboid_points[6], vostok::math::color(1.0f, 1.0f, 0.0f, 1.0f));
		draw_line(light_shift_xz + cascade_volume.light_cuboid_points[5], light_shift_xz + cascade_volume.light_cuboid_points[7], vostok::math::color(1.0f, 1.0f, 0.0f, 1.0f));
		draw_line(light_shift_xz + cascade_volume.light_cuboid_points[6], light_shift_xz + cascade_volume.light_cuboid_points[7], vostok::math::color(1.0f, 1.0f, 0.0f, 1.0f));

		light_shift_xz += adjastment;
		draw_line(light_shift_xz + cascade_volume.light_cuboid_points[0], light_shift_xz + cascade_volume.light_cuboid_points[1], vostok::math::color(1.0f, 0.0f, 0.0f, 1.0f));
		draw_line(light_shift_xz + cascade_volume.light_cuboid_points[0], light_shift_xz + cascade_volume.light_cuboid_points[3], vostok::math::color(1.0f, 0.0f, 0.0f, 1.0f));
		draw_line(light_shift_xz + cascade_volume.light_cuboid_points[0], light_shift_xz + cascade_volume.light_cuboid_points[7], vostok::math::color(1.0f, 0.0f, 0.0f, 1.0f));
		draw_line(light_shift_xz + cascade_volume.light_cuboid_points[1], light_shift_xz + cascade_volume.light_cuboid_points[2], vostok::math::color(1.0f, 0.0f, 0.0f, 1.0f));
		draw_line(light_shift_xz + cascade_volume.light_cuboid_points[1], light_shift_xz + cascade_volume.light_cuboid_points[6], vostok::math::color(1.0f, 0.0f, 0.0f, 1.0f));
		draw_line(light_shift_xz + cascade_volume.light_cuboid_points[2], light_shift_xz + cascade_volume.light_cuboid_points[3], vostok::math::color(1.0f, 0.0f, 0.0f, 1.0f));
		draw_line(light_shift_xz + cascade_volume.light_cuboid_points[2], light_shift_xz + cascade_volume.light_cuboid_points[4], vostok::math::color(1.0f, 0.0f, 0.0f, 1.0f));
		draw_line(light_shift_xz + cascade_volume.light_cuboid_points[3], light_shift_xz + cascade_volume.light_cuboid_points[5], vostok::math::color(1.0f, 0.0f, 0.0f, 1.0f));
		draw_line(light_shift_xz + cascade_volume.light_cuboid_points[4], light_shift_xz + cascade_volume.light_cuboid_points[5], vostok::math::color(1.0f, 0.0f, 0.0f, 1.0f));
		draw_line(light_shift_xz + cascade_volume.light_cuboid_points[4], light_shift_xz + cascade_volume.light_cuboid_points[6], vostok::math::color(1.0f, 0.0f, 0.0f, 1.0f));
		draw_line(light_shift_xz + cascade_volume.light_cuboid_points[5], light_shift_xz + cascade_volume.light_cuboid_points[7], vostok::math::color(1.0f, 0.0f, 0.0f, 1.0f));
		draw_line(light_shift_xz + cascade_volume.light_cuboid_points[6], light_shift_xz + cascade_volume.light_cuboid_points[7], vostok::math::color(1.0f, 0.0f, 0.0f, 1.0f));
	}
#endif // #if 0
// 	light_view_transform	=	create_camera_direction(  sun_position  + light_shift_xz/* + adjastment*/, sun->direction, float3( 1, 0, 0) /*this need to be north direction*/);
// 	light_full_transform_invert = invert4x3( light_full_transform);
//
// 	for	( int p = 0; p < 8; p++)	{
// 		float4	xf	= light_full_transform_invert.transform( float4( corners[p], 1.f));
// 		cascade_volume.light_cuboid_points[p] = xf.xyz();
//
// 		float3 line[2];
// 		line[0] = sun_position  + light_shift_xz;
// 		line[1] = xf.xyz();
// 	}

	m_context->set_w					( float4x4().identity() );
}

bool remove_model_if_in_frustum_predicate::operator()(
	render_surface_instance*
)
{
	// claude@NOTE: no legacy ancestor - remove predicates postdate the legacy corpus; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x781d0]
	return false;
}

bool remove_inappropriate_models::operator()( render_surface_instance* )
{
	// claude@NOTE: no legacy ancestor - remove predicates postdate the legacy corpus; matcher-phase work.
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
	// claude@NOTE: no legacy ancestor - absent from the legacy stage_shadow_direct; matcher-phase work.
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
	// claude@NOTE: no legacy ancestor - legacy has only render_terrain (retired) and render_speedtree_instances; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x6282e0]
}

void stage_shadow_direct::render_dynamic_models(
	u32,
	u32,
	float3 const&
)
{
	// claude@NOTE: no legacy ancestor - absent from the legacy stage_shadow_direct; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x6272d0]
}

void stage_shadow_direct::execute_cascade( u32, u32, u32 )
{
	// claude@NOTE: legacy port blocked - legacy body selects into the dropped m_caster_model
	// member via 2-arg select_models (canonical grew to 5 args), renders through retired
	// render_terrain/num_models_cascade_* statistics, uses per-cascade m_rt_shadow_map[4]
	// (canonical keeps ONE), and calls the 3-arg compute_aligment (canonical grew
	// multiplier/granularity); shipped likely routes through prepare_models/render_models;
	// legacy body kept in temp/render_legacy remainder - matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x6287b0]
}

float3 stage_shadow_direct::compute_aligment(
	float3 const&		light_xz_shift,
	float4x4 const&		light_space_transform,
	float				shadow_map_resolution,
	float				multiplier,
	float3&				granularity
)
{
	// FUNCTION BODY[0x6272f0]
	// claude@NOTE: post-legacy multiplier/granularity parameters - not inferable from the legacy
	// 3-arg body, consumed unreferenced
	VOSTOK_UNREFERENCED_PARAMETER(multiplier);
	VOSTOK_UNREFERENCED_PARAMETER(granularity);

	float3 align_origin( 0.f, 0.f, 0.f);

	// Moves align point near to view point
// 	align_origin = m_context->get_view_pos();
// 	const float		align_aim_step_coef = 8.f;
// 	align_origin.set( math::floor(align_origin.x/align_aim_step_coef)+align_aim_step_coef/2, math::floor(align_origin.y/align_aim_step_coef)+align_aim_step_coef/2, math::floor(align_origin.z/align_aim_step_coef)+align_aim_step_coef/2);
// 	align_origin *=	align_aim_step_coef;
	align_origin -= light_xz_shift;

	float4x4 viewport(	float4(	shadow_map_resolution/2.f,	0.0f,							0.0f,		0.0f),
						float4(	0.0f,						-shadow_map_resolution/2.f,		0.0f,		0.0f),
						float4(	0.0f,						0.0f,							1.0f,		0.0f),
						float4(	shadow_map_resolution/2.f,	shadow_map_resolution/2.f,		0.0f,		1.0f));

	float4x4 viewport_invert				= math::invert4x3( viewport);
	float4x4 light_space_transform_invert	= math::invert4x3( light_space_transform);

	float4	origin_pixel	= light_space_transform.transform( float4( align_origin, 1.f) );
	origin_pixel			*= (1.f/origin_pixel.w);
	origin_pixel			= viewport.transform( origin_pixel );

	const float	align_granularity = 4.f;

	origin_pixel.x	= origin_pixel.x / align_granularity - floorf	(origin_pixel.x / align_granularity);
	origin_pixel.y	= origin_pixel.y / align_granularity - floorf	(origin_pixel.y / align_granularity);
	origin_pixel.x	*= align_granularity;
	origin_pixel.y	*= align_granularity;
	origin_pixel.z	= 0;

	origin_pixel.xyz() = viewport_invert.transform_direction	(origin_pixel.xyz());
	origin_pixel.xyz() = light_space_transform_invert.transform_direction	(origin_pixel.xyz());

	float3 diff		= origin_pixel.xyz();
	static float sign_test = 1.f;
	diff	*=	sign_test;

	return diff;
}

} // namespace render
} // namespace vostok
