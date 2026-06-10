////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/game_material_manager_cook.h>

#include <vostok/game_core/game_material.h>
#include <vostok/game_core/game_material_manager.h>
#include <vostok/game_core/material_pair.h>
#include <vostok/physics/api.h>
#include <vostok/render/facade/material_effects_instance_cook_data.h>

namespace survarium {

// STATE[100%|DONE]
game_material_manager_cook::game_material_manager_cook( bool server_usage ) :
	resources::translate_query_cook	( resources::game_material_manager_class, reuse_true, use_current_thread_id ),
	m_server_usage					( server_usage )
{
	resources::register_cook( this );
}

// STATE[100%|DONE]
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

// STATE[94.26%|PARTIAL]
void game_material_manager_cook::delete_resource( resources::resource_base* res )
{
	// static_cast_checked is the project-wide cast idiom (see assembly_patterns.md)
	game_material_manager* mngr	= static_cast_checked< game_material_manager* >( res );

	mngr->~game_material_manager( );
	VOSTOK_DELETE_IMPL( g_allocator, mngr );

	// STRUCTURE DIFF: target 3 stmts / base 3 stmts
	// SIZE +0x4 | 53 | VOSTOK_DELETE_IMPL( g_allocator, mngr );
	// VERDICT: STRUCTURE MATCH (3/3) - memory::strip_pointer kept as an out-of-line empty call in base but inlined to a direct mov in target (per-site LTCG); non-steerable.
}

// STATE[98.53%|PARTIAL]
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

	// STRUCTURE DIFF: target 11 stmts / base 11 stmts
	// SIZE +0x4 | 75 | game_material_manager* manager = VOSTOK_NEW_IMPL( g_allocator, game_material_manager );
	// VERDICT: STRUCTURE MATCH (11/11) - sole residual is memory::strip_pointer kept out-of-line in base vs inlined in target (per-site LTCG) at the NEW site. The former cfg-ptr rows closed with the static_cast_resource_ptr const& signature fix (90.20 -> 98.53).
}

// STATE[97.79%|PARTIAL]
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

	// STRUCTURE DIFF: target 16 stmts / base 16 stmts
	// SIZE +0x4 | 113 | game_material* mtrl = VOSTOK_NEW_IMPL( g_allocator, game_material );
	// VERDICT: STRUCTURE MATCH (16/16) - memory::strip_pointer out-of-line empty call in base vs inlined in target (per-site LTCG) at the NEW site; non-steerable.
}

// STATE[57.81%|PARTIAL]: create_request LTCG convention promotion; cascades stack-offset shifts.
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
	vector< query_ext_data >* ext_pair_data = VOSTOK_NEW_IMPL( g_allocator, vector< query_ext_data > );	// sushi@TODO: Is this vector leaked?

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
				ext_data.type = query_ext_data::decal1;
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
				ext_data.type = query_ext_data::decal2;
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
				ext_data.type = query_ext_data::sound;
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
					ext_data.type = query_ext_data::particle;
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

	// STRUCTURE DIFF: target 60 stmts / base 60 stmts (all ONLY rows pair off as equal-size aligner shifts)
	// SIZE +0x27 x4 | 163/180/201/216 | resources::request r = resources::create_request( ... );
	// SIZE +0x6..+0x13 | 164/181/202/217/173/190/206/207 | requests/user_data_ptrs/ext_pair_data .push_back( ... );
	// SIZE +0x4..+0x5 | 149/153 | VOSTOK_NEW_IMPL rows
	// VERDICT: STRUCTURE MATCH (60/60) - target calls the LTCG-PROMOTED create_request (dest in eax, ICF-folded onto const_buffer::ctor; the original cdecl symbol is a 1-byte ret stub) while base keeps the cdecl eax:edx return + copy chain despite the __declspec(noinline) steer; vector push_back rows are the same whole-program convention promotion; NEW rows are the strip_pointer per-site LTCG. All non-steerable from this TU; the convention deltas cascade [esp+N] shifts through the body (57.81%).
}

// STATE[92.35%|PARTIAL]: LTCG for logging; rel32 brace-jmp line-attribution.
void game_material_manager_cook::on_decals_loaded( resources::queries_result& data, vector< query_ext_data >* ext_data )
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
		if ( it->type == query_ext_data::decal1 )
		{
			it->pair->set_decal1( data[idx].get_unmanaged_resource( ) );
			VOSTOK_DELETE_IMPL( g_allocator, it->cd );
		}
		else if ( it->type == query_ext_data::decal2 )
		{
			it->pair->set_decal2( data[idx].get_unmanaged_resource( ) );
			VOSTOK_DELETE_IMPL( g_allocator, it->cd );
		}
		else if ( it->type == query_ext_data::sound )
			it->pair->set_sound( data[idx].get_unmanaged_resource( ) );
		else // if ( it->type == query_ext_data::particle )
			it->pair->add_particle( data[idx].get_unmanaged_resource( ) );
	}

	data.get_parent_query( )->finish_query( result_success );

	// STRUCTURE DIFF: target 21 stmts / base 20 stmts
	// SIZE +0x6 | 267 | LOG_ERROR( "resource cook failed: %s", data[i].reusable_request_name( ) );
	// SIZE +0x5 | 280 | VOSTOK_DELETE_IMPL( g_allocator, it->cd );
	// TRGT_ONLY 0x5 | L247 (the decal1 branch closing brace)
	// VERDICT: STRUCTURE MISMATCH (quantity, attribution-only) - bytes identical: the decal1 branch-exit jmp is rel32 and gets its OWN line record in target (the `}` line) but folds into the DELETE row in base; the rel8 decal2/sound exits fold identically on both sides. Zero-byte, not source-steerable (same braced source both sides). LOG_ERROR SIZE is logging-macro LTCG.
}

} // namespace survarium
