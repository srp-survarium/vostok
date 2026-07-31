#include "pch.h"
#include <vostok/render/core/res_texture_list.h>
#include <vostok/render/core/dx11/texture_slot.h>
#include <vostok/render/core/resource_manager.h>

namespace vostok {
namespace render {

s32 res_texture_list::compare(
	fixed_vector<texture_slot, 128> const&
) const
{
	// claude@NOTE: legacy body diverged - legacy is bool equal(texture_slots), not an s32 compare (see legacy remainder note); matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x6e8080]
	return 0;
}

s32 res_texture_list::compare( res_texture_list const& ) const
{
	// claude@NOTE: no legacy ancestor - the list-vs-list overload's legacy equal was already consumed into the carcass header inline; the s32 form is new; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x6e8130]
	return 0;
}

res_texture_list::res_texture_list	( fixed_vector<texture_slot, 128> const & slots) :
	m_is_registered	( false )
{
	// FUNCTION BODY[0x6e81e0]
	u32 const size = slots.size();
	for (u32 i = 0; i < size; ++i)
	{
		if( slots[i].slot_id != enum_slot_ind_null)
		{
			m_container.resize(i+1, res_texture_ptr(NULL));
			m_container[i] = slots[i].texture;
		}
	}
}

void res_texture_list::destroy_impl	() const
{
	// FUNCTION BODY[0x6e82a0]
	resource_manager::ref().release( this );
}

} // namespace render
} // namespace vostok
