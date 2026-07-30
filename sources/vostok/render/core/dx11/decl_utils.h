#ifndef VOSTOK_RENDER_CORE_DX11_DECL_UTILS_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_DECL_UTILS_H_INCLUDED

#include <d3d9types.h>
#include <vostok/render/core/memory.h>
#include <vostok/render/core/render_include.h>

namespace vostok {
namespace render {
namespace decl_utils {

struct VertexFormatPairs {
	D3DDECLTYPE m_dx9_format;
	DXGI_FORMAT m_dx11_format;
};

struct VertexSemanticPairs {
	D3DDECLUSAGE m_dx9_semantic;
	pcstr m_dx11_semantic;
};

DXGI_FORMAT ConvertVertexFormat( D3DDECLTYPE format );
pcstr ConvertSemantic( D3DDECLUSAGE semantic );

void convert_vertex_declaration(
	vector<D3DVERTEXELEMENT9> const& input,
	vector<D3D11_INPUT_ELEMENT_DESC>& output
);

STATIC_SIZE_ASSERT( VertexFormatPairs, 0x8 );
STATIC_SIZE_ASSERT( VertexSemanticPairs, 0x8 );

} // namespace decl_utils
} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_DECL_UTILS_H_INCLUDED
