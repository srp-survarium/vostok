////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SHADER_CONSTANT_SLOT_H_INCLUDED
#define RENDER_SHADER_CONSTANT_SLOT_H_INCLUDED

namespace vostok {
namespace render {

class shader_constant_slot {
public:
	inline			shader_constant_slot( ) { /* no source */ }

	inline	u16		class_id			( ) const { /* no source */ }

	inline	u16		array_size			( ) const { /* no source */ }

	inline	u16		buffer_index		( ) const { /* no source */ }
	inline	u16		slot_index			( ) const { /* no source */ }

	inline	u64		value				( ) const { /* no source */ }

	/* 0x0000 */	u16		m_class_id;
	/* 0x0002 */	u16		m_array_size;
	/* 0x0004 */	u16		m_buffer_index;
	/* 0x0006 */	u16		m_slot_index;
	/* 0x0000 */	u64		m_value;
}; // class shader_constant_slot

STATIC_SIZE_ASSERT(shader_constant_slot, 0x8);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SHADER_CONSTANT_SLOT_H_INCLUDED
