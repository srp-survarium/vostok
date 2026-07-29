////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RENDER_SURFACE_INSTANCE_FLAGS_H_INCLUDED
#define RENDER_RENDER_SURFACE_INSTANCE_FLAGS_H_INCLUDED

namespace vostok {
namespace render {

enum render_surface_instance_flags
{
	fully_invisible		= 0x0,
	visible_flag		= 0x1,
	cast_shadow_flag	= 0x2,
};

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RENDER_SURFACE_INSTANCE_FLAGS_H_INCLUDED
