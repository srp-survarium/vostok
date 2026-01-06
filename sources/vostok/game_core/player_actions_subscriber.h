////////////////////////////////////////////////////////////////////////////
//	Created 	: 03.11.2025
////////////////////////////////////////////////////////////////////////////

#ifndef PLAYER_ACTIONS_SUBSCRIBER_H_INCLUDED
#define PLAYER_ACTIONS_SUBSCRIBER_H_INCLUDED

namespace survarium {

struct hit_receiver;

class player_actions_subscriber : public boost::noncopyable { // sushi@TODO: NO_VTABLE?
public:
	// sushi@NOTE: Based on `generic_anomaly_core::on_player_action`.
	enum action {
		walk			= 0,
		run				= 1,
		sprint			= 2,
		jump			= 3,
		shoot			= 4,
		character_hit	= 5
	};

	/*
	// sushi@TODO: Might be completely different enum.
	// Can figure out either from usage or from other debug symbols.
	// Definition in IDA:
	// `typedef survarium::action_type_enum survarium::player_actions_subscriber::action;`
 	enum action {
		none                     = 0,
		move_forward             = 1 << 0,
		move_backward            = 1 << 1,
		strafe_left              = 1 << 2,
		strafe_right             = 1 << 3,
		jump                     = 1 << 4,
		shoot                    = 1 << 5,
		reload                   = 1 << 6,
		aim                      = 1 << 7,
		crouch                   = 1 << 8,
		sprint                   = 1 << 9,
		set_next_fire_queue_type = 1 << 10,
		set_next_ammo_type       = 1 << 11,
		weapon_1                 = 1 << 12,
		weapon_2                 = 1 << 13,
		quick_action_1_start     = 1 << 14,
		quick_action_1_end       = 1 << 15,
		quick_action_2_start     = 1 << 16,
		quick_action_2_end       = 1 << 17,
		quick_action_3_start     = 1 << 18,
		quick_action_3_end       = 1 << 19,
		quick_action_4_start     = 1 << 20,
		quick_action_4_end       = 1 << 21,
		quick_action_5_start     = 1 << 22,
		quick_action_5_end       = 1 << 23,
		quick_action_6_start     = 1 << 24,
		quick_action_6_end       = 1 << 25,
		back_slot_action         = 1 << 26,
		hold_breath              = 1 << 27,
		use                      = 1 << 28,
		throw_missile_weapon     = 1 << 29,
		drop                     = 1 << 30
	};
	*/
	virtual	void	on_player_action			( hit_receiver const* receiver, player_actions_subscriber::action action, float param ) = 0;
}; // class player_actions_subscriber

STATIC_SIZE_ASSERT(player_actions_subscriber, 0x4);

} // namespace survarium

#endif // #ifndef PLAYER_ACTIONS_SUBSCRIBER_H_INCLUDED
