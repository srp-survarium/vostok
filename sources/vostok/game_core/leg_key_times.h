// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef LEG_KEY_TIMES_H_INCLUDED
#define LEG_KEY_TIMES_H_INCLUDED

namespace survarium {

struct leg_key_times {
	/* 0x0000 */	float		pre_land_time;
	/* 0x0004 */	float		strike_time;
	/* 0x0008 */	float		land_time;
	/* 0x000c */	float		stance_time;
	/* 0x0010 */	float		lift_time;
	/* 0x0014 */	float		liftoff_time;
	/* 0x0018 */	float		post_lift_time;
}; // struct leg_key_times

STATIC_SIZE_ASSERT(leg_key_times, 0x1C);

} // namespace survarium

#endif // #ifndef LEG_KEY_TIMES_H_INCLUDED
