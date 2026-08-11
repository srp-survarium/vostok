#ifndef VOSTOK_RENDER_CORE_INDEX_BUFFER_H_INCLUDED
#define VOSTOK_RENDER_CORE_INDEX_BUFFER_H_INCLUDED

#include <vostok/render/core/untyped_buffer.h>

namespace vostok {
namespace render {

class index_buffer {
public:
	explicit index_buffer( u32 size );

	u16* lock( u32 index_count, u32& index_offset );
	void unlock( );

	untyped_buffer& buffer( ) const { return *m_buffer; }
	u32 discard_id( ) const { return m_discard_id; }
	void flush( ) { m_position = m_size; }

private:
	untyped_buffer_ptr	m_buffer;
	u32				m_size;
	u32				m_position;
	u32				m_discard_id;
	u32				m_lock_size;
};

STATIC_SIZE_ASSERT( index_buffer, 0x14 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_INDEX_BUFFER_H_INCLUDED
