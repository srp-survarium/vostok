////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

// Game-module /OPT:REF reachability anchor for player_logic_preview_state.
// The state is constructed by the weapon-user animation-selector machinery, which
// is still a carcass, so no reachable call graph touches it and /OPT:REF would strip
// the ctor / vtable / selected_animations. The guarded construction (never run)
// keeps the out-of-line ctor/dtor/vtable; selected_animations is address-taken.
//
// Retire once the weapon-user animation selector reaches this state for real.

#include "pch.h"

#include "player_logic_preview_state.h"

namespace survarium {

void use_game_player_logic( )
{
	static volatile bool s_run = false;
	if ( !s_run )
		return;

	// placeholders sourced through volatile pointers so LTCG cannot const-fold the
	// null args into the carcass ctor (a constant arg folds the match away).
	static resources::managed_resource_ptr* volatile	s_animations	= 0;
	static u32 volatile									s_count			= 0;
	weapon_user_animations_selector&					owner			= *( weapon_user_animations_selector* )NULL;

	// constructing the object emits the vtable, which keeps the virtual
	// selected_animations (and the ctor/dtor) past /OPT:REF.
	player_logic_preview_state	state( s_animations, s_count, owner );
}

} // namespace survarium
