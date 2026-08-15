#include "pch.h"
#include <vostok/collision/api.h>
#include <vostok/collision/space_partitioning_tree.h>
#include <vostok/render/core/backend.h>
#include <vostok/render/core/options.h>
#include <vostok/render/core/res_effect.h>
#include <vostok/render/core/resource_manager.h>
#include <vostok/render/engine/model_format.h>
#include "grass_patch.h"
#include "grass_world.h"
#include "renderer.h"
#include "renderer_context.h"
#include "scene_view.h"
#include "statistics.h"
#include "system_renderer.h"

namespace vostok {
namespace render {

struct stream_1_type {
	float3	object_position;
	u32		color_and_wind;
};

STATIC_SIZE_ASSERT( stream_1_type, 0x10 );

static D3D11_INPUT_ELEMENT_DESC const layout[] = {
	{ "POSITION",       0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",         0, DXGI_FORMAT_R8G8B8A8_UNORM,  0, 12, D3D_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT",        0, DXGI_FORMAT_R8G8B8A8_UNORM,  0, 16, D3D_INPUT_PER_VERTEX_DATA, 0 },
	{ "BINORMAL",       0, DXGI_FORMAT_R8G8B8A8_UNORM,  0, 20, D3D_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",       0, DXGI_FORMAT_R32G32_FLOAT,     0, 24, D3D_INPUT_PER_VERTEX_DATA, 0 },
	{ "OBJECT_POSITON", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1,  0, D3D_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR_AND_WIND", 0, DXGI_FORMAT_R8G8B8A8_UNORM,  1, 12, D3D_INPUT_PER_VERTEX_DATA, 0 }
};

struct sort_indices_predicate {
	sort_indices_predicate( grass_patch* patch, float3 const& view_position ) :
		m_patch( patch ),
		m_view_pos( view_position )
	{
	}

	bool operator()(
		grass_patch::sort_info const& left,
		grass_patch::sort_info const& right
	) const
	{
		return math::squared_length( left.position - m_view_pos ) <
			math::squared_length( right.position - m_view_pos );
	}

private:
	grass_patch* m_patch;
	float3 m_view_pos;
};

static grass_render_surface* surface_by_lod( grass_render_model_ptr model, u32 const lod_index );

static bool has_surface_by_lod( grass_render_model_ptr model, u32 const lod_index )
{
	return surface_by_lod( model, lod_index ) != 0;
}

static grass_render_surface* surface_by_lod( grass_render_model_ptr model, u32 const lod_index )
{
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
	collision::space_partitioning_tree* const in_collision_tree,
	grass_template* templ,
	float3 const& in_origin,
	float const in_size
) :
	m_prev_view_pos( 0.f, 0.f, 0.f ),
	m_aabb( math::create_aabb_min_max( float3( -1.f, -1.f, -1.f ), float3( 0.f, 0.f, 0.f ) ) ),
	m_origin( in_origin ),
	m_size( 16.f ),
	m_occlusion_info_index( u32( -1 ) ),
	m_current_lod_index( 0 ),
	m_template( templ ),
	m_collision_tree( in_collision_tree ),
	m_collision_geometry( 0 ),
	m_collision_object( 0 ),
	m_visible( true ),
	m_occluded( false )
{
	VOSTOK_UNREFERENCED_PARAMETER( in_size );

	m_merged_indices[0] = 0; m_sort_info[0] = 0; m_merged_indices[1] = 0; m_sort_info[1] = 0; m_merged_indices[2] = 0; m_sort_info[2] = 0;

	if ( options::ref( ).current.m_use_vegetation_trample ) {
		m_movement_rt = resource_manager::ref( ).create_render_target(
			0,
			64,
			64,
			DXGI_FORMAT_R8_UNORM,
			enum_rt_usage_render_target,
			res_texture_ptr( ),
			0,
			D3D11_USAGE_DEFAULT,
			1,
			0
		);
		m_movement_texture = m_movement_rt->get_texture( );
	}

	m_aabb = math::create_aabb_center_radius(
		m_origin,
		float3( m_size * .5f, .1f, m_size * .5f )
	);
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
	view_port.Width						= 64.f;
	view_port.Height						= 64.f;
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
	for ( u32 i = 0; i < num_lods; ++i ) {
		FREE( m_merged_indices[i] );
		FREE( m_sort_info[i] );
	}

	if (m_collision_tree && m_collision_object)
		m_collision_tree->erase					(m_collision_object);

	collision::delete_object					(g_allocator, m_collision_object);
	collision::delete_geometry_instance			(g_allocator, m_collision_geometry);
}

void grass_patch::init_collision( )
{
	static math::random32 s_randomizer( 100000 );

	m_aabb.min.y = math::float_max;
	m_aabb.max.y = math::float_min;

	for ( grass_instance** it = m_instances.begin( ); it != m_instances.end( ); ++it ) {
		m_aabb.modify( (*it)->m_transform.c.xyz( ) );
	}

	m_aabb.min.y -= .2f;
	m_aabb.max.y += 3.f;
	m_origin = m_aabb.center( );
	m_origin += float3(
		s_randomizer.random_f( .1f ) - .25f,
		s_randomizer.random_f( .1f ) - .25f,
		s_randomizer.random_f( .1f ) - .25f
	);

	math::float4x4 transform = math::create_scale( m_aabb.extents( ) ) *
		math::create_translation( m_origin );
	m_collision_geometry						=	&*collision::new_box_geometry_instance(g_allocator, transform);
	m_collision_object							=	&*collision::new_collision_object(g_allocator, 1, m_collision_geometry, this);

	m_collision_tree->insert					(m_collision_object, transform);
}

u32 grass_patch::get_stream_1_stride( ) const
{
	return sizeof(stream_1_type);
}

u32 grass_patch::get_index_count( ) const
{
	return m_num_merged_indices[m_current_lod_index];
}

u32 grass_patch::get_valid_lod_index( u32 const lod_index ) const
{
	if ( !m_num_avaliable_lods )
		return 0;

	if ( lod_index >= m_num_avaliable_lods )
		return m_num_avaliable_lods - 1;

	return lod_index;
}

void grass_patch::render(
	grass_world*				in_grass_world,
	renderer_context*			context,
	float3 const&				viewer_position,
	enum_render_stage_type		stage_type,
	u32 const					tech_index,
	float const					draw_distance,
	res_effect*					debug_effect,
	u32 const					cascade_index
)
{
	float3 const aabb_center = m_aabb.center( );
	float const to_aabb_center_squared = math::squared_length( viewer_position - aabb_center );
	grass_render_surface* surface = surface_by_lod( m_template->m_render_model, m_current_lod_index );

	if ( !surface )
		return;

	if (
		( stage_type == sun_shadows_accumulate_render_stage || stage_type == shadow_render_stage ) &&
		!surface->get_material_effects( ).is_cast_shadow
	)
		return;

	if ( !surface->get_material_effects( ).m_effects[stage_type].c_ptr( ) )
		return;

	if ( to_aabb_center_squared > draw_distance * draw_distance )
		return;

	surface->get_material_effects( ).m_effects[stage_type]->apply( tech_index, 0 );
	float4x4 identity;
	identity.identity( );
	context->set_w( identity );
	m_geometry[m_current_lod_index]->apply( );
	backend::ref( ).set_vb_stream_1( &*m_vb_stream_1[m_current_lod_index], sizeof( stream_1_type ) );

	if ( options::ref( ).current.m_use_vegetation_trample ) {
		backend::ref( ).set_ps_texture( "t_grass_motion_mask", &*m_movement_texture );
		in_grass_world->set_patch_parameters( this );
	}

	post_process_parameters const& parameters = context->scene_view( )->post_process_parameters( );
	in_grass_world->set_wind_parameters(
		float2( parameters.wind_direction.x, parameters.wind_direction.z ),
		parameters.wind_strength
	);
	if ( stage_type == sun_shadows_accumulate_render_stage )
		in_grass_world->set_shadow_parameters( cascade_index );

	backend::ref( ).render_indexed(
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		m_num_merged_indices[m_current_lod_index],
		0,
		0
	);
	++statistics::ref( ).grass_stat_group.num_rendered_patches.value;
}

void grass_patch::sort_instances( float3 const& view_position )
{
	u32 const lod_index = m_current_lod_index;
	std::sort(
		m_sort_info[lod_index],
		m_sort_info[lod_index] + m_instances.size( ) - 1,
		sort_indices_predicate( this, view_position )
	);

	u16* merged_indices_copy = ALLOC(
		u16,
		m_num_merged_indices[lod_index]
	);
	u16* merged_indices_copy_it = merged_indices_copy;
	for ( u32 i = 0; i < m_instances.size( ); ++i ) {
		sort_info const& info = m_sort_info[lod_index][i];
		memcpy(
			merged_indices_copy_it,
			m_merged_indices[lod_index] + info.index_offset,
			info.num_indices * sizeof( u16 )
		);
		merged_indices_copy_it += info.num_indices;
	}

	device::ref( ).d3d_context( )->UpdateSubresource(
		m_geometry[lod_index]->m_ib->hardware_buffer( ),
		0,
		0,
		merged_indices_copy,
		0,
		0
	);
	FREE( merged_indices_copy );
}

static math::color transform_packed_normal(
	float4x4 const& transform_matrix,
	math::color const& packed_normal
)
{
	base_basis basis;
	basis.set(
		transform_matrix.transform_direction(
			float3(
				packed_normal.get_Rf( ) * 2.f - 1.f,
				packed_normal.get_Gf( ) * 2.f - 1.f,
				packed_normal.get_Bf( ) * 2.f - 1.f
			)
		)
	);
	return math::color( basis.x, basis.y, basis.z, 127 );
}

void grass_patch::merge_instances( )
{
	if ( m_instances.empty( ) )
		return;

	m_num_avaliable_lods				=	0;
	for ( u32 lod_index = 0; lod_index < 3; ++lod_index )
	{
		FREE								( m_merged_indices[lod_index] );
		m_num_merged_vertices[lod_index]	=	0;
		m_num_merged_indices[lod_index]	=	0;

		grass_instance* const* it		=	m_instances.begin( );
		grass_instance* const* end		=	m_instances.end( );
		for ( ; it != end; ++it )
		{
			if ( has_surface_by_lod( (*it)->m_template->m_render_model, lod_index ) )
				break;
		}

		if ( it == end )
			continue;

		for ( it = m_instances.begin( ); it != end; ++it )
		{
			grass_render_surface* surface	=	surface_by_lod(
				(*it)->m_template->m_render_model,
				lod_index
			);
			if ( !surface )
				continue;

			m_num_merged_vertices[lod_index] += surface->m_num_vertices;
			m_num_merged_indices[lod_index] += surface->m_num_indices;
		}

		m_num_merged_vertices[lod_index]	=	math::min(
			m_num_merged_vertices[lod_index],
			65536u
		);

		stream_1_type* stream_1_data		=	ALLOC(
			stream_1_type,
			m_num_merged_vertices[lod_index]
		);
		stream_1_type* stream_1_data_it	=	stream_1_data;
		grass_source_vertex* merged_vertices = ALLOC(
			grass_source_vertex,
			m_num_merged_vertices[lod_index]
		);
		grass_source_vertex* merged_vertices_it = merged_vertices;
		m_merged_indices[lod_index]		=	ALLOC(
			u16,
			m_num_merged_indices[lod_index]
		);

		u16* merged_indices_it			=	m_merged_indices[lod_index];
		u32 current_num_vertices		=	0;
		u32 current_num_indices			=	0;

		FREE								( m_sort_info[lod_index] );
		m_sort_info[lod_index]			=	ALLOC( sort_info, m_instances.size( ) );
		sort_info* sort_info_it			=	m_sort_info[lod_index];

		for ( it = m_instances.begin( ); it != end; ++it )
		{
			grass_instance& current_instance = **it;
			grass_render_surface* surface	=	surface_by_lod(
				current_instance.m_template->m_render_model,
				lod_index
			);
			if ( !surface )
				continue;

			if ( current_num_vertices + surface->m_num_vertices >= 65536u )
				continue;

			sort_info_it->position			=	current_instance.m_transform.transform_position(
				current_instance.m_template->m_render_model->m_aabbox.center( )
			);
			sort_info_it->index_offset		=	current_num_indices;
			sort_info_it->num_indices		=	surface->m_num_indices;
			++sort_info_it;

			u32 const copy_i_bytes			=	surface->m_num_indices * sizeof( u16 );
		memcpy(
				merged_vertices_it,
				surface->m_vertices,
				surface->m_num_vertices * sizeof( grass_source_vertex )
			);
			memcpy( merged_indices_it, surface->m_indices, copy_i_bytes );

			math::color color_and_wind		=	current_instance.m_color;
			color_and_wind.a				=	u8(
				math::clamp_r( current_instance.m_wind_scale, 0.f, 1.f ) * 255.f
			);

			for ( u32 i = 0; i < surface->m_num_indices; ++i )
			{
				*merged_indices_it			+=	u16( current_num_vertices ); ++merged_indices_it;
			}

			for ( u32 i = 0; i < surface->m_num_vertices; ++i )
			{
				stream_1_data_it->color_and_wind = color_and_wind.m_value;
				stream_1_data_it->object_position = merged_vertices_it->position;
				merged_vertices_it->position	=	current_instance.m_transform.transform_position(
					merged_vertices_it->position
				);
				merged_vertices_it->normal	=	transform_packed_normal(
					current_instance.m_transform,
					merged_vertices_it->normal
				);
				merged_vertices_it->binormal	=	transform_packed_normal(
					current_instance.m_transform,
					merged_vertices_it->binormal
				);
				merged_vertices_it->tangent	=	transform_packed_normal(
					current_instance.m_transform,
					merged_vertices_it->tangent
				);

				++stream_1_data_it; ++merged_vertices_it;
			}

			current_num_vertices			+=	surface->m_num_vertices;
			current_num_indices			+=	surface->m_num_indices;
		}

		m_vb_stream_1[lod_index]		=	resource_manager::ref( ).create_buffer(
			m_num_merged_vertices[lod_index] * sizeof( stream_1_type ),
			stream_1_data,
			enum_buffer_type_vertex,
			false,
			false
		);

		res_declaration* declaration		=	resource_manager::ref( ).create_declaration(
			layout,
			array_size( layout )
		);
		m_geometry[lod_index]				=	resource_manager::ref( ).create_geometry(
			declaration,
			sizeof( grass_source_vertex ),
			*resource_manager::ref( ).create_buffer(
				m_num_merged_vertices[lod_index] * sizeof( grass_source_vertex ),
				merged_vertices,
				enum_buffer_type_vertex,
				false,
				false
			),
			*resource_manager::ref( ).create_buffer(
				m_num_merged_indices[lod_index] * sizeof( u16 ),
				m_merged_indices[lod_index],
				enum_buffer_type_index,
				false,
				false
			)
		);

		FREE								( stream_1_data );
		FREE								( merged_vertices );
		++m_num_avaliable_lods;
	}
}

bool grass_patch::is_occluded( ) const
{
	return options::ref( ).current.m_use_hiz_occlusion_culling && m_occluded;
}

} // namespace render
} // namespace vostok
