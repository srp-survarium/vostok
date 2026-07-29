////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STAGE_AMBIENT_LIGHTING_H_INCLUDED
#define RENDER_STAGE_AMBIENT_LIGHTING_H_INCLUDED

/* INCLUDES */
class vostok::render::shader_constant_host;
class vostok::render::stage;
typedef vostok::intrusive_ptr<vostok::render::res_geometry,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_geometry_ptr;
typedef vostok::intrusive_ptr<vostok::render::untyped_buffer,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::untyped_buffer_ptr;
typedef vostok::resources::resource_ptr<vostok::render::res_effect,vostok::resources::unmanaged_intrusive_base>
	vostok::render::res_effect_ptr;
class vostok::render::res_effect;
class vostok::render::res_geometry;
class vostok::render::untyped_buffer;

/* FORWARD REFS */
class vostok::render::renderer;
class vostok::render::renderer_context;

namespace vostok {
namespace render {

class stage_ambient_lighting : public stage {
public:
					stage_ambient_lighting	( renderer* in_renderer, renderer_context* context );

	virtual	void	execute					( ) override;

			bool	is_effects_ready		( ) const;

	inline	void	use_probes				( bool arg_0 ) { /* no source */ }

	inline	void	set_ambient_multiplier	( float arg_0 ) { /* no source */ }

	virtual			~stage_ambient_lighting	( ) { /* no source */ }

	/* 0x0000 */	/* stage */
	/* 0x0010 */	res_effect_ptr				m_effect_accum_mask;
	/* 0x0014 */	res_effect_ptr				m_environment_probe_lighting_effect[2][2][2];
	/* 0x0034 */	res_effect_ptr				m_skylight_effect;
	/* 0x0038 */	res_effect_ptr				m_sky_ambient_occlusion_effect;
	/* 0x003c */	res_effect_ptr				m_ambient_volume_effect;
	/* 0x0040 */	res_effect_ptr				m_reflection_mask_effect;
	/* 0x0044 */	res_effect_ptr				m_sh_ssao_downsample_position_and_normal;
	/* 0x0048 */	float						m_ambient_multiplier;
	/* 0x004c */	bool						m_use_probes;
	/* 0x0050 */	shader_constant_host*		m_c_eye_ray_corner;
	/* 0x0054 */	shader_constant_host*		m_c_light_range;
	/* 0x0058 */	shader_constant_host*		m_c_num_mips;
	/* 0x005c */	shader_constant_host*		m_c_color_parameters;
	/* 0x0060 */	shader_constant_host*		m_c_skylight_parameters0;
	/* 0x0064 */	shader_constant_host*		m_c_skylight_parameters1;
	/* 0x0068 */	shader_constant_host*		m_c_skylight_parameters2;
	/* 0x006c */	shader_constant_host*		m_c_skylight_parameters3;
	/* 0x0070 */	shader_constant_host*		m_c_skylight_parameters4;
	/* 0x0074 */	shader_constant_host*		m_c_skylight_parameters5;
	/* 0x0078 */	shader_constant_host*		m_c_skylight_parameters6;
	/* 0x007c */	shader_constant_host*		m_c_skylight_parameters7;
	/* 0x0080 */	shader_constant_host*		m_c_world_to_ao_map;
	/* 0x0084 */	shader_constant_host*		m_c_ambient_volume_multiplier;
	/* 0x0088 */	shader_constant_host*		m_c_world_to_probe;
	/* 0x008c */	untyped_buffer_ptr			m_sphere_vertex_buffer;
	/* 0x0090 */	untyped_buffer_ptr			m_sphere_index_buffer;
	/* 0x0094 */	res_geometry_ptr			m_sphere_geometry;
	/* 0x0098 */	untyped_buffer_ptr			m_box_vertex_buffer;
	/* 0x009c */	untyped_buffer_ptr			m_box_index_buffer;
	/* 0x00a0 */	res_geometry_ptr			m_box_geometry;
}; // class stage_ambient_lighting

STATIC_SIZE_ASSERT(stage_ambient_lighting, 0xA4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STAGE_AMBIENT_LIGHTING_H_INCLUDED
