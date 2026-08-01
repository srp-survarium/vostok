#ifndef VOSTOK_RENDER_CORE_DX11_SHADER_CONSTANT_BUFFER_INLINE_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_SHADER_CONSTANT_BUFFER_INLINE_H_INCLUDED

namespace vostok {
namespace render {

inline void shader_constant_buffer::set( shader_constant_slot const& slot, void* pointer, u32 )
{
	u32 const data_size = slot.class_id( ) & constant_class_size_mask;
	set_memory( slot.slot_index( ), static_cast<pcstr>( pointer ), data_size );
}

inline void shader_constant_buffer::set( shader_constant_slot const& slot, void* pointer, u32, u32 array_size )
{
	u32 const data_size = slot.class_id( ) & constant_class_size_mask;
	set_memory(
		slot.slot_index( ),
		static_cast<pcstr>( pointer ),
		data_size * array_size
	);
}

inline void shader_constant_buffer::zero( shader_constant_slot const& slot, u32 )
{
	u32 const data_size = slot.class_id( ) & constant_class_size_mask;
	zero_memory( slot.slot_index( ), data_size );
}

inline void shader_constant_buffer::set_memory( u32 const offset, pcstr source, u32 const size )
{
	pcstr const end = static_cast<pstr>( m_buffer_data ) + offset + math::min( size, m_buffer_size - offset );
	u8 difference = 0;
	for ( pstr destination = static_cast<pstr>( m_buffer_data ) + offset; destination != end; ++destination, ++source ) {
		difference |= *destination ^ *source;
		*destination = *source;
	}
	m_changed |= difference != 0;
}

inline void shader_constant_buffer::zero_memory( u32 const offset, u32 const size )
{
	pcstr const end = static_cast<pstr>( m_buffer_data ) + offset + math::min( size, m_buffer_size - offset );
	u8 difference = 0;
	for ( pstr destination = static_cast<pstr>( m_buffer_data ) + offset; destination != end; ++destination ) {
		difference |= *destination;
		*destination = 0;
	}
	m_changed |= difference != 0;
}

inline void* shader_constant_buffer::access( u32 offset )
{
	return offset < m_buffer_size
		? static_cast<char*>( m_buffer_data ) + offset
		: 0;
}

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_SHADER_CONSTANT_BUFFER_INLINE_H_INCLUDED
