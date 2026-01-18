////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef BASE_PLAYER_H_INCLUDED
#define BASE_PLAYER_H_INCLUDED

#include <vostok/game_core/inventory_holder.h>
#include <vostok/game_core/collision_user.h>
#include <vostok/game_core/hit_initiator.h>
#include <vostok/game_core/hit_receiver.h>

#include <vostok/game_core/game_world_object.h>
#include <vostok/game_core/interactive_object.h>
#include <vostok/game_core/damage_model.h>
#include <vostok/game_core/game_team_id.h>
#include <vostok/game_core/player_death_subscriber.h>

#include <vostok/game_core/character_dispersion_params.h>
#include <vostok/game_core/character_recoil_params.h>
#include <vostok/game_core/breath_holding_params.h>

#include <vostok/game_core/scheduler.h>

#include <vostok/animation/api.h>
#include <vostok/animation/animation_player.h>

namespace vostok {
namespace network_core {
	class packet_reader;
	class udp_match_packet;
}
namespace physics {
	class bt_character_controller;
}
namespace animation {
	class skeleton;
	struct animation_callback_params;
}
}

namespace survarium {

struct engine;
class game_world_object;
class player_stamina;
struct player_input;
struct base_player_creation_params;

// sushi@TODO: <0x122e20>|0x000:'149'

struct base_player : public inventory_holder , public collision_user , public hit_initiator , public hit_receiver {
public:
	explicit									base_player						( base_player_creation_params const& params, survarium::scheduler& the_scheduler );
	virtual										~base_player					( );

	inline	interactive_object_ptr				get_current_active_object		( ) { return m_current_active_object; }
	inline	character_recoil_params const&		get_recoil_params				( ) const { return m_recoil_params; }
	inline	character_dispersion_params const&	get_dispersion_params			( ) const { return m_dispersion_params; }
	inline	breath_holding_params const&		get_breath_holding_params		( ) const { return m_breath_holding_params; }

	inline	interactive_object_ptr				target_active_object			( ) const { return m_target_active_object; }
	inline	interactive_object_ptr				current_active_object			( ) const { return m_current_active_object; }	// sushi@NOTE: There is also `get_current_active_object`.

	virtual	damage_model_ptr const&				damage_model					( ) const override { return m_damage_model; }

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

	inline	float4x4 const&						get_head_transform				( ) { return m_character_head_transform; }

	inline	void								set_movement_speed_factor		( float movement_speed_factor ) { m_movement_speed_factor = movement_speed_factor; }
	inline	float								get_movement_speed_factor		( ) const { return m_movement_speed_factor; }

	// STATE[STUB]
			bool								is_alive						( ) const { return m_is_alive; }

	virtual	base_player*						cast_to_base_player				( ) override { return this; }
	virtual	inventory_holder const*				cast_to_inventory_holder		( ) const override { return this; }
	// STATE[STUB]
	virtual	inventory_holder*					cast_to_inventory_holder		( ) override { return this; }

	// STATE[STUB]
			void								force_animation_selection		( ) { m_force_animation_selection = true; }

	inline	bool								has_been_inserted				( ) const { return m_has_been_inserted; }

public:
	typedef boost::function< animation::callback_return_type_enum( animation::animation_callback_params & ) > animation_callback;

	virtual	void								subscribe_animation_player		(
													animation::reserved_channel_ids_enum	arg_0,
													animation_callback const&				arg_1,
													pcvoid									arg_2,
													resources::managed_resource_ptr const&	arg_3,
													pcvoid									arg_4
												) = 0;

	virtual	void								subscribe_animation_player		(
													pcstr									arg_0,
													animation_callback const&				arg_1,
													pcvoid									arg_2,
													resources::managed_resource_ptr const&	arg_3,
													u8										arg_4,
													pcvoid									arg_5
												) = 0;

	virtual	void								unsubscribe_animation_player	( animation::reserved_channel_ids_enum arg_0, pcvoid arg_1 ) = 0;
	virtual	void								unsubscribe_animation_player	( pcstr arg_0, pcvoid arg_1 ) = 0;

	virtual	bool								is_replaying_history			( ) const = 0;

	virtual	void								serialize						( network_core::udp_match_packet& arg_0 ) const { /* no source */ }
	virtual	void								deserialize						( network_core::packet_reader& arg_0 )			{ /* no source */ }

	virtual	u32									local_time						( u32 arg_0 ) const = 0; // sushi@TODO

	// STATE[STUB]
	virtual	bool								get_animation_playback_state	( pcvoid object, u32 mask, animation::animation_playback_state& result ) const { VOSTOK_UNREFERENCED_PARAMETERS( object, mask, result ); VOSTOK_UNREACHABLE_CODE( ); }

	// STATE[STUB]
	virtual	void								insert_game_world_object		( game_world_object& object ) override { m_game_world_objects.push_back( &object ); }
	// STATE[STUB]
	virtual	void								remove_game_world_object		( game_world_object& object ) override { m_game_world_objects.erase( &object ); }

			void								subscribe_on_player_death		( player_death_subscriber* subscriber );
			void								unsubscribe_from_player_death	( player_death_subscriber* subscriber );

	inline	bool								animation_selection_is_forced	( ) const							{ return m_force_animation_selection; }
	inline	void								set_force_animation_selection	( bool force_animation_selection )	{ m_force_animation_selection = force_animation_selection; } // sushi@TODO: Repeat

	inline	void								send_game_world_objects			(
													boost::function< network_core::udp_match_packet& ( ) > const&		arg_0,
													boost::function< void ( network_core::udp_match_packet& ) > const&	arg_1
												) const { /* no source */ } // sushi@TODO: There is `send_game_world_object`

			void								deserialize_game_world_object	( network_core::packet_reader& reader );

	virtual	engine&								get_engine						( ) = 0;
			void								tick_active_object				( );

	inline	game_world_object_list const&		game_world_objects				( ) const { return m_game_world_objects; }

			void								on_player_death					( );
			void								send_game_world_object			(
													game_world_object const*											object,
													boost::function< network_core::udp_match_packet& ( ) > const&		reciver_packet_allocator,
													boost::function< void( network_core::udp_match_packet& ) > const&	reciver_enqueuer
												) const;

	virtual	animation::animation_player const&	animation_player				( ) const = 0;
	virtual	animation::animation_player&		animation_player				( ) = 0;

	virtual	void								on_before_active_object_changed	( interactive_object_ptr const& arg_0, interactive_object_ptr const& arg_1 ) const { /* no source */ }

public:
	/* 0x0000 */	/* inventory_holder */
	/* 0x000c */	/* collision_user */
	/* 0x0030 */	/* hit_initiator */
	/* 0x0038 */	/* hit_receiver */
	/* 0x0040 */	interactive_object_ptr				m_current_active_object;
	/* 0x0044 */	interactive_object_ptr				m_target_active_object;
	/* 0x0048 */	float4x4							m_character_head_transform;
	/* 0x0088 */	player_death_subscriber_list		m_player_death_subscribers;
	/* 0x0094 */	character_recoil_params				m_recoil_params;
	/* 0x00a4 */	character_dispersion_params			m_dispersion_params;
	/* 0x00dc */	breath_holding_params				m_breath_holding_params;
	/* 0x0104 */	game_world_object_list				m_game_world_objects;
	/* 0x0110 */	damage_model_ptr					m_damage_model;
	/* 0x0114 */	float								m_movement_speed_factor;
	/* 0x0118 */	bool								m_force_animation_selection;
	/* 0x0119 */	bool								m_is_alive;
	/* 0x011a */	bool								m_is_replaying_history;
	/* 0x011b */	bool								m_has_been_inserted;
}; // struct base_player

STATIC_SIZE_ASSERT(base_player, 0x11C);

} // namespace survarium

#endif // #ifndef BASE_PLAYER_H_INCLUDED
