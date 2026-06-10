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
// STRUCTURE DIFF[target 0x753690 | base 0x567490]: target 4 / base 4 stmts
// .. same ..
// 0x01d <0x22> | 0x01d <0x26> | VOSTOK_DELETE_IMPL( g_allocator, mngr ); // sushi@MATCH: A call to an empty function here in base.   SIZE
// ; aligned 3, size-diffs 1, quantity-diffs 0
// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is the VOSTOK_DELETE_IMPL operator-delete call (empty stub vs target's), non-steerable. trail: delete_resource.md
void game_material_manager_cook::delete_resource( resources::resource_base* res )
{
	game_material_manager* mngr	= static_cast_checked< game_material_manager* >( res ); // sushi@TODO: Use in all other static_cast places

	mngr->~game_material_manager( );
	VOSTOK_DELETE_IMPL( g_allocator, mngr ); // sushi@MATCH: A call to an empty function here in base.
}

// STATE[90.20%|PARTIAL]
// STRUCTURE DIFF[target 0x754330 | base 0x568240]: target 16 / base 16 stmts
// .. same ..
// 0x016 <0x22> | --          | L51   ONLY target
// .. same ..
// 0x038 <0x22> | 0x016 <0x1d> | configs::binary_config_ptr mtrl_cfg = static_cast_resource_ptr< configs::binary_config_ptr >( data[0].get_unmanaged_resource( ) );   SIZE
// --          | 0x033 <0x1d> | configs::binary_config_ptr pairs_cfg = static_cast_resource_ptr< configs::binary_config_ptr >( data[1].get_unmanaged_resource( ) );   ONLY base
// .. same ..
// 0x05a <0x4d> | 0x050 <0x51> | game_material_manager* manager = VOSTOK_NEW_IMPL( g_allocator, game_material_manager );   SIZE
// .. same ..
// ; aligned 13, size-diffs 2, quantity-diffs 2
// VERDICT: STRUCTURE MATCH (shape ok) - cfg-ptr SIZE/quantity is static_cast_resource_ptr copy via stack temporary (push ecx; mov esi,esp) + L51/L53 line attribution; manager SIZE is VOSTOK_NEW empty-stub vs target operator new. All non-steerable. trail: on_configs_loaded.md
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

// STATE[97.79%|PARTIAL]
// STRUCTURE DIFF[target 0x7536e0 | base 0x5674e0]: target 20 / base 19 stmts
// .. same ..
// 0x07a <0x47> | 0x07a <0x4b> | game_material* mtrl = VOSTOK_NEW_IMPL( g_allocator, game_material );   SIZE
// .. same ..
// <0>         | --          |    EMPTY only target
// .. same ..
// ; aligned 18, size-diffs 1, quantity-diffs 1
// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is VOSTOK_NEW empty-stub vs target operator new; quantity is a collapsed source-line gap. Non-steerable. trail: create_game_materials.md
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

		game_material* mtrl = VOSTOK_NEW_IMPL( g_allocator, game_material ); // sushi@MATCH: A call to an empty function here in base
		ASSERT( UNKNOWN_EXPRESSION_T( mtrl ) );
		mtrl->load_from_config( cfg );
		manager->add_game_material( mtrl );
		if ( strings::equal( mtrl->name( ), "default" ) )
			manager->m_default_material_id = mtrl->id( );
	}
	ASSERT( UNKNOWN_EXPRESSION );
	physics::setup_game_material_groups( g_material_physics_group, sizeof( g_material_physics_group ) /  sizeof( u16 ) );
}

// STATE[57.81%|PARTIAL]: create_request returns by copy; cascades stack-offset shifts.
// STRUCTURE DIFF[target 0x753b00 | base 0x567910]: target 90 / base 89 stmts (aligned 63, size-diffs 20, quantity-diffs 13)
// .. same ..
// 0x240 <0x15> | 0x251 <0x3c> | resources::request r = resources::create_request( decal1_name, resources::material_effects_instance_class );   SIZE
// 0x255 <0x13> | 0x28d <0x19> | requests.push_back( r );   SIZE
// 0x3ee <0x15> | 0x432 <0x3c> | resources::request r = resources::create_request( decal2_name, ... );   SIZE
// 0x5db <0x1b> | 0x652 <0x42> | resources::request r = resources::create_request( sound_name, resource_type );   SIZE
// 0x688 <0xf>  | 0x74f <0x2a> | resources::request r = resources::create_request( particle_name, ... );   SIZE
// (remaining size/quantity diffs are the same root cause cascading through stack offsets; + decl/sound-block source-line attribution)
// VERDICT: STRUCTURE MATCH (shape ok) - target returns the 8-byte resources::request in eax:edx and copies into the local r; base uses sret/RVO writing into r directly. This return-ABI difference at every create_request call cascades [esp+N] offset shifts through the rest of the body. Non-steerable (return ABI of create_request). trail: create_game_material_pairs.md
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
}

// STATE[92.35%|PARTIAL]: LTCG for logging and resource_ptr methods.
// STRUCTURE DIFF[target 0x753830 | base 0x567630]: target 32 / base 32 stmts
// .. same ..
// 0x06d <0xad> | 0x06d <0xb3> | LOG_ERROR( "resource cook failed: %s", data[i].reusable_request_name( ) );   SIZE
// .. same ..
// 0x1ba <0x1b> | 0x1c0 <0x20> | VOSTOK_DELETE_IMPL( g_allocator, it->cd );   SIZE
// 0x1d5 <0x5> | --          | L247   ONLY target
// .. same ..
// ; aligned 28, size-diffs 2, quantity-diffs 4
// VERDICT: STRUCTURE MATCH (shape ok) - LOG_ERROR SIZE is logging-macro LTCG; VOSTOK_DELETE_IMPL SIZE is the empty operator-delete stub; quantity diffs are }-brace / collapsed source-line attribution. Non-steerable. trail: on_decals_loaded.md
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
}

} // namespace survarium
