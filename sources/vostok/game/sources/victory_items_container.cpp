////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "victory_items_container.h"
#include <vostok/game_core/usable_object_user_data.h>
#include <vostok/game_core/collision_user.h>
#include <vostok/game_core/base_player.h>
#include <vostok/game_core/inventory_holder.h>
#include <vostok/game_core/inventory.h>

namespace survarium {

// claude@NOTE: structure correct (base-ctor + m_game_scene(w) ref init + compiler
// vtable stores). Residual is the anchor-sole-caller convention wall
// (anchor-sole-caller-convention.md): the only reachable construction is the
// /OPT:REF anchor (anchor_game_victory_item.cpp), so LTCG specialises the ctor to
// read the scene from the anchor's static `s_scene` (`mov edi,[s_scene]`, `ret`)
// instead of the real `[esp+4]` thiscall param (`ret 4`). Cannot take `&C::C` for the
// member-fn-ptr sink fix; pairs at the real convention once create_game_objects
// (project_cooker_simple, parked) constructs it for real.
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

	if ( player )
	{
		game_team_id		team			= player->team( );
		victory_item_core*	victory_item	= holder->inventory( ).get_victory_item( );
		if ( team == m_owner_team )
		{
			if ( victory_item )
				return "st_put_item";
		}
		else if ( !victory_item && m_victory_items.size( ) )
			return "st_thief_item";
	}

	return "";
}


} // namespace survarium
