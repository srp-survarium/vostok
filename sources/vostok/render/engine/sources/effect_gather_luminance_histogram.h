#ifndef VOSTOK_RENDER_ENGINE_EFFECT_GATHER_LUMINANCE_HISTOGRAM_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_GATHER_LUMINANCE_HISTOGRAM_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

class effect_gather_luminance_histogram : public effect_descriptor {
public:
	enum
	{
		gather_luminance_in_range,
		gather_luminance_count,
		gather_luminance_histogram,
	};

	effect_gather_luminance_histogram( ) { }
	virtual ~effect_gather_luminance_histogram( ) { }

	virtual void compile(
		effect_compiler&			compiler,
		custom_config_value const&	config
	) override;
};

STATIC_SIZE_ASSERT( effect_gather_luminance_histogram, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_GATHER_LUMINANCE_HISTOGRAM_H_INCLUDED
