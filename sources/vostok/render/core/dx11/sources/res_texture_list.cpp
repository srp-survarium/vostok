#include "pch.h"
#include <vostok/render/core/res_texture_list.h>
#include <vostok/render/core/dx11/texture_slot.h>

namespace vostok {
namespace render {

res_texture_list::res_texture_list( fixed_vector<texture_slot, 128> const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6e81e0]
}

void res_texture_list::destroy_impl( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x6e82a0]
}

s32 res_texture_list::compare( res_texture_list const& ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x6e8130]
	return 0;
}

s32 res_texture_list::compare(
	fixed_vector<texture_slot, 128> const&
) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x6e8080]
	return 0;
}

} // namespace render
} // namespace vostok
