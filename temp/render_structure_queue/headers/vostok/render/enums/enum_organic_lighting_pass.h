////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_ENUM_ORGANIC_LIGHTING_PASS_H_INCLUDED
#define RENDER_ENUM_ORGANIC_LIGHTING_PASS_H_INCLUDED

namespace vostok {
namespace render {

enum enum_organic_lighting_pass
{
	organic_lighting_position_pass		= 0x0,
	organic_lighting_lighting_pass		= 0x1,
	organic_lighting_blurring_pass		= 0x2,
	organic_lighting_combine_pass		= 0x3,
};

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_ENUM_ORGANIC_LIGHTING_PASS_H_INCLUDED
