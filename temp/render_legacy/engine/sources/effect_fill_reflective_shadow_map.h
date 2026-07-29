////////////////////////////////////////////////////////////////////////////
//	Created		: 19.09.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_ENGINE_EFFECT_FILL_REFLECTIVE_SHADOW_MAP_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_FILL_REFLECTIVE_SHADOW_MAP_H_INCLUDED

#include "effect_material_base.h"

namespace vostok {
namespace render {

class effect_fill_reflective_shadow_map: public effect_material_base
{
public:
	virtual	void compile(effect_compiler& compiler, custom_config_value const& config);
}; // class effect_fill_reflective_shadow_map

} // namespace vostok
} // namespace render

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_FILL_REFLECTIVE_SHADOW_MAP_H_INCLUDED