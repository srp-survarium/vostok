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

// STATE[PARTIAL]: serialize_animation_state guards a get_animation_playback_state probe
// (mask -3) then appends interval_id (u32) + interval_time (float). The lea/&success stub
// and the const interval_time sentinel resist a byte-exact init; logical shape matches rva 0x6ecee0.
void weapon_core_base_state::serialize( network_core::udp_match_packet& packet ) const
{
	if ( m_serialize_animation_state )
	{
		animation::animation_playback_state	playback_state;
		bool								success	= false;

		base_player const&	user	= *m_weapon.get_user( );
		success	= user.get_animation_playback_state( &m_weapon, animation::body_part_whole_body_but_hands, playback_state );
		if ( !success )
			playback_state.reset( );

		packet.append	( playback_state.interval_id );
		packet.append	( playback_state.interval_time );
	}

	// STRUCTURE DIFF[target 0x6ecee0 | base 0x4604b0]: target 10 / base 9 stmts
	//   3: 0x02f <0xc> | 0x02f <0x4> | bool								success	= false;   SIZE
	//   4: 0x03b <0x11> | --          | L39   ONLY target
	//   5: --          | 0x033 <0x12> | base_player const&	user	= *m_weapon.get_user( );   ONLY base
	//   8: --          | 0x06d <0x14> | playback_state.reset( );   ONLY base
	//   9: 0x074 <0x7> | 0x081 <0x14> | packet.append	( playback_state.interval_id );   SIZE
	//  10: 0x07b <0xd> | 0x095 <0x18> | packet.append	( playback_state.interval_time );   SIZE
	//  11: 0x088 <0xc> | --          | L50   ONLY target
	//  12: 0x094 <0xf> | --          | L51   ONLY target
	// ; aligned 4, size-diffs 3, quantity-diffs 5, blank-gaps 1
	// VERDICT: STRUCTURE MATCH (shape ok) - same if-guarded probe (decl/success/get_user/get_animation_playback_state/!success reset/2 appends); quantity rows are SIZE-drift mis-pairing of the LTCG-inlined get_animation_playback_state+append calls (target inline vs base call), non-steerable.
}

// STATE[PARTIAL]: when serialize_animation_state, reads interval_id (u32) + interval_time
// (float) back into m_animation_playback_state. Matches rva 0x6ece90.
void weapon_core_base_state::deserialize( network_core::packet_reader& reader )
{
	if ( m_serialize_animation_state )
	{
		m_animation_playback_state.interval_id		= reader.r< u32 >( );
		m_animation_playback_state.interval_time	= reader.r< float >( );
	}

	// STRUCTURE DIFF[target 0x6ece90 | base 0x460400]: target 3 / base 3 stmts
	//   2: 0x017 <0x11> | 0x017 <0x26> | m_animation_playback_state.interval_id		= reader.r< u32 >( );   SIZE
	//   3: 0x028 <0x13> | 0x03d <0x2e> | m_animation_playback_state.interval_time	= reader.r< float >( );   SIZE
	// ; aligned 1, size-diffs 2, quantity-diffs 0, blank-gaps 0
	// VERDICT: STRUCTURE MATCH (shape ok) - both if-guarded 2 reads; SIZE rows are packet_reader::r<u32>/r<float> LTCG inline (target) vs call (base), non-steerable.
}

} // namespace survarium
