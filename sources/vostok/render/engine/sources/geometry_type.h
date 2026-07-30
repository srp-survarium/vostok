#ifndef VOSTOK_RENDER_ENGINE_GEOMETRY_TYPE_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_GEOMETRY_TYPE_H_INCLUDED

namespace vostok {
namespace render {

enum enum_geometry_type
{
	terrain_geometry_type		= 0x01,
	static_geometry_type,
	speedtree_geometry_type,
	translucency_geometry_type,
	skeleton_geometry_type,
	particle_geometry_type,

	num_geometry_types			= 6,
	all_geometry_type			= 0x80,
	check_all_geometry_type		= 0x40,
}; // enum enum_geometry_type

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_GEOMETRY_TYPE_H_INCLUDED
