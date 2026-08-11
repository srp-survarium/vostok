#ifndef VOSTOK_RENDER_ENGINE_EFFECT_LIGHT_PROPAGATION_VOLUMES_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_LIGHT_PROPAGATION_VOLUMES_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

class effect_light_propagation_volumes : public effect_descriptor {
public:
	enum {
		inject_lighting_stage = 0,
		inject_occluders_light_view_stage,
		inject_occluders_camera_view_stage,
		inject_occluder_geometry_stage,
		propagate_lighting_stage,
		accumulate_propagation_stage,
	};

	virtual void compile(
		effect_compiler&			compiler,
		custom_config_value const&	config
	) override;

};

STATIC_SIZE_ASSERT( effect_light_propagation_volumes, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_LIGHT_PROPAGATION_VOLUMES_H_INCLUDED
