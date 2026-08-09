#include "pch.h"

#include <vostok/render/core/dx11/res_geometry.h>
#include <vostok/render/core/resource_manager.h>
#include <vostok/render/core/backend.h>

namespace vostok {
namespace render {

s32 res_geometry::compare( res_geometry const& other ) const
{
	if ( m_vb < other.m_vb )
		return -1;

	if ( m_vb > other.m_vb )
		return 1;

	if ( m_ib < other.m_ib )
		return -1;

	if ( m_ib > other.m_ib )
		return 1;

	if ( m_dcl < other.m_dcl )
		return -1;

	if ( m_dcl > other.m_dcl )
		return 1;

	if ( m_vb_stride < other.m_vb_stride )
		return -1;

	return other.m_vb_stride < m_vb_stride ? 1 : 0;
}

void res_geometry::apply()
{
	// FUNCTION BODY[0x6e8300]
	backend::ref().set_declaration( m_dcl.c_ptr());
	backend::ref().set_vb( m_vb.c_ptr(), m_vb_stride);
	backend::ref().set_ib( m_ib.c_ptr());
}

res_geometry::~res_geometry()
{
	// FUNCTION BODY[0x6e83c0]
}

void res_geometry::destroy_impl() const
{
	// FUNCTION BODY[0x6e8420]
	resource_manager::ref().release( this );
}

} // namespace render
} // namespace vostok
