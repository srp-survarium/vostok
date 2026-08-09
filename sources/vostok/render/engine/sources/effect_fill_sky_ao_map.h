#ifndef VOSTOK_RENDER_ENGINE_EFFECT_FILL_SKY_AO_MAP_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_FILL_SKY_AO_MAP_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

class effect_fill_sky_ao_map : public effect_descriptor {
public:
	virtual void compile(
		effect_compiler&			compiler,
		custom_config_value const&	config
	) override;

	effect_fill_sky_ao_map( ) { }
	virtual ~effect_fill_sky_ao_map( ) { }
};

STATIC_SIZE_ASSERT( effect_fill_sky_ao_map, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_FILL_SKY_AO_MAP_H_INCLUDED
