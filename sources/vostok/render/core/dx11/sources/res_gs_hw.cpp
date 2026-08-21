#include "pch.h"
#include <vostok/render/core/dx11/res_xs_hw.h>
#include <vostok/render/core/device.h>

namespace vostok {
namespace render {

template <>
HRESULT res_xs_hw<gs_data>::create_hw_shader(
	ID3D10Blob* shader_code,
	ID3D11GeometryShader** hardware_shader
)
{
	void const* bytecode_buffer	= shader_code->GetBufferPointer();
	u32 bytecode_buffer_size	= (u32)shader_code->GetBufferSize();

	return device::ref().d3d_device()->CreateGeometryShader ( bytecode_buffer, bytecode_buffer_size, NULL, hardware_shader);
}

} // namespace render
} // namespace vostok
