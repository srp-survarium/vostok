// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "victory_items_container.h"
#include <vostok/game_core/usable_object_user_data.h>
#include <vostok/game_core/collision_user.h>
#include <vostok/game_core/base_player.h>
#include <vostok/game_core/inventory_holder.h>
#include <vostok/game_core/inventory.h>

namespace survarium {

victory_items_container::victory_items_container( base_game_scene& w ) :
	m_game_scene( w )
{
}

void victory_items_container::load( configs::binary_config_value const& cfg )
{
	victory_items_container_core::load( cfg );
}

pcstr victory_items_container::use_info( usable_object_user_data* user )
{
	base_player*		player	= user->owner->cast_to_base_player( );
	inventory_holder*	holder	= user->owner->cast_to_inventory_holder( );

	if ( !player )
		return "";

	game_team_id		team			= player->team( );
	victory_item_core*	victory_item	= holder->inventory( ).get_victory_item( );
	if ( team == m_owner_team && victory_item )
		return "st_put_item";
	if ( team != m_owner_team && !victory_item && m_victory_items.size( ) > 0 )
		return "st_thief_item";

	return "";
}


} // namespace survarium
