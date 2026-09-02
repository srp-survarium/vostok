// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_ENGINE_EFFECT_SSAO_DOWNSAMPLE_POSITION_AND_NORMAL_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_SSAO_DOWNSAMPLE_POSITION_AND_NORMAL_H_INCLUDED
#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

class effect_ssao_downsample_position_and_normal : public effect_descriptor {
public:
	virtual void compile(
		effect_compiler&			compiler,
		custom_config_value const&	custom_config
	) override;
};

STATIC_SIZE_ASSERT( effect_ssao_downsample_position_and_normal, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_SSAO_DOWNSAMPLE_POSITION_AND_NORMAL_H_INCLUDED
