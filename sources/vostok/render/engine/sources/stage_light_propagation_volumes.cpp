#include "pch.h"
#include "stage_light_propagation_volumes.h"

#include "remove_model_if_in_frustum_predicate.h"

#include <vostok/render/core/backend.h>
#include <vostok/render/core/effect_manager.h>
#include <vostok/render/core/effect_options_descriptor.h>
#include <vostok/render/core/options.h>
#include <vostok/render/core/resource_manager.h>
#include <vostok/render/core/res_effect.h>
#include <vostok/render/core/dx11/res_geometry.h>
#include <vostok/render/facade/vertex_input_type.h>
#include "effect_apply_indirect_lighting.h"
#include "effect_downsample_gbuffer.h"
#include "effect_downsample_reflective_shadow_map.h"
#include "effect_fill_reflective_shadow_map.h"
#include "light.h"
#include "radiance_volume.h"
#include "renderer_context.h"
#include "renderer_context_targets.h"
#include "shared_names.h"

namespace vostok {
namespace render {

struct screen_vertex {
	float4	position;
	float2	tc;

	void set( float4 const& in_position, float2 const& in_tc )
	{
		position	= in_position;
		tc			= in_tc;
	}
};

STATIC_SIZE_ASSERT( screen_vertex, 0x18 );

struct remove_lpv_inappropriate_models {
	bool operator()( lpv_render_surface const& )
	{
		return false;
	}
};

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
	// FUNCTION BODY[0x619bf0]
	// claude@NOTE: legacy m_rt_rms_{albedo,normal,position}(_source) render-target members left the
	// canonical class (only m_rms_depth_stencil_source[4] survives - cascade [0] seeded with the
	// legacy name); the per-cascade smoothed pipeline members have no legacy ancestor - matcher-phase
	m_num_cascades				= options::ref().current.m_lpv_num_cascades;

	math::clamp<u32>			(m_num_cascades, 1, 4);

	m_grid_size					= options::ref().current.m_num_radiance_volume_cells;

	m_rsm_source_size			= options::ref().current.m_light_propagation_volumes_rsm_size;
	m_rsm_downsampled_size		= options::ref().current.m_light_propagation_volumes_rsm_size / 2;

	m_radiance_volume			= ALLOC(radiance_volume, m_num_cascades);

	float cascade_scales			[] = {1.0f, 2.0f, 4.0f, 6.0f, };
	float cascade_cells_scales		[] = {1.0f, 1.0f, 1.0f, 1.0f, };
	float cascade_flux_scales		[] = {1.0f, 1.0f, 1.0f, 1.0f, };
	float cascade_iteration_scales	[] = {1.0f, 1.0f / 2.0f, 1.0f / 4.0f, 1.0f / 8.0f, };

	for (u32 cascade_index = 0; cascade_index < m_num_cascades; cascade_index++)
	{
		new(&m_radiance_volume[cascade_index])radiance_volume(
			m_rsm_downsampled_size,
			u32(options::ref().current.m_num_radiance_volume_cells * cascade_cells_scales[cascade_index]),
			u32(options::ref().current.m_num_propagate_iterations * cascade_iteration_scales[cascade_index]),
			options::ref().current.m_radiance_volume_scale * cascade_scales[cascade_index],
			options::ref().current.m_lpv_flux_amplifier * cascade_flux_scales[cascade_index]
		);
	}

	m_rms_depth_stencil_source[0]	= resource_manager::ref().create_render_target("$user$rms_depth_stencil", m_rsm_source_size, m_rsm_source_size, DXGI_FORMAT_R24G8_TYPELESS, enum_rt_usage_depth_stencil, res_texture_ptr( ), 0, D3D11_USAGE_DEFAULT, 1, 0);

	m_c_interreflection_contribution		= backend::ref().register_constant_host("interreflection_contribution", rc_float);
	m_c_cascade_index						= backend::ref().register_constant_host("cascade_index", rc_int);
	m_c_num_cascades						= backend::ref().register_constant_host("num_cascades", rc_int);

	m_c_smaller_cascade_grid_cell_size		= backend::ref().register_constant_host("smaller_cascade_grid_cell_size", rc_float);
	m_c_smaller_cascade_grid_size			= backend::ref().register_constant_host("smaller_cascade_grid_size", rc_float);
	m_c_smaller_cascade_grid_origin			= backend::ref().register_constant_host("smaller_cascade_grid_origin", rc_float);
	m_c_radiance_blend_factor				= backend::ref().register_constant_host("radiance_blend_factor", rc_float);

	register_rsm_constans		();
	register_light_constans		();

	m_c_eye_ray_corner			= backend::ref().register_constant_host("s_eye_ray_corner", rc_float);

	for (u32 i = 0; i < num_vertex_input_types; i++)
	{
		if (i == post_process_vertex_input_type)
			continue;

		byte						data[Kb];
		effect_options_descriptor	desc(data, Kb);
		desc["vertex_input_type"]	= (enum_vertex_input_type)i;

		effect_manager::ref().create_effect<effect_fill_reflective_shadow_map>(&m_fill_rsm_effect[i], desc);
	}

	const D3D_INPUT_ELEMENT_DESC screen_vertex_layout[] =
	{
		{"POSITION",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 0,	D3D_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,	    0, 16,	D3D_INPUT_PER_VERTEX_DATA, 0},
	};

	u16 indices[6]			= { 0, 1, 2, 3, 2, 1 };
	m_screen_vertex_ib		= resource_manager::ref().create_buffer( 6*sizeof(u16), indices, enum_buffer_type_index, false, false);
	m_screen_vertex_geometry	= resource_manager::ref().create_geometry(
		screen_vertex_layout,
		sizeof(screen_vertex),
		backend::ref().vertex.buffer(),
		*m_screen_vertex_ib
	);

	effect_manager::ref().create_effect<effect_downsample_reflective_shadow_map>(&m_downsample_rsm_effect);
	effect_manager::ref().create_effect<effect_apply_indirect_lighting>(&m_apply_indirect_lighting_effect);
	effect_manager::ref().create_effect<effect_downsample_gbuffer>(&m_downsample_gbuffer_effect);

	m_enabled					= options::ref().current.m_enabled_light_propagation_volumes_stage;
}

stage_light_propagation_volumes::~stage_light_propagation_volumes( )
{
	// FUNCTION BODY[0x615980]
	FREE(m_radiance_volume);
}

bool stage_light_propagation_volumes::is_effects_ready( ) const
{
	// FUNCTION BODY[0x6146e0]
	for (u32 i = 0; i < num_vertex_input_types; i++)
	{
		if (i == post_process_vertex_input_type)
			continue;

		if (m_fill_rsm_effect[i].c_ptr() == NULL)
			return false;
	}

	for (u32 cascade_index = 0; cascade_index < m_num_cascades; cascade_index++)
	{
		if (!m_radiance_volume[cascade_index].is_effects_ready())
			return false;
	}

	return m_downsample_rsm_effect.c_ptr() != NULL
		&& m_apply_indirect_lighting_effect.c_ptr() != NULL
		&& m_downsample_gbuffer_effect.c_ptr() != NULL;
}

void stage_light_propagation_volumes::set_rsm_contants(
	float3 const&	light_direction,
	float3 const&	grid_origin,
	float			grid_scale
)
{
	// FUNCTION BODY[0x614cd0]
	backend::ref().set_ps_constant	(m_c_grid_origin, grid_origin);
	backend::ref().set_ps_constant	(m_c_grid_cell_size, grid_scale / float(m_grid_size));
	backend::ref().set_ps_constant	(m_c_invert_rsm_size, 1.0f / float(m_rsm_downsampled_size));
	backend::ref().set_ps_constant	(m_c_light_direction, light_direction);
}

void stage_light_propagation_volumes::register_rsm_constans( )
{
	// FUNCTION BODY[0x615510]
	m_c_grid_origin					= backend::ref().register_constant_host("grid_origin",  rc_float );
	m_c_grid_cell_size				= backend::ref().register_constant_host("grid_cell_size",  rc_float );
	m_c_grid_size					= backend::ref().register_constant_host("grid_size",  rc_float );
	m_c_invert_rsm_size				= backend::ref().register_constant_host("invert_rsm_size",  rc_float );
}

void stage_light_propagation_volumes::register_light_constans( )
{
	// FUNCTION BODY[0x614de0]
	m_c_view_to_light_matrix		= backend::ref().register_constant_host("view_to_light_matrix",  rc_float );
	m_c_light_color					= backend::ref().register_constant_host("light_color", rc_float );
	m_c_light_intensity				= backend::ref().register_constant_host("light_intensity", rc_float );
	m_c_light_position				= backend::ref().register_constant_host("light_position", rc_float );
	m_c_light_direction				= backend::ref().register_constant_host("light_direction", rc_float );
	m_c_light_attenuation_power		= backend::ref().register_constant_host("light_attenuation_power", rc_float );
	m_c_light_range					= backend::ref().register_constant_host("light_range", rc_float );
	m_c_lighting_model				= backend::ref().register_constant_host("lighting_model", rc_int );
	m_c_diffuse_influence_factor	= backend::ref().register_constant_host("light_diffuse_influence_factor", rc_float );
	m_c_specular_influence_factor	= backend::ref().register_constant_host("light_specular_influence_factor", rc_float );
	m_c_light_spot_penumbra_half_angle_cosine	= backend::ref().register_constant_host( "light_spot_penumbra_half_angle_cosine", rc_float );
	m_c_light_spot_umbra_half_angle_cosine		= backend::ref().register_constant_host( "light_spot_umbra_half_angle_cosine", rc_float );
	m_c_light_spot_inversed_umbra_half_angle_cosine_minus_penumbra_half_angle_cosine
		= backend::ref().register_constant_host( "light_spot_inversed_umbra_half_angle_cosine_minus_penumbra_half_angle_cosine", rc_float );
	m_c_light_spot_falloff			= backend::ref().register_constant_host("light_spot_falloff", rc_float );
	m_c_light_type					= backend::ref().register_constant_host("light_type", rc_int );
	m_c_light_capsule_half_width	= backend::ref().register_constant_host("light_capsule_half_width", rc_float );
	m_c_light_capsule_radius			= backend::ref().register_constant_host("light_capsule_radius", rc_float );
	m_c_light_sphere_radius			= backend::ref().register_constant_host("light_sphere_radius", rc_float );
	m_c_light_local_to_world		= backend::ref().register_constant_host("light_local_to_world", rc_float );
	m_c_light_sphere_radius			= backend::ref().register_constant_host("light_sphere_radius", rc_float );
}

void stage_light_propagation_volumes::pre_lpv_batch_render(
	float3 const&,
	float,
	geometry_batch const&
)
{
	// claude@NOTE: no legacy ancestor - batch-render hooks absent from the legacy stage_light_propagation_volumes; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x6168f0]
}

void stage_light_propagation_volumes::post_lpv_batch_render( geometry_batch const& )
{
	// claude@NOTE: no legacy ancestor - batch-render hooks absent from the legacy stage_light_propagation_volumes; matcher-phase work.
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
	// claude@NOTE: legacy render_to_rms (and the render_to_sun/point/sky/spot_rms family) is blocked -
	// the m_rt_rms_* source targets left the class, select_models grew to 5 args, and the shipped
	// per-cascade transforms/smoothed pipeline is post-legacy; legacy bodies kept in the
	// temp/render_legacy remainder
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
	// claude@NOTE: no legacy ancestor - legacy has only render_to_rms, no smoothed variants; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x615f70]
}

static float3 compute_aligment( float3 const& lightXZshift, float4x4 const& light_space_transform, float smap_res )
{
	// FUNCTION BODY[0x614760]
	float3 align_origin				(0.0f, 0.0f, 0.0f);

	align_origin					-= lightXZshift;

	float4x4 viewport(	float4(smap_res/2.f,	0.0f,				0.0f,		0.0f),
						float4(0.0f,			-smap_res/2.f,		0.0f,		0.0f),
						float4(0.0f,			0.0f,				1.0f,		0.0f),
						float4(smap_res/2.f,	smap_res/2.f,		0.0f,		1.0f));

	float4x4 viewport_invert				= math::invert4x3(viewport);
	float4x4 light_space_transform_invert	= math::invert4x3(light_space_transform);

	float4	origin_pixel			= light_space_transform.transform(float4(align_origin, 1.f));
	origin_pixel					*= (1.f/origin_pixel.w);
	origin_pixel					= viewport.transform(origin_pixel);

	float const align_granularity	= 4.f;

	origin_pixel.x					= origin_pixel.x / align_granularity - floorf	(origin_pixel.x / align_granularity);
	origin_pixel.y					= origin_pixel.y / align_granularity - floorf	(origin_pixel.y / align_granularity);
	origin_pixel.x					*= align_granularity;
	origin_pixel.y					*= align_granularity;
	origin_pixel.z					= 0;

	origin_pixel.xyz()				= viewport_invert.transform_direction(origin_pixel.xyz());
	origin_pixel.xyz()				= light_space_transform_invert.transform_direction(origin_pixel.xyz());

	return origin_pixel.xyz();
}

void stage_light_propagation_volumes::render_to_sun_rms(
	light*				sun,
	u32					cascade_index,
	vector<float4x4>	transforms
)
{
	// FUNCTION BODY[0x6188a0]
	// claude@NOTE: post-legacy transforms vector forwarded to render_to_rms (which grew the same
	// trailing transforms/cascade_index parameters)
	float max_scale					= m_radiance_volume[cascade_index].get_scale();

	float3 sun_position				= m_context->get_view_pos() - sun->direction * max_scale * 1.41421f * 2.0f;// * 20.0f;

	float4x4 view_matrix			= create_camera_direction(sun_position, sun->direction, float3(1, 0, 0));
	float4x4 projection_matrix		= math::create_orthographic_projection(
		max_scale,
		max_scale,
		0.01f,
		1.41421f * max_scale * 20.0f
	);

	float3 adjastment				= compute_aligment(float3(0.0f, 0.0f, 0.0f), view_matrix * projection_matrix, m_rsm_source_size);
	view_matrix						= create_camera_direction(sun_position + adjastment, sun->direction, float3(1, 0, 0));

	render_to_rms					(sun->color, sun->intensity, view_matrix, projection_matrix, transforms, cascade_index);
}

void stage_light_propagation_volumes::render_to_sun_rms_smoothed(
	light*,
	u32,
	vector<float4x4>,
	u32,
	u32
)
{
	// claude@NOTE: no legacy ancestor - legacy has no smoothed variant; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x6169b0]
}

static float3 view_matrix_parameters[6][3] = {
	{float3(0.0f, 0.0f, 0.0f), float3(1.0f, 0.0f, 0.0f),	float3(0.0f, 1.0f, 0.0f)},	//  x
	{float3(0.0f, 0.0f, 0.0f), float3(-1.0f, 0.0f, 0.0f),	float3(0.0f, 1.0f, 0.0f)},	// -x
	{float3(0.0f, 0.0f, 0.0f), float3(0.0f, 1.0f, 0.0f),	float3(0.0f, 0.0f, -1.0f)},	//  y
	{float3(0.0f, 0.0f, 0.0f), float3(0.0f, -1.0f, 0.0f),	float3(0.0f, 0.0f, 1.0f)},	// -y
	{float3(0.0f, 0.0f, 0.0f), float3(0.0f, 0.0f, 1.0f),	float3(0.0f, 1.0f, 0.0f)},	//  z
	{float3(0.0f, 0.0f, 0.0f), float3(0.0f, 0.0f, -1.0f),	float3(0.0f, 1.0f, 0.0f)},	// -z
};

void stage_light_propagation_volumes::render_to_point_rms(
	light*				l,
	u32					face_index,
	vector<float4x4>	transforms
)
{
	// FUNCTION BODY[0x618750]
	// claude@NOTE: post-legacy transforms vector forwarded to render_to_rms; cascade_index 0 assumed
	// (legacy render_to_rms had no cascade; only m_rms_depth_stencil_source[0] is seeded)
	math::float4x4 face_view_matrix = math::create_camera_at(
		l->position + view_matrix_parameters[face_index][0],
		l->position + view_matrix_parameters[face_index][1],
		view_matrix_parameters[face_index][2]
	);

	math::float4x4 face_projection_matrix = math::create_perspective_projection(math::pi_d2, 1.0f, 0.1f, l->range);

	render_to_rms					(l->color, l->intensity, face_view_matrix, face_projection_matrix, transforms, 0);
}

void stage_light_propagation_volumes::render_to_sky_rms(
	light*,
	u32,
	u32,
	vector<float4x4>
)
{
	// claude@NOTE: legacy body diverged - target prepends a light* the legacy body never had (legacy sources sky color from post-process parameters); per-face ortho core still reusable; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x618200]
}

void stage_light_propagation_volumes::render_to_spot_rms( light* l, vector<float4x4> transforms )
{
	// FUNCTION BODY[0x618010]
	// claude@NOTE: post-legacy transforms vector forwarded to render_to_rms; cascade_index 0 assumed
	// (legacy render_to_rms had no cascade; only m_rms_depth_stencil_source[0] is seeded)
	float const max_angle			 = math::max(l->spot_umbra_angle, l->spot_penumbra_angle);

	math::float4x4 projection_matrix = math::create_perspective_projection(max_angle, 1.0f, l->range / 1000.0f, l->range);

	math::float4x4 view_matrix		 = math::create_camera_at(l->position,
															  l->position + l->direction,
															  math::normalize(math::cross_product(l->direction, l->right)));

	render_to_rms					(l->color, l->intensity, view_matrix, projection_matrix, transforms, 0);
}

void stage_light_propagation_volumes::propagate_lighting( u32 cascade_index )
{
	// FUNCTION BODY[0x615f50]
	m_radiance_volume[cascade_index].propagate_lighting(cascade_index);
}

void stage_light_propagation_volumes::propagate_lighting_smoothed( u32, u32 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x615f30]
	// claude@NOTE: no legacy ancestor - the smoothed (time-sliced) propagation path is post-legacy
}

void stage_light_propagation_volumes::render_quad( )
{
	// FUNCTION BODY[0x615760]
	u32	offset;

	screen_vertex* pv = (screen_vertex*)backend::ref().vertex.lock(4, sizeof(screen_vertex), offset);
	pv->set(float4(-1.0f, -1.0f, 0.0f, 1.0f), float2(0.0f, 1.0f)); pv++;
	pv->set(float4(-1.0f,  1.0f, 0.0f, 1.0f), float2(0.0f, 0.0f)); pv++;
	pv->set(float4( 1.0f, -1.0f, 0.0f, 1.0f), float2(1.0f, 1.0f)); pv++;
	pv->set(float4( 1.0f,  1.0f, 0.0f, 1.0f), float2(1.0f, 0.0f)); pv++;
	backend::ref().vertex.unlock			();

	m_screen_vertex_geometry->apply			();
	backend::ref().render_indexed			(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 2*3, 0, offset);
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
	// claude@NOTE: legacy body wrote to the dropped m_rt_rms_{albedo,normal,position} members
	// (and the canonical adds a cascade u32) - legacy body kept in temp/render_legacy remainder
}

void stage_light_propagation_volumes::inject_lighting(
	u32				cascade_index,
	float3 const&	light_position,
	float3 const&	light_direction,
	float			light_fov
)
{
	// FUNCTION BODY[0x615d20]
	m_radiance_volume[cascade_index].inject_lighting(light_position, light_direction, light_fov, m_rsm_downsampled_size);
}

void stage_light_propagation_volumes::inject_occluders(
	u32				cascade_index,
	float3 const&	light_position,
	float3 const&	light_direction,
	vector<float4x4>
)
{
	// FUNCTION BODY[0x615cc0]
	// claude@NOTE: trailing transforms parameter is post-legacy (feeds inject_occluder_geometry) -
	// unused in the ported legacy body
	m_radiance_volume[cascade_index].inject_occluders(m_context, light_position, light_direction, m_rsm_downsampled_size);
}

void stage_light_propagation_volumes::execute_smoothed_impl( u32, u32, u32, u32, u32 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6173c0]
}

void stage_light_propagation_volumes::execute_impl( )
{
	// claude@NOTE: no legacy ancestor - shipped execute was split into execute_impl/execute_smoothed_impl; legacy has only the monolithic execute; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x618b60]
}

void stage_light_propagation_volumes::execute_disabled( )
{
	// claude@NOTE: no legacy ancestor - execute_disabled appears in the legacy file only as a call site, never defined; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x615690]
}

void stage_light_propagation_volumes::execute( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x619b30]
	// claude@NOTE: legacy monolithic execute blocked - shipped split into execute_impl/
	// execute_smoothed_impl over the per-cascade caster-model caches; legacy body kept in the
	// temp/render_legacy remainder
}

void stage_light_propagation_volumes::draw_debug( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6146d0]
}

} // namespace render
} // namespace vostok
