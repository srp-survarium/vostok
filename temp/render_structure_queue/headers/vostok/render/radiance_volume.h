////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RADIANCE_VOLUME_H_INCLUDED
#define RENDER_RADIANCE_VOLUME_H_INCLUDED

/* INCLUDES */
struct D3D11_VIEWPORT;
class vostok::math::aabb;
class vostok::render::shader_constant_host;
struct vostok::render::box_geometry;
struct vostok::render::injection_geometry;
struct vostok::render::sliced_cube_geometry;
typedef vostok::intrusive_ptr<vostok::render::render_target,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::render_target_ptr;
typedef vostok::intrusive_ptr<vostok::render::res_texture,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_texture_ptr;
typedef vostok::resources::resource_ptr<vostok::render::res_effect,vostok::resources::unmanaged_intrusive_base>
	vostok::render::res_effect_ptr;
class vostok::render::render_target;
class vostok::render::res_effect;
class vostok::render::res_texture;

/* FORWARD REFS */
class vostok::render::renderer_context;
class vostok::render::vector<vostok::math::float4x4>;

namespace vostok {
namespace render {

struct radiance_volume : public boost::noncopyable {
										radiance_volume					(
											const u32		in_rsm_size,
											const u32		in_num_cells,
											const u32		in_num_propagate_iterations,
											const float		in_scale,
											const float		in_flux_amplifier
										);
										~radiance_volume				( );

			void						set_origin						( float3 const& in_origin );
			float3 const&				get_origin						( ) const;
			float3 const&				get_previous_origin				( ) const;
	inline	float3 const&				get_prev_previous_origin		( ) const { /* no source */ }
			float						get_scale						( ) const;
			u32							get_num_cells					( ) const;
	inline	res_texture_ptr const&		get_radiance_r_texture			( ) const { /* no source */ }
	inline	res_texture_ptr const&		get_radiance_g_texture			( ) const { /* no source */ }
	inline	res_texture_ptr const&		get_radiance_b_texture			( ) const { /* no source */ }
	inline	res_texture_ptr const&		get_occluders_texture			( ) const { /* no source */ }
	inline	res_texture_ptr const&		get_previous_radiance_r_texture	( ) const { /* no source */ }
	inline	res_texture_ptr const&		get_previous_radiance_g_texture	( ) const { /* no source */ }
	inline	res_texture_ptr const&		get_previous_radiance_b_texture	( ) const { /* no source */ }
	inline	res_texture_ptr const&		get_accumulated_propagation_r	( ) const { /* no source */ }
	inline	res_texture_ptr const&		get_accumulated_propagation_g	( ) const { /* no source */ }
	inline	res_texture_ptr const&		get_accumulated_propagation_b	( ) const { /* no source */ }

			void						inject_lighting					(
											float3 const&		light_position,
											float3 const&		light_direction,
											float				light_fov,
											u32					rsm_size
										);
			void						inject_occluders				(
											renderer_context*		context,
											float3 const&			light_position,
											float3 const&			light_direction,
											u32						rsm_size
										);
			void						inject_occluder_geometry		(
											renderer_context*				context,
											float3 const&					light_position,
											float3 const&					light_direction,
											vector< float4x4 > const&		transforms
										);
			void						inject_camera_occluders			( renderer_context* context );

			void						propagate_lighting				( const u32 cascade_index );
			void						propagate_lighting_iter			( const u32 cascade_index, const u32 iteration_index );

			void						prepare_final					( );

			bool						is_effects_ready				( ) const;

			void						prepare							(
											float3 const&		view_position,
											float3 const&		view_direction,
											float				offset_from_center
										);

			void						prepare_gv						( );

			void						fill_previous_result			( );

private:
			void						begin_render_to_cells			( );
			void						end_render_to_cells				( );

	/* 0x0000 */	/* boost::noncopyable */
public:
	/* 0x0000 */	render_target_ptr			m_rt_rms_albedo_source;
	/* 0x0004 */	res_texture_ptr				m_t_rms_albedo_source;
	/* 0x0008 */	render_target_ptr			m_rt_rms_normal_source;
	/* 0x000c */	res_texture_ptr				m_t_rms_normal_source;
	/* 0x0010 */	render_target_ptr			m_rt_rms_position_source;
	/* 0x0014 */	res_texture_ptr				m_t_rms_position_source;
	/* 0x0018 */	render_target_ptr			m_rt_rms_albedo_source_temp;
	/* 0x001c */	res_texture_ptr				m_t_rms_albedo_source_temp;
	/* 0x0020 */	render_target_ptr			m_rt_rms_normal_source_temp;
	/* 0x0024 */	res_texture_ptr				m_t_rms_normal_source_temp;
	/* 0x0028 */	render_target_ptr			m_rt_rms_position_source_temp;
	/* 0x002c */	res_texture_ptr				m_t_rms_position_source_temp;
	/* 0x0030 */	render_target_ptr			m_rt_rms_albedo;
	/* 0x0034 */	res_texture_ptr				m_t_rms_albedo;
	/* 0x0038 */	render_target_ptr			m_rt_rms_normal;
	/* 0x003c */	res_texture_ptr				m_t_rms_normal;
	/* 0x0040 */	render_target_ptr			m_rt_rms_position;
	/* 0x0044 */	res_texture_ptr				m_t_rms_position;
	/* 0x0048 */	u32							m_rsm_downsampled_size;
	/* 0x004c */	u32							m_rsm_source_size;
private:
	/* 0x0050 */	box_geometry				m_box_geometry;
	/* 0x0060 */	D3D11_VIEWPORT				m_saved_viewport;
	/* 0x0078 */	injection_geometry			m_injection_geometry;
	/* 0x0090 */	injection_geometry			m_injection_geometry_from_camera;
	/* 0x00a8 */	sliced_cube_geometry		m_sliced_cube_geometry;
	/* 0x00bc */	float						m_cell_size;
	/* 0x00c0 */	float						m_scale;
	/* 0x00c4 */	float						m_flux_amplifier;
	/* 0x00c8 */	const u32					m_num_cells;
	/* 0x00cc */	math::aabb					m_bbox;
	/* 0x00e4 */	float3						m_previous_origin;
	/* 0x00f0 */	float3						m_prev_previous_origin;
	/* 0x00fc */	float3						m_next_origin;
	/* 0x0108 */	const u32					m_num_propagate_iterations;
	/* 0x010c */	render_target_ptr			m_3d_rt_previous_radiance_r;
	/* 0x0110 */	render_target_ptr			m_3d_rt_previous_radiance_g;
	/* 0x0114 */	render_target_ptr			m_3d_rt_previous_radiance_b;
	/* 0x0118 */	res_texture_ptr				m_3d_t_previous_radiance_r;
	/* 0x011c */	res_texture_ptr				m_3d_t_previous_radiance_g;
	/* 0x0120 */	res_texture_ptr				m_3d_t_previous_radiance_b;
	/* 0x0124 */	render_target_ptr			m_3d_rt_radiance_r;
	/* 0x0128 */	render_target_ptr			m_3d_rt_radiance_g;
	/* 0x012c */	render_target_ptr			m_3d_rt_radiance_b;
	/* 0x0130 */	res_texture_ptr				m_3d_t_radiance_r;
	/* 0x0134 */	res_texture_ptr				m_3d_t_radiance_g;
	/* 0x0138 */	res_texture_ptr				m_3d_t_radiance_b;
	/* 0x013c */	render_target_ptr			m_3d_rt_radiance_r_apply;
	/* 0x0140 */	render_target_ptr			m_3d_rt_radiance_g_apply;
	/* 0x0144 */	render_target_ptr			m_3d_rt_radiance_b_apply;
	/* 0x0148 */	res_texture_ptr				m_3d_t_radiance_r_apply;
	/* 0x014c */	res_texture_ptr				m_3d_t_radiance_g_apply;
	/* 0x0150 */	res_texture_ptr				m_3d_t_radiance_b_apply;
	/* 0x0154 */	render_target_ptr			m_3d_rt_radiance_intermediate_r;
	/* 0x0158 */	render_target_ptr			m_3d_rt_radiance_intermediate_g;
	/* 0x015c */	render_target_ptr			m_3d_rt_radiance_intermediate_b;
	/* 0x0160 */	res_texture_ptr				m_3d_t_radiance_intermediate_r;
	/* 0x0164 */	res_texture_ptr				m_3d_t_radiance_intermediate_g;
	/* 0x0168 */	res_texture_ptr				m_3d_t_radiance_intermediate_b;
	/* 0x016c */	render_target_ptr			m_3d_rt_accumulated_propagation_r;
	/* 0x0170 */	render_target_ptr			m_3d_rt_accumulated_propagation_g;
	/* 0x0174 */	render_target_ptr			m_3d_rt_accumulated_propagation_b;
	/* 0x0178 */	res_texture_ptr				m_3d_t_accumulated_propagation_r;
	/* 0x017c */	res_texture_ptr				m_3d_t_accumulated_propagation_g;
	/* 0x0180 */	res_texture_ptr				m_3d_t_accumulated_propagation_b;
	/* 0x0184 */	render_target_ptr			m_3d_rt_occluders;
	/* 0x0188 */	res_texture_ptr				m_3d_t_occluders;
	/* 0x018c */	render_target_ptr			m_radiance_depth_stencil;
	/* 0x0190 */	res_effect_ptr				m_lpv_effect;
	/* 0x0194 */	shader_constant_host*		m_c_rsm_size;
	/* 0x0198 */	shader_constant_host*		m_c_grid_size;
	/* 0x019c */	shader_constant_host*		m_c_grid_origin;
	/* 0x01a0 */	shader_constant_host*		m_c_grid_origin_and_inv_grid_scale;
	/* 0x01a4 */	shader_constant_host*		m_c_grid_cell_size;
	/* 0x01a8 */	shader_constant_host*		m_c_num_grid_cells;
	/* 0x01ac */	shader_constant_host*		m_c_light_direction;
	/* 0x01b0 */	shader_constant_host*		m_c_light_position;
	/* 0x01b4 */	shader_constant_host*		m_c_propagate_iteration_index;
	/* 0x01b8 */	shader_constant_host*		m_c_inject_flux_weight;
	/* 0x01bc */	shader_constant_host*		m_c_flux_amplifier;
	/* 0x01c0 */	shader_constant_host*		m_c_eye_ray_corner;
	/* 0x01c4 */	shader_constant_host*		m_c_cascade_index;
	/* 0x01c8 */	shader_constant_host*		m_c_occlusion_amplifier;
	/* 0x01cc */	bool						m_is_position_changed;
	/* 0x01d0 */	float3						m_prev_position;
}; // struct radiance_volume

STATIC_SIZE_ASSERT(radiance_volume, 0x1DC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RADIANCE_VOLUME_H_INCLUDED
