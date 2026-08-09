#ifndef VOSTOK_RENDER_ENGINE_SPEEDTREE_TREE_COMPONENT_LEAFCARD_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_SPEEDTREE_TREE_COMPONENT_LEAFCARD_H_INCLUDED

#include "speedtree_tree.h"

namespace vostok {
namespace render {

struct leafcard_vertex {
	float m_afCardCenter[3];
	float m_fWindScalar;
	u8 m_aucNormal[3];
	u8 m_ucAmbOcc;
	u8 m_aucTangent[3];
	u8 m_ucTangentPadding;
	u8 m_aucWindData[4];
	float m_afCardCorner[2];
	float m_fLodScale;
	float m_fWindScalarMag;
	float m_afDiffuseTexCoords[2];
	float m_fPlanarOffset;
	float m_fShadowOffset;
};

STATIC_SIZE_ASSERT( leafcard_vertex, 0x3c );

class speedtree_tree_component_leafcard : public speedtree_tree_component {
public:
	explicit speedtree_tree_component_leafcard( speedtree_tree& parent );
	virtual ~speedtree_tree_component_leafcard( ) { }

	virtual void set_material( material_ptr material );
	virtual void set_default_material( );
	virtual void render( lod_entry const* lod, renderer_context* context );

	virtual SpeedTree::EGeometryType get_geometry_type( ) const
	{
		// FUNCTION BODY[0x225e0]
		return SpeedTree::GEOMETRY_TYPE_LEAF_CARDS;
	}

	virtual enum_vertex_input_type get_vertex_input_type( )
	{
		return null_vertex_input_type;
	}

	void init_index_buffer(
		SpeedTree::SLeafCards const* lod,
		vector<u16>& out_indices,
		u32 num_accumulated_vertices
	);
};

STATIC_SIZE_ASSERT( speedtree_tree_component_leafcard, 0x70 );

} // namespace render
} // namespace vostok

#endif // VOSTOK_RENDER_ENGINE_SPEEDTREE_TREE_COMPONENT_LEAFCARD_H_INCLUDED
