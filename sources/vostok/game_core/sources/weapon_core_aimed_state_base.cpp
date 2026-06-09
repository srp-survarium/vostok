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
// STRUCTURE DIFF[target 0x6ea5c0 | base 0x44f700]: target 2 / base 2 stmts
// 0x007 <0xd> | 0x009 <0x24> | m_animation_playback_state.reset( );   SIZE
// .. same ..
// ; aligned 1, size-diffs 1, quantity-diffs 0
// VERDICT: STRUCTURE MATCH (shape ok) - both stmts present; sole SIZE is the reset() inline-vs-call: target keeps reset out-of-line (`add eax,120h; call reset`, LTCG this-in-EAX frameless callee @0x087f60), our /GL link inlines reset's member-zeroing body. Confirmed non-steerable (decl/def split, noinline, address-escape all still inline). trail: weapon_core_aimed_state_base.md
void weapon_core_aimed_state_base::finalize( )
{
	m_animation_playback_state.reset( );
	m_weapon.instant_aim_end( );
}

} // namespace survarium
