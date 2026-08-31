#include "pch.h"

#include <vostok/render/core/backend.h>
#include <vostok/render/core/resource_manager.h>

#include "fog_box_geometry.h"

using vostok::math::float3;

float3 du_box_vertices[] = {
	float3( -0.5f, -0.5f, -0.5f ),
	float3( -0.5f,  0.5f, -0.5f ),
	float3(  0.5f,  0.5f, -0.5f ),
	float3(  0.5f, -0.5f, -0.5f ),
	float3( -0.5f,  0.5f,  0.5f ),
	float3( -0.5f, -0.5f,  0.5f ),
	float3(  0.5f,  0.5f,  0.5f ),
	float3(  0.5f, -0.5f,  0.5f )
};

u16 du_box_faces[] = {
	0, 1, 2, 2, 3, 0,
	3, 2, 6, 6, 7, 3,
	6, 4, 5, 6, 5, 7,
	4, 1, 5, 1, 0, 5,
	3, 5, 0, 3, 7, 5,
	1, 4, 6, 1, 6, 2
};

namespace vostok {
namespace render {

fog_box_geometry::fog_box_geometry( )
{
	untyped_buffer_ptr vertex_buffer = resource_manager::ref( ).create_buffer(
		sizeof( du_box_vertices ),
		du_box_vertices,
		enum_buffer_type_vertex,
		false,
		false
	);
	untyped_buffer_ptr index_buffer = resource_manager::ref( ).create_buffer(
		sizeof( du_box_faces ),
		du_box_faces,
		enum_buffer_type_index,
		false,
		false
	);

	D3D11_INPUT_ELEMENT_DESC layout[1] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	m_geometry = resource_manager::ref( ).create_geometry(
		layout,
		sizeof( float3 ),
		*vertex_buffer,
		*index_buffer
	);
}

void fog_box_geometry::render( )
{
	m_geometry->apply( );
	backend::ref( ).render_indexed( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 36, 0, 0 );
}

} // namespace render
} // namespace vostok
