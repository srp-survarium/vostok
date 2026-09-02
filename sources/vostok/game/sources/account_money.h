// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef ACCOUNT_MONEY_H_INCLUDED
#define ACCOUNT_MONEY_H_INCLUDED

namespace survarium {

struct account_money {
	inline		account_money	( ) : generic_money( 0 ), premium_money( 0 ), total_skill_points( 0 ) { /* no source */ }

public:
	/* 0x0000 */	u32		generic_money;
	/* 0x0004 */	u32		premium_money;
	/* 0x0008 */	u8		total_skill_points;
}; // struct account_money

STATIC_SIZE_ASSERT(account_money, 0xC);

} // namespace survarium

#endif // #ifndef ACCOUNT_MONEY_H_INCLUDED
