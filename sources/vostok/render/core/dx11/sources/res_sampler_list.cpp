#include "pch.h"
#include <vostok/render/core/res_sampler_list.h>

namespace vostok {
namespace render {

res_sampler_list::res_sampler_list( fixed_vector<sampler_slot, 16> const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x739a40]
}

void res_sampler_list::rebind( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x739a00]
}

void res_sampler_list::destroy_impl( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x739b70]
}

s32 res_sampler_list::compare( res_sampler_list const& ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x739920]
	return 0;
}

s32 res_sampler_list::compare(
	fixed_vector<sampler_slot, 16> const&
) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x739860]
	return 0;
}

} // namespace render
} // namespace vostok
