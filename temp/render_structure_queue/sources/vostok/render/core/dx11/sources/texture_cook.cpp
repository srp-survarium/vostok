////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\core\dx11\sources\texture_cook.h"

namespace vostok {
namespace render {

// STATE[STUB]
explicit texture_data_resource::texture_data_resource( const u32 buffer_storage_size )
{
	// FUNCTION BODY[0x559460]: 0
	// ******
}

// STATE[STUB]
const_buffer texture_data_resource::buffer( ) const
{
	// FUNCTION BODY[0x559490]: 1
	// <0x559490>|0x000|+0x010:'24'
	// ******
}

// STATE[STUB]
 texture_cook::texture_cook( )
{
	// FUNCTION BODY[0x5594b0]: 0
	// ******
}

// STATE[STUB]
u32 texture_cook::calculate_resource_size( u32 file_size, u32& out_offset_to_file, bool file_exist )
{
	return 0;

	// FUNCTION BODY[0x559470]: 5
	// <0>
	// <1>
	// <0x559470>|0x000|+0x00a:'40'
	// <0x55947a>|0x00a|+0x007:'41'
	// <0>
	// ******
}

// STATE[STUB]
void texture_cook::create_resource(
	resources::query_result_for_cook&		in_out_query,
	resources::managed_resource_ptr			in_out_resource,
	u32										raw_file_size,
	u32&									out_final_resource_size
)
{
	// LOCALS
	// resources::pinned_ptr_mutable< texture_data_resource > managed_typed_ptr
	// ******

	// FUNCTION BODY[0x559520]: 4
	// <0x559524>|0x004|+0x025:'52'
	// <0x559549>|0x029|+0x00e:'53'
	// <0x559557>|0x037|+0x004:'54'
	// <0x55955b>|0x03b|+0x014:'55'
	// ******
}

// STATE[STUB]
void texture_cook::destroy_resource( resources::managed_resource* const dying_resource )
{
	// LOCALS
	// resources::pinned_ptr_mutable< texture_data_resource > managed_typed_ptr
	// ******

	// FUNCTION BODY[0x5594f0]: 2
	// <0x5594f0>|0x000|+0x020:'60'
	// <0>
	// ******
}

} // namespace render
} // namespace vostok
