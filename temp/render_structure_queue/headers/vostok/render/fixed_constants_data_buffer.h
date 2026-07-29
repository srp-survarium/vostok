////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_FIXED_CONSTANTS_DATA_BUFFER_H_INCLUDED
#define RENDER_FIXED_CONSTANTS_DATA_BUFFER_H_INCLUDED

/* INCLUDES */
struct vostok::render::fixed_constants_data_buffer;

namespace vostok {
namespace render {

struct fixed_constants_data_buffer {
	inline			fixed_constants_data_buffer	( ) { /* no source */ }

	inline	bool	is_overflowing				( u32 arg_0 ) const { /* no source */ }

	/* 0x0000 */	fixed_constants_data_buffer*	next;
private:
	/* 0x0004 */	u8								m_data[1024];
	/* 0x0404 */	u32								m_pos;
}; // struct fixed_constants_data_buffer

STATIC_SIZE_ASSERT(fixed_constants_data_buffer, 0x408);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_FIXED_CONSTANTS_DATA_BUFFER_H_INCLUDED
