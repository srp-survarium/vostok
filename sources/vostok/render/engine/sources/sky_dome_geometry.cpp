#include "pch.h"

#include <vostok/render/core/backend.h>
#include <vostok/render/core/resource_manager.h>

#include "sky_dome_geometry.h"

namespace vostok {
namespace render {

const D3D11_INPUT_ELEMENT_DESC vertex_layout[] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

sky_dome_geometry::sky_dome_geometry( ) :
	m_stride		( sizeof( vertex_type ) )
{
	u32 const num_vertices = 256;
	u32 const num_indices = 1260;

	vertex_type vertices[256];
	u16 indices[1260];

	u32 vertex_index = 0;

	for ( float i = 0.f; i < 16.f; i += 1.f )
	{
		float const move_xz = i / 15.f * math::deg2rad( 100.f );
		for ( float j = 0.f; j < 8.f; j += 1.f )
		{
			float const move_y = j / 7.f * math::pi;
			vertices[vertex_index].position.set(
				math::cos( move_y ) * math::cos( move_xz ),
				math::sin( move_xz ),
				math::sin( move_y ) * math::cos( move_xz ),
				1.f
			);
			vertices[vertex_index].position *= 10.f;
			vertices[vertex_index].uv.set(
				i / 16.f + 0.5f / 16.f,
				j / 8.f + 0.5f / 8.f
			);
			++vertex_index;
		}
	}

	for ( float i = 0.f; i < 16.f; i += 1.f )
	{
		float const move_xz = i / 15.f * math::deg2rad( 100.f );
		for ( float j = 0.f; j < 8.f; j += 1.f )
		{
			float const move_y = math::pi_x2 - j / 7.f * math::pi;
			vertices[vertex_index].position.set(
				math::cos( move_y ) * math::cos( move_xz ),
				math::sin( move_xz ),
				math::sin( move_y ) * math::cos( move_xz ),
				1.f
			);
			vertices[vertex_index].position *= 10.f;
			vertices[vertex_index].uv.set(
				i / 16.f + 0.5f / 16.f,
				j / 8.f + 0.5f / 8.f
			);
			++vertex_index;
		}
	}

	u16* index = indices;
	for ( u16 i = 1; i < 16; ++i )
	{
		for ( u16 j = 1; j < 8; ++j )
		{
			*( index++ ) = ( i - 1 ) * 8 + j - 1;
			*( index++ ) = i * 8 + j - 1;
			*( index++ ) = i * 8 + j;
			*( index++ ) = i * 8 + j;
			*( index++ ) = ( i - 1 ) * 8 + j;
			*( index++ ) = ( i - 1 ) * 8 + j - 1;
		}
	}

	for ( u16 i = 1; i < 16; ++i )
	{
		for ( u16 j = 1; j < 8; ++j )
		{
			*( index++ ) = 128 + ( i - 1 ) * 8 + j - 1;
			*( index++ ) = 128 + i * 8 + j;
			*( index++ ) = 128 + i * 8 + j - 1;
			*( index++ ) = 128 + ( i - 1 ) * 8 + j;
			*( index++ ) = 128 + i * 8 + j;
			*( index++ ) = 128 + ( i - 1 ) * 8 + j - 1;
		}
	}

	m_vertext_declaration = resource_manager::ref( ).create_declaration(
		vertex_layout,
		array_size( vertex_layout )
	);
	m_vertex_buffer = resource_manager::ref( ).create_buffer(
		m_stride * num_vertices,
		vertices,
		enum_buffer_type_vertex,
		false,
		false
	);
	m_index_buffer = resource_manager::ref( ).create_buffer(
		sizeof(u16) * num_indices,
		indices,
		enum_buffer_type_index,
		false,
		false
	);
	m_num_indices = num_indices;
}

void sky_dome_geometry::draw( )
{
	backend::ref( ).set_declaration( m_vertext_declaration.c_ptr( ) );
	backend::ref( ).set_vb( m_vertex_buffer.c_ptr( ), m_stride );
	backend::ref( ).set_ib( m_index_buffer.c_ptr( ), 0 );
	backend::ref( ).render_indexed(
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		m_num_indices,
		0,
		0
	);
}

} // namespace render
} // namespace vostok
