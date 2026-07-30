#ifndef VOSTOK_RENDER_ENGINE_EFFECT_EDITOR_ACCUMULATE_OVERDRAW_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_EDITOR_ACCUMULATE_OVERDRAW_H_INCLUDED

#include "effect_material_base.h"

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

class effect_editor_accumulate_overdraw : public effect_material_base {
public:
	effect_editor_accumulate_overdraw( ) { }
	virtual ~effect_editor_accumulate_overdraw( ) { }

	virtual void compile(
		effect_compiler&			compiler,
		custom_config_value const&	config
	) override;
};

STATIC_SIZE_ASSERT( effect_editor_accumulate_overdraw, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_EDITOR_ACCUMULATE_OVERDRAW_H_INCLUDED
