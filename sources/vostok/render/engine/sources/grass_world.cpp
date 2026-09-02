// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include <vostok/collision/api.h>
#include <vostok/collision/space_partitioning_tree.h>
#include <vostok/console_command.h>
#include <vostok/math_randoms_generator.h>
#include <vostok/memory_chunk_reader.h>
#include <vostok/render/core/backend.h>
#include <vostok/render/core/options.h>
#include <vostok/render/engine/vertex_colored.h>
#include <vostok/resources_pinned_ptr.h>
#include "grass_data.h"
#include "grass_patch.h"
#include "grass_world.h"
#include "renderer_context.h"
#include "scene_view.h"
#include "statistics.h"
#include "system_renderer.h"
namespace vostok {
namespace render {

static bool s_draw_grass_debug_value				=	false;
static vostok::console_commands::cc_bool s_draw_grass_debug("r_draw_grass_debug", s_draw_grass_debug_value, false, vostok::console_commands::command_type_engine_internal);

static bool s_use_grass_patches_sorting_value	=	true;
static vostok::console_commands::cc_bool s_use_grass_patches_sorting("r_use_grass_patches_sorting", s_use_grass_patches_sorting_value, false, vostok::console_commands::command_type_engine_internal);

static bool s_use_grass_instances_sorting_value	=	false;
static vostok::console_commands::cc_bool s_use_grass_instances_sorting("r_use_grass_instances_sorting", s_use_grass_instances_sorting_value, false, vostok::console_commands::command_type_engine_internal);

static math::random32 point_random_x;
static math::random32 point_random_z;
static math::random32 model_index_random;
static math::random32 model_orientation_random;
static math::random32 model_density_random;
static math::random32 model_scale_random;
static u32 g_template_counter;
static u32 g_instance_counter;

void setup_seed_clk( );

typedef vector<grass_patch*>	grass_patches_type;
typedef vector<grass_instance*>	grass_instances_type;

grass_world::grass_world( )
{
	m_patches_tree									=	&*collision::new_space_partitioning_tree(g_allocator, 1.f, 1024);
	m_ambient_color									=	backend::ref( ).register_constant_host( "ambient_color", rc_float );
	setup_seed_clk( );
	m_c_environment_skylight_upper_color			=	backend::ref( ).register_constant_host( "environment_skylight_upper_color", rc_float );
	m_c_environment_skylight_lower_color			=	backend::ref( ).register_constant_host( "environment_skylight_lower_color", rc_float );
	m_c_environment_skylight_parameters			=	backend::ref( ).register_constant_host( "environment_skylight_parameters", rc_float );
	m_c_sun_direction								=	backend::ref( ).register_constant_host( "sun_direction", rc_float );
	m_c_sun_color									=	backend::ref( ).register_constant_host( "sun_color", rc_float );
	m_patch_parameters								=	backend::ref( ).register_constant_host( "patch_parameters", rc_float );
	m_trample_parameters							=	backend::ref( ).register_constant_host( "trample_parameters", rc_float );
	m_shadow_cascade_index_parameter				=	backend::ref( ).register_constant_host( "shadow_cascade_index", rc_int );
	m_wind_info_parameters							=	backend::ref( ).register_constant_host( "wind_info_parameters", rc_float );
}

void grass_world::set_wind_parameters( float2 const& dir, float const strength )
{
	backend::ref( ).set_vs_constant( m_wind_info_parameters, float3( dir.x, dir.y, strength ) );
}

void grass_world::set_patch_parameters( grass_patch* patch )
{
	backend::ref( ).set_vs_constant( m_patch_parameters, float4( patch->m_origin, 16.f ) );
}

void grass_world::set_trample_parameters( trample_desc& desc )
{
	backend::ref( ).set_ps_constant( m_trample_parameters, desc.multiplier );
}

void grass_world::set_shadow_parameters( u32 const )
{
	backend::ref( ).set_ps_constant( m_shadow_cascade_index_parameter, 0 );
}

grass_world::~grass_world( )
{
	clear( );
	collision::delete_space_partitioning_tree		(m_patches_tree);
}

u32 grass_world::add_template( grass_render_model_ptr const& render_model )
{
	u32 const id = ++g_template_counter;
	m_templates.push_back( NEW( grass_template )( id, render_model ) );
	return id;
}

void grass_world::add_trample( trample_desc const& desc )
{
	m_trample_array.push_back	( desc );
}

u32 grass_world::add_instance(
	u32 const template_id,
	math::color const& color,
	float4x4 const& transform,
	u8 const layer,
	float const wind_scale
)
{
	grass_template* templ = id_to_template( template_id );
	u32 const id = ++g_instance_counter;
	grass_instance* instance = NEW( grass_instance )(
		id,
		templ,
		color,
		transform,
		layer,
		wind_scale
	);
	templ->m_instances.push_back( instance );
	return id;
}

void grass_world::remove_instance( u32 const in_id )
{
	grass_templates_type::const_iterator	it		=	m_templates.begin();
	grass_templates_type::const_iterator	end		=	m_templates.end();

	for (; it != end; ++it)
	{
		grass_template* templ						=	*it;

		grass_instances_type::iterator it_instance	=	templ->m_instances.begin();
		grass_instances_type::iterator end_instance	=	templ->m_instances.end();

		for (; it_instance != end_instance; ++it_instance)
		{
			grass_instance* instance				=	*it_instance;

			if (instance->m_index == in_id)
			{
				DELETE								(instance);
				templ->m_instances.erase			(it_instance);
				return;
			}
		}
	}
}

grass_template* grass_world::id_to_template( u32 const id ) const
{
	grass_template* result							=	NULL;
	grass_templates_type::const_iterator	it		=	m_templates.begin();
	grass_templates_type::const_iterator	end		=	m_templates.end();

	for (; it != end; ++it)
	{
		result										=	(*it);

		if (result->m_index == id)
			break;
	}

	return result;
}

grass_template* grass_world::find_template( grass_render_model_ptr const& model ) const
{
	grass_template* result = NULL;
	grass_templates_type::const_iterator it = m_templates.begin( );
	grass_templates_type::const_iterator end = m_templates.end( );
	for ( ; it != end; ++it )
	{
		result = *it;
		if ( result->m_render_model == model )
			break;
	}

	return result;
}

grass_patch* grass_world::find_patch( float3 const& point )
{
	grass_patches_type::const_iterator it			=	m_patches.begin();
	grass_patches_type::const_iterator end			=	m_patches.end();

	for (; it != end; ++it)
	{
		grass_patch* patch							=	*it;
		if (
			patch->get_aabb( ).min.x <= point.x &&
			patch->get_aabb( ).min.z <= point.z &&
			patch->get_aabb( ).max.x >= point.x &&
			patch->get_aabb( ).max.z >= point.z
		)
		{
			return patch;
		}
	}
	return NULL;
}

void grass_world::remove_patches( )
{
	grass_patch** it_p					=	m_patches.begin( );
	grass_patch* const* end_p			=	m_patches.end( );

	for ( ; it_p != end_p; ++it_p )
	{
		grass_patch* patch				=	*it_p;
		DELETE								( patch );
	}

	m_patches.clear( );
	m_visible_patches.clear( );
}

void grass_world::populate( float const patch_size_ground )
{
	remove_patches( );

	grass_template** it					=	m_templates.begin( );
	grass_template** end				=	m_templates.end( );

	for ( ; it != end; ++it )
	{
		grass_template* templ			=	*it;

		grass_instance** it_instance	=	templ->m_instances.begin( );
		grass_instance** end_instance	=	templ->m_instances.end( );

		for ( ; it_instance != end_instance; ++it_instance )
		{
			grass_instance* instance	=	*it_instance;
			float3 origin				=	instance->m_transform.c.xyz( );
			float3 origin_aligned		=	float3(
				math::floor( origin.x / patch_size_ground ) * patch_size_ground + patch_size_ground * .5f,
				0.f,
				math::floor( origin.z / patch_size_ground ) * patch_size_ground + patch_size_ground * .5f
			);

			grass_patch* new_patch		=	find_patch( origin_aligned );
			if ( !new_patch )
			{
				new_patch					=	NEW( grass_patch )(
					m_patches_tree,
					templ,
					origin_aligned,
					patch_size_ground
				);
				m_patches.push_back		( new_patch );
			}
			new_patch->m_instances.push_back( instance );
		}
	}

	merge_patches( );
}

void grass_world::merge_patches( )
{
	grass_patch** it_patch = m_patches.begin( );
	grass_patch* const* end_patch = m_patches.end( );
	for ( ; it_patch != end_patch; ++it_patch ) {
		grass_patch* patch = *it_patch;
		patch->init_collision( );
		patch->merge_instances( );
	}
}

// claude@NOTE: the grass debug guard is partial-inlined only in the base.
void grass_world::render_debug( renderer_context* context )
{
	if ( !s_draw_grass_debug_value )
		return;

	VOSTOK_UNREFERENCED_PARAMETER		( context );

	grass_patch* const* it				= m_visible_patches.begin( );
	grass_patch* const* end				= m_visible_patches.end( );
	float const fnum_patches			= float( m_visible_patches.size( ) );
	float fpatch_index					= 0.f;

	math::random32 r;

	for ( ; it != end; ++it )
	{
		grass_patch* patch				= *it;

		system_renderer::ref( ).draw_aabb	(
			patch->get_aabb( ),
			math::color( 1.f - fpatch_index / fnum_patches, 1.f - fpatch_index / fnum_patches, 1.f - fpatch_index / fnum_patches, 1.f )
		);

		grass_instance** it_instance	= patch->m_instances.begin( );
		grass_instance** end_instance	= patch->m_instances.end( );
		float const fnum_instances		= float( patch->m_instances.size( ) );
		float finstance_index			= 0.f;

		for ( ; it_instance != end_instance; ++it_instance )
		{
			grass_instance* instance	= *it_instance;
			float3 origin				= instance->m_transform.c.xyz( );

			math::color const c			= math::color(
				1.f - finstance_index / fnum_instances,
				1.f - finstance_index / fnum_instances,
				1.f - finstance_index / fnum_instances,
				1.f );

			vertex_colored vertices[]	= {
				vertex_colored( origin, c ),
				vertex_colored( origin + float3( r.random_f( 2.f ) - 1.f, 1.2f, r.random_f( 2.f ) - 1.f ), c )
			};
			u16 indices[]				= { 0, 1 };
			system_renderer::ref( ).draw_lines( vertices, vertices + array_size( vertices ), indices, indices + array_size( indices ), false );

			finstance_index				+= 1.f;
		}

		fpatch_index					+= 1.f;
	}
}

bool sort_grass_patch_predicate::operator()( grass_patch const* left, grass_patch const* right ) const
{
	return math::squared_length( left->m_origin - m_view_pos ) <
		math::squared_length( right->m_origin - m_view_pos );
}

void grass_world::process_sorting( float3 const& viewer_position, bool sort_instances )
{
	if ( s_use_grass_patches_sorting_value )
		std::sort(
			m_visible_patches.begin( ),
			m_visible_patches.end( ),
			sort_grass_patch_predicate( viewer_position )
		);

	if ( !s_use_grass_instances_sorting_value || !sort_instances )
		return;

	grass_patch** it = m_visible_patches.begin( );
	grass_patch** end = m_visible_patches.end( );
	u32 num_sorted_patches = 0;
	for ( ; it != end; ++it ) {
		if ( num_sorted_patches > 5 )
			break;

		(*it)->sort_instances( viewer_position );
		++num_sorted_patches;
	}
}

void grass_world::process_culling( renderer_context* context, float const first_lod_distance )
{
	m_visible_patches.clear							();

	statistics::ref().grass_stat_group.num_total_patches.value	=	m_patches.size();

	collision::objects_type objects					(	g_allocator );
	objects.reserve( m_patches.size() );

	math::frustum view_frustum						(context->get_vp());

	m_patches_tree->cuboid_query					(u32(-1), view_frustum, objects );

	float const first_lod_distance_squared			=	first_lod_distance * first_lod_distance;

	for (collision::objects_type::const_iterator it = objects.begin(), end = objects.end(); it != end; ++it)
	{
		grass_patch* patch							=	static_cast<grass_patch*>((*it)->user_data());

		float const to_aabb_center_squared			=	math::squared_length(context->get_view_pos() - patch->get_aabb().center());

		if (first_lod_distance_squared < to_aabb_center_squared)
			continue;

		patch->m_current_lod_index						=	0;
		float const distance							=	math::sqrt( to_aabb_center_squared ); if ( distance > options::ref( ).current.m_grass_lod1_distance )
			patch->m_current_lod_index					=	1;
		else if ( distance > options::ref( ).current.m_grass_lod2_distance )
			patch->m_current_lod_index					=	2;

		patch->m_current_lod_index						=	patch->get_valid_lod_index(
			patch->m_current_lod_index
		);
		m_visible_patches.push_back					(patch);
	}

	process_sorting(
		context->get_view_pos( ),
		!( context->scene_view( )->get_render_frame_index( ) & 31 )
	);
}

void grass_world::accumulate_trample( renderer* in_renderer, renderer_context* in_context )
{
	grass_patch* const* it_patch			= m_visible_patches.begin( );
	grass_patch* const* end_patch		= m_visible_patches.end( );

	for ( ; it_patch != end_patch; ++it_patch )
	{
		if ( options::ref( ).current.m_use_hiz_occlusion_culling && (*it_patch)->is_occluded( ) )
			continue;

		trample_desc* it_trample			= m_trample_array.begin( );
		trample_desc* end_trample		= m_trample_array.end( );

		for ( ; it_trample != end_trample; ++it_trample )
			(*it_patch)->try_accumulate_trample( *it_trample, this, in_renderer, in_context );
	}

	m_trample_array.clear( );
}

void grass_world::remove_trample( )
{
	grass_patch* const* it_patch			= m_visible_patches.begin( );
	grass_patch* const* end_patch		= m_visible_patches.end( );

	for ( ; it_patch != end_patch; ++it_patch )
		(*it_patch)->remove_trample( );
}

void grass_world::render(
	renderer_context*		context,
	float3 const&			viewer_position,
	enum_render_stage_type	stage_type,
	u32 const				tech_index,
	float const				draw_distance,
	bool,
	res_effect*				debug_effect,
	bool					shadow_pass,
	u32 const				cascade_index
)
{
	if ( !options::ref( ).current.m_draw_grass )
		return;

	if (
		!m_templates.empty( ) &&
		!m_templates.front( )->m_instances.empty( ) &&
		m_patches.empty( )
	)
		m_need_populate = true;

	if ( m_need_populate ) {
		populate( 16.f );
		m_need_populate = false;
	}

	grass_patch** it_patch = m_visible_patches.begin( );
	grass_patch* const* end_patch = m_visible_patches.end( );
	for ( ; it_patch != end_patch; ++it_patch ) {
		grass_patch* patch = *it_patch;
		if ( options::ref( ).current.m_use_hiz_occlusion_culling && patch->is_occluded( ) )
			continue;

		if ( shadow_pass ) {
			math::frustum shadow_frustum( context->get_culling_vp( ) );
			if ( shadow_frustum.test_inexact( patch->get_aabb( ) ) == math::intersection_outside )
				continue;
		}

		patch->render(
			this,
			context,
			viewer_position,
			stage_type,
			tech_index,
			draw_distance,
			debug_effect,
			cascade_index
		);
	}
}

void grass_world::add_grass_layer(
	grass_layer_desc* desc,
	grass_layer_data* layer_data,
	bool populate,
	bool from_cook
)
{
	update_grass_layer( desc, layer_data, true, populate, from_cook );
}

void grass_world::update_grass_layer(
	grass_layer_desc* desc,
	grass_layer_data* layer_data,
	bool is_set,
	bool do_populate,
	bool from_cook
)
{
	if ( is_set )
	{
		u8 const models_count = (u8)desc->models_list.size( );
		buffer_vector<resources::request> r( ALLOCA( sizeof( resources::request ) * models_count ), models_count );
		r.resize( models_count );
		for ( u8 i = 0; i < models_count; ++i )
		{
			r[i].path = desc->models_list[i].name.c_str( );
			r[i].id = resources::grass_render_model_class;
		}

		resources::query_resources(
			r.begin( ),
			r.size( ),
			from_cook ?
				boost::bind( &grass_world::grass_layer_resources_ready_from_cook, this, _1, desc, layer_data, do_populate ) :
				boost::bind( &grass_world::grass_layer_resources_ready, this, _1, desc, layer_data, do_populate ),
			g_allocator
		);
	}
	else
	{
		for ( u16 x = 0; x < layer_data->size_x_cells; ++x )
		{
			for ( u16 z = 0; z < layer_data->size_z_cells; ++z )
			{
				float2 const cell_pos_lt(
					layer_data->lt_x_m + x * layer_data->grass_cell_size_m,
					layer_data->lt_z_m + z * layer_data->grass_cell_size_m
				);
				float2 const cell_pos_rb = cell_pos_lt + float2(
					layer_data->grass_cell_size_m,
					layer_data->grass_cell_size_m
				);

				if ( layer_data->stencil_data[z * layer_data->size_x_cells + x] )
					remove_layer_instances( desc->id, cell_pos_lt, cell_pos_rb );
			}
		}

		if ( do_populate )
			m_need_populate = true;
	}
}

void grass_world::clear( )
{
	remove_patches( );

	grass_templates_type::iterator it_t = m_templates.begin( );
	grass_templates_type::iterator end_t = m_templates.end( );
	for ( ; it_t != end_t; ++it_t )
	{
		grass_template* templ = *it_t;
		grass_instances_type::iterator it_i = templ->m_instances.begin( );
		grass_instances_type::iterator end_i = templ->m_instances.end( );
		for ( ; it_i != end_i; ++it_i )
		{
			grass_instance* instance = *it_i;
			DELETE( instance );
		}

		templ->m_instances.clear( );
		DELETE( templ );
	}

	m_templates.clear( );
}

void grass_world::remove_grass_layer( u8 id, bool )
{
	grass_templates_type::iterator it = m_templates.begin( );
	grass_templates_type::iterator end = m_templates.end( );

	vector<u32> instances_to_remove;
	for ( ; it != end; ++it )
	{
		grass_template* templ = *it;
		grass_instances_type::iterator it_instance = templ->m_instances.begin( );
		grass_instances_type::iterator end_instance = templ->m_instances.end( );
		for ( ; it_instance != end_instance; ++it_instance )
		{
			grass_instance* instance = *it_instance;
			if ( instance->m_layer_id == id )
				instances_to_remove.push_back( instance->m_index );
		}
	}

	remove_instances( instances_to_remove );
}

void grass_world::remove_instances( vector<u32> const& v )
{
	vector<u32>::const_iterator it = v.begin( );
	vector<u32>::const_iterator end = v.end( );
	for ( ; it != end; ++it )
		remove_instance( *it );
}

void setup_seed_clk( )
{
	u32 const seed							= GetTickCount( );
	point_random_x.seed						( seed );
	point_random_z.seed						( seed );
	model_index_random.seed					( seed );
	model_orientation_random.seed			( seed );
	model_density_random.seed				( seed );
	model_scale_random.seed					( seed );
}

u8 select_model_template( float* values, float sum, u8 const count )
{
	float const p = model_index_random.random_f( sum );
	for ( u8 i = 0; i < count; ++i )
	{
		if ( values[i] > p )
			return i;
	}

	return count - 1;
}

float select_model_orientation( )
{
	return model_orientation_random.random_f( math::pi_x2 );
}

float select_model_scale( float const delta, float const base_scale )
{
	return base_scale - delta + model_scale_random.random_f( delta * 2.f );
}

void grass_world::grass_layer_resources_ready_from_cook(
	resources::queries_result& data,
	grass_layer_desc* desc,
	grass_layer_data* layer_data,
	bool do_populate
)
{
	resources::pinned_ptr_const<u8> pdata( layer_data->layer_data_raw_file );
	memory::chunk_reader chunk(
		pdata.c_ptr( ),
		pdata.size( ),
		memory::chunk_reader::chunk_type_sequential
	);

	if ( chunk.chunk_exists( 0x58 ) )
	{
		memory::reader instances_reader = chunk.open_reader( 0x58 );
		layer_data->instances_positions = (float3*)instances_reader.pointer( );
		layer_data->instances_count = instances_reader.length( ) / sizeof( float3 );

		if ( desc->use_face_normal )
		{
			memory::reader normals_reader = chunk.open_reader( 0x59 );
			layer_data->instances_normals = (float3*)normals_reader.pointer( );
		}

		grass_layer_resources_ready( data, desc, layer_data, do_populate );
	}
	else
	{
		layer_data->instances_positions = 0;
		layer_data->instances_count = 0;
		grass_layer_resources_ready( data, desc, layer_data, do_populate );
	}

	DELETE( desc );
	DELETE( layer_data );
}

void grass_world::grass_layer_resources_ready(
	resources::queries_result& data,
	grass_layer_desc* desc,
	grass_layer_data* layer_data,
	bool do_populate
)
{
	u8 models_count = (u8)desc->models_list.size( );
	u32* model_ids = (u32*)ALLOCA( sizeof( u32 ) * models_count );
	for ( u8 i = 0; i < models_count; ++i )
	{
		grass_render_model_ptr model = static_cast_resource_ptr<grass_render_model_ptr>(
			data[i].get_unmanaged_resource( )
		);
		grass_template* templ = find_template( model );
		if ( templ )
			model_ids[i] = templ->m_index;
		else
			model_ids[i] = add_template( model );
	}

	float* probabilities = (float*)ALLOCA( sizeof( float ) * models_count );
	float prob_sum = 0.f;
	for ( u8 i = 0; i < models_count; ++i )
	{
		prob_sum += desc->models_list[i].probability_;
		probabilities[i] = prob_sum;
	}

	math::color clr( 200, 200, 200 );
	for ( u32 i = 0; i < layer_data->instances_count; ++i )
	{
		u8 const model_index = select_model_template( probabilities, prob_sum, models_count );
		float orient = desc->random_orient ? select_model_orientation( ) : 0.f;
		float4x4 m = math::create_rotation_y( orient );
		m.c.xyz( ) = layer_data->instances_positions[i];

		if ( desc->use_face_normal )
		{
			m.j.xyz( ) = layer_data->instances_normals[i];
			m.k.xyz( ) = math::cross_product( m.i.xyz( ), m.j.xyz( ) );
			m.i.xyz( ) = math::cross_product( m.j.xyz( ), m.k.xyz( ) );
		}

		float const scale = select_model_scale(
			desc->random_scale,
			desc->models_list[model_index].scale
		);
		m.set_scale( float3( scale, scale, scale ) );
		add_instance( model_ids[model_index], clr, m, desc->id, desc->wind_factor );
	}

	m_need_populate = true;
	if ( do_populate )
		m_need_populate = true;
}

void grass_world::remove_layer_instances(
	u8 layer_id,
	float2 const& cell_pos_lt,
	float2 const& cell_pos_rb
)
{
	float3 pt( cell_pos_lt.x + 0.5f, 0.f, cell_pos_lt.y + 0.5f );
	grass_patch* patch = find_patch( pt );
	if ( !patch )
		return;

	vector<u32> instances_to_remove;
	grass_instances_type::iterator it = patch->m_instances.begin( );
	grass_instances_type::iterator end = patch->m_instances.end( );
	for ( ; it != end; ++it )
	{
		grass_instance* instance = *it;
		if ( instance->m_layer_id != layer_id )
			continue;

		float3 p = instance->m_transform.c.xyz( );
		if (
			p.x > cell_pos_lt.x &&
			p.z > cell_pos_lt.y &&
			p.x < cell_pos_rb.x &&
			p.z < cell_pos_rb.y
		)
			instances_to_remove.push_back( instance->m_index );
	}

	if ( !instances_to_remove.empty( ) )
	{
		remove_instances( instances_to_remove );
		m_need_populate = true;
	}
}

} // namespace render
} // namespace vostok
