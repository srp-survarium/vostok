////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef LOBBY_MENU_H_INCLUDED
#define LOBBY_MENU_H_INCLUDED

/* INCLUDES */
class survarium::base_game_scene;
class survarium::game_camera;
class survarium::lobby_menu_external_handler;
class survarium::profile_player_character;
class survarium::relocate_item_func;
struct vostok::input::handler;
struct survarium::lobby_menu::<unnamed-type-m_match_stats>;
struct survarium::scheduler::identifier;
enum vostok::lobby_client_message_types_enum;
class vostok::input::enum_gamepad_action;
class vostok::input::enum_keyboard;
class vostok::input::enum_keyboard_action;
class vostok::input::enum_mouse_key_action;
class vostok::input::gamepad_button;
class vostok::input::mouse_button;
typedef vostok::resources::resource_ptr<survarium::flash_movie_resource,vostok::resources::unmanaged_intrusive_base>
	survarium::flash_movie_resource_ptr;
typedef vostok::resources::resource_ptr<survarium::simple_game_project,vostok::resources::unmanaged_intrusive_base>
	survarium::simple_game_project_ptr;
class survarium::flash_movie_resource;
class survarium::simple_game_project;

/* FORWARD REFS */
class vostok::input::world;
class vostok::network::login_client;
class vostok::resources::queries_result;
class survarium::bullet_manager;
class survarium::game;
class survarium::lobby_client;
class survarium::messaging_client;
class survarium::player_parameters_cooker_data;

namespace survarium {

class lobby_menu : public base_game_scene , public input::handler {
public:
			explicit					lobby_menu							( game& g );
	virtual								~lobby_menu							( );

	virtual	void						on_activate							( ) override;
	virtual	void						on_deactivate						( ) override;

	virtual	void						show_ui								( bool b_show ) override;
	virtual	bool						is_mouse_over_ui					( ) override;

	virtual	void						tick								(
											const u32		frame_delta_in_ms,
											const u32		current_time_in_ms,
											const bool		is_game_paused
										) override;

	virtual	input::handler&				input_handler						( ) { /* no source */ }

	virtual	bullet_manager&				get_bullet_manager					( ) const override { /* no source */ }

	virtual	bool						on_keyboard_action					(
											input::world*					input_world,
											input::enum_keyboard			key,
											input::enum_keyboard_action		action
										) override;
	virtual	bool						on_gamepad_action					(
											input::world*					input_world,
											input::gamepad_button			button,
											input::enum_gamepad_action		action
										) override;
	virtual	bool						on_mouse_key_action					(
											input::world*					input_world,
											input::mouse_button				button,
											input::enum_mouse_key_action	action
										) override;
	virtual	bool						on_mouse_move						(
											input::world*		input_world,
											s32					x,
											s32					y,
											s32					z
										) override;

	virtual	s32							input_priority						( ) override { /* no source */ }

	virtual	void						clear_resources						( ) override;

			void						on_connected_to_lobby				( );
			void						on_disconnected_from_lobby			( );
			void						on_client_status_received			( lobby::query_info_types type );
			void						on_friendship_status_recivied		(
											const messaging::friendship_actions_enum	arg_0 /* messaging::friendship_actions_enum type */
										);
			void						on_operation_permitted_received		( lobby_client_message_types_enum op_type );
			void						on_operation_denied_received		( lobby_client_message_types_enum op_type, pcstr description );
			void						on_profile_arrived					( u8 profile_id );
			void						on_price_items_arrived				( u8 trader_id );
			void						on_shop_ui_ready					( );
			void						on_ui_destroy						( );
			void						on_match_message_arrived			( wchar_t const* w_text );
			void						on_stats_message_arrived			(
											wchar_t const*						w_text,
											wchar_t const*						w_sender_name,
											messaging::message_channel_enum		message_channel
										);
			void						on_player_reputations_arrived		( );

			void						update_level_loading_progress		( );

			void						reset_account_money					( );

			void						query_scene_resources				( );

			void						on_render_scenes_ready				( resources::queries_result& data );
			void						player_parameters_ready				( resources::queries_result& data, player_parameters_cooker_data* cook_data );

			lobby_client&				lobby_client						( );
	inline	network::login_client&		login_client						( ) { /* no source */ }
			messaging_client&			messaging_client					( );

			void						query_lobby_info					( );
			void						query_account_data					( );

			void						update_ui							( const u32 frame_delta_in_ms, const u32 current_time_in_ms );

			void						fill_profiles						( );
			void						fill_skills_tree					( );

			void						on_items_compatibility_arrived		( );
			void						on_slot_restrictions_arrived		( );

			void						fill_character_data					( );
			void						fill_inventory_contents				( );

			void						on_profile_changed					( u8 profile_id );

	inline	u8							selected_profile					( ) { /* no source */ }

			void						fill_items_dictionary				( );
			void						fill_inventory_labels				( );
			void						fill_service_prices					( );
			void						fill_friend_list					( );
			void						fill_ignore_list					( );
			void						fill_found_players					( );

			void						set_cursor							( u8 id );
			void						set_fps_stats						( float fps );
			void						set_ping							( u32 ping_val );

			void						show_match_making					( bool b_show );

			void						switch_to_level_loading				( );

	inline	bool						can_player_carry					(
											u32			arg_0,
											u32			arg_1,
											u32			arg_2,
											float		arg_3
										) { /* no source */ }

			void						update_status						( );

			void						request_status_from_server			( u32 delay_ms );
			void						request_status_from_server_impl		( const u32 frame_delta_ms, const u32 current_time_ms );
			void						request_friends_status_from_server	( u32 delay_ms );
			void						request_friends_status_from_server_impl( const u32 frame_delta_ms, const u32 current_time_ms );

			void						show_disconnected_message			( bool b_show );

private:
	/* 0x0000 */	/* base_game_scene */
	/* 0x00bc */	/* input::handler */
	/* 0x00c0 */	game_camera*					m_camera;
	/* 0x00c4 */	simple_game_project_ptr			m_lobby_game_project;
	/* 0x00c8 */	scheduler::identifier			m_update_status_handler;
	/* 0x00cc */	scheduler::identifier			m_update_friends_status_handler;
	/* 0x00d0 */	flash_movie_resource_ptr		m_cursor_ui;
	/* 0x00d4 */	flash_movie_resource_ptr		m_lobby_menu_ui;
	/* 0x00d8 */	flash_movie_resource_ptr		m_message_ui;
	/* 0x00dc */	flash_movie_resource_ptr		m_match_making_ui;
	/* 0x00e0 */	u8								m_selected_profile;
	/* 0x00e4 */	u32								m_ui_last_update_time;
	/* 0x00e8 */	profile_player_character*		m_character;
	/* 0x00ec */	relocate_item_func*				m_relocate_item_func;
	/* 0x00f0 */	lobby_menu_external_handler*	m_lobby_menu_external_handler;
	/* 0x00f4 */	bool							m_ui_static_info_initialized;
	/* 0x00f5 */	bool							m_is_in_match_making;
	/* 0x00f6 */	bool							m_is_connected_to_lobby;
	/* 0x00f8 */	u32								m_last_ping_time_in_ms;
	/* 0x00fc */	float							m_player_max_carried_weight;
	/* 0x0100 */	float							m_player_total_items_weight;
	/* 0x0104 */	float							m_level_loading_progress;
	/* 0x0108 */	u32								m_last_queries_count;
	/* 0x010c */	bool							m_in_destroying;
	/* 0x0110 */	lobby_menu::< unnamed-type-m_match_stats >	m_match_stats;
}; // class lobby_menu

STATIC_SIZE_ASSERT(lobby_menu, 0x12C);

} // namespace survarium

#endif // #ifndef LOBBY_MENU_H_INCLUDED
