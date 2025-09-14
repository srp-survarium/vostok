////////////////////////////////////////////////////////////////////////////
//	Created		: 20.09.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_ENGINE_EFFECT_DOWNSAMPLE_REFLECTIVE_SHADOW_MAP_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_DOWNSAMPLE_REFLECTIVE_SHADOW_MAP_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class effect_downsample_reflective_shadow_map: public effect_descriptor
{
public:
	virtual	void compile(effect_compiler& compiler, custom_config_value const& config);
}; // class effect_downsample_reflective_shadow_map

} // namespace vostok
} // namespace render

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_DOWNSAMPLE_REFLECTIVE_SHADOW_MAP_H_INCLUDED