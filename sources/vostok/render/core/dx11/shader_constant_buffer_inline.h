#ifndef VOSTOK_RENDER_CORE_DX11_SHADER_CONSTANT_BUFFER_INLINE_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_SHADER_CONSTANT_BUFFER_INLINE_H_INCLUDED

namespace vostok {
namespace render {

inline void shader_constant_buffer::set_memory( u32 const offset, pcstr src_ptr, u32 const size )
{
	ASSERT( static_cast<int>( size ) <= static_cast<int>( m_buffer_size ) - offset );
	pcstr const end = static_cast<pstr>( m_buffer_data ) + offset
		+ (static_cast<int>( size ) <= static_cast<int>( m_buffer_size ) - offset
			? size : static_cast<int>( m_buffer_size ) - offset);

	u8 diff = 0;
	for ( pstr ptr = static_cast<pstr>( m_buffer_data ) + offset; ptr != end; ++ptr, ++src_ptr ) {
		diff |= *ptr ^ *src_ptr;
		*ptr = *src_ptr;
	}

	m_changed |= diff > 0;
}

inline void shader_constant_buffer::zero_memory( u32 const offset, u32 const size )
{
	ASSERT( static_cast<int>( size ) <= static_cast<int>( m_buffer_size ) - offset );
	pstr const end = static_cast<pstr>( m_buffer_data ) + offset
		+ (static_cast<int>( size ) <= static_cast<int>( m_buffer_size ) - offset
			? size : static_cast<int>( m_buffer_size ) - offset);

	u8 diff = 0;
	for ( pstr ptr = static_cast<pstr>( m_buffer_data ) + offset; ptr != end; ++ptr ) {
		diff |= *ptr ^ 0;
		*ptr = 0;
	}

	m_changed |= diff > 0;
}

inline void* shader_constant_buffer::access( u32 offset )
{
	ASSERT( offset < m_buffer_size );
	return offset < m_buffer_size ? static_cast<pstr>( m_buffer_data ) + offset : 0;
}

inline void shader_constant_buffer::set( shader_constant_slot const& slot, void* pointer, u32 size )
{
	u32 data_size = slot.class_id( ) & constant_class_size_mask;
	ASSERT_U( size >= data_size && slot.array_size( ) == 1 );
	set_memory( slot.slot_index( ) * line_size, static_cast<pstr>( pointer ), data_size );
}

inline void shader_constant_buffer::set(
	shader_constant_slot const& slot,
	void* pointer,
	u32 size,
	u32 array_size
)
{
	u32 data_size = slot.class_id( ) & constant_class_size_mask;
	ASSERT_U( size == data_size && slot.array_size( ) >= array_size && slot.array_size( ) > 0 );
	set_memory( slot.slot_index( ) * line_size, static_cast<pstr>( pointer ), data_size * slot.array_size( ) );
}

inline void shader_constant_buffer::zero( shader_constant_slot const& slot, u32 size )
{
	u32 data_size = slot.class_id( ) & constant_class_size_mask;
	ASSERT_U( size >= data_size );
	zero_memory( slot.slot_index( ) * line_size, data_size );
}

template < typename T >
inline void shader_constant_buffer::set_typed( shader_constant_slot const& slot, T const& value )
{
	set( slot, const_cast<T*>( &value ), constant_type_traits<T>::size );
}

template < typename T >
inline void shader_constant_buffer::set_typed( shader_constant_slot const& slot, T const* values, u32 array_size )
{
	set( slot, const_cast<T*>( values ), constant_type_traits<T>::size, array_size );
}

inline void shader_constant_buffer::set( shader_constant const& constant )
{
	set(
		constant.slot( ),
		constant.source( ).pointer( ),
		constant.source( ).size( ),
		constant.slot( ).array_size( )
	);
}

inline void shader_constant_buffer::zero( shader_constant const& constant )
{
	zero( constant.slot( ), constant.source( ).size( ) );
}

inline ID3D11Buffer* shader_constant_buffer::hardware_buffer( ) const
{
	return m_hardware_buffer;
}

inline fixed_string<64> const& shader_constant_buffer::name( ) const
{
	return m_name;
}

inline enum_shader_type& shader_constant_buffer::dest( )
{
	return m_dest;
}

inline enum_shader_type const& shader_constant_buffer::dest( ) const
{
	return m_dest;
}

inline D3D_CBUFFER_TYPE shader_constant_buffer::type( ) const
{
	return m_type;
}

inline u32 shader_constant_buffer::size( ) const
{
	return m_buffer_size;
}

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_SHADER_CONSTANT_BUFFER_INLINE_H_INCLUDED
