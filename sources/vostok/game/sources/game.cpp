////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "game.h"

#include <vostok/console_command.h>	// cc_float (max_angular_velocity_command)
#include <vostok/console_command_processor.h>	// console_commands::save (cfg_save_*)
#include <vostok/memory_extensions.h>	// memory::g_mt_allocator (cfg_save_*)
#include <vostok/render/world.h>	// game_renderer() (ctor init)
#include <vostok/render/facade/game_renderer.h>	// renderer().ui() (draw_debug_window)
#include <vostok/input/world.h>	// m_input_world->on_activate/on_deactivate
#include <vostok/ui/world.h>	// m_ui_world->create_window (debug window)
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
#include "network_client.h"	// NEW( network_client ) (create_network_client)
#include "stats.h"	// DELETE( m_stats ) (~game)
#include "stats_graph.h"	// DELETE( m_fps_graph ) (~game)
#include "key_binder.h"	// DELETE( m_key_binder ) (~game)
#include "chat_handler.h"	// DELETE( m_chat_handler ) (~game)
#include "main_menu.h"	// main_menu derives base_game_scene (switch_to_main_menu)
#include "lobby_menu.h"	// lobby_menu derives base_game_scene (switch_to_lobby)
#include "login_menu.h"	// login_menu derives base_game_scene + set_status (switch_to_login)
#include "animated_model_instance_cook.h"	// register_cooks function-statics
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

// claude@NOTE: PARKED - 's_draw_snd_stats' (sound-debug cc_bool): its command-name
// string + command_type are not recoverable from the init asm (the name string lives
// in the data section, not the init function). A guessed name would fabricate
// data-section bytes. Recover once the data-section string is read. Its backing value
// bool s_draw_snd_stats_value is referenced by update_stats's PARKED sound-debug tail
// (see the note there); restore the static alongside that tail.

static bool s_draw_stats_value = true;
static cc_bool s_draw_stats( "draw_stats", s_draw_stats_value, true, command_type_user_specific );

// claude@NOTE: PARKED - 's_show_profiler_command' (profiler cc_bool): command-name
// string + command_type unrecoverable (data-section string), same as s_draw_snd_stats.

// claude@NOTE: cfg_save_user is GLOBAL (?cfg_save_user@@YAXXZ); target body is
//   console_commands::save( "user.cfg", command_type_user_specific, s_engine->allocator( ) );
// where s_engine is a file-static engine pointer (the engine virtual at vtable+0x4C
// yields the allocator). That static is compiler-generated and NOT in our source, so
// the allocator arg cannot be reproduced; we approximate with g_mt_allocator. The path
// + command_type ARE matched. Restore the engine-allocator form once s_engine is recovered.
// sushi@TODO: recover the s_engine file-static (+ its write site) to lift cfg_save_user
// AND game::~game line 279 to byte-exact - see docs/binary_matching/review_todos.md.
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

namespace survarium {

// TU static 's_net_login_client' (compiler-generated; a matcher recovers its type
// and initializer from the init asm).
/*
// STATE[STUB]
void `dynamic initializer for 's_net_login_client''( )
{
	// FUNCTION BODY[0x7d79c0]
	// <0x7d79c0>|0x000|      :'54'	{
	// ******
}
*/

// TU static 's_is_spectator' (compiler-generated; a matcher recovers its type
// and initializer from the init asm).
/*
// STATE[STUB]
void `dynamic initializer for 's_is_spectator''( )
{
	// FUNCTION BODY[0x7d79e0]
	// <0x7d79e0>|0x000|      :'55'	{
	// ******
}
*/

// TU-local (canonical headers/max_angular_velocity_command.h; owner mapping
// in temp/triage_log.md) - the type of the s_max_angular_velocity_command static
class max_angular_velocity_command : public console_commands::cc_float {
public:
					max_angular_velocity_command	(
						pcstr			name,
						const float		min,
						const float		max,
						bool			serializable,
						const console_commands::command_type	arg_4 /* console_commands::command_type command_type */,
						const console_commands::execution_filter	arg_5 /* console_commands::execution_filter execution_filter */
					);

	inline	void	set_engine						( engine_user::engine& arg_0 ) { /* no source */ }

	virtual	void	execute							( pcstr arg_0 ) override { /* no source */ }

	virtual			~max_angular_velocity_command	( ) { /* no source */ }

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
	console_commands::cc_float( name, m_value, min, max, serializable, arg_4, arg_5 ),
	m_engine( NULL ),
	m_value( 720.f )
{
}

// TU static 's_max_angular_velocity_command' (compiler-generated; a matcher recovers its type
// and initializer from the init asm).
/*
// STATE[STUB]
void `dynamic initializer for 's_max_angular_velocity_command''( )
{
	// FUNCTION BODY[0x7d7af0]
	// <0x7d7af0>|0x000|      :'114'	{
	// ******
}
*/

// TU static 's_max_angular_velocity_command' (compiler-generated; a matcher recovers its type
// and initializer from the init asm).
/*
// STATE[STUB]
void `dynamic atexit destructor for 's_max_angular_velocity_command''( )
{
	// FUNCTION BODY[0x7ef9f0]
	// <0x7d7d70>|0x000|      :'144'	{
	// ******
}
*/

// STATE[STUB]
 game::game(
	engine_user::engine&	engine,
	render::world&			render_world,
	sound::world&			sound,
	network::world&			network
) :
	// ref/value-member sources per the legacy ctor (temp/game_legacy/game.cpp:170);
	// m_scheduler's allocator is a buildability placeholder - a matcher confirms
	m_engine( engine ),
	m_render_world( render_world ),
	m_sound_world( sound ),
	m_network_world( network ),
	m_renderer( render_world.game_renderer( ) ),
	m_game_world( *this ),
	m_scheduler( g_allocator ),
	m_game_options( *this )
{
	// STATICS
	// static player_parameters_modifyer_cook s_player_parameters_cook = <0x4c2bb70>;
	// static console_commands::cc_string s_current_render_configuration_cc = <0x4c2bd30>;
	// static profile_skin_visual_cook 	s_profile_skin_visual_cook = <0x4c277d0>;
	// static console_commands::cc_delegate pause_game_command = <0x4c2bc70>;
	// static console_commands::cc_delegate commit_suicide_cc = <0x4c2bbb0>;
	// static console_commands::cc_delegate s_reload_shaders = <0x4c2bd78>;
	// static console_commands::cc_delegate s_reload_modified_textures = <0x4c2bcd0>;
	// static fixed_string< 512 > 		s_current_render_configuration = <0x4c26a20>;
	// static inventory_cook 			s_inventory_cook = <0x4c2bb90>;
	// static scaleform_movie_cook 		s_scaleform_movie_cook = <0x4c27838>;
	// static items_dictionary_cook 	s_items_dictionary_cook = <0x4c2bb50>;
	// static console_commands::cc_delegate s_build_lpv_geometry = <0x4c2bdd8>;
	// static console_commands::cc_delegate resume_game_command = <0x4c2bc10>;
	// ******

	// FUNCTION BODY[0x5e7a70]: 58
	// <0x5e7a70>|0x000|+0x1e8:'206'	{
	// <0x5e7c58>|0x1e8|+0x05e:'207'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5e7cb6>|0x246|+0x097:'214'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5e7d4d>|0x2dd|+0x075:'221'
	// <0>
	// <0x5e7dc2>|0x352|+0x02e:'223'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x5e7df0>|0x380|+0x039:'231'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5e7e29>|0x3b9|+0x075:'238'
	// <0x5e7e9e>|0x42e|+0x092:'239'
	// <0x5e7f30>|0x4c0|+0x092:'240'
	// <0x5e7fc2>|0x552|+0x08f:'241'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5e8051>|0x5e1|+0x02e:'247'
	// <0x5e807f>|0x60f|+0x01c:'248'
	// <0>
	// <0x5e809b>|0x62b|+0x031:'250'
	// <0>
	// <0x5e80cc>|0x65c|+0x02a:'252'
	// <0x5e80f6>|0x686|+0x02a:'253'
	// <0x5e8120>|0x6b0|+0x02a:'254'
	// <0x5e814a>|0x6da|+0x02c:'255'
	// <0x5e8176>|0x706|+0x026:'256'
	// <0>
	// <0x5e819c>|0x72c|+0x007:'258'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5e81a3>|0x733|+0x02a:'263'
	// <0x5e81cd>|0x75d|-0x00b:'263'
	// <0>
	// <0x5e81c2>|0x752|+0x011:'265'
	// <0x5e81d3>|0x763|      :'265'	}
	// ******
}

void game::execute_scaleform_command( scaleform_render_command command )
{
	m_renderer.execute_scaleform_command	( command );
}

// STATE[STUB]
// claude@NOTE: target body is
//   m_renderer.scene( ).build_lpv_geometry( m_game_world.render_scene( ) );
// but render::scene_renderer::build_lpv_geometry( base_scene_ptr const& ) is NOT
// declared in our sources/vostok/render/facade/scene_renderer.h (it IS in the target
// structure). That header is the render cluster's; cannot add the decl here. Keep
// the stub buildable; restore the real call once render declares build_lpv_geometry.
void game::build_lpv_geometry( )
{
	// FUNCTION BODY[0x5e6750]: 1
	// <0x5e6750>|0x000|+0x016:'274'
	// ******
}

 game::~game( )
{
	// claude@NOTE: line 279 calls console_commands::save( "user.cfg", ..., s_engine->allocator() )
	// where s_engine is the same unrecoverable file-static as cfg_save_user (see its note +
	// review_todos.md); approximated with g_mt_allocator. Remaining 11 statements are exact.
	vostok::console_commands::save	( "user.cfg", command_type_user_specific, memory::g_mt_allocator );

	DELETE							( m_network_client );
	DELETE							( m_main_menu );
	DELETE							( m_lobby_menu );
	DELETE							( m_login_menu );
	DELETE							( m_console );
	DELETE							( m_stats );
	DELETE							( m_fps_graph );

	DELETE							( m_flash_factory );
	DELETE							( m_key_binder );
	DELETE							( m_chat_handler );

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

// STATE[STUB]
void game::on_configs_loaded( resources::queries_result& result )
{
	// LOCALS
	// variant< 32 > 					window_data
	// variant< 32 > const*[1] 			data
	// render::output_window_configuration window_configuration
	// variant< 32 > 					output_window_data
	// resources::request[1] 			requests
	// math::uint2 						resolution_xy
	// console_commands::console_command* fullscreen_command
	// ******

	// CALL SITE INFO
	// <0x5e7814> -> HWND__* < unknown >() const
	// <0x5e7839> -> bool < unknown >()
	// ******

	// FUNCTION BODY[0x5e7760]: 47
	// <0x5e7771>|0x011|+0x02d:'330'
	// <0>
	// <0x5e779e>|0x03e|+0x021:'332'
	// <0>
	// <0x5e77bf>|0x05f|+0x01f:'334'
	// <0x5e77de>|0x07e|+0x01a:'335'
	// <0>
	// <0x5e77f8>|0x098|+0x00c:'337'
	// <0>
	// <1>
	// <0x5e7804>|0x0a4|+0x012:'340'
	// <0>
	// <1>
	// <0x5e7816>|0x0b6|+0x009:'343'
	// <0>
	// <0x5e781f>|0x0bf|+0x020:'345'
	// <0>
	// <1>
	// <0x5e783f>|0x0df|+0x018:'348'
	// <0>
	// <1>
	// <2>
	// <0x5e7857>|0x0f7|+0x00d:'352'
	// <0>
	// <1>
	// <0x5e7864>|0x104|+0x008:'355'
	// <0x5e786c>|0x10c|+0x00e:'356'
	// <0x5e787a>|0x11a|+0x00d:'357'
	// <0>
	// <1>
	// <0x5e7887>|0x127|+0x00e:'360'
	// <0x5e7895>|0x135|+0x01a:'361'
	// <0>
	// <1>
	// <0x5e78af>|0x14f|+0x012:'364'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <0x5e78c1>|0x161|+0x0b3:'376'
	// ******
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

// claude@NOTE: 28-statement central init. Structure FULLY decoded from 0x5e7200; two
// classes of blocker stop a clean body, so it stays a STUB (bodying it with the parts
// below would emit the high-leverage `static global_input_handler g_input_handler(*this)`
// that unblocks the global_input_handler ctor 82%->100%, but the residual pieces are
// unrecoverable and risk fabrication):
//   399 m_items_dictionary = static_cast_resource_ptr<items_dictionary_ptr>( data[0].get_unmanaged_resource() );
//   403 static global_input_handler g_input_handler( *this );    // <- the leverage point
//   404 m_input_world->add_handler( g_input_handler );
//   406 register_cooks( );
//   408 m_game_options.initialize( );
//   410 m_chat_handler->initialize( data[1].get_unmanaged_resource() );
//   412 m_text_wnd = m_ui_world->create_window( );
//   413 m_text_wnd->set_position( float2( 300.f, 0.f ) );
//   414 m_text_wnd->set_size( float2( 600.f, 600.f ) );
//   415 m_text_wnd->set_visible( true );
//   417 m_console = m_engine.create_game_console( ui_world( ), input_world( ) );
//   418 m_stats = NEW(stats)( *m_ui_world ); m_stats->create( );
//   422 m_fps_graph = NEW(stats_graph)( <t>, math::infinity, <fps>, 60.f, 0xFF00FF00 );  // <t>,<fps> = UNRESOLVED float pool consts (clear_value/default_fps)
//   424 m_main_menu = NEW(main_menu)( *this ); m_main_menu->query_resources( );
//   426 create_debug_window( );
//   428 m_viewport = rectangle<float2>( float2(0,0), float2(<v>,<v>) );   // <v> = UNRESOLVED (clear_value)
//   433 enable( m_enabled );
//   435/438 if ( m_is_active ) { on_application_deactivate( ); m_is_active = false; }
//   446-471 if ( s_net_login_client.is_set_as_string( &client_str ) && strchr(client_str,':') )
//             create_and_assign_network_client( client_str, s_is_spectator.is_set( ) );
//           else create_and_assign_network_client( "188.93.23.27:5100", false );
// BLOCKERS: (1) the m_fps_graph/m_viewport float pool constants resolve only to delinker
// symbol names (clear_value/default_fps), not literal values; (2) the s_net_login_client /
// s_is_spectator command_line::key statics' ctor strings live in the data section, not the
// init asm, so declaring them (and their own dynamic-initializer STUBs) would be a guess.
// Finish once those data-section strings + float literals are recovered.
// STATE[STUB]
void game::on_base_resources_created( resources::queries_result& data )
{
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
		NEW( class network_client )	( *this, is_spectator ),
		host.c_str					( ),
		port,
		is_spectator
	);
}

void game::create_lobby_menu( )
{
	m_lobby_menu					= NEW( class lobby_menu )( *this );
}

void game::create_login_menu( )
{
	m_login_menu					= NEW( class login_menu )( *this );
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

void game::on_config_loaded( resources::queries_result& data, bool create_renderer )
{
	if ( !data.is_successful( ) )
	{
		LOG_ERROR					( "config file loading FAILED" );
		return;
	}

	load_cc_script					( data[0].get_managed_resource( ), create_renderer );
}

// claude@NOTE: 9-statement body, two halves. First half (608-615) is clean + recoverable:
//   if ( cfg && cfg.c_ptr( ) ) {
//     resources::pinned_ptr_const<u8> pinned_data( cfg );
//     memory::reader F( pinned_data.c_ptr( ), pinned_data.size( ) );
//     console_commands::load( F, console_commands::execution_filter_general );
//   }
// Second half (618-629) is BLOCKED: `if ( create_renderer )` builds a "renderer" creation
// request whose user_data is the render ENGINE world read straight off the renderer facade
// (asm: mov ecx,[m_renderer]; mov esi,[ecx+4] == render::game::renderer::m_render_engine_world)
// then query_create_resources( ..., on_renderer_created, ... ). m_render_engine_world is a
// PRIVATE member of render::game::renderer with no accessor and game is not a friend, so the
// exact `[m_renderer+4]` load cannot be produced from game.cpp (m_render_world.engine_world()
// reads a different slot). Writing only the first half would give the wrong (truncated)
// structure, so the whole function stays a STUB. Lifts once the render cluster exposes the
// renderer's engine world (an accessor or friend).
// STATE[STUB]
void game::load_cc_script( resources::managed_resource_ptr cfg, bool create_renderer )
{
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
		m_ui_world->default_font( )
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

	// claude@NOTE: PARKED tail (6 stmts, target lines 845-862) - the s_draw_snd_stats
	// sound-debug block:
	//   if ( s_draw_snd_stats_value && m_game_world.is_active( ) ) {
	//     if ( !m_sound_stats )
	//       m_sound_stats = NEW( sound::sound_debug_stats )( g_allocator,
	//         m_sound_world.get_logic_world_user( ), m_game_world.get_sound_scene( ), *m_ui_world );
	//     if ( m_sound_stats->is_stats_available( ) ) {
	//       sound::sound_debug_stats::set_debug_draw_mode( sound::sound_debug_stats::overall );
	//       m_sound_stats->draw( m_active_scene->render_scene( ), m_active_scene->render_scene_view( ) );
	//     }
	//   }
	// SOUND SIDE RESOLVED: sound_debug_stats.cpp is built for Master Gold and its
	// ctor/create_statistic/draw_overall_stats/draw calls link. The remaining
	// blocker is s_draw_snd_stats_value: its backing cc_bool's command-name
	// string and command_type have not been recovered from target data, so the
	// static cannot be defined faithfully. Restore this tail together with that
	// static once its data-section evidence is available.
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
		m_network_client->unload	( );
		DELETE						( m_network_client );
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

	m_game_world.load				( project_resource_name, requests_begin, requests_end, user_datas_begin, callback );

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

// claude@NOTE: cook inventory + order verified EXACT against the target disasm
// (0x5e5940): the 7 statics and the 3 explicit register_cook calls all appear in
// target order - this is NOT a content/order divergence. The residual is all
// non-steerable codegen: (1) `mov esi,eax` this-in-eax convention - the fn is
// reached only via the member-fn-ptr anchor in anchor_game_world.cpp (address-take,
// not a direct call); (2) animated_model_instance_cook ctor inline-vs-call (base
// inlines its body, target out-of-lines it - cross-module knob); (3) the free
// resources::register_cook(cook) is inlined into resources_manager::register_cook
// in the target but CALLed in our base x3 (cross-module inline knob); (4) the
// s_victory_item_cook ctor is this-const-folded in the target (see
// victory_item_cooker.cpp note). None steerable from this TU.
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
	if ( m_input_world )
	{
		threading::mutex_raii guard		( m_application_activation );

		m_input_world->on_deactivate	( );
		m_is_active						= false;
	}
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
	else
		tasks::fill_stats			( tree.root( ) );

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

// STATE[STUB]
// claude@NOTE: BLOCKED on cross-module timing::timer header. Target body:
//   m_is_paused = !m_is_paused;
//   if ( m_is_paused )  pause( );
//   else {  m_timer.resume( );  m_sound_world.get_logic_world_user().set_time_scale_factor( m_last_sound_timescale_factor );  }
// but timing::timer::resume()/pause()/is_paused() are NOT declared in our
// sources/vostok/timing_timer.h (they ARE inline in the target timing/timer.h).
// That header is the core/timing module's; cannot add them here. Keep stub buildable.
void game::toggle_pause( )
{
	// CALL SITE INFO
	// <0x5e5d90> -> sound::world_user& < unknown >() const
	// ******

	// FUNCTION BODY[0x5e5d50]: 5
	// <0x5e5d50>|0x000|+0x010:'1185'
	// <0x5e5d60>|0x010|+0x004:'1186'
	// <0x5e5d64>|0x014|+0x008:'1187'
	// <0>
	// <0x5e5d6c>|0x01c|+0x02e:'1189'
	// ******
}

// STATE[STUB]
// claude@NOTE: BLOCKED on cross-module timing::timer header (see toggle_pause). Target:
//   m_is_paused = true;  m_timer.pause( );
//   m_last_sound_timescale_factor = m_sound_world.get_logic_world_user().get_time_scale_factor( );
//   m_sound_world.get_logic_world_user().set_time_scale_factor( 0.f );
// timing::timer::pause() not in our sources/vostok/timing_timer.h. Keep stub buildable.
void game::pause( )
{
	// CALL SITE INFO
	// <0x5e5d1c> -> sound::world_user& < unknown >() const
	// <0x5e5d3c> -> sound::world_user& < unknown >() const
	// ******

	// FUNCTION BODY[0x5e5cc0]: 4
	// <0>
	// <0x5e5cc7>|0x007|+0x04a:'1195'
	// <0x5e5d11>|0x051|+0x01a:'1196'
	// <0x5e5d2b>|0x06b|+0x01c:'1197'
	// ******
}

// STATE[STUB]
// claude@NOTE: BLOCKED on cross-module timing::timer header (see toggle_pause). Target:
//   m_is_paused = false;  m_timer.resume( );
//   m_sound_world.get_logic_world_user().set_time_scale_factor( m_last_sound_timescale_factor );
// timing::timer::resume() not in our sources/vostok/timing_timer.h. Keep stub buildable.
void game::resume( )
{
	// CALL SITE INFO
	// <0x5e5928> -> sound::world_user& < unknown >() const
	// ******

	// FUNCTION BODY[0x5e5900]: 3
	// <0>
	// <0x5e5904>|0x004|+0x00f:'1203'
	// <0x5e5913>|0x013|+0x020:'1204'
	// ******
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

	flash_movie_resource* const resource	= NEW( flash_movie_resource );

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
