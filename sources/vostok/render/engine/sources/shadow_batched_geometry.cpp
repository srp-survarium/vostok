#include "pch.h"
#include "shadow_batched_geometry.h"

#include "render_surface.h"
#include "render_surface_instance.h"

namespace vostok {
namespace render {

D3D11_INPUT_ELEMENT_DESC shadow_layout[] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

shadow_batched_geometry::shadow_batched_geometry( ) :
	batched_geometry<shadow_vertex>( shadow_layout, array_size( shadow_layout ), 32768 )
{
}

void shadow_batched_geometry::add_vertex(
	batched_vertex_source const& in_vertex,
	float3 const& not_modified_position
)
{
	shadow_vertex vertex;
	vertex.position = in_vertex.position;
	vertex.uv = in_vertex.uv;
	vertex.object_position = not_modified_position;
	m_vertices.push_back( vertex );
}

void shadow_batched_geometry::build(
	vector<render_model_instance_impl_ptr>& model_instances
)
{
	struct surface_set {
		float4x4		transform;
		render_surface*	surface;
	};

	typedef vector<surface_set> surfaces_type;

	invalidate( );

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
			if ( !( *it_surf )->m_render_surface->m_materail_effects_instance )
			{
				continue;
			}

			surface_set set;
			set.transform = *( *it_surf )->m_transform;
			set.surface = ( *it_surf )->m_render_surface;
			surfaces.push_back( set );
		}
	}

	struct sort_predicate {
		bool operator()( surface_set const& left, surface_set const& right ) const
		{
			return left.surface->m_materail_effects_instance < right.surface->m_materail_effects_instance;
		}
	};

	std::sort( surfaces.begin( ), surfaces.end( ), sort_predicate( ) );

	surface_set* surface = surfaces.begin( );
	for ( ; surface != surfaces.end( ); ++surface )
		surface->surface->add_shadow_vertices( this, surface->transform );

	finalize_batch( );
}

} // namespace render
} // namespace vostok
