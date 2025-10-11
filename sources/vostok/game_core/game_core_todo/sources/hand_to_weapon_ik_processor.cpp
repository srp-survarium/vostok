////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "hand_to_weapon_ik_processor.h"

namespace survarium {

// STATE[STUB]
// void survarium::`dynamic atexit destructor for 's_ik_hands_debug_draw_cc''()
void `dynamic atexit destructor for 's_ik_hands_debug_draw_cc''( )
{
}

// STATE[STUB]
// survarium::hand_to_weapon_ik_processor::hand_to_weapon_ik_processor()
hand_to_weapon_ik_processor::hand_to_weapon_ik_processor( )
{
}

// STATE[STUB]
// void survarium::hand_to_weapon_ik_processor::activate(vostok::animation::skeleton const&, vostok::animation::skeleton const&)
void hand_to_weapon_ik_processor::activate( vostok::animation::skeleton const& user_skeleton, vostok::animation::skeleton const& weapon_skeleton )
{
	// FUNCTION BODY
	// <0x59469a>|0x000|0x000:'53'
	// <0x5946a3>|0x009|0x009:'54'
	// 1
	// <0x5946b6>|0x01c|0x013:'56'
	// <0x5946bd>|0x023|0x007:'57'
	// <0x5946d0>|0x036|0x013:'58'
	// <0x5946e6>|0x04c|0x016:'59'
	// 1
	// <0x594705>|0x06b|0x01f:'61'
	// <0x59470c>|0x072|0x007:'62'
	// <0x59471f>|0x085|0x013:'63'
	// <0x594735>|0x09b|0x016:'64'
	// ******
}

// STATE[STUB]
// void survarium::hand_to_weapon_ik_processor::activate_hand(const survarium::hand_to_weapon_ik_processor::hands_enum, const bool, const unsigned int)
void hand_to_weapon_ik_processor::activate_hand( hand_to_weapon_ik_processor::hands_enum hand, bool active, u32 current_time_in_ms )
{
	// FUNCTION BODY
	// <0x593f09>|0x000|0x000:'69'
	// <0x593f1f>|0x016|0x016:'70'
	// <0x593f2f>|0x026|0x010:'71'
	// <0x593f51>|0x048|0x022:'72'
	// 1
	// ******
}

// STATE[STUB]
// static bool survarium::hand_to_weapon_ik_processor::hand_need_correction(survarium::hand_to_weapon_ik_processor::hand const&, const unsigned int)
static bool hand_to_weapon_ik_processor::hand_need_correction( hand_to_weapon_ik_processor::hand const& h, u32 current_time_in_ms )
{
	return false;
	// FUNCTION BODY
	// <0x593e34>|0x000|0x000:'78'
	// ******
}

// STATE[STUB]
// static bool survarium::hand_to_weapon_ik_processor::hand_need_interpolation(survarium::hand_to_weapon_ik_processor::hand const&, const unsigned int)
static bool hand_to_weapon_ik_processor::hand_need_interpolation( hand_to_weapon_ik_processor::hand const& h, u32 current_time_in_ms )
{
	return false;
	// FUNCTION BODY
	// <0x593e13>|0x000|0x000:'83'
	// ******
}

// STATE[STUB]
// static unsigned int survarium::hand_to_weapon_ik_processor::get_hand_new_start_transition_time(survarium::hand_to_weapon_ik_processor::hand const&, const unsigned int)
static u32 hand_to_weapon_ik_processor::get_hand_new_start_transition_time( hand_to_weapon_ik_processor::hand const& h, u32 current_time_in_ms )
{
	// LOCALS
	// u32 							current_transition_time
	// ******

	return 0;
	// FUNCTION BODY
	// <0x593dd6>|0x000|0x000:'88'
	// 1
	// 2
	// <0x593de1>|0x00b|0x00b:'91'
	// ******
}

// STATE[STUB]
// void survarium::hand_to_weapon_ik_processor::process(const unsigned int, vostok::math::float4x4 const*, vostok::math::float4x4*) const
void hand_to_weapon_ik_processor::process( u32 current_time_in_ms, vostok::math::float4x4 const* weapon_matrices, vostok::math::float4x4* user_matrices ) const
{
	// LOCALS
	// vostok::animation::skeleton_bone const& weapon_bone
	// vostok::math::float4x4 const& weapon_transform
	// hand_to_weapon_ik_processor::hand const* h<1>
	// vostok::math::float4x4 const& locator_transform<2>
	// vostok::math::float4x4 const& target_transform<3>
	// vostok::math::float4x4 const& hand_transform<3>
	// ******

	// SKIPPED BLOCKS
	// <0x5947e8><2>
	// ******

	// FUNCTION BODY
	// <0x59476f>|0x000|0x000:'96'
	// <0x594789>|0x01a|0x01a:'97'
	// 1
	// <0x5947ad>|0x03e|0x024|[1]:'99'
	// <0x5947d3>|0x064|0x026:'100'
	// <0x5947ee>|0x07f|0x01b:'101'
	// <0x59481a>|0x0ab|0x02c:'102'
	// <0x594831>|0x0c2|0x017:'103'
	// <0x59484b>|0x0dc|0x01a:'104'
	// <0x594850>|0x0e1|0x005|[3]:'105'
	// <0x59488e>|0x11f|0x03e:'106'
	// <0x5948cc>|0x15d|0x03e:'107'
	// 1
	// <0x5948e6>|0x177|0x01a:'109'
	// 1
	// ******
}

// STATE[STUB]
// void survarium::hand_to_weapon_ik_processor::process_hand(survarium::hand_to_weapon_ik_processor::hand const&, vostok::math::float4x4 const&, vostok::math::float4x4*) const
void hand_to_weapon_ik_processor::process_hand( hand_to_weapon_ik_processor::hand const& h, vostok::math::float4x4 const& target_hand_obj_space_transform, vostok::math::float4x4* matrices ) const
{
	// LOCALS
	// vostok::math::float3 const& 	arm_to_hand_dir
	// u32 							forearm_matrix_index
	// vostok::math::float3 const& 	original_forearm_dir
	// vostok::animation::skeleton_bone const& arm_bone
	// u32 							arm_matrix_index
	// vostok::math::float3 		arm_pos
	// vostok::math::float3 const& 	new_forearm_dir
	// vostok::math::float4x4 		forearm_obj_matrix
	// vostok::animation::skeleton_bone const& hand_bone
	// vostok::math::float3 const& 	target_arm_dir
	// float 						arm_alpha_angle
	// vostok::math::float4x4 const& forearm_rotation_matrix
	// vostok::math::float3 		forearm_pos
	// float 						arm_len
	// vostok::math::float4x4 const& arm_rotation_matrix
	// vostok::math::float3 const& 	initial_hand_pos
	// vostok::math::float4x4 		arm_obj_matrix
	// vostok::math::float3 const& 	original_arm_dir
	// vostok::animation::skeleton_bone const& forearm_bone
	// float 						arm_to_hand_len
	// vostok::math::float3 const& 	initial_forearm_pos
	// vostok::math::float4x4 const& alpha_rotation_matrix
	// float 						forearm_len
	// vostok::math::float3 const& 	rotation_axis
	// ******

	// FUNCTION BODY
	// <0x5940b1>|0x000|0x000:'115'
	// <0x5940cb>|0x01a|0x01a:'116'
	// <0x5940d7>|0x026|0x00c:'117'
	// 1
	// <0x5940e3>|0x032|0x00c:'119'
	// <0x594140>|0x08f|0x05d:'120'
	// 1
	// <0x5941a0>|0x0ef|0x060:'122'
	// <0x5941bd>|0x10c|0x01d:'123'
	// <0x5941dd>|0x12c|0x020:'124'
	// <0x5941ef>|0x13e|0x012:'125'
	// 1
	// <0x594201>|0x150|0x012:'127'
	// <0x594225>|0x174|0x024:'128'
	// <0x59424a>|0x199|0x025:'129'
	// <0x594278>|0x1c7|0x02e:'130'
	// <0x59428c>|0x1db|0x014:'131'
	// 1
	// <0x594291>|0x1e0|0x005:'133'
	// <0x5942cc>|0x21b|0x03b:'134'
	// <0x5942f8>|0x247|0x02c:'135'
	// <0x594326>|0x275|0x02e:'136'
	// <0x594354>|0x2a3|0x02e:'137'
	// <0x59437a>|0x2c9|0x026:'138'
	// <0x59439e>|0x2ed|0x024:'139'
	// <0x5943bf>|0x30e|0x021:'140'
	// <0x5943e5>|0x334|0x026:'141'
	// 1
	// <0x5943fb>|0x34a|0x016:'143'
	// <0x59441e>|0x36d|0x023:'144'
	// <0x594443>|0x392|0x025:'145'
	// <0x594478>|0x3c7|0x035:'146'
	// <0x5944ac>|0x3fb|0x034:'147'
	// 1
	// <0x5944e7>|0x436|0x03b:'149'
	// <0x59450a>|0x459|0x023:'150'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// 8
	// 9
	// 10
	// 11
	// 12
	// 13
	// 14
	// 15
	// 16
	// 17
	// 18
	// 19
	// 20
	// 21
	// 22
	// 23
	// 24
	// 25
	// <0x594520>|0x46f|0x016:'176'
	// <0x59454f>|0x49e|0x02f:'177'
	// <0x594591>|0x4e0|0x042:'178'
	// 1
	// <0x5945ee>|0x53d|0x05d:'180'
	// <0x594620>|0x56f|0x032:'181'
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

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x582ec3 }, type_index: TypeIndex(0x65c1) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x582ee3 }, type_index: TypeIndex(0x65c1) })
	// ******

	return 0.0f;
	// FUNCTION BODY
	// <0x593e79>|0x000|0x000:'186'
	// <0x593e85>|0x00c|0x00c:'187'
	// 1
	// 2
	// <0x593ea3>|0x02a|0x01e:'190'
	// <0x593ef2>|0x079|0x04f:'191'
	// ******
}

// STATE[STUB]
// void survarium::hand_to_weapon_ik_processor::serialize(vostok::network_core::udp_match_packet&, unsigned int) const
void hand_to_weapon_ik_processor::serialize( vostok::network_core::udp_match_packet& packet, u32 client_offset ) const
{
	// LOCALS
	// u8 							active_hands
	// ******

	// FUNCTION BODY
	// <0x594049>|0x000|0x000:'196'
	// <0x594069>|0x020|0x020:'197'
	// <0x594076>|0x02d|0x00d:'198'
	// <0x594087>|0x03e|0x011:'199'
	// ******
}

// STATE[STUB]
// void survarium::hand_to_weapon_ik_processor::deserialize(vostok::network_core::packet_reader&)
void hand_to_weapon_ik_processor::deserialize( vostok::network_core::packet_reader& reader )
{
	// LOCALS
	// u8 							active_hands
	// ******

	// FUNCTION BODY
	// <0x593fd9>|0x000|0x000:'204'
	// <0x593fe4>|0x00b|0x00b:'205'
	// <0x593ff1>|0x018|0x00d:'206'
	// 1
	// <0x593fff>|0x026|0x00e:'208'
	// <0x59400b>|0x032|0x00c:'209'
	// <0x59401e>|0x045|0x013:'210'
	// ******
}

} // namespace survarium
