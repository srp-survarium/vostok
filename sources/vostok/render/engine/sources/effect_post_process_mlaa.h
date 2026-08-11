#ifndef VOSTOK_RENDER_ENGINE_EFFECT_POST_PROCESS_MLAA_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_POST_PROCESS_MLAA_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

class effect_post_process_mlaa : public effect_descriptor {
public:
	enum
	{
		color_edge_detection		= 0,
		blending_weight_calculation,
		neighborhood_blending,
	};

	virtual void compile(
		effect_compiler&			compiler,
		custom_config_value const&	custom_config
	) override;

};

STATIC_SIZE_ASSERT( effect_post_process_mlaa, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_POST_PROCESS_MLAA_H_INCLUDED
