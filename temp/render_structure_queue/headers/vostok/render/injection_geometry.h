////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_INJECTION_GEOMETRY_H_INCLUDED
#define RENDER_INJECTION_GEOMETRY_H_INCLUDED

/* INCLUDES */
typedef vostok::intrusive_ptr<vostok::render::res_declaration,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_declaration_ptr;
typedef vostok::intrusive_ptr<vostok::render::untyped_buffer,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::untyped_buffer_ptr;
class vostok::render::res_declaration;
class vostok::render::untyped_buffer;

namespace vostok {
namespace render {

struct injection_geometry : public boost::noncopyable {
			explicit	injection_geometry	( u32 rsm_size );
						injection_geometry	( u32 rsm_size_x, u32 rsm_size_y );

			void		draw				( );

			void		prepare				( u32 rsm_size_x, u32 rsm_size_y );

	inline				~injection_geometry	( ) { /* no source */ }

	/* 0x0000 */	/* boost::noncopyable */
private:
	/* 0x0000 */	res_declaration_ptr		m_vertext_declaration;
	/* 0x0004 */	untyped_buffer_ptr		m_vertex_buffer;
	/* 0x0008 */	u32						m_num_points;
	/* 0x000c */	const u32				m_stride;
	/* 0x0010 */	u32						m_rsm_size_x;
	/* 0x0014 */	u32						m_rsm_size_y;
}; // struct injection_geometry

STATIC_SIZE_ASSERT(injection_geometry, 0x18);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_INJECTION_GEOMETRY_H_INCLUDED
