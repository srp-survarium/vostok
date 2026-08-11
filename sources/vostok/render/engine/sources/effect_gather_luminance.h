#ifndef VOSTOK_RENDER_ENGINE_EFFECT_GATHER_LUMINANCE_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_GATHER_LUMINANCE_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

class effect_gather_luminance : public effect_descriptor {
public:
	enum
	{
		gather_log_luminance,
		gather_luminance,
		gather_exp_luminance,
	};

	virtual void compile(
		effect_compiler&			compiler,
		custom_config_value const&	custom_config
	) override;

};

STATIC_SIZE_ASSERT( effect_gather_luminance, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_GATHER_LUMINANCE_H_INCLUDED
