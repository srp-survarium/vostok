////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SPHERE_GEOMETRY_H_INCLUDED
#define RENDER_SPHERE_GEOMETRY_H_INCLUDED

/* INCLUDES */
class vostok::core::noncopyable;
typedef vostok::intrusive_ptr<vostok::render::res_declaration,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_declaration_ptr;
typedef vostok::intrusive_ptr<vostok::render::untyped_buffer,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::untyped_buffer_ptr;
class vostok::render::res_declaration;
class vostok::render::untyped_buffer;

namespace vostok {
namespace render {

struct sphere_geometry : public core::noncopyable {
					sphere_geometry	( const u32 num_sides, const u32 num_rings );

			void	draw			( );

	inline			~sphere_geometry( ) { /* no source */ }

	/* 0x0000 */	/* core::noncopyable */
private:
	/* 0x0000 */	res_declaration_ptr		m_vertext_declaration;
	/* 0x0004 */	untyped_buffer_ptr		m_vertex_buffer;
	/* 0x0008 */	untyped_buffer_ptr		m_index_buffer;
	/* 0x000c */	const u32				m_stride;
	/* 0x0010 */	u32						m_num_indices;
}; // struct sphere_geometry

STATIC_SIZE_ASSERT(sphere_geometry, 0x14);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SPHERE_GEOMETRY_H_INCLUDED
