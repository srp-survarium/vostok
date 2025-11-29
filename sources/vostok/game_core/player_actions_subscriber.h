////////////////////////////////////////////////////////////////////////////
//	Created 	: 03.11.2025
////////////////////////////////////////////////////////////////////////////

#ifndef PLAYER_ACTIONS_SUBSCRIBER_H_INCLUDED
#define PLAYER_ACTIONS_SUBSCRIBER_H_INCLUDED

namespace survarium {

class player_actions_subscriber : public boost::noncopyable { // sushi@TODO: NO_VTABLE? 
public:
	virtual	void	on_player_action			( hit_receiver const* arg_0, player_actions_subscriber::action arg_1, float arg_2 ) = 0;
}; // class player_actions_subscriber

STATIC_SIZE_ASSERT(player_actions_subscriber, 0x4);

} // namespace survarium

#endif // #ifndef PLAYER_ACTIONS_SUBSCRIBER_H_INCLUDED
