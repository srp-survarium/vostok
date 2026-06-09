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
}

} // namespace survarium
