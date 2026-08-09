#include "pch.h"

#include <vostok/geometry_primitives.h>
#include <vostok/render/core/backend.h>
#include <vostok/render/core/resource_manager.h>

#include "box_geometry.h"

namespace vostok {
namespace render {

box_geometry::box_geometry( ) :
	m_stride( sizeof( float3 ) )
{
	m_vertex_buffer = resource_manager::ref( ).create_buffer(
		geometry_utils::cube_solid::vertex_count * m_stride,
		geometry_utils::cube_solid::vertices,
		enum_buffer_type_vertex,
		false,
		false
	);

	u16* indices = static_cast<u16*>( ALLOCA( geometry_utils::cube_solid::index_count * sizeof( u16 ) ) );
	std::copy(
		geometry_utils::cube_solid::faces,
		geometry_utils::cube_solid::faces + geometry_utils::cube_solid::index_count,
		indices
	);

	m_index_buffer = resource_manager::ref( ).create_buffer(
		geometry_utils::cube_solid::index_count * sizeof( u16 ),
		indices,
		enum_buffer_type_index,
		false,
		false
	);

	D3D11_INPUT_ELEMENT_DESC desc[1] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	m_vertext_declaration = resource_manager::ref( ).create_declaration( desc, array_size( desc ) );
}

void box_geometry::draw( )
{
	backend::ref( ).set_declaration( m_vertext_declaration.c_ptr( ) );
	backend::ref( ).set_vb( m_vertex_buffer.c_ptr( ), m_stride );
	backend::ref( ).set_ib( m_index_buffer.c_ptr( ), 0 );
	backend::ref( ).render_indexed( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 36, 0, 0 );
}

} // namespace render
} // namespace vostok
