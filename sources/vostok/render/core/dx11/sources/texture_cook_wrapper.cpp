#include "pch.h"
#include <vostok/render/core/texture_cook_wrapper.h>

namespace vostok {
namespace render {

texture_cook_wrapper::texture_cook_wrapper( )
	: resources::translate_query_cook(
		resources::texture_wrapper_class,
#ifndef MASTER_GOLD
		reuse_false,
#else // #ifndef MASTER_GOLD
		reuse_true,
#endif // #ifndef MASTER_GOLD
		use_any_thread_id
	)
{
	// STATE[STUB]
	// FUNCTION BODY[0x759980]
}

void texture_cook_wrapper::translate_query(
	resources::query_result_for_cook&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x759b50]
}

void texture_cook_wrapper::query_converted_texture(
	resources::query_result_for_cook*
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x759a90]
}

void texture_cook_wrapper::on_texture_loaded( resources::queries_result& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x759a30]
}

void texture_cook_wrapper::delete_resource( resources::resource_base* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x7599c0]
}

} // namespace render
} // namespace vostok
