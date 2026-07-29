////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STAGE_AMBIENT_OCCLUSION_H_INCLUDED
#define RENDER_STAGE_AMBIENT_OCCLUSION_H_INCLUDED

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

class stage_ambient_occlusion : public stage {
public:
					stage_ambient_occlusion	( renderer* in_renderer, renderer_context* context );

	virtual	void	execute					( ) override;

	virtual	void	execute_disabled		( ) override;

private:
			bool	is_effects_ready		( ) const;

public:
	virtual			~stage_ambient_occlusion( ) { /* no source */ }

	/* 0x0000 */	/* stage */
private:
	/* 0x0010 */	res_effect_ptr				m_sh_combine;
	/* 0x0014 */	res_effect_ptr				m_sh_ssao_accumulation;
	/* 0x0018 */	res_effect_ptr				m_sh_ssao_filter4x4;
	/* 0x001c */	res_effect_ptr				m_sh_ssao_downsample_position_and_normal;
	/* 0x0020 */	res_effect_ptr				m_post_process_antialiasing_shader;
	/* 0x0024 */	res_effect_ptr				m_post_process_deferred_transparency_shader;
	/* 0x0028 */	res_geometry_ptr			m_g_combine;
	/* 0x002c */	untyped_buffer_ptr			m_vb;
	/* 0x0030 */	shader_constant_host*		m_ao_parameters;
	/* 0x0034 */	shader_constant_host*		m_c_eye_ray_corner;
	/* 0x0038 */	shader_constant_host*		m_prev_view_parameter;
	/* 0x003c */	shader_constant_host*		m_prev_ssao_valid_parameter;
}; // class stage_ambient_occlusion

STATIC_SIZE_ASSERT(stage_ambient_occlusion, 0x40);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STAGE_AMBIENT_OCCLUSION_H_INCLUDED
