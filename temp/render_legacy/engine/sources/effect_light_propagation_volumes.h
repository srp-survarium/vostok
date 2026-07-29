////////////////////////////////////////////////////////////////////////////
//	Created		: 27.09.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_ENGINE_EFFECT_LIGHT_PROPAGATION_VOLUMES_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_LIGHT_PROPAGATION_VOLUMES_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class effect_light_propagation_volumes: public effect_descriptor
{
public:
	enum
	{
		inject_lighting_stage	 = 0,
		inject_occluders_light_view_stage,
		inject_occluders_camera_view_stage,
		propagate_lighting_stage,
		accumulate_propagation_stage,
	};
	virtual	void compile(effect_compiler& compiler, custom_config_value const& config);
}; // class effect_light_propagation_volumes

} // namespace vostok
} // namespace render

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_LIGHT_PROPAGATION_VOLUMES_H_INCLUDED