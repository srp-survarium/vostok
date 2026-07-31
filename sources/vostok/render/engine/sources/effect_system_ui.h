#ifndef VOSTOK_RENDER_ENGINE_EFFECT_SYSTEM_UI_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_SYSTEM_UI_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

class effect_system_ui : public effect_descriptor {
public:
	enum techniques{ ui_font, ui, ui_fill, ui_line_strip };

	effect_system_ui( ) { }
	virtual ~effect_system_ui( ) { }

	virtual void compile(
		effect_compiler&			compiler,
		custom_config_value const&	config
	) override;
};

STATIC_SIZE_ASSERT( effect_system_ui, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_SYSTEM_UI_H_INCLUDED
