////////////////////////////////////////////////////////////////////////////
//	Created		: 06.09.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_ENGINE_EFFECT_POST_PROCESS_MLAA_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_POST_PROCESS_MLAA_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class effect_post_process_mlaa: public effect_descriptor
{
public:
	enum
	{
		color_edge_detection		= 0,
		blending_weight_calculation,
		neighborhood_blending,
	};
	virtual void compile( effect_compiler& compiler, const custom_config_value& custom_config);
}; // class effect_post_process_mlaa

} // namespace render 
} // namespace vostok 


#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_POST_PROCESS_MLAA_H_INCLUDED