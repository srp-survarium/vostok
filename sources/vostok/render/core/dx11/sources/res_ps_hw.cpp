#include "pch.h"
#include <vostok/render/core/dx11/res_xs_hw.h>

namespace vostok {
namespace render {

template <>
HRESULT res_xs_hw<ps_data>::create_hw_shader(
	ID3D10Blob*,
	ID3D11PixelShader**
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x738ec0]
	return 0;
}

} // namespace render
} // namespace vostok
