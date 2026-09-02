// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include <vostok/render/core/backend.h>
#include <vostok/render/core/resource_manager.h>

#include "hw_hiz_point_list.h"

namespace vostok {
namespace render {

struct vertex_struct {
	float4 bound;
	float2 pixel;
};

hw_hiz_point_list::hw_hiz_point_list( ) :
	m_num_points( 0 )
{
	D3D11_INPUT_ELEMENT_DESC point_list_layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	m_declaration = resource_manager::ref( ).create_declaration(
		point_list_layout,
		array_size( point_list_layout )
	);
}

void hw_hiz_point_list::initialize( u32 const num_points )
{
	m_num_points = num_points;
	m_vertex_buffer = resource_manager::ref( ).create_buffer(
		num_points * sizeof( vertex_struct ),
		0,
		enum_buffer_type_vertex,
		true,
		false
	);
}

void hw_hiz_point_list::set_points(
	float4 const* in_data,
	u32 const culling_results_buffer_width
)
{
	vertex_struct* out_data = static_cast< vertex_struct* >(
		m_vertex_buffer->map( D3D11_MAP_WRITE_DISCARD )
	);
	for ( u32 i = 0; i < m_num_points; ++i ) {
		out_data[i].bound = in_data[i];
		out_data[i].pixel = float2(
			float( i % culling_results_buffer_width ),
			float( i / culling_results_buffer_width )
		);
	}
	m_vertex_buffer->unmap( );
}

void hw_hiz_point_list::render( u32 const num_points )
{
	backend::ref( ).set_declaration( m_declaration.c_ptr( ) );
	backend::ref( ).set_ib( 0, 0 );
	backend::ref( ).set_vb( m_vertex_buffer.c_ptr( ), sizeof( vertex_struct ), 0 );
	backend::ref( ).render( D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, num_points, 0 );
}

} // namespace render
} // namespace vostok
