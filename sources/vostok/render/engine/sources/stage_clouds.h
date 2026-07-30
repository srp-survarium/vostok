#ifndef VOSTOK_RENDER_ENGINE_STAGE_CLOUDS_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_STAGE_CLOUDS_H_INCLUDED

#include <vostok/math_float3.h>
#include <vostok/math_float4.h>
#include <vostok/render/core/res_effect.h>
#include <vostok/render/core/untyped_buffer.h>
#include <vostok/render/facade/cloud_key.h>
#include <vostok/tasks_task.h>

#include "cloud_interp_textures.h"
#include "render_target.h"
#include "res_geometry.h"
#include "sphere_geometry.h"
#include "stage.h"

namespace vostok {
namespace render {

class cloud_simulation;
class renderer;
class renderer_context;
class shader_constant_host;

typedef resources::resource_ptr<
	res_effect,
	resources::unmanaged_intrusive_base
> res_effect_ptr;

struct environment_temp {
	environment_temp( );
	~environment_temp( );

	void tick( float )
	{
		// STATE[STUB]
	}

	cloud_key_parameters get_interp_key( float time );
	cloud_key_parameters get_next_key( u32 index );

	float3 get_sun_direction( float )
	{
		// STATE[STUB]
		return float3( 0.0f, 0.0f, 0.0f );
	}

	cloud_key_parameters*	keys;
	float					key_time_step;
	u32						num_keys;
	float4					sky_light[5];
	float					time;
};

STATIC_SIZE_ASSERT( environment_temp, 0x60 );

class stage_clouds : public stage {
public:
	stage_clouds(
		renderer*				in_renderer,
		cloud_interp_textures&	interp_textures,
		cloud_simulation&		simulation,
		renderer_context*		context
	) :
		stage						( in_renderer, context ),
		m_evaluate_geometry			( 0, 0 ),
		m_camera_offset				( 0.0f ),
		m_clouds_size_x				( 0 ),
		m_clouds_size_y				( 0 ),
		m_clouds_size_z				( 0 ),
		m_clouds_scale_multiplier	( 0.0f ),
		m_fixed_time				( 0.0f ),
		m_need_generate				( false ),
		m_tasks_type				( 0 ),
		m_first_tick				( true ),
		m_simulation				( simulation ),
		m_interp_textures			( interp_textures ),
		m_x_rotation				( 0.0f ),
		m_y_rotation				( 0.0f )
	{
		// STATE[STUB]
	}

	virtual ~stage_clouds( );

	virtual void execute( ) override;

	bool is_effects_ready( ) const;

private:
	struct voxel {
		u8 x;
		u8 y;
		u8 z;
		u8 w;
	};

	STATIC_SIZE_ASSERT( voxel, 0x4 );

	float evaluate_noise( float, float, u32 )
	{
		// STATE[STUB]
		return 0.0f;
	}

	void fill_surface( render_target_ptr )
	{
		// STATE[STUB]
	}

	void generate_cloud_task( u32 )
	{
		// STATE[STUB]
	}

	void fill_cloud_texture( u32 index );

	res_texture_ptr			m_3d_clouds_density_texture[2];
	res_texture_ptr			m_3d_clouds_density_texture_left;
	res_texture_ptr			m_3d_clouds_density_texture_right;
	res_effect_ptr			m_clouds_effect;
	res_effect_ptr			m_read_cloud_base_effect;
	res_effect_ptr			m_god_rays_effect;
	res_effect_ptr			m_ss_god_rays_effect;
	sphere_geometry			m_evaluate_geometry;
	environment_temp		m_environment;
	untyped_buffer_ptr		m_screen_vertex_ib;
	res_geometry_ptr		m_screen_vertex_geometry;
	shader_constant_host*	m_c_sphere_to_sky_matrix;
	shader_constant_host*	m_c_clouds_grid_size;
	shader_constant_host*	m_c_cloud_movement;
	shader_constant_host*	m_c_clouds_offset;
	shader_constant_host*	m_c_layer_height;
	shader_constant_host*	m_c_interp_alpha;
	shader_constant_host*	m_c_cloud_base;
	shader_constant_host*	m_c_light_multiplier_parameters;
	shader_constant_host*	m_c_inverted_view_projection_matrix;
	shader_constant_host*	m_to_sun_direction_parameter;
	float3					m_clouds_scale;
	float3					m_previous_view_position;
	float					m_camera_offset;
	u32 const				m_clouds_size_x;
	u32 const				m_clouds_size_y;
	u32 const				m_clouds_size_z;
	float					m_clouds_scale_multiplier;
	float3					m_wind_offset;
	float3					m_wind_direction;
	float					m_fixed_time;
	bool					m_need_generate;
	tasks::task_type*		m_tasks_type;
	tasks::task				m_parent_task;
	bool					m_first_tick;
	cloud_simulation&		m_simulation;
	cloud_interp_textures&	m_interp_textures;
	float					m_x_rotation;
	float					m_y_rotation;
	float3					m_prev_right_vector;
	float3					m_prev_up_vector;
};

STATIC_SIZE_ASSERT( stage_clouds, 0x1B8 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_STAGE_CLOUDS_H_INCLUDED
