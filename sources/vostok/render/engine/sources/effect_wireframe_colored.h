// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_ENGINE_EFFECT_WIREFRAME_COLORED_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_WIREFRAME_COLORED_H_INCLUDED
#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

class effect_wireframe_colored : public effect_descriptor {
public:
	effect_wireframe_colored( ) { }
	virtual ~effect_wireframe_colored( ) { }

	virtual void compile(
		effect_compiler&			compiler,
		custom_config_value const&	config
	) override;
};

STATIC_SIZE_ASSERT( effect_wireframe_colored, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_WIREFRAME_COLORED_H_INCLUDED
