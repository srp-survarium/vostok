// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_CORE_DX11_TEXTURE_OPTIONS_COOKER_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_TEXTURE_OPTIONS_COOKER_H_INCLUDED
#include <vostok/resources_cook_classes.h>

namespace vostok {

namespace resources {
class queries_result;
} // namespace resources

namespace render {

struct texture_options_cooker : public resources::translate_query_cook {
	typedef resources::translate_query_cook super;

	texture_options_cooker(
		resources::class_id_enum resource_class,
		resources::cook_base::reuse_enum reuse_type,
		u32 translate_query_thread,
		enum_flags<resources::cook_base::flags_enum> flags = 0
	);

	virtual void delete_resource( resources::resource_base* resource );
};

struct texture_options_binary_cooker : public texture_options_cooker {
	typedef texture_options_cooker super;

	texture_options_binary_cooker( );
	virtual void translate_query( resources::query_result_for_cook& parent );

private:
	void on_lua_options_loaded( resources::queries_result& result );
	void on_binary_config_loaded( resources::queries_result& result );
};

STATIC_SIZE_ASSERT( texture_options_cooker, 0x20 );
STATIC_SIZE_ASSERT( texture_options_binary_cooker, 0x20 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_TEXTURE_OPTIONS_COOKER_H_INCLUDED
