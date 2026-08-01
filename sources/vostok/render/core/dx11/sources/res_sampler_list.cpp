#include "pch.h"
// claude@NOTE: legacy-harvest disposition: stem file consumed; rebind never existed in the corpus - matcher-phase work.
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

s32 res_sampler_list::compare( res_sampler_list const& base ) const
{
	u32 const size = std::min( m_samplers.size( ), base.m_samplers.size( ) );
	for ( u32 cmp = 0; cmp < size; ++cmp )
	{
		if ( m_samplers[cmp] < base.m_samplers[cmp] )
			return -1;

		if ( m_samplers[cmp] > base.m_samplers[cmp] )
			return 1;
	}

	if ( m_samplers.size( ) < base.m_samplers.size( ) )
		return -1;

	return base.m_samplers.size( ) < m_samplers.size( ) ? 1 : 0;
}

s32 res_sampler_list::compare(
	fixed_vector<sampler_slot, 16> const& base
) const
{
	u32 const base_size = base.size( );
	for ( u32 i = 0, count = m_samplers.size( ); i < base_size; ++i )
	{
		if ( i < count )
		{
			if ( m_samplers[i] < base[i].state )
				return -1;

			if ( m_samplers[i] > base[i].state )
				return 1;
		}
		else if ( base[i].slot_id != enum_slot_ind_null )
			return -1;
	}

	return 0;
}

} // namespace render
} // namespace vostok
