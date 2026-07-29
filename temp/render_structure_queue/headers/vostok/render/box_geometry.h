////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_BOX_GEOMETRY_H_INCLUDED
#define RENDER_BOX_GEOMETRY_H_INCLUDED

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

struct box_geometry : public core::noncopyable {
					box_geometry	( );

			void	draw			( );

	inline			~box_geometry	( ) { /* no source */ }

	/* 0x0000 */	/* core::noncopyable */
private:
	/* 0x0000 */	res_declaration_ptr		m_vertext_declaration;
	/* 0x0004 */	untyped_buffer_ptr		m_vertex_buffer;
	/* 0x0008 */	untyped_buffer_ptr		m_index_buffer;
	/* 0x000c */	const u32				m_stride;
}; // struct box_geometry

STATIC_SIZE_ASSERT(box_geometry, 0x10);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_BOX_GEOMETRY_H_INCLUDED
