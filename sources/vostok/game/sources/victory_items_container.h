// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef VICTORY_ITEMS_CONTAINER_H_INCLUDED
#define VICTORY_ITEMS_CONTAINER_H_INCLUDED

#include <vostok/game_core/victory_items_container_core.h>

namespace survarium {

class base_game_scene;

class victory_items_container : public victory_items_container_core {
	typedef victory_items_container_core super;

public:
			explicit	victory_items_container	( base_game_scene& w );

	virtual	void		load					( configs::binary_config_value const& cfg ) override;

	virtual	pcstr		use_info				( usable_object_user_data* user ) override;


private:
	/* 0x0000 */	/* victory_items_container_core */
	/* 0x0038 */	base_game_scene&	m_game_scene;
}; // class victory_items_container

STATIC_SIZE_ASSERT(victory_items_container, 0x3C);

} // namespace survarium

#endif // #ifndef VICTORY_ITEMS_CONTAINER_H_INCLUDED
