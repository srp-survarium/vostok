////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_ENUM_GEOMETRY_TYPE_H_INCLUDED
#define RENDER_ENUM_GEOMETRY_TYPE_H_INCLUDED

namespace vostok {
namespace render {

enum enum_geometry_type
{
	terrain_geometry_type			= 0x01,
	static_geometry_type			= 0x02,
	speedtree_geometry_type			= 0x03,
	translucency_geometry_type		= 0x04,
	skeleton_geometry_type			= 0x05,
	particle_geometry_type			= 0x06,
	num_geometry_types				= 0x06,
	all_geometry_type				= 0x80,
	check_all_geometry_type			= 0x40,
};

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_ENUM_GEOMETRY_TYPE_H_INCLUDED
