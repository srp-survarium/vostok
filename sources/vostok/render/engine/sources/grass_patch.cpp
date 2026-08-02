#include "pch.h"
#include <vostok/collision/api.h>
#include <vostok/collision/space_partitioning_tree.h>
#include <vostok/render/core/backend.h>
#include <vostok/render/core/options.h>
#include <vostok/render/core/res_effect.h>
#include "grass_patch.h"
#include "grass_world.h"
#include "renderer.h"
#include "system_renderer.h"

namespace vostok {
namespace render {

struct stream_1_type {
	float3	object_position;
	u32		color_and_wind;
};

STATIC_SIZE_ASSERT( stream_1_type, 0x10 );

struct sort_indices_predicate {
	sort_indices_predicate( grass_patch* patch, float3 const& view_position ) :
		m_patch( patch ),
		m_view_pos( view_position )
	{
	}

	bool operator()(
		grass_patch::sort_info const&,
		grass_patch::sort_info const&
	) const
	{
		return false;
	}

private:
	grass_patch* m_patch;
	float3 m_view_pos;
};

bool has_surface_by_lod( grass_render_model_ptr model, u32 lod_index )
{
	// FUNCTION BODY[0x5f43e0]
	return surface_by_lod( model, lod_index ) != 0;
}

grass_render_surface* surface_by_lod( grass_render_model_ptr model, u32 lod_index )
{
	// FUNCTION BODY[0x5f4310]
	if ( !model )
		return 0;

	switch ( lod_index ) {
	case 0: return model->m_l0;
	case 1: return model->m_l1;
	case 2: return model->m_l2;
	default: return 0;
	}
}

grass_patch::grass_patch(
	collision::space_partitioning_tree* in_collision_tree,
	grass_template* templ,
	float3 const& in_origin,
	float in_size
) :
	m_prev_view_pos( 0.f, 0.f, 0.f ),
	m_aabb( math::create_zero_aabb( ) ),
	m_origin( in_origin ),
	m_size( in_size ),
	m_occlusion_info_index( 0 ),
	m_current_lod_index( 0 ),
	m_num_avaliable_lods( 0 ),
	m_template( templ ),
	m_collision_tree( in_collision_tree ),
	m_collision_geometry( 0 ),
	m_collision_object( 0 ),
	m_visible( false ),
	m_occluded( false )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f5280]
	for ( u32 i = 0; i != 3; ++i ) {
		m_merged_indices[i] = 0;
		m_sort_info[i] = 0;
		m_num_merged_vertices[i] = 0;
		m_num_merged_indices[i] = 0;
	}
}

void grass_patch::remove_trample( )
{
	backend::ref( ).set_render_targets( &*m_movement_rt, 0, 0, 0 );
	backend::ref( ).clear_render_targets( 0.f, 0.f, 0.f, 0.f );
}

void grass_patch::try_accumulate_trample(
	trample_desc& desc,
	grass_world* in_grass_world,
	renderer* in_renderer,
	renderer_context*
)
{
	if ( !options::ref( ).current.m_use_vegetation_trample )
		return;

	float3 const position				= (desc.position - (m_origin - float3( 8.f, 8.f, 8.f ))) * (1.f / 16.f) - float3( 1.f / 63.f, 1.f / 63.f, 1.f / 63.f );
	float const linear_radius		= desc.radius * (1.f / 16.f);

	if (
		position.x < 0.f || position.y < -.25f || position.z < 0.f ||
		position.x > 1.f || position.y > 1.f || position.z > 1.f
	)
		return;

	in_renderer->get_grass_trample_effect( )->apply( 0, 0 );
	in_grass_world->set_trample_parameters( desc );

	D3D11_VIEWPORT view_port;
	view_port.TopLeftX					= 0.f;
	view_port.TopLeftY					= 0.f;
	view_port.Width						= 128.f;
	view_port.Height						= 128.f;
	view_port.MinDepth					= 0.f;
	view_port.MaxDepth					= 1.f;

	system_renderer::ref( ).fill_surface(
		m_movement_rt,
		render_target_ptr( ),
		render_target_ptr( ),
		render_target_ptr( ),
		render_target_ptr( ),
		false,
		&view_port,
		position.x - linear_radius,
		position.z - linear_radius,
		linear_radius + linear_radius,
		linear_radius + linear_radius
	);
}

grass_patch::~grass_patch( )
{
	// FUNCTION BODY[0x5f4480]
	// claude@NOTE: legacy dtor; freeing of the canonical merged-stream arrays
	// (m_merged_indices/m_sort_info) is matcher-phase.
	if (m_collision_tree && m_collision_object)
		m_collision_tree->erase					(m_collision_object);

	collision::delete_object					(g_allocator, m_collision_object);
	collision::delete_geometry_instance			(g_allocator, m_collision_geometry);
}

void grass_patch::init_collision( )
{
	// FUNCTION BODY[0x5f3ba0]
	// claude@NOTE: adapted from the legacy ctor tail (collision setup moved into
	// init_collision; legacy float3 m_sizes became the scalar m_size).
	float3 const sizes							=	float3(m_size, m_size, m_size);
	math::float4x4 const transform				=	math::create_scale(sizes * 0.5f) * math::create_translation(m_origin + sizes * 0.5f);
	m_collision_geometry						=	&*collision::new_box_geometry_instance(g_allocator, transform);
	m_collision_object							=	&*collision::new_collision_object(g_allocator, 1, m_collision_geometry, this);

	m_collision_tree->insert					(m_collision_object, transform);
	m_aabb.modify								(transform);
}

u32 grass_patch::get_stream_1_stride( ) const
{
	// FUNCTION BODY[0x5f3ae0]
	// claude@NOTE: legacy returned sizeof(grass_instance_data); canonical stream layout.
	return sizeof(stream_1_type);
}

u32 grass_patch::get_index_count( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f3ad0]
	return 0;
}

u32 grass_patch::get_valid_lod_index( u32 ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f3ab0]
	return 0;
}

void grass_patch::render(
	grass_world*,
	renderer_context*,
	float3 const&,
	enum_render_stage_type,
	u32,
	float,
	res_effect*,
	u32
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f4df0]
}

void grass_patch::sort_instances( float3 const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f41d0]
}

math::color transform_packed_normal( float4x4 const&, math::color const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f40a0]
	return math::color( );
}

void grass_patch::merge_instances( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f4700]
}

bool grass_patch::is_occluded( ) const
{
	// FUNCTION BODY[0x5f3af0]
	return m_occluded;
}

} // namespace render
} // namespace vostok
