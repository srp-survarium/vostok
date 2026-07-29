////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_INDEX_BUFFER_H_INCLUDED
#define RENDER_INDEX_BUFFER_H_INCLUDED

/* INCLUDES */
typedef vostok::intrusive_ptr<vostok::render::untyped_buffer,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::untyped_buffer_ptr;
class vostok::render::untyped_buffer;

namespace vostok {
namespace render {

class index_buffer {
public:
			explicit			index_buffer	( u32 size );

			u16*				lock			( u32 i_count, u32& i_offset );

			void				unlock			( );

	inline	untyped_buffer&		buffer			( ) const { /* no source */ }

	inline	u32					discard_id		( ) const { /* no source */ }

	inline	void				flush			( ) { /* no source */ }

	inline						~index_buffer	( ) { /* no source */ }

private:
	/* 0x0000 */	untyped_buffer_ptr		m_buffer;
	/* 0x0004 */	u32						m_size;
	/* 0x0008 */	u32						m_position;
	/* 0x000c */	u32						m_discard_id;
	/* 0x0010 */	u32						m_lock_size;
}; // class index_buffer

STATIC_SIZE_ASSERT(index_buffer, 0x14);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_INDEX_BUFFER_H_INCLUDED
