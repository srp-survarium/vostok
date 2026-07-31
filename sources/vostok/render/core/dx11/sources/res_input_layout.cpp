#include "pch.h"

#include <vostok/render/core/dx11/res_input_layout.h>
#include <vostok/render/core/dx11/res_signature.h>
#include <vostok/render/core/device.h>
#include <vostok/render/core/dx11/res_declaration.h>
#include "com_utils.h"
#include <vostok/render/core/resource_manager.h>

namespace vostok {
namespace render {

res_input_layout::res_input_layout(
	res_declaration const* declaration,
	res_signature const* signature
) :
	m_declaration( declaration ),
	m_signature( signature ),
	m_is_registered( false )
{
	// FUNCTION BODY[0x7a5ce0]
	m_hw_input_layout = NULL;
	ID3DBlob * hw_singiture = signature->hw_signature();
	device::ref().d3d_device()->CreateInputLayout( &m_declaration->dcl_code[0], m_declaration->dcl_code.size(), hw_singiture ->GetBufferPointer(), hw_singiture->GetBufferSize(), &m_hw_input_layout);
	ASSERT( m_hw_input_layout);
}

res_input_layout::~res_input_layout( )
{
	// FUNCTION BODY[0x7a5c90]
	safe_release( m_hw_input_layout);
}

void res_input_layout::destroy_impl( ) const
{
	// FUNCTION BODY[0x7a5cd0]
	resource_manager::ref().release( this );
}

} // namespace render
} // namespace vostok
