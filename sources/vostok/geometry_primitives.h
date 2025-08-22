////////////////////////////////////////////////////////////////////////////
//	Created 	: 13.11.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef GEOMETRY_PRIMITIVES_H_INCLUDED
#define GEOMETRY_PRIMITIVES_H_INCLUDED

namespace vostok {
namespace geometry_utils{

////////////////////////////////////////////////////////////////////////////
// Oriented Bounding Box
////////////////////////////////////////////////////////////////////////////

namespace obb {

	extern VOSTOK_CORE_API const u32	vertex_count;
	extern VOSTOK_CORE_API const float vertices[];

	extern VOSTOK_CORE_API const u32	pair_count;
	extern VOSTOK_CORE_API const u16	pairs[];

} // namespace obb

////////////////////////////////////////////////////////////////////////////
// Rectangle
////////////////////////////////////////////////////////////////////////////
namespace rectangle {

	extern VOSTOK_CORE_API const u32	vertex_count;
	extern VOSTOK_CORE_API const float	vertices[];

	extern VOSTOK_CORE_API const u32	pair_count;
	extern VOSTOK_CORE_API const u16	pairs[];

} // namespace Rectangle

////////////////////////////////////////////////////////////////////////////
// Ellipsoid
////////////////////////////////////////////////////////////////////////////
namespace ellipsoid {
	extern VOSTOK_CORE_API const u32	vertex_count;
	extern VOSTOK_CORE_API const float	vertices[ ];

	extern VOSTOK_CORE_API const u32	pair_count;
	extern VOSTOK_CORE_API const u16	pairs[];
} // namespace ellipsoid

////////////////////////////////////////////////////////////////////////////
// Cone
////////////////////////////////////////////////////////////////////////////
namespace cone {
	extern VOSTOK_CORE_API const u32	vertex_count;
	extern VOSTOK_CORE_API const float	vertices[];

	extern VOSTOK_CORE_API const u32	pair_count;
	extern VOSTOK_CORE_API const u16	pairs[];
} // namespace cone


////////////////////////////////////////////////////////////////////////////
// Ellipse 
////////////////////////////////////////////////////////////////////////////
namespace ellipse {
	extern VOSTOK_CORE_API const u32	vertex_count;
	extern VOSTOK_CORE_API const float	vertices[];

	extern VOSTOK_CORE_API const u32	pair_count;
	extern VOSTOK_CORE_API const u16	pairs[];
} // namespace ellipse
////////////////////////////////////////////////////////////////////////////
// Cylinder 
////////////////////////////////////////////////////////////////////////////
namespace cylinder {
	extern VOSTOK_CORE_API const u32	vertex_count;
	extern VOSTOK_CORE_API const float	vertices[];

	extern VOSTOK_CORE_API const u32	pair_count;
	extern VOSTOK_CORE_API const u16	pairs[];
} // namespace ellipse

////////////////////////////////////////////////////////////////////////////
// Circle 
////////////////////////////////////////////////////////////////////////////
namespace circle {
	extern VOSTOK_CORE_API const u32	vertex_count;
	extern VOSTOK_CORE_API const float	vertices[];

	extern VOSTOK_CORE_API const u32	pair_count;
	extern VOSTOK_CORE_API const u16	pairs[];
} // namespace ellipse

////////////////////////////////////////////////////////////////////////////
// Cube solid
////////////////////////////////////////////////////////////////////////////
namespace cube_solid {
	extern VOSTOK_CORE_API const u32 vertex_count;
	extern VOSTOK_CORE_API const float vertices[];

	extern VOSTOK_CORE_API const u32 face_count; 
	extern VOSTOK_CORE_API const u32 index_count; 
	extern VOSTOK_CORE_API const u16 faces[];
} //namespace cube_solid 


////////////////////////////////////////////////////////////////////////////
// Rectangle solid
////////////////////////////////////////////////////////////////////////////
namespace rectangle_solid {

	extern VOSTOK_CORE_API const u32 vertex_count;
	extern VOSTOK_CORE_API const float vertices[];

	extern VOSTOK_CORE_API const u32 face_count;
	extern VOSTOK_CORE_API const u32 index_count; 
	extern VOSTOK_CORE_API const u16 faces[];

} // namespace Rectangle

////////////////////////////////////////////////////////////////////////////
// Cone solid
////////////////////////////////////////////////////////////////////////////
namespace cone_solid {
	extern VOSTOK_CORE_API const u32 vertex_count;
	extern VOSTOK_CORE_API const float vertices[];

	extern VOSTOK_CORE_API const u32 face_count; 
	extern VOSTOK_CORE_API const u32 index_count; 
	extern VOSTOK_CORE_API const u16 faces[];
} // namespace cone_solid


////////////////////////////////////////////////////////////////////////////
// Cylinder
////////////////////////////////////////////////////////////////////////////
namespace cylinder_solid {
	extern VOSTOK_CORE_API const u32 vertex_count;
	extern VOSTOK_CORE_API const float vertices[];

	extern VOSTOK_CORE_API const u32 face_count; 
	extern VOSTOK_CORE_API const u32 index_count; 
	extern VOSTOK_CORE_API const u16 faces[];
} //namespace cylinder_solid 

////////////////////////////////////////////////////////////////////////////
// Ellipsoid solid
////////////////////////////////////////////////////////////////////////////
namespace ellipsoid_solid {
	extern VOSTOK_CORE_API const u32 vertex_count;
	extern VOSTOK_CORE_API const float vertices[];

	extern VOSTOK_CORE_API const u32 face_count; 
	extern VOSTOK_CORE_API const u32 index_count; 
	extern VOSTOK_CORE_API const u16 faces[];
} //namespace ellipsoid_solid 

namespace octahedron_solid {
	extern VOSTOK_CORE_API const u32 vertex_count;
	extern VOSTOK_CORE_API const float vertices[];

	extern VOSTOK_CORE_API const u32 face_count; 
	extern VOSTOK_CORE_API const u32 index_count; 
	extern VOSTOK_CORE_API const u16 faces[];

	extern VOSTOK_CORE_API const u32	pair_count;
	extern VOSTOK_CORE_API const u16	pairs[];
} //namespace octahedron_solid

namespace line_ellipsoid{
	extern VOSTOK_CORE_API const u32 vertex_count;
	extern VOSTOK_CORE_API const float vertices_xy[];
	extern VOSTOK_CORE_API const float vertices_yz[];
	extern VOSTOK_CORE_API const float vertices_xz[];
	extern VOSTOK_CORE_API const u32	pair_count;
	extern VOSTOK_CORE_API const u16	pairs[];
} //line_ellipsoid

namespace line_arc{
	extern VOSTOK_CORE_API const u32 vertex_count;
	extern VOSTOK_CORE_API const float vertices_xy[];
	extern VOSTOK_CORE_API const u32	pair_count;
	extern VOSTOK_CORE_API const u16	pairs[];
} //line_arc

bool generate_torus			( vostok::vectora< math::float3 >& vertices, vostok::vectora< u16 >& indices, math::float4x4 transform,
									 float outer_raduius,  float inner_raduius, u16 outer_segments, u16 inner_segments );

} // namespace geometry_utils
} // namespace vostok

#endif // #ifndef GEOMETRY_PRIMITIVES_H_INCLUDED