////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STAGE_VOLUME_FOG_H_INCLUDED
#define RENDER_STAGE_VOLUME_FOG_H_INCLUDED

/* INCLUDES */
class vostok::render::shader_constant_host;
class vostok::render::stage;
struct vostok::render::fog_box_geometry;
struct vostok::render::sphere_geometry;
typedef vostok::resources::resource_ptr<vostok::render::res_effect,vostok::resources::unmanaged_intrusive_base>
	vostok::render::res_effect_ptr;
class vostok::render::res_effect;

/* FORWARD REFS */
class vostok::render::renderer;
class vostok::render::renderer_context;

namespace vostok {
namespace render {

class stage_volume_fog : public stage {
public:
					stage_volume_fog	( renderer* in_renderer, renderer_context* in_context );
	virtual			~stage_volume_fog	( ) { /* no source */ }

	virtual	void	execute				( ) override;

private:
			bool	is_effects_ready	( ) const;

	/* 0x0000 */	/* stage */
	/* 0x0010 */	fog_box_geometry			m_fog_box_geometry;
	/* 0x0014 */	sphere_geometry				m_fog_sphere_geometry;
	/* 0x0028 */	res_effect_ptr				m_exponential_volume_fog_effect;
	/* 0x002c */	res_effect_ptr				m_simple_fog_effect;
	/* 0x0030 */	shader_constant_host*		m_eye_ray_corner_parameter;
	/* 0x0034 */	shader_constant_host*		m_inverted_world_matrix_parameter;
	/* 0x0038 */	shader_constant_host*		m_eye_pos_os_parameter;
	/* 0x003c */	shader_constant_host*		m_eye_pos_ws_parameter;
	/* 0x0040 */	shader_constant_host*		m_is_inside_volume_parameter;
	/* 0x0044 */	shader_constant_host*		m_fog_parameters0;
	/* 0x0048 */	shader_constant_host*		m_fog_parameters1;
	/* 0x004c */	shader_constant_host*		m_fog_parameters2;
	/* 0x0050 */	shader_constant_host*		m_fog_parameters3;
	/* 0x0054 */	shader_constant_host*		m_far_fog_color_and_distance;
	/* 0x0058 */	shader_constant_host*		m_near_fog_distance;
	/* 0x005c */	shader_constant_host*		m_fog_alpha;
}; // class stage_volume_fog

STATIC_SIZE_ASSERT(stage_volume_fog, 0x60);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STAGE_VOLUME_FOG_H_INCLUDED
