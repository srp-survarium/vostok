// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef PLAYER_DEATH_SUBSCRIBER_H_INCLUDED
#define PLAYER_DEATH_SUBSCRIBER_H_INCLUDED

namespace survarium {

struct player_death_subscriber {
	typedef boost::function< void( ) > player_death_callback_type;

	inline	player_death_subscriber	( player_death_callback_type const& subscription_callback_ ) : subscription_callback( subscription_callback_ ) { }
	inline	player_death_subscriber	( ) { /* no source */ }

	/* 0x0000 */	player_death_callback_type		subscription_callback;
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
