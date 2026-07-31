#include "pch.h"
#include <vostok/render/core/res_sampler_list.h>
#include <vostok/render/core/resource_manager.h>

namespace vostok {
namespace render {

res_sampler_list::res_sampler_list( fixed_vector<sampler_slot, 16> const& slots )
{
	// FUNCTION BODY[0x739a40]
	u32 const slots_size = slots.size();
	for (u32 i = 0; i < slots_size; ++i)
	{
		if( slots[i].slot_id != enum_slot_ind_null)
		{
			m_samplers.resize(i+1, (ID3D11SamplerState*)NULL);
			m_samplers[i] = slots[i].state;
		}
	}
}

void res_sampler_list::rebind( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x739a00]
}

void res_sampler_list::destroy_impl( ) const
{
	// FUNCTION BODY[0x739b70]
	resource_manager::ref().release( this );
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
