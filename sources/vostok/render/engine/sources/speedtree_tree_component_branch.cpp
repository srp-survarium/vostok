#include "pch.h"
#include <vostok/render/core/dx11/res_declaration.h>
#include <vostok/render/core/dx11/res_geometry.h>
#include <vostok/render/core/resource_manager.h>
#include "speedtree_tree_component_branch.h"

namespace vostok {
namespace render {

const D3D_INPUT_ELEMENT_DESC branch_layout[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	 0, 0,  D3D_INPUT_PER_VERTEX_DATA, 0 }, // pos.xyz
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D_INPUT_PER_VERTEX_DATA, 0 },	// LOD position.xyz + wind scalar mag
	{ "TEXCOORD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 28, D3D_INPUT_PER_VERTEX_DATA, 0 },	// diffuse texcoords.st + detail texcoords.st
	{ "TEXCOORD", 2, DXGI_FORMAT_R8G8B8A8_UINT,		 0, 44, D3D_INPUT_PER_VERTEX_DATA, 0 },	// normal.xyz + amb occ
	{ "TEXCOORD", 3, DXGI_FORMAT_R8G8B8A8_UINT,		 0, 48, D3D_INPUT_PER_VERTEX_DATA, 0 },	// tangent.xyz
	{ "TEXCOORD", 4, DXGI_FORMAT_R8G8B8A8_UINT,		 0, 52, D3D_INPUT_PER_VERTEX_DATA, 0 },	// wind data.xyzw
};

using namespace SpeedTree;

speedtree_tree_component_branch::speedtree_tree_component_branch( speedtree_tree& parent ) :
	speedtree_tree_component( parent )
{
	// FUNCTION BODY[0x5f8760]
	ref_declaration decl = resource_manager::ref().create_declaration( branch_layout, array_size( branch_layout ) );

	// claude@NOTE: legacy bound `CTree& tree = parent` - the canonical speedtree_tree
	// derives SpeedTree::CCore (GetGeometry lives there).
	SpeedTree::CCore& tree = parent;

	const SGeometry* tree_geometry  = tree.GetGeometry( );

	st_int32 num_lods				= tree_geometry->m_nNumBranchLods;
	const SIndexedTriangles* lods	= tree_geometry->m_pBranchLods;

	lod_render_info& lod_info		= parent.get_lod_render_info(SpeedTree::GEOMETRY_TYPE_BRANCHES);
	lod_info.num_lods				= num_lods;
	lod_info.lods					= NEW_ARRAY(lod_entry, num_lods);

	render::vector<u16>				total_indices;
	render::vector<branch_vertex>	total_vertices;

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
		render::vector<branch_vertex> vertices;
		vertices.resize(lod->m_nNumVertices);

		branch_vertex* p_vertex = &vertices[0];
		for (s32 vertex_index=0; vertex_index<lod->m_nNumVertices; vertex_index++)
		{
			// position
			vostok::memory::copy(p_vertex->m_afCoord, sizeof(p_vertex->m_afCoord), lod->m_pCoords + vertex_index * 3, sizeof(p_vertex->m_afCoord));

			parent.m_bbox.modify(float3(p_vertex->m_afCoord[0], p_vertex->m_afCoord[1], p_vertex->m_afCoord[2]));

			// LOD position
			vostok::memory::copy(p_vertex->m_afLodCoord, sizeof(p_vertex->m_afLodCoord), lod->m_pLodCoords ? lod->m_pLodCoords + vertex_index * 3 : lod->m_pCoords + vertex_index * 3, sizeof(p_vertex->m_afLodCoord));

			// normal
			vostok::memory::copy(p_vertex->m_aucNormal, sizeof(p_vertex->m_aucNormal), lod->m_pNormals + vertex_index * 3, sizeof(p_vertex->m_aucNormal));

			// ambient occlusion
			p_vertex->m_ucAmbOcc = lod->m_pAmbientOcclusionValues ? lod->m_pAmbientOcclusionValues[vertex_index] : 255;

			// diffuse texcoords
			p_vertex->m_afDiffuseTexCoords[0] = lod->m_pTexCoordsDiffuse[vertex_index * 2];
			p_vertex->m_afDiffuseTexCoords[1] = lod->m_pTexCoordsDiffuse[vertex_index * 2 + 1];

			// detail texcoords
			if (lod->m_pTexCoordsDetail)
			{
				p_vertex->m_afDetailTexCoords[0] = lod->m_pTexCoordsDetail[vertex_index * 2];
				p_vertex->m_afDetailTexCoords[1] = lod->m_pTexCoordsDetail[vertex_index * 2 + 1];
			}
			else
				p_vertex->m_afDetailTexCoords[0] = p_vertex->m_afDetailTexCoords[1] = 0.0f;

			// tangent
			vostok::memory::copy(p_vertex->m_aucTangent, sizeof(p_vertex->m_aucTangent), lod->m_pTangents + vertex_index * 3, sizeof(p_vertex->m_aucTangent));
			p_vertex->m_ucTangentPadding = 0;

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

			++p_vertex;
		}

		for (render::vector<branch_vertex>::const_iterator it=vertices.begin(); it!=vertices.end(); ++it)
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
	u32 stride							= sizeof(branch_vertex);
	untyped_buffer_ptr vb				= resource_manager::ref().create_buffer	(	m_render_geometry.vertex_count * stride,
		&total_vertices[0],
		enum_buffer_type_vertex,
		dynamic,
		false
		);

	m_render_geometry.geom				= resource_manager::ref().create_geometry( &*decl, stride, *vb, *ib);
}

void speedtree_tree_component_branch::init_index_buffer( SpeedTree::SIndexedTriangles const* lod, vector<u16>& out_indices )
{
	// FUNCTION BODY[0x5f83b0]
	for (s32 mg_index = 0; mg_index < lod->m_nNumMaterialGroups; mg_index++)
	{
		const SDrawCallInfo* info = lod->m_pDrawCallInfo + mg_index;

		ASSERT(lod->m_pTriangleIndices16);

		s32 const num_triangles = (info->m_nOffset + info->m_nLength);

		for (s32 i = info->m_nOffset; i < num_triangles; i+=3)
		{
			out_indices.push_back(u16(lod->m_pTriangleIndices16[i + 2]));
			out_indices.push_back(u16(lod->m_pTriangleIndices16[i + 1]));
			out_indices.push_back(u16(lod->m_pTriangleIndices16[i + 0]));
		}
	}
}

void speedtree_tree_component_branch::set_material( material_ptr mtl_ptr )
{
	// FUNCTION BODY[0x5f8730]
	// claude@NOTE: legacy body was fully commented out (initialize_material_effects era).
	VOSTOK_UNREFERENCED_PARAMETER	( mtl_ptr );
}

void speedtree_tree_component_branch::set_default_material( )
{
	// FUNCTION BODY[0x5f8390]
	//m_material_effects = material::nomaterial_material(speedtree_branch_vertex_input_type);
}

void speedtree_tree_component_branch::render( lod_entry const* lod, renderer_context* context )
{
	// FUNCTION BODY[0x5f83a0]
	VOSTOK_UNREFERENCED_PARAMETERS(lod, context);
}

} // namespace render
} // namespace vostok
