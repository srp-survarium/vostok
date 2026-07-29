////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STAGE_SUN_H_INCLUDED
#define RENDER_STAGE_SUN_H_INCLUDED

/* INCLUDES */
class vostok::render::cloud_simulation;
class vostok::render::shader_constant_host;
class vostok::render::stage;
struct vostok::render::box_geometry;
struct vostok::render::cloud_interp_textures;
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

class stage_sun : public stage {
public:
					stage_sun		(
						renderer*					in_renderer,
						renderer_context*			in_context,
						cloud_interp_textures&		in_cloud_interp_textures,
						cloud_simulation&			in_simulation
					);

	virtual	void	execute			( ) override;

private:
			bool	is_effects_ready( ) const;

public:
	virtual			~stage_sun		( ) { /* no source */ }

	/* 0x0000 */	/* stage */
private:
	/* 0x0010 */	res_effect_ptr				m_sun_effect;
	/* 0x0014 */	res_texture_ptr				m_shadow_jitter;
	/* 0x0018 */	box_geometry				m_box_geometry;
	/* 0x0028 */	shader_constant_host*		m_c_light_direction;
	/* 0x002c */	shader_constant_host*		m_c_light_color;
	/* 0x0030 */	shader_constant_host*		m_c_shadow_transparency;
	/* 0x0034 */	shader_constant_host*		m_c_light_intensity;
	/* 0x0038 */	shader_constant_host*		m_shadow[4];
	/* 0x0048 */	shader_constant_host*		m_c_diffuse_influence_factor;
	/* 0x004c */	shader_constant_host*		m_c_specular_influence_factor;
	/* 0x0050 */	shader_constant_host*		m_c_environment_skylight_upper_color;
	/* 0x0054 */	shader_constant_host*		m_c_environment_skylight_lower_color;
	/* 0x0058 */	shader_constant_host*		m_c_clouds_offset;
	/* 0x005c */	shader_constant_host*		m_c_world_to_cloud;
	/* 0x0060 */	shader_constant_host*		m_c_cloud_interp_alpha;
	/* 0x0064 */	shader_constant_host*		m_c_inverted_view_projection_matrix;
	/* 0x0068 */	shader_constant_host*		m_c_sun_fixed_matrix;
	/* 0x006c */	shader_constant_host*		m_c_eye_ray_corner;
	/* 0x0070 */	cloud_interp_textures&		m_cloud_interp_textures;
	/* 0x0074 */	cloud_simulation&			m_simulation;
}; // class stage_sun

STATIC_SIZE_ASSERT(stage_sun, 0x78);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STAGE_SUN_H_INCLUDED
