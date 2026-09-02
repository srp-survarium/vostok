// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_ENGINE_SPEEDTREE_TREE_COMPONENT_BRANCH_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_SPEEDTREE_TREE_COMPONENT_BRANCH_H_INCLUDED
#include "speedtree_tree.h"

namespace vostok {
namespace render {

struct branch_vertex {
	float m_afCoord[3];
	float m_afLodCoord[3];
	float m_fWindScalarMag;
	float m_afDiffuseTexCoords[2];
	float m_afDetailTexCoords[2];
	u8 m_aucNormal[3];
	u8 m_ucAmbOcc;
	u8 m_aucTangent[3];
	u8 m_ucTangentPadding;
	u8 m_aucWindData[4];
};

STATIC_SIZE_ASSERT( branch_vertex, 0x38 );

class speedtree_tree_component_branch : public speedtree_tree_component {
public:
	explicit speedtree_tree_component_branch( speedtree_tree& parent );

	virtual void set_material( material_ptr material );
	virtual void set_default_material( );

	virtual SpeedTree::EGeometryType get_geometry_type( ) const
	{
		return SpeedTree::GEOMETRY_TYPE_BRANCHES;
	}

	virtual void render( lod_entry const* lod, renderer_context* context );

	virtual enum_vertex_input_type get_vertex_input_type( )
	{
		return unknown_vertex_input_type;
	}

	void init_index_buffer( SpeedTree::SIndexedTriangles const* lod, vector<u16>& out_indices );
};

STATIC_SIZE_ASSERT( speedtree_tree_component_branch, 0x70 );

} // namespace render
} // namespace vostok

#endif // VOSTOK_RENDER_ENGINE_SPEEDTREE_TREE_COMPONENT_BRANCH_H_INCLUDED
