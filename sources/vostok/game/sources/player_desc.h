// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef PLAYER_DESC_H_INCLUDED
#define PLAYER_DESC_H_INCLUDED

#include <vostok/resources_unmanaged_resource.h>

namespace survarium {

struct player_desc {
	inline		player_desc	( ) :
		player			( ),
		is_connected	( false )
	{
	}

public:
	/* 0x0000 */	resources::unmanaged_resource_ptr		player;
	/* 0x0004 */	bool									is_connected;
}; // struct player_desc

STATIC_SIZE_ASSERT(player_desc, 0x8);

} // namespace survarium

#endif // #ifndef PLAYER_DESC_H_INCLUDED
