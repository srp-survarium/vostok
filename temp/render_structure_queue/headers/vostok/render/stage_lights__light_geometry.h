////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STAGE_LIGHTS_LIGHT_GEOMETRY_H_INCLUDED
#define RENDER_STAGE_LIGHTS_LIGHT_GEOMETRY_H_INCLUDED

/* INCLUDES */
typedef vostok::intrusive_ptr<vostok::render::res_geometry,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_geometry_ptr;
typedef vostok::intrusive_ptr<vostok::render::untyped_buffer,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::untyped_buffer_ptr;
class vostok::render::res_geometry;
class vostok::render::untyped_buffer;

namespace vostok {
namespace render {

struct stage_lights::light_geometry {
	inline		light_geometry	( ) { /* no source */ }
	inline		~light_geometry	( ) { /* no source */ }

	/* 0x0000 */	untyped_buffer_ptr		vertex_buffer;
	/* 0x0004 */	untyped_buffer_ptr		index_buffer;
	/* 0x0008 */	res_geometry_ptr		geometry;
}; // struct stage_lights::light_geometry

STATIC_SIZE_ASSERT(stage_lights::light_geometry, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STAGE_LIGHTS_LIGHT_GEOMETRY_H_INCLUDED
