// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef GAME_MAP_DESCRIPTION_H_INCLUDED
#define GAME_MAP_DESCRIPTION_H_INCLUDED

namespace survarium {

struct game_map_description {
public:
	/* 0x0000 */	u32		id;
	/* 0x0004 */	char	name[128];
	/* 0x0084 */	char	description[128];

	inline	bool	operator==	( const u32 arg_0 ) const { /* no source */ return id == arg_0; }
}; // struct game_map_description

STATIC_SIZE_ASSERT(game_map_description, 0x104);

} // namespace survarium

#endif // #ifndef GAME_MAP_DESCRIPTION_H_INCLUDED
