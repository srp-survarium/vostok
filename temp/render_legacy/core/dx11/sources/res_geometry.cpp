////////////////////////////////////////////////////////////////////////////
//	Created		: 20.04.2010
//	Author		: Armen Abroyan
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

// HARVEST REMAINDER (2026-07-31): dtor + destroy_impl + apply ported into
// sources/vostok/render/core/dx11/sources/res_geometry.cpp; canonical
// res_geometry.h already carries equal( ) inline as compare( other ) == 0.
// The memberwise body below is kept as the surviving ancestor of the
// still-stubbed
//   s32 res_geometry::compare( res_geometry const& ) const [0x6e82b0]
// (equal -> compare rename; matcher-phase work).

#include "pch.h"
#include <vostok/render/core/res_geometry.h>

namespace vostok {
namespace render {

bool res_geometry::equal(const res_geometry& other) const
{
	return m_vb==other.m_vb && m_ib==other.m_ib &&
		m_dcl==other.m_dcl && m_vb_stride==other.m_vb_stride;
}

} // namespace render
} // namespace vostok
