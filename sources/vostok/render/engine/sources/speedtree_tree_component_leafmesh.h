#ifndef VOSTOK_RENDER_ENGINE_SPEEDTREE_TREE_COMPONENT_LEAFMESH_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_SPEEDTREE_TREE_COMPONENT_LEAFMESH_H_INCLUDED

#include "speedtree_tree.h"

namespace vostok {
namespace render {

struct leafmesh_vertex {
	float m_afCoord[3];
	float m_fWindScalar;
	float m_afLodCoord[3];
	float m_fWindScalarMag;
	u8 m_aucNormal[3];
	u8 m_ucAmbOcc;
	u8 m_aucTangent[3];
	u8 m_ucWindActiveFlag;
	u8 m_aucWindData[4];
	float m_afDiffuseTexCoords[2];
	float m_afLeafMeshWind[3];
};

STATIC_SIZE_ASSERT( leafmesh_vertex, 0x40 );

class speedtree_tree_component_leafmesh : public speedtree_tree_component {
public:
	explicit speedtree_tree_component_leafmesh( speedtree_tree& parent );

	virtual void set_material( material_ptr material );
	virtual void set_default_material( );

	virtual SpeedTree::EGeometryType get_geometry_type( ) const
	{
		return SpeedTree::GEOMETRY_TYPE_LEAF_MESHES;
	}

	virtual void render( lod_entry const* lod, renderer_context* context );

	virtual enum_vertex_input_type get_vertex_input_type( )
	{
		return null_vertex_input_type;
	}

	void init_index_buffer( SpeedTree::SIndexedTriangles const* lod, vector<u16>& out_indices );
};

STATIC_SIZE_ASSERT( speedtree_tree_component_leafmesh, 0x70 );

} // namespace render
} // namespace vostok

#endif // VOSTOK_RENDER_ENGINE_SPEEDTREE_TREE_COMPONENT_LEAFMESH_H_INCLUDED
