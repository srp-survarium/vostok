#ifndef VOSTOK_RENDER_CORE_VERTEX_BUFFER_H_INCLUDED
#define VOSTOK_RENDER_CORE_VERTEX_BUFFER_H_INCLUDED

#include <vostok/render/core/untyped_buffer.h>

namespace vostok {
namespace render {

class vertex_buffer {
public:
	explicit vertex_buffer( u32 size );

	untyped_buffer& buffer( ) const { return *m_buffer; }
	u32 discard_id( ) const { return m_discard_id; }
	u32 size( ) const { return m_size; }
	void flush( ) { m_position = m_size; }

	template < typename VertexType >
	VertexType* lock( u32 vertex_count, u32& vertex_offset )
	{
		return static_cast< VertexType* >(
			lock( vertex_count, sizeof( VertexType ), vertex_offset )
		);
	}

	template < typename VertexType >
	void lock( u32 vertex_count, VertexType** buffer, u32& vertex_offset )
	{
		*buffer = lock< VertexType >( vertex_count, vertex_offset );
	}

	void* lock( u32 vertex_count, u32 vertex_stride, u32& vertex_offset );
	void unlock( );

private:
	untyped_buffer_ptr	m_buffer;
	u32				m_size;
	u32				m_position;
	u32				m_discard_id;
	u32				m_lock_count;
	u32				m_lock_stride;
};

STATIC_SIZE_ASSERT( vertex_buffer, 0x18 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_VERTEX_BUFFER_H_INCLUDED
