////////////////////////////////////////////////////////////////////////////
//	Created		: 26.09.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_ENGINE_EFFECT_RADIANCE_VOLUME_PROPAGATE_LIGHTING_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_RADIANCE_VOLUME_PROPAGATE_LIGHTING_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class effect_radiance_volume_propagate_lighting: public effect_descriptor
{
public:
	virtual	void compile(effect_compiler& compiler, custom_config_value const& config);
}; // class effect_radiance_volume_propagate_lighting

} // namespace vostok
} // namespace render

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_RADIANCE_VOLUME_PROPAGATE_LIGHTING_H_INCLUDED