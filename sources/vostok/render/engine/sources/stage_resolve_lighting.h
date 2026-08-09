#ifndef VOSTOK_RENDER_ENGINE_STAGE_RESOLVE_LIGHTING_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_STAGE_RESOLVE_LIGHTING_H_INCLUDED

#include <vostok/render/core/memory.h>
#include <vostok/render/core/res_effect.h>

#include "stage.h"

namespace vostok {
namespace render {

class renderer;
class renderer_context;
class shader_constant_host;
struct render_surface_instance;

typedef resources::resource_ptr<
	res_effect,
	resources::unmanaged_intrusive_base
> res_effect_ptr;

class stage_resolve_lighting : public stage {
public:
	stage_resolve_lighting(
		renderer* in_renderer,
		renderer_context* context
	);

	virtual void execute( ) override;

	bool is_effects_ready( ) const;
	void render_models(
		vector<render_surface_instance*>& models,
		u32& out_num_rendered
	);

private:
	res_effect_ptr			m_resolve_lighting_effect;
	shader_constant_host*	m_wind_info_parameters;
	shader_constant_host*	m_sun_light_parameters;
	shader_constant_host*	m_eye_ray_corner_parameter;
};

STATIC_SIZE_ASSERT( stage_resolve_lighting, 0x20 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_STAGE_RESOLVE_LIGHTING_H_INCLUDED
