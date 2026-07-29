////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_BRANCH_VERTEX_H_INCLUDED
#define RENDER_BRANCH_VERTEX_H_INCLUDED

namespace vostok {
namespace render {

struct branch_vertex {

	/* 0x0000 */	float		m_afCoord[3];
	/* 0x000c */	float		m_afLodCoord[3];
	/* 0x0018 */	float		m_fWindScalarMag;
	/* 0x001c */	float		m_afDiffuseTexCoords[2];
	/* 0x0024 */	float		m_afDetailTexCoords[2];
	/* 0x002c */	u8			m_aucNormal[3];
	/* 0x002f */	u8			m_ucAmbOcc;
	/* 0x0030 */	u8			m_aucTangent[3];
	/* 0x0033 */	u8			m_ucTangentPadding;
	/* 0x0034 */	u8			m_aucWindData[4];
}; // struct branch_vertex

STATIC_SIZE_ASSERT(branch_vertex, 0x38);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_BRANCH_VERTEX_H_INCLUDED
