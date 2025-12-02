////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef BASE_PLAYER_H_INCLUDED
#define BASE_PLAYER_H_INCLUDED

#include <vostok/game_core/inventory_holder.h>
#include <vostok/game_core/collision_user.h>
#include <vostok/game_core/hit_initiator.h>
#include <vostok/game_core/hit_receiver.h>

namespace survarium {

/*
animation::animation_callback_params 
animation::animation_playback_state
animation::animation_player 
animation::callback_return_type_enum
engine
inventory_holder
network_core::packet_reader
network_core::udp_match_packet 
physics::bt_character_controller
player_death_subscriber
player_input
player_stamina
struct survarium::breath_holding_params;
struct survarium::character_dispersion_params;
struct survarium::character_recoil_params;
survarium::damage_model
survarium::game_world_object
survarium::interactive_object
survarium::player_death_subscriber

# include 

animation::reserved_channel_ids_enum
game_team_id
base_player_creation_params const
animation::skeleton 
*/

struct survarium::breath_holding_params;
struct survarium::character_dispersion_params;
struct survarium::character_recoil_params;
class vostok::intrusive_list<survarium::game_world_object,vostok::resources::resource_ptr<survarium::game_world_object,vostok::resources::unmanaged_intrusive_base>,264,vostok::threading::single_threading_policy,vostok::no_size_policy,vostok::no_debug_policy>;
class vostok::intrusive_list<survarium::player_death_subscriber,survarium::player_death_subscriber *,32,vostok::threading::single_threading_policy,vostok::no_size_policy,vostok::no_debug_policy>;
class vostok::resources::resource_ptr<survarium::damage_model,vostok::resources::unmanaged_intrusive_base>;
class vostok::resources::resource_ptr<survarium::interactive_object,vostok::resources::unmanaged_intrusive_base>;

//////////////////////////
//     DEFINITIONS      //
//////////////////////////

struct base_player : public inventory_holder , public collision_user , public hit_initiator , public hit_receiver {
public:
												base_player						( base_player_creation_params const& params, scheduler& the_scheduler );
	virtual										~base_player					( );

	inline	resources::resource_ptr<interactive_object,resources::unmanaged_intrusive_base>	get_current_active_object		( ) { /* no source */ }
	inline	character_recoil_params const&		get_recoil_params				( ) const { /* no source */ }
	inline	character_dispersion_params const&	get_dispersion_params			( ) const { /* no source */ }
	inline	breath_holding_params const&		get_breath_holding_params		( ) const { /* no source */ }

	inline	resources::resource_ptr<interactive_object,resources::unmanaged_intrusive_base>	target_active_object			( ) const { /* no source */ }
	inline	resources::resource_ptr<interactive_object,resources::unmanaged_intrusive_base>	current_active_object			( ) const { /* no source */ }

	virtual	resources::resource_ptr<damage_model,resources::unmanaged_intrusive_base> const&	damage_model					( ) const override { /* no source */ }

	virtual	animation::skeleton const&			skeleton						( ) const = 0;
	virtual	player_input const&					input							( ) const = 0;
	virtual	float3 const&						position						( ) const = 0;
	virtual	float								get_look_pitch					( ) const = 0;

	virtual	physics::bt_character_controller&	physics_controller				( ) = 0;
	virtual	player_stamina&						stamina							( ) = 0;

	virtual	void								jump							( ) = 0;
	virtual	void								end_jump						( ) = 0;
	virtual	void								crouch							( ) = 0;
	virtual	void								stand_up						( ) = 0;

	virtual	game_team_id						team							( ) const = 0;

	inline	float4x4 const&						get_head_transform				( ) { /* no source */ }

	inline	void								set_movement_speed_factor		( float arg_0 ) { /* no source */ }
	inline	float								get_movement_speed_factor		( ) const { /* no source */ }

			bool								is_alive						( ) const;

	virtual	base_player*						cast_to_base_player				( ) override { /* no source */ }
	virtual	inventory_holder const*				cast_to_inventory_holder		( ) const override { /* no source */ }
	virtual	inventory_holder*					cast_to_inventory_holder		( ) override;

			void								force_animation_selection		( );

	inline	bool								has_been_inserted				( ) const { /* no source */ }

	virtual	void								subscribe_animation_player		(
																					animation::reserved_channel_ids_enum	arg_0,
																					boost::function<enum animation::callback_return_type_enum(animation::animation_callback_params &)> const&	arg_1,
																					pcvoid								arg_2,
																					resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base> const&	arg_3,
																					pcvoid								arg_4
																				) = 0;

	virtual	void								subscribe_animation_player		(
																					pcstr								arg_0,
																					boost::function<enum animation::callback_return_type_enum(animation::animation_callback_params &)> const&	arg_1,
																					pcvoid								arg_2,
																					resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base> const&	arg_3,
																					u8									arg_4,
																					pcvoid								arg_5
																				) = 0;

	virtual	void								unsubscribe_animation_player	( animation::reserved_channel_ids_enum arg_0, pcvoid arg_1 ) = 0;

	virtual	void								unsubscribe_animation_player	( pcstr arg_0, pcvoid arg_1 ) = 0;

	virtual	bool								is_replaying_history			( ) const = 0;

	virtual	void								serialize						( network_core::udp_match_packet& arg_0 ) const { /* no source */ }
	virtual	void								deserialize						( network_core::packet_reader& arg_0 ) { /* no source */ }

	virtual	u32									local_time						( u32 arg_0 ) const = 0;

	virtual	bool								get_animation_playback_state	( pcvoid object, u32 mask, animation::animation_playback_state& result ) const;

	virtual	void								insert_game_world_object		( game_world_object& object ) override;
	virtual	void								remove_game_world_object		( game_world_object& object ) override;

			void								subscribe_on_player_death		( player_death_subscriber* subscriber );
			void								unsubscribe_from_player_death	( player_death_subscriber* subscriber );

	inline	bool								animation_selection_is_forced	( ) const { /* no source */ }
	inline	void								set_force_animation_selection	( bool arg_0 ) { /* no source */ }

	inline	void								send_game_world_objects			( boost::function<network_core::udp_match_packet &()> const& arg_0, boost::function<void(network_core::udp_match_packet &)> const& arg_1 ) const { /* no source */ }
			void								deserialize_game_world_object	( network_core::packet_reader& reader );

	virtual	engine&								get_engine						( ) = 0;
			void								tick_active_object				( );

	inline	intrusive_list<game_world_object,resources::resource_ptr<game_world_object,resources::unmanaged_intrusive_base>,264,threading::single_threading_policy,no_size_policy,no_debug_policy> const&	game_world_objects				( ) const { /* no source */ }

			void								on_player_death					( );
			void								send_game_world_object			( game_world_object const* object, boost::function<network_core::udp_match_packet &()> const& reciver_packet_allocator, boost::function<void(network_core::udp_match_packet &)> const& reciver_enqueuer ) const;

	virtual	animation::animation_player const&	animation_player				( ) const = 0;
	virtual	animation::animation_player&		animation_player				( ) = 0;

	virtual	void								on_before_active_object_changed	( resources::resource_ptr<interactive_object,resources::unmanaged_intrusive_base> const& arg_0, resources::resource_ptr<interactive_object,resources::unmanaged_intrusive_base> const& arg_1 ) const { /* no source */ }


public:
	/* 0x0000 */	/* inventory_holder */
	/* 0x000c */	/* collision_user */
	/* 0x0030 */	/* hit_initiator */
	/* 0x0038 */	/* hit_receiver */
	/* 0x0040 */	resources::resource_ptr<interactive_object,resources::unmanaged_intrusive_base>	m_current_active_object;
	/* 0x0044 */	resources::resource_ptr<interactive_object,resources::unmanaged_intrusive_base>	m_target_active_object;
	/* 0x0048 */	float4x4							m_character_head_transform;
	/* 0x0088 */	intrusive_list<player_death_subscriber,player_death_subscriber *,32,threading::single_threading_policy,no_size_policy,no_debug_policy>	m_player_death_subscribers;
	/* 0x0094 */	character_recoil_params				m_recoil_params;
	/* 0x00a4 */	character_dispersion_params			m_dispersion_params;
	/* 0x00dc */	breath_holding_params				m_breath_holding_params;
	/* 0x0104 */	intrusive_list<game_world_object,resources::resource_ptr<game_world_object,resources::unmanaged_intrusive_base>,264,threading::single_threading_policy,no_size_policy,no_debug_policy>	m_game_world_objects;
	/* 0x0110 */	resources::resource_ptr<damage_model,resources::unmanaged_intrusive_base>	m_damage_model;
	/* 0x0114 */	float								m_movement_speed_factor;
	/* 0x0118 */	bool								m_force_animation_selection;
	/* 0x0119 */	bool								m_is_alive;
	/* 0x011a */	bool								m_is_replaying_history;
	/* 0x011b */	bool								m_has_been_inserted;
}; // struct base_player

STATIC_SIZE_ASSERT(base_player, 0x11C);

} // namespace survarium

#endif // #ifndef BASE_PLAYER_H_INCLUDED
