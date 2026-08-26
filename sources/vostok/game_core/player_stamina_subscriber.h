////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef PLAYER_STAMINA_SUBSCRIBER_H_INCLUDED
#define PLAYER_STAMINA_SUBSCRIBER_H_INCLUDED

namespace survarium {

struct player_stamina_subscriber {
public:
	typedef boost::function< void( ) > stamina_event_callback_type;

	inline	explicit	player_stamina_subscriber	( stamina_event_callback_type const& subscription_callback_ ) : subscription_callback( subscription_callback_ ) { }
	inline	explicit	player_stamina_subscriber	( ) { }

	/* 0x0000 */	stamina_event_callback_type		subscription_callback;
	/* 0x0020 */	player_stamina_subscriber*		next;
}; // struct player_stamina_subscriber

STATIC_SIZE_ASSERT(player_stamina_subscriber, 0x28);

typedef intrusive_list<
	player_stamina_subscriber,
	player_stamina_subscriber *,
	&player_stamina_subscriber::next,
	threading::mutex,
	size_policy,
	no_debug_policy> player_stamina_subscriber_list;

} // namespace survarium

#endif // #ifndef PLAYER_STAMINA_SUBSCRIBER_H_INCLUDED
