// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include <vostok/render/core/backend.h>
#include <vostok/render/core/resource_manager.h>

#include "du_sphere.h"
#include "sphere_occluder_geometry.h"

namespace vostok {
namespace render {

sphere_occluder_geometry::sphere_occluder_geometry( )
{
	untyped_buffer_ptr vertex_buffer = resource_manager::ref( ).create_buffer(
		DU_SPHERE_NUMVERTEX * sizeof( float3 ),
		du_sphere_vertices,
		enum_buffer_type_vertex,
		false,
		false
	);
	untyped_buffer_ptr index_buffer = resource_manager::ref( ).create_buffer(
		DU_SPHERE_NUMFACES * 3 * sizeof( u16 ),
		du_sphere_faces,
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

void sphere_occluder_geometry::render( )
{
	m_geometry->apply( );
	backend::ref( ).render_indexed( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 540, 0, 0 );
}

} // namespace render
} // namespace vostok
