// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include "lpv_batched_geometry.h"
#include "render_surface.h"
#include "render_surface_instance.h"

namespace vostok {
namespace render {

D3D11_INPUT_ELEMENT_DESC const lpv_layout[] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

lpv_batched_geometry::lpv_batched_geometry( ) :
	batched_geometry<lpv_vertex>( lpv_layout, array_size( lpv_layout ), 65536 )
{
}

void lpv_batched_geometry::add_vertex(
	batched_vertex_source const& in_vertex,
	float3 const&
)
{
	lpv_vertex vertex;
	vertex.position = in_vertex.position;
	vertex.normal = in_vertex.normal;
	vertex.clr = in_vertex.clr;
	m_vertices.push_back( vertex );
}

void lpv_batched_geometry::build(
	vector<render_model_instance_impl_ptr>& model_instances
)
{
	typedef vector<float4x4> surface_matrices_type;
	typedef vector<render_surface*> surfaces_type;

	invalidate( );

	surface_matrices_type matrices;
	surfaces_type surfaces;
	render_model_instance_impl_ptr* it = model_instances.begin( );
	render_model_instance_impl_ptr* end = model_instances.end( );
	for ( ; it != end; ++it )
	{
		vector<render_surface_instance*> model_surfaces;
		( *it )->get_surfaces( NULL, NULL, model_surfaces, false, 0, 3 );

		render_surface_instance** it_surf = model_surfaces.begin( );
		render_surface_instance** end_surf = model_surfaces.end( );
		for ( ; it_surf != end_surf; ++it_surf )
		{
			surfaces.push_back( ( *it_surf )->m_render_surface );
			matrices.push_back( *( *it_surf )->m_transform );
		}
	}

	render_surface** surface = surfaces.begin( );
	float4x4* matrix = matrices.begin( );
	for ( ; surface != surfaces.end( ); ++surface, ++matrix )
		( *surface )->fill_lpv_vertex_color( this, *matrix );

	finalize_batch( );
}

} // namespace render
} // namespace vostok
