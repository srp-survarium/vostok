////////////////////////////////////////////////////////////////////////////
//	Created		: 14.04.2010
//	Author		: Armen Abroyan
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

// HARVEST REMAINDER (2026-07-31): ctor + destroy_impl ported into
// sources/vostok/render/core/dx11/sources/res_texture_list.cpp; the
// list-vs-list `equal` body was filled into the canonical res_texture_list.h
// inline stub. The slots overload below is kept as the surviving ancestor of
// the still-stubbed
//   s32 res_texture_list::compare( fixed_vector<texture_slot, 128> const& ) [0x6e8080]
// (equal -> compare rename; the header inline equal( fixed_vector ... ) stub
// was NOT filled because core/res_texture_list.h only forward-declares
// texture_slot).

#include "pch.h"
#include <vostok/render/core/res_texture_list.h>

namespace vostok {
namespace render {

bool res_texture_list::equal( texture_slots const & base) const
{
	u32 const base_size = base.size();
	if( base_size != m_container.size())
		return false;

	for (u32 i = 0; i < base_size; ++i)
		if (m_container[i] != base[i].texture)
			return false;

	return true;
}

} // namespace render
} // namespace vostok
