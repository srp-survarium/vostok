#ifndef VOSTOK_RENDER_ENGINE_STAGE_TRANSLUCENCY_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_STAGE_TRANSLUCENCY_H_INCLUDED

#include <vostok/render/core/res_effect.h>

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

class stage_translucency : public stage {
public:
	stage_translucency(
		renderer* in_renderer,
		renderer_context* context
	);
	virtual void execute( ) override;

	bool is_effects_ready( ) const;

private:
	res_effect_ptr			m_translucency_effect;
	shader_constant_host*	m_shadow[4];
	shader_constant_host*	m_c_sun_direction;
	shader_constant_host*	m_c_sun_color;
	shader_constant_host*	m_c_eye_ray_corner;
};

STATIC_SIZE_ASSERT( stage_translucency, 0x30 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_STAGE_TRANSLUCENCY_H_INCLUDED
