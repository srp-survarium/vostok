// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_ENGINE_SCR_QUAD_EFFECT_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_SCR_QUAD_EFFECT_H_INCLUDED
#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class scr_quad_effect : public effect_descriptor {
public:
	enum {
		back_buffer_only,
		prev_and_back_buffer
	};

	virtual void compile(
		effect_compiler& compiler,
		custom_config_value const& config
	) override;
};

STATIC_SIZE_ASSERT( scr_quad_effect, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_SCR_QUAD_EFFECT_H_INCLUDED
