// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/game_material_manager_cook.h>

#include <vostok/game_core/game_material.h>
#include <vostok/game_core/game_material_manager.h>
#include <vostok/game_core/material_pair.h>
#include <vostok/physics/api.h>
#include <vostok/render/facade/material_effects_instance_cook_data.h>

namespace survarium {

game_material_manager_cook::game_material_manager_cook( bool server_usage ) :
	resources::translate_query_cook	( resources::game_material_manager_class, reuse_true, use_current_thread_id ),
	m_server_usage					( server_usage )
{
	resources::register_cook( this );
}

void game_material_manager_cook::translate_query( resources::query_result_for_cook& parent )
{
	resources::request r[2] = {
		{ "resources/game_materials/game.materials", resources::binary_config_class_impl },
		{ "resources/game_materials/material.pairs", resources::binary_config_class_impl}
	};

	resources::query_resources(
		r,
		boost::bind( &game_material_manager_cook::on_configs_loaded, this, _1 ),
		g_allocator,
		NULL,
		&parent,
		assert_on_fail_true
	);
}

void game_material_manager_cook::delete_resource( resources::resource_base* res )
{
	// static_cast_checked is the project-wide cast idiom (see assembly_patterns.md)
	game_material_manager* mngr	= static_cast_checked< game_material_manager* >( res );

	mngr->~game_material_manager( );
	VOSTOK_DELETE_IMPL( g_allocator, mngr );
}

void game_material_manager_cook::on_configs_loaded( resources::queries_result& data )
{
	ASSERT( UNKNOWN_EXPRESSION_T( data.is_successful( ) ) );

	configs::binary_config_ptr mtrl_cfg		= static_cast_resource_ptr< configs::binary_config_ptr >( data[0].get_unmanaged_resource( ) );
	configs::binary_config_ptr pairs_cfg	= static_cast_resource_ptr< configs::binary_config_ptr >( data[1].get_unmanaged_resource( ) );

	game_material_manager* manager = VOSTOK_NEW_IMPL( g_allocator, game_material_manager );
	if ( !manager )
	{
		data.get_parent_query( )->set_out_of_memory( resources::unmanaged_memory, sizeof( game_material_manager ) );
		data.get_parent_query( )->finish_query( result_out_of_memory );
		return;
	}

	data.get_parent_query( )->set_unmanaged_resource( manager, resources::nocache_memory, sizeof( game_material_manager ) );

	create_game_materials( manager, mtrl_cfg->get_root( )["materials"] );
	create_game_material_pairs( *data.get_parent_query( ), manager, pairs_cfg->get_root( )["pairs"] );
}

void game_material_manager_cook::create_game_materials(
	game_material_manager* const			manager,
	configs::binary_config_value const&		materials_root
)
{
	memory::zero( g_material_physics_group );
	configs::binary_config_value const* it	= materials_root.begin( );
	configs::binary_config_value const* end	= materials_root.end( );

	for ( ; it != end ; it++ )
	{
		configs::binary_config_value const& cfg = *it;
		if ( cfg.value_exists( "deleted" ) && (bool)cfg["deleted"] )
			continue;

		game_material* mtrl = VOSTOK_NEW_IMPL( g_allocator, game_material );
		ASSERT( UNKNOWN_EXPRESSION_T( mtrl ) );
		mtrl->load_from_config( cfg );
		manager->add_game_material( mtrl );
		if ( strings::equal( mtrl->name( ), "default" ) )
			manager->m_default_material_id = mtrl->id( );
	}
	ASSERT( UNKNOWN_EXPRESSION );
	physics::setup_game_material_groups( g_material_physics_group, sizeof( g_material_physics_group ) /  sizeof( u16 ) );
}

// claude@NOTE: structure matches (60/60 stmts, decl order verified via PDB-local set), but base
// inlines the three vector ctors, create_request, material_effects_instance_cook_data new, and the
// query_resources bind that the target keeps out-of-line. That inline-vs-call shift moves nearly
// every register/offset, so the byte % stays low even though the shape is right - non-steerable.
void game_material_manager_cook::create_game_material_pairs(
	resources::query_result_for_cook&		parent_query,
	game_material_manager* const			manager,
	configs::binary_config_value const&		pairs_root
)
{
	configs::binary_config_value const* it			= pairs_root.begin( );
	configs::binary_config_value const* const end	= pairs_root.end( );

	vector< resources::request > requests;
	vector< variant<32> const* > user_data_ptrs;
	vector< variant<32> >		 user_datas;

	user_datas.resize( 2 * pairs_root.size( ) );
	u32 ud_idx = 0;
	ext_data_vec* ext_pair_data = VOSTOK_NEW_IMPL( g_allocator, ext_data_vec );	// sushi@TODO: Is this vector leaked?

	for ( ; it != end ; it++ )
	{
		material_pair* pair = VOSTOK_NEW_IMPL( g_allocator, material_pair );
		ASSERT( UNKNOWN_EXPRESSION_T( pair ) );
		pair->load_from_config( *manager, *it );

		if ( !m_server_usage )
		{
			pcstr decal1_name = (pcstr)(*it)["decal1"];
			if ( strings::length( decal1_name ) )
			{
				query_ext_data ext_data;
				resources::request r = resources::create_request( decal1_name, resources::material_effects_instance_class );
				requests.push_back( r );

				ext_data.pair = pair;
				ext_data.type = decal1;
				ext_data.cd	  = VOSTOK_NEW_IMPL( g_allocator, render::material_effects_instance_cook_data )( render::decal_vertex_input_type, resources::unmanaged_resource_ptr( NULL ), false );

				user_datas[ud_idx].set<render::material_effects_instance_cook_data*>( ext_data.cd );

				ext_pair_data->push_back( ext_data );
				user_data_ptrs.push_back( &user_datas[ud_idx++] );
			}

			pcstr decal2_name = (pcstr)(*it)["decal2"];
			if ( strings::length( decal2_name ) )
			{
				query_ext_data ext_data;
				resources::request r = resources::create_request( decal2_name, resources::material_effects_instance_class );
				requests.push_back( r );

				ext_data.pair = pair;
				ext_data.type = decal2;
				ext_data.cd	  = VOSTOK_NEW_IMPL( g_allocator, render::material_effects_instance_cook_data )( render::decal_vertex_input_type, resources::unmanaged_resource_ptr( NULL ), false );

				user_datas[ud_idx].set<render::material_effects_instance_cook_data*>( ext_data.cd );

				ext_pair_data->push_back( ext_data );
				user_data_ptrs.push_back( &user_datas[ud_idx++] );
			}

			pcstr sound_name = (pcstr)(*it)["sound"];
			if ( strings::length( sound_name ) )
			{
				query_ext_data ext_data;
				u32 sound_type = (u32)(*it)["sound_type"];
				resources::class_id_enum resource_type = (resources::class_id_enum)( sound_type );
				ASSERT( UNKNOWN_EXPRESSION );

				resources::request r = resources::create_request( sound_name, resource_type );
				requests.push_back( r );

				ext_data.pair = pair;
				ext_data.type = sound;
				ext_pair_data->push_back( ext_data );
				user_data_ptrs.push_back( NULL );
			}

			pcstr particle_name = (pcstr)(*it)["particle"];
			if ( strings::length( particle_name ) )
			{
				for ( u32 i = 0 ; i < 8 ; ++i ) // sushi@TODO: Why 8?
				{
					query_ext_data ext_data;
					resources::request r = resources::create_request( particle_name, resources::particle_system_instance_class );
					requests.push_back( r );

					ext_data.pair = pair;
					ext_data.type = particle;
					ext_pair_data->push_back( ext_data );

					user_data_ptrs.push_back( NULL );
				}
			}
		}

		manager->add_pair( pair );
	}

	if ( requests.empty( ) )
	{
		parent_query.finish_query( result_success );
	}
	else
	{
		resources::query_resources(
			requests.begin( ),
			requests.size( ),
			boost::bind( &game_material_manager_cook::on_decals_loaded, this, _1, ext_pair_data ),
			g_allocator,
			user_data_ptrs.begin( ),	// sushi@TODO: How ownership works here? `user_data` is dropped when the function exits
			&parent_query,
			assert_on_fail_true
		);
	}
}

// claude@NOTE: target has one extra statement - a bare `jmp` (5 bytes) closing the decal1 block in
// the if/else-if chain; base folds that jump into the decal2 else-if. Same source shape; a codegen
// quirk of the first else-if arm, not steerable. Rest is LOG_ERROR / VOSTOK_DELETE inline residual.
void game_material_manager_cook::on_decals_loaded( resources::queries_result& data, ext_data_vec* ext_data )
{
	if ( !data.is_successful( ) ) // sushi@TODO: I think this is the first place where multiple queries are processed?
	{
		for ( u32 i = 0 ; i <  data.size( ) ; ++i )
		{
			if ( !data[i].is_successful( ) )
			{
				LOG_ERROR( "resource cook failed: %s", data[i].reusable_request_name( ) );
			}
		}
	}	// sushi@NOTE: No return!

	query_ext_data* it	= ext_data->begin( );
	query_ext_data* end	= ext_data->end( );

	for ( u32 idx = 0 ; it != end ; ++it, ++idx )
	{
		if ( it->type == decal1 )
		{
			it->pair->set_decal1( data[idx].get_unmanaged_resource( ) );
			VOSTOK_DELETE_IMPL( g_allocator, it->cd );
		}
		else if ( it->type == decal2 )
		{
			it->pair->set_decal2( data[idx].get_unmanaged_resource( ) );
			VOSTOK_DELETE_IMPL( g_allocator, it->cd );
		}
		else if ( it->type == sound )
		{
			it->pair->set_sound( data[idx].get_unmanaged_resource( ) );
		}
		else // if ( it->type == particle )
			it->pair->add_particle( data[idx].get_unmanaged_resource( ) );
	}

	data.get_parent_query( )->finish_query( result_success );
}

} // namespace survarium
