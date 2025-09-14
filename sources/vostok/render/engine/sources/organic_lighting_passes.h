////////////////////////////////////////////////////////////////////////////
//	Created		: 29.08.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_ENGINE_ORGANIC_LIGHTING_PASSES_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_ORGANIC_LIGHTING_PASSES_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

enum enum_organic_lighting_pass
{
	organic_lighting_position_pass = 0,
	organic_lighting_lighting_pass,
	organic_lighting_blurring_pass,
	organic_lighting_combine_pass,
}; // enum enum_organic_lighting_pass

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_ORGANIC_LIGHTING_PASSES_H_INCLUDED

