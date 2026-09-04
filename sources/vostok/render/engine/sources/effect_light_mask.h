// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_ENGINE_EFFECT_LIGHT_MASK_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_LIGHT_MASK_H_INCLUDED
#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

class effect_light_mask : public effect_descriptor {
public:
	virtual void compile(
		effect_compiler&			compiler,
		custom_config_value const&	config
	) override;

};

STATIC_SIZE_ASSERT( effect_light_mask, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_LIGHT_MASK_H_INCLUDED
