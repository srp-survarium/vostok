////////////////////////////////////////////////////////////////////////////
//	Created 	: 13.11.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_BASE_DEBUG_RENDERER_H_INCLUDED
#define VOSTOK_RENDER_BASE_DEBUG_RENDERER_H_INCLUDED

#include <vostok/render/base/common_types.h>

namespace vostok {
namespace render {

namespace logic {
	template < typename T >
	class vector;
} // namespace logic

namespace debug {

struct VOSTOK_NOVTABLE renderer {
	virtual	void 	tick			( ) = 0;

	// wireframe functions
	virtual	void 	draw_line		( float3 const& start_point, float3 const& end_point, math::color color ) = 0;
	virtual	void 	draw_line		( float3 const& start_point, float3 const& end_point, u32 color ) = 0;
	virtual	void 	draw_lines		( vectora< vertex_colored > const& vertices, vostok::vectora< u16 > const& indices ) = 0;
	virtual	void 	draw_obb		( float4x4 const& matrix, float3 const& size, math::color color ) = 0;
	virtual	void	draw_rectangle	( float4x4 const& matrix, float3 const& size, math::color color ) = 0;
	virtual	void 	draw_aabb		( float3 const& center, float3 const& size, math::color color ) = 0;
	virtual	void 	draw_ellipse	( float4x4 const& matrix, math::color color ) = 0;
	virtual	void 	draw_ellipsoid	( float4x4 const& matrix, math::color color ) = 0;
	virtual	void 	draw_ellipsoid	( float4x4 const& matrix, float3 const& size, math::color color ) = 0;
	virtual	void 	draw_sphere		( float3 const& center, const float &radius, math::color color ) = 0;
	virtual	void 	draw_cone		( float4x4 const& matrix, float3 const& size, math::color color ) = 0;
	virtual	void 	draw_arrow		( float3 const& start_point, float3 const& end_point, math::color arrow_color, math::color cone_color ) = 0;
	virtual	void 	draw_arrow		( float3 const& start_point, float3 const& end_point, math::color color ) = 0;

	// solid functions
	virtual	void 	draw_triangle	( float3 const& point_0, float3 const& point_1, float3 const& point_2, math::color color ) = 0;
	virtual	void 	draw_triangle	( float3 const& point_0, float3 const& point_1, float3 const& point_2, u32 color ) = 0;
	virtual	void 	draw_triangle	( vertex_colored const& vertex_0, vertex_colored const& vertex_1, vertex_colored const& vertex_2 ) = 0;
	virtual	void 	draw_triangle	( vertex_colored const ( &vertices )[ 3 ] ) = 0;
	virtual	void 	draw_triangles	( vectora< vertex_colored > const &vertices ) = 0;
	virtual	void 	draw_triangles	( vectora< vertex_colored > const &vertices, vectora< u16 > const& indices ) = 0;

	virtual	void 	draw_rectangle_solid( float4x4 const& matrix, float3 const& size, math::color color ) = 0;
	virtual	void 	draw_cube_solid		( float4x4 const& matrix, float3 const& size, math::color color ) = 0;
	virtual	void 	draw_cone_solid		( float4x4 const& matrix, float3 const& size, math::color color ) = 0;
	virtual	void 	draw_cylinder_solid	( float4x4 const& matrix, float3 const& size, math::color color ) = 0;
	virtual	void 	draw_ellipsoid_solid( float4x4 const& matrix, float3 const& size, math::color color ) = 0;

protected:
	VOSTOK_DECLARE_PURE_VIRTUAL_DESTRUCTOR( renderer )
}; // struct render

} // namespace debug
} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_BASE_DEBUG_RENDERER_H_INCLUDED