#include "pch.h"

#include <vostok/render/core/dx11/res_declaration.h>

namespace vostok {
namespace render {

signature_layout_pair::signature_layout_pair(
	res_declaration const*,
	res_signature const* in_signature
) :
	input_layout( 0 ),
	signature( in_signature )
{
	// STATE[STUB]
	// FUNCTION BODY[0x7a7430]
}

res_declaration::res_declaration(
	D3D11_INPUT_ELEMENT_DESC const* declaration,
	u32 count
) :
	dcl_code( declaration, declaration + count ),
	m_is_registered( false )
{
	// FUNCTION BODY[0x7a74d0]
}

void res_declaration::destroy_impl( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x7a7510]
}

res_declaration::~res_declaration( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x7a7480]
}

} // namespace render
} // namespace vostok
