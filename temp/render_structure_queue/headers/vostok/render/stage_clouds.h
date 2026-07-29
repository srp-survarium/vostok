////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STAGE_CLOUDS_H_INCLUDED
#define RENDER_STAGE_CLOUDS_H_INCLUDED

/* INCLUDES */
class vostok::render::cloud_simulation;
class vostok::render::shader_constant_host;
class vostok::render::stage;
class vostok::tasks::task;
class vostok::tasks::task_type;
struct vostok::render::cloud_interp_textures;
struct vostok::render::environment_temp;
struct vostok::render::sphere_geometry;
class vostok::render::render_target;
typedef vostok::intrusive_ptr<vostok::render::res_geometry,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_geometry_ptr;
typedef vostok::intrusive_ptr<vostok::render::res_texture,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_texture_ptr;
typedef vostok::intrusive_ptr<vostok::render::untyped_buffer,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::untyped_buffer_ptr;
typedef vostok::resources::resource_ptr<vostok::render::res_effect,vostok::resources::unmanaged_intrusive_base>
	vostok::render::res_effect_ptr;
class vostok::render::res_effect;
class vostok::render::res_geometry;
class vostok::render::res_texture;
class vostok::render::untyped_buffer;

/* FORWARD REFS */
class vostok::render::renderer;
class vostok::render::renderer_context;

namespace vostok {
namespace render {

class stage_clouds : public stage {
public:
	inline				stage_clouds		(
							renderer*					arg_0,
							cloud_interp_textures&		arg_1,
							cloud_simulation&			arg_2,
							renderer_context*			arg_3
						) { /* no source */ }
	virtual				~stage_clouds		( );

	virtual	void		execute				( ) override;

			bool		is_effects_ready	( ) const;

private:
	inline	float		evaluate_noise		( float arg_0, float arg_1, u32 arg_2 ) { /* no source */ }

	inline	void		fill_surface		( render_target_ptr arg_0 ) { /* no source */ }

	inline	void		generate_cloud_task	( u32 arg_0 ) { /* no source */ }

			void		fill_cloud_texture	( u32 index );

	/* 0x0000 */	/* stage */
	/* 0x0010 */	res_texture_ptr				m_3d_clouds_density_texture[2];
	/* 0x0018 */	res_texture_ptr				m_3d_clouds_density_texture_left;
	/* 0x001c */	res_texture_ptr				m_3d_clouds_density_texture_right;
	/* 0x0020 */	res_effect_ptr				m_clouds_effect;
	/* 0x0024 */	res_effect_ptr				m_read_cloud_base_effect;
	/* 0x0028 */	res_effect_ptr				m_god_rays_effect;
	/* 0x002c */	res_effect_ptr				m_ss_god_rays_effect;
	/* 0x0030 */	sphere_geometry				m_evaluate_geometry;
	/* 0x0044 */	environment_temp			m_environment;
	/* 0x00a4 */	untyped_buffer_ptr			m_screen_vertex_ib;
	/* 0x00a8 */	res_geometry_ptr			m_screen_vertex_geometry;
	/* 0x00ac */	shader_constant_host*		m_c_sphere_to_sky_matrix;
	/* 0x00b0 */	shader_constant_host*		m_c_clouds_grid_size;
	/* 0x00b4 */	shader_constant_host*		m_c_cloud_movement;
	/* 0x00b8 */	shader_constant_host*		m_c_clouds_offset;
	/* 0x00bc */	shader_constant_host*		m_c_layer_height;
	/* 0x00c0 */	shader_constant_host*		m_c_interp_alpha;
	/* 0x00c4 */	shader_constant_host*		m_c_cloud_base;
	/* 0x00c8 */	shader_constant_host*		m_c_light_multiplier_parameters;
	/* 0x00cc */	shader_constant_host*		m_c_inverted_view_projection_matrix;
	/* 0x00d0 */	shader_constant_host*		m_to_sun_direction_parameter;
	/* 0x00d4 */	float3						m_clouds_scale;
	/* 0x00e0 */	float3						m_previous_view_position;
	/* 0x00ec */	float						m_camera_offset;
	/* 0x00f0 */	const u32					m_clouds_size_x;
	/* 0x00f4 */	const u32					m_clouds_size_y;
	/* 0x00f8 */	const u32					m_clouds_size_z;
	/* 0x00fc */	float						m_clouds_scale_multiplier;
	/* 0x0100 */	float3						m_wind_offset;
	/* 0x010c */	float3						m_wind_direction;
	/* 0x0118 */	float						m_fixed_time;
	/* 0x011c */	bool						m_need_generate;
	/* 0x0120 */	tasks::task_type*			m_tasks_type;
	/* 0x0128 */	tasks::task					m_parent_task;
	/* 0x0188 */	bool						m_first_tick;
	/* 0x018c */	cloud_simulation&			m_simulation;
	/* 0x0190 */	cloud_interp_textures&		m_interp_textures;
	/* 0x0194 */	float						m_x_rotation;
	/* 0x0198 */	float						m_y_rotation;
	/* 0x019c */	float3						m_prev_right_vector;
	/* 0x01a8 */	float3						m_prev_up_vector;
}; // class stage_clouds

STATIC_SIZE_ASSERT(stage_clouds, 0x1B8);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STAGE_CLOUDS_H_INCLUDED
