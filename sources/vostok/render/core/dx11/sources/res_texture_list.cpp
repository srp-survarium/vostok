#include "pch.h"
#include <vostok/render/core/res_texture_list.h>
#include <vostok/render/core/dx11/texture_slot.h>
#include <vostok/render/core/resource_manager.h>

namespace vostok {
namespace render {

res_texture_list::res_texture_list	( fixed_vector<texture_slot, 128> const & slots) :
	m_is_registered	( false )
{
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
	resource_manager::ref().release( this );
}

// claude@NOTE: the two compare residuals are LTCG calling-convention differences.
s32 res_texture_list::compare( res_texture_list const& base ) const
{
	for ( u32 size = std::min( m_container.size( ), base.m_container.size( ) ), cmp = 0; cmp < size; ++cmp )
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
	for ( u32 size = std::min( m_container.size( ), base.size( ) ), i = 0; i < size; ++i )
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
