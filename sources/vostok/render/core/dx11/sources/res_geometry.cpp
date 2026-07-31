#include "pch.h"

#include <vostok/render/core/dx11/res_geometry.h>
#include <vostok/render/core/resource_manager.h>
#include <vostok/render/core/backend.h>

namespace vostok {
namespace render {

s32 res_geometry::compare( res_geometry const& ) const
{
	// claude@NOTE: legacy body diverged - legacy is bool equal over the same four members, not an s32 three-way compare (see legacy remainder note); matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x6e82b0]
	return 0;
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
