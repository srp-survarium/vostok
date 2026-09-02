// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include "organic_lighting_passes.h"
#include "stage_lights.h"
#include "light.h"
#include <vostok/geometry_utils.h>
#include <vostok/geometry_primitives.h>
#include <vostok/render/core/backend.h>
#include <vostok/render/core/effect_manager.h>
#include <vostok/render/core/options.h>
#include <vostok/render/core/resource_manager.h>
#include <vostok/render/core/res_effect.h>
#include "capsule_light_effect.h"
#include "du_sphere.h"
#include "effect_downsample_skin_irradiance_texture.h"
#include "environment_probe.h"
#include "effect_fix_irradiance_texture.h"
#include "effect_light_mask.h"
#include "effect_shadow_map.h"
#include "obb_light_effect.h"
#include "plane_spot_light_effect.h"
#include "point_light_effect.h"
#include "shared_names.h"
#include "sphere_light_effect.h"
#include "spot_light_effect.h"

#include <vostok/collision/geometry_instance.h>
#include <vostok/collision/space_partitioning_tree.h>
#include <vostok/console_command.h>
#include <vostok/console_command_processor.h>
#include <vostok/particle/world.h>
#include <vostok/render/engine/vertex_colored.h>
#include <vostok/render/facade/particles.h>
#include <vostok/render/facade/render_stage_types.h>

#include "lights_db.h"
#include "material_effects.h"
#include "render_geometry.h"
#include "render_particle_emitter_instance.h"
#include "render_surface.h"
#include "render_surface_instance.h"
#include "renderer_context.h"
#include "renderer_context_targets.h"
#include "scene.h"
#include "scene_view.h"
#include "speedtree_forest.h"
#include "speedtree_tree.h"
#include "stage_particles.h"
#include "statistics.h"
#include "system_renderer.h"

static float s_shadow_z_near_value = 0.025f;
static vostok::console_commands::cc_float s_shadow_z_near("shadow_z_near", s_shadow_z_near_value, 0.01f, 1.0f, false, vostok::console_commands::command_type_engine_internal);

static bool s_one_light_dip_value = true;
static vostok::console_commands::cc_bool s_one_light_dip("one_light_dip", s_one_light_dip_value, false, vostok::console_commands::command_type_engine_internal);

static bool s_lights_cull_value = true;
static vostok::console_commands::cc_bool s_lights_cull("lights_cull", s_lights_cull_value, false, vostok::console_commands::command_type_engine_internal);

static bool s_draw_lights_value = false;
static vostok::console_commands::cc_bool s_draw_lights("draw_lights", s_draw_lights_value, false, vostok::console_commands::command_type_engine_internal);

static bool s_alpha_blending_value = true;
static vostok::console_commands::cc_bool s_alpha_blending("alpha_blending", s_alpha_blending_value, false, vostok::console_commands::command_type_engine_internal);

static vostok::math::float3 view_matrix_parameters[6][3] = {
	{vostok::math::float3(0.0f, 0.0f, 0.0f), vostok::math::float3(1.0f, 0.0f, 0.0f),	vostok::math::float3(0.0f, 1.0f, 0.0f)},	//  x
	{vostok::math::float3(0.0f, 0.0f, 0.0f), vostok::math::float3(-1.0f, 0.0f, 0.0f),	vostok::math::float3(0.0f, 1.0f, 0.0f)},	// -x
	{vostok::math::float3(0.0f, 0.0f, 0.0f), vostok::math::float3(0.0f, 1.0f, 0.0f),	vostok::math::float3(0.0f, 0.0f, -1.0f)},	//  y
	{vostok::math::float3(0.0f, 0.0f, 0.0f), vostok::math::float3(0.0f, -1.0f, 0.0f),	vostok::math::float3(0.0f, 0.0f, 1.0f)},	// -y
	{vostok::math::float3(0.0f, 0.0f, 0.0f), vostok::math::float3(0.0f, 0.0f, 1.0f),	vostok::math::float3(0.0f, 1.0f, 0.0f)},	//  z
	{vostok::math::float3(0.0f, 0.0f, 0.0f), vostok::math::float3(0.0f, 0.0f, -1.0f),	vostok::math::float3(0.0f, 1.0f, 0.0f)},	// -z
};

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

D3D11_INPUT_ELEMENT_DESC const instance_data_layout_small[] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TempFloat", 0, DXGI_FORMAT_R32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
};

void stage_lights::new_sphere_geometry( )
{
	m_sphere_geometry.vertex_buffer	= resource_manager::ref().create_buffer( DU_SPHERE_NUMVERTEX*sizeof( float3), du_sphere_vertices, enum_buffer_type_vertex, false, false);
	m_sphere_geometry.index_buffer	= resource_manager::ref().create_buffer( DU_SPHERE_NUMFACES*3*sizeof( u16), du_sphere_faces, enum_buffer_type_index, false, false);

	D3D_INPUT_ELEMENT_DESC desc[]	= {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,	D3D_INPUT_PER_VERTEX_DATA, 0 }
	};
	m_sphere_geometry.geometry		= resource_manager::ref().create_geometry( desc, sizeof(float3), *m_sphere_geometry.vertex_buffer, *m_sphere_geometry.index_buffer);
}

void stage_lights::create_pyramid_geometry( )
{
	float3 vertices[]					= {
		float3(0.f, 0.f, 0.f),
		float3(-1.f,-1.f,1.f),
		float3(-1.f, 1.f,1.f),
		float3( 1.f,-1.f,1.f),
		float3( 1.f, 1.f,1.f),
	};
	u16 indices[]						= {
		0, 1, 2,
		0, 2, 4,
		0, 4, 3,
		0, 3, 1,
		1, 3, 2,
		2, 3, 4,
	};

	m_pyramid_geometry.vertex_buffer	= resource_manager::ref().create_buffer( array_size(vertices)*sizeof(vertices[0]), vertices, enum_buffer_type_vertex, false, false);
	m_pyramid_geometry.index_buffer		= resource_manager::ref().create_buffer( array_size(indices)*sizeof(indices[0]), indices, enum_buffer_type_index, false, false);

	D3D_INPUT_ELEMENT_DESC desc[]		= {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,	D3D_INPUT_PER_VERTEX_DATA, 0 }
	};

	m_pyramid_geometry.geometry			= resource_manager::ref().create_geometry( desc, sizeof(float3), *m_pyramid_geometry.vertex_buffer, *m_pyramid_geometry.index_buffer);
}

void stage_lights::create_obb_geometry( )
{
	m_obb_geometry.vertex_buffer	=
		resource_manager::ref().create_buffer(
			geometry_utils::cube_solid::vertex_count*sizeof(float3),
			geometry_utils::cube_solid::vertices,
			enum_buffer_type_vertex,
			false,
			false
		);
	u32 const buffer_size	= geometry_utils::cube_solid::index_count*sizeof(u16);
	u16* const indices		= static_cast<u16*>( ALLOCA( buffer_size ) );
	std::copy				( geometry_utils::cube_solid::faces, geometry_utils::cube_solid::faces + geometry_utils::cube_solid::index_count, indices );
	m_obb_geometry.index_buffer		=
		resource_manager::ref().create_buffer(
			geometry_utils::cube_solid::index_count*sizeof(u16),
			indices,
			enum_buffer_type_index,
			false,
			false
		);

	D3D_INPUT_ELEMENT_DESC desc[]	= {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,	D3D_INPUT_PER_VERTEX_DATA, 0 }
	};
	m_obb_geometry.geometry			= resource_manager::ref().create_geometry( desc, sizeof(float3), *m_obb_geometry.vertex_buffer, *m_obb_geometry.index_buffer);
}

stage_lights::stage_lights(
	renderer*			in_renderer,
	renderer_context*	context,
	bool				is_forward_lighting_pass
) :
	stage						( in_renderer, context ),
	m_enable_env_probes			( true ),
	m_is_forward_lighting_pass	( is_forward_lighting_pass )
{
	m_enabled						= options::ref().current.m_enabled_lighting_stage;

	if (m_enabled)
	{
		if (is_forward_lighting_pass)
			m_enabled				= options::ref().current.m_enabled_forward_lighting_stage;
		else
			m_enabled				= options::ref().current.m_enabled_deferred_lighting_stage;
	}

	if (options::ref().current.m_enabled_local_light_shadows)
	{
		m_shadow_depth_stencil[0] = resource_manager::ref().create_render_target(r2_rt_shadow_map_size_1024, 1024, 1024, DXGI_FORMAT_R24G8_TYPELESS, enum_rt_usage_depth_stencil, res_texture_ptr( ), 0, D3D11_USAGE_DEFAULT, 1, 0);
		m_shadow_depth_stencil[1] = resource_manager::ref().create_render_target(r2_rt_shadow_map_size_512,   512,  512, DXGI_FORMAT_R24G8_TYPELESS, enum_rt_usage_depth_stencil, res_texture_ptr( ), 0, D3D11_USAGE_DEFAULT, 1, 0);
		m_shadow_depth_stencil[2] = resource_manager::ref().create_render_target(r2_rt_shadow_map_size_256,   256,  256, DXGI_FORMAT_R24G8_TYPELESS, enum_rt_usage_depth_stencil, res_texture_ptr( ), 0, D3D11_USAGE_DEFAULT, 1, 0);
		m_shadow_depth_stencil_texture[0] = resource_manager::ref().create_texture(r2_rt_shadow_map_size_1024, 0, 0, false, true, true, u32(-1));
		m_shadow_depth_stencil_texture[1] = resource_manager::ref().create_texture(r2_rt_shadow_map_size_512, 0, 0, false, true, true, u32(-1));
		m_shadow_depth_stencil_texture[2] = resource_manager::ref().create_texture(r2_rt_shadow_map_size_256, 0, 0, false, true, true, u32(-1));
	}

	effect_manager::ref( ).create_effect<effect_shadow_map>( &m_shadow_effect );
	effect_manager::ref( ).create_effect<effect_light_mask>( &m_effect_accum_mask );
	effect_manager::ref( ).create_effect<point_light_effect<false, false> >( &m_point_light_accumulator );
	effect_manager::ref( ).create_effect<point_light_effect<false, true> >( &m_point_light_shadower );
	effect_manager::ref( ).create_effect<point_light_effect<true, false> >( &m_shadowed_point_light_accumulator );
	effect_manager::ref( ).create_effect<spot_light_effect<false> >( &m_spot_light_accumulator );
	effect_manager::ref( ).create_effect<spot_light_effect<true> >( &m_shadowed_spot_light_accumulator );
	effect_manager::ref( ).create_effect<capsule_light_effect>( &m_capsule_light_accumulator );
	effect_manager::ref( ).create_effect<obb_light_effect<false> >( &m_obb_light_accumulator );
	effect_manager::ref( ).create_effect<obb_light_effect<true> >( &m_shadowed_obb_light_accumulator );
	effect_manager::ref( ).create_effect<sphere_light_effect<false> >( &m_sphere_light_accumulator );
	effect_manager::ref( ).create_effect<sphere_light_effect<true> >( &m_shadowed_sphere_light_accumulator );
	effect_manager::ref( ).create_effect<plane_spot_light_effect<false> >( &m_plane_spot_light_accumulator );
	effect_manager::ref( ).create_effect<plane_spot_light_effect<true> >( &m_shadowed_plane_spot_light_accumulator );
	effect_manager::ref( ).create_effect<effect_downsample_skin_irradiance_texture>( &m_sh_downsample_skin_irradiance_texture );
	effect_manager::ref( ).create_effect<effect_fix_irradiance_texture>( &m_sh_fix_irradiance_texture );

	new_sphere_geometry( );
	create_pyramid_geometry( );
	create_obb_geometry( );

	m_c_view_to_light_matrix = backend::ref( ).register_constant_host( "view_to_light_matrix", rc_float );
	m_c_shadow_z_bias = backend::ref( ).register_constant_host( "shadow_z_bias", rc_float );
	m_c_shadow_map_size = backend::ref( ).register_constant_host( "shadow_map_size", rc_float );
	m_c_shadow_transparency = backend::ref( ).register_constant_host( "shadow_transparency", rc_float );
	m_c_light_color = backend::ref( ).register_constant_host( "light_color", rc_float );
	m_c_light_intensity = backend::ref( ).register_constant_host( "light_intensity", rc_float );
	m_c_light_position = backend::ref( ).register_constant_host( "light_position", rc_float );
	m_c_light_direction = backend::ref( ).register_constant_host( "light_direction", rc_float );
	m_c_light_attenuation_power = backend::ref( ).register_constant_host( "light_attenuation_power", rc_float );
	m_c_light_range = backend::ref( ).register_constant_host( "light_range", rc_float );
	m_c_lighting_model = backend::ref( ).register_constant_host( "lighting_model", rc_int );
	m_c_diffuse_influence_factor = backend::ref( ).register_constant_host( "light_diffuse_influence_factor", rc_float );
	m_c_specular_influence_factor = backend::ref( ).register_constant_host( "light_specular_influence_factor", rc_float );
	m_c_is_shadower = backend::ref( ).register_constant_host( "is_shadower", rc_float );
	m_c_light_spot_penumbra_half_angle_cosine = backend::ref( ).register_constant_host( "light_spot_penumbra_half_angle_cosine", rc_float );
	m_c_light_spot_umbra_half_angle_cosine = backend::ref( ).register_constant_host( "light_spot_umbra_half_angle_cosine", rc_float );
	m_c_light_spot_inversed_umbra_half_angle_cosine_minus_penumbra_half_angle_cosine =
		backend::ref( ).register_constant_host( "light_spot_inversed_umbra_half_angle_cosine_minus_penumbra_half_angle_cosine", rc_float );
	m_c_light_spot_falloff = backend::ref( ).register_constant_host( "light_spot_falloff", rc_float );
	m_c_light_type = backend::ref( ).register_constant_host( "light_type", rc_int );
	m_c_light_capsule_half_width = backend::ref( ).register_constant_host( "light_capsule_half_width", rc_float );
	m_c_light_capsule_radius = backend::ref( ).register_constant_host( "light_capsule_radius", rc_float );
	m_c_light_sphere_radius = backend::ref( ).register_constant_host( "light_sphere_radius", rc_float );
	m_c_light_local_to_world = backend::ref( ).register_constant_host( "light_local_to_world", rc_float );
	m_c_light_sphere_radius = backend::ref( ).register_constant_host( "light_sphere_radius", rc_float );
	m_c_eye_ray_corner = backend::ref( ).register_constant_host( "s_eye_ray_corner", rc_float );
	m_c_near_far = backend::ref( ).register_constant_host( "s_near_far", rc_float );
	m_far_fog_color_and_distance = backend::ref( ).register_constant_host( "far_fog_color_and_distance", rc_float );
	m_near_fog_distance = backend::ref( ).register_constant_host( "near_fog_distance", rc_float );
	m_c_is_unwrap_pass = backend::ref( ).register_constant_host( "is_unwrap_pass", rc_float );
	m_blur_offsets_weights = backend::ref( ).register_constant_host( "offsets_weights", rc_float );
	m_kernel_offsets = backend::ref( ).register_constant_host( "kernel_offsets", rc_float );
	m_c_use_shadows = backend::ref( ).register_constant_host( "use_shadows", rc_int );
	m_ambient_color = backend::ref( ).register_constant_host( "ambient_color", rc_float );
	m_gamma_correction_factor = backend::ref( ).register_constant_host( "gamma_correction_factor", rc_float );
	m_probe_parameters0 = backend::ref( ).register_constant_host( "m_probe_parameters0", rc_float );
	m_probe_parameters1 = backend::ref( ).register_constant_host( "m_probe_parameters1", rc_float );
	m_shadow[0] = backend::ref( ).register_constant_host( "m_shadow0", rc_float );
	m_shadow[1] = backend::ref( ).register_constant_host( "m_shadow1", rc_float );
	m_shadow[2] = backend::ref( ).register_constant_host( "m_shadow2", rc_float );
	m_shadow[3] = backend::ref( ).register_constant_host( "m_shadow3", rc_float );

	D3D11_INPUT_ELEMENT_DESC const screen_vertex_layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	u16 indices[6] = { 0, 1, 2, 3, 2, 1 };
	m_screen_vertex_ib = resource_manager::ref( ).create_buffer(
		sizeof( indices ),
		indices,
		enum_buffer_type_index,
		false,
		false
	);
	m_screen_vertex_geometry = resource_manager::ref( ).create_geometry(
		screen_vertex_layout,
		sizeof( screen_vertex ),
		backend::ref( ).vertex.buffer( ),
		*m_screen_vertex_ib
	);

	m_num_instanced_lights = options::ref( ).current.m_num_test_lights;
	m_c_light_instances = backend::ref( ).register_constant_host( "light_instances", rc_float );
	m_light_instances = NEW_ARRAY( float4x4, m_num_instanced_lights );

	float* instance_data = NEW_ARRAY( float, m_num_instanced_lights );
	for ( u32 i = 0; i < m_num_instanced_lights; ++i )
		instance_data[i] = float( i );
	m_instance_vb_small = resource_manager::ref( ).create_buffer(
		m_num_instanced_lights * sizeof( float ),
		instance_data,
		enum_buffer_type_vertex,
		false,
		false
	);
	DELETE_ARRAY( instance_data );

	m_instance_declaration_small = resource_manager::ref( ).create_declaration(
		instance_data_layout_small,
		array_size( instance_data_layout_small )
	);
}

bool stage_lights::is_effects_ready( ) const
{
	return
		   m_shadow_effect.c_ptr() != NULL
		&& m_effect_accum_mask.c_ptr() != NULL
		&& m_point_light_accumulator.c_ptr() != NULL
		&& m_point_light_shadower.c_ptr() != NULL
		&& m_shadowed_point_light_accumulator.c_ptr() != NULL
		&& m_spot_light_accumulator.c_ptr() != NULL
		&& m_shadowed_spot_light_accumulator.c_ptr() != NULL
		&& m_capsule_light_accumulator.c_ptr() != NULL
		&& m_obb_light_accumulator.c_ptr() != NULL
		&& m_shadowed_obb_light_accumulator.c_ptr() != NULL
		&& m_sphere_light_accumulator.c_ptr() != NULL
		&& m_shadowed_sphere_light_accumulator.c_ptr() != NULL
		&& m_shadowed_plane_spot_light_accumulator.c_ptr() != NULL
		&& m_sh_downsample_skin_irradiance_texture.c_ptr() != NULL
		&& m_sh_fix_irradiance_texture.c_ptr() != NULL
		&& m_plane_spot_light_accumulator.c_ptr() != NULL;
}

void stage_lights::make_spot_light_shadowmap( u32 shadow_quality, light* l )
{
	float const max_angle			 = math::max(l->spot_umbra_angle, l->spot_penumbra_angle);

	math::float4x4 projection_matrix = math::create_perspective_projection(max_angle, 1.0f, l->range / 1000.0f, l->range);

	math::float4x4 view_matrix		 = math::create_camera_at(l->position, l->position + l->direction, math::normalize(math::cross_product(l->direction, l->right)));

	u32 const spot_shadow_map_size	 = index_to_shadow_size(l->shadow_map_size_index);
	render_to_hw_shadowmap			 (l, shadow_quality, l->shadow_z_bias, spot_shadow_map_size, l->shadow_map_size_index, view_matrix, projection_matrix, 0);
}

void stage_lights::make_plane_spot_light_shadowmap( u32 shadow_quality, light* l )
{
	float const max_angle			 = math::max(l->spot_umbra_angle, l->spot_penumbra_angle);
	float const max_scale			 = math::max(l->scale.x, l->scale.z);

	float const inv_distance		 = max_scale / math::tan(max_angle / 2.0f);

	float3 const direction			 = -math::normalize(math::cross_product(l->direction, l->right));
	float3 const up				     = l->direction;
	float3 const new_position		 = l->position - inv_distance * direction;

	math::float4x4 projection_matrix = math::create_perspective_projection(max_angle, 1.0f, inv_distance, inv_distance + l->range);

	math::float4x4 view_matrix		 = math::create_camera_at(new_position, new_position + direction, up);

	u32 const spot_shadow_map_size	 = index_to_shadow_size(l->shadow_map_size_index);
	render_to_hw_shadowmap			 (l, shadow_quality, l->shadow_z_bias, spot_shadow_map_size, l->shadow_map_size_index, view_matrix, projection_matrix, 0);
}

void stage_lights::render_to_hw_shadowmap(
	light*				l,
	u32					shadow_quality,
	float const			z_bias,
	u32					smap_size,
	u32					smap_size_index,
	float4x4 const&		view_matrix,
	float4x4 const&		projection_matrix,
	u32					marge
)
{
	VOSTOK_UNREFERENCED_PARAMETERS(shadow_quality, marge);

	BEGIN_CPUGPU_TIMER(statistics::ref().lights_stat_group.shadow_map_time);
	backend::ref().flush_rt_shader_resources();

	D3D11_VIEWPORT orig_viewport;
	backend::ref().get_viewport( orig_viewport);

	backend::ref().set_render_targets		(0, 0, 0, 0);
	if ( l->static_shadows )
	{
		backend::ref().set_depth_stencil_target( &*l->m_shadow_depth_stencil );
		smap_size = 256;
	}
	else
	{
		backend::ref().set_depth_stencil_target( &*m_shadow_depth_stencil[smap_size_index] );
		backend::ref().clear_depth_stencil( D3D_CLEAR_DEPTH | D3D_CLEAR_STENCIL, 1.0f, 0 );
	}

	D3D11_VIEWPORT tmp_viewport;

	tmp_viewport.TopLeftX	= 0.0f;//float(marge);
	tmp_viewport.TopLeftY	= 0.0f;//float(marge);
	tmp_viewport.Width		= float(smap_size);// - marge * 2);
	tmp_viewport.Height		= float(smap_size);// - marge * 2);
	tmp_viewport.MinDepth	= 0;
	tmp_viewport.MaxDepth	= 1.f;

	backend::ref().set_viewport( tmp_viewport);

	float3 view_position	= m_context->get_v_inverted().c.xyz();

	m_context->push_set_v( view_matrix);
	m_context->push_set_p( projection_matrix);

	vector< render_surface_instance* > m_dynamic_visuals_to_shadow;
	m_context->scene()->select_models(
		m_context->get_culling_vp(),
		m_dynamic_visuals_to_shadow,
		m_context->get_view_pos(),
		visible_flag,
		l->static_shadows
	);

	if ( !l->static_shadows || !m_dynamic_visuals_to_shadow.empty() || l->need_refresh_static_shadows )
	{
		if ( l->static_shadows )
		{
			m_context->scene()->select_models(
				m_context->get_culling_vp(),
				m_dynamic_visuals_to_shadow,
				m_context->get_view_pos(),
				visible_flag,
				false
			);
			backend::ref().set_depth_stencil_target( &*l->m_shadow_depth_stencil );
			backend::ref().clear_depth_stencil( D3D_CLEAR_DEPTH | D3D_CLEAR_STENCIL, 1.0f, 0 );
			if ( l->need_refresh_static_shadows )
				l->need_refresh_static_shadows = false;
		}

		vector< render_surface_instance* >::iterator it_d = m_dynamic_visuals_to_shadow.begin();
		vector< render_surface_instance* >::const_iterator end_d = m_dynamic_visuals_to_shadow.end();

		for ( ; it_d != end_d; ++it_d)
		{
			render_surface_instance& instance = *(*it_d);
			material_effects& me = instance.m_render_surface->get_material_effects();
			render_geometry& geometry = instance.m_render_surface->m_render_geometry;

			if (
				!me.is_cast_shadow ||
				!geometry.geom.c_ptr() ||
				!me.m_effects[gbuffer_render_stage]
			)
				continue;

			if ( options::ref().current.m_use_hiz_occlusion_culling && instance.m_occluded )
				continue;

			res_effect* effect = me.m_effects[shadow_render_stage].c_ptr();
			if ( !effect )
				effect = m_shadow_effect.c_ptr();
			effect->apply( 0, 0 );

			m_context->set_w(*instance.m_transform);
			instance.set_constants( );
			geometry.geom->apply();

			backend::ref().render_indexed( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, geometry.primitive_count*3, 0, 0);
		}

		if (options::ref().current.m_enabled_draw_speedtree)
		{
			speedtree_forest::tree_render_info_array_type visible_trees;
			m_context->scene()->get_speedtree_forest()->get_visible_tree_components(m_context, view_position, true, visible_trees);

			for (speedtree_forest::tree_render_info_array_type::iterator it=visible_trees.begin(); it!=visible_trees.end(); ++it)
			{
				if ( !it->tree_component->get_material_effects().stage_enable[shadow_render_stage] ||
					 !it->tree_component->get_material_effects().m_effects[shadow_render_stage] ||
					 !it->tree_component->get_material_effects().is_cast_shadow)
					continue;

				it->tree_component->get_material_effects().m_effects[shadow_render_stage]->apply( 0, 0 );

				m_context->scene()->get_speedtree_forest()->get_speedtree_wind_parameters().set		(it->tree_component->m_parent->GetWind());
				m_context->scene()->get_speedtree_forest()->get_speedtree_common_parameters().set	(m_context, it->tree_component, view_position);
				if (it->instance)
					m_context->set_w(
						m_context->scene()->get_speedtree_forest()->get_instance_transform(*it->instance)
					);
				else
					m_context->set_w(math::float4x4().identity());

				if (it->tree_component->get_geometry_type()==SpeedTree::GEOMETRY_TYPE_VERTICAL_BILLBOARDS)
				{
					m_context->scene()->get_speedtree_forest()->get_speedtree_billboard_parameters().set(m_context, it->tree_component);
					it->tree_component->m_render_geometry.geom->apply();
					backend::ref().render_indexed( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, it->tree_component->m_render_geometry.index_count, 0, 0);
				}
				else
				{
					m_context->scene()->get_speedtree_forest()->get_speedtree_tree_parameters().set(it->tree_component, it->instance, it->instance_lod);
					it->tree_component->m_render_geometry.geom->apply();
					backend::ref().render_indexed( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, it->lod->num_indices, it->lod->start_index, 0);

					statistics::ref().visibility_stat_group.num_triangles.value += it->lod->num_indices / 3;
					statistics::ref().visibility_stat_group.num_speedtree_instances.value++;
				}
			}
		}
	}

	float4x4 shadow_transform = m_context->get_vp();

	m_context->pop_v();
	m_context->pop_p();

	m_view_to_light_matrix		= shadow_transform;//mul4x3(m_context->get_v_inverted(), shadow_transform);
	m_shadow_z_bias				= 0.1f * z_bias;
	m_shadow_map_size			= float(smap_size);

	backend::ref().set_viewport( orig_viewport);

	END_CPUGPU_TIMER;
}

stage_lights::~stage_lights( )
{
	DELETE_ARRAY( m_light_instances );
}

void stage_lights::render_particle_lighting(
	render_particle_emitter_instance*	instance,
	light*								l,
	u32									num_particles
)
{
	float light_range				= l->range;
	float3 light_color				= l->color;
	float3 const light_position		= m_context->get_v().transform_position( l->position );
	float3 const light_direction	= m_context->get_v().transform_direction( l->direction );

	bool draw = false;

	switch ( l->get_type() ) {
		case light_type_point : {
			instance->get_material_effects().m_effects[lighting_render_stage]->apply( 0, 0 );
			backend::ref().set_ps_constant	( m_c_light_position,		light_position );
			backend::ref().set_ps_constant	( m_c_light_range,			light_range );
			backend::ref().set_ps_constant	( m_c_light_attenuation_power,	l->attenuation_power );

			draw = true;
			break;
		}
		case light_type_spot : {
			instance->get_material_effects().m_effects[lighting_render_stage]->apply( 0, 0 );
			backend::ref().set_ps_constant	( m_c_light_position,		light_position );
			backend::ref().set_ps_constant	( m_c_light_direction,		light_direction );
			backend::ref().set_ps_constant	( m_c_light_range,			light_range );
			backend::ref().set_ps_constant	( m_c_light_attenuation_power,	l->attenuation_power );

			float const penumbra_half_angle_cosine	= math::cos( l->spot_penumbra_angle/2.f );
			backend::ref().set_ps_constant	( m_c_light_spot_penumbra_half_angle_cosine, penumbra_half_angle_cosine );
			backend::ref().set_ps_constant	( m_c_light_spot_inversed_umbra_half_angle_cosine_minus_penumbra_half_angle_cosine,
				1.f/( math::max(math::cos( l->spot_umbra_angle/2.f ) - penumbra_half_angle_cosine, 0.0001f) )
			);
			backend::ref().set_ps_constant	( m_c_light_spot_falloff, l->spot_falloff );
			draw = true;
			break;
		}
		case light_type_obb : {
			instance->get_material_effects().m_effects[lighting_render_stage]->apply( 0, 0 );

			backend::ref().set_ps_constant	( m_c_light_position,		light_position );
			backend::ref().set_ps_constant	( m_c_light_range,			light_range );
			backend::ref().set_ps_constant	( m_c_light_attenuation_power,	l->attenuation_power );

			float4x4 obb_world				= l->m_xform;
			obb_world.set_scale				( l->scale );
			backend::ref().set_ps_constant	( m_c_light_local_to_world,	obb_world * m_context->get_v() );

			backend::ref().set_ps_constant	( m_c_light_color, light_color );
			backend::ref().set_ps_constant	( m_c_light_intensity, l->intensity );
			backend::ref().set_ps_constant	( m_c_lighting_model, l->lighting_model );

			float3 const* const eye_rays	= m_context->get_eye_rays();
			backend::ref().set_ps_constant	( m_c_eye_ray_corner,	((float4*)eye_rays)[0] );
			backend::ref().set_vs_constant	( m_c_near_far, m_context->get_near_far());
			draw = true;
			break;
		}
		case light_type_capsule : {
			instance->get_material_effects().m_effects[lighting_render_stage]->apply( 0, 0 );

			backend::ref().set_ps_constant	( m_c_light_position,		light_position );
			backend::ref().set_ps_constant	( m_c_light_direction,		light_direction );
			backend::ref().set_ps_constant	( m_c_light_range,			light_range );
			backend::ref().set_ps_constant	( m_c_light_attenuation_power,	l->attenuation_power );

			backend::ref().set_ps_constant	( m_c_light_capsule_half_width,	l->scale.z );
			backend::ref().set_ps_constant	( m_c_light_capsule_radius,	l->scale.x );

			backend::ref().set_ps_constant	( m_c_light_color, light_color );
			backend::ref().set_ps_constant	( m_c_light_intensity, l->intensity );
			backend::ref().set_ps_constant	( m_c_lighting_model, l->lighting_model );

			float3 const* const eye_rays	= m_context->get_eye_rays();
			backend::ref().set_ps_constant	( m_c_eye_ray_corner,	((float4*)eye_rays)[0] );
			backend::ref().set_vs_constant	( m_c_near_far, m_context->get_near_far());
			draw = true;
			break;
		}
		case light_type_parallel : {
			if (instance->get_material_effects().m_effects[lighting_render_stage])
			{
				instance->get_material_effects().m_effects[lighting_render_stage]->apply( 0, 0 );
				for ( u32 i = 0; i < 4; ++i )
					backend::ref().set_ps_constant( m_shadow[i], math::transpose( m_context->get_view2shadow( i ) ) );
				backend::ref().set_ps_constant( m_c_shadow_transparency, l->shadow_transparency );
				backend::ref().set_ps_constant	( m_c_light_direction, light_direction );
				draw = true;
			}
			break;
		}
		case light_type_sphere : {
			instance->get_material_effects().m_effects[lighting_render_stage]->apply( 0, 0 );
			backend::ref().set_ps_constant	( m_c_light_position,		light_position );
			backend::ref().set_ps_constant	( m_c_light_range,			light_range );
			backend::ref().set_ps_constant	( m_c_light_attenuation_power,	l->attenuation_power );

			backend::ref().set_ps_constant	( m_c_light_sphere_radius,	l->scale.x );

			backend::ref().set_ps_constant	( m_c_light_color, light_color );
			backend::ref().set_ps_constant	( m_c_light_intensity, l->intensity );
			backend::ref().set_ps_constant	( m_c_lighting_model, l->lighting_model );

			float3 const* const eye_rays	= m_context->get_eye_rays();
			backend::ref().set_ps_constant	( m_c_eye_ray_corner,	((float4*)eye_rays)[0] );
			backend::ref().set_vs_constant	( m_c_near_far, m_context->get_near_far());
			draw = true;
			break;
		}
		case light_type_plane_spot : {
			instance->get_material_effects().m_effects[lighting_render_stage]->apply( 0, 0 );
			backend::ref().set_ps_constant	( m_c_light_position,		light_position );
			backend::ref().set_ps_constant	( m_c_light_direction,		light_direction );
			backend::ref().set_ps_constant	( m_c_light_range,			light_range/math::sin(l->spot_penumbra_angle/2.f) );
			backend::ref().set_ps_constant	( m_c_light_attenuation_power,	l->attenuation_power );

			float const penumbra_half_angle_cosine	= math::cos( l->spot_penumbra_angle/2.f );
			backend::ref().set_ps_constant	( m_c_light_spot_penumbra_half_angle_cosine, penumbra_half_angle_cosine );
			float const umbra_half_angle_cosine	= math::cos( l->spot_umbra_angle/2.f );
			backend::ref().set_ps_constant	( m_c_light_spot_umbra_half_angle_cosine, umbra_half_angle_cosine );
			backend::ref().set_ps_constant	( m_c_light_spot_inversed_umbra_half_angle_cosine_minus_penumbra_half_angle_cosine,
				1.f/( math::max(umbra_half_angle_cosine - penumbra_half_angle_cosine, 0.0001f) )
			);
			backend::ref().set_ps_constant	( m_c_light_spot_falloff, l->spot_falloff );

			// plane spot light specific
			backend::ref().set_ps_constant	( m_c_light_local_to_world,	l->m_plane_spot_xform * m_context->get_v() );

			backend::ref().set_ps_constant	( m_c_light_color, light_color );
			backend::ref().set_ps_constant	( m_c_light_intensity, l->intensity );
			backend::ref().set_ps_constant	( m_c_lighting_model, l->lighting_model );

			float3 const* const eye_rays	= m_context->get_eye_rays();
			backend::ref().set_ps_constant	( m_c_eye_ray_corner,	((float4*)eye_rays)[0] );
			backend::ref().set_vs_constant	( m_c_near_far, m_context->get_near_far());
			draw = true;
			break;
		}
		default : NODEFAULT( );
	}

	if (draw)
	{
		backend::ref().set_ps_constant	( m_c_light_color, light_color );
		backend::ref().set_ps_constant	( m_c_light_intensity, l->intensity );
		backend::ref().set_ps_constant	( m_c_lighting_model, l->lighting_model );
		backend::ref().set_ps_constant	( m_c_light_type, s32(l->get_type()));
		backend::ref().set_ps_constant	( m_c_diffuse_influence_factor, l->diffuse_influence_factor );
		backend::ref().set_ps_constant	( m_c_specular_influence_factor, l->specular_influence_factor );

		particle_shader_constants::ref().set(
			m_context->get_v_inverted().transform_direction(float3(0,1000,0)).normalize(),
			m_context->get_v_inverted().transform_direction(float3(1000,0,0)).normalize(),
			m_context->get_v_inverted().lines[3].xyz(),
			instance->locked_axis(),
			instance->screen_alignment()
			);
		particle_shader_constants::ref().set_time(m_context->m_current_time);

		m_context->set_w( instance->transform() );

		instance->render(m_context->get_v_inverted().lines[3].xyz(), num_particles);
	}
}

} // namespace render
} // namespace vostok

static float compute_gaussian_value( float x, float /*mean*/, float std_deviation )
{
	return ( 1.0f / vostok::math::sqrt( 2.0f * vostok::math::pi * std_deviation * std_deviation ) )
		* vostok::math::exp( (-x*x)/(2.0f * std_deviation * std_deviation) );
}

static void get_gaussain_weights_offsets(
	float*	out_weights,
	float*	out_offsets,
	u32		buffer_size,
	float	blur_scale,
	float	blur_intencity,
	u32		num_samples
)
{
	for (u32 i=0; i<num_samples; i++)
	{
		out_offsets[i] = (static_cast<float>(i)-4.0f)*(1.0f/static_cast<float>(buffer_size));
		float x = (static_cast<float>(i) - 4.0f) / 4.0f;
		out_weights[i] = blur_intencity * compute_gaussian_value( x, 0.0f, vostok::math::max(blur_scale,0.25f));
	}
}

namespace vostok {
namespace render {

void stage_lights::fill_surface( render_target_ptr surf )
{
	backend::ref().set_render_targets( &*surf, 0, 0, 0);
	backend::ref().set_depth_stencil_target(0);

	u32		offset;

	screen_vertex* pv = (screen_vertex*)backend::ref().vertex.lock(4, sizeof(screen_vertex), offset);
	pv->set( float4(-1.0f, -1.0f, 0.0f, 1.0f), float2(0.0f, 1.0f)); pv++;
	pv->set( float4(-1.0f,  1.0f, 0.0f, 1.0f), float2(0.0f, 0.0f)); pv++;
	pv->set( float4( 1.0f, -1.0f, 0.0f, 1.0f), float2(1.0f, 1.0f)); pv++;
	pv->set( float4( 1.0f,  1.0f, 0.0f, 1.0f), float2(1.0f, 0.0f)); pv++;
	backend::ref().vertex.unlock();

	m_screen_vertex_geometry->apply();

	backend::ref().render_indexed( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 2*3, 0, offset);
}

void stage_lights::make_skin_scattering_texture(
	render_surface_instance*	instance,
	light*						l
)
{
	math::float4 clear_color				= float4(-1.0f, 0.0f, 0.0f, 0.0f);//float4(m_context->get_scene_view()->post_process_parameters().environment_ambient_color, 0.0f) + instance->m_render_surface->get_material_effects().organic_clear_color;
	backend::ref().set_render_targets		(&*m_rt_skin_scattering_temp, &*m_rt_skin_scattering_stretch, 0, 0);
	backend::ref().set_depth_stencil_target (0);
	backend::ref().clear_render_targets		(clear_color.x, clear_color.y, clear_color.z, 0.0f);

	D3D11_VIEWPORT							orig_viewport;
	D3D11_VIEWPORT							tmp_viewport;

	backend::ref().get_viewport				(orig_viewport);

	tmp_viewport.TopLeftX					= 0.0f;
	tmp_viewport.TopLeftY					= 0.0f;
	tmp_viewport.Width						= float(options::ref().current.m_organic_irradiance_texture_size);
	tmp_viewport.Height						= float(options::ref().current.m_organic_irradiance_texture_size);
	tmp_viewport.MinDepth					= 0;
	tmp_viewport.MaxDepth					= 1.f;

	backend::ref().set_viewport				(tmp_viewport);

	float light_range						= l->range;
	float3 light_color						= l->color;
	float3 const light_position				= m_context->get_v().transform_position( l->position );
	float3 const light_direction			= m_context->get_v().transform_direction( l->direction );

	render_geometry& geometry				= instance->m_render_surface->m_render_geometry;

	bool draw								= false;

	switch ( l->get_type() ) {
		case light_type_point : {
			instance->m_render_surface->get_material_effects().m_effects[lighting_render_stage]->apply(organic_lighting_lighting_pass, 0);
			backend::ref().set_ps_constant	(m_c_light_position, light_position);
			backend::ref().set_ps_constant	(m_c_light_range, light_range );
			backend::ref().set_ps_constant	(m_c_light_attenuation_power, l->attenuation_power);
			backend::ref().set_ps_constant	(m_c_is_shadower, 0.0f);

			if (l->is_cast_shadows() && options::ref().current.m_enabled_local_light_shadows)
			{
				backend::ref().set_ps_constant(m_c_shadow_transparency, l->shadow_transparency);
				backend::ref().set_ps_constant(m_c_view_to_light_matrix, transpose(m_view_to_light_matrix));
				backend::ref().set_ps_constant(m_c_shadow_z_bias, m_shadow_z_bias);
				backend::ref().set_ps_constant(m_c_shadow_map_size, m_shadow_map_size);
				backend::ref().set_ps_constant(m_c_use_shadows, u32(1));
				backend::ref().set_ps_texture ("shadowmap_texture", &*m_shadow_depth_stencil_texture[l->shadow_map_size_index]);
			}
			else
			{
				backend::ref().set_ps_constant(m_c_use_shadows, u32(0));
			}
			draw = true;
			break;
		}
		case light_type_spot : {
			instance->m_render_surface->get_material_effects().m_effects[lighting_render_stage]->apply(organic_lighting_lighting_pass, 0);

			backend::ref().set_ps_constant	(m_c_is_shadower, 0.0f);

			backend::ref().set_ps_constant	(m_c_light_position, light_position);
			backend::ref().set_ps_constant	(m_c_light_direction, light_direction);
			backend::ref().set_ps_constant	(m_c_light_range, light_range/math::sin(l->spot_penumbra_angle/2.f));
			backend::ref().set_ps_constant	(m_c_light_attenuation_power, l->attenuation_power);

			backend::ref().set_ps_constant	(m_c_diffuse_influence_factor, l->diffuse_influence_factor);
			backend::ref().set_ps_constant	(m_c_specular_influence_factor, l->specular_influence_factor);

			float const penumbra_half_angle_cosine	= math::cos( l->spot_penumbra_angle/2.f);
			backend::ref().set_ps_constant	(m_c_light_spot_penumbra_half_angle_cosine, penumbra_half_angle_cosine);
			float const umbra_half_angle_cosine	= math::cos( l->spot_umbra_angle/2.f);
			backend::ref().set_ps_constant	(m_c_light_spot_umbra_half_angle_cosine, umbra_half_angle_cosine);
			backend::ref().set_ps_constant	(m_c_light_spot_inversed_umbra_half_angle_cosine_minus_penumbra_half_angle_cosine,
				1.f/( math::max(umbra_half_angle_cosine - penumbra_half_angle_cosine, 0.0001f))
				);
			backend::ref().set_ps_constant	(m_c_light_spot_falloff, l->spot_falloff);

			backend::ref().set_ps_constant	(m_c_light_color, light_color);
			backend::ref().set_ps_constant	(m_c_light_intensity, l->intensity);
			backend::ref().set_ps_constant	(m_c_lighting_model, l->lighting_model);

			float3 const* const eye_rays	= m_context->get_eye_rays();
			backend::ref().set_ps_constant	(m_c_eye_ray_corner,	((float4*)eye_rays)[0]);
			//backend::ref().set_vs_constant(m_c_near_far, m_context->get_near_far());

			if (l->is_cast_shadows() && options::ref().current.m_enabled_local_light_shadows)
			{
				backend::ref().set_ps_constant(m_c_shadow_transparency, l->shadow_transparency);
				backend::ref().set_ps_constant(m_c_view_to_light_matrix, transpose(m_view_to_light_matrix));
				backend::ref().set_ps_constant(m_c_shadow_z_bias, m_shadow_z_bias);
				backend::ref().set_ps_constant(m_c_shadow_map_size, m_shadow_map_size);
				backend::ref().set_ps_constant(m_c_use_shadows, u32(1));
				backend::ref().set_ps_texture ("shadowmap_texture", &*m_shadow_depth_stencil_texture[l->shadow_map_size_index]);
			}
			else
			{
				backend::ref().set_ps_constant(m_c_use_shadows, u32(0));
			}
			draw = true;
			break;
		}
		case light_type_obb : {
			NOT_IMPLEMENTED();
			break;
		}
		case light_type_capsule : {
			NOT_IMPLEMENTED();
			break;
		}
		case light_type_parallel : {
			instance->m_render_surface->get_material_effects().m_effects[lighting_render_stage]->apply(organic_lighting_lighting_pass, 0);
			backend::ref().set_ps_constant	( m_c_light_direction, light_direction );
			backend::ref().set_ps_constant(m_c_use_shadows, u32(0));
			backend::ref().set_ps_constant	(m_c_is_shadower, 0.0f);
			draw = true;
			break;
		}
		case light_type_sphere : {
			NOT_IMPLEMENTED();
			break;
		}
		case light_type_plane_spot : {
			NOT_IMPLEMENTED();
			break;
		}
		default : NODEFAULT( );
	}

	if (draw)
	{
		backend::ref().set_ps_constant		(m_far_fog_color_and_distance, float4(m_context->get_scene_view()->post_process_parameters().environment_far_fog_color, m_context->get_scene_view()->post_process_parameters().environment_far_fog_distance));
		backend::ref().set_ps_constant		(m_near_fog_distance, m_context->get_scene_view()->post_process_parameters().environment_near_fog_distance);
		backend::ref().set_ps_constant		(m_c_light_color, light_color);
		backend::ref().set_ps_constant		(m_c_light_intensity, l->intensity);
		backend::ref().set_ps_constant		(m_c_lighting_model, l->lighting_model);
		backend::ref().set_ps_constant		(m_c_light_type, s32(l->get_type()));
		backend::ref().set_ps_constant		(m_c_diffuse_influence_factor, l->diffuse_influence_factor);
		backend::ref().set_ps_constant		(m_c_specular_influence_factor, l->specular_influence_factor);

		backend::ref().set_ps_constant(
			m_ambient_color,
			float4(
				m_context->get_scene_view()->post_process_parameters().environment_ambient_color,
				0
			)
		);

		geometry.geom->apply();
		m_context->set_w					(*instance->m_transform);
		backend::ref().render_indexed		(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, instance->m_render_surface->m_render_geometry.primitive_count*3, 0, 0);

		m_sh_fix_irradiance_texture->apply	( 0, 0 );
		fill_surface						(m_rt_skin_scattering);

		float weights_h[9];
		float offsets_h[9];
		float weights_v[9];
		float offsets_v[9];

		float const blur_kernel				= 0.5f;
		float const blur_intencity			= 0.5f;

		float t_w							= float(options::ref().current.m_organic_irradiance_texture_size);
		float t_h							= float(options::ref().current.m_organic_irradiance_texture_size);

		get_gaussain_weights_offsets		(weights_h, offsets_h, u32(t_w), blur_kernel, blur_intencity, 9);
		get_gaussain_weights_offsets		(weights_v, offsets_v, u32(t_h), blur_kernel, blur_intencity, 9);

		math::float4						kernel_offsets[8];
		math::float4						blur_offsets_weights[9];

		for (u32 i = 0; i < 9; i++)
			blur_offsets_weights[i]			= float4(offsets_h[i], weights_h[i], offsets_v[i], weights_v[i]);

		m_sh_downsample_skin_irradiance_texture->apply( 0, 0 );
		backend::ref().set_ps_texture		("t_base", &*m_t_skin_scattering);
		backend::ref().set_ps_constant		(m_ambient_color, float4(m_context->get_scene_view()->post_process_parameters().environment_ambient_color, 0));
		fill_surface						(m_rt_skin_scattering_small);

		instance->m_render_surface->get_material_effects().m_effects[lighting_render_stage]->apply(organic_lighting_blurring_pass, 0);
		backend::ref().set_ps_texture		("t_base", &*m_t_skin_scattering_small);
		backend::ref().set_ps_constant		(m_ambient_color, float4(m_context->get_scene_view()->post_process_parameters().environment_ambient_color, 0));
		backend::ref().set_ps_constant		(m_blur_offsets_weights, blur_offsets_weights);
		fill_surface						(m_rt_skin_scattering_blurred_0);

		backend::ref().flush_rt_shader_resources();
		instance->m_render_surface->get_material_effects().m_effects[lighting_render_stage]->apply(organic_lighting_blurring_pass, 0);
		backend::ref().set_ps_texture		("t_base", &*m_t_skin_scattering_blurred_0);
		backend::ref().set_ps_constant		(m_ambient_color, float4(m_context->get_scene_view()->post_process_parameters().environment_ambient_color, 0));
		backend::ref().set_ps_constant		(m_blur_offsets_weights, blur_offsets_weights);
		fill_surface						(m_rt_skin_scattering_blurred_1);

		backend::ref().flush_rt_shader_resources();
		instance->m_render_surface->get_material_effects().m_effects[lighting_render_stage]->apply(organic_lighting_blurring_pass, 0);
		backend::ref().set_ps_texture		("t_base", &*m_t_skin_scattering_blurred_1);
		backend::ref().set_ps_constant		(m_ambient_color, float4(m_context->get_scene_view()->post_process_parameters().environment_ambient_color, 0));
		backend::ref().set_ps_constant		(m_blur_offsets_weights, blur_offsets_weights);
		fill_surface						(m_rt_skin_scattering_blurred_2);

		backend::ref().flush_rt_shader_resources();
		instance->m_render_surface->get_material_effects().m_effects[lighting_render_stage]->apply(organic_lighting_blurring_pass, 0);
		backend::ref().set_ps_texture		("t_base", &*m_t_skin_scattering_blurred_2);
		backend::ref().set_ps_constant		(m_ambient_color, float4(m_context->get_scene_view()->post_process_parameters().environment_ambient_color, 0));
		backend::ref().set_ps_constant		(m_blur_offsets_weights, blur_offsets_weights);
		fill_surface						(m_rt_skin_scattering_blurred_3);

		backend::ref().flush_rt_shader_resources();
		instance->m_render_surface->get_material_effects().m_effects[lighting_render_stage]->apply(organic_lighting_blurring_pass, 0);
		backend::ref().set_ps_texture		("t_base", &*m_t_skin_scattering_blurred_3);
		backend::ref().set_ps_constant		(m_blur_offsets_weights, blur_offsets_weights);
		fill_surface						(m_rt_skin_scattering_blurred_4);

		// Set old. TODO: get_render_targets(), set_render_targets()
		backend::ref().set_render_targets( &*m_context->get_rt(rt_generic_0), 0, 0, 0);
		backend::ref().reset_depth_stencil_target();
		backend::ref().set_viewport( orig_viewport);

		instance->m_render_surface->get_material_effects().m_effects[lighting_render_stage]->apply(organic_lighting_combine_pass, 0);
		backend::ref().set_ps_constant	( m_c_light_position,		light_position );
		backend::ref().set_ps_constant	( m_c_light_range,			light_range );
		backend::ref().set_ps_constant	( m_c_light_color,			light_color );

		backend::ref().set_ps_constant(
			m_ambient_color,
			float4(
				m_context->get_scene_view()->post_process_parameters().environment_ambient_color,
				0
			)
		);

		geometry.geom->apply();
		m_context->set_w(*instance->m_transform);
		backend::ref().render_indexed( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, instance->m_render_surface->m_render_geometry.primitive_count*3, 0, 0);
	}

	// Set old. TODO: get_render_targets(), set_render_targets()
	backend::ref().set_render_targets( &*m_context->get_rt(rt_generic_0), 0, 0, 0);
	backend::ref().reset_depth_stencil_target();
	backend::ref().set_viewport( orig_viewport);
}

void stage_lights::render_particle_probe_lighting(
	render_particle_emitter_instance*	instance,
	environment_probe*			probe,
	u32					num_particles
)
{
	res_effect_ptr& effect = instance->get_material_effects( ).m_effects[lighting_render_stage];
	if ( !effect )
		return;

	effect->apply( 1, 0 );
	backend::ref( ).set_ps_texture( "t_probe_cubemap", &*probe->m_texture );

	post_process_parameters const& parameters =
		m_context->get_scene_view( )->post_process_parameters( );
	backend::ref( ).set_ps_constant(
		m_probe_parameters0,
		float4( probe->m_properties.location, probe->m_properties.radius )
	);

	backend::ref( ).set_ps_constant(
		m_probe_parameters1,
		float4(
			parameters.environment_probes_diffuse_instensity_multiplier * probe->m_properties.diffuse_multiplier,
			parameters.environment_probes_specular_instensity_multiplier * probe->m_properties.specular_multiplier,
			float( probe->m_num_mips ),
			0.f
		)
	);

	particle_shader_constants::ref( ).set(
		m_context->get_v_inverted( ).transform_direction( float3( 0, 1000, 0 ) ).normalize( ),
		m_context->get_v_inverted( ).transform_direction( float3( 1000, 0, 0 ) ).normalize( ),
		m_context->get_v_inverted( ).lines[3].xyz( ),
		instance->locked_axis( ),
		instance->screen_alignment( )
	);
	particle_shader_constants::ref( ).set_time( m_context->m_current_time );

	m_context->set_w( instance->transform( ) );
	instance->render( m_context->get_v_inverted( ).lines[3].xyz( ), num_particles );
}

void stage_lights::render_model_probe_lighting(
	render_surface_instance*	instance,
	environment_probe*		probe,
	float				min_probe_scale
)
{
	material_effects& effects = instance->m_render_surface->get_material_effects( );
	effects.m_effects[lighting_render_stage]->apply( 1, 0 );
	backend::ref( ).set_ps_texture( "t_probe_cubemap", &*probe->m_texture );

	post_process_parameters const& parameters =
		m_context->get_scene_view( )->post_process_parameters( );
	backend::ref( ).set_ps_constant(
		m_probe_parameters0,
		float4(
			probe->m_properties.location,
			math::max( probe->m_properties.radius, min_probe_scale )
		)
	);

	backend::ref( ).set_ps_constant(
		m_probe_parameters1,
		float4(
			parameters.environment_probes_diffuse_instensity_multiplier * probe->m_properties.diffuse_multiplier,
			parameters.environment_probes_specular_instensity_multiplier * probe->m_properties.specular_multiplier,
			float( probe->m_num_mips ),
			0.f
		)
	);

	backend::ref( ).render_indexed(
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		instance->m_render_surface->m_render_geometry.primitive_count * 3,
		0,
		0
	);
}

void stage_lights::render_model_lighting(
	render_surface_instance*	instance,
	light*						l
)
{
	float light_range				= l->range;
	float3 light_color				= l->color;
	float3 const light_position		= m_context->get_v().transform_position( l->position );
	float3 const light_direction	= m_context->get_v().transform_direction( l->direction );

	material_effects& me			= instance->m_render_surface->get_material_effects();

	bool draw = false;
	bool const is_organic = me.is_organic;

	if (is_organic && l->get_type() == light_type_spot)
	{
		if (l->is_cast_shadows() && options::ref().current.m_enabled_local_light_shadows)
			make_spot_light_shadowmap(0, l);

		make_skin_scattering_texture	(instance, l);

		return;
	}
	if (is_organic && l->get_type() == light_type_point)
	{
		make_skin_scattering_texture	(instance, l);
		return;
	}
	if (is_organic && l->get_type() == light_type_parallel)
	{
		make_skin_scattering_texture	(instance, l);
		return;
	}

	switch ( l->get_type() ) {
		case light_type_point : {
			me.m_effects[lighting_render_stage]->apply( 0, 0 );
			backend::ref().set_ps_constant( m_c_light_position, light_position );
			backend::ref().set_ps_constant( m_c_light_range, light_range );
			backend::ref().set_ps_constant( m_c_light_attenuation_power, l->attenuation_power );
			draw = true;
			break;
		}
		case light_type_spot : {
			me.m_effects[lighting_render_stage]->apply( 0, 0 );
			backend::ref().set_ps_constant( m_c_light_position, light_position );
			backend::ref().set_ps_constant( m_c_light_direction, light_direction );
			backend::ref().set_ps_constant( m_c_light_range, light_range );
			backend::ref().set_ps_constant( m_c_light_attenuation_power, l->attenuation_power );

			float const penumbra_half_angle_cosine = math::cos( l->spot_penumbra_angle / 2.f );
			backend::ref().set_ps_constant( m_c_light_spot_penumbra_half_angle_cosine, penumbra_half_angle_cosine );
			backend::ref().set_ps_constant(
				m_c_light_spot_inversed_umbra_half_angle_cosine_minus_penumbra_half_angle_cosine,
				1.f / math::max( math::cos( l->spot_umbra_angle / 2.f ) - penumbra_half_angle_cosine, 0.0001f )
			);
			backend::ref().set_ps_constant( m_c_light_spot_falloff, l->spot_falloff );
			draw = true;
			break;
		}
		case light_type_obb : {
			me.m_effects[lighting_render_stage]->apply( 0, 0 );

			backend::ref().set_ps_constant( m_c_light_position, light_position );
			backend::ref().set_ps_constant( m_c_light_range, light_range );
			backend::ref().set_ps_constant( m_c_light_attenuation_power, l->attenuation_power );

			float4x4 obb_world = l->m_xform;
			obb_world.set_scale( l->scale );
			backend::ref().set_ps_constant( m_c_light_local_to_world, obb_world * m_context->get_v() );

			backend::ref().set_ps_constant( m_c_light_color, light_color );
			backend::ref().set_ps_constant( m_c_light_intensity, l->intensity );
			backend::ref().set_ps_constant( m_c_lighting_model, l->lighting_model );

			float3 const* const eye_rays = m_context->get_eye_rays();
			backend::ref().set_ps_constant( m_c_eye_ray_corner, ((float4*)eye_rays)[0] );
			backend::ref().set_vs_constant( m_c_near_far, m_context->get_near_far() );
			draw = true;
			break;
		}
		case light_type_capsule : {
			me.m_effects[lighting_render_stage]->apply( 0, 0 );

			backend::ref().set_ps_constant( m_c_light_position, light_position );
			backend::ref().set_ps_constant( m_c_light_direction, light_direction );
			backend::ref().set_ps_constant( m_c_light_range, light_range );
			backend::ref().set_ps_constant( m_c_light_attenuation_power, l->attenuation_power );

			backend::ref().set_ps_constant( m_c_light_capsule_half_width, l->scale.z );
			backend::ref().set_ps_constant( m_c_light_capsule_radius, l->scale.x );

			backend::ref().set_ps_constant( m_c_light_color, light_color );
			backend::ref().set_ps_constant( m_c_light_intensity, l->intensity );
			backend::ref().set_ps_constant( m_c_lighting_model, l->lighting_model );

			float3 const* const eye_rays = m_context->get_eye_rays();
			backend::ref().set_ps_constant( m_c_eye_ray_corner, ((float4*)eye_rays)[0] );
			backend::ref().set_vs_constant( m_c_near_far, m_context->get_near_far() );
			draw = true;
			break;
		}
		case light_type_parallel : {
			if ( me.m_effects[lighting_render_stage] )
			{
				me.m_effects[lighting_render_stage]->apply( 0, 0 );
				for ( u32 i = 0; i < 4; ++i )
					backend::ref().set_ps_constant( m_shadow[i], math::transpose( m_context->get_view2shadow( i ) ) );
				backend::ref().set_ps_constant( m_c_shadow_transparency, l->shadow_transparency );
				backend::ref().set_ps_constant( m_c_light_direction, light_direction );
				draw = true;
			}
			break;
		}
		case light_type_sphere : {
			me.m_effects[lighting_render_stage]->apply( 0, 0 );
			backend::ref().set_ps_constant( m_c_light_position, light_position );
			backend::ref().set_ps_constant( m_c_light_range, light_range );
			backend::ref().set_ps_constant( m_c_light_attenuation_power, l->attenuation_power );
			backend::ref().set_ps_constant( m_c_light_sphere_radius, l->scale.x );
			draw = true;
			break;
		}
		case light_type_plane_spot : {
			me.m_effects[lighting_render_stage]->apply( 0, 0 );
			backend::ref().set_ps_constant( m_c_light_position, light_position );
			backend::ref().set_ps_constant( m_c_light_direction, light_direction );
			backend::ref().set_ps_constant( m_c_light_range, light_range / math::sin( l->spot_penumbra_angle / 2.f ) );
			backend::ref().set_ps_constant( m_c_light_attenuation_power, l->attenuation_power );

			float const penumbra_half_angle_cosine = math::cos( l->spot_penumbra_angle / 2.f );
			backend::ref().set_ps_constant( m_c_light_spot_penumbra_half_angle_cosine, penumbra_half_angle_cosine );
			float const umbra_half_angle_cosine = math::cos( l->spot_umbra_angle / 2.f );
			backend::ref().set_ps_constant( m_c_light_spot_umbra_half_angle_cosine, umbra_half_angle_cosine );
			backend::ref().set_ps_constant(
				m_c_light_spot_inversed_umbra_half_angle_cosine_minus_penumbra_half_angle_cosine,
				1.f / math::max( umbra_half_angle_cosine - penumbra_half_angle_cosine, 0.0001f )
			);
			backend::ref().set_ps_constant( m_c_light_spot_falloff, l->spot_falloff );

			backend::ref().set_ps_constant( m_c_light_local_to_world, l->m_plane_spot_xform * m_context->get_v() );

			backend::ref().set_ps_constant( m_c_light_color, light_color );
			backend::ref().set_ps_constant( m_c_light_intensity, l->intensity );
			backend::ref().set_ps_constant( m_c_lighting_model, l->lighting_model );

			float3 const* const eye_rays = m_context->get_eye_rays();
			backend::ref().set_ps_constant( m_c_eye_ray_corner, ((float4*)eye_rays)[0] );
			backend::ref().set_vs_constant( m_c_near_far, m_context->get_near_far() );
			draw = true;
			break;
		}
		default : NODEFAULT( );
	}

	if ( draw )
	{
		backend::ref().set_ps_constant(
			m_far_fog_color_and_distance,
			float4(
				m_context->get_scene_view()->post_process_parameters().environment_far_fog_color,
				m_context->get_scene_view()->post_process_parameters().environment_far_fog_distance
			)
		);
		backend::ref().set_ps_constant(
			m_near_fog_distance,
			m_context->get_scene_view()->post_process_parameters().environment_near_fog_distance
		);
		backend::ref().set_ps_constant( m_c_light_color, light_color );
		backend::ref().set_ps_constant( m_c_light_intensity, l->intensity );
		backend::ref().set_ps_constant( m_c_lighting_model, l->lighting_model );
		backend::ref().set_ps_constant( m_c_light_type, s32( l->get_type() ) );
		backend::ref().set_ps_constant( m_c_diffuse_influence_factor, l->diffuse_influence_factor );
		backend::ref().set_ps_constant( m_c_specular_influence_factor, l->specular_influence_factor );
		backend::ref().render_indexed(
			D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			instance->m_render_surface->m_render_geometry.primitive_count * 3,
			0,
			0
		);
	}
}

void stage_lights::render_speedtree_lighting(
	lod_entry const*				lod,
	SpeedTree::CInstance const*		instance,
	SpeedTree::SInstanceLod const*	instance_lod,
	speedtree_tree_component*		tree_component,
	light*							l
)
{
	float light_range				= l->range;
	float3 light_color				= l->color;
	float3 const light_position		= m_context->get_v().transform_position( l->position );
	float3 const light_direction	= m_context->get_v().transform_direction( l->direction );

	bool draw = false;

	switch ( l->get_type() ) {
		case light_type_point : {
			tree_component->get_material_effects().m_effects[lighting_render_stage]->apply( 0, 0 );
			backend::ref().set_ps_constant	( m_c_light_position,		light_position );
			backend::ref().set_ps_constant	( m_c_light_range,			light_range );
			backend::ref().set_ps_constant	( m_c_light_attenuation_power,	l->attenuation_power );
			draw = true;
			break;
		}
		case light_type_spot : {
			tree_component->get_material_effects().m_effects[lighting_render_stage]->apply( 0, 0 );
			backend::ref().set_ps_constant	( m_c_light_position,		light_position );
			backend::ref().set_ps_constant	( m_c_light_direction,		light_direction );
			backend::ref().set_ps_constant	( m_c_light_range,			light_range );
			backend::ref().set_ps_constant	( m_c_light_attenuation_power,	l->attenuation_power );

			float const penumbra_half_angle_cosine	= math::cos( l->spot_penumbra_angle/2.f );
			backend::ref().set_ps_constant	( m_c_light_spot_penumbra_half_angle_cosine, penumbra_half_angle_cosine );
			backend::ref().set_ps_constant	( m_c_light_spot_inversed_umbra_half_angle_cosine_minus_penumbra_half_angle_cosine,
				1.f/( math::max(math::cos( l->spot_umbra_angle/2.f ) - penumbra_half_angle_cosine, 0.0001f) )
			);
			backend::ref().set_ps_constant	( m_c_light_spot_falloff, l->spot_falloff );
			draw = true;
			break;
		}
		case light_type_obb : {
			tree_component->get_material_effects().m_effects[lighting_render_stage]->apply( 0, 0 );

			backend::ref().set_ps_constant	( m_c_light_position,		light_position );
			backend::ref().set_ps_constant	( m_c_light_range,			light_range );
			backend::ref().set_ps_constant	( m_c_light_attenuation_power,	l->attenuation_power );

			float4x4 obb_world				= l->m_xform;
			obb_world.set_scale				( l->scale );
			backend::ref().set_ps_constant	( m_c_light_local_to_world,	obb_world * m_context->get_v() );

			backend::ref().set_ps_constant	( m_c_light_color, light_color );
			backend::ref().set_ps_constant	( m_c_light_intensity, l->intensity );
			backend::ref().set_ps_constant	( m_c_lighting_model, l->lighting_model );

			float3 const* const eye_rays	= m_context->get_eye_rays();
			backend::ref().set_ps_constant	( m_c_eye_ray_corner,	((float4*)eye_rays)[0] );
			backend::ref().set_vs_constant	( m_c_near_far, m_context->get_near_far());
			draw = true;
			break;
		}
		case light_type_capsule : {
			tree_component->get_material_effects().m_effects[lighting_render_stage]->apply( 0, 0 );

			backend::ref().set_ps_constant	( m_c_light_position,		light_position );
			backend::ref().set_ps_constant	( m_c_light_direction,		light_direction );
			backend::ref().set_ps_constant	( m_c_light_range,			light_range );
			backend::ref().set_ps_constant	( m_c_light_attenuation_power,	l->attenuation_power );

			backend::ref().set_ps_constant	( m_c_light_capsule_half_width,	l->scale.z );
			backend::ref().set_ps_constant	( m_c_light_capsule_radius,	l->scale.x );

			backend::ref().set_ps_constant	( m_c_light_color, light_color );
			backend::ref().set_ps_constant	( m_c_light_intensity, l->intensity );
			backend::ref().set_ps_constant	( m_c_lighting_model, l->lighting_model );

			float3 const* const eye_rays	= m_context->get_eye_rays();
			backend::ref().set_ps_constant	( m_c_eye_ray_corner,	((float4*)eye_rays)[0] );
			backend::ref().set_vs_constant	( m_c_near_far, m_context->get_near_far());
			draw = true;
			break;
		}
		case light_type_parallel : {
			if (tree_component->get_material_effects().m_effects[lighting_render_stage])
			{
				tree_component->get_material_effects().m_effects[lighting_render_stage]->apply( 0, 0 );
				backend::ref().set_ps_constant	( m_c_light_direction, light_direction );
				draw = true;
			}
			break;
		}
		case light_type_sphere : {
			tree_component->get_material_effects().m_effects[lighting_render_stage]->apply( 0, 0 );
			backend::ref().set_ps_constant	( m_c_light_position,		light_position );
			backend::ref().set_ps_constant	( m_c_light_range,			light_range );
			backend::ref().set_ps_constant	( m_c_light_attenuation_power,	l->attenuation_power );

			backend::ref().set_ps_constant	( m_c_light_sphere_radius,	l->scale.x );

			backend::ref().set_ps_constant	( m_c_light_color, light_color );
			draw = true;
			break;
		}
		case light_type_plane_spot : {
			tree_component->get_material_effects().m_effects[lighting_render_stage]->apply( 0, 0 );
			backend::ref().set_ps_constant	( m_c_light_position,		light_position );
			backend::ref().set_ps_constant	( m_c_light_direction,		light_direction );
			backend::ref().set_ps_constant	( m_c_light_range,			light_range/math::sin(l->spot_penumbra_angle/2.f) );
			backend::ref().set_ps_constant	( m_c_light_attenuation_power,	l->attenuation_power );

			float const penumbra_half_angle_cosine	= math::cos( l->spot_penumbra_angle/2.f );
			backend::ref().set_ps_constant	( m_c_light_spot_penumbra_half_angle_cosine, penumbra_half_angle_cosine );
			float const umbra_half_angle_cosine	= math::cos( l->spot_umbra_angle/2.f );
			backend::ref().set_ps_constant	( m_c_light_spot_umbra_half_angle_cosine, umbra_half_angle_cosine );
			backend::ref().set_ps_constant	( m_c_light_spot_inversed_umbra_half_angle_cosine_minus_penumbra_half_angle_cosine,
				1.f/( math::max(umbra_half_angle_cosine - penumbra_half_angle_cosine, 0.0001f) )
			);
			backend::ref().set_ps_constant	( m_c_light_spot_falloff, l->spot_falloff );

			// plane spot light specific
			backend::ref().set_ps_constant	( m_c_light_local_to_world,	l->m_plane_spot_xform * m_context->get_v() );

			backend::ref().set_ps_constant	( m_c_light_color, light_color );
			backend::ref().set_ps_constant	( m_c_light_intensity, l->intensity );
			backend::ref().set_ps_constant	( m_c_lighting_model, l->lighting_model );

			float3 const* const eye_rays	= m_context->get_eye_rays();
			backend::ref().set_ps_constant	( m_c_eye_ray_corner,	((float4*)eye_rays)[0] );
			backend::ref().set_vs_constant	( m_c_near_far, m_context->get_near_far());
			draw = true;
			break;
		}
		default : NODEFAULT( );
	}

	if (draw)
	{
		backend::ref().set_ps_constant	( m_far_fog_color_and_distance, float4(m_context->get_scene_view()->post_process_parameters().environment_far_fog_color, m_context->get_scene_view()->post_process_parameters().environment_far_fog_distance));
		backend::ref().set_ps_constant	( m_near_fog_distance, m_context->get_scene_view()->post_process_parameters().environment_near_fog_distance);
		backend::ref().set_ps_constant	( m_c_light_color, light_color );
		backend::ref().set_ps_constant	( m_c_light_intensity, l->intensity );
		backend::ref().set_ps_constant	( m_c_lighting_model, l->lighting_model );
		backend::ref().set_ps_constant	( m_c_diffuse_influence_factor, l->diffuse_influence_factor );
		backend::ref().set_ps_constant	( m_c_specular_influence_factor, l->specular_influence_factor );

		m_context->scene()->get_speedtree_forest()->get_speedtree_wind_parameters().set		(tree_component->m_parent->GetWind());
		m_context->scene()->get_speedtree_forest()->get_speedtree_common_parameters().set	(m_context, tree_component, m_context->get_v_inverted().c.xyz());

		backend::ref().set_ps_constant	( m_c_light_type, s32(l->get_type()));

		if (instance)
			m_context->set_w																(
				m_context->scene()->get_speedtree_forest()->get_instance_transform(*instance)
			);
		else
			m_context->set_w																(math::float4x4().identity());

		if (tree_component->get_geometry_type()==SpeedTree::GEOMETRY_TYPE_VERTICAL_BILLBOARDS)
		{
			m_context->scene()->get_speedtree_forest()->get_speedtree_billboard_parameters().set(m_context, tree_component);
			tree_component->m_render_geometry.geom->apply();
			backend::ref().render_indexed( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, tree_component->m_render_geometry.index_count, 0, 0);
		}
		else
		{
			m_context->scene()->get_speedtree_forest()->get_speedtree_tree_parameters().set(tree_component, instance, instance_lod);
			tree_component->m_render_geometry.geom->apply();
			backend::ref().render_indexed( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, lod->num_indices, lod->start_index, 0);
		}
	}
}

void stage_lights::execute_disabled( )
{
	backend::ref().set_render_targets			(&*m_context->get_rt(rt_accumulator_diffuse), &*m_context->get_rt(rt_accumulator_specular), 0, 0);
	backend::ref().reset_depth_stencil_target	();

	float packed_color = math::pow( 0.125f, 0.5f );

	backend::ref().clear_render_targets			(
		math::color( packed_color, packed_color, packed_color, packed_color),
		math::color( 0.0f, 0.0f, 0.0f, 0.0f),
		math::color( 0.0f, 0.0f, 0.0f, 0.0f),
		math::color( 0.0f, 0.0f, 0.0f, 0.0f)
	);
}

void stage_lights::execute( )
{
	if (!is_effects_ready())
		return;

	if (!is_enabled())
	{
		execute_disabled();
		return;
	}

	lights_db::lights_type const&	e_lights = m_context->scene()->lights().get_lights();

	if (!identity(m_is_forward_lighting_pass))
	{
		m_context->set_w							(float4x4().identity());
		backend::ref().set_render_targets			(&*m_context->get_rt(rt_accumulator_diffuse), &*m_context->get_rt(rt_accumulator_specular), 0, 0);
		backend::ref().reset_depth_stencil_target	();

		vector< light_ptr >& visible_lights = m_context->get_scene_view()->get_visible_lights();
		for (vector< light_ptr >::const_iterator it = visible_lights.begin(); it != visible_lights.end(); ++it)
		{
			light* const l = &**it;
			if ((options::ref().current.m_use_hiz_occlusion_culling && l->is_occluded()) || !l->m_enabled)
				continue;

			render_light(l, false);
		}

		m_context->set_w					( float4x4().identity() );
		return;
	}

	BEGIN_CPUGPU_TIMER(statistics::ref().lights_stat_group.forward_lighting_time);

	backend::ref().set_render_targets( &*m_context->get_rt(rt_generic_0), 0, 0, 0);

	environment_probe** probe_it = m_context->get_scene_view()->get_visible_environment_probes().begin();

	struct sort_by_size_predicate {
		bool operator()( environment_probe const* left, environment_probe const* right ) const
		{
			return left->m_properties.radius > right->m_properties.radius;
		}
	};

	std::sort(
		probe_it,
		m_context->get_scene_view()->get_visible_environment_probes().end(),
		sort_by_size_predicate()
	);

	environment_probe* found_probe = 0;
	float min_dist = math::float_max;
	float max_rad = 0.0f;

	vector< render_surface_instance* > m_dynamic_visuals;
	m_context->scene()->select_models( m_context->get_culling_vp(), m_dynamic_visuals, m_context->get_v_inverted().c.xyz(), visible_flag, false );

	vector< render_surface_instance* >::iterator it_d		= m_dynamic_visuals.begin();
	vector< render_surface_instance* >::const_iterator end_d	= m_dynamic_visuals.end();

	for ( ; it_d != end_d; ++it_d)
	{
		render_surface_instance& instance = *(*it_d);
		material_effects& me		= instance.m_render_surface->get_material_effects();
		render_geometry& geometry	= instance.m_render_surface->m_render_geometry;

		if ( !me.stage_enable[lighting_render_stage])
			continue;

		m_context->set_w(*instance.m_transform);
		geometry.geom->apply();

		for ( ; probe_it != m_context->get_scene_view()->get_visible_environment_probes().end(); ++probe_it)
		{
			environment_probe* probe = *probe_it;
			if ((options::ref().current.m_use_hiz_occlusion_culling && probe->is_occluded()) || !probe->m_texture || !probe->m_properties.enabled)
				continue;

			math::aabb bbox = instance.m_parent->get_aabb();
			bbox.modify(*instance.m_transform);

			float3 model_location = instance.m_transform->c.xyz();
			float const model_max_scale = math::max(
				math::max(
					(bbox.max.x - bbox.min.x) * 0.5f,
					(bbox.max.y - bbox.min.y) * 0.5f
				),
				(bbox.max.z - bbox.min.z) * 0.5f
			);

			if (bbox.extents().squared_length() > 2.0f)
			{
				if (probe->m_properties.radius <= max_rad)
					continue;

				max_rad = probe->m_properties.radius;
				found_probe = probe;
				continue;
			}

			float const model_probe_sqdist = (model_location - probe->m_properties.location).length();

			if (
				probe->m_properties.transform.i.xyz().squared_length() &&
				probe->m_properties.transform.j.xyz().squared_length() &&
				probe->m_properties.transform.k.xyz().squared_length()
			)
				probe->m_properties.transform.get_scale();
			else
				LOG_ERROR("invalid probe->m_properties.transform");

			if (probe->m_properties.radius * 0.45f + model_max_scale < model_probe_sqdist)
				continue;

			if (min_dist <= model_probe_sqdist)
				continue;

			min_dist = model_probe_sqdist;
			found_probe = probe;
		}

		if (found_probe)
			render_model_probe_lighting(&instance, found_probe, 0.0f);

		light* L = m_context->scene()->lights().get_sun().c_ptr();

		if (L && L->m_enabled)
			render_model_lighting( &instance, L );

		for (lights_db::lights_type::const_iterator e_it=e_lights.begin() ; e_it!=e_lights.end(); ++e_it)
		{
			light_ptr L = e_it->light;
			if ((options::ref().current.m_use_hiz_occlusion_culling && L->is_occluded()) || !L->m_enabled)
				continue;

			render_model_lighting( &instance, &*L);
		}
	}

	// SpeedTree
	if (options::ref().current.m_enabled_draw_speedtree && m_context->scene()->get_speedtree_forest())
	{
		speedtree_forest::tree_render_info_array_type visible_trees;

		m_context->scene()->get_speedtree_forest()->get_visible_tree_components(m_context, m_context->get_v_inverted().c.xyz(), true, visible_trees);

		for (speedtree_forest::tree_render_info_array_type::iterator it = visible_trees.begin(); it != visible_trees.end(); ++it)
		{
			m_context->set_w(math::float4x4().identity());

			if (!it->tree_component->get_material_effects().stage_enable[lighting_render_stage] || !it->tree_component->get_material_effects().m_effects[lighting_render_stage])
				continue;

			light* L = m_context->scene()->lights().get_sun().c_ptr();

			if (L && L->m_enabled)
				render_speedtree_lighting(it->lod, it->instance, it->instance_lod, it->tree_component, L);

			for (lights_db::lights_type::const_iterator e_it=e_lights.begin() ; e_it!=e_lights.end(); ++e_it)
			{
				render_speedtree_lighting(it->lod, it->instance, it->instance_lod, it->tree_component, &*e_it->light);
			}
		}
	}

	if (options::ref().current.m_lighting_quality)
	{
		particle::world* part_world = m_context->scene()->particle_world();

		if (!part_world)
		{
			m_context->set_w					( float4x4().identity() );
			return;
		}

		D3D11_VIEWPORT orig_viewport;
		backend::ref().get_viewport(orig_viewport);

		D3D11_VIEWPORT tmp_viewport;
		tmp_viewport.TopLeftX = 0.0f;
		tmp_viewport.TopLeftY = 0.0f;
		tmp_viewport.Width = float(m_context->get_rt(rt_generic_0)->width());
		tmp_viewport.Height = float(m_context->get_rt(rt_generic_0)->height());
		tmp_viewport.MinDepth = 0.0f;
		tmp_viewport.MaxDepth = 1.0f;
		backend::ref().set_viewport(tmp_viewport);

		backend::ref().set_render_targets(&*m_context->get_rt(rt_generic_0), 0, 0, 0);

		particle::render_particle_emitter_instances_type emitters(g_allocator);
		part_world->get_render_emitter_instances(m_context->get_culling_vp(), emitters);

		for (particle::render_particle_emitter_instances_type::const_iterator it=emitters.begin(); it!=emitters.end(); ++it)
		{
			render::render_particle_emitter_instance* instance = static_cast< render::render_particle_emitter_instance* >(*it);
			u32 const num_particles = instance->get_num_particles();

			if (!num_particles)
				continue;

			particle::enum_particle_render_mode particle_render_mode = m_context->get_scene_view()->get_particles_render_mode();

			for ( ; probe_it != m_context->get_scene_view()->get_visible_environment_probes().end(); ++probe_it)
			{
				environment_probe* probe = *probe_it;
				if ((options::ref().current.m_use_hiz_occlusion_culling && probe->is_occluded()) || !probe->m_texture || !probe->m_properties.enabled)
					continue;

				math::aabb bbox = instance->get_aabb();
				bbox.modify(instance->transform());

				if (probe->m_properties.radius <= max_rad)
					continue;

				max_rad = probe->m_properties.radius;
				found_probe = probe;
			}

			if (found_probe)
				render_particle_probe_lighting(instance, found_probe, num_particles);

			light* L = m_context->scene()->lights().get_sun().c_ptr();

			if (L && L->m_enabled)
				render_particle_lighting(instance, L, num_particles);

			if (particle_render_mode==particle::normal_particle_render_mode && instance->get_material_effects().stage_enable[lighting_render_stage])
			{
				for (lights_db::lights_type::const_iterator e_it=e_lights.begin() ; e_it!=e_lights.end(); ++e_it)
				{
					if (&*e_it->light == L)
						continue;

					render_particle_lighting(instance, &*e_it->light, num_particles);
				}
			}
		}

		backend::ref().set_viewport(orig_viewport);
	}

	END_CPUGPU_TIMER;

	m_context->set_w					( float4x4().identity() );

	backend::ref().reset_render_targets();
	backend::ref().reset_depth_stencil_target();
}

u32 stage_lights::index_to_shadow_size( u32 size_index ) const
{
	switch (size_index)
	{
		case 0:
			return 1024;
		case 1:
			return 512;
		default:
			return 256;
	}
}

void stage_lights::render_shadowed_light( light* l )
{
	float light_range								= l->range;
	float3 light_color								= l->color;
	float3 const light_position						= m_context->get_v().transform_position( l->position );

	res_geometry_ptr geometry						= m_pyramid_geometry.geometry;

	for (u32 face_index=0; face_index<6; face_index++)
	{
		float const range_X_tan_penumbra_angle_div_2= light_range * math::tan(math::pi_d4);
		bool const distribute_shadow = l->is_cast_shadows_in(light::shadow_distribution_side(face_index));

		math::float4x4 face_view_matrix = math::create_camera_at(
			l->position + view_matrix_parameters[face_index][0],
			l->position + view_matrix_parameters[face_index][1],
			view_matrix_parameters[face_index][2]
		);

		float4x4	scale_matrix					= math::create_scale( float3( range_X_tan_penumbra_angle_div_2, range_X_tan_penumbra_angle_div_2, light_range ) );

		float4x4 local_to_world						= face_view_matrix;
		local_to_world.try_invert(local_to_world);
		local_to_world								= scale_matrix * local_to_world;

		if (distribute_shadow)
		{
			math::float4x4 face_projection_matrix	= math::create_perspective_projection(math::pi_d2, 1.0f, s_shadow_z_near_value, light_range);

			u32 const spot_shadow_map_size			= index_to_shadow_size(l->shadow_map_size_index);//spot_shadow_map_size_option->value();
			u32 const marge							= 0; // TODO: get from point light filter settings , and pass to shader

			backend::ref().flush_rt_shader_resources();

			render_to_hw_shadowmap					(
				l,
				0,
				l->shadow_z_bias,
				spot_shadow_map_size,
				l->shadow_map_size_index,
				face_view_matrix,
				face_projection_matrix, marge
			);
		}

		m_context->set_w							( local_to_world );

		BEGIN_CPUGPU_TIMER(statistics::ref().lights_stat_group.accumulate_lighting_time);

		backend::ref().set_render_targets			(&*m_context->get_rt(rt_accumulator_diffuse), &*m_context->get_rt(rt_accumulator_specular), 0, 0);
		backend::ref().reset_depth_stencil_target	();

		// Mask opaque geometry.
		m_effect_accum_mask->apply					(0 /* tech_mask_local_light */, 0);
		geometry->apply								();
		backend::ref().render_indexed				( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 6*3, 0, 0);

		float3 const light_direction				= m_context->get_v().transform_direction( l->direction );

		switch ( l->get_type() ) {
			case light_type_point : {
				for (u32 tech_index=0; tech_index<2; tech_index++)
				{
					if (distribute_shadow)
					{
						m_shadowed_point_light_accumulator->apply( tech_index, 0 );
						backend::ref().set_ps_constant( m_c_shadow_transparency, l->shadow_transparency );
						backend::ref().set_ps_constant( m_c_view_to_light_matrix, transpose(m_view_to_light_matrix));
						backend::ref().set_ps_constant( m_c_shadow_z_bias, m_shadow_z_bias);
						backend::ref().set_ps_constant( m_c_shadow_map_size, m_shadow_map_size);
						backend::ref().set_ps_texture("shadowmap_texture", &*m_shadow_depth_stencil_texture[l->shadow_map_size_index]);
					}
					else
					{
						m_point_light_accumulator->apply( tech_index, 0 );
					}

					backend::ref().set_ps_constant	( m_c_light_position,		light_position );
					backend::ref().set_ps_constant	( m_c_light_range,			l->range );
					backend::ref().set_ps_constant	( m_c_light_attenuation_power,	l->attenuation_power );

					backend::ref().set_ps_constant	( m_c_diffuse_influence_factor, l->diffuse_influence_factor );
					backend::ref().set_ps_constant	( m_c_specular_influence_factor, l->specular_influence_factor );
					backend::ref().set_ps_constant	( m_c_is_shadower, 0.0f );

					backend::ref().set_ps_constant	( m_c_light_color, light_color );
					backend::ref().set_ps_constant	( m_c_light_intensity, l->intensity );
					backend::ref().set_ps_constant	( m_c_lighting_model, l->lighting_model );

					float3 const* const eye_rays	= m_context->get_eye_rays();
					backend::ref().set_ps_constant	( m_c_eye_ray_corner,	((float4*)eye_rays)[0] );
					backend::ref().set_vs_constant	( m_c_near_far, m_context->get_near_far());
					geometry->apply					( );
					backend::ref().render_indexed	( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 6*3, 0, 0);
				}
				break;
			}
			case light_type_obb : {
				for (u32 tech_index=0; tech_index<2; tech_index++)
				{
					if (distribute_shadow)
					{
						m_shadowed_obb_light_accumulator->apply( tech_index, 0 );
						backend::ref().set_ps_constant( m_c_shadow_transparency, l->shadow_transparency );
						backend::ref().set_ps_constant( m_c_view_to_light_matrix, transpose(m_view_to_light_matrix));
						backend::ref().set_ps_constant( m_c_shadow_z_bias, m_shadow_z_bias);
						backend::ref().set_ps_constant( m_c_shadow_map_size, m_shadow_map_size);
						backend::ref().set_ps_texture ("shadowmap_texture", &*m_shadow_depth_stencil_texture[l->shadow_map_size_index]);
					}
					else
					{
						m_obb_light_accumulator->apply( tech_index, 0 );
					}

					backend::ref().set_ps_constant	( m_c_light_position,		light_position );
					backend::ref().set_ps_constant	( m_c_light_range,			light_range );
					backend::ref().set_ps_constant	( m_c_light_attenuation_power,	l->attenuation_power );

					backend::ref().set_ps_constant	( m_c_diffuse_influence_factor, l->diffuse_influence_factor );
					backend::ref().set_ps_constant	( m_c_specular_influence_factor, l->specular_influence_factor );

					float4x4 obb_world				= l->m_xform;
					obb_world.set_scale				( l->scale );
					backend::ref().set_ps_constant	( m_c_light_local_to_world,	obb_world * m_context->get_v() );

					backend::ref().set_ps_constant	( m_c_light_color, light_color );
					backend::ref().set_ps_constant	( m_c_light_intensity, l->intensity );
					backend::ref().set_ps_constant	( m_c_lighting_model, l->lighting_model );

					float3 const* const eye_rays	= m_context->get_eye_rays();
					backend::ref().set_ps_constant	( m_c_eye_ray_corner,	((float4*)eye_rays)[0] );
					backend::ref().set_vs_constant	( m_c_near_far, m_context->get_near_far());
					geometry->apply					( );
					backend::ref().render_indexed	( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 6*3, 0, 0);
				}
				break;
			}
			case light_type_sphere : {
				for (u32 tech_index=0; tech_index<2; tech_index++)
				{
					if (distribute_shadow)
					{
						m_shadowed_sphere_light_accumulator->apply( tech_index, 0 );
						backend::ref().set_ps_constant( m_c_shadow_transparency, l->shadow_transparency );
						backend::ref().set_ps_constant( m_c_view_to_light_matrix, transpose(m_view_to_light_matrix));
						backend::ref().set_ps_constant( m_c_shadow_z_bias, m_shadow_z_bias);
						backend::ref().set_ps_constant( m_c_shadow_map_size, m_shadow_map_size);
						backend::ref().set_ps_texture("shadowmap_texture", &*m_shadow_depth_stencil_texture[l->shadow_map_size_index]);
					}
					else
					{
						m_sphere_light_accumulator->apply( tech_index, 0 );
					}
					backend::ref().set_ps_constant	( m_c_light_position,		light_position );
					backend::ref().set_ps_constant	( m_c_light_range,			light_range );
					backend::ref().set_ps_constant	( m_c_light_attenuation_power,	l->attenuation_power );

					backend::ref().set_ps_constant	( m_c_diffuse_influence_factor, l->diffuse_influence_factor );
					backend::ref().set_ps_constant	( m_c_specular_influence_factor, l->specular_influence_factor );

					backend::ref().set_ps_constant	( m_c_light_sphere_radius,	l->scale.x );

					backend::ref().set_ps_constant	( m_c_light_color, light_color );
					backend::ref().set_ps_constant	( m_c_light_intensity, l->intensity );
					backend::ref().set_ps_constant	( m_c_lighting_model, l->lighting_model );

					float3 const* const eye_rays	= m_context->get_eye_rays();
					backend::ref().set_ps_constant	( m_c_eye_ray_corner,	((float4*)eye_rays)[0] );
					backend::ref().set_vs_constant	( m_c_near_far, m_context->get_near_far());
					geometry->apply					( );
					backend::ref().render_indexed	( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 6*3, 0, 0);
				}
				break;
			}
			default : NODEFAULT( );
		}
		END_CPUGPU_TIMER;
	}
}

void stage_lights::render_light( light* l, bool shadowers_pass )
{
	if (l->is_shadower && !shadowers_pass)
		return;

	if (!l->is_shadower && shadowers_pass)
		return;

	if (l->get_type()==light_type_parallel)
		return;

	if ((l->get_type()==light_type_point  ||
		 l->get_type()==light_type_sphere ||
		 l->get_type()==light_type_obb) && l->is_cast_shadows() && options::ref().current.m_enabled_local_light_shadows)
	{
		render_shadowed_light(l);
		return;
	}

	// Common
	float light_range				= l->range;
	float3 light_color				= l->color;
	float3 const light_position		= m_context->get_v().transform_position( l->position );
	float3 const light_direction	= m_context->get_v().transform_direction( l->direction );

	// Xforms
	l->xform_calc					( );
	m_context->set_w				( l->m_xform );

	res_geometry_ptr geometry;
	switch ( l->get_type() ) {
		case light_type_point :
		case light_type_sphere : {
			geometry				= m_sphere_geometry.geometry;
			break;
		}
		case light_type_spot : {
			geometry				= m_pyramid_geometry.geometry;
			break;
		}
		case light_type_obb :
		case light_type_capsule :
		case light_type_plane_spot : {
			geometry				= m_obb_geometry.geometry;
			break;
		}
		case light_type_parallel : break;
		default : NODEFAULT( );
	}

	switch ( l->get_type() ) {
		case light_type_spot : {
			if (l->is_cast_shadows() && options::ref().current.m_enabled_local_light_shadows)
				make_spot_light_shadowmap(0, l);
			break;
		}
		case light_type_plane_spot : {
			if (l->is_cast_shadows() && options::ref().current.m_enabled_local_light_shadows)
				make_plane_spot_light_shadowmap(0, l);
			break;
		}
	}

	m_context->set_w				( l->m_xform );

	BEGIN_CPUGPU_TIMER(statistics::ref().lights_stat_group.accumulate_lighting_time);

	backend::ref().set_render_targets	( &*m_context->get_rt(rt_accumulator_diffuse), &*m_context->get_rt(rt_accumulator_specular), 0, 0);
	backend::ref().reset_depth_stencil_target();

	{
		// Mask opaque geometry.
		m_effect_accum_mask->apply	(0 /* tech_mask_local_light */, 0);
		geometry->apply				();
		draw_geometry				(l);
	}

	switch ( l->get_type() ) {
		case light_type_point : {

			u32 num_passes = 2;

			for (u32 tech_index = 0; tech_index < num_passes; tech_index++)
			{
				{
					if (l->is_shadower)
						m_point_light_shadower->apply( tech_index, 0 );
					else
						m_point_light_accumulator->apply( tech_index, 0 );
				}

				backend::ref().set_ps_constant	( m_c_light_position,		light_position );
				backend::ref().set_ps_constant	( m_c_light_range,			light_range );
				backend::ref().set_ps_constant	( m_c_light_attenuation_power,	l->attenuation_power );

				backend::ref().set_ps_constant	( m_c_diffuse_influence_factor, l->diffuse_influence_factor );
				backend::ref().set_ps_constant	( m_c_specular_influence_factor, l->specular_influence_factor );

				backend::ref().set_ps_constant	( m_c_is_shadower, l->is_shadower ? 1.0f : 0.0f );

				backend::ref().set_ps_constant	( m_c_light_color, light_color );
				backend::ref().set_ps_constant	( m_c_light_intensity, l->intensity );
				backend::ref().set_ps_constant	( m_c_lighting_model, l->lighting_model );
				float3 const* const eye_rays	= m_context->get_eye_rays();
				backend::ref().set_ps_constant	( m_c_eye_ray_corner,	((float4*)eye_rays)[0] );
				backend::ref().set_vs_constant	( m_c_near_far, m_context->get_near_far());
				geometry->apply					( );
				draw_geometry					( l );
			}
			break;
		}
		case light_type_spot : {
			for (u32 tech_index=0; tech_index<2; tech_index++)
			{
				if (l->is_cast_shadows() && options::ref().current.m_enabled_local_light_shadows)
				{
					m_shadowed_spot_light_accumulator->apply( tech_index, 0 );
					backend::ref().set_ps_constant( m_c_shadow_transparency, l->shadow_transparency );
					backend::ref().set_ps_constant( m_c_view_to_light_matrix, transpose(m_view_to_light_matrix));
					backend::ref().set_ps_constant( m_c_shadow_z_bias, m_shadow_z_bias);
					backend::ref().set_ps_constant( m_c_shadow_map_size, m_shadow_map_size);
					if (l->static_shadows)
						backend::ref().set_ps_texture("shadowmap_texture", &*l->m_shadow_depth_stencil_texture);
					else
						backend::ref().set_ps_texture("shadowmap_texture", &*m_shadow_depth_stencil_texture[l->shadow_map_size_index]);
				}
				else
					m_spot_light_accumulator->apply	( tech_index, 0 );

				backend::ref().set_ps_constant	( m_c_light_position,		light_position );
				backend::ref().set_ps_constant	( m_c_light_direction,		light_direction );
				backend::ref().set_ps_constant	( m_c_light_range,			light_range/math::sin(l->spot_penumbra_angle/2.f) );
				backend::ref().set_ps_constant	( m_c_light_attenuation_power,	l->attenuation_power );

				backend::ref().set_ps_constant	( m_c_diffuse_influence_factor, l->diffuse_influence_factor );
				backend::ref().set_ps_constant	( m_c_specular_influence_factor, l->specular_influence_factor );
				backend::ref().set_ps_constant	( m_c_is_shadower, 0.0f );

				float const penumbra_half_angle_cosine	= math::cos( l->spot_penumbra_angle/2.f );
				backend::ref().set_ps_constant	( m_c_light_spot_penumbra_half_angle_cosine, penumbra_half_angle_cosine );
				float const umbra_half_angle_cosine	= math::cos( l->spot_umbra_angle/2.f );
				backend::ref().set_ps_constant	( m_c_light_spot_umbra_half_angle_cosine, umbra_half_angle_cosine );
				backend::ref().set_ps_constant	( m_c_light_spot_inversed_umbra_half_angle_cosine_minus_penumbra_half_angle_cosine,
					1.f/( math::max(umbra_half_angle_cosine - penumbra_half_angle_cosine, 0.0001f) )
				);
				backend::ref().set_ps_constant	( m_c_light_spot_falloff, l->spot_falloff );

				backend::ref().set_ps_constant	( m_c_light_color, light_color );
				backend::ref().set_ps_constant	( m_c_light_intensity, l->intensity );
				backend::ref().set_ps_constant	( m_c_lighting_model, l->lighting_model );

				float3 const* const eye_rays	= m_context->get_eye_rays();
				backend::ref().set_ps_constant	( m_c_eye_ray_corner,	((float4*)eye_rays)[0] );
				backend::ref().set_vs_constant	( m_c_near_far, m_context->get_near_far());
				geometry->apply					( );
				draw_geometry					( l );
			}

			break;
		}
		case light_type_obb : {
			for (u32 tech_index=0; tech_index<2; tech_index++)
			{
				m_obb_light_accumulator->apply	( tech_index, 0 );

				backend::ref().set_ps_constant	( m_c_light_position,		light_position );
				backend::ref().set_ps_constant	( m_c_light_range,			light_range );
				backend::ref().set_ps_constant	( m_c_light_attenuation_power,	l->attenuation_power );

				backend::ref().set_ps_constant	( m_c_diffuse_influence_factor, l->diffuse_influence_factor );
				backend::ref().set_ps_constant	( m_c_specular_influence_factor, l->specular_influence_factor );
				backend::ref().set_ps_constant	( m_c_is_shadower, 0.0f );

				float4x4 obb_world				= l->m_xform;
				obb_world.set_scale				( l->scale );
				backend::ref().set_ps_constant	( m_c_light_local_to_world,	obb_world * m_context->get_v() );

				backend::ref().set_ps_constant	( m_c_light_color, light_color );
				backend::ref().set_ps_constant	( m_c_light_intensity, l->intensity );
				backend::ref().set_ps_constant	( m_c_lighting_model, l->lighting_model );

				float3 const* const eye_rays	= m_context->get_eye_rays();
				backend::ref().set_ps_constant	( m_c_eye_ray_corner,	((float4*)eye_rays)[0] );
				backend::ref().set_vs_constant	( m_c_near_far, m_context->get_near_far());
				geometry->apply					( );
				draw_geometry					( l );
			}
			break;
		}
		case light_type_capsule : {
			for (u32 tech_index=0; tech_index<2; tech_index++)
			{
				m_capsule_light_accumulator->apply ( tech_index, 0 );

				backend::ref().set_ps_constant	( m_c_light_position,		light_position );
				backend::ref().set_ps_constant	( m_c_light_direction,		light_direction );
				backend::ref().set_ps_constant	( m_c_light_range,			light_range );
				backend::ref().set_ps_constant	( m_c_light_attenuation_power,	l->attenuation_power );

				backend::ref().set_ps_constant	( m_c_diffuse_influence_factor, l->diffuse_influence_factor );
				backend::ref().set_ps_constant	( m_c_specular_influence_factor, l->specular_influence_factor );
				backend::ref().set_ps_constant	( m_c_is_shadower, 0.0f );

				backend::ref().set_ps_constant	( m_c_light_capsule_half_width,	l->scale.z );
				backend::ref().set_ps_constant	( m_c_light_capsule_radius,	l->scale.x );

				backend::ref().set_ps_constant	( m_c_light_color, light_color );
				backend::ref().set_ps_constant	( m_c_light_intensity, l->intensity );
				backend::ref().set_ps_constant	( m_c_lighting_model, l->lighting_model );

				float3 const* const eye_rays	= m_context->get_eye_rays();
				backend::ref().set_ps_constant	( m_c_eye_ray_corner,	((float4*)eye_rays)[0] );
				backend::ref().set_vs_constant	( m_c_near_far, m_context->get_near_far());
				geometry->apply					( );
				draw_geometry					( l );
			}
			break;
		}
		case light_type_parallel : {
			NODEFAULT();
		}
		case light_type_sphere : {
			for (u32 tech_index=0; tech_index<2; tech_index++)
			{
				m_sphere_light_accumulator->apply( tech_index, 0 );

				backend::ref().set_ps_constant	( m_c_light_position,		light_position );
				backend::ref().set_ps_constant	( m_c_light_range,			light_range );
				backend::ref().set_ps_constant	( m_c_light_attenuation_power,	l->attenuation_power );

				backend::ref().set_ps_constant	( m_c_diffuse_influence_factor, l->diffuse_influence_factor );
				backend::ref().set_ps_constant	( m_c_specular_influence_factor, l->specular_influence_factor );
				backend::ref().set_ps_constant	( m_c_is_shadower, 0.0f );

				backend::ref().set_ps_constant	( m_c_light_sphere_radius,	l->scale.x );

				backend::ref().set_ps_constant	( m_c_light_color, light_color );
				backend::ref().set_ps_constant	( m_c_light_intensity, l->intensity );
				backend::ref().set_ps_constant	( m_c_lighting_model, l->lighting_model );

				float3 const* const eye_rays	= m_context->get_eye_rays();
				backend::ref().set_ps_constant	( m_c_eye_ray_corner,	((float4*)eye_rays)[0] );
				backend::ref().set_vs_constant	( m_c_near_far, m_context->get_near_far());
				geometry->apply					( );
				draw_geometry					( l );
			}
			break;
		}
		case light_type_plane_spot : {
			for (u32 tech_index=0; tech_index<2; tech_index++)
			{
				if (l->is_cast_shadows() && options::ref().current.m_enabled_local_light_shadows)
				{
					m_shadowed_plane_spot_light_accumulator->apply( tech_index, 0 );
					backend::ref().set_ps_constant( m_c_view_to_light_matrix, transpose( m_view_to_light_matrix ) );
					backend::ref().set_ps_constant( m_c_shadow_transparency, l->shadow_transparency );
					backend::ref().set_ps_constant( m_c_shadow_z_bias, m_shadow_z_bias);
					backend::ref().set_ps_constant( m_c_shadow_map_size, m_shadow_map_size);
					backend::ref().set_ps_texture("shadowmap_texture", &*m_shadow_depth_stencil_texture[l->shadow_map_size_index]);
				}
				else
					m_plane_spot_light_accumulator->apply	( tech_index, 0 );

				backend::ref().set_ps_constant	( m_c_light_position,		light_position );
				backend::ref().set_ps_constant	( m_c_light_direction,		light_direction );
				backend::ref().set_ps_constant	( m_c_light_range,			light_range/math::sin(l->spot_penumbra_angle/2.f) );
				backend::ref().set_ps_constant	( m_c_light_attenuation_power,	l->attenuation_power );

				backend::ref().set_ps_constant	( m_c_diffuse_influence_factor, l->diffuse_influence_factor );
				backend::ref().set_ps_constant	( m_c_specular_influence_factor, l->specular_influence_factor );
				backend::ref().set_ps_constant	( m_c_is_shadower, 0.0f );

				float const penumbra_half_angle_cosine	= math::cos( l->spot_penumbra_angle/2.f );
				backend::ref().set_ps_constant	( m_c_light_spot_penumbra_half_angle_cosine, penumbra_half_angle_cosine );
				float const umbra_half_angle_cosine	= math::cos( l->spot_umbra_angle/2.f );
				backend::ref().set_ps_constant	( m_c_light_spot_umbra_half_angle_cosine, umbra_half_angle_cosine );
				backend::ref().set_ps_constant	( m_c_light_spot_inversed_umbra_half_angle_cosine_minus_penumbra_half_angle_cosine,
					1.f/( math::max(umbra_half_angle_cosine - penumbra_half_angle_cosine, 0.0001f) )
				);
				backend::ref().set_ps_constant	( m_c_light_spot_falloff, l->spot_falloff );

				// plane spot light specific
				backend::ref().set_ps_constant	( m_c_light_local_to_world,	transpose( l->m_plane_spot_xform * m_context->get_v() ) );

				backend::ref().set_ps_constant	( m_c_light_color, light_color );
				backend::ref().set_ps_constant	( m_c_light_intensity, l->intensity );
				backend::ref().set_ps_constant	( m_c_lighting_model, l->lighting_model );

				float3 const* const eye_rays	= m_context->get_eye_rays();
				backend::ref().set_ps_constant	( m_c_eye_ray_corner,	((float4*)eye_rays)[0] );
				backend::ref().set_vs_constant	( m_c_near_far, m_context->get_near_far());
				geometry->apply					( );
				draw_geometry					( l );
			}
			break;
		}
		default : NODEFAULT( );
	}
	END_CPUGPU_TIMER;

	if (!s_one_light_dip_value)
	{
		backend::ref().flush_rt_shader_resources();
	}
}

void stage_lights::draw_geometry( light* l )
{
	switch ( l->get_type() ) {
		case light_type_sphere :
		case light_type_point : {
			backend::ref().render_indexed( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, DU_SPHERE_NUMFACES*3, 0, 0);
			break;
		}
		case light_type_spot : {
			backend::ref().render_indexed( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 6*3, 0, 0);
			break;
		}
		case light_type_obb :
		case light_type_capsule :
		case light_type_plane_spot : {
			backend::ref().render_indexed( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, geometry_utils::cube_solid::face_count*3, 0, 0);
			break;
		}
		default : NODEFAULT( );
	}
}

void stage_lights::debug_render( )
{
	static bool s_debug_render = false;
	if ( !s_debug_render )
		return;

	math::color const color	= math::color(u32(0), 255, 32);

	backend::ref().set_render_targets	( &*m_context->get_rt(rt_present), 0, 0, 0);

	typedef lights_db::lights_type	lights_type;
	lights_type const& lights	= m_context->scene()->lights().get_lights();
	for ( lights_type::const_iterator i = lights.begin(), e = lights.end(); i != e; ++i ) {
		system_renderer::ref().draw_aabb	( (*i).light->m_aabb, math::color( u32(255), 0, 0 ) );
		if ( (*i).light->get_type() != light_type_plane_spot )
			continue;

		float4x4 transform		= (*i).light->m_xform;
		vertex_colored vertices[8];
		for ( u32 i = 0; i<array_size(vertices); ++i )
			vertices[i]			= vertex_colored(
				transform.transform_position(
					float3(
						geometry_utils::cube_solid::vertices[3*i + 0],
						geometry_utils::cube_solid::vertices[3*i + 1],
						geometry_utils::cube_solid::vertices[3*i + 2]
					)
				),
				color
			);

		system_renderer::ref().draw_triangles(
			vertices,
			&vertices[0] + array_size(vertices),
			geometry_utils::cube_solid::faces,
			geometry_utils::cube_solid::faces + geometry_utils::cube_solid::index_count,
			false
		);
	}
}

} // namespace render
} // namespace vostok
