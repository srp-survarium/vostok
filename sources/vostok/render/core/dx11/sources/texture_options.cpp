#include "pch.h"
#include <vostok/render/core/texture_options.h>
#include <vostok/render/core/texture_options_cooker.h>

namespace vostok {
namespace render {

texture_options_cooker::texture_options_cooker(
	resources::class_id_enum resource_class,
	resources::cook_base::reuse_enum reuse_type,
	u32 translate_query_thread,
	enum_flags<resources::cook_base::flags_enum> flags
)
	: super( resource_class, reuse_type, translate_query_thread, flags )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5591b0]
}

void texture_options_cooker::delete_resource( resources::resource_base* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5591a0]
}

texture_options_binary_cooker::texture_options_binary_cooker( )
	: super(
		resources::texture_options_binary_class,
		reuse_true,
		use_resource_manager_thread_id
	)
{
	// STATE[STUB]
	// FUNCTION BODY[0x5591d0]
}

void texture_options_binary_cooker::translate_query(
	resources::query_result_for_cook&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x5593c0]
}

void texture_options_binary_cooker::on_binary_config_loaded(
	resources::queries_result&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x559240]
}

void texture_options_binary_cooker::on_lua_options_loaded(
	resources::queries_result&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x559210]
}

} // namespace render
} // namespace vostok
