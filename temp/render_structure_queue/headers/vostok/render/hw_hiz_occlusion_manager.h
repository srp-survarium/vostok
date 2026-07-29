////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_HW_HIZ_OCCLUSION_MANAGER_H_INCLUDED
#define RENDER_HW_HIZ_OCCLUSION_MANAGER_H_INCLUDED

/* INCLUDES */
class vostok::render::shader_constant_host;
struct vostok::render::hw_hiz_point_list;
struct vostok::render::sphere_occluder_geometry;
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

namespace vostok {
namespace render {

struct hw_hiz_occlusion_manager : public boost::noncopyable {
	inline			hw_hiz_occlusion_manager		( const bool arg_0, const u32 arg_1, const u32 arg_2 ) { /* no source */ }
					~hw_hiz_occlusion_manager		( );

			void	process_culling					(
						renderer_context*		in_context,
						float4 const*			in_bounds,
						const u32				in_num_bounds_and_results
					);

			bool	is_ready_effects				( ) const;

			void	render_debug					(
						renderer_context*		in_context,
						float4 const*			in_bounds,
						pcbyte					in_results,
						const u32				in_num_bounds_and_results
					);

			bool	quary_and_get_results_if_ready	( pbyte out_results, const u32 in_num_results );

private:
			void	render_occluders				( renderer_context* in_context );

			void	copy_scene_depth				( );

			void	downsample_occlusion_buffer		( );

			void	render_model_bounds				( renderer_context* in_context, float4 const* in_bounds, const u32 in_num_bounds );

			void	check_culling_buffer			( const u32 in_num_bounds );

	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	const bool					m_use_scene_depth_buffer;
	/* 0x0004 */	res_effect_ptr				m_hiz_occlusion_effect;
	/* 0x0008 */	render_target_ptr			m_rt_depth_mips_work[16];
	/* 0x0048 */	res_texture_ptr				m_t_depth_mips_work[16];
	/* 0x0088 */	render_target_ptr			m_rt_depth_mips[16];
	/* 0x00c8 */	render_target_ptr			m_ds_occlusion_depth;
	/* 0x00cc */	res_texture_ptr				m_t_depth_mips;
	/* 0x00d0 */	res_texture_ptr				m_t_depth_mips_copy;
	/* 0x00d4 */	res_texture_ptr				m_t_depth_mips_lockable;
	/* 0x00d8 */	const u32					m_rasterize_width;
	/* 0x00dc */	const u32					m_rasterize_height;
	/* 0x00e0 */	const u32					m_num_mips;
	/* 0x00e4 */	sphere_occluder_geometry	m_sphere_occluder_geometry;
	/* 0x00e8 */	shader_constant_host*		m_source_mip_level_parameter;
	/* 0x00ec */	shader_constant_host*		m_draw_color_parameter;
	/* 0x00f0 */	shader_constant_host*		m_render_target_size_parameter;
	/* 0x00f4 */	shader_constant_host*		m_rasterize_size_parameter;
	/* 0x00f8 */	shader_constant_host*		m_prev_texture_size_parameter;
	/* 0x00fc */	u32							m_current_num_bounds;
	/* 0x0100 */	u32							m_culling_buffer_width;
	/* 0x0104 */	u32							m_culling_buffer_height;
	/* 0x0108 */	render_target_ptr			m_rt_culling_result;
	/* 0x010c */	res_texture_ptr				m_t_culling_result;
	/* 0x0110 */	res_texture_ptr				m_t_culling_result_lockable;
	/* 0x0114 */	hw_hiz_point_list			m_hw_hiz_point_list;
}; // struct hw_hiz_occlusion_manager

STATIC_SIZE_ASSERT(hw_hiz_occlusion_manager, 0x120);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_HW_HIZ_OCCLUSION_MANAGER_H_INCLUDED
