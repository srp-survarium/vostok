// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef SKILL_BOOSTER_H_INCLUDED
#define SKILL_BOOSTER_H_INCLUDED

namespace survarium {

struct skill_booster {
	explicit	skill_booster	( ) : id ( 0 ), value ( 0 ) {}

	/* 0x0000 */	u8			id;
	/* 0x0004 */	float		value;
}; // struct skill_booster

STATIC_SIZE_ASSERT(skill_booster, 0x8);

} // namespace survarium

#endif // #ifndef SKILL_BOOSTER_H_INCLUDED
