////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SPHERE_OCCLUDER_GEOMETRY_H_INCLUDED
#define RENDER_SPHERE_OCCLUDER_GEOMETRY_H_INCLUDED

/* INCLUDES */
typedef vostok::intrusive_ptr<vostok::render::res_geometry,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_geometry_ptr;
class vostok::render::res_geometry;

namespace vostok {
namespace render {

struct sphere_occluder_geometry {
					sphere_occluder_geometry	( );

			void	render						( );

	inline			~sphere_occluder_geometry	( ) { /* no source */ }

private:
	/* 0x0000 */	res_geometry_ptr	m_geometry;
}; // struct sphere_occluder_geometry

STATIC_SIZE_ASSERT(sphere_occluder_geometry, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SPHERE_OCCLUDER_GEOMETRY_H_INCLUDED
