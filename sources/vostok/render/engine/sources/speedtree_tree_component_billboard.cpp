// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include <vostok/render/core/dx11/res_declaration.h>
#include <vostok/render/core/dx11/res_geometry.h>
#include <vostok/render/core/resource_manager.h>
#include "speedtree_convert_type.h"
#include "speedtree_forest.h"
#include "speedtree_tree_component_billboard.h"
namespace vostok {
namespace render {

const D3D_INPUT_ELEMENT_DESC layout[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,  D3D_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 1, DXGI_FORMAT_R8_UINT,			 0, 32, D3D_INPUT_PER_VERTEX_DATA, 0 },
};

using namespace SpeedTree;

void speedtree_tree_component_billboard::init(
	speedtree_forest& forest,
	SpeedTree::CArray<SpeedTree::CInstance, 1> const& instances_of_tree
)
{
	ref_declaration decl				= resource_manager::ref().create_declaration( layout, array_size( layout ) );

	render::vector<u16>					total_indices;
	render::vector<billboard_vertex>	total_vertices;

	total_indices.reserve				(instances_of_tree.size() * 6);
	total_vertices.reserve				(instances_of_tree.size() * 4);

	for (u32 instance_index=0; instance_index<instances_of_tree.size(); instance_index++)
	{
		CInstance const& instance		= instances_of_tree.at(instance_index);

		float4x4 const transform		= forest.get_instance_transform(instance);
		float3 const rotation_xyz		= transform.get_angles_xyz();

		float3 const up_direction		= math::normalize(transform.lines[1].xyz());

		total_vertices.push_back( billboard_vertex( speedtree_to_vostok(instance.GetPos()), instance.GetScale(), up_direction, instance.GetRotationAngle(), 0) );
		total_vertices.push_back( billboard_vertex( speedtree_to_vostok(instance.GetPos()), instance.GetScale(), up_direction, instance.GetRotationAngle(), 1) );
		total_vertices.push_back( billboard_vertex( speedtree_to_vostok(instance.GetPos()), instance.GetScale(), up_direction, instance.GetRotationAngle(), 2) );
		total_vertices.push_back( billboard_vertex( speedtree_to_vostok(instance.GetPos()), instance.GetScale(), up_direction, instance.GetRotationAngle(), 3) );

		u16 i_index = u16(instance_index) * 4;

		total_indices.push_back(i_index + 0);
		total_indices.push_back(i_index + 1);
		total_indices.push_back(i_index + 2);

		total_indices.push_back(i_index + 0);
		total_indices.push_back(i_index + 2);
		total_indices.push_back(i_index + 3);
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
	u32 stride							= sizeof(billboard_vertex);
	untyped_buffer_ptr vb				= resource_manager::ref().create_buffer	(	m_render_geometry.vertex_count * stride,
		&total_vertices[0],
		enum_buffer_type_vertex,
		dynamic,
		false
		);

	m_render_geometry.geom				= resource_manager::ref().create_geometry( &*decl, stride, *vb, *ib);

	m_is_init							= true;
}

speedtree_tree_component_billboard::speedtree_tree_component_billboard( speedtree_tree& parent ) :
	speedtree_tree_component( parent ),
	m_is_init( false )
{
}

void speedtree_tree_component_billboard::set_material( material_ptr mtl_ptr )
{
	VOSTOK_UNREFERENCED_PARAMETER	( mtl_ptr );
}

void speedtree_tree_component_billboard::set_default_material( )
{
}

void speedtree_tree_component_billboard::render( lod_entry const* lod, renderer_context* context )
{
	VOSTOK_UNREFERENCED_PARAMETERS(lod, context);
}

} // namespace render
} // namespace vostok
