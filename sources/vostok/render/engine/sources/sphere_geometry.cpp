// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include <vostok/render/core/backend.h>
#include <vostok/render/core/resource_manager.h>

#include "sphere_geometry.h"

namespace vostok {
namespace render {

const D3D_INPUT_ELEMENT_DESC sphere_geometry_vertex_layout[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 0,	D3D_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,			0, 16,	D3D_INPUT_PER_VERTEX_DATA, 0 },
};

sphere_geometry::sphere_geometry( u32 const num_sides, u32 const num_rings ) :
	m_stride		( sizeof( vertex_type ) )
{
	u32 const num_vertices = (num_sides + 1) * (num_rings + 1);
	u32 const num_indices = num_sides * num_rings * 6;

	vertex_type* vertices = static_cast<vertex_type*>( ALLOCA( m_stride * num_vertices ) );
	vertex_type* side_vertices = static_cast<vertex_type*>( ALLOCA( m_stride * (num_rings + 1) ) );
	vertex_type* vertices_it = vertices;
	u16* indices = static_cast<u16*>( ALLOCA( sizeof(u16) * num_indices ) );
	u16* indices_it = indices;

	for ( u32 i = 0; i < num_rings + 1; ++i )
		new (vertices_it++) vertex_type;

	for ( u32 i = 0; i < num_rings + 1; ++i )
	{
		float const angle = float(i) / float(num_rings) * math::pi;
		side_vertices[i].position.x = math::sin( angle );
		side_vertices[i].position.y = math::cos( angle );
		side_vertices[i].position.z = 0.0f;
		side_vertices[i].position.w = 1.0f;
		side_vertices[i].uv.x = 0.0f;
		side_vertices[i].uv.y = float(i) / float(num_rings);
	}

	vertices_it = vertices;
	u32 s;
	for ( s = 0; s < num_sides + 1; ++s, vertices_it += num_rings + 1 )
	{
		float4x4 rotation_matrix = math::create_rotation_y( float(s) * math::pi_x2 / float(num_sides) );
		float texcoord_u = float(s) / float(num_sides);

		for ( u32 i = 0; i < num_rings + 1; ++i )
		{
			vertices_it[i].position = float4( rotation_matrix.transform_position( side_vertices[i].position.xyz() ), 1.f );
			vertices_it[i].uv.x = texcoord_u;
			vertices_it[i].uv.y = side_vertices[i].uv.y;
		}
	}

	for ( s = 0; s < num_sides; ++s )
	{
		for ( u32 i = 0; i < num_rings; ++i )
		{
			*(indices_it++) = static_cast<u16>( s * (num_rings + 1) + i );
			*(indices_it++) = static_cast<u16>( s * (num_rings + 1) + i + 1 );
			*(indices_it++) = static_cast<u16>( (s + 1) * (num_rings + 1) + i );
			*(indices_it++) = static_cast<u16>( (s + 1) * (num_rings + 1) + i );
			*(indices_it++) = static_cast<u16>( s * (num_rings + 1) + i + 1 );
			*(indices_it++) = static_cast<u16>( (s + 1) * (num_rings + 1) + i + 1 );
		}
	}

	m_vertext_declaration = resource_manager::ref( ).create_declaration(
		sphere_geometry_vertex_layout,
		array_size( sphere_geometry_vertex_layout )
	);
	m_vertex_buffer = resource_manager::ref( ).create_buffer(
		num_vertices * m_stride,
		vertices,
		enum_buffer_type_vertex,
		false,
		false
	);
	m_index_buffer = resource_manager::ref( ).create_buffer(
		num_indices * sizeof(u16),
		indices,
		enum_buffer_type_index,
		false,
		false
	);
	m_num_indices = num_indices;
}

void sphere_geometry::draw( )
{
	backend::ref( ).set_declaration( m_vertext_declaration.c_ptr( ) );
	backend::ref( ).set_vb( m_vertex_buffer.c_ptr( ), m_stride );
	backend::ref( ).set_ib( m_index_buffer.c_ptr( ), 0 );
	backend::ref( ).render_indexed( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, m_num_indices, 0, 0 );
}

} // namespace render
} // namespace vostok
