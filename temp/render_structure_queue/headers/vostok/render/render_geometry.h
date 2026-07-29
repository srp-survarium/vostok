////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RENDER_GEOMETRY_H_INCLUDED
#define RENDER_RENDER_GEOMETRY_H_INCLUDED

/* INCLUDES */
class vostok::fixed_string<64>;
typedef vostok::intrusive_ptr<vostok::render::res_geometry,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_geometry_ptr;
class vostok::render::res_geometry;

namespace vostok {
namespace render {

struct render_geometry {
	inline		render_geometry	( ) { /* no source */ }
	inline		~render_geometry( ) { /* no source */ }

	/* 0x0000 */	res_geometry_ptr		geom;
	/* 0x0004 */	res_geometry_ptr		shadow_pass_geom;
	/* 0x0008 */	res_geometry_ptr		lpv_pass_geom;
	/* 0x000c */	u32						vertex_count;
	/* 0x0010 */	u32						index_count;
	/* 0x0014 */	u32						primitive_count;
	/* 0x0018 */	fixed_string< 64 >		shading_group_name;
}; // struct render_geometry

STATIC_SIZE_ASSERT(render_geometry, 0x64);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RENDER_GEOMETRY_H_INCLUDED
