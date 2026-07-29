////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SHADER_FILE_INFO_H_INCLUDED
#define RENDER_SHADER_FILE_INFO_H_INCLUDED

namespace vostok {
namespace render {

struct shader_file_info {
	inline		shader_file_info( ) { /* no source */ }

	/* 0x0000 */	char	name[260];
	/* 0x0104 */	u32		time;
}; // struct shader_file_info

STATIC_SIZE_ASSERT(shader_file_info, 0x108);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SHADER_FILE_INFO_H_INCLUDED
