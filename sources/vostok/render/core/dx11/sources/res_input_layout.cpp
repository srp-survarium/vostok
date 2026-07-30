#include "pch.h"

#include <vostok/render/core/dx11/res_input_layout.h>

namespace vostok {
namespace render {

res_input_layout::res_input_layout(
	res_declaration const* declaration,
	res_signature const* signature
) :
	m_hw_input_layout( 0 ),
	m_declaration( declaration ),
	m_signature( signature ),
	m_is_registered( false )
{
	// STATE[STUB]
	// FUNCTION BODY[0x7a5ce0]
}

res_input_layout::~res_input_layout( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x7a5c90]
}

void res_input_layout::destroy_impl( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x7a5cd0]
}

} // namespace render
} // namespace vostok
