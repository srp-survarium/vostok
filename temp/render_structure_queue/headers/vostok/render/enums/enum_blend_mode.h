////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_ENUM_BLEND_MODE_H_INCLUDED
#define RENDER_ENUM_BLEND_MODE_H_INCLUDED

namespace vostok {
namespace render {

enum enum_blend_mode
{
	blend_mode_opaque			= 0x0,
	blend_mode_translucent		= 0x1,
	blend_mode_additive			= 0x2,
	blend_mode_modulate			= 0x3,
	blend_mode_subtractive		= 0x4,
};

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_ENUM_BLEND_MODE_H_INCLUDED
