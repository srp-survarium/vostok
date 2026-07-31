#include "pch.h"
#include <vostok/render/core/dx11/res_declaration.h>
#include <vostok/render/core/dx11/res_geometry.h>
#include <vostok/render/core/resource_manager.h>
#include <vostok/render/engine/model_format.h>
#include "skeleton_mesh_gpu_skinning_2weights.h"

// claude@NOTE: seeded from the 4weights legacy ancestor (the only shipped-era source);
// layout offsets derived from hardware_2weights_skinning_vertex in model_format.h.

namespace vostok {
namespace render {

static D3D_INPUT_ELEMENT_DESC hardware_2weights_skinning_vertex_layout[] =
{
	{"POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,	D3D_INPUT_PER_VERTEX_DATA, 0},
	{"BLENDINDICES",0, DXGI_FORMAT_R8G8B8A8_UINT,	0, 12, 	D3D_INPUT_PER_VERTEX_DATA, 0},
	{"BLENDWEIGHT",	0, DXGI_FORMAT_R32_FLOAT,		0, 16,	D3D_INPUT_PER_VERTEX_DATA, 0},
	{"NORMAL",		0, DXGI_FORMAT_R8G8B8A8_UNORM,	0, 20,	D3D_INPUT_PER_VERTEX_DATA, 0},
	{"TANGENT",		0, DXGI_FORMAT_R8G8B8A8_UNORM,	0, 24,	D3D_INPUT_PER_VERTEX_DATA, 0},
	{"BINORMAL",	0, DXGI_FORMAT_R8G8B8A8_UNORM,	0, 28, 	D3D_INPUT_PER_VERTEX_DATA, 0},
	{"TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,	0, 32, 	D3D_INPUT_PER_VERTEX_DATA, 0},
};

skeleton_mesh_gpu_skinning_2weights::skeleton_mesh_gpu_skinning_2weights( )
{
	// FUNCTION BODY[0x5f3600]
	m_vertex_input_type	= skeletal_2_bones_mesh_vertex_input_type;
}

skeleton_mesh_gpu_skinning_2weights::~skeleton_mesh_gpu_skinning_2weights( )
{
	// FUNCTION BODY[0x5f35c0]
}

void skeleton_mesh_gpu_skinning_2weights::load(
	configs::binary_config_value const& properties,
	memory::chunk_reader& r
)
{
	// FUNCTION BODY[0x5f3430]
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

	ref_declaration const& declaration	= resource_manager::ref().create_declaration( hardware_2weights_skinning_vertex_layout, array_size( hardware_2weights_skinning_vertex_layout ) );

	void* ptr = (void*)verts_reader.pointer();
	hardware_2weights_skinning_vertex* const prepared_vertices = static_cast< hardware_2weights_skinning_vertex* >( ptr );

	bool const dynamic		= true;
	u32 const stride		= sizeof( hardware_2weights_skinning_vertex );
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

void skeleton_mesh_gpu_skinning_2weights::update( vector< float4x4 > const& bones )
{
	// FUNCTION BODY[0x5f3420]
	VOSTOK_UNREFERENCED_PARAMETER	( bones );
}

} // namespace render
} // namespace vostok
