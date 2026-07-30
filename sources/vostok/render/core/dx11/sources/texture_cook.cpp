#include "pch.h"
#include <vostok/render/core/texture_cook.h>

namespace vostok {
namespace render {

texture_data_resource::texture_data_resource( u32 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x559460]
}

const_buffer texture_data_resource::buffer( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x559490]
	return const_buffer( );
}

texture_cook::texture_cook( )
	: resources::inplace_managed_cook(
		resources::texture_class,
		reuse_false,
		use_current_thread_id
	)
{
	// STATE[STUB]
	// FUNCTION BODY[0x5594b0]
}

u32 texture_cook::calculate_resource_size( u32, u32&, bool )
{
	// STATE[STUB]
	// FUNCTION BODY[0x559470]
	return 0;
}

void texture_cook::create_resource(
	resources::query_result_for_cook&,
	resources::managed_resource_ptr,
	u32,
	u32&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x559520]
}

void texture_cook::destroy_resource( resources::managed_resource* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5594f0]
}

} // namespace render
} // namespace vostok
