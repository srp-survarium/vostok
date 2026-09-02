// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef VICTORY_ITEMS_CONTAINER_CORE_H_INCLUDED
#define VICTORY_ITEMS_CONTAINER_CORE_H_INCLUDED

#include <vostok/game_core/game_team_id.h>
#include <vostok/game_core/usable_object.h>

namespace survarium {

class victory_item_core;

class victory_items_container_core : public usable_object {
public:
	typedef usable_object super;

			explicit				victory_items_container_core	( );

	virtual	void					load							( configs::binary_config_value const& cfg ) override;

	virtual	bool					use_initialize					( usable_object_user_data* user ) override;
	virtual	bool					use_execute						( usable_object_user_data* user ) override;
	virtual	bool					use_finalize					( usable_object_user_data* user ) override;
	virtual	pcstr					use_info						( usable_object_user_data* user ) override;

	virtual	void					put_item						( victory_item_core* item );
	virtual	victory_item_core*		take_item						( );

	virtual	game_team_id			team							( ) { return m_owner_team; }
	inline	bool					empty							( ) { return m_victory_items.empty( ); }
	inline	u8						id								( ) { return m_container_id; }

public:
	// network_client clears m_victory_items directly (on_world_sync_request);
	// PDB does not record friendship, codegen-neutral
	friend class network_client;
	// victory_items_container::use_info reads m_victory_items directly.
	friend class victory_items_container;

	/* 0x0000 */	/* usable_object */
	/* 0x0020 */	vectora<victory_item_core *>	m_victory_items;
protected:
	/* 0x0030 */	game_team_id					m_owner_team;
	/* 0x0034 */	u8								m_container_id;
}; // class victory_items_container_core

STATIC_SIZE_ASSERT(victory_items_container_core, 0x38);

} // namespace survarium

#endif // #ifndef VICTORY_ITEMS_CONTAINER_CORE_H_INCLUDED
