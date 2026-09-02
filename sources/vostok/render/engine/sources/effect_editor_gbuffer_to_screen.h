// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_ENGINE_EFFECT_EDITOR_GBUFFER_TO_SCREEN_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_EDITOR_GBUFFER_TO_SCREEN_H_INCLUDED
#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

class effect_editor_gbuffer_to_screen : public effect_descriptor {
public:

	virtual void compile(
		effect_compiler&			compiler,
		custom_config_value const&	config
	) override;
};

STATIC_SIZE_ASSERT( effect_editor_gbuffer_to_screen, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_EDITOR_GBUFFER_TO_SCREEN_H_INCLUDED
