#include "pch.h"
#include <vostok/render/core/dx11/res_xs_hw.h>

namespace vostok {
namespace render {

template <>
HRESULT res_xs_hw<vs_data>::create_hw_shader(
	ID3D10Blob*,
	ID3D11VertexShader**
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x7a7310]
	return 0;
}

} // namespace render
} // namespace vostok
