// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef GAME_ACTION_DESCR_H_INCLUDED
#define GAME_ACTION_DESCR_H_INCLUDED

#include "game_action_id.h"
#include "toggle_action_enum.h"

namespace survarium {

struct game_action_descr {

public:
	/* 0x0000 */	pcstr					action_name;
	/* 0x0004 */	game_action_id			id;
	/* 0x0008 */	s32						key_group;
	/* 0x000c */	toggle_action_enum		actions_mask_type;
	/* 0x0010 */	pcstr					default_key;
}; // struct game_action_descr

STATIC_SIZE_ASSERT(game_action_descr, 0x14);

} // namespace survarium

#endif // #ifndef GAME_ACTION_DESCR_H_INCLUDED
