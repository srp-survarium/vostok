////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef PLAYER_DEATH_SUBSCRIBER_H_INCLUDED
#define PLAYER_DEATH_SUBSCRIBER_H_INCLUDED

namespace survarium {

struct player_death_subscriber {
	inline	player_death_subscriber	( ) { /* no source */ }
	inline	player_death_subscriber	( boost::function< void( ) > const& subscription_callback_ ) : subscription_callback( subscription_callback_ ) { }

public:
	/* 0x0000 */	boost::function< void( ) >		subscription_callback;
	/* 0x0020 */	player_death_subscriber*		next;
}; // struct player_death_subscriber

STATIC_SIZE_ASSERT(player_death_subscriber, 0x28);

typedef intrusive_list<
	player_death_subscriber,
	player_death_subscriber *,
	&player_death_subscriber::next,
	threading::single_threading_policy,
	no_size_policy,
	no_debug_policy > player_death_subscriber_list;

} // namespace survarium

#endif // #ifndef PLAYER_DEATH_SUBSCRIBER_H_INCLUDED
