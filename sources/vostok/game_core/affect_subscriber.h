// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef AFFECT_SUBSCRIBER_H_INCLUDED
#define AFFECT_SUBSCRIBER_H_INCLUDED

namespace survarium {

enum hit_affects_type_enum;
enum affect_event_type_enum;

struct affect_subscriber : private boost::noncopyable {
public:
	typedef boost::function< void (
		pcstr,
		hit_affects_type_enum,
		affect_event_type_enum
	) > affect_event_callback_type;

public:
	inline explicit affect_subscriber( affect_event_callback_type const& subscription_callback ) :
		subscription_callback( subscription_callback ),
		next( NULL )
	{
	}

	inline affect_subscriber( ) : next( NULL )
	{
	}

public:
	affect_event_callback_type subscription_callback;
	affect_subscriber* next;
}; // struct affect_subscriber

STATIC_SIZE_ASSERT(affect_subscriber, 0x28);

} // namespace survarium

#endif // #ifndef AFFECT_SUBSCRIBER_H_INCLUDED
