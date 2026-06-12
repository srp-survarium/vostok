////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

/* INCLUDES */
class vostok::engine::console;
class vostok::fixed_string<512>;
class vostok::math::rectangle<vostok::math::float2>;
class vostok::render::game::renderer;
class vostok::render::world;
class vostok::sound::sound_debug_stats;
class vostok::threading::mutex;
class vostok::timing::timer;
class survarium::base_game_scene;
class survarium::base_network_client;
class survarium::chat_handler;
class survarium::flash_factory;
class survarium::game_options;
class survarium::game_world;
class survarium::key_binder;
class survarium::lobby_menu;
class survarium::login_menu;
class survarium::main_menu;
class survarium::scaleform_game_engine;
class survarium::scheduler;
class survarium::stats;
class survarium::stats_graph;
class survarium::swf_input_translator;
class survarium::text_translator;
struct vostok::engine_user::engine;
struct vostok::engine_user::world;
struct vostok::input::engine;
struct vostok::input::world;
struct vostok::network::world;
struct vostok::physics::engine;
struct vostok::sound::world;
struct vostok::ui::engine;
struct vostok::ui::window;
struct vostok::ui::world;
enum survarium::login_menu_status_enum;
class vostok::resources::managed_resource;
class survarium::base_network_client* const;
class survarium::scaleform_render_command;
class survarium::scene_ready_type;
typedef vostok::resources::resource_ptr<survarium::items_dictionary,vostok::resources::unmanaged_intrusive_base>
	survarium::items_dictionary_ptr;
typedef vostok::resources::resource_ptr<vostok::render::base_output_window,vostok::resources::unmanaged_intrusive_base>
	vostok::render::base_output_window_ptr;
class vostok::render::base_output_window;
class survarium::items_dictionary;

/* FORWARD REFS */
class boost::function<void __cdecl(vostok::resources::queries_result &)>;
class vostok::resources::queries_result;
class vostok::resources::request;
class vostok::variant<32>;

namespace survarium {

enum survarium::game::debug_window_enum
{
	debug_window_none			= 0x0,
	debug_window_resources		= 0x1,
	debug_window_tasks			= 0x2,
};

class game : public engine_user::world , public input::engine , public ui::engine , public physics::engine , public scaleform_game_engine , public boost::noncopyable {
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

	virtual	ui::world&							ui_world						( ) override { /* no source */ }
	virtual	input::world&						input_world						( ) override { /* no source */ }

	virtual	void								execute_scaleform_command		( scaleform_render_command command ) override;

	inline	u32									game_time_ms					( ) const { /* no source */ }
	inline	u32									game_permanent_time_ms			( ) const { /* no source */ }
	inline	float								game_time_sec					( ) { /* no source */ }

	inline	float								last_frame_time					( ) const { /* no source */ }

	virtual	void								load							( pcstr project_resource_name ) override;
			void								load							(
													pcstr						project_resource_name,
													resources::request*			requests_begin,
													resources::request*			requests_end,
													variant< 32 > const**		user_datas_begin,
													boost::function< void( resources::queries_result& ) > const&	callback
												);

	virtual	void								unload							( pcstr __formal, bool destroying ) override;

	virtual	bool								is_loading						( ) const override;

			void								load_config_query				( pcstr cfg_name, bool create_renderer );

			void								on_config_loaded				( resources::queries_result& data, bool create_renderer );

			void								load_cc_script					( resources::managed_resource_ptr cfg, bool create_renderer );

	inline	swf_input_translator&				input_translator				( ) { /* no source */ }

	inline	game_world const&					get_game_world					( ) const { /* no source */ }
	inline	game_world&							get_game_world					( ) { /* no source */ }
	inline	game_options&						get_game_options				( ) { /* no source */ }
	inline	sound::world&						get_sound_world					( ) const { /* no source */ }
	inline	network::world&						get_network_world				( ) const { /* no source */ }
	inline	key_binder&							get_key_binder					( ) const { /* no source */ }
	inline	chat_handler&						get_chat_handler				( ) const { /* no source */ }

	inline	engine_user::engine&				engine							( ) const { /* no source */ }

	inline	render::world&						render_world					( ) { /* no source */ }

			flash_factory&						get_flash_factory				( );
	inline	stats&								get_stats						( ) { /* no source */ }

	inline	render::game::renderer&				renderer						( ) { /* no source */ }

	inline	render::base_output_window_ptr		render_output_window			( ) { /* no source */ }

	inline	math::rectangle< float2 > const&	viewport						( ) const { /* no source */ }

	inline	main_menu&							main_menu						( ) { /* no source */ }
	inline	lobby_menu&							lobby_menu						( ) { /* no source */ }
	inline	login_menu&							login_menu						( ) { /* no source */ }

	inline	text_translator&					text_translator					( ) { /* no source */ }

	inline	items_dictionary&					items_dictionary				( ) const { /* no source */ }

	inline	base_game_scene*					active_scene					( ) const { /* no source */ }

	inline	scheduler&							scheduler						( ) { /* no source */ }

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

	inline	u32									current_frame_id				( ) const { /* no source */ }

	inline	timing::timer const&				permanent_timer					( ) const { /* no source */ }

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

	inline	base_network_client*				get_network_client				( ) { /* no source */ }

	inline	pcstr								project_resource_name			( ) { /* no source */ }

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

			void								create_debug_window				( );
			void								destroy_debug_window			( );
			void								draw_debug_window				( );
	inline	void								toggle_debug_window				( ) { /* no source */ }

private:
	/* 0x0000 */	/* engine_user::world */
	/* 0x0008 */	/* input::engine */
	/* 0x0009 */	/* ui::engine */
	/* 0x000a */	/* physics::engine */
	/* 0x0004 */	/* scaleform_game_engine */
	/* 0x000b */	/* boost::noncopyable */
	/* 0x000b */	bool								hide_game_stats;
	/* 0x0010 */	timing::timer						m_timer;
	/* 0x0028 */	timing::timer						m_permanent_timer;
	/* 0x0040 */	threading::mutex					m_application_activation;
	/* 0x0058 */	render::base_output_window_ptr		m_render_output_window;
	/* 0x005c */	math::rectangle< float2 >			m_viewport;
	/* 0x006c */	stats_graph*						m_fps_graph;
	/* 0x0070 */	engine::console*					m_console;
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
	/* 0x0370 */	main_menu*							m_main_menu;
	/* 0x0374 */	lobby_menu*							m_lobby_menu;
	/* 0x0378 */	login_menu*							m_login_menu;
	/* 0x037c */	chat_handler*						m_chat_handler;
	/* 0x0380 */	scheduler							m_scheduler;
	/* 0x03ac */	base_game_scene*					m_active_scene;
	/* 0x03b0 */	ui::window*							m_text_wnd;
	/* 0x03b4 */	items_dictionary_ptr				m_items_dictionary;
	/* 0x03b8 */	base_network_client*				m_network_client;
	/* 0x03bc */	flash_factory*						m_flash_factory;
	/* 0x03c0 */	swf_input_translator				m_swf_input_translator;
	/* 0x03d8 */	bool								m_is_active;
	/* 0x03dc */	text_translator						m_text_translator;
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
	/* 0x0870 */	game::debug_window_enum				m_debug_window_type;
	/* 0x0874 */	ui::window*							m_debug_window;
}; // class game

STATIC_SIZE_ASSERT(game, 0x878);

} // namespace survarium

#endif // #ifndef GAME_H_INCLUDED
