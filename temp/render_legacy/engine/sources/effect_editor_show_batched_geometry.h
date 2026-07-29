////////////////////////////////////////////////////////////////////////////
//	Created		: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_ENGINE_EFFECT_EDITOR_SHOW_BATCHED_GEOMETRY_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_EDITOR_SHOW_BATCHED_GEOMETRY_H_INCLUDED

#include "effect_material_base.h"

namespace vostok {
namespace render {

class effect_editor_show_batched_geometry: public effect_material_base
{
public:
	virtual	void compile(effect_compiler& compiler, custom_config_value const& config);
}; // class effect_editor_show_batched_geometry

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_EDITOR_SHOW_BATCHED_GEOMETRY_H_INCLUDED
