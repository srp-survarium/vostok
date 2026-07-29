////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SHADER_TYPE_TRAITS_H_INCLUDED
#define RENDER_SHADER_TYPE_TRAITS_H_INCLUDED

namespace vostok {
namespace render {

struct shader_type_traits< 2 > {

	static	inline	pcstr	name( ) { /* no source */ }
	static	inline	pcstr	short_name( ) { /* no source */ }
}; // struct shader_type_traits< 2 >

STATIC_SIZE_ASSERT(shader_type_traits< 2 >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SHADER_TYPE_TRAITS_H_INCLUDED
