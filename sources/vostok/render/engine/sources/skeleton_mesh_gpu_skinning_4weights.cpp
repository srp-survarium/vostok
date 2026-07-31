#include "pch.h"
#include <vostok/render/core/dx11/res_declaration.h>
#include <vostok/render/core/dx11/res_geometry.h>
#include <vostok/render/core/resource_manager.h>
#include <vostok/render/engine/model_format.h>
#include "skeleton_mesh_gpu_skinning_4weights.h"

namespace vostok {
namespace render {

static D3D_INPUT_ELEMENT_DESC hardware_4weights_skinning_vertex_layout[] =
{
	{"POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,	D3D_INPUT_PER_VERTEX_DATA, 0},
	{"BLENDINDICES",0, DXGI_FORMAT_R8G8B8A8_UINT,	0, 12, 	D3D_INPUT_PER_VERTEX_DATA, 0},
	{"BLENDWEIGHT",	0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 16,	D3D_INPUT_PER_VERTEX_DATA, 0},
	{"NORMAL",		0, DXGI_FORMAT_R8G8B8A8_UNORM,	0, 28,	D3D_INPUT_PER_VERTEX_DATA, 0},
	{"TANGENT",		0, DXGI_FORMAT_R8G8B8A8_UNORM,	0, 32,	D3D_INPUT_PER_VERTEX_DATA, 0},
	{"BINORMAL",	0, DXGI_FORMAT_R8G8B8A8_UNORM,	0, 36, 	D3D_INPUT_PER_VERTEX_DATA, 0},
	{"TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,	0, 40, 	D3D_INPUT_PER_VERTEX_DATA, 0},
};

skeleton_mesh_gpu_skinning_4weights::skeleton_mesh_gpu_skinning_4weights( )
{
	// FUNCTION BODY[0x5f3a00]
}

skeleton_mesh_gpu_skinning_4weights::~skeleton_mesh_gpu_skinning_4weights( )
{
	// FUNCTION BODY[0x5f39c0]
}

void skeleton_mesh_gpu_skinning_4weights::load(
	configs::binary_config_value const& properties,
	memory::chunk_reader& r
)
{
	// FUNCTION BODY[0x5f3840]
	skeleton_render_surface::load		( properties, r );

	memory::reader verts_reader		= r.open_reader( model_chunk_vertices );
	m_render_geometry.vertex_count	= verts_reader.r_u32( );

	memory::reader indices_reader	= r.open_reader( model_chunk_indices );
	m_render_geometry.index_count	= indices_reader.r_u32( );

	m_render_geometry.primitive_count	= m_render_geometry.index_count/3;

	untyped_buffer_ptr ib			=
		resource_manager::ref().create_buffer(
			m_render_geometry.index_count*sizeof(u16),
			indices_reader.pointer(),
			enum_buffer_type_index,
			false,
			false
		);

	ref_declaration const& declaration	= resource_manager::ref().create_declaration( hardware_4weights_skinning_vertex_layout, array_size( hardware_4weights_skinning_vertex_layout ) );

	void* ptr = (void*)verts_reader.pointer();
	hardware_4weights_skinning_vertex* const prepared_vertices = static_cast< hardware_4weights_skinning_vertex* >( ptr );

	bool const dynamic		= true;
	u32 const stride		= sizeof( hardware_4weights_skinning_vertex );
	m_vertex_buffer			=
		resource_manager::ref().create_buffer	(
			m_render_geometry.vertex_count * stride,
			prepared_vertices,
			enum_buffer_type_vertex,
			dynamic,
			false
		);

	m_render_geometry.geom	= resource_manager::ref().create_geometry( &*declaration, stride, *m_vertex_buffer, *ib);
}

void skeleton_mesh_gpu_skinning_4weights::update( vector< float4x4 > const& bones )
{
	// FUNCTION BODY[0x5f3830]
	VOSTOK_UNREFERENCED_PARAMETER	( bones );
}

} // namespace render
} // namespace vostok
