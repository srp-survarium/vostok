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
#include <vostok/resources.h>	// query_resources / request (query_base_resources)

// SetWindowTextA (USER32) is stripped from os_include.h by NOUSER; declare the import
// directly for on_application_activate's title-bar update.
extern "C" __declspec( dllimport ) int __stdcall SetWindowTextA( void* hWnd, char const* lpString );

#include "scaleform_movie_cook.h"	// its out-of-line bodies live here per the PDB
#include "base_game_scene.h"	// m_active_scene->on_activate/on_deactivate (switch_to_scene)
#include "base_network_client.h"	// m_network_client virtuals (commit_suicide etc.)
#include "main_menu.h"	// main_menu derives base_game_scene (switch_to_main_menu)
#include "lobby_menu.h"	// lobby_menu derives base_game_scene (switch_to_lobby)
#include "login_menu.h"	// login_menu derives base_game_scene + set_status (switch_to_login)

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

// TU static 's_draw_snd_stats' (compiler-generated; a matcher recovers its type
// and initializer from the init asm).
/*
// STATE[STUB]
void `dynamic initializer for 's_draw_snd_stats''( )
{
	// FUNCTION BODY[0x7d7a00]
	// <0x7d7a00>|0x000|      :'58'	{
	// ******
}
*/

// TU static 's_draw_stats' (compiler-generated; a matcher recovers its type
// and initializer from the init asm).
/*
// STATE[STUB]
void `dynamic initializer for 's_draw_stats''( )
{
	// FUNCTION BODY[0x7d7a50]
	// <0x7d7a50>|0x000|      :'70'	{
	// ******
}
*/

// TU static 's_show_profiler_command' (compiler-generated; a matcher recovers its type
// and initializer from the init asm).
/*
// STATE[STUB]
void `dynamic initializer for 's_show_profiler_command''( )
{
	// FUNCTION BODY[0x7d7aa0]
	// <0x7d7aa0>|0x000|      :'74'	{
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

// STATE[STUB]
 max_angular_velocity_command::max_angular_velocity_command(
	pcstr			name,
	const float		min,
	const float		max,
	bool			serializable,
	const console_commands::command_type	arg_4 /* console_commands::command_type command_type */,
	const console_commands::execution_filter	arg_5 /* console_commands::execution_filter execution_filter */
) :
	// the cc_float base stores this->m_value by ref; the remaining base args
	// come straight from the params - a matcher confirms when a TU instantiates
	console_commands::cc_float( name, m_value, min, max, serializable, arg_4, arg_5 )
{
	// FUNCTION BODY[0x8d940]: 0
	// <0x8d940>|0x000|+0x050:'87'	{
	// <0x8d990>|0x050|      :'88'	}
	// ******
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

// STATE[STUB]
void cfg_save_user( )
{
	vostok::console_commands::save( NULL, vostok::console_commands::command_type_user_specific, memory::g_mt_allocator );

	// FUNCTION BODY[0x5e5e30]: 1
	// <0x5e5e30>|0x000|+0x01a:'118'
	// ******
}

// STATE[STUB]
void cfg_save_system( )
{
	vostok::console_commands::save( NULL, vostok::console_commands::command_type_engine_internal, memory::g_mt_allocator );

	// FUNCTION BODY[0x5e5e20]: 1
	// <0x5e5e20>|0x000|+0x000:'122'	{
	// <0>
	// <0x5e5e20>|0x000|      :'124'	}
	// ******
}

// TU static 'cfg_save_system_cc' (compiler-generated; a matcher recovers its type
// and initializer from the init asm).
/*
// STATE[STUB]
void `dynamic initializer for 'cfg_save_system_cc''( )
{
	// FUNCTION BODY[0x7d7b10]
	// <0x7d7b10>|0x000|      :'126'	{
	// ******
}
*/

// TU static 'cfg_save_user_cc' (compiler-generated; a matcher recovers its type
// and initializer from the init asm).
/*
// STATE[STUB]
void `dynamic initializer for 'cfg_save_user_cc''( )
{
	// FUNCTION BODY[0x7d7be0]
	// <0x7d7be0>|0x000|      :'127'	{
	// ******
}
*/

// TU static 's_max_particles' (compiler-generated; a matcher recovers its type
// and initializer from the init asm).
/*
// STATE[STUB]
void `dynamic initializer for 's_max_particles''( )
{
	// FUNCTION BODY[0x7d7cb0]
	// <0x7d7cb0>|0x000|      :'138'	{
	// ******
}
*/

// TU static 's_particle_lod' (compiler-generated; a matcher recovers its type
// and initializer from the init asm).
/*
// STATE[STUB]
void `dynamic initializer for 's_particle_lod''( )
{
	// FUNCTION BODY[0x7d7d10]
	// <0x7d7d10>|0x000|      :'141'	{
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

// STATE[STUB]
// claude@NOTE: target body is m_renderer.execute_scaleform_command( command );
// but render::game::renderer::execute_scaleform_command is NOT declared in our
// sources/vostok/render/facade/game_renderer.h (it IS in the target structure).
// That header is the render cluster's; cannot add the decl here. Keep the stub
// buildable; restore the real call once render declares execute_scaleform_command.
void game::execute_scaleform_command( scaleform_render_command command )
{
	// FUNCTION BODY[0x5e5e00]: 1
	// <0x5e5e00>|0x000|+0x011:'269'
	// ******
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

// STATE[STUB]
 game::~game( )
{
	// CALL SITE INFO
	// <0x5e6415> -> void* < unknown >( u32 )
	// <0x5e6455> -> void* < unknown >( u32 )
	// <0x5e6495> -> void* < unknown >( u32 )
	// <0x5e64d5> -> void* < unknown >( u32 )
	// <0x5e650f> -> void* < unknown >( u32 )
	// <0x5e65d1> -> void* < unknown >( u32 )
	// ******

	// FUNCTION BODY[0x5e63c0]: 21
	// <0x5e63c4>|0x004|+0x029:'279'
	// <0>
	// <1>
	// <0x5e63ed>|0x02d|+0x041:'282'
	// <0>
	// <0x5e642e>|0x06e|+0x040:'284'
	// <0x5e646e>|0x0ae|+0x040:'285'
	// <0x5e64ae>|0x0ee|+0x040:'286'
	// <0x5e64ee>|0x12e|+0x037:'287'
	// <0x5e6525>|0x165|+0x02e:'288'
	// <0x5e6553>|0x193|+0x026:'289'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5e6579>|0x1b9|+0x013:'295'
	// <0x5e658c>|0x1cc|+0x01e:'296'
	// <0x5e65aa>|0x1ea|+0x040:'297'
	// <0>
	// <0x5e65ea>|0x22a|+0x007:'299'
	// ******
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

// STATE[STUB]
void game::on_base_resources_created( resources::queries_result& data )
{
	// LOCALS
	// fixed_string< 512 > 				client_str
	// fixed_string< 512 > 				host
	// ******

	// STATICS
	// static global_input_handler 		g_input_handler = <0x4c26c38>;
	// ******

	// CALL SITE INFO
	// <0x5e72fa> -> void < unknown >( input::handler& )
	// <0x5e7361> -> ui::window* < unknown >()
	// <0x5e738b> -> void < unknown >( float2 const& )
	// <0x5e73b1> -> void < unknown >( float2 const& )
	// <0x5e73c0> -> void < unknown >( bool )
	// <0x5e73d1> -> input::world& < unknown >()
	// <0x5e73db> -> ui::world& < unknown >()
	// <0x5e73e3> -> engine::console* < unknown >( ui::world&, input::world& )
	// <0x5e7500> -> void < unknown >( bool )
	// <0x5e7517> -> void < unknown >()
	// ******

	// FUNCTION BODY[0x5e7200]: 72
	// <0x5e7200>|0x000|+0x00c:'398'	{
	// <0x5e720c>|0x00c|+0x0aa:'399'
	// <0>
	// <1>
	// <2>
	// <0x5e72b6>|0x0b6|+0x034:'403'
	// <0x5e72ea>|0x0ea|+0x012:'404'
	// <0>
	// <0x5e72fc>|0x0fc|+0x007:'406'
	// <0>
	// <0x5e7303>|0x103|+0x00c:'408'
	// <0>
	// <0x5e730f>|0x10f|+0x047:'410'
	// <0>
	// <0x5e7356>|0x156|+0x00d:'412'
	// <0x5e7363>|0x163|+0x02a:'413'
	// <0x5e738d>|0x18d|+0x026:'414'
	// <0x5e73b3>|0x1b3|+0x00f:'415'
	// <0>
	// <0x5e73c2>|0x1c2|+0x023:'417'
	// <0x5e73e5>|0x1e5|+0x03d:'418'
	// <0>
	// <1>
	// <2>
	// <0x5e7422>|0x222|+0x063:'422'
	// <0>
	// <0x5e7485>|0x285|+0x047:'424'
	// <0>
	// <0x5e74cc>|0x2cc|+0x007:'426'
	// <0>
	// <0x5e74d3>|0x2d3|+0x003:'428'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5e74d6>|0x2d6|+0x02c:'433'
	// <0>
	// <0x5e7502>|0x302|+0x008:'435'
	// <0>
	// <1>
	// <0x5e750a>|0x30a|+0x00f:'438'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x5e7519>|0x319|+0x015:'446'
	// <0x5e752e>|0x32e|+0x01e:'447'
	// <0>
	// <0x5e754c>|0x34c|+0x010:'449'
	// <0x5e755c>|0x35c|+0x037:'450'
	// <0x5e7593>|0x393|+0x009:'451'
	// <0>
	// <0x5e759c>|0x39c|+0x044:'453'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5e75e0>|0x3e0|+0x012:'460'
	// <0x5e75f2>|0x3f2|-0x009:'460'
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
	// <0x5e75e9>|0x3e9|+0x049:'471'
	// <0x5e7632>|0x432|      :'471'	}
	// ******
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
		case scene_ready_login:
		{
			m_login_scene_ready		= true;
			break;
		}
		case scene_ready_lobby:
		{
			m_lobby_scene_ready		= true;
			break;
		}
	}

	if ( m_lobby_scene_ready && m_login_scene_ready )
		create_network_client		( false );
}

// STATE[STUB]
void game::create_network_client( const bool is_spectator )
{
	// LOCALS
	// fixed_string< 512 > 				host
	// ******

	// FUNCTION BODY[0x5e6f70]: 10
	// <0x5e6f81>|0x011|+0x00a:'511'
	// <0x5e6f8b>|0x01b|+0x037:'512'
	// <0x5e6fc2>|0x052|+0x009:'513'
	// <0>
	// <0x5e6fcb>|0x05b|+0x072:'515'
	// <0x5e703d>|0x0cd|+0x00f:'516'
	// <0x5e704c>|0x0dc|+0x042:'517'
	// <0>
	// <1>
	// <2>
	// ******
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

// STATE[STUB]
void game::enable( bool value )
{
	// CALL SITE INFO
	// <0x5e569f> -> void < unknown >()
	// <0x5e56a7> -> void < unknown >()
	// ******

	// FUNCTION BODY[0x5e5680]: 20
	// <0x5e5680>|0x000|+0x000:'567'	{
	// <0>
	// <1>
	// <0x5e5680>|0x000|+0x010:'570'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5e5690>|0x010|+0x00f:'575'
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
	// <0x5e569f>|0x01f|+0x005:'586'
	// <0x5e56a4>|0x024|-0x003:'586'
	// <0>
	// <0x5e56a1>|0x021|+0x008:'588'
	// <0x5e56a9>|0x029|      :'588'	}
	// ******
}

void game::on_renderer_created( resources::queries_result& data )
{
}

// STATE[STUB]
void game::on_config_loaded( resources::queries_result& data, bool create_renderer )
{
	// FUNCTION BODY[0x5e6b00]: 7
	// <0x5e6b00>|0x000|+0x009:'596'	{
	// <0x5e6b09>|0x009|+0x019:'597'
	// <0>
	// <0x5e6b22>|0x022|+0x0c2:'599'
	// <0>
	// <1>
	// <2>
	// <0x5e6be4>|0x0e4|-0x008:'603'
	// <0x5e6bdc>|0x0dc|+0x023:'604'
	// <0x5e6bff>|0x0ff|      :'604'	}
	// ******
}

// STATE[STUB]
void game::load_cc_script( resources::managed_resource_ptr cfg, bool create_renderer )
{
	// LOCALS
	// resources::pinned_ptr_const< u8 > pinned_data
	// memory::reader 					F
	// mutable_buffer 					creation_buffer
	// variant< 32 > 					ud
	// ******

	// FUNCTION BODY[0x5e6980]: 22
	// <0x5e6986>|0x006|+0x013:'608'
	// <0>
	// <0x5e6999>|0x019|+0x035:'610'
	// <0x5e69ce>|0x04e|+0x00c:'611'
	// <0x5e69da>|0x05a|+0x010:'612'
	// <0x5e69ea>|0x06a|+0x008:'613'
	// <0>
	// <0x5e69f2>|0x072|+0x00a:'615'
	// <0>
	// <1>
	// <0x5e69fc>|0x07c|+0x01f:'618'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x5e6a1b>|0x09b|+0x0be:'628'
	// <0x5e6ad9>|0x159|+0x012:'629'
	// ******
}

// STATE[STUB]
void game::load_config_query( pcstr cfg_name, bool create_renderer )
{
	// FUNCTION BODY[0x5e6c60]: 9
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x5e6c69>|0x009|+0x099:'642'
	// ******
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

// STATE[STUB]
void game::tick( const u32 current_frame_id )
{
	// CALL SITE INFO
	// <0x5e829d> -> void < unknown >( u32 )
	// <0x5e82ba> -> void < unknown >( const u32, const u32, const bool )
	// <0x5e8302> -> void < unknown >( u32, const bool )
	// <0x5e830f> -> void < unknown >()
	// <0x5e831c> -> void < unknown >()
	// <0x5e8329> -> void < unknown >()
	// <0x5e8348> -> render::ui::renderer& < unknown >()
	// <0x5e8354> -> void < unknown >( render::ui::renderer&, render::base_scene_view_ptr const& )
	// <0x5e8360> -> void < unknown >()
	// <0x5e836a> -> bool < unknown >() const
	// <0x5e8382> -> void < unknown >( render::base_scene_view_ptr const& )
	// <0x5e839a> -> bool < unknown >() const
	// <0x5e83ae> -> ui::world& < unknown >()
	// <0x5e83f3> -> ui::font const* < unknown >()
	// <0x5e845b> -> void < unknown >( u32, const bool )
	// ******

	// FUNCTION BODY[0x5e81e0]: 117
	// <0x5e81e0>|0x000|+0x00e:'691'	{
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5e81ee>|0x00e|+0x02b:'697'
	// <0x5e8219>|0x039|+0x008:'698'
	// <0>
	// <1>
	// <0x5e8221>|0x041|+0x031:'701'
	// <0>
	// <0x5e8252>|0x072|+0x009:'703'
	// <0>
	// <0x5e825b>|0x07b|+0x019:'705'
	// <0>
	// <0x5e8274>|0x094|+0x1c9:'707'
	// <0>
	// <0x5e843d>|0x25d|+0x00a:'709'
	// <0x5e8447>|0x267|+0x016:'710'
	// <0>
	// <0x5e845d>|0x27d|-0x1d1:'712'
	// <0>
	// <1>
	// <2>
	// <0x5e828c>|0x0ac|+0x013:'716'
	// <0x5e829f>|0x0bf|+0x01d:'717'
	// <0>
	// <0x5e82bc>|0x0dc|+0x009:'719'
	// <0x5e82c5>|0x0e5|+0x00d:'720'
	// <0>
	// <0x5e82d2>|0x0f2|+0x00c:'722'
	// <0x5e82de>|0x0fe|+0x006:'723'
	// <0>
	// <0x5e82e4>|0x104|+0x00a:'725'
	// <0x5e82ee>|0x10e|+0x016:'726'
	// <0>
	// <0x5e8304>|0x124|+0x00d:'728'
	// <0>
	// <0x5e8311>|0x131|+0x00d:'730'
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
	// <11>
	// <12>
	// <13>
	// <14>
	// <15>
	// <16>
	// <17>
	// <18>
	// <19>
	// <20>
	// <21>
	// <22>
	// <23>
	// <24>
	// <25>
	// <0x5e831e>|0x13e|+0x00d:'757'
	// <0x5e832b>|0x14b|+0x02b:'758'
	// <0>
	// <0x5e8356>|0x176|+0x00c:'760'
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
	// <11>
	// <12>
	// <13>
	// <0x5e8362>|0x182|+0x00e:'775'
	// <0x5e8370>|0x190|+0x014:'776'
	// <0>
	// <0x5e8384>|0x1a4|+0x005:'778'
	// <0>
	// <0x5e8389>|0x1a9|+0x017:'780'
	// <0x5e83a0>|0x1c0|+0x007:'781'
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
	// <11>
	// <12>
	// <0x5e83a7>|0x1c7|+0x081:'795'
	// <0>
	// <0x5e8428>|0x248|+0x00c:'797'
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
	// <0x5e8434>|0x254|+0x035:'809'
	// <0x5e8469>|0x289|      :'809'	}
	// ******
}

// STATE[STUB]
void game::update_stats( const u32 current_frame_id )
{
	// LOCALS
	// const float 						last_frame_time
	// char[64] 						buff
	// ******

	// STATICS
	// static console_commands::cc_bool fps_graph = <0x4c2b578>;
	// static bool 						draw_fps_graph = <0x4c265c4>;
	// ******

	// CALL SITE INFO
	// <0x5e6237> -> ui::world& < unknown >()
	// <0x5e6249> -> render::ui::renderer& < unknown >()
	// <0x5e62d2> -> ui::world& < unknown >()
	// <0x5e62e4> -> render::ui::renderer& < unknown >()
	// <0x5e6366> -> sound::world_user& < unknown >() const
	// ******

	// FUNCTION BODY[0x5e60b0]: 52
	// <0x5e60b3>|0x003|+0x025:'813'
	// <0x5e60d8>|0x028|+0x06d:'814'
	// <0>
	// <0x5e6145>|0x095|+0x012:'816'
	// <0>
	// <0x5e6157>|0x0a7|+0x01b:'818'
	// <0x5e6172>|0x0c2|+0x02e:'819'
	// <0>
	// <0x5e61a0>|0x0f0|+0x024:'821'
	// <0>
	// <0x5e61c4>|0x114|+0x032:'823'
	// <0>
	// <1>
	// <0x5e61f6>|0x146|+0x025:'826'
	// <0x5e621b>|0x16b|+0x015:'827'
	// <0x5e6230>|0x180|+0x026:'828'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5e6256>|0x1a6|+0x048:'835'
	// <0>
	// <0x5e629e>|0x1ee|+0x018:'837'
	// <0>
	// <0x5e62b6>|0x206|+0x015:'839'
	// <0x5e62cb>|0x21b|+0x035:'840'
	// <0>
	// <0x5e6300>|0x250|+0x002:'842'
	// <0>
	// <1>
	// <0x5e6302>|0x252|+0x015:'845'
	// <0>
	// <1>
	// <0x5e6317>|0x267|+0x01a:'848'
	// <0>
	// <0x5e6331>|0x281|+0x009:'850'
	// <0x5e633a>|0x28a|+0x047:'851'
	// <0>
	// <0x5e6381>|0x2d1|+0x00e:'853'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x5e638f>|0x2df|+0x007:'861'
	// <0x5e6396>|0x2e6|+0x01c:'862'
	// <0>
	// <1>
	// ******
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
		m_network_client			= 0;
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

// STATE[STUB]
void game::load(
	pcstr const						project_resource_name,
	resources::request* const		requests_begin,
	resources::request* const		requests_end,
	variant< 32 > const** const		user_datas_begin,
	boost::function< void( resources::queries_result& ) > const&	callback
)
{
	// FUNCTION BODY[0x5e6c10]: 9
	// <0x5e6c10>|0x000|+0x01f:'914'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5e6c2f>|0x01f|+0x023:'921'
	// <0x5e6c52>|0x042|+0x007:'922'
	// ******
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

// claude@NOTE: BLOCKED on sibling cook TUs not enabled. Target body (structure
// recovered from the $S9-guarded function-static cook block + atexit dtors):
//   static animated_model_instance_cook          s_animated_model_instance_cook;
//   static game_material_manager_cook            s_material_manager_cook( false );
//   static project_cooker_simple                 s_simple_project_cook( engine( ).command_line_editor( ) );
//   static animation_analysis_result_cook        s_animation_analysis_result_cook;
//   resources::register_cook( &s_animation_analysis_result_cook );
//   static ladder_cook                           s_ladder_cook;
//   resources::register_cook( &s_ladder_cook );
//   static weapon_user_animations_container_cook  s_animation_container_cook;
//   resources::register_cook( &s_animation_container_cook );
//   static victory_item_cook                     s_victory_item_cook( m_game_world );
// Constructing s_simple_project_cook needs project_cooker_simple's vtable, whose
// delete_resource() body lives in the (still-excluded) project_cooker_simple.cpp
// sibling TU -> LNK2001. Re-enable + match once that cook cluster is built.
// STATE[STUB]
void game::register_cooks( )
{
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

// STATE[STUB]
// claude@NOTE: target body resolves m_render_output_window -> res_render_output and
// calls render::res_render_output::goto_fullscreen, which is NOT declared in our
// sources/vostok/render/core/dx11/res_render_output.h (it IS in the target structure).
// That header is the render cluster's; cannot add the decl here. Keep stub buildable.
void game::on_fullscreen_alttab( bool first )
{
	// FUNCTION BODY[0x5e5f80]: 2
	// <0>
	// <0x5e5f80>|0x000|+0x010:'1126'
	// ******
}

// STATE[STUB]
void game::draw_debug_window( )
{
	// LOCALS
	// strings::text_tree 				tree
	// ******

	// CALL SITE INFO
	// <0x5e5f43> -> void < unknown >()
	// ******

	// FUNCTION BODY[0x5e5ee0]: 14
	// <0>
	// <1>
	// <2>
	// <0x5e5ee9>|0x009|+0x00c:'1134'
	// <0x5e5ef5>|0x015|+0x016:'1135'
	// <0>
	// <0x5e5f0b>|0x02b|+0x009:'1137'
	// <0x5e5f14>|0x034|+0x013:'1138'
	// <0>
	// <0x5e5f27>|0x047|+0x011:'1140'
	// <0>
	// <1>
	// <2>
	// <0x5e5f38>|0x058|+0x00d:'1144'
	// ******
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

// STATE[STUB]
void game::set_network_client(
	base_network_client* const		network_client,
	pcstr							host,
	const u16						port,
	const bool						is_spectator
)
{
	// LOCALS
	// fixed_string< 128 > 				name
	// fixed_string< 128 > 				password
	// ******

	// CALL SITE INFO
	// <0x5e5ba4> -> bool < unknown >() const
	// <0x5e5bcf> -> void < unknown >( pcstr, const u16, pcstr, pcstr )
	// <0x5e5be0> -> network::login_client& < unknown >()
	// ******

	// FUNCTION BODY[0x5e5b80]: 20
	// <0x5e5b80>|0x000|+0x010:'1208'	{
	// <0>
	// <0x5e5b90>|0x010|+0x00f:'1210'
	// <0>
	// <0x5e5b9f>|0x01f|+0x00f:'1212'
	// <0>
	// <0x5e5bae>|0x02e|+0x008:'1214'
	// <0x5e5bb6>|0x036|+0x027:'1215'
	// <0>
	// <1>
	// <0x5e5bdd>|0x05d|+0x007:'1218'
	// <0x5e5be4>|0x064|+0x012:'1219'
	// <0>
	// <0x5e5bf6>|0x076|+0x04f:'1221'
	// <0x5e5c45>|0x0c5|+0x060:'1222'
	// <0>
	// <1>
	// <0x5e5ca5>|0x125|-0x0d4:'1225'
	// <0>
	// <1>
	// <2>
	// <0x5e5bd1>|0x051|+0x0e2:'1229'
	// <0x5e5cb3>|0x133|      :'1229'	}
	// ******
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

// STATE[STUB]
scaleform_movie_cook::scaleform_movie_cook( flash_factory& factory )
:
	// base args are buildability placeholders (the game_core cook convention);
	// a matcher confirms when this TU is enabled
	resources::translate_query_cook( resources::flash_movie_class, reuse_false, use_current_thread_id ),
	m_factory( factory )
{
	// FUNCTION BODY[0x5e5880]: 1
	// <0x5e58c1>|0x041|+0x029:'1261'
	// ******
}

// STATE[STUB]
void scaleform_movie_cook::translate_query( resources::query_result_for_cook& parent )
{
	// FUNCTION BODY[0x5e68c0]: 8
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x5e68ca>|0x00a|+0x0a9:'1273'
	// ******
}

// STATE[STUB]
void scaleform_movie_cook::delete_resource( resources::resource_base* resource )
{
	// CALL SITE INFO
	// <0x5e5de0> -> void* < unknown >( u32 )
	// ******

	// FUNCTION BODY[0x5e5da0]: 3
	// <0>
	// <0x5e5da2>|0x002|+0x025:'1279'
	// <0x5e5dc7>|0x027|+0x030:'1280'
	// ******
}

// STATE[STUB]
void scaleform_movie_cook::on_raw_data_loaded( resources::queries_result& data, resources::query_result_for_cook* parent )
{
	// LOCALS
	// resources::pinned_ptr_const< u8 > pinned
	// resources::managed_resource_ptr 	raw_data
	// ******

	// FUNCTION BODY[0x5e5fa0]: 10
	// <0>
	// <0x5e5fa6>|0x006|+0x02a:'1286'
	// <0x5e5fd0>|0x030|+0x03b:'1287'
	// <0>
	// <0x5e600b>|0x06b|+0x029:'1289'
	// <0>
	// <0x5e6034>|0x094|+0x023:'1291'
	// <0>
	// <0x5e6057>|0x0b7|+0x02d:'1293'
	// <0x5e6084>|0x0e4|+0x00b:'1294'
	// ******
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
