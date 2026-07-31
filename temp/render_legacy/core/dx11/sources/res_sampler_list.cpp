////////////////////////////////////////////////////////////////////////////
//	Created		: 06.04.2010
//	Author		: Armen Abroyan
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/render/core/res_sampler_list.h>
#include <vostok/render/core/resource_manager.h>

namespace vostok {
namespace render {

// REMAINDER: equal x2 = ancestor evidence for the canonical compare x2
// (equal->compare s32 rename; 3-way bodies from 0x739920/0x739860); rebind
// is target-new.



bool res_sampler_list::equal(const res_sampler_list& base) const
{
	if (m_samplers.size() != base.m_samplers.size())
		return false;

	for (u32 cmp = 0, count = m_samplers.size(); cmp < count; ++cmp)
	{
		if (m_samplers[cmp] != base.m_samplers[cmp])	return false;
	}

	return true;
}

bool res_sampler_list::equal( sampler_slots const & base) const
{
	u32 const base_size = base.size();
	for (u32 i = 0, count = m_samplers.size(); i < base_size; ++i)
	{
		if( i < count)
		{
			if (m_samplers[i] != base[i].state)	
				return false;
		}
		else if( base[i].slot_id != enum_slot_ind_null)
			return false;
	}

	return true;
}


// res_sampler_list::res_sampler_list()
// {
// //	ZeroMemory( &m_samplers, sizeof(m_samplers));
// }


} // namespace render
} // namespace vostok
