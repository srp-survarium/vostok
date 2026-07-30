#ifndef VOSTOK_RENDER_ENGINE_STAGE_AMBIENT_OCCLUSION_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_STAGE_AMBIENT_OCCLUSION_H_INCLUDED

#include <vostok/render/core/res_effect.h>
#include <vostok/render/core/untyped_buffer.h>

#include "res_geometry.h"
#include "stage.h"

namespace vostok {
namespace render {

class renderer;
class renderer_context;
class shader_constant_host;

typedef resources::resource_ptr<
	res_effect,
	resources::unmanaged_intrusive_base
> res_effect_ptr;

class stage_ambient_occlusion : public stage {
public:
	stage_ambient_occlusion( renderer* in_renderer, renderer_context* context );
	virtual ~stage_ambient_occlusion( ) { }

	virtual void execute( ) override;
	virtual void execute_disabled( ) override;

private:
	bool is_effects_ready( ) const;

private:
	res_effect_ptr			m_sh_combine;
	res_effect_ptr			m_sh_ssao_accumulation;
	res_effect_ptr			m_sh_ssao_filter4x4;
	res_effect_ptr			m_sh_ssao_downsample_position_and_normal;
	res_effect_ptr			m_post_process_antialiasing_shader;
	res_effect_ptr			m_post_process_deferred_transparency_shader;
	res_geometry_ptr		m_g_combine;
	untyped_buffer_ptr		m_vb;
	shader_constant_host*	m_ao_parameters;
	shader_constant_host*	m_c_eye_ray_corner;
	shader_constant_host*	m_prev_view_parameter;
	shader_constant_host*	m_prev_ssao_valid_parameter;
};

STATIC_SIZE_ASSERT( stage_ambient_occlusion, 0x40 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_STAGE_AMBIENT_OCCLUSION_H_INCLUDED
