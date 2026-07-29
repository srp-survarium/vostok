////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STAGE_FORWARD_H_INCLUDED
#define RENDER_STAGE_FORWARD_H_INCLUDED

/* INCLUDES */
class vostok::render::shader_constant_host;
class vostok::render::stage;
class vostok::render::stage_forward::stage_type;
typedef vostok::resources::resource_ptr<vostok::render::res_effect,vostok::resources::unmanaged_intrusive_base>
	vostok::render::res_effect_ptr;
class vostok::render::res_effect;

/* FORWARD REFS */
class vostok::render::renderer;
class vostok::render::renderer_context;
class vostok::render::vector<vostok::render::render_surface_instance *>;

namespace vostok {
namespace render {

enum vostok::render::stage_forward::stage_type
{
	forward_base	= 0x0,
	forward_sky		= 0x1,
};

class stage_forward : public stage {
public:
	inline			stage_forward				( renderer* arg_0, renderer_context* arg_1, stage_forward::stage_type arg_2 ) { /* no source */ }
	virtual			~stage_forward				( );

	virtual	void	execute						( ) override;

			void	accumulate_local_reflections( );

			bool	is_effects_ready			( ) const;

			void	render_opaque_models		( );
			void	render_forward_models		( vector< render_surface_instance* >& dynamic_visuals, const u32 pass_index );

	/* 0x0000 */	/* stage */
	/* 0x0010 */	res_effect_ptr					m_debug_tracer_effect;
	/* 0x0014 */	res_effect_ptr					m_opaque_geometry_mask_effect;
	/* 0x0018 */	res_effect_ptr					m_gbuffer_depth_effect[15];
	/* 0x0054 */	shader_constant_host*			m_tree_position_and_scale_parameter;
	/* 0x0058 */	shader_constant_host*			m_tree_rotation_parameter;
	/* 0x005c */	shader_constant_host*			m_far_fog_color_and_distance;
	/* 0x0060 */	shader_constant_host*			m_fog_alpha;
	/* 0x0064 */	shader_constant_host*			m_near_fog_distance;
	/* 0x0068 */	shader_constant_host*			m_eye_ray_corner_parameter;
	/* 0x006c */	shader_constant_host*			m_view_to_shadow_parameter;
	/* 0x0070 */	shader_constant_host*			m_rain_offset_parameter;
	/* 0x0074 */	shader_constant_host*			m_use_rain_parameter;
	/* 0x0078 */	shader_constant_host*			m_tracer_debug_color_parameter;
	/* 0x007c */	shader_constant_host*			m_c_inscatter_parameters;
	/* 0x0080 */	float							m_rain_offset;
	/* 0x0084 */	float							m_rain_offset_counter;
	/* 0x0088 */	stage_forward::stage_type		m_type;
}; // class stage_forward

STATIC_SIZE_ASSERT(stage_forward, 0x8C);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STAGE_FORWARD_H_INCLUDED
