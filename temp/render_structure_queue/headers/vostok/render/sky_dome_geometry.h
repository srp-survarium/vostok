////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SKY_DOME_GEOMETRY_H_INCLUDED
#define RENDER_SKY_DOME_GEOMETRY_H_INCLUDED

/* INCLUDES */
typedef vostok::intrusive_ptr<vostok::render::res_declaration,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_declaration_ptr;
typedef vostok::intrusive_ptr<vostok::render::untyped_buffer,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::untyped_buffer_ptr;
class vostok::render::res_declaration;
class vostok::render::untyped_buffer;

namespace vostok {
namespace render {

struct sky_dome_geometry : public boost::noncopyable {
					sky_dome_geometry	( );

			void	draw				( );

	inline			~sky_dome_geometry	( ) { /* no source */ }

	/* 0x0000 */	/* boost::noncopyable */
private:
	/* 0x0000 */	res_declaration_ptr		m_vertext_declaration;
	/* 0x0004 */	untyped_buffer_ptr		m_vertex_buffer;
	/* 0x0008 */	untyped_buffer_ptr		m_index_buffer;
	/* 0x000c */	const u32				m_stride;
	/* 0x0010 */	u32						m_num_indices;
}; // struct sky_dome_geometry

STATIC_SIZE_ASSERT(sky_dome_geometry, 0x14);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SKY_DOME_GEOMETRY_H_INCLUDED
