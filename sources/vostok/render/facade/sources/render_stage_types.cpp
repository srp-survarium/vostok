////////////////////////////////////////////////////////////////////////////
//	Created		: 16.09.2010
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/render/facade/sources/render_stage_types.h>

namespace vostok {
namespace render {

pcstr stage_type_to_string(enum_render_stage_type stage_type)
{
	switch (stage_type)
	{
		case gbuffer_render_stage:										return "g_stage";
		case accumulate_distortion_render_stage:						return "distortion";
		case forward_render_stage:										return "forward";
		case shadow_render_stage:										return "shadow";
		case ambient_occlusion_render_stage:							return "ambient_occlusion";
		case light_propagation_volumes_render_stage:					return "light_propagation_volumes";
		case lighting_render_stage:										return "lighting";
		case post_process_render_stage:									return "post_process";
		case debug_post_process_render_stage:							return "debug_post_process";
		case debug_render_stage:										return "debug";
		case decals_accumulate_render_stage:							return "decals";
		default:														return "unknown";
	};
}

} // namespace render
} // namespace vostok
