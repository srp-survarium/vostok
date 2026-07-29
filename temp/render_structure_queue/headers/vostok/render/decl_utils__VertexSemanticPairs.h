////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_DECL_UTILS_VERTEXSEMANTICPAIRS_H_INCLUDED
#define RENDER_DECL_UTILS_VERTEXSEMANTICPAIRS_H_INCLUDED

namespace vostok {
namespace render {

enum _D3DDECLUSAGE
{
	D3DDECLUSAGE_POSITION			= 0x0,
	D3DDECLUSAGE_BLENDWEIGHT		= 0x1,
	D3DDECLUSAGE_BLENDINDICES		= 0x2,
	D3DDECLUSAGE_NORMAL				= 0x3,
	D3DDECLUSAGE_PSIZE				= 0x4,
	D3DDECLUSAGE_TEXCOORD			= 0x5,
	D3DDECLUSAGE_TANGENT			= 0x6,
	D3DDECLUSAGE_BINORMAL			= 0x7,
	D3DDECLUSAGE_TESSFACTOR			= 0x8,
	D3DDECLUSAGE_POSITIONT			= 0x9,
	D3DDECLUSAGE_COLOR				= 0xa,
	D3DDECLUSAGE_FOG				= 0xb,
	D3DDECLUSAGE_DEPTH				= 0xc,
	D3DDECLUSAGE_SAMPLE				= 0xd,
};

struct decl_utils::VertexSemanticPairs {

	/* 0x0000 */	_D3DDECLUSAGE		m_dx9Semantic;
	/* 0x0004 */	pcstr				m_dx10Semantic;
}; // struct decl_utils::VertexSemanticPairs

STATIC_SIZE_ASSERT(decl_utils::VertexSemanticPairs, 0x8);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_DECL_UTILS_VERTEXSEMANTICPAIRS_H_INCLUDED
