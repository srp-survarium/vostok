// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_ENGINE_ORGANIC_LIGHTING_PASSES_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_ORGANIC_LIGHTING_PASSES_H_INCLUDED
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
