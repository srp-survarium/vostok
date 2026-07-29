////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STAGE_RAIN_H_INCLUDED
#define RENDER_STAGE_RAIN_H_INCLUDED

/* INCLUDES */
class vostok::render::shader_constant_host;
class vostok::render::stage;
struct vostok::render::sphere_geometry;
typedef vostok::intrusive_ptr<vostok::render::res_texture,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_texture_ptr;
typedef vostok::resources::resource_ptr<vostok::render::res_effect,vostok::resources::unmanaged_intrusive_base>
	vostok::render::res_effect_ptr;
class vostok::render::res_effect;
class vostok::render::res_texture;

/* FORWARD REFS */
class vostok::render::renderer;
class vostok::render::renderer_context;

namespace vostok {
namespace render {

class stage_rain : public stage {
public:
					stage_rain		( renderer* in_renderer, renderer_context* context );
	virtual			~stage_rain		( );

	virtual	void	execute			( ) override;

private:
			bool	is_effects_ready( ) const;

	/* 0x0000 */	/* stage */
	/* 0x0010 */	res_texture_ptr				m_t_rain_shadow_map;
	/* 0x0014 */	res_effect_ptr				m_rain_effect;
	/* 0x0018 */	res_effect_ptr				m_effect_shadow_direct;
	/* 0x001c */	sphere_geometry				m_rain_geometry;
	/* 0x0030 */	float2*						m_rain_offsets;
	/* 0x0034 */	float						m_camera_offset_view;
	/* 0x0038 */	float						m_camera_offset_right;
	/* 0x003c */	float						m_rain_rotation_x[100];
	/* 0x01cc */	float						m_rain_rotation_y[100];
	/* 0x035c */	u32							m_shadow_map_size;
	/* 0x0360 */	float3						m_previous_view_position;
	/* 0x036c */	float3						m_moving_direction;
	/* 0x0378 */	shader_constant_host*		m_radius_parameter;
	/* 0x037c */	shader_constant_host*		m_rain_speed_parameter;
	/* 0x0380 */	shader_constant_host*		m_rain_density_parameter;
	/* 0x0384 */	shader_constant_host*		m_rain_uv_scales_parameter;
	/* 0x0388 */	shader_constant_host*		m_view_to_shadow_parameter;
}; // class stage_rain

STATIC_SIZE_ASSERT(stage_rain, 0x38C);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STAGE_RAIN_H_INCLUDED
