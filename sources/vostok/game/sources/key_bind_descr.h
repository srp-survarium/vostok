////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef KEY_BIND_DESCR_H_INCLUDED
#define KEY_BIND_DESCR_H_INCLUDED

#include "action_type.h"
#include "game_action_id.h"
#include "keybind_group_type.h"

namespace survarium {

struct key_bind_descr {

public:
	/* 0x0000 */	game_action_id			action_id;
	/* 0x0004 */	pcstr					str_description;
	/* 0x0008 */	keybind_group_type		group_id;
	/* 0x000c */	action_type				type;
	/* 0x0010 */	fixed_string< 32 >		new_binded_key;
	/* 0x003c */	fixed_string< 32 >		old_binded_key;
}; // struct key_bind_descr

STATIC_SIZE_ASSERT(key_bind_descr, 0x68);

} // namespace survarium

#endif // #ifndef KEY_BIND_DESCR_H_INCLUDED
