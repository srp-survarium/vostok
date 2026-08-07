#ifndef VOSTOK_RENDER_ENGINE_RADIANCE_VOLUME_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_RADIANCE_VOLUME_H_INCLUDED

#include <boost/noncopyable.hpp>
#include <d3d11.h>
#include <vostok/math_aabb.h>
#include <vostok/render/core/dx11/res_declaration.h>
#include <vostok/render/core/memory.h>
#include <vostok/render/core/res_effect.h>
#include <vostok/render/core/untyped_buffer.h>

#include "box_geometry.h"
#include "render_target.h"
#include "res_texture.h"

namespace vostok {
namespace render {

class renderer_context;
class shader_constant_host;

typedef intrusive_ptr<
	res_texture,
	resource_intrusive_base,
	threading::single_threading_policy
> res_texture_ptr;

typedef resources::resource_ptr<
	res_effect,
	resources::unmanaged_intrusive_base
> res_effect_ptr;

struct injection_geometry : public boost::noncopyable {
	explicit injection_geometry( u32 rsm_size );
	injection_geometry( u32 rsm_size_x, u32 rsm_size_y );
	~injection_geometry( ) { }

	void draw( );
	void prepare( u32 rsm_size_x, u32 rsm_size_y );

private:
	res_declaration_ptr	m_vertext_declaration;
	untyped_buffer_ptr	m_vertex_buffer;
	u32					m_num_points;
	u32 const			m_stride;
	u32					m_rsm_size_x;
	u32					m_rsm_size_y;
};

STATIC_SIZE_ASSERT( injection_geometry, 0x18 );

struct sliced_cube_geometry : public boost::noncopyable {
	struct sliced_cube_geometry_vertex {
		sliced_cube_geometry_vertex( ) { }

		float4	position;
		float4	xy_and_slice_index;
	};

	explicit sliced_cube_geometry( u32 in_num_cells );
	~sliced_cube_geometry( ) { }

	void draw( );

private:
	res_declaration_ptr	m_vertext_declaration;
	untyped_buffer_ptr	m_vertex_buffer;
	untyped_buffer_ptr	m_index_buffer;
	u32 const			m_slices;
	u32					m_stride;
};

STATIC_SIZE_ASSERT( sliced_cube_geometry::sliced_cube_geometry_vertex, 0x20 );
STATIC_SIZE_ASSERT( sliced_cube_geometry, 0x14 );

struct radiance_volume : public boost::noncopyable {
	radiance_volume(
		u32 const	in_rsm_size,
		u32 const	in_num_cells,
		u32 const	in_num_propagate_iterations,
		float const	in_scale,
		float const	in_flux_amplifier
	);

	~radiance_volume( );

	void set_origin( float3 const& in_origin );
	float3 const& get_origin( ) const;
	float3 const& get_previous_origin( ) const;

	float3 const& get_prev_previous_origin( ) const
	{
		return m_prev_previous_origin;
	}

	float get_scale( ) const;
	u32 get_num_cells( ) const;

	res_texture_ptr const& get_radiance_r_texture( ) const
	{
		return m_3d_t_radiance_r;
	}

	res_texture_ptr const& get_radiance_g_texture( ) const
	{
		return m_3d_t_radiance_g;
	}

	res_texture_ptr const& get_radiance_b_texture( ) const
	{
		return m_3d_t_radiance_b;
	}

	res_texture_ptr const& get_occluders_texture( ) const
	{
		return m_3d_t_occluders;
	}

	res_texture_ptr const& get_previous_radiance_r_texture( ) const
	{
		return m_3d_t_previous_radiance_r;
	}

	res_texture_ptr const& get_previous_radiance_g_texture( ) const
	{
		return m_3d_t_previous_radiance_g;
	}

	res_texture_ptr const& get_previous_radiance_b_texture( ) const
	{
		return m_3d_t_previous_radiance_b;
	}

	res_texture_ptr const& get_accumulated_propagation_r( ) const
	{
		return m_3d_t_accumulated_propagation_r;
	}

	res_texture_ptr const& get_accumulated_propagation_g( ) const
	{
		return m_3d_t_accumulated_propagation_g;
	}

	res_texture_ptr const& get_accumulated_propagation_b( ) const
	{
		return m_3d_t_accumulated_propagation_b;
	}

	void inject_lighting(
		float3 const&	light_position,
		float3 const&	light_direction,
		float			light_fov,
		u32				rsm_size
	);

	void inject_occluders(
		renderer_context*	context,
		float3 const&		light_position,
		float3 const&		light_direction,
		u32					rsm_size
	);

	void inject_occluder_geometry(
		renderer_context*		context,
		float3 const&			light_position,
		float3 const&			light_direction,
		vector<float4x4> const&	transforms
	);

	void inject_camera_occluders( renderer_context* context );
	void propagate_lighting( u32 const cascade_index );
	void propagate_lighting_iter( u32 const cascade_index, u32 const iteration_index );
	void prepare_final( );
	bool is_effects_ready( ) const;

	void prepare(
		float3 const&	view_position,
		float3 const&	view_direction,
		float			offset_from_center
	);

	void prepare_gv( );
	void fill_previous_result( );

private:
	void begin_render_to_cells( );
	void end_render_to_cells( );

public:
	render_target_ptr	m_rt_rms_albedo_source;
	res_texture_ptr		m_t_rms_albedo_source;
	render_target_ptr	m_rt_rms_normal_source;
	res_texture_ptr		m_t_rms_normal_source;
	render_target_ptr	m_rt_rms_position_source;
	res_texture_ptr		m_t_rms_position_source;
	render_target_ptr	m_rt_rms_albedo_source_temp;
	res_texture_ptr		m_t_rms_albedo_source_temp;
	render_target_ptr	m_rt_rms_normal_source_temp;
	res_texture_ptr		m_t_rms_normal_source_temp;
	render_target_ptr	m_rt_rms_position_source_temp;
	res_texture_ptr		m_t_rms_position_source_temp;
	render_target_ptr	m_rt_rms_albedo;
	res_texture_ptr		m_t_rms_albedo;
	render_target_ptr	m_rt_rms_normal;
	res_texture_ptr		m_t_rms_normal;
	render_target_ptr	m_rt_rms_position;
	res_texture_ptr		m_t_rms_position;
	u32					m_rsm_downsampled_size;
	u32					m_rsm_source_size;

private:
	box_geometry		m_box_geometry;
	D3D11_VIEWPORT		m_saved_viewport;
	injection_geometry	m_injection_geometry;
	injection_geometry	m_injection_geometry_from_camera;
	sliced_cube_geometry	m_sliced_cube_geometry;
	float				m_cell_size;
	float				m_scale;
	float				m_flux_amplifier;
	u32 const			m_num_cells;
	math::aabb			m_bbox;
	float3				m_previous_origin;
	float3				m_prev_previous_origin;
	float3				m_next_origin;
	u32 const			m_num_propagate_iterations;
	render_target_ptr	m_3d_rt_previous_radiance_r;
	render_target_ptr	m_3d_rt_previous_radiance_g;
	render_target_ptr	m_3d_rt_previous_radiance_b;
	res_texture_ptr		m_3d_t_previous_radiance_r;
	res_texture_ptr		m_3d_t_previous_radiance_g;
	res_texture_ptr		m_3d_t_previous_radiance_b;
	render_target_ptr	m_3d_rt_radiance_r;
	render_target_ptr	m_3d_rt_radiance_g;
	render_target_ptr	m_3d_rt_radiance_b;
	res_texture_ptr		m_3d_t_radiance_r;
	res_texture_ptr		m_3d_t_radiance_g;
	res_texture_ptr		m_3d_t_radiance_b;
	render_target_ptr	m_3d_rt_radiance_r_apply;
	render_target_ptr	m_3d_rt_radiance_g_apply;
	render_target_ptr	m_3d_rt_radiance_b_apply;
	res_texture_ptr		m_3d_t_radiance_r_apply;
	res_texture_ptr		m_3d_t_radiance_g_apply;
	res_texture_ptr		m_3d_t_radiance_b_apply;
	render_target_ptr	m_3d_rt_radiance_intermediate_r;
	render_target_ptr	m_3d_rt_radiance_intermediate_g;
	render_target_ptr	m_3d_rt_radiance_intermediate_b;
	res_texture_ptr		m_3d_t_radiance_intermediate_r;
	res_texture_ptr		m_3d_t_radiance_intermediate_g;
	res_texture_ptr		m_3d_t_radiance_intermediate_b;
	render_target_ptr	m_3d_rt_accumulated_propagation_r;
	render_target_ptr	m_3d_rt_accumulated_propagation_g;
	render_target_ptr	m_3d_rt_accumulated_propagation_b;
	res_texture_ptr		m_3d_t_accumulated_propagation_r;
	res_texture_ptr		m_3d_t_accumulated_propagation_g;
	res_texture_ptr		m_3d_t_accumulated_propagation_b;
	render_target_ptr	m_3d_rt_occluders;
	res_texture_ptr		m_3d_t_occluders;
	render_target_ptr	m_radiance_depth_stencil;
	res_effect_ptr		m_lpv_effect;
	shader_constant_host*	m_c_rsm_size;
	shader_constant_host*	m_c_grid_size;
	shader_constant_host*	m_c_grid_origin;
	shader_constant_host*	m_c_grid_origin_and_inv_grid_scale;
	shader_constant_host*	m_c_grid_cell_size;
	shader_constant_host*	m_c_num_grid_cells;
	shader_constant_host*	m_c_light_direction;
	shader_constant_host*	m_c_light_position;
	shader_constant_host*	m_c_propagate_iteration_index;
	shader_constant_host*	m_c_inject_flux_weight;
	shader_constant_host*	m_c_flux_amplifier;
	shader_constant_host*	m_c_eye_ray_corner;
	shader_constant_host*	m_c_cascade_index;
	shader_constant_host*	m_c_occlusion_amplifier;
	bool					m_is_position_changed;
	float3					m_prev_position;
};

STATIC_SIZE_ASSERT( radiance_volume, 0x1DC );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_RADIANCE_VOLUME_H_INCLUDED
