// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "game.h"
#include "game_map_description.h"

#include <vostok/console_command.h>	// cc_float (max_angular_velocity_command)
#include <vostok/console_command_processor.h>	// console_commands::save (cfg_save_*)
#include <vostok/command_line_extensions.h>
#include <vostok/memory_extensions.h>	// memory::g_mt_allocator (cfg_save_*)
#include <vostok/render/world.h>	// game_renderer() (ctor init)
#include <vostok/render/facade/common_types.h>
#include <vostok/render/facade/game_renderer.h>	// renderer().ui() (draw_debug_window)
#include <vostok/render/facade/scene_renderer.h>
#include <vostok/input/world.h>	// m_input_world->on_activate/on_deactivate
#include <vostok/ui/world.h>	// m_ui_world->create_window (debug window)
#include <vostok/sound/world.h>	// m_sound_world.get_logic_world_user (pause/resume)
#include <vostok/sound/world_user.h>	// sound time-scale accessors (pause/resume)
#include <vostok/sound/sound_debug_stats.h>	// update_stats: s_draw_snd_stats tail
#include <vostok/engine/console.h>	// m_console->get_active/on_activate (toggle_console)
#include <vostok/resources.h>	// query_resources / request (query_base_resources) + fill_stats (draw_debug_window)
#include <vostok/text_tree.h>	// strings::text_tree (draw_debug_window)
#include <vostok/tasks_system.h>	// tasks::fill_stats (draw_debug_window)
#include <vostok/ui/ui.h>	// ui::window::remove_all_children (draw_debug_window)

// SetWindowTextA (USER32) is stripped from os_include.h by NOUSER; declare the import
// directly for on_application_activate's title-bar update.
extern "C" __declspec( dllimport ) int __stdcall SetWindowTextA( void* hWnd, char const* lpString );

using vostok::console_commands::cc_bool;
using vostok::console_commands::cc_u32;
using vostok::console_commands::cc_delegate;
using vostok::console_commands::command_type_user_specific;
using vostok::console_commands::command_type_engine_internal;

#include "scaleform_movie_cook.h"	// its out-of-line bodies live here per the PDB
#include <vostok/scaleform/sources/flash_factory.h>	// m_factory.build_movie (on_raw_data_loaded)
#include <vostok/scaleform/sources/flash_movie_resource.h>	// flash_movie_resource (delete_resource)
#include <vostok/scaleform/sources/flash_movie.h>	// delete movie (delete_resource)
#include "base_game_scene.h"	// m_active_scene->on_activate/on_deactivate (switch_to_scene)
#include "base_network_client.h"	// m_network_client virtuals (commit_suicide etc.)
#include "network_client.h"	// network_client (create_network_client)
#include "stats.h"	// m_stats (~game)
#include "stats_graph.h"	// m_fps_graph (~game)
#include "key_binder.h"	// m_key_binder (~game)
#include "chat_handler.h"	// m_chat_handler (~game)
#include "global_input_handler.h"
#include "main_menu.h"	// main_menu derives base_game_scene (switch_to_main_menu)
#include "lobby_menu.h"	// lobby_menu derives base_game_scene (switch_to_lobby)
#include "login_menu.h"	// login_menu derives base_game_scene + set_status (switch_to_login)
#include "animated_model_instance_cook.h"	// register_cooks function-statics
#include "profile_skin_visual_cook.h"
#include <vostok/game_core/inventory_cook.h>
#include <vostok/game_core/player_parameters_modifyer_cook.h>
#include <vostok/game_core/items_dictionary_cook.h>
#include <vostok/game_core/game_material_manager_cook.h>
#include "project_cooker_simple.h"
#include <vostok/game_core/animation_analysis_result_cook.h>
#include <vostok/game_core/ladder_cook.h>
#include <vostok/game_core/weapon_user_animations_container_cook.h>
#include "victory_item_cook.h"

// File-scope console-command statics + their command functions. The PDB mangles these
// undecorated (global scope, not survarium::); the dynamic initializers / atexit
// destructors the compiler emits for each are the matched targets. Source order (init
// line numbers): draw_snd_stats(58), draw_stats(70), show_profiler(74), cfg_save pair
// (126/127), particle commands (138/141).

vostok::command_line::key s_net_login_client( "client", "", "", "connect to server" );
static vostok::command_line::key s_is_spectator( "spectator", "", "", "connect as spectator" );

static bool s_draw_snd_stats_value;
static cc_bool s_draw_snd_stats( "draw_sound_stats", s_draw_snd_stats_value, true, command_type_user_specific );

static bool s_draw_stats_value = false;
static cc_bool s_draw_stats( "draw_stats", s_draw_stats_value, true, command_type_user_specific );

static bool s_show_profiler;
static cc_bool s_show_profiler_command( "show_profiler", s_show_profiler, false, command_type_user_specific );

namespace survarium {

float g_max_angular_velocity[ 2 ] = {
	math::deg2rad( 2880000.f ), math::deg2rad( 2880000.f )
};

// TU-local (canonical headers/max_angular_velocity_command.h; owner mapping
// in git show :temp/triage_log.md) - the type of the s_max_angular_velocity_command static
class max_angular_velocity_command : public console_commands::cc_float {
	typedef console_commands::cc_float super;

public:
					max_angular_velocity_command	(
						pcstr			name,
						const float		min,
						const float		max,
						bool			serializable,
						const console_commands::command_type	arg_4 /* console_commands::command_type command_type */,
						const console_commands::execution_filter	arg_5 /* console_commands::execution_filter execution_filter */
					);

	inline	void	set_engine						( engine_user::engine& arg_0 ) { m_engine = &arg_0; }

private:
	virtual	void	execute							( pcstr arg_0 ) override;

public:

private:
	/* 0x0000 */	/* console_commands::cc_float */
	/* 0x0050 */	engine_user::engine*	m_engine;
	/* 0x0054 */	float					m_value;
}; // class max_angular_velocity_command

STATIC_SIZE_ASSERT(max_angular_velocity_command, 0x58);

 max_angular_velocity_command::max_angular_velocity_command(
	pcstr			name,
	const float		min,
	const float		max,
	bool			serializable,
	const console_commands::command_type	arg_4 /* console_commands::command_type command_type */,
	const console_commands::execution_filter	arg_5 /* console_commands::execution_filter execution_filter */
) :
	super( name, m_value, min, max, serializable, arg_4, arg_5 ),
	m_engine( NULL ),
	m_value( 720.f )
{
}

void max_angular_velocity_command::execute( pcstr args )
{
	super::execute( args );
	g_max_angular_velocity[ 1 ] = math::deg2rad( m_value );

	float2 const window_size = m_engine->get_render_window_size( );
	float const aspect_ratio = window_size.x / window_size.y;
	g_max_angular_velocity[ 0 ] = aspect_ratio * g_max_angular_velocity[ 1 ];
}

} // namespace survarium
static survarium::max_angular_velocity_command s_max_angular_velocity_command(
	"max_angular_velocity",
	360.f,
	3600.f,
	true, command_type_engine_internal, vostok::console_commands::execution_filter_general
);

void cfg_save_user( )
{
	vostok::console_commands::save( "user.cfg", command_type_user_specific, memory::g_mt_allocator );
}

// target body is empty (single `ret`); the system-save logic is compiled out in this build.
void cfg_save_system( )
{
}

static cc_delegate cfg_save_system_cc( "cfg_save_system", boost::bind( &cfg_save_system ), false );
static cc_delegate cfg_save_user_cc( "cfg_save_user", boost::bind( &cfg_save_user ), false );

static u32 s_max_particles_value = 100;
static cc_u32 s_max_particles( "max_particles", s_max_particles_value, 0, 1000, true, command_type_engine_internal );

static u32 s_particle_lod_value = 0;
static cc_u32 s_particle_lod( "particle_lod", s_particle_lod_value, 0, 10, true, command_type_engine_internal );

static bool s_show_detailed_statistics_value;
static cc_bool show_detailed_statistics( "r_show_detailed_statistics", s_show_detailed_statistics_value, true, command_type_user_specific );

namespace survarium {
// claude@NOTE: exact statement/local structure; residual is LTCG register and call convention scheduling.
 game::game(
	engine_user::engine&	engine,
	render::world&			render_world,
	sound::world&			sound,
	network::world&			network
) :
	hide_game_stats( false ),
	m_engine( engine ),
	m_render_world( render_world ),
	m_sound_world( sound ),
	m_network_world( network ),
	m_input_world( NULL ),
	m_ui_world( NULL ),
	m_renderer( render_world.game_renderer( ) ),
	m_game_world( *this ),
	m_main_menu( NULL ),
	m_lobby_menu( NULL ),
	m_login_menu( NULL ),
	m_scheduler( g_allocator ),
	m_active_scene( NULL ),
	m_text_wnd( NULL ),
	m_network_client( NULL ),
	m_is_active( false ),
	m_first_frame_time_in_ms( 0 ),
	m_previous_frame_time_in_ms( 0 ),
	m_permanent_time_in_ms( 0 ),
	m_last_frame_time( 0.f ),
	m_current_frame_id( u32(-1) ),
	m_current_time_in_ms( 0 ),
	m_enabled( false ),
	m_initialized( false ),
	m_last_sound_timescale_factor( 1.f ),
	m_is_paused( false ),
	m_lpv_geometry_builded( false ),
	m_game_options( *this ),
	m_debug_window_type( debug_window_none ),
	m_debug_window( NULL )
{
	s_max_angular_velocity_command.set_engine( engine );

	static cc_delegate s_build_lpv_geometry(
		"build_lpv_geometry",
		boost::bind( &game::build_lpv_geometry, this ),
		false,
		command_type_engine_internal
	);

	static cc_delegate s_reload_shaders(
		"reload_shaders",
		boost::bind( &render::scene_renderer::reload_shaders, &m_renderer.scene( ) ),
		false,
		command_type_engine_internal
	);

	static fixed_string< 512 > s_current_render_configuration;
	static console_commands::cc_string s_current_render_configuration_cc(
		"r_current_render_configuration",
		s_current_render_configuration.get_buffer( ),
		256,
		true,
		command_type_user_specific
	);

	static cc_delegate s_reload_modified_textures(
		"reload_modified_textures",
		boost::bind( &render::scene_renderer::reload_modified_textures, &m_renderer.scene( ) ),
		false,
		command_type_engine_internal
	);

	static cc_delegate pause_game_command( "pause", boost::bind( &game::pause, this ), false, command_type_engine_internal );
	static cc_delegate resume_game_command( "resume", boost::bind( &game::resume, this ), false, command_type_engine_internal );
	static cc_delegate commit_suicide_cc( "suicide", boost::bind( &game::commit_suicide, this ), false, command_type_engine_internal );

	m_timer.start( );
	m_permanent_timer.start( );

	m_flash_factory = VOSTOK_NEW_IMPL( *g_allocator, flash_factory )( *this );

	static inventory_cook s_inventory_cook;
	static player_parameters_modifyer_cook s_player_parameters_cook;
	static items_dictionary_cook s_items_dictionary_cook;
	static scaleform_movie_cook s_scaleform_movie_cook( *m_flash_factory );
	static profile_skin_visual_cook s_profile_skin_visual_cook( *this );

	query_base_resources( );

	m_chat_handler = VOSTOK_NEW_IMPL( *g_allocator, chat_handler )( *this );
}

void game::execute_scaleform_command( scaleform_render_command command )
{
	m_renderer.execute_scaleform_command	( command );
}

void game::build_lpv_geometry( )
{
	m_renderer.scene( ).build_lpv_geometry( m_game_world.render_scene( ) );
}

 game::~game( )
{
	vostok::console_commands::save	( "user.cfg", command_type_user_specific, memory::g_mt_allocator );

	VOSTOK_DELETE_IMPL				( ::survarium::g_allocator, m_network_client );
	VOSTOK_DELETE_IMPL				( ::survarium::g_allocator, m_main_menu );
	VOSTOK_DELETE_IMPL				( ::survarium::g_allocator, m_lobby_menu );
	VOSTOK_DELETE_IMPL				( ::survarium::g_allocator, m_login_menu );
	VOSTOK_DELETE_IMPL				( ::survarium::g_allocator, m_console );
	VOSTOK_DELETE_IMPL				( ::survarium::g_allocator, m_stats );
	VOSTOK_DELETE_IMPL				( ::survarium::g_allocator, m_fps_graph );

	VOSTOK_DELETE_IMPL				( ::survarium::g_allocator, m_flash_factory );
	VOSTOK_DELETE_IMPL				( ::survarium::g_allocator, m_key_binder );
	VOSTOK_DELETE_IMPL				( ::survarium::g_allocator, m_chat_handler );

	deinitialize_modules			( );
}

math::uint2 parse_resolution( pcstr in_str )
{
	char xy_str[16];

	if ( in_str && in_str[0] )
	{
		pcstr const height_str	= vostok::strings::get_token( in_str, xy_str, vostok::strings::length( in_str ), 'x' );
		if ( height_str )
		{
			u32 const width		= atoi( xy_str );
			u32 const height	= atoi( height_str );
			if ( width && height )
				return vostok::math::uint2( width, height );
		}
	}

	return vostok::math::uint2( 1280, 720 );
}

void game::on_configs_loaded( resources::queries_result& result )
{
	initialize_modules				( );

	m_key_binder					= VOSTOK_NEW_IMPL( ::survarium::g_allocator, key_binder )( *this );

	load_cc_script					( result[0].get_managed_resource( ), false );
	load_cc_script					( result[1].get_managed_resource( ), true );

	m_text_translator.load_text_localization( );

	render::output_window_configuration window_configuration;
	window_configuration.hwnd				= m_engine.get_render_window_handle( );
	window_configuration.scaleform_render_queue	= m_flash_factory->m_render_thread_queue;
	window_configuration.create_flash_renderer	= true;
	window_configuration.windowed				= true;

	if ( !m_engine.command_line_editor( ) )
	{
		console_commands::console_command* fullscreen_command = console_commands::find( "r_fullscreen" );
		math::uint2 resolution_xy = parse_resolution( static_cast< console_commands::cc_string* >( console_commands::find( "r_resolution" ) )->get_value( ) );
		window_configuration.width		= resolution_xy.x;
		window_configuration.height		= resolution_xy.y;
		window_configuration.windowed	= !static_cast< console_commands::cc_value< bool >* >( fullscreen_command )->get_value( );
	}

	resources::user_data_variant window_data;
	window_data.set					( window_configuration );
	resources::user_data_variant output_window_data;
	resources::user_data_variant const* data[] = { &window_data };

	resources::request requests[] =
	{
		resources::create_request( "game_render_output_window", resources::render_output_window_class ),
	};

	resources::query_resources		(
		requests,
		1,
		boost::bind					( &game::on_render_output_window_created, this, _1 ),
		g_allocator,
		data,
		NULL,
		assert_on_fail_true
	);
}

void game::on_render_output_window_created( resources::queries_result& data )
{
	m_render_output_window			= static_cast_resource_ptr< render::render_output_window_ptr >( data[0].get_unmanaged_resource( ) );

	resources::request requests[]	=
	{
		resources::create_request	( "items_dictionary", resources::items_dictionary_class ),
		resources::create_request	( "resources/flash_movies/chat.swf", resources::flash_movie_class ),
	};

	resources::query_resources		(
		requests,
		2,
		boost::bind					( &game::on_base_resources_created, this, _1 ),
		g_allocator,
		NULL,
		NULL,
		assert_on_fail_true
	);
}
// claude@NOTE: target keeps key::is_set_as_string out of line and passes create_and_assign_network_client this on the stack.
void game::on_base_resources_created( resources::queries_result& data )
{
	m_items_dictionary = static_cast_resource_ptr< items_dictionary_ptr >( data[0].get_unmanaged_resource( ) );
	m_initialized = true;

	static global_input_handler g_input_handler( *this );
	m_input_world->add_handler( g_input_handler );

	register_cooks( );

	m_game_options.initialize( );

	m_chat_handler->initialize( data[1].get_unmanaged_resource( ) );

	m_text_wnd = m_ui_world->create_window( );
	m_text_wnd->set_position( float2( 300.f, 0.f ) );
	m_text_wnd->set_size( float2( 600.f, 600.f ) );
	m_text_wnd->set_visible( true );


	m_console = m_engine.create_game_console( ui_world( ), input_world( ) );
	m_stats = VOSTOK_NEW_IMPL( *g_allocator, stats )( *m_ui_world );

	m_fps_graph = VOSTOK_NEW_IMPL( *g_allocator, stats_graph )( 1.f, math::infinity, 30.f, 60.f, 0xff00ff00 );

	m_main_menu = VOSTOK_NEW_IMPL( *g_allocator, class main_menu )( *this );

	create_debug_window( );

	m_viewport.left = 0.f; m_viewport.top = 0.f; m_viewport.right = 1.f; m_viewport.bottom = 1.f;

	enable( m_enabled );

	if ( m_is_active )
	{
		m_is_active = false;
		on_application_deactivate( );
	}

	fixed_string< 512 > client_str;
	if ( s_net_login_client.is_set_as_string( &client_str ) )
	{
		fixed_string< 512 > host;
		u32 const offset = client_str.find( ':' );
		if ( offset != u32( -1 ) )

			create_and_assign_network_client( client_str, s_is_spectator.is_set( ) );
	}
	else
	{

		create_and_assign_network_client( "188.93.23.27:25100", false );

	}
}

void game::create_and_assign_network_client( fixed_string< 512 > client_options, const bool is_spectator )
{
	m_network_client_options		= client_options;

	if ( is_spectator )
	{
		create_network_client		( true );
		return;
	}

	create_lobby_menu				( );
	create_login_menu				( );
}

void game::on_queried_by_network_client_scene_ready( scene_ready_type scene_ready )
{
	switch ( scene_ready )
	{
		case login_scene_ready:
		{
			m_login_scene_ready		= true;
			break;
		}
		case lobby_scene_ready:
		{
			m_lobby_scene_ready		= true;
			break;
		}
		default:
			return;
	}

	if ( m_lobby_scene_ready && m_login_scene_ready )
		create_network_client		( false );
}

// claude@NOTE: structure + 6 stmts match. Residual is LTCG convention only - the
// target passes `this` ON THE STACK (ret 8, mov ebx,[esp+42Ch]) and pushes
// is_spectator to the network_client ctor; base keeps __thiscall (this in ecx,
// ret 4) and the ctor's is_spectator goes via register. The 4-byte frame shift
// cascades through every [esp+XX] slot. Not source-steerable (arg-passing).
void game::create_network_client( const bool is_spectator )
{
	fixed_string< 512 > host;

	const u32 offset				= m_network_client_options.find( ':' );
	if ( offset == u32( -1 ) )
		return;

	host							= m_network_client_options.substr( 0, offset );

	const u16 port					= atoi( m_network_client_options.c_str( ) + offset + 1 );

	set_network_client				(
		VOSTOK_NEW_IMPL( ::survarium::g_allocator, class network_client )	( *this, is_spectator ),
		host.c_str					( ),
		port,
		is_spectator
	);
}

void game::create_lobby_menu( )
{
	m_lobby_menu					= VOSTOK_NEW_IMPL( ::survarium::g_allocator, class lobby_menu )( *this );
}

void game::create_login_menu( )
{
	m_login_menu					= VOSTOK_NEW_IMPL( ::survarium::g_allocator, class login_menu )( *this );
}

void game::query_base_resources( )
{
	register_console_commands		( );

	resources::request requests[]	=
	{
		resources::create_request	( "resources/startup.cfg", resources::raw_data_class ),
		resources::create_request	( "user_data/user.cfg", resources::raw_data_class ),
	};

	resources::query_resources		(
		requests,
		2,
		boost::bind					( &game::on_configs_loaded, this, _1 ),
		g_allocator,
		NULL,
		NULL,
		assert_on_fail_true
	);
}

void game::enable( bool value )
{
	m_enabled						= value;

	if ( !m_render_output_window )
		return;

	if ( value )
		m_input_world->acquire		( );
	else
		m_input_world->unacquire	( );
}

void game::on_renderer_created( resources::queries_result& data )
{
}

#line 595
void game::on_config_loaded( resources::queries_result& data, bool create_renderer )
{
	if ( !data.is_successful( ) )
	{
		LOG_ERROR					( "config file loading FAILED" );
		return;
	}

	load_cc_script					( data[0].get_managed_resource( ), create_renderer );
}

void game::load_cc_script( resources::managed_resource_ptr cfg, bool create_renderer )
{
	if ( cfg && cfg.c_ptr( ) )
	{
		resources::pinned_ptr_const< u8 > pinned_data( cfg );
		memory::reader F				( pinned_data.c_ptr( ), pinned_data.size( ) );
		console_commands::load			( F, console_commands::execution_filter_general );
	}

	if ( create_renderer )
	{
		mutable_buffer creation_buffer	( (pvoid)" ", 1 );
		resources::user_data_variant ud;
		ud.set							( &m_renderer.engine_world( ) );
		resources::user_data_variant const* data[] = { &ud };

		resources::query_create_resources(
			&resources::creation_request( "renderer", const_buffer( creation_buffer ), resources::renderer_class ),
			1,
			boost::bind					( &game::on_renderer_created, this, _1 ),
			g_allocator,
			data,
			NULL,
			assert_on_fail_true
		);
	}
}

void game::load_config_query( pcstr cfg_name, bool create_renderer )
{
	resources::query_resources_and_wait	( &resources::create_request( cfg_name, resources::raw_data_class ), 1, boost::bind( &game::on_config_loaded, this, _1, create_renderer ), g_allocator, NULL, NULL, assert_on_fail_false );
}

void game::register_console_commands( )
{
	static console_commands::cc_delegate	game_exit_cc( "quit", boost::bind( &game::exit, this, _1 ), false );
	static console_commands::cc_delegate	cfg_load_cc( "cfg_load", boost::bind( &game::load_config_query, this, _1, false ), true );
	static console_commands::cc_delegate	cfg_load_level( "level_load", boost::bind( &game::load_cmd, this, _1 ), true );
	static console_commands::cc_delegate	cfg_unload_level( "level_unload", boost::bind( &game::unload_cmd, this, _1 ), false );
}

void game::switch_to_scene( base_game_scene* scene )
{
	if ( m_active_scene == scene )
		return;

	if ( m_active_scene )
		m_active_scene->on_deactivate( );

	m_active_scene	= scene;
	m_active_scene->on_activate		( );
}

void game::toggle_console( )
{
	if ( m_console->get_active( ) )
		m_console->on_deactivate	( );
	else
		m_console->on_activate		( );
}

void game::exit( pcstr str )
{
	unload							( str, true );

	if ( m_engine.command_line_editor( ) )
		m_engine.enter_editor_mode	( );
	else
		m_engine.exit				( 0 );
}

void game::tick( const u32 current_frame_id )
{
	u32 const		current_time_in_ms		= m_timer.get_elapsed_msec( );
	u32 const		frame_delta				= current_time_in_ms - m_current_time_in_ms;
	m_current_time_in_ms					= current_time_in_ms;

	m_permanent_time_in_ms					= m_permanent_timer.get_elapsed_msec( );

	m_current_frame_id						= current_frame_id;

	m_scheduler.on_frame					( frame_delta, m_current_time_in_ms );

	if ( !m_active_scene || !m_active_scene->render_scene( ).c_ptr( ) )
	{
		if ( m_network_client )
			m_network_client->tick			( m_current_time_in_ms, m_is_paused );

		m_renderer.end_frame				( );
		return;
	}

	m_input_world->tick						( m_permanent_time_in_ms );
	m_active_scene->tick					( frame_delta, m_current_time_in_ms, m_is_paused );

	if ( m_game_options.is_active( ) )
		m_game_options.tick					( frame_delta, m_current_time_in_ms, m_is_paused );

	if ( m_chat_handler->is_active( ) )
		m_chat_handler->tick				( frame_delta );

	if ( m_network_client )
		m_network_client->tick				( m_current_time_in_ms, m_is_paused );

	m_active_scene->on_after_tick			( );

	m_text_wnd->remove_all_children			( );
	m_text_wnd->tick						( );
	m_text_wnd->draw						( m_ui_world->get_renderer( ), m_active_scene->render_scene_view( ) );

	m_ui_world->tick						( );

	if ( m_console->get_active( ) )
		m_console->tick						( m_active_scene->render_scene_view( ) );

	update_stats							( current_frame_id );

	if ( m_debug_window_type != debug_window_none && !m_console->get_active( ) )
		draw_debug_window					( );

	m_renderer.draw_scene					(
		m_active_scene->render_scene( ),
		m_active_scene->render_scene_view( ),
		render_output_window( ),
		viewport( ),
		ui_world( ).default_font( )
	);

	m_renderer.end_frame					( );
}

void game::update_stats( const u32 current_frame_id )
{
	float const last_frame_time				= float( m_permanent_time_in_ms - m_first_frame_time_in_ms ) * math::epsilon_3;
	m_fps_graph->add_value					( last_frame_time, math::is_zero( last_frame_time - m_last_frame_time ) ? 10000.f : 1.f / ( last_frame_time - m_last_frame_time ) );

	m_previous_frame_time_in_ms				= m_permanent_time_in_ms;

	m_last_frame_time						= last_frame_time;
	if ( m_lobby_menu && m_lobby_menu->is_active( ) )
		m_lobby_menu->set_fps_stats			( m_fps_graph->cumulative_count( ) / m_fps_graph->cumulative_time( ) );

	if ( s_draw_stats_value && !hide_game_stats && m_game_world.is_active( ) )
	{
		m_stats->set_fps_stats				( m_fps_graph->cumulative_count( ) / m_fps_graph->cumulative_time( ) );

		char buff[64];
		vostok::sprintf						( buff, "pending queries: %d", vostok::resources::pending_queries_count( ) );
		m_stats->set_resources_stats		( buff );

		m_stats->draw						( ui_world( ).get_renderer( ), m_active_scene->render_scene_view( ) );
	}

	static bool draw_fps_graph				= false;
	static cc_bool fps_graph				( "draw_fps_graph", draw_fps_graph, false, command_type_user_specific );

	if ( draw_fps_graph && !hide_game_stats && m_game_world.is_active( ) )
	{
		m_fps_graph->set_time_interval		( 5.f );
		// only top_margin (574) + height (128) survive; arg_4/5/6 are UNREFERENCED in
		// render() so LTCG drops them - the trailing args here are placeholders.
		m_fps_graph->render					( ui_world( ).get_renderer( ), m_active_scene->render_scene_view( ), 574, 128, current_frame_id, 1004, 10 );
	}
	else
		m_fps_graph->set_time_interval		( 1.f );

	if ( s_draw_snd_stats_value && m_game_world.is_active( ) )
	{
		if ( !m_sound_stats )
			m_sound_stats = VOSTOK_NEW_IMPL( ::survarium::g_allocator, sound::sound_debug_stats )( g_allocator,
				m_sound_world.get_logic_world_user( ), m_game_world.get_sound_scene( ), *m_ui_world );

		if ( m_sound_stats->is_stats_available( ) )
		{
			sound::sound_debug_stats::set_debug_draw_mode( sound::sound_debug_stats::overall );
			m_sound_stats->draw( m_active_scene->render_scene( ), m_active_scene->render_scene_view( ) );
		}
	}
}

void game::clear_resources( )
{
	destroy_debug_window			( );

	m_main_menu->clear_resources	( );

	if ( m_lobby_menu )
		m_lobby_menu->clear_resources( );

	if ( m_login_menu )
		m_login_menu->clear_resources( );

	if ( m_active_scene )
		m_active_scene->on_deactivate( );

	if ( m_network_client )
	{
		m_network_client->disconnect( );
		VOSTOK_DELETE_IMPL			( ::survarium::g_allocator, m_network_client );
	}

	m_input_world->clear_resources	( );
	m_ui_world->clear_resources		( );

	if ( !m_game_world.empty( ) )
		m_game_world.unload			( );

	m_game_world.clear_resources	( );
}

void game::load_cmd( pcstr project_name )
{
	load							( project_name );
}

void game::unload_cmd( pcstr s )
{
	unload							( s, false );
}

void game::load(
	pcstr const						project_resource_name,
	resources::request* const		requests_begin,
	resources::request* const		requests_end,
	variant< 32 > const** const		user_datas_begin,
	boost::function< void( resources::queries_result& ) > const&	callback
)
{
	m_project_resource_name			= project_resource_name;

	m_game_world.load				( m_project_resource_name.c_str( ), requests_begin, requests_end, user_datas_begin, callback );

	m_lpv_geometry_builded			= false;
}

void game::load( pcstr const project_resource_name )
{
	m_network_client->load			( project_resource_name, m_game_world.get_camera_director( ) );
}

void game::unload( pcstr __formal, bool destroying )
{
	if ( !m_game_world.empty( ) )
		m_game_world.unload			( );

	if ( !destroying )
		switch_to_scene				( m_main_menu );
}

void game::switch_to_game_world( )
{
	switch_to_scene					( &m_game_world );
}

void game::switch_to_main_menu( )
{
	switch_to_scene					( m_main_menu );
}

void game::switch_to_lobby( )
{
	if ( !m_network_client->has_bandwidth( ) )
		return;

	switch_to_scene					( m_lobby_menu );
}

void game::switch_to_login( login_menu_status_enum status )
{
	if ( !m_network_client->has_bandwidth( ) )
		return;

	m_login_menu->set_status		( status );
	switch_to_scene					( m_login_menu );
}

// claude@NOTE: target receives this in EAX and drops project_cooker_simple's constant this argument.
// The first static's base PDB line attribution returns when the caller's LTCG convention matches.

void game::register_cooks( )
{
	static animated_model_instance_cook				s_animated_model_instance_cook;
	static game_material_manager_cook				s_material_manager_cook( false );
	static project_cooker_simple					s_simple_project_cook( engine( ).command_line_editor( ) );

	static animation_analysis_result_cook			s_animation_analysis_result_cook;
	resources::register_cook						( &s_animation_analysis_result_cook );

	static ladder_cook								s_ladder_cook;
	resources::register_cook						( &s_ladder_cook );

	static weapon_user_animations_container_cook	s_animation_container_cook;
	resources::register_cook						( &s_animation_container_cook );

	static victory_item_cook						s_victory_item_cook( m_game_world );
}

void game::on_application_activate( )
{
	::SetWindowTextA					( m_engine.get_main_window_handle( ), "Survarium\x99 v0.1 - Copyright\xA9 Vostok Games\xAE" );

	threading::mutex_raii guard			( m_application_activation );

	if ( m_input_world )
		m_input_world->on_activate		( );

	m_is_active							= true;
}

void game::on_application_deactivate( )
{
	if ( !m_input_world )
		return;

	threading::mutex_raii guard		( m_application_activation );

	m_input_world->on_deactivate		( );
	m_is_active							= false;
}

void game::on_fullscreen_alttab( bool first )
{
	VOSTOK_UNREFERENCED_PARAMETER	( first );
	m_renderer.goto_fullscreen		( m_render_output_window );
}

void game::draw_debug_window( )
{
	u32 const buffer_size			= u32( 64 * Kb );
	pvoid const buffer				= ALLOCA( buffer_size );
	strings::text_tree tree			( buffer, buffer_size, "resources stats" );

	if ( m_debug_window_type == debug_window_resources )
		resources::fill_stats		( tree.root( ) );
	else tasks::fill_stats			( tree.root( ) );

	m_debug_window->remove_all_children	( );
}

void game::create_debug_window( )
{
	m_debug_window						= m_ui_world->create_window( );
	m_debug_window->set_visible			( true );
	m_debug_window->set_position		( float2( 0.f, 120.f ) );
	m_debug_window->set_size			( float2( 2024.f, 768.f ) );
}

void game::destroy_debug_window( )
{
	m_ui_world->destroy_window			( m_debug_window );
	m_debug_window						= 0;
}

flash_factory& game::get_flash_factory( )
{
	return *m_flash_factory;
}

void game::toggle_pause( )
{
	m_is_paused						= !m_is_paused;
	if ( m_is_paused )
		pause						( );
	else
		resume						( );
}

void game::pause( )
{
	m_is_paused = true;
	m_timer.pause( );
	m_last_sound_timescale_factor	= m_sound_world.get_logic_world_user( ).get_time_scale_factor( );
	m_sound_world.get_logic_world_user( ).set_time_scale_factor( 0.f );
}

void game::resume( )
{
	m_is_paused = false;
	m_timer.resume( );
	m_sound_world.get_logic_world_user( ).set_time_scale_factor( m_last_sound_timescale_factor );
}

void game::set_network_client(
	base_network_client* const		network_client,
	pcstr							host,
	const u16						port,
	const bool						is_spectator
)
{
	m_network_client				= network_client;

	if ( !m_network_client->has_bandwidth( ) )
		return;

	if ( is_spectator )
	{
		m_network_client->connect_to_login( host, port, "", "" );
		return;
	}

	network::login_client& login_client	= m_network_client->login_client( );
	strings::copy					( login_client.m_server_host, host );
	login_client.m_server_port		= port;

	fixed_string< 128 > name		= login_client.account_name( );
	fixed_string< 128 > password	= login_client.account_password( );

	switch_to_login					( login_menu_status_disconnected );
}

void game::commit_suicide( )
{
	m_network_client->initiate_kill_current_player( );
}

void game::respawn_local_player( )
{
	m_network_client->initiate_respawn_current_player( );
}

bool game::is_loading( ) const
{
	return m_game_world.is_loading( );
}

scaleform_movie_cook::scaleform_movie_cook( flash_factory& factory )
:
	resources::translate_query_cook( resources::flash_movie_class, reuse_false, use_current_thread_id ),
	m_factory( factory )
{
	resources::register_cook		( this );
}

void scaleform_movie_cook::translate_query( resources::query_result_for_cook& parent )
{
	resources::query_resources		( &resources::create_request( parent.get_requested_path( ), resources::raw_data_class ), 1, boost::bind( &scaleform_movie_cook::on_raw_data_loaded, this, _1, &parent ), g_allocator, NULL, &parent );
}

void scaleform_movie_cook::delete_resource( resources::resource_base* resource )
{
	m_factory.destroy_movie			( static_cast< flash_movie_resource* >( resource )->movie );

	VOSTOK_DELETE_IMPL				( *g_allocator, resource );
}

void scaleform_movie_cook::on_raw_data_loaded( resources::queries_result& data, resources::query_result_for_cook* parent )
{
	resources::managed_resource_ptr raw_data	= data[0].get_managed_resource( );
	resources::pinned_ptr_const< u8 > pinned	( raw_data );

	flash_movie_resource* const resource	= VOSTOK_NEW_IMPL( ::survarium::g_allocator, flash_movie_resource );

	resource->movie					= m_factory.build_movie( (void*)pinned.c_ptr( ), pinned.size( ), parent->reusable_request_name( ).c_str( ) );

	parent->set_unmanaged_resource	( resource, resources::nocache_memory, 0x110 );
	parent->finish_query			( result_success );
}

void game::activate_main_menu( )
{
	m_active_scene->show_ui			( false );
	m_game_options.activate			( m_active_scene );
}

void game::deactivate_main_menu( )
{
	m_game_options.deactivate		( );
	m_active_scene->show_ui			( true );
}

void game::discard_current_match( )
{
	m_network_client->close_current_match( true );
}


} // namespace survarium
