#ifndef VOSTOK_RENDER_ENGINE_EFFECT_DEBUG_EDITOR_WIREFRAME_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_DEBUG_EDITOR_WIREFRAME_H_INCLUDED

#include "effect_material_base.h"

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

class effect_debug_editor_wireframe : public effect_material_base {
public:
	effect_debug_editor_wireframe( ) { }
	virtual ~effect_debug_editor_wireframe( ) { }

	virtual void compile(
		effect_compiler&,
		custom_config_value const&
	) override;
};

STATIC_SIZE_ASSERT( effect_debug_editor_wireframe, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_DEBUG_EDITOR_WIREFRAME_H_INCLUDED
