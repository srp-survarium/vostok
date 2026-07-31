#include "pch.h"
#include <vostok/render/core/dx11/res_declaration.h>
#include <vostok/render/core/dx11/res_geometry.h>
#include <vostok/render/core/resource_manager.h>
#include "speedtree_tree_component_leafcard.h"

namespace vostok {
namespace render {

const D3D_INPUT_ELEMENT_DESC leafcard_layout[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,  D3D_INPUT_PER_VERTEX_DATA, 0 },// pos.xyz _ wind scalar
	{ "TEXCOORD", 0, DXGI_FORMAT_R8G8B8A8_UINT,		 0, 16, D3D_INPUT_PER_VERTEX_DATA, 0 },// normal.xyz + amb occ
	{ "TEXCOORD", 1, DXGI_FORMAT_R8G8B8A8_UINT,		 0, 20, D3D_INPUT_PER_VERTEX_DATA, 0 },// tangent.xyz + tangent pad
	{ "TEXCOORD", 2, DXGI_FORMAT_R8G8B8A8_UINT,		 0, 24, D3D_INPUT_PER_VERTEX_DATA, 0 },// wind data.xyzw
	{ "TEXCOORD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 28, D3D_INPUT_PER_VERTEX_DATA, 0 },// lod scales.xy, card corner.zw
	{ "TEXCOORD", 4, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 44, D3D_INPUT_PER_VERTEX_DATA, 0 },// diffuse texcoords.xy, planar offset.z, shadow offset.z
};

using namespace SpeedTree;

speedtree_tree_component_leafcard::speedtree_tree_component_leafcard( speedtree_tree& parent ) :
	speedtree_tree_component( parent )
{
	// FUNCTION BODY[0x5f64e0]
	static const st_float32 c_fUncompressWindScalar = 10.0f;

	ref_declaration decl = resource_manager::ref().create_declaration( leafcard_layout, array_size( leafcard_layout ) );

	// claude@NOTE: legacy bound `CTree& tree = parent` - the canonical speedtree_tree
	// derives SpeedTree::CCore (GetGeometry lives there).
	SpeedTree::CCore& tree = parent;

	const SGeometry* tree_geometry  = tree.GetGeometry( );

	st_int32 num_lods				= tree_geometry->m_nNumLeafCardLods;
	const SLeafCards* lods			= tree_geometry->m_pLeafCardLods;

	lod_render_info& lod_info		= parent.get_lod_render_info(SpeedTree::GEOMETRY_TYPE_LEAF_CARDS);
	lod_info.num_lods				= num_lods;
	lod_info.lods					= NEW_ARRAY(lod_entry, num_lods);

	render::vector<u16>				total_indices;
	render::vector<leafcard_vertex>	total_vertices;

	for (s32 lod_index=0; lod_index<num_lods; lod_index++)
	{
		const SLeafCards* lod = lods + lod_index;

		if (!lod->HasGeometry())
			continue;

		ASSERT(lod->m_pNormals && lod->m_pTexCoordsDiffuse);

		render::vector<u16> indices;
		init_index_buffer(lod, indices, total_vertices.size());

		lod_info.lods[lod_index].start_index = total_indices.size();
		lod_info.lods[lod_index].num_indices = indices.size();

		for (render::vector<u16>::const_iterator it=indices.begin(); it!=indices.end(); ++it)
			total_indices.push_back(*it);

		// fill out vertex buffer
		render::vector<leafcard_vertex> vertices;
		vertices.resize(lod->m_nTotalNumCards * 4);

		leafcard_vertex* p_vertex = &vertices[0];

		for (s32 card_index=0; card_index<lod->m_nTotalNumCards; card_index++)
		{
			const float card_offsets[4][2] =  { { -0.5f, 0.5f }, { 0.5f, 0.5f }, { 0.5f, -0.5f }, { -0.5f, -0.5f } };

			// setup card values used by each vertex
			const st_float32* card_dimensons = lod->m_pDimensions + card_index * 2;
			const st_float32* card_pivot_point = lod->m_pPivotPoints + card_index * 2;

			Vec3 pivot(card_pivot_point[0] * card_dimensons[0], card_pivot_point[1] * card_dimensons[1], 0.0f);

			// determine the largest distance from the pivot point; useful for the shadow offset value
			st_float32 fMaxDistanceFromPivotPoint = 0.0f;
			st_int32 nCorner = 0;
			for (nCorner = 0; nCorner < 4; ++nCorner)
			{
				Vec3 vCorner(card_offsets[nCorner][0] * card_dimensons[0], card_offsets[nCorner][1] * card_dimensons[1], 0.0f);
				fMaxDistanceFromPivotPoint = st_max(fMaxDistanceFromPivotPoint, vCorner.Distance(pivot));
			}

			for (nCorner = 0; nCorner < 4; ++nCorner)
			{
				// card center
				vostok::memory::copy(p_vertex->m_afCardCenter, sizeof(p_vertex->m_afCardCenter), lod->m_pPositions + card_index * 3, sizeof(p_vertex->m_afCardCenter));

				parent.m_bbox.modify(float3(p_vertex->m_afCardCenter[0], p_vertex->m_afCardCenter[1], p_vertex->m_afCardCenter[2]));

				// wind scalar
				if (lod->m_pWindData)
					p_vertex->m_fWindScalar = c_fUncompressWindScalar * CCore::UncompressScalar(lod->m_pWindData[card_index * 5 + 4]);
				else
					p_vertex->m_fWindScalar = 0.0f;

				// normal
				vostok::memory::copy(p_vertex->m_aucNormal, sizeof(p_vertex->m_aucNormal), lod->m_pNormals + card_index * 12 + nCorner * 3, sizeof(p_vertex->m_aucNormal));

				// ambient occlusion
				p_vertex->m_ucAmbOcc = lod->m_pAmbientOcclusionValues ? lod->m_pAmbientOcclusionValues[card_index] : 255;

				// tangent
				vostok::memory::copy(p_vertex->m_aucTangent, sizeof(p_vertex->m_aucTangent), lod->m_pTangents + card_index * 12 + nCorner * 3, sizeof(p_vertex->m_aucTangent));
				p_vertex->m_ucTangentPadding = 0;

				// wind data
				if (lod->m_pWindData)
				{
					p_vertex->m_fWindScalarMag = lod->m_fWindDataMagnitude;
					vostok::memory::copy(p_vertex->m_aucWindData, sizeof(p_vertex->m_aucWindData), lod->m_pWindData + card_index * 5, sizeof(p_vertex->m_aucWindData));
				}
				else
				{
					p_vertex->m_fWindScalarMag = 0.0f;
					p_vertex->m_aucWindData[0] = p_vertex->m_aucWindData[1] = p_vertex->m_aucWindData[2] = p_vertex->m_aucWindData[3] = 0;
				}

				// lod scales
				const st_float32* pLodScales = lod->m_pLodScales + card_index * 2;
				if (pLodScales[1] == 0.0f)
					p_vertex->m_fLodScale = 0.0f;
				else if (pLodScales[0] == pLodScales[1])
					p_vertex->m_fLodScale = 1.0f;
				else
					p_vertex->m_fLodScale = pLodScales[1] / pLodScales[0];

				// card corner
				const st_float32 c_fStartLodScale = pLodScales[0];
				if (CCoordSys::IsYAxisUp( ))
				{
					p_vertex->m_afCardCorner[0] = (card_offsets[nCorner][1] - card_pivot_point[1]) * card_dimensons[1] * c_fStartLodScale;
					p_vertex->m_afCardCorner[1] = (card_offsets[nCorner][0] - card_pivot_point[0]) * card_dimensons[0] * c_fStartLodScale;
				}
				else
				{
					p_vertex->m_afCardCorner[0] = (card_offsets[nCorner][0] - card_pivot_point[0]) * card_dimensons[0] * c_fStartLodScale;
					p_vertex->m_afCardCorner[1] = (card_offsets[nCorner][1] - card_pivot_point[1]) * card_dimensons[1] * c_fStartLodScale;
				}

				// diffuse texcoords
				const st_float32* pTexCoords = lod->m_pTexCoordsDiffuse + card_index * 8 + nCorner * 2;
				p_vertex->m_afDiffuseTexCoords[0] = pTexCoords[0];
				p_vertex->m_afDiffuseTexCoords[1] = pTexCoords[1];

				// planar offset
				p_vertex->m_fPlanarOffset = lod->m_pLeafCardOffsets ? lod->m_pLeafCardOffsets[card_index * 4 + nCorner] : 0.0f;

				// shadow offset
				p_vertex->m_fShadowOffset = fMaxDistanceFromPivotPoint;

				++p_vertex;
			}
		}

		for (render::vector<leafcard_vertex>::const_iterator it=vertices.begin(); it!=vertices.end(); ++it)
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
	u32 stride							= sizeof(leafcard_vertex);
	untyped_buffer_ptr vb				= resource_manager::ref().create_buffer	(	m_render_geometry.vertex_count * stride,
		&total_vertices[0],
		enum_buffer_type_vertex,
		dynamic,
		false
		);

	m_render_geometry.geom				= resource_manager::ref().create_geometry( &*decl, stride, *vb, *ib);
}

void speedtree_tree_component_leafcard::init_index_buffer(
	SpeedTree::SLeafCards const* lod,
	vector<u16>& out_indices,
	u32 num_accumulated_vertices
)
{
	// FUNCTION BODY[0x5f5fd0]
	out_indices.reserve( lod->m_nNumMaterialGroups * lod->m_nTotalNumCards * 6 );

	for (s32 mg_index = 0; mg_index < lod->m_nNumMaterialGroups; mg_index++)
	{
		for (s32 card_index=0; card_index<lod->m_nTotalNumCards; card_index++)
		{
			u16 c_index = u16(num_accumulated_vertices + card_index * 4);

			out_indices.push_back(c_index + 0);
			out_indices.push_back(c_index + 1);
			out_indices.push_back(c_index + 2);
			out_indices.push_back(c_index + 0);
			out_indices.push_back(c_index + 2);
			out_indices.push_back(c_index + 3);
		}
	}
}

void speedtree_tree_component_leafcard::set_material( material_ptr mtl_ptr )
{
	// FUNCTION BODY[0x5f64b0]
	// claude@NOTE: legacy body was fully commented out (initialize_material_effects era).
	VOSTOK_UNREFERENCED_PARAMETER	( mtl_ptr );
}

void speedtree_tree_component_leafcard::set_default_material( )
{
	// FUNCTION BODY[0x5f5fb0]
	//m_material_effects = material::nomaterial_material(speedtree_leafcard_vertex_input_type);
}

void speedtree_tree_component_leafcard::render( lod_entry const* lod, renderer_context* context )
{
	// FUNCTION BODY[0x5f5fc0]
	VOSTOK_UNREFERENCED_PARAMETERS(lod, context);
}

} // namespace render
} // namespace vostok
