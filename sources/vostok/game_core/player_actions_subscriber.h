////////////////////////////////////////////////////////////////////////////
//	Created 	: 03.11.2025
////////////////////////////////////////////////////////////////////////////

#ifndef PLAYER_ACTIONS_SUBSCRIBER_H_INCLUDED
#define PLAYER_ACTIONS_SUBSCRIBER_H_INCLUDED

namespace survarium {

struct hit_receiver;

class player_actions_subscriber : private boost::noncopyable { // sushi@TODO: NO_VTABLE?
public:
	enum action {
		run				= 0,
		sprint			= 1,
		jump			= 2,
		shoot			= 3,
		hit				= 4,
		kill			= 5
	};
	virtual	void	on_player_action			( hit_receiver const* receiver, player_actions_subscriber::action action, float param ) = 0;
}; // class player_actions_subscriber

STATIC_SIZE_ASSERT(player_actions_subscriber, 0x4);

} // namespace survarium

#endif // #ifndef PLAYER_ACTIONS_SUBSCRIBER_H_INCLUDED
