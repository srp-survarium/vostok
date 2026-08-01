#include "pch.h"
#include <vostok/render/core/res_texture_list.h>
#include <vostok/render/core/dx11/texture_slot.h>
#include <vostok/render/core/resource_manager.h>

namespace vostok {
namespace render {

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

// claude@NOTE: both `compare` overloads are LTCG-convention walls, not shape work. The target
// takes `this` in edi and returns `ret 4`; our build puts `this` on the stack and returns
// `ret 8`, which is what makes it spill the loop bound and reload it (`mov edx,[esp+18h]`) -
// that reload is a 1-instruction fall-through block, and it is the whole reason
// `sema sweep` files these as TOPOLOGY: contracting it makes the two CFGs isomorphic.
s32 res_texture_list::compare( res_texture_list const& base ) const
{
	u32 const size = std::min( m_container.size( ), base.m_container.size( ) );
	for ( u32 cmp = 0; cmp < size; ++cmp )
	{
		if ( m_container[cmp] < base.m_container[cmp] )
			return -1;

		if ( m_container[cmp] > base.m_container[cmp] )
			return 1;
	}

	if ( m_container.size( ) < base.m_container.size( ) )
		return -1;

	return base.m_container.size( ) < m_container.size( ) ? 1 : 0;
}

s32 res_texture_list::compare(
	fixed_vector<texture_slot, 128> const& base
) const
{
	u32 const size = std::min( m_container.size( ), base.size( ) );
	for ( u32 i = 0; i < size; ++i )
	{
		if ( m_container[i] < base[i].texture )
			return -1;

		if ( m_container[i] > base[i].texture )
			return 1;
	}

	if ( m_container.size( ) < base.size( ) )
		return -1;

	return base.size( ) < m_container.size( ) ? 1 : 0;
}

} // namespace render
} // namespace vostok
