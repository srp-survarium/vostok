#include "pch.h"

#include <vostok/render/core/dx11/res_declaration.h>
#include <vostok/render/core/resource_manager.h>

namespace vostok {
namespace render {

signature_layout_pair::signature_layout_pair( res_declaration const* decl, res_signature const * signature):
signature	(signature)
{
	// FUNCTION BODY[0x7a7430]
	input_layout = resource_manager::ref().create_input_layout( decl, signature);
}

res_declaration::res_declaration(
	D3D11_INPUT_ELEMENT_DESC const* declaration,
	u32 count
) :
	dcl_code( declaration, declaration + count ),
	m_is_registered( false )
{
	// FUNCTION BODY[0x7a74d0]
// 	CHECK_RESULT(device::ref().device()->CreateVertexDeclaration(decl, &m_dcl));
// 	u32 dcl_size = D3DXGetDeclLength(decl)+1;
//	m_dcl_code.assign();
}

void res_declaration::destroy_impl() const
{
	// FUNCTION BODY[0x7a7510]
	resource_manager::ref().release( this );
}

res_declaration::~res_declaration()
{
	// FUNCTION BODY[0x7a7480]
	//	Release vertex layout
//	safe_release(m_dcl);
}

} // namespace render
} // namespace vostok
