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

// STATE[70.31%|DONE]: LTCG for translate_query_cook
game_material_manager_cook::game_material_manager_cook( bool server_usage ) :
	resources::translate_query_cook	( resources::game_material_manager_class, reuse_true, use_current_thread_id ),
	m_server_usage					( server_usage )
{
	resources::register_cook( this );

	// FUNCTION BODY
	// <0x76366e>|0x03e|+0x00c:'21'
	// ******
}

// STATE[97.30%|DONE]: LTCG for boost::function destructor
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


	// FUNCTION BODY
	// <0>
	// <0x7644b9>|0x009|+0x00e:'27'
	// <0x7644c7>|0x017|+0x00e:'28'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x7644d5>|0x025|+0x0ce:'37'
	// ******
}

// STATE[94.26%|DONE]
void game_material_manager_cook::delete_resource( resources::resource_base* res )
{
	game_material_manager* mngr	= static_cast_checked< game_material_manager* >( res ); // sushi@TODO: Use in all other static_cast places

	mngr->~game_material_manager( );
	VOSTOK_DELETE_IMPL( g_allocator, mngr ); // sushi@MATCH: A call to an empty function here in base.

	// FUNCTION BODY
	// <0x763699>|0x009|+0x006:'42'
	// <0>
	// <0x76369f>|0x00f|+0x00e:'44'
	// <0x7636ad>|0x01d|+0x022:'45'
	// ******
}

// STATE[75.71%|PARTIAL]
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

	// FUNCTION BODY
	// <0x76433a>|0x00a|+0x00c:'50'
	// <0x764346>|0x016|+0x022:'51'
	// <0>
	// <0x764368>|0x038|+0x022:'53'
	// <0>
	// <0x76438a>|0x05a|+0x04d:'55'
	// <0x7643d7>|0x0a7|+0x006:'56'
	// <0>
	// <0x7643dd>|0x0ad|+0x017:'58'
	// <0x7643f4>|0x0c4|+0x013:'59'
	// <0x764407>|0x0d7|+0x015:'60'		return
	// <0>							}
	// <1>
	// <0x76441c>|0x0ec|+0x025:'63'
	// <0>
	// <0x764441>|0x111|+0x026:'65'
	// <0x764467>|0x137|+0x02f:'66'
	// ******
}

// STATE[90.02%|DONE]
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

	// FUNCTION BODY
	// <0>
	// <1>
	// <0x7636e9>|0x009|+0x00d:'75'
	// <0x7636f6>|0x016|+0x00b:'76'
	// <0x763701>|0x021|+0x00b:'77'
	// <0>
	// <0x76370c>|0x02c|+0x017:'79'	for (
	// <0>							{
	// <0x763723>|0x043|+0x006|[1]:'81'	configs::binary_config_value const& cfg = *it;
	// <0x763729>|0x049|+0x02f:'82'		if ( cfg.value_exists( "deleted" ) && (bool)cfg["deleted"] )
	// <0x763758>|0x078|+0x002:'83'			continue;
	// <0>
	// <0x76375a>|0x07a|+0x047:'85'		game_material* mtrl = VOSTOK_NEW_IMPL( g_allocator, game_material );
	// <0x7637a1>|0x0c1|+0x00c:'86'		ASSERT( UNKNOWN_EXPRESSION_T( mtrl ) );
	// <0x7637ad>|0x0cd|+0x00c:'87'		mtrl->load_from_config( cfg );
	// <0x7637b9>|0x0d9|+0x00c:'88'		manager->add_game_material( mtrl );
	// <0x7637c5>|0x0e5|+0x01f:'89'
	// <0x7637e4>|0x104|+0x019:'90'
	// <0x7637fd>|0x11d|+0x005:'91'	}
	// <0x763802>|0x122|+0x00c:'92'	ASSERT( UNKNOWN_EXPRESSION );
	// <0>
	// <0x76380e>|0x12e|+0x00f:'94'	physics::setup_game_material_groups( g_material_physics_group, sizeof( g_material_physics_group ) /  sizeof( u16 ) );
	// ******
}

// STATE[76.02%|PARTIAL]: Structurally is correct, only resources::create_request seems to do a copy in base and lots of LTCG stuff.
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

	// FUNCTION BODY
	// <0>
	// <0x763b14>|0x014|+0x00f:'102'
	// <0x763b23>|0x023|+0x00f:'103'
	// <0>
	// <1>
	// <0x763b32>|0x032|+0x00c:'106'	vector< resources::request >	requests;
	// <0>
	// <1>
	// <0x763b3e>|0x03e|+0x00c:'109'	vector< variant<32> const* > 	user_data_ptrs;
	// <0>
	// <1>
	// <0x763b4a>|0x04a|+0x014:'112'	vector< variant<32> >			user_datas;
	// <0x763b5e>|0x05e|+0x04c:'113'	u32 i = 0;
	// <0x763baa>|0x0aa|+0x00b:'114'	user_datas.push_back( variant<32>() );
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x763bb5>|0x0b5|+0x081:'121'	vector< query_ext_data >* ext_pair_data = VOSTOK_NEW_IMPL( g_allocator, vector< query_ext_data > );
	// <0>
	// <0x763c36>|0x136|+0x027:'123'	for ( ; it != end ; it++ )
	// <0>
	// <0x763c5d>|0x15d|+0x073|[1]:'125'	material_pair* 						pair = VOSTOK_NEW_IMPL( g_allocator, material_pair );
	// <0x763cd0>|0x1d0|+0x014:'126'		ASSERT( UNKNOWN_EXPRESSION_T( pair ) );
	// <0x763ce4>|0x1e4|+0x018:'127'		pair->load_from_config( *manager, *it );
	// <0>
	// <0x763cfc>|0x1fc|+0x013:'129'		if ( !m_server_usage )
	// <0>
	// <0x763d0f>|0x20f|+0x01d|[2]:'131'		pcstr decal1_name = (pcstr)val["decal1"];
	// <0x763d2c>|0x22c|+0x014:'132'			if ( strings::length( decal1_name ) )
	// <0>
	// <1>
	// <0x763d40>|0x240|+0x015|[3]:'135'			resources::request r = resources::create_request( decal1_name, resources::material_effects_instance_class );
	// <0x763d55>|0x255|+0x013:'136'				requests.push_back( r );
	// <0>
	// <0x763d68>|0x268|+0x00e:'138'				ext_data.pair = pair;
	// <0x763d76>|0x276|+0x00b:'139'				ext_data.type = query_ext_data::decal1;
	// <0>
	// <0x763d81>|0x281|+0x07c:'141'				ext_data.cd	  = VOSTOK_NEW_IMPL( g_allocator,
	// <0x763dfd>|0x2fd|+0x040:'142'				user_datas[i].set<render::material_effects_instance_cook_data*>( ext_data.cd );
	// <0>
	// <0x763e3d>|0x33d|+0x014:'144'				ext_pair_data->push_back( ext_data );
	// <0x763e51>|0x351|+0x06c:'145'				user_data_ptrs.push_back( &user_datas[i++] );
	// <0>
	// <1>
	// <0x763ebd>|0x3bd|+0x01d:'148'
	// <0x763eda>|0x3da|+0x014:'149'
	// <0>
	// <1>
	// <0x763eee>|0x3ee|+0x015|[3]:'152'
	// <0x763f03>|0x403|+0x013:'153'
	// <0>
	// <0x763f16>|0x416|+0x00e:'155'
	// <0x763f24>|0x424|+0x00b:'156'
	// <0x763f2f>|0x42f|+0x07c:'157'
	// <0>
	// <0x763fab>|0x4ab|+0x040:'159'
	// <0x763feb>|0x4eb|+0x014:'160'
	// <0x763fff>|0x4ff|+0x06c:'161'
	// <0>
	// <1>
	// <0x76406b>|0x56b|+0x01d:'164'		pcstr sound_name = (pcstr)(*it)["sound"];
	// <0x764088>|0x588|+0x014:'165'		if ( strings::length( sound_name ) )
	// <0>
	// <0x76409c>|0x59c|+0x01d|[3]:'167'		u32 sound_type = (u32)(*it)["sound_type"];
	// <0x7640b9>|0x5b9|+0x00e:'168'			query_ext_data ext_data;
	// <0x7640c7>|0x5c7|+0x014:'169'
	// <0>
	// <0x7640db>|0x5db|+0x01b:'171'			resources::request r = resources::create_request( sound_name, resources::class_id_enum( sound_type ) );
	// <0x7640f6>|0x5f6|+0x013:'172'			requests.push_back( r );
	// <0>
	// <0x764109>|0x609|+0x00b:'174'
	// <0x764114>|0x614|+0x008:'175'
	// <0x76411c>|0x61c|+0x011:'176'
	// <0x76412d>|0x62d|+0x018:'177'			user_data_ptrs.push_back( NULL );
	// <0>									}
	// <1>
	// <0x764145>|0x645|+0x01a:'180'		pcstr particle_name = (pcstr)(*it)["particle"];
	// <0x76415f>|0x65f|+0x00d:'181'		if ( strings::length( particle_name ) )
	// <0>									{
	// <1>
	// <2>
	// <3>
	// <0x76416c>|0x66c|+0x01c|[3]:'186'		for ( u32 = i ; i < 8 ; ++i )
	// <0>										{
	// <1>
	// <0x764188>|0x688|+0x00f|[4]:'189'			resources::request r = resources::create_request( particle_name, particle_system_instance_class );
	// <0x764197>|0x697|+0x010:'190'				requests.push_back( r );
	// <0>
	// <0x7641a7>|0x6a7|+0x00b:'192'				ext_data.pair = pair;
	// <0x7641b2>|0x6b2|+0x008:'193'				ext_data.type = query_ext_data::particle;
	// <0x7641ba>|0x6ba|+0x011:'194'				ext_pair_data->push_back( ext_data );
	// <0>
	// <1>
	// <2>
	// <0x7641cb>|0x6cb|+0x018:'198'
	// <0x7641e3>|0x6e3|+0x002:'199'			}
	// <0>
	// <1>
	// <0x7641e5>|0x6e5|+0x010:'202'		manager->add_pair( pair );
	// <0x7641f5>|0x6f5|+0x005:'203'	}
	// <0>
	// <0x7641fa>|0x6fa|+0x013:'205'	if ( requests.empty( ) )
	// <0>
	// <0x76420d>|0x70d|+0x00c:'207'		parent_query.finish_query( result_success );
	// <0>								}
	// <1>								else
	// <0x764219>|0x719|+0x005:'210'	{
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x76421e>|0x71e|+0x0d8:'219'
	// <0>
	// ******
}

// STATE[81.23%|DONE]: LTCG for logging and resource_ptr methods.
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

	// FUNCTION BODY
	// <0>
	// <0x76384b>|0x01b|+0x013:'226'		if ( !data.is_successful( ) )
	// <0>
	// <0x76385e>|0x02e|+0x023|[1]:'228'		for ( u32 i = 0 ; i <  data.size( ) ; ++i )
	// <0>										{
	// <0x763881>|0x051|+0x01c:'230'				if ( !data[i].is_successful( ) )
	// <0>											{
	// <0x76389d>|0x06d|+0x0ad:'232'
	// <0>											}
	// <0x76394a>|0x11a|+0x005:'234'			}
	// <0>
	// <1>
	// <2>
	// <0x76394f>|0x11f|+0x014:'238'		query_ext_data* it	= ext_data->begin( );
	// <0x763963>|0x133|+0x015:'239'		query_ext_data* end	= ext_data->end( );
	// <0>
	// <0x763978>|0x148|+0x027|[1]:'241'	for ( u32 idx = 0 ; it != end ; ++it, ++idx )
	// <0>									{
	// <0x76399f>|0x16f|+0x009:'243'			if ( it->type == query_ext_data::decal1 )
	// <0>										{
	// <0x7639a8>|0x178|+0x042:'245'				it->pair->set_decal1( data[idx].get_unmanaged_resource( ) );
	// <0x7639ea>|0x1ba|+0x01b:'246'				VOSTOK_DELETE_IMPL( g_allocator, it->cd );
	// <0x763a05>|0x1d5|+0x005:'247'			}
	// <0x763a0a>|0x1da|+0x009:'248'			else if ( it->type == query_ext_data::decal2 )
	// <0>										{
	// <0x763a13>|0x1e3|+0x042:'250'				it->pair->set_decal2( data[idx].get_unmanaged_resource( ) );
	// <0x763a55>|0x225|+0x01d:'251'				VOSTOK_DELETE_IMPL( g_allocator, it->cd );
	// <0x763a72>|0x242|+0x009:'252'			else if ( it->type == query_ext_data::sound )
	// <0x763a7b>|0x24b|+0x042:'253'				it->pair->set_sound( data[idx].get_unmanaged_resource( ) );
	// <0x763abd>|0x28d|+0x002:'254'			else if
	// <0x763abf>|0x28f|+0x01e:'255'				it->pair->add_particle( data[idx].get_unmanaged_resource( ) );
	// <0>
	// <1>
	// <0x763add>|0x2ad|+0x005:'258'		}
	// <0>
	// <0x763ae2>|0x2b2|+0x013:'260'		data.get_parent_query( ).finish_query( result_success );
	// ******
}

} // namespace survarium
