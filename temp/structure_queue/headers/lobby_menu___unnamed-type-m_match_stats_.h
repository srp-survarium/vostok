////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef LOBBY_MENU__H_INCLUDED
#define LOBBY_MENU__H_INCLUDED

namespace survarium {

struct lobby_menu::< unnamed-type-m_match_stats > {

public:
	/* 0x0000 */	u32		last_match_exp_delta;
	/* 0x0004 */	u32		last_match_r1_delta;
	/* 0x0008 */	u32		last_match_r2_delta;
	/* 0x000c */	u32		last_match_r3_delta;
	/* 0x0010 */	u32		last_match_r4_delta;
	/* 0x0014 */	u32		last_match_r5_delta;
	/* 0x0018 */	u32		last_match_money_delta;
}; // struct lobby_menu::< unnamed-type-m_match_stats >

STATIC_SIZE_ASSERT(lobby_menu::< unnamed-type-m_match_stats >, 0x1C);

} // namespace survarium

#endif // #ifndef LOBBY_MENU__H_INCLUDED
