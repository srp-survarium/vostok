#include "pch.h"
#include <vostok/render/core/dx11/res_declaration.h>
#include <vostok/render/core/dx11/res_geometry.h>
#include <vostok/render/core/resource_manager.h>
#include "speedtree_tree_component_leafmesh.h"

namespace vostok {
namespace render {

const D3D_INPUT_ELEMENT_DESC layout[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,  D3D_INPUT_PER_VERTEX_DATA, 0 }, // pos.xyz _ wind scalar
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D_INPUT_PER_VERTEX_DATA, 0 },	// LOD position.xyz + wind scalar mag
	{ "TEXCOORD", 1, DXGI_FORMAT_R8G8B8A8_UINT,		 0, 32, D3D_INPUT_PER_VERTEX_DATA, 0 },	// normal.xyz + amb occ
	{ "TEXCOORD", 2, DXGI_FORMAT_R8G8B8A8_UINT,		 0, 36, D3D_INPUT_PER_VERTEX_DATA, 0 },	// tangent.xyz + wind active flag
	{ "TEXCOORD", 3, DXGI_FORMAT_R8G8B8A8_UINT,		 0, 40, D3D_INPUT_PER_VERTEX_DATA, 0 },	// wind data.xyzw
	{ "TEXCOORD", 4, DXGI_FORMAT_R32G32_FLOAT,		 0, 44, D3D_INPUT_PER_VERTEX_DATA, 0 },	// diffuse texcoords.st
	{ "TEXCOORD", 5, DXGI_FORMAT_R32G32B32_FLOAT,	 0, 52, D3D_INPUT_PER_VERTEX_DATA, 0 },	// leaf mesh wind.xyz
};

using namespace SpeedTree;

speedtree_tree_component_leafmesh::speedtree_tree_component_leafmesh( speedtree_tree& parent ) :
	speedtree_tree_component( parent )
{
	// FUNCTION BODY[0x5f7160]
	ref_declaration decl = resource_manager::ref().create_declaration( layout, array_size( layout ) );

	// claude@NOTE: legacy bound `CTree& tree = parent` - the canonical speedtree_tree
	// derives SpeedTree::CCore (GetGeometry lives there).
	SpeedTree::CCore& tree = parent;

	const SGeometry* tree_geometry  = tree.GetGeometry( );

	st_int32 num_lods				= tree_geometry->m_nNumLeafMeshLods;
	const SIndexedTriangles* lods	= tree_geometry->m_pLeafMeshLods;

	lod_render_info& lod_info		= parent.get_lod_render_info(SpeedTree::GEOMETRY_TYPE_LEAF_MESHES);
	lod_info.num_lods				= num_lods;
	lod_info.lods					= NEW_ARRAY(lod_entry, num_lods);

	render::vector<u16>				total_indices;
	render::vector<leafmesh_vertex>	total_vertices;

	for (s32 lod_index=0; lod_index<num_lods; lod_index++)
	{
		const SIndexedTriangles* lod = lods + lod_index;

		if (!lod->HasGeometry())
			continue;

		ASSERT(lod->m_pNormals && lod->m_pTexCoordsDiffuse);

		render::vector<u16> indices;
		init_index_buffer(lod, indices);

		lod_info.lods[lod_index].start_index = total_indices.size();
		lod_info.lods[lod_index].num_indices = indices.size();

		for (render::vector<u16>::const_iterator it=indices.begin(); it!=indices.end(); ++it)
			total_indices.push_back(*it + u16(total_vertices.size()));

		// fill out vertex buffer
		render::vector<leafmesh_vertex> vertices;
		vertices.resize(lod->m_nNumVertices);

		leafmesh_vertex* p_vertex = &vertices[0];
		for (s32 vertex_index=0; vertex_index<lod->m_nNumVertices; vertex_index++)
		{
			// position
			vostok::memory::copy(p_vertex->m_afCoord, sizeof(p_vertex->m_afCoord), lod->m_pCoords + vertex_index * 3, sizeof(p_vertex->m_afCoord));

			parent.m_bbox.modify(float3(p_vertex->m_afCoord[0], p_vertex->m_afCoord[1], p_vertex->m_afCoord[2]));

			static const st_float32 c_fUncompressWindScalar = 10.0f;

			// wind scalar
			if (lod->m_pWindData)
				p_vertex->m_fWindScalar = c_fUncompressWindScalar * CCore::UncompressScalar(lod->m_pWindData[vertex_index * 6 + 4]);
			else
				p_vertex->m_fWindScalar = 0.0f;

			// LOD position
			vostok::memory::copy(p_vertex->m_afLodCoord, sizeof(p_vertex->m_afLodCoord), lod->m_pLodCoords ? lod->m_pLodCoords + vertex_index * 3 : lod->m_pCoords + vertex_index * 3, sizeof(p_vertex->m_afLodCoord));

			// normal
			vostok::memory::copy(p_vertex->m_aucNormal, sizeof(p_vertex->m_aucNormal), lod->m_pNormals + vertex_index * 3, sizeof(p_vertex->m_aucNormal));

			// ambient occlusion
			p_vertex->m_ucAmbOcc = lod->m_pAmbientOcclusionValues ? lod->m_pAmbientOcclusionValues[vertex_index] : 255;

			// tangent
			vostok::memory::copy(p_vertex->m_aucTangent, sizeof(p_vertex->m_aucTangent), lod->m_pTangents + vertex_index * 3, sizeof(p_vertex->m_aucTangent));

			// wind active flag
			p_vertex->m_ucWindActiveFlag = st_float32(lod->m_pWindData[vertex_index * 6 + 5]) ? 255 : 0;

			// wind data
			if (lod->m_pWindData)
			{
				p_vertex->m_fWindScalarMag = lod->m_fWindDataMagnitude;
				vostok::memory::copy(p_vertex->m_aucWindData, sizeof(p_vertex->m_aucWindData), lod->m_pWindData + vertex_index * 6, sizeof(p_vertex->m_aucWindData));
			}
			else
			{
				p_vertex->m_fWindScalarMag = 0.0f;
				p_vertex->m_aucWindData[0] = p_vertex->m_aucWindData[1] = p_vertex->m_aucWindData[2] = p_vertex->m_aucWindData[3] = 0;
			}

			// diffuse texcoords
			p_vertex->m_afDiffuseTexCoords[0] = lod->m_pTexCoordsDiffuse[vertex_index * 2];
			p_vertex->m_afDiffuseTexCoords[1] = lod->m_pTexCoordsDiffuse[vertex_index * 2 + 1];

			// leaf mesh wind
			p_vertex->m_afLeafMeshWind[0] = lod->m_pLeafMeshWind[vertex_index * 3];
			p_vertex->m_afLeafMeshWind[1] = lod->m_pLeafMeshWind[vertex_index * 3 + 1];
			p_vertex->m_afLeafMeshWind[2] = lod->m_pLeafMeshWind[vertex_index * 3 + 2];

			++p_vertex;
		}

		for (render::vector<leafmesh_vertex>::const_iterator it=vertices.begin(); it!=vertices.end(); ++it)
			total_vertices.push_back(*it);
	}

	m_render_geometry.index_count		= total_indices.size();
	m_render_geometry.primitive_count	= m_render_geometry.index_count/3;
	untyped_buffer_ptr ib				= resource_manager::ref().create_buffer( m_render_geometry.index_count*sizeof(u16),
		&total_indices[0],
		enum_buffer_type_index,
		false,
		false
		);

	m_render_geometry.vertex_count		= total_vertices.size();
	bool dynamic						= true;
	u32 stride							= sizeof(leafmesh_vertex);
	untyped_buffer_ptr vb				= resource_manager::ref().create_buffer	(	m_render_geometry.vertex_count * stride,
		&total_vertices[0],
		enum_buffer_type_vertex,
		dynamic,
		false
		);

	m_render_geometry.geom				= resource_manager::ref().create_geometry( &*decl, stride, *vb, *ib);
}

void speedtree_tree_component_leafmesh::init_index_buffer( SpeedTree::SIndexedTriangles const* lod, vector<u16>& out_indices )
{
	// FUNCTION BODY[0x5f6fb0]
	st_int32 index_buffer_size = 0;

	for (s32 mg_index = 0; mg_index < lod->m_nNumMaterialGroups; mg_index++)
	{
		const SDrawCallInfo* info = lod->m_pDrawCallInfo + mg_index;
		index_buffer_size += info->m_nLength;
	}

	for (s32 mg_index = 0; mg_index < lod->m_nNumMaterialGroups; mg_index++)
	{
		const SDrawCallInfo* info = lod->m_pDrawCallInfo + mg_index;

		ASSERT(lod->m_pTriangleIndices16);

		for (s32 i=info->m_nOffset; i<info->m_nOffset + info->m_nLength; i++)
			out_indices.push_back(u16(lod->m_pTriangleIndices16[i]));
	}
}

void speedtree_tree_component_leafmesh::set_material( material_ptr mtl_ptr )
{
	// FUNCTION BODY[0x5f7130]
	// claude@NOTE: legacy body was fully commented out (initialize_material_effects era).
	VOSTOK_UNREFERENCED_PARAMETER	( mtl_ptr );
}

void speedtree_tree_component_leafmesh::set_default_material( )
{
	// FUNCTION BODY[0x5f6f90]
//	m_material_effects = material::nomaterial_material(speedtree_leafmesh_vertex_input_type);
}

void speedtree_tree_component_leafmesh::render( lod_entry const* lod, renderer_context* context )
{
	// FUNCTION BODY[0x5f6fa0]
	VOSTOK_UNREFERENCED_PARAMETERS(lod, context);
}

} // namespace render
} // namespace vostok
