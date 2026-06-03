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

	// FUNCTION BODY (kept: this is a PARTIAL match, not 100%)
	// target 0x6ea5c0:
	//   mov eax,[ebp-4]; add eax,120h; call reset            ; m_animation_playback_state.reset() - INLINED in base
	//   mov eax,[ebp-4]; mov ecx,[eax+128h]; mov edx,[ebp-4]; mov eax,[edx+128h]
	//   mov edx,[ecx]; mov ecx,eax; mov eax,[edx+90h]; call eax   ; m_weapon.instant_aim_end() (virtual)
	// <0x6ea5c7>|0x007|+0x00d:'28'
	// <0x6ea5d4>|0x014|+0x01e:'29'
	// ******
}

} // namespace survarium
