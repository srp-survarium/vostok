////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/hand_to_weapon_ik_processor.h>
#include <vostok/network_core/udp_match_packet.h>
#include <vostok/network_core/packet_reader.h>

namespace survarium {
/*
// STATE[STUB]
// void survarium::`dynamic atexit destructor for 's_ik_hands_debug_draw_cc''()
void `dynamic atexit destructor for 's_ik_hands_debug_draw_cc''( )
{
	// FUNCTION BODY
	// <0x7db180>|0x000|      :'21'	{
	// ******
}
*/
// STATE[STUB]
// survarium::hand_to_weapon_ik_processor::hand_to_weapon_ik_processor()
hand_to_weapon_ik_processor::hand_to_weapon_ik_processor( ) : m_interpolator( 0.0f /* s_aim_transition_time */ ), m_active( true )
{
	// FUNCTION BODY
	// <0x593f70>|0x000|+0x04b:'48'	{
	// <0x593fbb>|0x04b|      :'49'	}
	// ******
}

// STATE[STUB]
// void survarium::hand_to_weapon_ik_processor::activate(vostok::animation::skeleton const&, vostok::animation::skeleton const&)
void hand_to_weapon_ik_processor::activate( animation::skeleton const& user_skeleton, animation::skeleton const& weapon_skeleton )
{
	// FUNCTION BODY
	// <0x59469a>|0x00a|+0x009:'53'
	// <0x5946a3>|0x013|+0x013:'54'
	// <0>
	// <0x5946b6>|0x026|+0x007:'56'
	// <0x5946bd>|0x02d|+0x013:'57'
	// <0x5946d0>|0x040|+0x016:'58'
	// <0x5946e6>|0x056|+0x01f:'59'
	// <0>
	// <0x594705>|0x075|+0x007:'61'
	// <0x59470c>|0x07c|+0x013:'62'
	// <0x59471f>|0x08f|+0x016:'63'
	// <0x594735>|0x0a5|+0x01f:'64'
	// ******
}

// STATE[STUB]
// void survarium::hand_to_weapon_ik_processor::activate_hand(const survarium::hand_to_weapon_ik_processor::hands_enum, const bool, const unsigned int)
void hand_to_weapon_ik_processor::activate_hand( hand_to_weapon_ik_processor::hands_enum hand, bool active, u32 current_time_in_ms )
{
	// FUNCTION BODY
	// <0x593f09>|0x009|+0x016:'69'
	// <0x593f1f>|0x01f|+0x010:'70'
	// <0x593f2f>|0x02f|+0x022:'71'
	// <0x593f51>|0x051|+0x00c:'72'
	// <0>
	// ******
}

// STATE[STUB]
// static bool survarium::hand_to_weapon_ik_processor::hand_need_correction(survarium::hand_to_weapon_ik_processor::hand const&, const unsigned int)
bool hand_to_weapon_ik_processor::hand_need_correction( hand_to_weapon_ik_processor::hand const& h, u32 current_time_in_ms )
{
	return false;

	// FUNCTION BODY
	// <0x593e34>|0x004|+0x035:'78'
	// ******
}

// STATE[STUB]
// static bool survarium::hand_to_weapon_ik_processor::hand_need_interpolation(survarium::hand_to_weapon_ik_processor::hand const&, const unsigned int)
bool hand_to_weapon_ik_processor::hand_need_interpolation( hand_to_weapon_ik_processor::hand const& h, u32 current_time_in_ms )
{
	return false;

	// FUNCTION BODY
	// <0x593e13>|0x003|+0x012:'83'
	// ******
}

// STATE[STUB]
// static unsigned int survarium::hand_to_weapon_ik_processor::get_hand_new_start_transition_time(survarium::hand_to_weapon_ik_processor::hand const&, const unsigned int)
u32 hand_to_weapon_ik_processor::get_hand_new_start_transition_time( hand_to_weapon_ik_processor::hand const& h, u32 current_time_in_ms )
{
	// LOCALS
	// u32 							current_transition_time
	// ******

	return 0;

	// FUNCTION BODY
	// <0x593dd6>|0x006|+0x00b:'88'
	// <0>
	// <1>
	// <0x593de1>|0x011|+0x024:'91'
	// ******
}

// STATE[STUB]
// void survarium::hand_to_weapon_ik_processor::process(const unsigned int, vostok::math::float4x4 const*, vostok::math::float4x4*) const
void hand_to_weapon_ik_processor::process( u32 current_time_in_ms, float4x4 const* weapon_matrices, float4x4* user_matrices ) const
{
	// LOCALS
	// animation::skeleton_bone const& weapon_bone
	// float4x4 const& 				weapon_transform
	// hand_to_weapon_ik_processor::hand const* h<1>
	// float4x4 const& 				locator_transform<2>
	// float4x4 const& 				target_transform<3>
	// float4x4 const& 				hand_transform<3>
	// ******

	// SKIPPED BLOCKS
	// <0x5947e8><2>
	// ******

	// FUNCTION BODY
	// <0x59476f>|0x00f|+0x01a:'96'
	// <0x594789>|0x029|+0x024:'97'
	// <0>
	// <0x5947ad>|0x04d|+0x026|[1]:'99'
	// <0x5947d3>|0x073|+0x01b:'100'
	// <0x5947ee>|0x08e|+0x02c:'101'
	// <0x59481a>|0x0ba|+0x017:'102'
	// <0x594831>|0x0d1|+0x01a:'103'
	// <0x59484b>|0x0eb|+0x005:'104'
	// <0x594850>|0x0f0|+0x03e|[3]:'105'
	// <0x59488e>|0x12e|+0x03e:'106'
	// <0x5948cc>|0x16c|+0x01a:'107'
	// <0>
	// <0x5948e6>|0x186|+0x005:'109'
	// <0>
	// ******
}

// STATE[STUB]
// void survarium::hand_to_weapon_ik_processor::process_hand(survarium::hand_to_weapon_ik_processor::hand const&, vostok::math::float4x4 const&, vostok::math::float4x4*) const
void hand_to_weapon_ik_processor::process_hand( hand_to_weapon_ik_processor::hand const& h, float4x4 const& target_hand_obj_space_transform, float4x4* matrices ) const
{
	// LOCALS
	// float3 const& 				arm_to_hand_dir
	// u32 							forearm_matrix_index
	// float3 const& 				original_forearm_dir
	// animation::skeleton_bone const& arm_bone
	// u32 							arm_matrix_index
	// float3 						arm_pos
	// float3 const& 				new_forearm_dir
	// float4x4 					forearm_obj_matrix
	// animation::skeleton_bone const& hand_bone
	// float3 const& 				target_arm_dir
	// float 						arm_alpha_angle
	// float4x4 const& 				forearm_rotation_matrix
	// float3 						forearm_pos
	// float 						arm_len
	// float4x4 const& 				arm_rotation_matrix
	// float3 const& 				initial_hand_pos
	// float4x4 					arm_obj_matrix
	// float3 const& 				original_arm_dir
	// animation::skeleton_bone const& forearm_bone
	// float 						arm_to_hand_len
	// float3 const& 				initial_forearm_pos
	// float4x4 const& 				alpha_rotation_matrix
	// float 						forearm_len
	// float3 const& 				rotation_axis
	// ******

	// FUNCTION BODY
	// <0x5940b1>|0x011|+0x01a:'115'
	// <0x5940cb>|0x02b|+0x00c:'116'
	// <0x5940d7>|0x037|+0x00c:'117'
	// <0>
	// <0x5940e3>|0x043|+0x05d:'119'
	// <0x594140>|0x0a0|+0x060:'120'
	// <0>
	// <0x5941a0>|0x100|+0x01d:'122'
	// <0x5941bd>|0x11d|+0x020:'123'
	// <0x5941dd>|0x13d|+0x012:'124'
	// <0x5941ef>|0x14f|+0x012:'125'
	// <0>
	// <0x594201>|0x161|+0x024:'127'
	// <0x594225>|0x185|+0x025:'128'
	// <0x59424a>|0x1aa|+0x02e:'129'
	// <0x594278>|0x1d8|+0x014:'130'
	// <0x59428c>|0x1ec|+0x005:'131'
	// <0>
	// <0x594291>|0x1f1|+0x03b:'133'
	// <0x5942cc>|0x22c|+0x02c:'134'
	// <0x5942f8>|0x258|+0x02e:'135'
	// <0x594326>|0x286|+0x02e:'136'
	// <0x594354>|0x2b4|+0x026:'137'
	// <0x59437a>|0x2da|+0x024:'138'
	// <0x59439e>|0x2fe|+0x021:'139'
	// <0x5943bf>|0x31f|+0x026:'140'
	// <0x5943e5>|0x345|+0x016:'141'
	// <0>
	// <0x5943fb>|0x35b|+0x023:'143'
	// <0x59441e>|0x37e|+0x025:'144'
	// <0x594443>|0x3a3|+0x035:'145'
	// <0x594478>|0x3d8|+0x034:'146'
	// <0x5944ac>|0x40c|+0x03b:'147'
	// <0>
	// <0x5944e7>|0x447|+0x023:'149'
	// <0x59450a>|0x46a|+0x016:'150'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <12>
	// <13>
	// <14>
	// <15>
	// <16>
	// <17>
	// <18>
	// <19>
	// <20>
	// <21>
	// <22>
	// <23>
	// <24>
	// <0x594520>|0x480|+0x02f:'176'
	// <0x59454f>|0x4af|+0x042:'177'
	// <0x594591>|0x4f1|+0x05d:'178'
	// <0>
	// <0x5945ee>|0x54e|+0x032:'180'
	// <0x594620>|0x580|+0x05f:'181'
	// ******
}

// STATE[STUB]
// float survarium::hand_to_weapon_ik_processor::get_hand_coefficient(survarium::hand_to_weapon_ik_processor::hand const&, const unsigned int) const
float hand_to_weapon_ik_processor::get_hand_coefficient( hand_to_weapon_ik_processor::hand const& h, u32 current_time_in_ms ) const
{
	// LOCALS
	// float 						interpolation_coeff
	// float 						hand_transition_time
	// ******

	// CALL SITE INFO
	// <0x593ec3> -> float <unknown>(float) const
	// <0x593ee3> -> float <unknown>(float) const
	// ******

	return 0.0f;

	// FUNCTION BODY
	// <0x593e79>|0x009|+0x00c:'186'
	// <0x593e85>|0x015|+0x01e:'187'
	// <0>
	// <1>
	// <0x593ea3>|0x033|+0x04f:'190'
	// <0x593ef2>|0x082|+0x003:'191'
	// ******
}

// STATE[PARTIAL]: packs both hands' is_active into a u8 bitfield (bit0=left, bit1=right),
// appends it, then appends each hand's start_transition_time_in_ms biased by client_offset.
void hand_to_weapon_ik_processor::serialize( network_core::udp_match_packet& packet, u32 client_offset ) const
{
	u8 active_hands = ( m_hands[ left ].is_active ? 1 : 0 ) | ( m_hands[ right ].is_active ? 2 : 0 );

	packet.append( active_hands );
	packet.append( m_hands[ left  ].start_transition_time_in_ms - client_offset );
	packet.append( m_hands[ right ].start_transition_time_in_ms - client_offset );

	// STRUCTURE DIFF[target 0x584040 | base 0x51aef0]: target 4 / base 4 stmts
	//   2: 0x029 <0xd> | 0x029 <0x14> | packet.append( active_hands );   SIZE
	//   3: 0x036 <0x11> | 0x03d <0x19> | packet.append( m_hands[ left  ].start_transition_time_in_ms - client_offset );   SIZE
	//   4: 0x047 <0x12> | 0x056 <0x1a> | packet.append( m_hands[ right ].start_transition_time_in_ms - client_offset );   SIZE
	// ; aligned 1, size-diffs 3, quantity-diffs 0, blank-gaps 1
	// VERDICT: STRUCTURE MATCH (shape ok) - pack byte + 3 appends; SIZE rows are packet<T>::append LTCG inline (target) vs call (base), non-steerable.
}

// STATE[PARTIAL]: reads the packed active-hands byte then both start times; the trailing
// stub is a compiled-out assert. Bits 0/1 of active_hands drive each hand's is_active.
void hand_to_weapon_ik_processor::deserialize( network_core::packet_reader& reader )
{
	u8 active_hands = reader.r< bool >( );

	m_hands[ left  ].start_transition_time_in_ms = reader.r< u32 >( );
	m_hands[ right ].start_transition_time_in_ms = reader.r< u32 >( );

	m_hands[ left  ].is_active = ( active_hands & 1 ) != 0;
	m_hands[ right ].is_active = ( active_hands & 2 ) != 0;

	// STRUCTURE DIFF[target 0x583fd0 | base 0x51ae50]: target 6 / base 5 stmts
	//   1: 0x009 <0xb> | 0x009 <0x20> | u8 active_hands = reader.r< bool >( );   SIZE
	//   2: 0x014 <0xd> | 0x029 <0x22> | m_hands[ left  ].start_transition_time_in_ms = reader.r< u32 >( );   SIZE
	//   3: 0x021 <0xe> | 0x04b <0x23> | m_hands[ right ].start_transition_time_in_ms = reader.r< u32 >( );   SIZE
	//   4: 0x02f <0xc> | --          | L208   ONLY target
	// ; aligned 2, size-diffs 3, quantity-diffs 1, blank-gaps 1
	// VERDICT: STRUCTURE MATCH (shape ok) - read byte + 2 times + 2 bit-tests; SIZE/quantity are r<bool>/r<u32> LTCG inline (target) vs call (base), non-steerable.
}

} // namespace survarium
