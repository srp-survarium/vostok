////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_LEAFMESH_VERTEX_H_INCLUDED
#define RENDER_LEAFMESH_VERTEX_H_INCLUDED

namespace vostok {
namespace render {

struct leafmesh_vertex {

	/* 0x0000 */	float		m_afCoord[3];
	/* 0x000c */	float		m_fWindScalar;
	/* 0x0010 */	float		m_afLodCoord[3];
	/* 0x001c */	float		m_fWindScalarMag;
	/* 0x0020 */	u8			m_aucNormal[3];
	/* 0x0023 */	u8			m_ucAmbOcc;
	/* 0x0024 */	u8			m_aucTangent[3];
	/* 0x0027 */	u8			m_ucWindActiveFlag;
	/* 0x0028 */	u8			m_aucWindData[4];
	/* 0x002c */	float		m_afDiffuseTexCoords[2];
	/* 0x0034 */	float		m_afLeafMeshWind[3];
}; // struct leafmesh_vertex

STATIC_SIZE_ASSERT(leafmesh_vertex, 0x40);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_LEAFMESH_VERTEX_H_INCLUDED
