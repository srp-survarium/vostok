// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_ENGINE_BATCHED_GEOMETRY_INLINE_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_BATCHED_GEOMETRY_INLINE_H_INCLUDED
#include <vostok/render/core/backend.h>
#include <vostok/render/core/resource_manager.h>
#include <vostok/render/engine/model_format.h>

#include "renderer_context.h"

namespace vostok {
namespace render {

template < typename Vertex >
inline batched_geometry< Vertex >::batched_geometry(
	D3D11_INPUT_ELEMENT_DESC const* layout,
	u32 const num_elements,
	u32 const in_batched_geometry_max_vertices_count
) :
	m_batched_geometry_max_vertices_count	( in_batched_geometry_max_vertices_count ),
	m_bbox									( math::create_zero_aabb( ) )
{
	m_layout = resource_manager::ref( ).create_declaration( layout, num_elements );
	m_indices.reserve( m_batched_geometry_max_vertices_count );
	m_vertices.reserve( m_batched_geometry_max_vertices_count );
}

template < typename Vertex >
inline batched_geometry< Vertex >::~batched_geometry( )
{
	invalidate( );
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
	render_batch_callback_type const&,
	render_batch_callback_type const&,
	u32 const,
	u32 const
)
{
}

template < typename Vertex >
inline u32 batched_geometry< Vertex >::prepare_visible_batches(
	renderer_context*,
	float3 const&,
	bool,
	u32 const
)
{
	return 0;
}

template < typename Vertex >
inline u32 batched_geometry< Vertex >::get_num_visible_batches( u32 const index ) const
{
	return m_num_visible_batches[index];
}

template < typename Vertex >
inline void batched_geometry< Vertex >::for_each_batch_render(
	renderer_context*												context,
	render_batch_callback_type const&	pre_render_predicate,
	render_batch_callback_type const&	post_render_predicate
)
{
	if ( m_geometry_batches.size( ) == 0 )
		return;

	geometry_batch const* end = m_geometry_batches.end( );
	math::frustum view_frustum( context->get_culling_vp( ) );
	for ( geometry_batch* it = m_geometry_batches.begin( ); it != end; ++it )
	{
		if ( view_frustum.test_inexact( it->bbox ) == math::intersection_outside )
			continue;

		it->geometry->apply( );
		pre_render_predicate( *it );
		backend::ref( ).render_indexed(
			D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			it->num_indices,
			0,
			0
		);
		post_render_predicate( *it );
	}
}

template < typename Vertex >
void batched_geometry< Vertex >::finalize_batch( )
{
	if ( !m_vertices.size( ) || !m_indices.size( ) )
		return;

	untyped_buffer_ptr vb = resource_manager::ref( ).create_buffer(
		m_vertices.size( ) * sizeof( Vertex ),
		m_vertices.begin( ),
		enum_buffer_type_vertex,
		false,
		false
	);
	untyped_buffer_ptr ib = resource_manager::ref( ).create_buffer(
		m_indices.size( ) * sizeof( u16 ),
		m_indices.begin( ),
		enum_buffer_type_index,
		false,
		false
	);

	m_geometry_batches.push_back(
		geometry_batch(
			m_bbox,
			resource_manager::ref( ).create_geometry(
				m_layout.c_ptr( ),
				sizeof( Vertex ),
				*vb,
				*ib
			),
			m_indices.size( ),
			m_materail_effects_instance
		)
	);

	m_bbox = math::create_zero_aabb( );
	m_indices.clear( );
	m_vertices.clear( );
}

template < typename Vertex >
inline void batched_geometry< Vertex >::invalidate( )
{
	geometry_batch* it = m_geometry_batches.begin( );
	geometry_batch* end = m_geometry_batches.end( );
	for ( ; it != end; ++it )
	{
		it->geometry = 0;
		it->mtl = 0;
	}

	m_geometry_batches.clear( );
	m_indices.clear( );
	m_vertices.clear( );
}

template < typename Vertex >
void batched_geometry< Vertex >::add_data(
	batched_vertex_source const* vertices,
	u32 const num_vertices,
	u16 const* indices,
	u32 const num_indices,
	float4x4 const& transform,
	material_effects_instance_ptr const& in_materail_effects_instance
)
{
	if ( !vertices || !indices )
		return;

	if (
		m_indices.size( ) + num_indices >= m_batched_geometry_max_vertices_count
		|| m_materail_effects_instance != in_materail_effects_instance
	)
	{
		finalize_batch( );
		m_materail_effects_instance = in_materail_effects_instance;
	}

	for ( u32 i = 0; i < num_indices; ++i )
		m_indices.push_back( m_vertices.size( ) + indices[i] );

	for ( u32 i = 0; i < num_vertices; ++i )
	{
		batched_vertex_source vertex = vertices[i];
		float3 normal = math::normalize_safe(
			float3(
				vertex.normal.get_Rf( ) * 2.f - 1.f,
				vertex.normal.get_Gf( ) * 2.f - 1.f,
				vertex.normal.get_Bf( ) * 2.f - 1.f
			),
			float3( 0.f, 0.f, 0.f )
		);
		float3 not_modified_pos = vertex.position;
		vertex.position = transform.transform_position( vertex.position );
		normal = transform.transform_direction( normal );

		base_basis basis;
		basis.set( normal );
		vertex.normal = math::color( basis.x, basis.y, basis.z, 127 );
		m_bbox.modify( vertex.position );
		m_materail_effects_instance = in_materail_effects_instance;
		add_vertex( vertex, not_modified_pos );
	}
}

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_BATCHED_GEOMETRY_INLINE_H_INCLUDED
