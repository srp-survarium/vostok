#include "pch.h"
#include <vostok/render/core/decl_utils.h>

namespace vostok {
namespace render {
namespace decl_utils {

DXGI_FORMAT ConvertVertexFormat( D3DDECLTYPE )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6e7a10]
	return DXGI_FORMAT_UNKNOWN;
}

pcstr ConvertSemantic( D3DDECLUSAGE )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6e79f0]
	return 0;
}

void convert_vertex_declaration(
	vector<D3DVERTEXELEMENT9> const&,
	vector<D3D11_INPUT_ELEMENT_DESC>&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x6e7a30]
}

} // namespace decl_utils
} // namespace render
} // namespace vostok
