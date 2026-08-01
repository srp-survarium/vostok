#ifndef VOSTOK_RENDER_CORE_SHADER_CONSTANT_SLOT_INLINE_H_INCLUDED
#define VOSTOK_RENDER_CORE_SHADER_CONSTANT_SLOT_INLINE_H_INCLUDED

namespace vostok {
namespace render {

inline shader_constant_slot::shader_constant_slot( ) :
	m_class_id( static_cast<u16>( -1 ) ),
	m_array_size( 0 ),
	m_buffer_index( slot_dest_buffer_null ),
	m_slot_index( static_cast<u16>( -1 ) )
{
}

inline bool operator==( shader_constant_slot const& left, shader_constant_slot const& right )
{
	return left.value( ) == right.value( );
}

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_SHADER_CONSTANT_SLOT_INLINE_H_INCLUDED
