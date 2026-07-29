////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RESOURCE_MANAGER_SHADER_SOURCE_H_INCLUDED
#define RENDER_RESOURCE_MANAGER_SHADER_SOURCE_H_INCLUDED

/* INCLUDES */
class vostok::vectora<char const *>;
class vostok::vectora<unsigned char>;

namespace vostok {
namespace render {

struct resource_manager::shader_source {
	inline		shader_source	( ) { /* no source */ }
	inline		~shader_source	( ) { /* no source */ }

	/* 0x0000 */	vectora< u8 >		data;
	/* 0x0010 */	u32					time;
	/* 0x0014 */	vectora< pcstr >	includes;
}; // struct resource_manager::shader_source

STATIC_SIZE_ASSERT(resource_manager::shader_source, 0x24);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RESOURCE_MANAGER_SHADER_SOURCE_H_INCLUDED
