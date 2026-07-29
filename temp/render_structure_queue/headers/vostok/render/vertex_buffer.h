////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VERTEX_BUFFER_H_INCLUDED
#define RENDER_VERTEX_BUFFER_H_INCLUDED

/* INCLUDES */
typedef vostok::intrusive_ptr<vostok::render::untyped_buffer,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::untyped_buffer_ptr;
class vostok::render::untyped_buffer;

namespace vostok {
namespace render {

class vertex_buffer {
public:
			explicit			vertex_buffer	( u32 size );

	inline	untyped_buffer&		buffer			( ) const { /* no source */ }

	inline	u32					discard_id		( ) const { /* no source */ }

	inline	u32					size			( ) const { /* no source */ }

	inline	void				flush			( ) { /* no source */ }

			void*				lock			( u32 v_count, u32 v_stride, u32& v_offset );

			void				unlock			( );

	inline						~vertex_buffer	( ) { /* no source */ }

private:
	/* 0x0000 */	untyped_buffer_ptr		m_buffer;
	/* 0x0004 */	u32						m_size;
	/* 0x0008 */	u32						m_position;
	/* 0x000c */	u32						m_discard_id;
	/* 0x0010 */	u32						m_lock_count;
	/* 0x0014 */	u32						m_lock_stride;
}; // class vertex_buffer

STATIC_SIZE_ASSERT(vertex_buffer, 0x18);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VERTEX_BUFFER_H_INCLUDED
