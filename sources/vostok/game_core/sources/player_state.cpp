////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/player_state.h>
#include <vostok/network_core/udp_match_packet.h>
#include <vostok/network_core/packet_reader.h>

namespace survarium {

// STATE[PARTIAL]: read position(float3) + yaw(float) + look_pitch(float), then
// rebuild transform = create_rotation_y(yaw) with translation = position.
void player_state::deserialize( network_core::packet_reader& packet )
{
	float3 const&	position	= packet.r< math::float3 >( );
	float			yaw			= packet.r< float >( );
	look_pitch					= packet.r< float >( );
	transform					= math::create_rotation_y( yaw );
	transform.c.xyz( )			= position;

	// STRUCTURE DIFF[target 0x7665d0 | base 0x56d7d0]: target 5 / base 5 stmts
	//   1: 0x011 <0x11> | 0x011 <0x3e> | float3 const&	position	= packet.r< math::float3 >( );   SIZE
	//   2: 0x022 <0xd> | 0x04f <0x28> | float			yaw			= packet.r< float >( );   SIZE
	//   3: 0x02f <0x13> | 0x077 <0x2e> | look_pitch					= packet.r< float >( );   SIZE
	// ; aligned 2, size-diffs 3, quantity-diffs 0, blank-gaps 0
	// VERDICT: STRUCTURE MATCH (shape ok) - 3 reads + transform rebuild; SIZE rows are r<float3>/r<float> LTCG inline (target) vs call (base), non-steerable.
}

// STATE[PARTIAL]: append translation(float3), yaw from get_angles().y, look_pitch.
void player_state::serialize( network_core::udp_match_packet& packet ) const
{
	packet.append		( transform.c.xyz( ) );
	packet.append		( transform.get_angles( math::rotation_zxy ).y );
	packet.append		( look_pitch );

	// STRUCTURE DIFF[target 0x766570 | base 0x56d760]: target 3 / base 3 stmts
	//   1: 0x00b <0x13> | 0x00b <0x1c> | packet.append		( transform.c.xyz( ) );   SIZE
	//   2: 0x01e <0x1a> | 0x027 <0x23> | packet.append		( transform.get_angles( math::rotation_zxy ).y );   SIZE
	//   3: 0x038 <0x12> | 0x04a <0x1b> | packet.append		( look_pitch );   SIZE
	// ; aligned 0, size-diffs 3, quantity-diffs 0, blank-gaps 0
	// VERDICT: STRUCTURE MATCH (shape ok) - 3 appends; SIZE rows are packet<T>::append/get_angles LTCG inline (target) vs call (base), non-steerable.
}

} // namespace survarium
