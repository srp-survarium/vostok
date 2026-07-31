#ifndef VOSTOK_RENDER_ENGINE_STAGE_LIGHTS_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_STAGE_LIGHTS_H_INCLUDED

#include <vostok/fixed_string.h>
#include <vostok/math_float3.h>
#include <vostok/math_float4x4.h>
#include <vostok/render/core/dx11/res_declaration.h>
#include <vostok/render/core/res_effect.h>
#include <vostok/render/core/untyped_buffer.h>

#include "render_target.h"
#include "res_geometry.h"
#include "res_texture.h"
#include "stage.h"

namespace SpeedTree {

class CInstance;
struct SInstanceLod;

} // namespace SpeedTree

namespace vostok {
namespace render {

struct environment_probe;
class light;
class lod_entry;
class render_particle_emitter_instance;
struct render_surface_instance;
class renderer;
class renderer_context;
class shader_constant_host;
class speedtree_tree_component;

typedef intrusive_ptr<
	res_texture,
	resource_intrusive_base,
	threading::single_threading_policy
> res_texture_ptr;

typedef resources::resource_ptr<
	res_effect,
	resources::unmanaged_intrusive_base
> res_effect_ptr;

class stage_lights : public stage {
public:
	struct instance_data {
		instance_data( ) { }

		float3	color;
		float3	position;
		float	range;
	};

	STATIC_SIZE_ASSERT( instance_data, 0x1C );

	struct light_geometry {
		light_geometry( ) { }
		~light_geometry( ) { }

		untyped_buffer_ptr	vertex_buffer;
		untyped_buffer_ptr	index_buffer;
		res_geometry_ptr	geometry;
	};

	STATIC_SIZE_ASSERT( light_geometry, 0xC );

	struct lights_instance {
		lights_instance( ) { }
		~lights_instance( ) { }

		untyped_buffer_ptr m_instance_vb;
	};

	STATIC_SIZE_ASSERT( lights_instance, 0x4 );

	stage_lights(
		renderer*			in_renderer,
		renderer_context*	context,
		bool				is_forward_lighting_pass
	);

	virtual ~stage_lights( );

	virtual void execute( ) override;
	virtual void execute_disabled( ) override;
	virtual void debug_render( ) override;

private:
	bool is_effects_ready( ) const;

	void render_model_lighting( render_surface_instance* instance, light* l );
	void render_model_probe_lighting(
		render_surface_instance*	instance,
		environment_probe*			probe,
		float						min_probe_scale
	);

	void render_speedtree_lighting(
		lod_entry const*				lod,
		SpeedTree::CInstance const*		instance,
		SpeedTree::SInstanceLod const*	instance_lod,
		speedtree_tree_component*		tree_component,
		light*							l
	);

	void render_particle_lighting(
		render_particle_emitter_instance*	instance,
		light*								l,
		u32									num_particles
	);

	void render_particle_probe_lighting(
		render_particle_emitter_instance*	instance,
		environment_probe*					probe,
		u32									num_particles
	);

	void render_light( light* l, bool shadowers_pass );
	void render_shadowed_light( light* l );
	void draw_geometry( light* l );

	u32 index_to_shadow_size( u32 size_index ) const;

	void new_sphere_geometry( );
	void create_pyramid_geometry( );
	void create_obb_geometry( );
	void fill_surface( render_target_ptr surf );

	void render_to_cubemap( u32, light* ) { }
	void render_to_cubemap_face( u32, float3 const&, float ) { }

	void make_spot_light_shadowmap( u32 shadow_quality, light* l );
	void make_plane_spot_light_shadowmap( u32 shadow_quality, light* l );

	void render_to_hw_shadowmap(
		light*				l,
		u32					shadow_quality,
		float				z_bias,
		u32					smap_size,
		u32					smap_size_index,
		float4x4 const&		view_matrix,
		float4x4 const&		projection_matrix,
		u32					marge
	);

	void make_skin_scattering_texture( render_surface_instance* instance, light* l );

	void render_instanced_lights( ) { }
	void flush_instanced_lights( u32 ) { }

private:
	bool					m_enable_env_probes;
	render_target_ptr		m_rt_skin_scattering_position;
	res_texture_ptr			m_t_skin_scattering_position;
	render_target_ptr		m_rt_skin_scattering_temp;
	res_texture_ptr			m_t_skin_scattering_temp;
	render_target_ptr		m_rt_skin_scattering;
	res_texture_ptr			m_t_skin_scattering;
	render_target_ptr		m_rt_skin_scattering_stretch;
	res_texture_ptr			m_t_skin_scattering_stretch;
	render_target_ptr		m_rt_skin_scattering_small;
	res_texture_ptr			m_t_skin_scattering_small;
	render_target_ptr		m_rt_skin_scattering_blurred_0;
	res_texture_ptr			m_t_skin_scattering_blurred_0;
	render_target_ptr		m_rt_skin_scattering_blurred_1;
	res_texture_ptr			m_t_skin_scattering_blurred_1;
	render_target_ptr		m_rt_skin_scattering_blurred_2;
	res_texture_ptr			m_t_skin_scattering_blurred_2;
	render_target_ptr		m_rt_skin_scattering_blurred_3;
	res_texture_ptr			m_t_skin_scattering_blurred_3;
	render_target_ptr		m_rt_skin_scattering_blurred_4;
	res_texture_ptr			m_t_skin_scattering_blurred_4;
	render_target_ptr		m_skin_scattering_render_target;
	render_target_ptr		m_skin_scattering_depth_stencil;
	res_texture_ptr			m_skin_scattering_texture;
	render_target_ptr		m_shadow_depth_stencil[3];
	res_texture_ptr			m_shadow_depth_stencil_texture[3];
	render_target_ptr		m_vcm_render_target;
	render_target_ptr		m_vcm_depth_stencil;
	render_target_ptr		m_lookup_vcm_render_target[6];
	render_target_ptr		m_lookup_vcm_depth_stencil[6];
	res_texture_ptr			m_lookup_vcm_texture;
	res_texture_ptr			m_lookup_vcm_depth_stencil_texture;
	fixed_string<64>		m_lookup_vcm_render_target_names[6];
	fixed_string<64>		m_lookup_vcm_depth_stencil_names[6];
	untyped_buffer_ptr		m_lookup_vcm_ib;
	res_geometry_ptr		m_lookup_vcm_geometry;
	render_target_ptr		m_cubemap_face_render_target[6];
	render_target_ptr		m_cubemap_face_depth_stencil[6];
	res_texture_ptr			m_cubemap_texture;
	res_texture_ptr			m_depth_stencil_cubemap_texture;
	fixed_string<64>		m_cubemap_face_render_target_names[6];
	fixed_string<64>		m_cubemap_face_depth_stencil_names[6];
	res_effect_ptr			m_effect_accum_mask;
	res_effect_ptr			m_point_light_shadower;
	res_effect_ptr			m_point_light_accumulator;
	res_effect_ptr			m_shadowed_point_light_accumulator;
	res_effect_ptr			m_spot_light_accumulator;
	res_effect_ptr			m_shadowed_spot_light_accumulator;
	res_effect_ptr			m_capsule_light_accumulator;
	res_effect_ptr			m_obb_light_accumulator;
	res_effect_ptr			m_shadowed_obb_light_accumulator;
	res_effect_ptr			m_sphere_light_accumulator;
	res_effect_ptr			m_shadowed_sphere_light_accumulator;
	res_effect_ptr			m_plane_spot_light_accumulator;
	res_effect_ptr			m_shadowed_plane_spot_light_accumulator;
	res_effect_ptr			m_sh_downsample_skin_irradiance_texture;
	res_effect_ptr			m_sh_fix_irradiance_texture;
	res_effect_ptr			m_shadow_effect;
	light_geometry			m_sphere_geometry;
	light_geometry			m_pyramid_geometry;
	light_geometry			m_obb_geometry;
	untyped_buffer_ptr		m_screen_vertex_ib;
	res_geometry_ptr		m_screen_vertex_geometry;
	untyped_buffer_ptr		m_instance_vb_small;
	res_declaration_ptr		m_instance_declaration;
	res_declaration_ptr		m_instance_declaration_small;
	u32						m_num_instanced_lights;
	instance_data*			m_instance_data_array;
	lights_instance			m_lights_instance[4];
	shader_constant_host*	m_probe_parameters0;
	shader_constant_host*	m_probe_parameters1;
	shader_constant_host*	m_gamma_correction_factor;
	shader_constant_host*	m_shadow[4];
	shader_constant_host*	m_kernel_offsets;
	shader_constant_host*	m_blur_offsets_weights;
	shader_constant_host*	m_c_light_type;
	shader_constant_host*	m_c_light_color;
	shader_constant_host*	m_c_light_intensity;
	shader_constant_host*	m_c_light_position;
	shader_constant_host*	m_c_light_direction;
	shader_constant_host*	m_c_light_attenuation_power;
	shader_constant_host*	m_c_light_range;
	shader_constant_host*	m_far_fog_color_and_distance;
	shader_constant_host*	m_near_fog_distance;
	shader_constant_host*	m_c_diffuse_influence_factor;
	shader_constant_host*	m_c_specular_influence_factor;
	shader_constant_host*	m_c_is_shadower;
	shader_constant_host*	m_c_light_instances;
	float4x4*				m_light_instances;
	shader_constant_host*	m_c_light_spot_penumbra_half_angle_cosine;
	shader_constant_host*	m_c_light_spot_umbra_half_angle_cosine;
	shader_constant_host*	m_c_light_spot_inversed_umbra_half_angle_cosine_minus_penumbra_half_angle_cosine;
	shader_constant_host*	m_c_light_spot_falloff;
	shader_constant_host*	m_c_light_capsule_half_width;
	shader_constant_host*	m_c_light_capsule_radius;
	shader_constant_host*	m_c_light_sphere_radius;
	shader_constant_host*	m_c_light_local_to_world;
	shader_constant_host*	m_c_eye_ray_corner;
	shader_constant_host*	m_c_near_far;
	shader_constant_host*	m_c_view_to_light_matrix;
	shader_constant_host*	m_c_shadow_z_bias;
	shader_constant_host*	m_c_shadow_map_size;
	shader_constant_host*	m_c_use_shadows;
	shader_constant_host*	m_c_shadow_transparency;
	shader_constant_host*	m_c_is_unwrap_pass;
	shader_constant_host*	m_c_lighting_model;
	shader_constant_host*	m_ambient_color;
	float4x4				m_view_to_light_matrix;
	float4x4				m_light_to_texture_matrix;
	float					m_shadow_z_bias;
	float					m_shadow_map_size;
	bool const				m_is_forward_lighting_pass;
};

STATIC_SIZE_ASSERT( stage_lights, 0x9E8 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_STAGE_LIGHTS_H_INCLUDED
