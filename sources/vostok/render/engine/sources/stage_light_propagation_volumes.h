#ifndef VOSTOK_RENDER_ENGINE_STAGE_LIGHT_PROPAGATION_VOLUMES_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_STAGE_LIGHT_PROPAGATION_VOLUMES_H_INCLUDED

#include <vostok/render/core/memory.h>
#include <vostok/render/core/res_effect.h>
#include <vostok/render/core/untyped_buffer.h>

#include "box_geometry.h"
#include "lpv_render_surface.h"
#include "render_target.h"
#include "res_geometry.h"
#include "res_texture.h"
#include "stage.h"

namespace vostok {
namespace render {

struct geometry_batch;
class light;
class renderer;
class renderer_context;
class shader_constant_host;
struct radiance_volume;

typedef intrusive_ptr<
	res_texture,
	resource_intrusive_base,
	threading::single_threading_policy
> res_texture_ptr;

typedef resources::resource_ptr<
	res_effect,
	resources::unmanaged_intrusive_base
> res_effect_ptr;

class stage_light_propagation_volumes : public stage {
public:
	stage_light_propagation_volumes( renderer* in_renderer, renderer_context* context );
	virtual ~stage_light_propagation_volumes( );

	virtual void execute( ) override;

	void execute_impl( );
	void execute_smoothed_impl(
		u32 const	current_cascade_index,
		u32 const	stage_index,
		u32 const	propagation_iteration_index,
		u32 const	render_stage_index,
		u32 const	num_render_stages
	);

	bool is_effects_ready( ) const;
	void draw_debug( );

private:
	virtual void execute_disabled( ) override;

	void pre_lpv_batch_render(
		float3 const&			light_color,
		float const			light_intensity,
		geometry_batch const&	batch
	);

	void post_lpv_batch_render( geometry_batch const& batch );

	void render_to_rms(
		float3 const&		light_color,
		float const			light_intensity,
		float4x4 const&		view_matrix,
		float4x4 const&		projection_matrix,
		vector<float4x4>	transforms,
		u32 const			cascade_index
	);

	void render_to_rms_smoothed(
		float3 const&,
		float const,
		float4x4 const&,
		float4x4 const&,
		vector<float4x4>,
		u32 const,
		u32 const,
		u32 const
	)
	{
	}

	void render_to_rms_smoothed2(
		float3 const&		light_color,
		float const			light_intensity,
		float4x4 const&		view_matrix,
		float4x4 const&		projection_matrix,
		vector<float4x4>	transforms,
		u32 const			cascade_index,
		u32 const			render_stage_index,
		u32 const			num_render_stages
	);

	void render_to_point_rms( light* l, u32 const face_index, vector<float4x4> transforms );
	void render_to_sky_rms( light* sun, u32 const face_index, u32 const cascade_index, vector<float4x4> transforms );
	void render_to_spot_rms( light* l, vector<float4x4> transforms );
	void render_to_sun_rms( light* sun, u32 const cascade_index, vector<float4x4> transforms );
	void render_to_sun_rms_smoothed(
		light*				sun,
		u32 const			cascade_index,
		vector<float4x4>	transforms,
		u32 const			stage_render_index,
		u32 const			num_render_stages
	);

	void downsample_rsm(
		float3 const&	light_direction,
		float3 const&	grid_origin,
		float			grid_scale,
		u32 const		cascade_index
	);

	void downsample_gbuffer( )
	{
	}

	void inject_lighting(
		u32 const		cascade_index,
		float3 const&	light_position,
		float3 const&	light_direction,
		float			light_fov
	);

	void inject_occluders(
		u32 const			cascade_index,
		float3 const&		light_position,
		float3 const&		light_direction,
		vector<float4x4>	transforms
	);

	void propagate_lighting( u32 const cascade_index );
	void propagate_lighting_smoothed( u32 const cascade_index, u32 const propagation_iteration_index );

	void register_light_constans( );
	void register_rsm_constans( );

	void set_rsm_contants( float3 const& light_direction, float3 const& grid_origin, float grid_scale );

	void set_light_constans( light* )
	{
	}

	void render_quad( );

private:
	float3							start_render_eye_position;
	box_geometry					m_box_geometry;
	radiance_volume*				m_radiance_volume;
	u32								m_num_cascades;
	u32								m_rsm_source_size;
	u32								m_rsm_downsampled_size;
	u32								m_grid_size;
	bool							m_has_indirect_lighting;
	render_target_ptr				m_rt_downsampled_scene;
	res_texture_ptr					m_t_downsampled_scene;
	float4x4						m_previous_view_matrix[4];
	float4x4						m_previous_proj_matrix[4];
	vector<lpv_render_surface>		m_caster_models[4];
	render_target_ptr				m_rms_depth_stencil_source[4];
	res_effect_ptr					m_fill_rsm_effect[15];
	res_effect_ptr					m_downsample_rsm_effect;
	res_effect_ptr					m_apply_indirect_lighting_effect;
	res_effect_ptr					m_downsample_gbuffer_effect;
	untyped_buffer_ptr				m_screen_vertex_ib;
	res_geometry_ptr				m_screen_vertex_geometry;
	shader_constant_host*			m_c_radiance_blend_factor;
	shader_constant_host*			m_c_light_type;
	shader_constant_host*			m_c_light_color;
	shader_constant_host*			m_c_light_intensity;
	shader_constant_host*			m_c_light_position;
	shader_constant_host*			m_c_light_direction;
	shader_constant_host*			m_c_light_attenuation_power;
	shader_constant_host*			m_c_light_range;
	shader_constant_host*			m_c_view_to_light_matrix;
	shader_constant_host*			m_c_diffuse_influence_factor;
	shader_constant_host*			m_c_specular_influence_factor;
	shader_constant_host*			m_c_light_spot_penumbra_half_angle_cosine;
	shader_constant_host*			m_c_light_spot_umbra_half_angle_cosine;
	shader_constant_host*			m_c_light_spot_inversed_umbra_half_angle_cosine_minus_penumbra_half_angle_cosine;
	shader_constant_host*			m_c_light_spot_falloff;
	shader_constant_host*			m_c_light_capsule_half_width;
	shader_constant_host*			m_c_light_capsule_radius;
	shader_constant_host*			m_c_light_sphere_radius;
	shader_constant_host*			m_c_lighting_model;
	shader_constant_host*			m_c_light_local_to_world;
	shader_constant_host*			m_c_near_far;
	shader_constant_host*			m_c_grid_origin;
	shader_constant_host*			m_c_grid_cell_size;
	shader_constant_host*			m_c_invert_rsm_size;
	shader_constant_host*			m_c_interreflection_contribution;
	shader_constant_host*			m_c_cascade_index;
	shader_constant_host*			m_c_num_cascades;
	shader_constant_host*			m_c_ambient_color;
	shader_constant_host*			m_c_smaller_cascade_grid_cell_size;
	shader_constant_host*			m_c_smaller_cascade_grid_size;
	shader_constant_host*			m_c_smaller_cascade_grid_origin;
	shader_constant_host*			m_c_grid_size;
	shader_constant_host*			m_c_eye_ray_corner;
};

STATIC_SIZE_ASSERT( stage_light_propagation_volumes, 0x360 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_STAGE_LIGHT_PROPAGATION_VOLUMES_H_INCLUDED
