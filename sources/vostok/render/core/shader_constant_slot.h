#ifndef VOSTOK_RENDER_CORE_SHADER_CONSTANT_SLOT_H_INCLUDED
#define VOSTOK_RENDER_CORE_SHADER_CONSTANT_SLOT_H_INCLUDED

namespace vostok {
namespace render {

enum {
	slot_dest_buffer_null = static_cast<u16>( -1 ),
};

class shader_constant_slot {
public:
	shader_constant_slot( );

	u16 class_id( ) const { return m_class_id; }
	u16 array_size( ) const { return m_array_size; }
	u16 buffer_index( ) const { return m_buffer_index; }
	u16 slot_index( ) const { return m_slot_index; }
	u64 value( ) const { return m_value; }

	union {
#pragma warning( push )
#pragma warning( disable : 4201 )
		struct {
			u16 m_class_id;
			u16 m_array_size;
			u16 m_buffer_index;
			u16 m_slot_index;
		};
#pragma warning( pop )
		u64 m_value;
	};
};

inline bool operator==( shader_constant_slot const& left, shader_constant_slot const& right );

} // namespace render
} // namespace vostok

#include <vostok/render/core/shader_constant_slot_inline.h>

#endif // #ifndef VOSTOK_RENDER_CORE_SHADER_CONSTANT_SLOT_H_INCLUDED
