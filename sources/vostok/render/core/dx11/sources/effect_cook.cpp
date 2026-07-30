#include "pch.h"
#include <vostok/render/core/effect_cook.h>

namespace vostok {
namespace render {

effect_cook::effect_cook( )
	: resources::unmanaged_cook(
		resources::render_effect_class,
		reuse_true,
		use_current_thread_id,
		use_current_thread_id
	)
{
	// STATE[STUB]
	// FUNCTION BODY[0x758e30]
}

void effect_cook::on_binary_shaders(
	resources::query_result_for_cook*,
	res_effect*,
	effect_compile_data*,
	resources::queries_result&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x758f10]
}

void effect_cook::create_resource(
	resources::query_result_for_cook&,
	const_buffer,
	mutable_buffer
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x759260]
}

void effect_cook::destroy_resource( resources::unmanaged_resource* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x758eb0]
}

mutable_buffer effect_cook::allocate_resource(
	resources::query_result_for_cook&,
	const_buffer,
	bool
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x758ee0]
	return mutable_buffer( );
}

void effect_cook::deallocate_resource( void* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x758e90]
}

} // namespace render
} // namespace vostok
