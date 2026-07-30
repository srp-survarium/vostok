#include "pch.h"
#include <vostok/render/core/dx11/res_xs_hw.h>

namespace vostok {
namespace render {

template <>
HRESULT res_xs_hw<gs_data>::create_hw_shader(
	ID3D10Blob*,
	ID3D11GeometryShader**
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x738f00]
	return 0;
}

} // namespace render
} // namespace vostok
