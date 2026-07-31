#include "pch.h"
#include <vostok/render/core/texture_cook_wrapper.h>
#include <vostok/render/core/texture_cook.h>
#include <vostok/render/core/texture_options_cooker.h>

// claude@NOTE: legacy unconditionally included "texture_converter_cook.h"; the
// canonical carcass has no such header - its types are referenced only inside
// the #ifndef MASTER_GOLD blocks below, which this build strips.

namespace vostok {
namespace render {

void register_texture_cook	( );
void unregister_texture_cook( );

} // namespace render
} // namespace vostok

static vostok::uninitialized_reference< vostok::render::texture_cook_wrapper >	s_texture_wrapper_cook;
static vostok::uninitialized_reference< vostok::render::texture_options_binary_cooker >	s_texture_options_binary_cooker;

#ifndef MASTER_GOLD
	static vostok::uninitialized_reference< vostok::render::texture_converter_cook >	s_texture_converter_cook;
	static vostok::uninitialized_reference< vostok::render::texture_options_lua_cooker>	s_texture_options_lua_cooker;
#endif // #ifndef MASTER_GOLD

static vostok::uninitialized_reference< vostok::render::texture_cook >			s_texture_cook;

void vostok::render::register_texture_cook	( )
{
	// FUNCTION BODY[0x773ad0]
	VOSTOK_CONSTRUCT_REFERENCE	( s_texture_wrapper_cook, texture_cook_wrapper );
	resources::register_cook	( s_texture_wrapper_cook.c_ptr() );

#ifndef MASTER_GOLD
	VOSTOK_CONSTRUCT_REFERENCE	( s_texture_converter_cook, texture_converter_cook );
	resources::register_cook	( s_texture_converter_cook.c_ptr() );

	VOSTOK_CONSTRUCT_REFERENCE	( s_texture_options_lua_cooker, texture_options_lua_cooker );
	resources::register_cook	( s_texture_options_lua_cooker.c_ptr() );
#endif // #ifndef MASTER_GOLD

	VOSTOK_CONSTRUCT_REFERENCE	( s_texture_cook, texture_cook );
	resources::register_cook	( s_texture_cook.c_ptr() );

	VOSTOK_CONSTRUCT_REFERENCE	( s_texture_options_binary_cooker, texture_options_binary_cooker );
	resources::register_cook	( s_texture_options_binary_cooker.c_ptr() );
}

void vostok::render::unregister_texture_cook	( )
{
	// FUNCTION BODY[0x773c30]
	resources::unregister_cook	( resources::texture_wrapper_class );
	VOSTOK_DESTROY_REFERENCE		( s_texture_wrapper_cook );

#ifndef MASTER_GOLD
	resources::unregister_cook	( resources::texture_converter_class );
	VOSTOK_DESTROY_REFERENCE		( s_texture_converter_cook );
#endif // #ifndef MASTER_GOLD

	resources::unregister_cook	( resources::texture_class );
	VOSTOK_DESTROY_REFERENCE		( s_texture_cook );
}
