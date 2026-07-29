////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STAGE_GBUFFER_H_INCLUDED
#define RENDER_STAGE_GBUFFER_H_INCLUDED

/* INCLUDES */
class vostok::render::shader_constant_host;
class vostok::render::stage;
typedef vostok::intrusive_ptr<vostok::render::res_state,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_state_ptr;
typedef vostok::resources::resource_ptr<vostok::render::res_effect,vostok::resources::unmanaged_intrusive_base>
	vostok::render::res_effect_ptr;
class vostok::render::res_effect;
class vostok::render::res_state;

/* FORWARD REFS */
class vostok::render::renderer;
class vostok::render::renderer_context;
class vostok::render::vector<vostok::render::render_surface_instance *>;

namespace vostok {
namespace render {

class stage_gbuffer : public stage {
public:
					stage_gbuffer	( renderer* in_renderer, renderer_context* context );
	virtual			~stage_gbuffer	( );

	virtual	void	execute			( ) override;

			bool	is_effects_ready( ) const;

private:
			void	render_models	(
						vector< render_surface_instance* >&		models,
						u32										shader_lod_index,
						u32&									out_num_rendered,
						bool									z_only
					);
			void	render_speedtree( );
			void	render_grass	( bool z_only );
			void	render_particles( bool z_only );

			void	z_only_pass		( );

	/* 0x0000 */	/* stage */
	/* 0x0010 */	res_state_ptr				m_state;
	/* 0x0014 */	shader_constant_host*		m_sun_direction;
	/* 0x0018 */	shader_constant_host*		m_c_start_corner;
	/* 0x001c */	shader_constant_host*		m_object_transparency_scale_parameter;
	/* 0x0020 */	shader_constant_host*		m_far_fog_color_and_distance;
	/* 0x0024 */	shader_constant_host*		m_c_sun_near_aabb_point;
	/* 0x0028 */	shader_constant_host*		m_c_bound_box_min;
	/* 0x002c */	shader_constant_host*		m_c_bound_box_max;
	/* 0x0030 */	shader_constant_host*		m_near_fog_distance;
	/* 0x0034 */	shader_constant_host*		m_fog_alpha;
	/* 0x0038 */	shader_constant_host*		m_ambient_color;
	/* 0x003c */	shader_constant_host*		m_c_environment_skylight_upper_color;
	/* 0x0040 */	shader_constant_host*		m_c_environment_skylight_lower_color;
	/* 0x0044 */	shader_constant_host*		m_c_environment_skylight_parameters;
	/* 0x0048 */	shader_constant_host*		m_c_gs_test_constant;
	/* 0x004c */	shader_constant_host*		m_c_sun_direction;
	/* 0x0050 */	shader_constant_host*		m_c_translucency_max_scatter;
	/* 0x0054 */	shader_constant_host*		m_c_sun_color;
	/* 0x0058 */	shader_constant_host*		m_wind_info_parameters;
	/* 0x005c */	shader_constant_host*		m_smoothness_multiplier;
	/* 0x0060 */	shader_constant_host*		m_shadow[4];
	/* 0x0070 */	res_effect_ptr				m_copy_depth_rt;
	/* 0x0074 */	res_effect_ptr				m_fill_depth_effect;
	/* 0x0078 */	bool						m_is_pre_pass;
public:
	/* 0x0079 */	bool						m_fill_view_space_depth;
}; // class stage_gbuffer

STATIC_SIZE_ASSERT(stage_gbuffer, 0x7C);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STAGE_GBUFFER_H_INCLUDED
