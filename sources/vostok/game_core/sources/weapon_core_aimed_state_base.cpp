////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_aimed_state_base.h>
#include <vostok/game_core/weapon_core.h>

namespace survarium {

// STATE[100%|DONE]
weapon_core_aimed_state_base::weapon_core_aimed_state_base( weapon_core& weapon ) :
	weapon_core_base_state( weapon, false )
{
}

// STATE[100%|DONE]
void weapon_core_aimed_state_base::initialize( )
{
	m_weapon.instant_aim_start( );
}

// STATE[54.39%|PARTIAL]: source is correct (m_animation_playback_state.reset() + virtual
// instant_aim_end). Residual is the inline-vs-call of animation_playback_state::reset().
void weapon_core_aimed_state_base::finalize( )
{
	m_animation_playback_state.reset( );
	m_weapon.instant_aim_end( );

	// STRUCTURE DIFF: target 2 stmts / base 2 stmts
	// SIZE +0x17 | 32 | m_animation_playback_state.reset( );
	// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is reset() out-of-line in target (LTCG this-in-EAX frameless callee) vs inlined member-zeroing in base; confirmed non-steerable (decl/def split, noinline, address-escape all tried). trail: weapon_core_aimed_state_base.md
}

} // namespace survarium
