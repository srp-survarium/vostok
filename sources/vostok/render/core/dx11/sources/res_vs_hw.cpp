#include "pch.h"
#include <vostok/render/core/dx11/res_xs_hw.h>
#include <vostok/render/core/device.h>

namespace vostok {
namespace render {

template <>
HRESULT res_xs_hw<vs_data>::create_hw_shader(
	ID3D10Blob* shader_code,
	ID3D11VertexShader** hardware_shader
)
{
	// FUNCTION BODY[0x7a7310]
	void const* bytecode_buffer	= shader_code->GetBufferPointer();
	u32 bytecode_buffer_size	= (u32)shader_code->GetBufferSize();

	m_shader_data.signature = resource_manager::ref().create_signature( shader_code);

	return device::ref().d3d_device()->CreateVertexShader	( bytecode_buffer, bytecode_buffer_size, NULL, hardware_shader);
}

} // namespace render
} // namespace vostok
