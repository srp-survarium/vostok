////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SHADER_DATA_BASE_H_INCLUDED
#define RENDER_SHADER_DATA_BASE_H_INCLUDED

namespace vostok {
namespace render {

struct shader_data_base {
	inline		shader_data_base( ) { /* no source */ }

	/* 0x0000 */	u16		instruction_count;
}; // struct shader_data_base

STATIC_SIZE_ASSERT(shader_data_base, 0x2);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SHADER_DATA_BASE_H_INCLUDED
