////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

/* INCLUDES */
enum const survarium::affect_event_type_enum;
enum const survarium::hit_affects_type_enum;
class vostok::collision::animated_object;
class vostok::resources::unmanaged_resource;
class survarium::base_game_scene;
class survarium::circular_buffer<survarium::client_player_history_item>;
class survarium::game;
class survarium::game_world_ui;
class survarium::player_input_handler;
class survarium::player_stamina;
class survarium::player_stealth;
class survarium::stats_graph;
class survarium::vector<survarium::player_actions_subscriber *>;
struct survarium::base_player;
struct survarium::client_player_state;
struct survarium::flash_text;
struct survarium::player_input;
enum vostok::animation::callback_return_type_enum;
enum vostok::animation::reserved_channel_ids_enum;
class vostok::resources::managed_resource;
class survarium::bullet* const;
class survarium::damage_model;
class survarium::game_team_id;
class survarium::hit_initiator const* const;
class survarium::inventory_item;
typedef vostok::resources::resource_ptr<survarium::interactive_object,vostok::resources::unmanaged_intrusive_base>
	survarium::interactive_object_ptr;
class survarium::interactive_object;

/* FORWARD REFS */
class boost::function<enum vostok::animation::callback_return_type_enum __cdecl(vostok::animation::animation_callback_params &)>;
class vostok::animation::animation_player;
class vostok::animation::skeleton;
class vostok::collision::bone_collision_data;
class vostok::network_core::packet_reader;
class vostok::physics::bt_character_controller;
class vostok::physics::world;
class survarium::client_player_history_item;
class survarium::engine;
class survarium::hit_info;
class survarium::ladder;
class survarium::player_actions_subscriber;
class survarium::player_creation_params;
class survarium::server_player_update;

namespace survarium {

enum survarium::game_team_id
{
	team_1				= 0x00,
	team_2				= 0x01,
	team_neutral		= 0x02,
	team_undefined		= 0x03,
	team_invalid		= 0xff,
};

class player : public base_player , public resources::unmanaged_resource {
public:
			explicit								player								( player_creation_params const& params );
	virtual											~player								( );

			void									insert								( const bool is_alive );
			void									remove								( );

			void									tick								( const u32 current_time_in_ms );

			void									kill								( const u32 current_time_in_ms );

			void									update_camera						( );

			void									apply_hit_directly					( hit_info const& info, u32 current_time_in_ms );
			void									apply_damage_model_affect			(
														pcstr							part_name,
														const hit_affects_type_enum		arg_1 /* hit_affects_type_enum affect */,
														const affect_event_type_enum	arg_2 /* affect_event_type_enum event_type */
													);

			void									hide								( );

			void									show								( );

			void									set_character_transform				( float3 const& position, float orientation, float look_pitch );

			void									time_warp							( server_player_update const& action, u32 time_in_ms );

			void									set_use_physics_controller_for_current( const bool value );

			void									attach_controller					(
														player_input_handler*		handler,
														stats_graph*				linear_speed,
														stats_graph*				angular_speed,
														game_world_ui*				ui
													);
			void									detach_controller					( );

	inline	float4x4 const&							transform							( ) const { /* no source */ }

	inline	client_player_state&					get_target							( ) { /* no source */ }
	inline	client_player_state const&				get_current							( ) const { /* no source */ }

	inline	bool									is_visible							( ) const { /* no source */ }

	virtual	game_team_id							team								( ) const override { /* no source */ }

	inline	wchar_t const*							get_profile_name					( ) const { /* no source */ }

	virtual	damage_model_ptr const&					damage_model						( ) const override { /* no source */ }

	virtual	void									deserialize							( network_core::packet_reader& reader ) override;

	inline	player_input_handler&					get_input_handler					( ) { /* no source */ }
	virtual	physics::world*							get_physics_world					( ) override;

			void									reset_fov_factor					( );
			void									set_near_plane_factor				( float near_plane_factor );
			void									set_target_fov_factor				( float target_fov_factor, float transition_time );
	inline	void									set_target_fov_factor				( float arg_0 ) { /* no source */ }
			float									fov_factor							( const u32 current_time_in_ms ) const;
	inline	float									target_fov_factor					( ) const { /* no source */ }

	inline	circular_buffer< client_player_history_item > const&	history								( ) const { /* no source */ }

	inline	float4x4 const&							get_target_character_transform		( ) const { /* no source */ }
	inline	float									get_player_max_carried_weight		( ) const { /* no source */ }
	virtual	float4x4 const&							get_transform						( ) const override { /* no source */ }

	virtual	void									hit									(
														hit_initiator const* const		initiator,
														const u32						bone_index,
														pcstr							damage_type,
														const float						amount,
														const float						armor_piercing,
														bullet* const					bullet
													) override;
	virtual	void									hit									(
														hit_initiator const* const		initiator,
														collision::bone_collision_data const&	bone_data,
														pcstr							damage_type,
														const float						amount,
														const float						armor_piercing,
														bullet* const					bullet
													) override;

	virtual	float									get_speed							( ) const override;

	virtual	player_input const&						input								( ) const override { /* no source */ }

	virtual	float3 const&							position							( ) const override { /* no source */ }

	virtual	float									get_look_pitch						( ) const override { /* no source */ }

	virtual	void									use_ladder							( ladder* __formal ) override;

			void									set_head_visibility					( bool is_visible );

	virtual	animation::skeleton const&				skeleton							( ) const override;

	virtual	void									subscribe_on_actions				( player_actions_subscriber* subscriber ) override;
	virtual	void									unsubscribe_from_actions			( player_actions_subscriber* subscriber ) override;

	virtual	void									on_fire								( ) override;

	virtual	void									jump								( ) override;

	virtual	void									end_jump							( ) override;

	virtual	void									crouch								( ) override;

	virtual	void									stand_up							( ) override;

	virtual	bool									set_new_active_item					( inventory_item_ptr const& item ) override;

	virtual	physics::bt_character_controller&		physics_controller					( ) override { /* no source */ }

	virtual	player_stamina&							stamina								( ) override { /* no source */ }

	virtual	void									take_inventory_item					( inventory_item_ptr const& item ) override;

	virtual	engine&									get_engine							( ) override;

	virtual	void									subscribe_animation_player			(
														pcstr			channel_id,
														boost::function< enum animation::callback_return_type_enum( animation::animation_callback_params& ) > const&	callback,
														pcvoid			callback_uid,
														resources::managed_resource_ptr const&	animation,
														const u8		event_type,
														pcvoid const	animated_object
													) override;
	virtual	void									subscribe_animation_player			(
														animation::reserved_channel_ids_enum	channel_id,
														boost::function< enum animation::callback_return_type_enum( animation::animation_callback_params& ) > const&	callback,
														pcvoid			callback_uid,
														resources::managed_resource_ptr const&	animation,
														pcvoid const	animated_object
													) override;
	virtual	void									unsubscribe_animation_player		( pcstr channel_id, pcvoid callback_uid ) override;
	virtual	void									unsubscribe_animation_player		( animation::reserved_channel_ids_enum channel_id, pcvoid callback_uid ) override;

	virtual	bool									is_replaying_history				( ) const override { /* no source */ }

	virtual	u32										local_time							( const u32 time_in_ms ) const override { /* no source */ }

			void									add_models_to_scene					( );
			void									remove_models_from_scene			( );

			void									serialize_current_state				( const u32 current_time_in_ms );

			void									select_animations					( const u32 current_time_in_ms );

			void									set_physics_controller_walk_vector	( client_player_state& state );

			void									smooth								( const float time_delta );

			void									render								( const u32 __formal, const u32 current_time_in_ms );

			void									render_crosshair_info				( );
			void									update_speed_info					( );

			u32										history_lower_bound_index			( const u32 time_in_ms ) const;

			void									remove_oldest_history_items			( const u32 new_oldest_time_in_ms );
			void									remove_oldest_history_item			( );
			void									restore_history_item				( client_player_history_item& item );
			void									update_history_item					(
														client_player_history_item&				item,
														client_player_history_item const*		next_item,
														server_player_update const&				server_action,
														const u32								server_action_time_in_ms,
														float4x4&								previous_transform,
														bool&									__formal
													);

			void									replay_history						( const u32 from_index, float4x4& previous_transform );

			void									update_history_item_from_previous	(
														client_player_history_item const&		previous_item,
														client_player_history_item&				item_to_update,
														float4x4&								previous_transform
													);

	inline	animation::callback_return_type_enum	on_animation_event					(
														pcvoid const	arg_0,
														resources::managed_resource_ptr const&	arg_1,
														pcstr const		arg_2,
														const u32		arg_3,
														const u8		arg_4
													) { /* no source */ }

			player_input							local_input							( ) const;
			player_input							remote_input						( ) const;
			void									apply_input							( client_player_state& player_state, float2 const& rotation_to_apply );
			void									apply_input							(
														client_player_state&	player_state,
														float2 const&			previous_velocity,
														float2 const&			current_acceleration,
														const float				time_delta
													);

			float4x4								get_transform_for_animation_player	( pcvoid const animated_object, float4x4 const& character_transform ) const;

	inline	animation::callback_return_type_enum	on_animation_ik_interval			(
														pcvoid const	arg_0,
														resources::managed_resource_ptr const&	arg_1,
														pcstr const		arg_2,
														const u32		arg_3,
														const u8		arg_4
													) { /* no source */ }

			void									detect_usable_objects				( const u32 current_time_in_ms );

			void									notify_actions_subscribers			( );

	inline	void									on_weapon_aim_started				( ) { /* no source */ }
	inline	void									on_weapon_aim_ended					( ) { /* no source */ }

	inline	void									set_near_plane						( float arg_0 ) { /* no source */ }

	inline	void									update_recoil						( const u32 arg_0, const u32 arg_1 ) { /* no source */ }
	inline	void									update_dispersion					( const u32 arg_0, const u32 arg_1 ) { /* no source */ }
	inline	void									update_breath_vibration				( const u32 arg_0, const u32 arg_1 ) { /* no source */ }

	virtual	void									on_before_active_object_changed		(
														interactive_object_ptr const&		current_active_object,
														interactive_object_ptr const&		target_active_object
													) const override;

			void									log_active_object					( pcstr const header ) const;

			void									process_quick_slots_for_proxy_player( );
			void									process_quick_slots_for_current_player( );

			void									insert_alive						( );
			void									remove_alive						( );

	virtual	animation::animation_player&			animation_player					( ) override { /* no source */ }
	virtual	animation::animation_player const&		animation_player					( ) const override { /* no source */ }

			void									apply_input_before_new_transform	(
														client_player_state&	player_state,
														player_input const&		previous_input,
														const float				time_delta
													);

			void									compute_bones						( const u32 current_time_in_ms );

	inline	bool									is_demo_player						( ) const { /* no source */ }

	inline	game_world_ui*							game_ui								( ) { /* no source */ }

private:
	/* 0x0000 */	/* base_player */
	/* 0x0120 */	/* resources::unmanaged_resource */
	/* 0x0228 */	client_player_state				m_current;
	/* 0x87fc */	client_player_state				m_target;
	/* 0x10dd0 */	float4x4						m_root_transform;
	/* 0x10e10 */	vector< player_actions_subscriber* >	m_player_actions_subscribers;
	/* 0x10e1c */	circular_buffer< client_player_history_item >	m_history;
	/* 0x10e30 */	player_stamina					m_stamina;
	/* 0x10e98 */	player_stealth					m_stealth;
	/* 0x10ec4 */	float3							m_last_frame_position;
	/* 0x10ed0 */	player_input					m_input;
	/* 0x10ee4 */	flash_text						m_text;
	/* 0x10ef0 */	collision::animated_object*		m_damage_collision;
	/* 0x10ef4 */	player_input_handler*			m_local_input_controller;
	/* 0x10ef8 */	stats_graph*					m_linear_speed_graph;
	/* 0x10efc */	stats_graph*					m_angular_speed_graph;
	/* 0x10f00 */	base_game_scene&				m_game_scene;
	/* 0x10f04 */	game&							m_game;
	/* 0x10f08 */	interactive_object_ptr			m_empty_hands;
	/* 0x10f0c */	u32								m_current_time_in_ms;
	/* 0x10f10 */	float							m_last_frame_rotation;
	/* 0x10f14 */	u32								m_last_server_correction_time;
	/* 0x10f18 */	float							m_target_fov_factor;
	/* 0x10f1c */	float							m_start_fov_factor;
	/* 0x10f20 */	float							m_current_fov_factor;
	/* 0x10f24 */	float							m_fov_factor_transition_time;
	/* 0x10f28 */	u32								m_start_fov_factor_change_time_in_ms;
	/* 0x10f2c */	game_team_id					m_team_id;
	/* 0x10f30 */	u8								foot_3rd_view_game_material_id;
	/* 0x10f31 */	u8								foot_1st_view_game_material_id;
	/* 0x10f32 */	bool							m_show_server_player;
	/* 0x10f33 */	bool							m_show_client_player;
	/* 0x10f34 */	bool							m_is_visible;
	/* 0x10f35 */	bool							m_is_first_born;
	/* 0x10f36 */	bool							m_use_physics_controller_for_current;
	/* 0x10f37 */	bool							m_is_first_tick;
	/* 0x10f38 */	wchar_t							profile_name[32];
	/* 0x10f78 */	bool							m_player_head_visible;
	/* 0x10f7c */	game_world_ui*					m_game_ui;
	/* 0x10f80 */	bool							m_is_demo_player;
	/* 0x10f81 */	bool							m_force_bones_recompute;
}; // class player

STATIC_SIZE_ASSERT(player, 0x10F88);

} // namespace survarium

#endif // #ifndef PLAYER_H_INCLUDED
