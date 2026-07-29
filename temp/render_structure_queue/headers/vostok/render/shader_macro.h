////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SHADER_MACRO_H_INCLUDED
#define RENDER_SHADER_MACRO_H_INCLUDED

/* INCLUDES */
class vostok::fixed_string<256>;
class vostok::fs_new::virtual_path_string;

namespace vostok {
namespace render {

struct shader_macro {
	inline				shader_macro( ) { /* no source */ }
			explicit	shader_macro( shader_macro const& __that );

	/* 0x0000 */	fs_new::virtual_path_string		name;
	/* 0x0114 */	fixed_string< 256 >				definition;
}; // struct shader_macro

STATIC_SIZE_ASSERT(shader_macro, 0x220);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SHADER_MACRO_H_INCLUDED
