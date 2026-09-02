// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////
#ifndef ANOMALY_STATE_H_INCLUDED
#define ANOMALY_STATE_H_INCLUDED

namespace survarium {

class generic_anomaly_core;
class damage_zone_core;
struct zone_group;

struct anomaly_state {
public:
	// sushi@NOTE: Inlined in `generic_anomaly_core::load`
	inline	explicit	anomaly_state	( generic_anomaly_core* owner ) : owner	( owner ) { }

	/* 0x0000 */	bool						enabled;
	/* 0x0004 */	u32							debug_idx;
	/* 0x0008 */	u32							energy_threshold;
	/* 0x000c */	bool						shoot_trigger;
	/* 0x000d */	bool						zone_activity_trigger;
	/* 0x0010 */	u32							active_time_sec;
	/* 0x0014 */	u32							energy_on_exit;
	/* 0x0018 */	u32							select_priority;
	/* 0x001c */	vector<zone_group *>		groups;
	/* 0x0028 */	generic_anomaly_core*		owner;
	/* 0x002c */	u32							m_finish_time_ms;

	inline	void		on_zone_act		( damage_zone_core* arg_0, hit_receiver* arg_1 ) { /* no source */ }

			void		initialize		( );
			void		execute			( const u32 time_delta_ms, const u32 current_time_ms );
			void		finalize		( );
}; // struct anomaly_state

STATIC_SIZE_ASSERT(anomaly_state, 0x30);

} // namespace survarium

#endif // #ifndef ANOMALY_STATE_H_INCLUDED
