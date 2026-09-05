// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "game_world.h"
#include "game.h"			// get_game().get_sound_world() needs game complete
#include "game_memory.h"	// g_allocator for allocator-taking vectora member
#include "camera_director.h"	// switch_to_camera (switch_to_free_fly_camera)
#include "free_fly_camera.h"	// free_fly_camera complete (game_camera* arg)
#include "player_input_handler.h"	// set_input_mode (switch_to_player_camera)
#include "base_network_client.h"	// get_current_player_position (add_enemy_position_for_team)
#include "victory_item.h"			// victory_item::put (put_victory_item)
#include "stats.h"					// stats::set_camera_stats (on_after_tick)
#include "chat_handler.h"			// chat_handler::set_mode (on_activate)
#include "npc_stats.h"				// DELETE m_active_npc_stats (clear_resources)
#include "damage_model_stats.h"		// DELETE m_damage_model_stats (clear_resources)
#include <vostok/game_core/bullet_manager.h>	// DELETE m_bullet_manager (clear_resources)
#include <vostok/game_core/respawn_point_core.h>	// selected_for_respawn (clear_player_spawn_info)
#include <vostok/input/world.h>		// input::world add/remove_handler (on_activate/on_deactivate)
#include <vostok/sound/world.h>		// get_logic_world_user (on_after_tick/on_activate)
#include <vostok/sound/world_user.h>	// set_listener_properties_interlocked/set_active_sound_scene
#include <vostok/ai/world.h>		// ai::world::clear_resources/tick (clear_resources/tick)
#include <vostok/ai_navigation/world.h>	// ai::navigation::world::clear_resources (clear_resources)
#include <vostok/physics/world.h>	// physics::world::tick (tick)
#include "flash_text_manager.h"		// DrawTextManager::Capture (tick)
#include <vostok/scaleform/sources/flash_movie.h>	// Movie::ForceCollectGarbage (on_deactivate)
#include "GFx/GFx_DrawText.h"		// Scaleform::GFx::DrawTextManager::Capture (tick)
#include "step_manager.h"			// NEW step_manager (ctor)
#include <vostok/console_command.h>	// console_commands::cc_delegate (ctor)
#include <vostok/ai/api.h>			// ai::create_world (ctor)
#include <vostok/ai_navigation/api.h>	// ai::navigation::create_world (ctor)
#include <vostok/render/facade/game_renderer.h>	// renderer().debug() (ctor)
#include <vostok/render/facade/scene_renderer.h>	// set_portal_system (on_portal_system_loaded)
#include <vostok/render/facade/common_types.h>	// render::scene_configuration (load)
#include <vostok/sound/sound_scene_creation_params.h>	// sound::sound_scene_creation_params (load)
#include <vostok/game_core/game_net_defines.h>	// match_options::victory_items_count (load)
#include <vostok/buffer_vector.h>	// buffer_vector (load)
#include "lobby_menu.h"
#include "match_client.h"			// match_client::get_match_options (on_project_loaded)
#include "lobby_client.h"			// lobby_client::status (on_project_loaded)
#include "game_project.h"			// simple_game_project::insert/m_config (on_project_loaded)
#include "game_camera.h"			// free_fly camera set_position_direction (on_project_loaded)
#include <vostok/scaleform/sources/flash_factory.h>	// m_gfx_loader for flash_text_manager (on_project_loaded)
#include <vostok/configs_binary_config.h>	// m_config->get_root() (on_project_loaded)
#include <vostok/fixed_string.h>	// fixed_string<8> name (load)
// register_cooks: the cook family + the resources::register_cook free function
#include "sound_player_cook.h"
#include "human_npc_cook.h"
#include "animation_space_graph_cook.h"
#include "player_cook.h"
#include "booby_trap_set_cook.h"
#include "booby_trap_cook.h"
#include "weapon_cook.h"
#include "damage_zone_cook.h"
#include "rifle_scope_cook.h"
#include "empty_hands_cook.h"

using namespace vostok;

// file-scope debug console commands (global namespace - mangled @@3IA / @@3_NA);
// s_max_tracers_count caps the per-frame bullet-tracer request fan-out in load()
u32	s_max_tracers_count		= 64;
bool	s_draw_respawn_debug	= false;
bool	s_draw_game_match_stats	= false;

static console_commands::cc_bool	draw_respawn_debug_cc(
	"draw_respawn_debug", s_draw_respawn_debug, true, console_commands::command_type_user_specific );
static console_commands::cc_bool	draw_match_stats_cc(
	"draw_match_stats", s_draw_game_match_stats, true, console_commands::command_type_user_specific );
static console_commands::cc_u32		bullet_tracers_max_count_cc(
	"bullet_tracers_max_count", s_max_tracers_count, 2, 0x80, true, console_commands::command_type_engine_internal );

namespace survarium {

// defined in game_world_npc.cpp (no public header); unload() walks m_npcs and
// strips each NPC's weapons
void delete_weapons( human_npc_ptr& owner );

game_world::game_world( game& game ) :
	base_game_scene( game ),
	game_ui( *this ),
	m_game_project( NULL ),
	m_player_camera( NULL ),
	m_bullet_manager( NULL ),
	m_step_manager( NULL ),
	m_ai_world( NULL ),
	m_ai_navigation_world( NULL ),
	m_death_particles_it( 0 ),
	m_active_npc_stats( NULL ),
	m_damage_model_stats( NULL ),
	m_input_mode( free_fly_mode ),
	m_is_dictionary_created( false ),
	m_active_npc_set( false ),
	m_is_loading( false ),
	m_victory_items( g_allocator )
{
	static console_commands::cc_delegate add_enemy_position_cc( "add_enemy", boost::bind( &game_world::add_enemy_position_for_team, this, _1 ), false );
	static console_commands::cc_delegate clear_enemies_position_cc( "clear_enemies", boost::bind( &game_world::clear_enemies_positions_for_team, this, _1 ), false );
	static console_commands::cc_delegate clear_player_spawn_cc( "clear_player_spawn", boost::bind( &game_world::clear_player_spawn_info, this ), false );

	m_step_manager = VOSTOK_NEW_IMPL( *g_allocator, step_manager )( );

	initialize_physics( );

	m_free_fly_camera = VOSTOK_NEW_IMPL( *g_allocator, free_fly_camera )( *this, get_camera_director( ) );
	register_cooks( );
	m_ai_world = ai::create_world( *this );
	m_ai_navigation_world = ai::navigation::create_world( *this, render_scene( ), get_game( ).renderer( ).debug( ) );
}

void game_world::register_cooks( )
{
	static sound_player_cook			s_sound_player_cook( &get_game( ).get_sound_world( ), resources::sound_player_class );
	static human_npc_cook				s_human_npc_cook( *this );
	static animation_space_graph_cook	s_animation_space_graph_cook( *m_ai_navigation_world );
	static player_cook					s_player_cook;
	static booby_trap_set_cook			s_booby_trap_set_cook( *this );
	static booby_trap_cook				s_booby_trap_cook( *this );

	resources::register_cook( &s_sound_player_cook );
	resources::register_cook( &s_human_npc_cook );
	resources::register_cook( &s_animation_space_graph_cook );
	resources::register_cook( &s_player_cook );

	static weapon_cook					s_weapon_cook( get_game( ) );
	weapon_cook::register_cooks_for_logic_states( );

	static damage_zone_cook				s_damage_zone_cook( *this );
	static rifle_scope_cook				s_rifle_scope_cook;
	static empty_hands_cook				s_empty_hands_cook;
}

game_world::~game_world( )
{
	VOSTOK_DELETE_IMPL( ::survarium::g_allocator, m_free_fly_camera );
	VOSTOK_DELETE_IMPL( ::survarium::g_allocator, m_active_npc_stats );
	VOSTOK_DELETE_IMPL( ::survarium::g_allocator, m_step_manager );
	VOSTOK_DELETE_IMPL( ::survarium::g_allocator, m_damage_model_stats );
}

void game_world::on_after_tick( )
{
	get_camera_director( ).apply( );

	get_game( ).get_sound_world( ).get_logic_world_user( ).set_listener_properties_interlocked(
		reinterpret_cast< sound::sound_scene_ptr& >( get_sound_scene( ) ),
		m_inverted_view_matrix.c.xyz( ),
		m_inverted_view_matrix.k.xyz( ),
		m_inverted_view_matrix.j.xyz( ) );

	get_game( ).get_stats( ).set_camera_stats( m_inverted_view_matrix.c.xyz( ), m_inverted_view_matrix.k.xyz( ) );
}

void game_world::tick( const u32 frame_delta_ms, const u32 current_time_in_ms, const bool is_game_paused )
{
	base_game_scene::tick( frame_delta_ms, current_time_in_ms, is_game_paused );
	tick_bullet_manager_engine( is_game_paused );

	if ( m_is_dictionary_created )
		tick_npcs( current_time_in_ms, is_game_paused );

	if ( !is_game_paused )
		m_ai_world->tick( );

	update_npc_stats( );

	if ( m_is_ui_shown )
		game_ui.update_ui( frame_delta_ms, current_time_in_ms );

	get_text_manager( )->tick( );
}

void game_world::switch_camera_mode( const input_mode_type_enum input_mode )
{
	switch ( input_mode )
	{
	case first_person_mode:
		switch_to_player_camera( true );
		break;
	case free_fly_mode:
		switch_to_free_fly_camera( );
		break;
	case third_person_mode:
		switch_to_player_camera( false );
		break;
	}
}

void game_world::switch_to_player_camera( const bool first_person_view )
{
	if ( m_player_camera )
	{
		m_input_mode = first_person_view ? first_person_mode : third_person_mode;
		m_player_camera->set_input_mode( m_input_mode );
		get_camera_director( ).switch_to_camera( m_player_camera, first_person_view ? "First Person View" : "Third Person View" );
	}
}

void game_world::switch_to_free_fly_camera( )
{
	m_input_mode = free_fly_mode;
	get_camera_director( ).switch_to_camera( m_free_fly_camera, "Free Fly View" );
}

bool game_world::empty( )
{
	return m_game_project == NULL;
}

void game_world::on_project_loaded(
	resources::queries_result&		data,
	const u32						results_offset,
	boost::function< void( resources::queries_result& ) > const&	callback
)
{
	u32 resource_index = results_offset;

	if ( !m_render_scene )
	{
		m_render_scene		= static_cast_resource_ptr< render::base_scene_ptr >( data[resource_index++].get_unmanaged_resource( ) );
		m_render_scene_view	= static_cast_resource_ptr< render::base_scene_view_ptr >( data[resource_index++].get_unmanaged_resource( ) );
		m_sound_scene		= data[resource_index++].get_unmanaged_resource( );

		game_ui.initialize_resources( data[resource_index++].get_unmanaged_resource( ) );

		for ( u32 i = 0; i < s_max_tracers_count; ++i )
			m_bullet_tracers.push_back( bullet_tracer( NULL, static_cast_resource_ptr< render::tracer_model_instance_ptr >( data[resource_index++].get_unmanaged_resource( ) ) ) );

		for ( u8 i = 0; i < 16; ++i )
			death_particles[i] = data[resource_index++].get_unmanaged_resource( );

		show_text_manager( m_text_manager = new flash_text_manager( m_game.get_flash_factory( ).m_gfx_loader ) );
	}

	if ( !m_game_material_manager )
	{
		m_game_material_manager	= static_cast_resource_ptr< game_material_manager_ptr >( data[resource_index++].get_unmanaged_resource( ) );
		m_bullet_manager		= VOSTOK_NEW_IMPL( ::survarium::g_allocator, bullet_manager )( m_game_material_manager.c_ptr( ), get_physics_world( ), this );
	}

	m_game_project = static_cast_resource_ptr< simple_game_project_ptr >( data[resource_index++].get_unmanaged_resource( ) );

	if ( m_game.get_network_client( )->has_bandwidth( ) && m_game.get_network_client( )->lobby_client( ).status( ) == lobby::surf_lobby_menu )
	{
		unload( );

		m_game.lobby_menu( ).show_match_making( false );

		m_is_loading = false;
		return;
	}

	show_ui( true );

	for ( u8 i = 0; i < m_game.get_network_client( )->match_options( ).victory_items_count; ++i )
	{
		victory_item_ptr victory_item = static_cast_resource_ptr< victory_item_ptr >( data[resource_index++].get_unmanaged_resource( ) );
		m_victory_items.push_back( victory_item );
	}

	m_game_project->insert( m_game.scheduler( ) );

	float3 camera_position	= m_game_project->m_config->get_root( )["camera"]["position"];
	float3 camera_direction	= m_game_project->m_config->get_root( )["camera"]["direction"];

	m_camera_director->set_position_direction( camera_position, camera_direction );
	m_free_fly_camera->set_position_direction( camera_position, camera_direction );

	m_input_mode = free_fly_mode;
	m_camera_director->switch_to_camera( m_free_fly_camera, "Free Fly View" );

	if ( is_active( ) )
		m_game.get_sound_world( ).get_logic_world_user( ).set_active_sound_scene( m_sound_scene, 1000, 0 );

	if ( m_game.get_network_client( )->has_bandwidth( ) )
		game_ui.initialize( m_game.get_network_client( )->match_client( ).get_match_options( ) );

	variant< 32 > user_data;
	user_data.set( m_game_project->m_config );

	resources::query_resource(
		"unused string",
		resources::portal_sector_structure_class,
		boost::bind( &game_world::on_portal_system_loaded, this, _1 ),
		g_allocator,
		&user_data );

	game_ui.initialize_minimap( );

	if ( m_game.get_network_client( )->has_bandwidth( ) )
		game_ui.show_capture_progress( true );

	if ( !callback.empty( ) )
		callback( data );

	m_game.switch_to_game_world( );

	m_is_loading = false;
}

void game_world::unload( )
{
#line 450
	switch_to_free_fly_camera( );

	if ( get_game( ).get_network_client( ) )
		get_game( ).get_network_client( )->unload( );

	for ( human_npc_ptr it_npc = m_npcs.front( ); it_npc; it_npc = m_npcs.get_next_of_object( it_npc ) )
	{
#line 456
		delete_weapons( it_npc );
#line 456
		it_npc->clear_resources( );
#line 456
	}

#line 458
	m_selected_npc = NULL;
	m_active_npc_set = false;
	m_npcs.clear( );

	get_camera_director( ).switch_to_camera( NULL, "null" );

	if ( m_game_project && m_game_project->is_inserted( ) ) {
		m_game_project->remove( );
		m_game_project = NULL;
	}

#line 469
	for ( vectora< victory_item_ptr >::iterator it = m_victory_items.begin( ); it != m_victory_items.end( ); ++it )
#line 471
		if ( ( *it )->is_inserted( ) )
			( *it )->unload( );

	m_victory_items.clear( );

	game_ui.on_unload( );
}

#line 344
void game_world::load(
	pcstr						project_resource_name,
	resources::request*			requests_begin,
	resources::request*			requests_end,
	variant< 32 > const**		user_datas_begin,
	boost::function< void( resources::queries_result& ) > const&	callback
)
{
	m_is_loading = true;

	const u8 victory_items_count = get_game( ).get_network_client( )->match_options( ).victory_items_count;
	const u32 user_datas_count = victory_items_count + s_max_tracers_count + 23;
	const u32 requests_count = requests_end - requests_begin;

	buffer_vector< resources::request >		requests		( ALLOCA( ( requests_count + user_datas_count ) * sizeof( resources::request ) ), requests_count + user_datas_count );
	buffer_vector< variant< 32 > >			user_datas		( ALLOCA( user_datas_count * sizeof( variant< 32 > ) ), user_datas_count );
	buffer_vector< variant< 32 > const* >	user_data_ptrs	( ALLOCA( ( requests_count + user_datas_count ) * sizeof( variant< 32 > const* ) ), requests_count + user_datas_count );

	for ( ; requests_begin != requests_end; ++requests_begin, ++user_datas_begin ) {
		requests.push_back( *requests_begin );
		user_data_ptrs.push_back( *user_datas_begin );
	}


	if ( !render_scene( ) ) {
		m_damage_model_stats = VOSTOK_NEW_IMPL( *g_allocator, damage_model_stats )( get_game( ).ui_world( ) );
		m_active_npc_stats = VOSTOK_NEW_IMPL( *g_allocator, npc_stats )( get_game( ).ui_world( ) );

		render::scene_configuration scene_configuration;
		scene_configuration.m_create_terrain			= false;
		scene_configuration.m_create_particle_world		= true;
		scene_configuration.m_has_clouds			= false;
		scene_configuration.m_create_speedtree_world	= false;
		scene_configuration.m_create_grass_world		= false;
		scene_configuration.m_sky_enabled			= true;
		scene_configuration.m_use_occlusion_culling	= true;

		sound::sound_scene_creation_params sound_configuration;
		sound_configuration.proxies_count		= 0x80;
		sound_configuration.propagators_count	= 0xc4;
		sound_configuration.receivers_count		= 1;

		user_datas.push_back( variant< 32 >( ) );
		user_datas.back( ).set( scene_configuration );

		user_datas.push_back( variant< 32 >( ) );
		user_datas.back( ).set( sound_configuration );

		requests.push_back( resources::create_request( "game_scene", resources::scene_class ) );
		user_data_ptrs.push_back( &user_datas[0] );

		requests.push_back( resources::create_request( "game_scene_view", resources::scene_view_class ) );
		user_data_ptrs.push_back( NULL );

		requests.push_back( resources::create_request( "game_sound_scene", resources::sound_scene_class ) );
		user_data_ptrs.push_back( &user_datas[1] );

		requests.push_back( resources::create_request( "resources/flash_movies/hud.swf", resources::flash_movie_class ) );
		user_data_ptrs.push_back( NULL );

		for ( u32 i = 0; i < s_max_tracers_count; ++i ) {
			requests.push_back( resources::create_request( "weapons/trace", resources::tracer_model_instance_class ) );
			user_data_ptrs.push_back( NULL );
		}

		for ( u32 i = 0; i < 16; ++i ) {
			requests.push_back( resources::create_request( "player_death", resources::particle_system_instance_class ) );
			user_data_ptrs.push_back( NULL );
		}
	}

	if ( m_game_project.c_ptr( ) )
		unload( );

	LOG_INFO( "game_world::load : %s", project_resource_name );

	if ( !m_game_material_manager )
	{
		requests.push_back( resources::create_request( "game_material_manager", resources::game_material_manager_class ) );
		user_data_ptrs.push_back( NULL );
	}

	user_datas.push_back( variant< 32 >( ) );
	user_datas.back( ).set( static_cast< base_game_scene* >( this ) );

	requests.push_back( resources::create_request( project_resource_name, resources::client_game_project_class ) );
	user_data_ptrs.push_back( &user_datas.back( ) );

	for ( u8 i = 0; i < victory_items_count; ++i )
	{
		fixed_string< 8 > name;
		name.assignf( "vp_%d", i );
		requests.push_back( resources::create_request( name.c_str( ), resources::victory_item_class ) );
		user_data_ptrs.push_back( NULL );
	}


	resources::query_resources(
		requests.begin( ),
		requests.size( ),
		boost::function< void( resources::queries_result& ) >(
			boost::bind( &game_world::on_project_loaded, this, _1, requests_count, callback ) ),
		g_allocator,
		user_data_ptrs.begin( ) );
}

void game_world::on_portal_system_loaded( resources::queries_result& data )
{
	if ( data.is_successful( ) )
	{
		m_portal_sector_structure = static_cast_resource_ptr< render::culling::portal_sector_structure_ptr >( data[0].get_unmanaged_resource( ) );
		scene_renderer( ).set_portal_system( render_scene( ).c_ptr( ), data[0].get_unmanaged_resource( ) );

		if ( m_sound_scene )
			m_game.get_sound_world( ).get_logic_world_user( ).set_active_sound_scene( reinterpret_cast< sound::sound_scene_ptr& >( m_sound_scene ), m_portal_sector_structure, 0, 0 );
	}
}

void game_world::on_activate( )
{
	base_game_scene::on_activate( );

	get_game( ).input_world( ).add_handler( *this );

	if ( get_sound_scene( ) )
		get_game( ).get_sound_world( ).get_logic_world_user( ).set_active_sound_scene(
			reinterpret_cast< sound::sound_scene_ptr& >( get_sound_scene( ) ), 0, 0 );

	if ( get_game( ).get_network_client( )->has_bandwidth( ) )
		get_game( ).get_chat_handler( ).set_mode( true );
}

void game_world::on_deactivate( )
{
	base_game_scene::on_deactivate( );

	get_game( ).input_world( ).remove_handler( *this );

	get_game( ).deactivate_main_menu( );

	game_ui.get_ui( )->movie->ForceCollectGarbage( );
}

void game_world::show_ui( bool b_show )
{
	if ( m_is_ui_shown == b_show )
		return;

	if ( b_show )
	{
		show_movie( game_ui.get_ui( ) );
		if ( get_game( ).get_network_client( )->has_bandwidth( ) )
			get_game( ).get_chat_handler( ).show( this );
	}
	else
	{
		hide_movie( game_ui.get_ui( ) );
		if ( get_game( ).get_chat_handler( ).is_active( ) )
			get_game( ).get_chat_handler( ).hide( this );
	}

	m_is_ui_shown = b_show;
}

void game_world::clear_resources( )
{
	destroy_physics( );

	if ( get_sound_scene( ) )
		get_game( ).get_sound_world( ).get_logic_world_user( ).remove_sound_scene(
			reinterpret_cast< sound::sound_scene_ptr& >( get_sound_scene( ) ) );

	VOSTOK_DELETE_IMPL( ::survarium::g_allocator, m_damage_model_stats );
	VOSTOK_DELETE_IMPL( ::survarium::g_allocator, m_active_npc_stats );

	m_ai_navigation_world->clear_resources( );
	m_ai_world->clear_resources( );

	show_ui( false );

	if ( get_text_manager( ) )
		hide_text_manager( get_text_manager( ) );

	if ( m_game_material_manager )
	{
		m_game_material_manager->clear_resources( );
		m_game_material_manager = NULL;

		VOSTOK_DELETE_IMPL( ::survarium::g_allocator, m_bullet_manager );
	}
}

sound::world& game_world::get_sound_world( ) const
{
	return get_game( ).get_sound_world( );
}

void game_world::add_enemy_position_for_team( pcstr const team_name )
{
	float3 const position = get_game( ).get_network_client( )->get_current_player_position( );

	if ( !strcmp( team_name, "1" ) )
		m_enemies_for_team_1.push_back( position );
	else if ( !strcmp( team_name, "2" ) )
		m_enemies_for_team_2.push_back( position );
}

void game_world::clear_enemies_positions_for_team( pcstr const team_name )
{
	if ( !strcmp( team_name, "1" ) )
		m_enemies_for_team_1.clear( );
	else if ( !strcmp( team_name, "2" ) )
		m_enemies_for_team_2.clear( );
}

void game_world::clear_player_spawn_info( )
{
	map< u32, respawn_point_core*, std::less< u32 > >::iterator it = m_game_project->m_respawn_points.begin( );
	map< u32, respawn_point_core*, std::less< u32 > >::iterator end = m_game_project->m_respawn_points.end( );
	for ( ; it != end; ++it )
		it->second->selected_for_respawn = false;
}

void game_world::on_player_killed( player_ptr& player, const u8 arg_1, const u8 arg_2 )
{
}

u32 game_world::get_current_time_in_ms( ) const
{
	return get_game( ).game_time_ms( );
}

void game_world::set_local_player_camera( player_input_handler* camera )
{
	m_player_camera = camera;
}

void game_world::put_victory_item( u8 item_id, float4x4 const& transform )
{
	m_victory_items[item_id]->put( get_physics_world( ), transform, get_game( ).scheduler( ) );
}


} // namespace survarium
