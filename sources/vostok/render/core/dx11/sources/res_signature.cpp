#include "pch.h"

#include <vostok/render/core/dx11/res_signature.h>
#include "com_utils.h"
#include <vostok/render/core/resource_manager.h>

namespace vostok {
namespace render {

res_signature::~res_signature( )
{
	// FUNCTION BODY[0x558840]
	safe_release(m_signature);
}

void res_signature::destroy_impl( ) const
{
	// FUNCTION BODY[0x558860]
	resource_manager::ref().release( this );
}

} // namespace render
} // namespace vostok
