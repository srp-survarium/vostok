////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_LIGHT_TYPE_H_INCLUDED
#define RENDER_LIGHT_TYPE_H_INCLUDED

namespace vostok {
namespace render {

enum light_type
{
	light_type_point			= 0x0,
	light_type_spot				= 0x1,
	light_type_obb				= 0x2,
	light_type_capsule			= 0x3,
	light_type_parallel			= 0x4,
	light_type_sphere			= 0x5,
	light_type_plane_spot		= 0x6,
	num_light_types				= 0x7,
};

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_LIGHT_TYPE_H_INCLUDED
