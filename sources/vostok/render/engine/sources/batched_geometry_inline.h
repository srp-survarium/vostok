#ifndef VOSTOK_RENDER_ENGINE_BATCHED_GEOMETRY_INLINE_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_BATCHED_GEOMETRY_INLINE_H_INCLUDED

#include <vostok/render/core/backend.h>

#include "renderer_context.h"

namespace vostok {
namespace render {

template < typename Vertex >
inline batched_geometry< Vertex >::batched_geometry(
	D3D11_INPUT_ELEMENT_DESC const*,
	u32,
	u32 in_batched_geometry_max_vertices_count
) :
	m_batched_geometry_max_vertices_count	( in_batched_geometry_max_vertices_count ),
	m_bbox									( math::create_zero_aabb( ) )
{
	// STATE[STUB]
	// FUNCTION BODY[0x891e0] for <lpv_vertex>
	// FUNCTION BODY[0x894b0] for <shadow_vertex>

	for ( u32 i = 0; i < 8; ++i )
		m_num_visible_batches[i] = 0;
}

template < typename Vertex >
inline batched_geometry< Vertex >::~batched_geometry( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x694f0] for <lpv_vertex>
}

template < typename Vertex >
inline batched_geometry< Vertex >::batched_geometry( ) :
	m_batched_geometry_max_vertices_count	( 0 ),
	m_bbox									( math::create_zero_aabb( ) )
{
	for ( u32 i = 0; i < 8; ++i )
		m_num_visible_batches[i] = 0;
}

template < typename Vertex >
inline void batched_geometry< Vertex >::for_each_batch_visible_render_first_and_remove(
	boost::function<void ( geometry_batch const& )> const&,
	boost::function<void ( geometry_batch const& )> const&,
	u32,
	u32
)
{
}

template < typename Vertex >
inline u32 batched_geometry< Vertex >::prepare_visible_batches(
	renderer_context*,
	float3 const&,
	bool,
	u32
)
{
	return 0;
}

template < typename Vertex >
inline u32 batched_geometry< Vertex >::get_num_visible_batches( u32 index ) const
{
	return m_num_visible_batches[index];
}

template < typename Vertex >
inline void batched_geometry< Vertex >::for_each_batch_render(
	renderer_context*												context,
	boost::function<void ( geometry_batch const& )> const&	pre_render,
	boost::function<void ( geometry_batch const& )> const&	post_render
)
{
	// FUNCTION BODY[0x7d2d0] for <lpv_vertex>
	if ( m_geometry_batches.size( ) == 0 )
		return;

	geometry_batch const* end = m_geometry_batches.end( );
	math::frustum view_frustum( context->get_culling_vp( ) );
	for ( geometry_batch* it = m_geometry_batches.begin( ); it != end; ++it )
	{
		if ( view_frustum.test_inexact( it->bbox ) == math::intersection_outside )
			continue;

		it->geometry->apply( );
		pre_render( *it );
		backend::ref( ).render_indexed(
			D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			it->num_indices,
			0,
			0
		);
		post_render( *it );
	}
}

template < typename Vertex >
inline void batched_geometry< Vertex >::finalize_batch( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6ae50] for <lpv_vertex>
}

template < typename Vertex >
inline void batched_geometry< Vertex >::invalidate( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6b0d0] for <lpv_vertex>
}

template < typename Vertex >
inline void batched_geometry< Vertex >::add_data(
	batched_vertex_source const*,
	u32,
	u16 const*,
	u32,
	float4x4 const&,
	material_effects_instance_ptr const&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x69600] for <lpv_vertex>
}

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_BATCHED_GEOMETRY_INLINE_H_INCLUDED
