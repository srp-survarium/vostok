#ifndef VOSTOK_RENDER_ENGINE_EFFECT_ENVIRONMENT_PROBE_LIGHTING_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_ENVIRONMENT_PROBE_LIGHTING_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

template <
	bool t_option_0,
	bool t_option_1,
	bool t_option_2
>
class effect_environment_probe_lighting : public effect_descriptor {
public:
	effect_environment_probe_lighting( ) { }
	virtual ~effect_environment_probe_lighting( ) { }

	virtual void compile(
		effect_compiler&,
		custom_config_value const&
	) override
	{
		// claude@NOTE: no legacy ancestor - effect postdates the legacy corpus; matcher-phase work.
		// STATE[STUB]
		// FUNCTION BODY[0x75280] for <false, false, false>
	}
};

typedef effect_environment_probe_lighting<
	false,
	false,
	false
> effect_environment_probe_lighting_000;

STATIC_SIZE_ASSERT( effect_environment_probe_lighting_000, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_ENVIRONMENT_PROBE_LIGHTING_H_INCLUDED
