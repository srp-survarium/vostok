////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_base_state.h>
#include <vostok/game_core/weapon_core.h>		// m_weapon.deserializing() inlines a member read
#include <vostok/game_core/base_player.h>
#include <vostok/network_core/udp_match_packet.h>
#include <vostok/network_core/packet_reader.h>

namespace survarium {

// STATE[100%|DONE]
weapon_core_base_state::weapon_core_base_state( weapon_core& weapon, bool serialize_animation_state )
	:	m_weapon						( weapon ),
		m_is_firing_ptr					( NULL ),
		m_body_part_mask_for_user		( animation::body_part_whole_body ),	// -1
		m_is_ready_to_be_deactivated	( false ),
		m_animation_has_been_ended		( false ),
		m_serialize_animation_state		( serialize_animation_state )
{
}

// STATE[100%|DONE]
bool weapon_core_base_state::deserializing( ) const
{
	return m_weapon.deserializing( );
}

// STATE[75.60%|PARTIAL]: serialize_animation_state guards a get_animation_playback_state probe
// (mask -3) then appends interval_id (u32) + interval_time (float). Restructured to the target's
// 10-stmt skeleton: compiled-out ASSERT after the decl (the byte+lea+empty_stub triple at
// t.0x02f <0xc>), success declared AT the probe call (target locals order playback_state, user,
// success; no `= false` init row), and the !success arm written as the two explicit member
// resets the target line table shows (two separate rows, t.<0x7>+<0xd>) - NOT a
// playback_state.reset() call.
void weapon_core_base_state::serialize( network_core::udp_match_packet& packet ) const
{
	if ( m_serialize_animation_state )
	{
		animation::animation_playback_state	playback_state;
		ASSERT( UNKNOWN_EXPRESSION );
		base_player const&	user	= *m_weapon.get_user( );

		bool success	= user.get_animation_playback_state( &m_weapon, animation::body_part_whole_body_but_hands, playback_state );
		if ( !success )
		{
			playback_state.interval_id		= 0;
			playback_state.interval_time	= 0.0f;
		}

		packet.append	( playback_state.interval_id );
		packet.append	( playback_state.interval_time );
	}

	// STRUCTURE DIFF: target 10 stmts / base 10 stmts (post-fix; was 10/9)
	// b.diff   |t.addr  |b.addr  |t.sz|b.sz|b.ln|b.code
	// ---------+--------+--------+----+----+----+------
	// SIZE +0x1|0x6ecf1b|0x47320b|0x11|0x12|0   |base_player const&	user	= *m_weapon.get_user( );
	// SIZE +0x8|0x6ecf68|0x473259|0xc |0x14|+9  |packet.append	( playback_state.interval_id );
	// SIZE +0x9|0x6ecf74|0x47326d|0xf |0x18|+10 |packet.append	( playback_state.interval_time );
	// VERDICT: STRUCTURE MATCH (fixed quantity) - 10/10 after recovering the ASSERT, folding the
	//   success decl into the probe call, and writing the !success arm as the two explicit member
	//   resets; residual SIZE rows are the target's LTCG custom-convention get_user call and the
	//   inlined packet<T>::append pair, non-steerable.
}

// STATE[44.05%|PARTIAL]: when serialize_animation_state, reads interval_id (u32) + interval_time
// (float) back into m_animation_playback_state. Matches rva 0x6ece90.
void weapon_core_base_state::deserialize( network_core::packet_reader& reader )
{
	if ( m_serialize_animation_state )
	{
		m_animation_playback_state.interval_id		= reader.r< u32 >( );
		m_animation_playback_state.interval_time	= reader.r< float >( );
	}

	// STRUCTURE DIFF: target 3 stmts / base 3 stmts
	// b.diff   |t.addr  |b.addr  |t.sz|b.sz|b.ln|b.code
	// ---------+--------+--------+----+----+----+------
	// SIZE +0xb|0x6ecea7|0x4732a7|0x11|0x1c|0   |m_animation_playback_state.interval_id		= reader.r< u32 >( );
	// SIZE +0xd|0x6eceb8|0x4732c3|0x13|0x20|+1  |m_animation_playback_state.interval_time	= reader.r< float >( );
	// VERDICT: STRUCTURE MATCH (shape ok) - both sides if-guarded 2 reads; SIZE rows are the
	//   target's LTCG-folded r<u32>/r<float> calls vs the base's inlined wrapper + inner r() call,
	//   non-steerable.
}

} // namespace survarium
