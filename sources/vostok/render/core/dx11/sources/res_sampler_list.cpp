// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include <vostok/render/core/res_sampler_list.h>
#include <vostok/render/core/resource_manager.h>
namespace vostok {
namespace render {

res_sampler_list::res_sampler_list( fixed_vector<sampler_slot, 16> const& slots ) :
	m_is_registered( false )
{
	u32 const slots_size = slots.size();
	for (u32 i = 0; i < slots_size; ++i)
	{
		if( slots[i].slot_id != enum_slot_ind_null)
		{
			m_samplers.resize(i+1, (ID3D11SamplerState*)NULL);
			m_names.resize(i+1, fixed_string<32>( ));
			m_samplers[i] = slots[i].state;
			m_names[i] = slots[i].name;
		}
	}
}

void res_sampler_list::rebind( )
{
	for ( u32 i = 0, count = m_samplers.size( ); i < count; ++i )
		m_samplers[i] = resource_manager::ref( ).find_registered_sampler( m_names[i].c_str( ) );
}

void res_sampler_list::destroy_impl( ) const
{
	resource_manager::ref().release( this );
}

s32 res_sampler_list::compare( res_sampler_list const& base ) const
{
	for ( u32 size = std::min( m_samplers.size( ), base.m_samplers.size( ) ), cmp = 0; cmp < size; ++cmp )
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
