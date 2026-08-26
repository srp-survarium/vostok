////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef ZONE_GROUP_H_INCLUDED
#define ZONE_GROUP_H_INCLUDED

#include <vostok/game_core/anomaly_state.h>

namespace survarium {

class damage_zone_core;
struct hit_receiver;
struct anomaly_state;

struct zone_group {
public:
	// sushi@NOTE: Inlined in `generic_anomaly_core::load`
	inline	explicit					zone_group	( anomaly_state* owner ) : owner( owner ) { }

	/* 0x0000 */	bool								enabled;
	/* 0x0004 */	u32									max_charged_count;
	/* 0x0008 */	u32									recharge_time_sec;

	struct zone_wrapper {
		/* 0x0000 */	damage_zone_core*		zone;
		/* 0x0004 */	bool					active;
	}; // struct zone_group::zone_wrapper

	/* 0x000c */	vector< zone_group::zone_wrapper >	zones;
	/* 0x0018 */	anomaly_state*						owner;

	inline	generic_anomaly_core*		core		( ) { return owner->owner; }

			void						on_zone_act	( damage_zone_core* zone, hit_receiver* receiver );
			void						recharge	( );
	/* 0x001c */	u32									charged_count;
	/* 0x0020 */	u32									next_recharge_time;

			void						initialize	( );
			void						execute		( const u32 time_delta_ms, const u32 current_time_ms );
			void						finalize	( );
}; // struct zone_group

STATIC_SIZE_ASSERT(zone_group, 0x24);

} // namespace survarium

#endif // #ifndef ZONE_GROUP_H_INCLUDED
