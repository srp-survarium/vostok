// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <vostok/engine/engine_user.h>
#include <vostok/input/engine.h>
#include <vostok/ui/engine.h>
#include <vostok/physics/engine.h>

#include <vostok/game_core/items_dictionary.h>
#include <vostok/game_core/scheduler.h>
#include <vostok/math_rectangle.h>
#include <vostok/render/engine/base_classes.h>	// render_output_window_ptr
#include <vostok/threading_mutex.h>
#include <vostok/timing_timer.h>

#include "game_options.h"
#include "game_world.h"
#include <vostok/scaleform/sources/scaleform_game_engine.h>
#include "swf_input_translator.h"
#include "text_translator.h"

namespace vostok {
namespace sound {
	class sound_debug_stats;
} // namespace sound
namespace ui {
	struct window;
} // namespace ui
} // namespace vostok

namespace survarium {

class base_network_client;
class chat_handler;
class flash_factory;
class key_binder;
class lobby_menu;
class login_menu;
class main_menu;
class stats;
class stats_graph;
enum login_menu_status_enum;

// param of on_queried_by_network_client_scene_ready (mangling W4scene_ready_type@2@);
// canonical: headers/survarium/enums/scene_ready_type.h
enum scene_ready_type
{
	login_scene_ready	= 0x0,
	lobby_scene_ready	= 0x1,
};

typedef resources::resource_ptr<
	items_dictionary,
	resources::unmanaged_intrusive_base
> items_dictionary_ptr;

// void* game::`scalar deleting destructor'( u32 ) // FUNCTION BODY[0x8da10]: 8, <0x8d9a6>|0x006|+0x00f:'100' ...

class game : public engine_user::world , private input::engine , private ui::engine , private physics::engine , private scaleform_game_engine , private boost::noncopyable {
	// game_world reaches game's private hide_game_stats directly (game_world::
	// update_npc_stats writes it) - the original befriends its game_world scene.
	friend class game_world;
	// network_client::connect_to_login inlines game's private switch_to_scene
	// (codegen-neutral friendship; PDB does not record it)
	friend class network_client;
public:
											game							(
														engine_user::engine&	engine,
														render::world&			render_world,
														sound::world&			sound,
														network::world&			network
													);
	virtual										~game							( );

	virtual	void								enable							( bool value ) override;

	virtual	void								tick							( u32 current_frame_id ) override;

	virtual	void								clear_resources					( ) override;

	virtual	ui::world&							ui_world						( ) override		{ return *m_ui_world; }

	virtual	input::world&						input_world						( ) override		{ return *m_input_world; }

	virtual	void								execute_scaleform_command		( scaleform_render_command command ) override;

	inline	u32									game_time_ms					( ) const { return m_current_time_in_ms; }
	inline	u32									game_permanent_time_ms			( ) const { /* no source */ return m_permanent_time_in_ms; }
	inline	float								game_time_sec					( ) { /* no source */ return 0.f; }

	inline	float								last_frame_time					( ) const { /* no source */ return m_last_frame_time; }

	virtual	void								load							( pcstr project_resource_name ) override;
			void								load							(
													pcstr						project_resource_name,
													resources::request*			requests_begin,
													resources::request*			requests_end,
													variant< 32 > const**		user_datas_begin,
													boost::function< void( resources::queries_result& ) > const&	callback
												);

	// PDB: first param __formal, genuinely unused
	virtual	void								unload							( pcstr __formal, bool destroying ) override;

	virtual	bool								is_loading						( ) const override;

			void								load_config_query				( pcstr cfg_name, bool create_renderer );

			void								on_config_loaded				( resources::queries_result& data, bool create_renderer );

			void								load_cc_script					( resources::managed_resource_ptr cfg, bool create_renderer );

	inline	swf_input_translator&				input_translator				( ) { /* no source */ return m_swf_input_translator; }

	inline	game_world const&					get_game_world					( ) const { /* no source */ return m_game_world; }
	inline	game_world&							get_game_world					( ) { /* no source */ return m_game_world; }
	inline	game_options&						get_game_options				( ) { /* no source */ return m_game_options; }
	inline	sound::world&						get_sound_world					( ) const { /* no source */ return m_sound_world; }
	inline	network::world&						get_network_world				( ) const { /* no source */ return m_network_world; }
	inline	key_binder&							get_key_binder					( ) const { /* no source */ return *m_key_binder; }
	inline	chat_handler&						get_chat_handler				( ) const { /* no source */ return *m_chat_handler; }

	inline	engine_user::engine&				engine							( ) const { /* no source */ return m_engine; }

	inline	render::world&						render_world					( ) { /* no source */ return m_render_world; }

			flash_factory&						get_flash_factory				( );
	inline	stats&								get_stats						( ) { /* no source */ return *m_stats; }

	inline	render::game::renderer&				renderer						( ) { /* no source */ return m_renderer; }

	// PDB spells the return type vostok::render::base_output_window_ptr - the
	// same resource_ptr type our render tree typedefs as render_output_window_ptr
	inline	render::render_output_window_ptr	render_output_window			( ) { /* no source */ return m_render_output_window; }

	inline	math::rectangle< float2 > const&	viewport						( ) const { /* no source */ return m_viewport; }

	// method names hide the same-named class names from here on - the member
	// declarations below use class-elaborated spellings (mangling-neutral)
	inline	class main_menu&					main_menu						( ) { /* no source */ return *m_main_menu; }
	inline	class lobby_menu&					lobby_menu						( ) { /* no source */ return *m_lobby_menu; }
	inline	class login_menu&					login_menu						( ) { /* no source */ return *m_login_menu; }

	inline	class text_translator&				text_translator					( ) { /* no source */ return m_text_translator; }

	inline	class items_dictionary&				items_dictionary				( ) const { /* no source */ return *m_items_dictionary; }

	inline	base_game_scene*					active_scene					( ) const { /* no source */ return m_active_scene; }

	inline	class scheduler&					scheduler						( ) { /* no source */ return m_scheduler; }

			void								create_network_client			( const bool is_spectator );
			void								create_lobby_menu				( );
			void								create_login_menu				( );

			void								toggle_console					( );

			void								switch_to_game_world			( );
			void								switch_to_main_menu				( );
			void								switch_to_lobby					( );
			void								switch_to_login					( login_menu_status_enum status );

			void								commit_suicide					( );

			void								respawn_local_player			( );

			void								exit							( pcstr str );

	inline	u32									current_frame_id				( ) const { /* no source */ return m_current_frame_id; }

	inline	timing::timer const&				permanent_timer					( ) const { /* no source */ return m_permanent_timer; }

	inline	void								test_action_portal_system		( ) { /* no source */ }

			void								toggle_pause					( );

			void								pause							( );

			void								resume							( );

			void								build_lpv_geometry				( );

			void								create_and_assign_network_client( fixed_string< 512 > client_options, const bool is_spectator );

			void								on_queried_by_network_client_scene_ready( scene_ready_type scene_ready );

			void								activate_main_menu				( );
			void								deactivate_main_menu			( );

			void								discard_current_match			( );

	/* 0x000b */	bool								hide_game_stats;

	inline	base_network_client*				get_network_client				( ) { /* no source */ return m_network_client; }

	inline	pcstr								project_resource_name			( ) { /* no source */ return m_project_resource_name.c_str( ); }

private:
			void								set_network_client				(
													base_network_client* const		network_client,
													pcstr							host,
													const u16						port,
													const bool						is_spectator
												);

			void								switch_to_scene					( base_game_scene* scene );

			void								update_stats					( const u32 current_frame_id );

			void								register_console_commands		( );
			void								register_cooks					( );

			void								load_cmd						( pcstr project_name );
			void								unload_cmd						( pcstr s );

			void								initialize_input				( );
			void								initialize_ui					( );
			void								initialize_modules				( );
			void								deinitialize_modules			( );

	virtual	void								on_application_activate			( ) override;
	virtual	void								on_application_deactivate		( ) override;

	virtual	void								on_fullscreen_alttab			( bool first ) override;

			void								query_base_resources			( );
	inline	void								query_render_window				( bool arg_0 ) { /* no source */ }

			void								on_configs_loaded				( resources::queries_result& result );
			void								on_base_resources_created		( resources::queries_result& data );
			void								on_render_output_window_created	( resources::queries_result& data );
			void								on_renderer_created				( resources::queries_result& data );

private:
	/* 0x0000 */	/* engine_user::world */
	/* 0x0008 */	/* input::engine */
	/* 0x0009 */	/* ui::engine */
	/* 0x000a */	/* physics::engine */
	/* 0x0004 */	/* scaleform_game_engine */
	/* 0x000b */	/* boost::noncopyable */

	/* 0x0010 */	timing::timer						m_timer;
	/* 0x0028 */	timing::timer						m_permanent_timer;
	/* 0x0040 */	threading::mutex					m_application_activation;
	/* 0x0058 */	render::render_output_window_ptr	m_render_output_window;
	/* 0x005c */	math::rectangle< float2 >			m_viewport;
	/* 0x006c */	stats_graph*						m_fps_graph;
	/* 0x0070 */	vostok::engine::console*			m_console;
	/* 0x0074 */	stats*								m_stats;
	/* 0x0078 */	key_binder*							m_key_binder;
	/* 0x007c */	engine_user::engine&				m_engine;
	/* 0x0080 */	render::world&						m_render_world;
	/* 0x0084 */	sound::world&						m_sound_world;
	/* 0x0088 */	network::world&						m_network_world;
	/* 0x008c */	input::world*						m_input_world;
	/* 0x0090 */	ui::world*							m_ui_world;
	/* 0x0094 */	render::game::renderer&				m_renderer;
	/* 0x0098 */	game_world							m_game_world;
	/* 0x0370 */	class main_menu*					m_main_menu;
	/* 0x0374 */	class lobby_menu*					m_lobby_menu;
	/* 0x0378 */	class login_menu*					m_login_menu;
	/* 0x037c */	chat_handler*						m_chat_handler;
	/* 0x0380 */	class scheduler						m_scheduler;
	/* 0x03ac */	base_game_scene*					m_active_scene;
	/* 0x03b0 */	ui::window*							m_text_wnd;
	/* 0x03b4 */	items_dictionary_ptr				m_items_dictionary;
	/* 0x03b8 */	base_network_client*				m_network_client;
	/* 0x03bc */	flash_factory*						m_flash_factory;
	/* 0x03c0 */	swf_input_translator				m_swf_input_translator;
	/* 0x03d8 */	bool								m_is_active;
	/* 0x03dc */	class text_translator				m_text_translator;
	/* 0x03e0 */	u32									m_first_frame_time_in_ms;
	/* 0x03e4 */	u32									m_previous_frame_time_in_ms;
	/* 0x03e8 */	u32									m_permanent_time_in_ms;
	/* 0x03ec */	float								m_last_frame_time;
	/* 0x03f0 */	u32									m_current_frame_id;
	/* 0x03f4 */	u32									m_current_time_in_ms;
	/* 0x03f8 */	bool								m_enabled;
	/* 0x03f9 */	bool								m_initialized;
	/* 0x03fc */	float								m_last_sound_timescale_factor;
	/* 0x0400 */	bool								m_is_paused;
	/* 0x0401 */	bool								m_lpv_geometry_builded;
	/* 0x0402 */	bool								m_lobby_scene_ready;
	/* 0x0403 */	bool								m_login_scene_ready;
	/* 0x0404 */	fixed_string< 512 >					m_network_client_options;
	/* 0x0610 */	fixed_string< 512 >					m_project_resource_name;
	/* 0x081c */	game_options						m_game_options;
	/* 0x086c */	sound::sound_debug_stats*			m_sound_stats;

	enum debug_window_enum
	{
		debug_window_none			= 0x0,
		debug_window_resources		= 0x1,
		debug_window_tasks			= 0x2,
	};

	/* 0x0870 */	game::debug_window_enum				m_debug_window_type;
	/* 0x0874 */	ui::window*							m_debug_window;

			void								create_debug_window				( );
			void								destroy_debug_window			( );
			void								draw_debug_window				( );
	inline	void								toggle_debug_window				( ) { /* no source */ }
}; // class game

STATIC_SIZE_ASSERT(game, 0x878);

} // namespace survarium

#endif // #ifndef GAME_H_INCLUDED
