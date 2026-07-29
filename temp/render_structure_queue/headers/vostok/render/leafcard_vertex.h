////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_LEAFCARD_VERTEX_H_INCLUDED
#define RENDER_LEAFCARD_VERTEX_H_INCLUDED

namespace vostok {
namespace render {

struct leafcard_vertex {

	/* 0x0000 */	float		m_afCardCenter[3];
	/* 0x000c */	float		m_fWindScalar;
	/* 0x0010 */	u8			m_aucNormal[3];
	/* 0x0013 */	u8			m_ucAmbOcc;
	/* 0x0014 */	u8			m_aucTangent[3];
	/* 0x0017 */	u8			m_ucTangentPadding;
	/* 0x0018 */	u8			m_aucWindData[4];
	/* 0x001c */	float		m_afCardCorner[2];
	/* 0x0024 */	float		m_fLodScale;
	/* 0x0028 */	float		m_fWindScalarMag;
	/* 0x002c */	float		m_afDiffuseTexCoords[2];
	/* 0x0034 */	float		m_fPlanarOffset;
	/* 0x0038 */	float		m_fShadowOffset;
}; // struct leafcard_vertex

STATIC_SIZE_ASSERT(leafcard_vertex, 0x3C);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_LEAFCARD_VERTEX_H_INCLUDED
