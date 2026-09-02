// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include <vostok/render/core/dx11/decl_utils.h>
#include <vostok/render/core/resource_manager.h>
#include <vostok/render/engine/model_format.h>

#pragma warning( push )
#pragma warning( disable : 4995 )
#include <d3dx9mesh.h>
#pragma warning( pop )

#pragma comment( lib, "d3dx9.lib" )

#include "render_model_grass.h"

namespace vostok {
namespace render {

grass_render_surface::grass_render_surface( ) :
	m_vertices( 0 ),
	m_num_vertices( 0 ),
	m_indices( 0 ),
	m_num_indices( 0 )
{
	m_vertex_input_type = grassmesh_vertex_input_type;
}

void grass_render_surface::load(
	configs::binary_config_value const& properties,
	memory::chunk_reader& chunk
)
{
	render_surface::load( properties, chunk );

	memory::reader reader = chunk.open_reader( model_chunk_vertices );
	D3DVERTEXELEMENT9 const* vertex_format =
		reinterpret_cast<D3DVERTEXELEMENT9 const*>( reader.pointer( ) );
	u32 const declaration_length = D3DXGetDeclLength( vertex_format ) + 1;

	vector<D3D11_INPUT_ELEMENT_DESC> decl_code;
	decl_utils::convert_vertex_declaration(
		vector<D3DVERTEXELEMENT9>(
			vertex_format,
			vertex_format + declaration_length
		),
		decl_code
	);

	res_declaration_ptr decl = resource_manager::ref( ).create_declaration(
		decl_code.begin( ),
		decl_code.size( )
	);
	reader.advance( declaration_length * sizeof( D3DVERTEXELEMENT9 ) );

	m_num_vertices = reader.r_u32( );
	u32 const vertex_stride = D3DXGetDeclVertexSize( vertex_format, 0 );
	m_vertices = ALLOC( grass_source_vertex, m_num_vertices );
	memory::copy(
		m_vertices,
		m_num_vertices * sizeof( grass_source_vertex ),
		reader.pointer( ),
		m_num_vertices * vertex_stride
	);

	reader = chunk.open_reader( model_chunk_indices );
	m_num_indices = reader.r_u32( );
	m_indices = ALLOC( u16, m_num_indices );
	memory::copy(
		m_indices,
		m_num_indices * sizeof( u16 ),
		reader.pointer( ),
		m_num_indices * sizeof( u16 )
	);
}

grass_render_surface::~grass_render_surface( )
{
	FREE( m_vertices );
	FREE( m_indices );
}

void grass_render_model::set_children(
	render_surface** children,
	u8 count,
	model_lods_descriptor* lods
)
{
	render_model::set_children(children, count, lods);

	for (u32 i = 0; i < 3; ++i)
	{
		if (!lods->m_lod_surfaces_count[i])
			continue;

		u8 surface_index = lods->m_lod_surfaces[i][0];
		switch (i)
		{
		case 0: m_l0 = static_cast<grass_render_surface*>(children[surface_index]);
		case 1: m_l1 = static_cast<grass_render_surface*>(children[surface_index]);
		case 2: m_l2 = static_cast<grass_render_surface*>(children[surface_index]);
		}
	}

	if (!m_l0)
		m_l0 = static_cast<grass_render_surface*>(children[0]);
}

} // namespace render
} // namespace vostok
