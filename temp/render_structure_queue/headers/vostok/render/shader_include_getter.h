////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SHADER_INCLUDE_GETTER_H_INCLUDED
#define RENDER_SHADER_INCLUDE_GETTER_H_INCLUDED

namespace vostok {
namespace render {

struct shader_include_getter {
	virtual	pcstr		get_shader_include		( pcstr arg_0 ) const { /* no source */ }

	inline				shader_include_getter	( ) { /* no source */ }
}; // struct shader_include_getter

STATIC_SIZE_ASSERT(shader_include_getter, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SHADER_INCLUDE_GETTER_H_INCLUDED
