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
// instant_aim_end). Residual is the inline-vs-call of animation_playback_state::reset(): the
// target keeps reset out-of-line (`add eax,120h; call reset`, callee uses an LTCG custom
// `this`-in-EAX frameless convention @0x087f60), while our /Od /Ob2 /GL link INLINES reset's
// member-zeroing body here (`mov [ecx],0; movss [edx+4],xmm0`, frame grows to sub esp,8). Not
// steerable from this source: own-header decl/def split, __declspec(noinline), &reset
// address-escape and multiple real callers all still inline (see the .md). Faithful source
// (reset implemented) is kept over the empty-stub elision that coincidentally scored 83.33%.
void weapon_core_aimed_state_base::finalize( )
{
	m_animation_playback_state.reset( );
	m_weapon.instant_aim_end( );

	// STRUCTURE DIFF[target 0x6ea5c0 | base 0x44f5e0]: target 4 / base 3 stmts
	// .. same ..
	// 0x007 <0xd> | --         | L28   ONLY target   ; m_animation_playback_state.reset()
	// 0x014 <0x1e>| 0x007 <0x1e>| m_weapon.instant_aim_end( );
	// .. same ..
	// ; aligned 3, size-diffs 0, quantity-diffs 1
	// QUANTITY diff: target keeps reset() out-of-line (add eax,120h; call reset @0x087f60,
	// LTCG this-in-EAX frameless), our /Od /Ob2 /GL link elides/inlines reset to nothing,
	// dropping the statement. Source is faithful (reset + virtual instant_aim_end); the
	// divergence is inline-vs-call codegen, not steerable from this source (see the .md).
}

} // namespace survarium
