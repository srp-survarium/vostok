////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/player_state.h>
#include <vostok/network_core/udp_match_packet.h>
#include <vostok/network_core/packet_reader.h>

namespace survarium {

// STATE[49.74%|PARTIAL]: read position(float3) + yaw(float) + look_pitch(float), then
// rebuild transform = create_rotation_y(yaw) with translation = position.
void player_state::deserialize( network_core::packet_reader& packet )
{
	float3 const&	position	= packet.r< math::float3 >( );
	float			yaw			= packet.r< float >( );
	look_pitch					= packet.r< float >( );
	transform					= math::create_rotation_y( yaw );
	transform.c.xyz( )			= position;

	// STRUCTURE DIFF: target 5 stmts / base 5 stmts
	// b.diff    |t.addr  |b.addr  |t.sz|b.sz|b.ln|b.code
	// ----------+--------+--------+----+----+----+------
	// SIZE +0x1f|0x7665e1|0x590491|0x11|0x30|0   |float3 const&	position	= packet.r< math::float3 >( );
	// SIZE +0xd |0x7665f2|0x5904c1|0xd |0x1a|+1  |float			yaw			= packet.r< float >( );
	// SIZE +0x13|0x7665ff|0x5904db|0x13|0x26|+2  |look_pitch					= packet.r< float >( );
	// VERDICT: STRUCTURE MATCH (shape ok) - 5/5, last 2 (create_rotation_y + translation store) align clean; SIZE rows are the target's LTCG-folded r<float3>/r<float> calls vs the base's inlined wrapper + inner r() call, non-steerable.
}

// STATE[45.63%|PARTIAL]: append translation(float3), yaw from get_angles().y, look_pitch.
void player_state::serialize( network_core::udp_match_packet& packet ) const
{
	packet.append		( transform.c.xyz( ) );
	packet.append		( transform.get_angles( math::rotation_zxy ).y );
	packet.append		( look_pitch );

	// STRUCTURE DIFF: target 3 stmts / base 3 stmts
	// b.diff   |t.addr  |b.addr  |t.sz|b.sz|b.ln|b.code
	// ---------+--------+--------+----+----+----+------
	// SIZE +0x9|0x76657b|0x59041b|0x13|0x1c|0   |packet.append		( transform.c.xyz( ) );
	// SIZE +0x9|0x76658e|0x590437|0x1a|0x23|+1  |packet.append		( transform.get_angles( math::rotation_zxy ).y );
	// SIZE +0x9|0x7665a8|0x59045a|0x12|0x1b|+2  |packet.append		( look_pitch );
	// VERDICT: STRUCTURE MATCH (shape ok) - 3 appends; uniform +0x9 per row is packet<T>::append/get_angles LTCG inline (target) vs call (base), non-steerable.
}

} // namespace survarium
