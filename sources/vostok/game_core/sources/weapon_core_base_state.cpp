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

weapon_core_base_state::weapon_core_base_state( weapon_core& weapon, bool serialize_animation_state )
	:	m_weapon						( weapon ),
		m_is_firing_ptr					( NULL ),
		m_body_part_mask_for_user		( animation::body_part_whole_body ),	// -1
		m_is_ready_to_be_deactivated	( false ),
		m_animation_has_been_ended		( false ),
		m_serialize_animation_state		( serialize_animation_state )
{
}

void weapon_core_base_state::finalize( )
{
	m_animation_playback_state.reset( );
}

void weapon_core_base_state::execute( )
{
	m_animation_playback_state.reset( );
}

bool weapon_core_base_state::deserializing( ) const
{
	return m_weapon.deserializing( );
}

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
}

void weapon_core_base_state::deserialize( network_core::packet_reader& reader )
{
	if ( m_serialize_animation_state )
	{
		m_animation_playback_state.interval_id		= reader.r< u32 >( );
		m_animation_playback_state.interval_time	= reader.r< float >( );
	}
}

} // namespace survarium
