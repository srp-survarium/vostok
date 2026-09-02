// SPDX-License-Identifier: GPL-3.0-or-later

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

// claude@NOTE: forwarding target of base_player::get_animation_playback_state (see below);
// the real callee folded to a bare `ret` and is reconstructed in base_player.cpp.
bool query_animation_playback_state( pcvoid const object, u32 const mask, animation::animation_playback_state result );

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

			bool								is_alive						( ) const { return m_is_alive; }

	virtual	base_player*						cast_to_base_player				( ) override { return this; }
	virtual	inventory_holder*					cast_to_inventory_holder		( ) override { return this; }
	virtual	inventory_holder const*				cast_to_inventory_holder		( ) const override { return this; }

			void								force_animation_selection		( ) { m_force_animation_selection = true; }

	inline	bool								has_been_inserted				( ) const { return m_has_been_inserted; }

	// claude@MATCH: MSVC assigns vtable slots to same-name overloaded virtuals in
	// REVERSE declaration order. Target wants enum-subscribe at vtable+0x4C (lower)
	// and pcstr-subscribe at +0x50, so the enum overload must be declared LAST.
	virtual	void								subscribe_animation_player		(
													pcstr									arg_0,
													boost::function< animation::callback_return_type_enum( animation::animation_callback_params & ) > const& arg_1,
													pcvoid									arg_2,
													resources::managed_resource_ptr const&	arg_3,
													const u8								arg_4,
													pcvoid const							arg_5
												) = 0;

	virtual	void								subscribe_animation_player		(
													animation::reserved_channel_ids_enum	arg_0,
													boost::function< animation::callback_return_type_enum( animation::animation_callback_params & ) > const& arg_1,
													pcvoid									arg_2,
													resources::managed_resource_ptr const&	arg_3,
													pcvoid const							arg_4
												) = 0;

	virtual	void								unsubscribe_animation_player	( pcstr arg_0, pcvoid arg_1 ) = 0;
	virtual	void								unsubscribe_animation_player	( animation::reserved_channel_ids_enum arg_0, pcvoid arg_1 ) = 0;

	virtual	bool								is_replaying_history			( ) const = 0;

	virtual	void								serialize						( network_core::udp_match_packet& arg_0 ) const { /* no source */ }
	virtual	void								deserialize						( network_core::packet_reader& arg_0 )			{ /* no source */ }

	virtual	u32									local_time						( const u32 arg_0 ) const = 0;

	// claude@NOTE: inline COMDAT attributed to this header (target emits it from base_player.h).
	// `object`/`mask` are top-level `const` (QBX/IBE mangle); result is passed BY VALUE to the
	// forwarding helper (reconstructed in base_player.cpp - its real callee folded away).
	virtual	bool								get_animation_playback_state	( pcvoid const object, u32 const mask, animation::animation_playback_state& result ) const
	{
		return query_animation_playback_state( object, mask, result );
	}

	virtual	void								insert_game_world_object		( game_world_object& object ) override { m_game_world_objects.push_back( &object ); }
	virtual	void								remove_game_world_object		( game_world_object& object ) override { m_game_world_objects.erase( &object ); }

			void								subscribe_on_player_death		( player_death_subscriber* subscriber );
			void								unsubscribe_from_player_death	( player_death_subscriber* subscriber );

	inline	bool								animation_selection_is_forced	( ) const { return m_force_animation_selection; }

	inline	void								set_force_animation_selection	( bool force_animation_selection ) { m_force_animation_selection = force_animation_selection; }

	typedef boost::function< network_core::udp_match_packet& ( ) > reciver_packet_allocator_type;
	typedef boost::function< void( network_core::udp_match_packet& ) > reciver_enqueuer_type;

	inline	void								send_game_world_objects			(
													reciver_packet_allocator_type const&	arg_0,
													reciver_enqueuer_type const&			arg_1
												) const { /* no source */ } // sushi@TODO: There is `send_game_world_object`

			void								deserialize_game_world_object	( network_core::packet_reader& reader );

protected:
	virtual	engine&								get_engine						( ) = 0;
			void								tick_active_object				( );

	inline	game_world_object_list const&		game_world_objects				( ) const { return m_game_world_objects; }

protected:
			void								on_player_death					( );

	/* 0x0040 */	interactive_object_ptr				m_current_active_object;
	/* 0x0044 */	interactive_object_ptr				m_target_active_object;
	/* 0x0048 */	float4x4							m_character_head_transform;
private:
	// target mangles ABE (private const) - ?send_game_world_object@..@@ABE..
			void								send_game_world_object			(
													game_world_object const*											object,
													reciver_packet_allocator_type const&							reciver_packet_allocator,
													reciver_enqueuer_type const&								reciver_enqueuer
												) const;
private:

	virtual	animation::animation_player&		animation_player				( ) = 0;
	virtual	animation::animation_player const&	animation_player				( ) const = 0;

	virtual	void								on_before_active_object_changed	( interactive_object_ptr const& arg_0, interactive_object_ptr const& arg_1 ) const { /* no source */ }

	typedef intrusive_list<
		player_death_subscriber,
		player_death_subscriber*,
		&player_death_subscriber::next,
		threading::single_threading_policy,
		no_size_policy,
		no_debug_policy
	> player_death_subscribers_type;

	/* 0x0000 */	/* inventory_holder */
	/* 0x000c */	/* collision_user */
	/* 0x0030 */	/* hit_initiator */
	/* 0x0038 */	/* hit_receiver */
private:
	/* 0x0088 */	player_death_subscribers_type	m_player_death_subscribers;
	/* 0x0094 */	const character_recoil_params		m_recoil_params;
	/* 0x00a4 */	const character_dispersion_params	m_dispersion_params;
	/* 0x00dc */	const breath_holding_params			m_breath_holding_params;
	/* 0x0104 */	game_world_object_list				m_game_world_objects;
	/* 0x0110 */	damage_model_ptr					m_damage_model;
	/* 0x0114 */	float								m_movement_speed_factor;
	/* 0x0118 */	bool								m_force_animation_selection;
protected:
	/* 0x0119 */	bool								m_is_alive;
	/* 0x011a */	bool								m_is_replaying_history;
	/* 0x011b */	bool								m_has_been_inserted;

	friend class player;
}; // struct base_player

STATIC_SIZE_ASSERT(base_player, 0x11C);

} // namespace survarium

#endif // #ifndef BASE_PLAYER_H_INCLUDED
