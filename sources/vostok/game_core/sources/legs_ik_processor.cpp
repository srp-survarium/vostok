////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/legs_ik_processor.h>

namespace survarium {

/*
// STATE[STUB]
// void survarium::`dynamic initializer for 's_ik_legs_debug_draw_cc''()
void `dynamic initializer for 's_ik_legs_debug_draw_cc''( )
{
	// FUNCTION BODY
	// <0x7db1c0>|0x000|      :'20'	{
	// ******
}

// STATE[STUB]
// void survarium::`dynamic initializer for 's_ik_foot_capsule_radius_cc''()
void `dynamic initializer for 's_ik_foot_capsule_radius_cc''( )
{
	// FUNCTION BODY
	// <0x7db200>|0x000|      :'23'	{
	// ******
}

// STATE[STUB]
// void survarium::`dynamic initializer for 's_ik_legs_rot_axis_cc''()
void `dynamic initializer for 's_ik_legs_rot_axis_cc''( )
{
	// FUNCTION BODY
	// <0x7db250>|0x000|      :'26'	{
	// ******
}

// STATE[STUB]
// void survarium::`dynamic atexit destructor for 's_ik_legs_debug_draw_cc''()
void `dynamic atexit destructor for 's_ik_legs_debug_draw_cc''( )
{
	// FUNCTION BODY
	// <0x7db290>|0x000|      :'29'	{
	// ******
}
*/

// STATE[STUB]
// survarium::legs_ik_processor::leg_params::leg_params()
legs_ik_processor::leg_params::leg_params( )
{
	// FUNCTION BODY
	// <0x6fa6c0>|0x000|+0x062:'38'	{
	// <0x6fa722>|0x062|      :'39'	}
	// ******
}

// STATE[STUB]
// void survarium::legs_ik_processor::leg_params::activate(vostok::animation::skeleton const&, char const*)
void legs_ik_processor::leg_params::activate( animation::skeleton const& skeleton, pcstr foot_bone_name )
{
	// LOCALS
	// animation::skeleton_bone const& foot_bone
	// ******

	// FUNCTION BODY
	// <0x6fad89>|0x009|+0x016:'43'
	// <0x6fad9f>|0x01f|+0x02e:'44'
	// <0x6fadcd>|0x04d|+0x03a:'45'
	// <0x6fae07>|0x087|+0x038:'46'
	// <0x6fae3f>|0x0bf|+0x041:'47'
	// <0x6fae80>|0x100|+0x04a:'48'
	// ******
}

// STATE[STUB]
// void survarium::legs_ik_processor::~legs_ik_processor()
legs_ik_processor::~legs_ik_processor( )
{
	// FUNCTION BODY
	// <0x6faf49>|0x009|+0x029:'53'
	// ******
}

// STATE[STUB]
// void survarium::legs_ik_processor::leg_params::tick(float)
void legs_ik_processor::leg_params::tick( float dt )
{
	// FUNCTION BODY
	// <0x6fa969>|0x009|+0x022:'64'
	// <0x6fa98b>|0x02b|+0x022:'65'
	// <0x6fa9ad>|0x04d|+0x015:'66'
	// ******
}

// STATE[STUB]
// void survarium::legs_ik_processor::leg_params::set_heel_transition_time(float)
void legs_ik_processor::leg_params::set_heel_transition_time( float tr_time )
{
	// FUNCTION BODY
	// <0x6fa939>|0x009|+0x01a:'71'
	// ******
}

// STATE[STUB]
// void survarium::legs_ik_processor::leg_params::set_toe_transition_time(float)
void legs_ik_processor::leg_params::set_toe_transition_time( float tr_time )
{
	// FUNCTION BODY
	// <0x6fa909>|0x009|+0x01a:'76'
	// ******
}

// STATE[STUB]
// void survarium::legs_ik_processor::leg_params::set_heel_on_ground(const bool)
void legs_ik_processor::leg_params::set_heel_on_ground( bool value )
{
	// FUNCTION BODY
	// <0x6fa899>|0x009|+0x009:'81'
	// <0x6fa8a2>|0x012|+0x03a:'82'
	// <0x6fa8dc>|0x04c|+0x010:'83'
	// ******
}

// STATE[STUB]
// void survarium::legs_ik_processor::leg_params::set_toe_on_ground(const bool)
void legs_ik_processor::leg_params::set_toe_on_ground( bool value )
{
	// FUNCTION BODY
	// <0x6fa829>|0x009|+0x009:'88'
	// <0x6fa832>|0x012|+0x03a:'89'
	// <0x6fa86c>|0x04c|+0x010:'90'
	// ******
}

// STATE[STUB]
// survarium::legs_ik_processor::legs_ik_processor()
// this->m_heel_transition_time = this->m_heel_interpolator.transition_time(&this->m_heel_interpolator);
// this->m_toe_transition_time = this->m_toe_interpolator.transition_time(&this->m_toe_interpolator);
legs_ik_processor::legs_ik_processor( ) : m_heel_interpolator( 0.1f ), m_toe_interpolator( 0.1f )
{
	// CALL SITE INFO
	// <0x6fa7ea> -> float <unknown>() const
	// <0x6fa804> -> float <unknown>() const
	// ******

	// FUNCTION BODY
	// <0x6fa730>|0x000|+0x0df:'100'	{
	// <0x6fa80f>|0x0df|      :'101'	}
	// ******
}

// STATE[STUB]
// void survarium::legs_ik_processor::activate(vostok::animation::skeleton const&)
void legs_ik_processor::activate( animation::skeleton const& skeleton )
{
	// FUNCTION BODY
	// <0x6faed9>|0x009|+0x00c:'105'
	// <0x6faee5>|0x015|+0x014:'106'
	// <0x6faef9>|0x029|+0x014:'107'
	// <0x6faf0d>|0x03d|+0x01e:'108'
	// ******
}

// STATE[STUB]
// float survarium::get_additional_length(vostok::math::float3 const&, vostok::math::float3 const&, float)
float get_additional_length( float3 const& upleg_dir, float3 const& leg_dir, float knee_len )
{
	// LOCALS
	// float 						knee_angle_cos
	// ******

	return 0.0f;

	// FUNCTION BODY
	// <0xcb1f6>|0x006|+0x018:'113'
	// <0xcb20e>|0x01e|+0x075:'114'
	// ******
}

// STATE[STUB]
// void survarium::legs_ik_processor::process(vostok::math::float4x4*, vostok::math::float4x4 const&)
void legs_ik_processor::process( float4x4* matrices, float4x4 const& transform )
{
	// LOCALS
	// float4x4 					inverted_transform
	// float 						right_delta_len
	// float 						left_delta_len
	// float4x4 					hip_obj_matrix
	// float4x4 const& 				right_foot_fixed_transform
	// float4x4 const& 				hip_world_matrix
	// bool 						success
	// float4x4 const& 				left_foot_fixed_transform
	// ******

	// FUNCTION BODY
	// <0x6fc651>|0x011|+0x029:'119'
	// <0x6fc67a>|0x03a|+0x023:'120'
	// <0x6fc69d>|0x05d|+0x010:'121'
	// <0x6fc6ad>|0x06d|+0x02e:'122'
	// <0x6fc6db>|0x09b|+0x010:'123'
	// <0x6fc6eb>|0x0ab|+0x037:'124'
	// <0>
	// <0x6fc722>|0x0e2|+0x00b:'126'
	// <0x6fc72d>|0x0ed|+0x012:'127'
	// <0x6fc73f>|0x0ff|+0x028:'128'
	// <0>
	// <0x6fc767>|0x127|+0x035:'130'
	// <0>
	// <0x6fc79c>|0x15c|+0x03f:'132'
	// <0x6fc7db>|0x19b|+0x047:'133'
	// <0>
	// <0x6fc822>|0x1e2|+0x092:'135'
	// <0>
	// <1>
	// <0x6fc8b4>|0x274|+0x018:'138'
	// <0>
	// <0x6fc8cc>|0x28c|+0x038:'140'
	// <0>
	// <1>
	// <0x6fc904>|0x2c4|+0x03f:'143'
	// <0x6fc943>|0x303|+0x047:'144'
	// <0>
	// <0x6fc98a>|0x34a|+0x092:'146'
	// <0>
	// <1>
	// <0x6fca1c>|0x3dc|+0x018:'149'
	// <0>
	// <0x6fca34>|0x3f4|+0x038:'151'
	// <0>
	// <1>
	// <0x6fca6c>|0x42c|+0x03f:'154'
	// <0x6fcaab>|0x46b|+0x047:'155'
	// <0>
	// <0x6fcaf2>|0x4b2|+0x0bd:'157'
	// <0>
	// <1>
	// <2>
	// ******
}

// STATE[STUB]
// void survarium::legs_ik_processor::process_leg(survarium::legs_ik_processor::leg_params&, vostok::math::float4x4 const&, vostok::math::float4x4 const&, vostok::math::float4x4*, vostok::math::float4x4 const&)
void legs_ik_processor::process_leg(
	legs_ik_processor::leg_params&		params,
	float4x4 const&						target_foot_obj_matrix,
	float4x4 const&						hip_obj_matrix,
	float4x4*							matrices,
	float4x4 const&						transform
)
{
	// LOCALS
	// float4x4 					up_leg_obj_matrix
	// float4x4 					foot_obj_matrix
	// float3 const& 				target_up_leg_to_foot_dir
	// u32 							up_leg_matrix_index
	// u32 							leg_matrix_index
	// float4x4 					knee_obj_matrix
	// float4x4 					toe_obj_matrix
	// u32 							foot_matrix_index
	// float 						leg_len
	// u32 							toe_matrix_index
	// float3 						foot_pos
	// float 						up_leg_len
	// float4x4 					leg_obj_matrix
	// u32 							knee_matrix_index
	// float 						knee_len
	// float3 const& 				original_up_leg_to_foot_dir<1>
	// float 						up_leg_alpha_angle<1>
	// float 						additive_len<1>
	// float 						up_leg_to_foot_len<1>
	// float3 const& 				original_up_leg_dir<1>
	// float3 const& 				target_up_leg_dir<1>
	// float4x4 const& 				rotation_matrix<1>
	// float4x4 const& 				alpha_rotation_matrix<1>
	// float4x4 const& 				rotation_matrix<1>
	// float3 const& 				original_knee_dir<1>
	// float3 const& 				target_leg_dir<1>
	// float4x4 const& 				rotation_matrix<1>
	// float3 const& 				original_leg_dir<1>
	// float3 const& 				foot_pos<1>
	// float3 const& 				toe_pos<1>
	// u32 							toe_matrix_index<1>
	// ******

	// SKIPPED BLOCKS
	// <0x6fb8a8><1>
	// <0x6fb91d><1>
	// ******

	// FUNCTION BODY
	// <0x6fafb1>|0x011|+0x018:'166'
	// <0x6fafc9>|0x029|+0x017:'167'
	// <0x6fafe0>|0x040|+0x01b:'168'
	// <0x6faffb>|0x05b|+0x018:'169'
	// <0x6fb013>|0x073|+0x01b:'170'
	// <0>
	// <0x6fb02e>|0x08e|+0x01a:'172'
	// <0x6fb048>|0x0a8|+0x01d:'173'
	// <0x6fb065>|0x0c5|+0x01a:'174'
	// <0>
	// <0x6fb07f>|0x0df|+0x020:'176'
	// <0x6fb09f>|0x0ff|+0x020:'177'
	// <0x6fb0bf>|0x11f|+0x020:'178'
	// <0x6fb0df>|0x13f|+0x01d:'179'
	// <0x6fb0fc>|0x15c|+0x01d:'180'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6fb119>|0x179|+0x0d7:'185'
	// <0x6fb1f0>|0x250|+0x005:'186'
	// <0>
	// <0x6fb1f5>|0x255|+0x01f:'188'
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
	// <0x6fb214>|0x274|+0x0cb:'200'
	// <0>
	// <1>
	// <0x6fb2df>|0x33f|+0x03e:'203'
	// <0>
	// <0x6fb31d>|0x37d|+0x036|[1]:'205'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6fb353>|0x3b3|+0x084:'210'
	// <0>
	// <0x6fb3d7>|0x437|+0x03e:'212'
	// <0>
	// <0x6fb415>|0x475|+0x044:'214'
	// <0x6fb459>|0x4b9|+0x044:'215'
	// <0x6fb49d>|0x4fd|+0x025:'216'
	// <0x6fb4c2>|0x522|+0x04a:'217'
	// <0>
	// <0x6fb50c>|0x56c|+0x02a:'219'
	// <0x6fb536>|0x596|+0x024:'220'
	// <0x6fb55a>|0x5ba|+0x029:'221'
	// <0x6fb583>|0x5e3|+0x016:'222'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x6fb599>|0x5f9|+0x02f|[1]:'231'
	// <0x6fb5c8>|0x628|+0x02f:'232'
	// <0x6fb5f7>|0x657|+0x041:'233'
	// <0x6fb638>|0x698|+0x029:'234'
	// <0x6fb661>|0x6c1|+0x016:'235'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x6fb677>|0x6d7|+0x02f|[1]:'245'
	// <0x6fb6a6>|0x706|+0x02c:'246'
	// <0x6fb6d2>|0x732|+0x041:'247'
	// <0x6fb713>|0x773|+0x041:'248'
	// <0x6fb754>|0x7b4|+0x029:'249'
	// <0x6fb77d>|0x7dd|+0x013:'250'
	// <0x6fb790>|0x7f0|+0x010:'251'
	// <0>
	// <0x6fb7a0>|0x800|+0x01f:'253'
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
	// <0x6fb7bf>|0x81f|+0x0d4:'265'
	// <0>
	// <0x6fb893>|0x8f3|+0x017:'267'
	// <0>
	// <0x6fb8aa>|0x90a|+0x028:'269'
	// <0x6fb8d2>|0x932|+0x036:'270'
	// <0>
	// <0x6fb908>|0x968|+0x01b:'272'
	// <0>
	// <0x6fb923>|0x983|+0x03c:'274'
	// <0x6fb95f>|0x9bf|+0x045:'275'
	// <0x6fb9a4>|0xa04|+0x031:'276'
	// <0>
	// <1>
	// <2>
	// <0x6fb9d5>|0xa35|+0x02f:'280'
	// <0x6fba04>|0xa64|+0x02f:'281'
	// <0x6fba33>|0xa93|+0x02f:'282'
	// <0x6fba62>|0xac2|+0x023:'283'
	// <0x6fba85>|0xae5|+0x029:'284'
	// <0x6fbaae>|0xb0e|+0x023:'285'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// ******
}

// STATE[STUB]
// vostok::math::float4x4 survarium::legs_ik_processor::get_foot_fixed_transform(survarium::legs_ik_processor::leg_params const&, vostok::math::float4x4 const&, vostok::math::float4x4 const*, float&) const
float4x4 legs_ik_processor::get_foot_fixed_transform(
	legs_ik_processor::leg_params const&	params,
	float4x4 const&						hip_world_matrix,
	float4x4 const*						matrices,
	float&								delta_len
) const
{
	// LOCALS
	// float4x4 const& 				leg_world_matrix
	// float3 const& 				foot_to_toe_dir
	// float3 const& 				foot_to_leg_dir
	// float4x4 					result
	// float 						rotation_angle
	// float3 						foot_to_cube_center_offset
	// float3 						capsule_size
	// float4x4 const& 				foot_to_center_rel
	// float3 						start
	// math::color 					original_color
	// float 						leg_len
	// math::color 					fixed_color
	// float 						up_leg_to_original_foot_dist_sqr
	// float 						rotation_interpolation_koef
	// float 						up_leg_len
	// float4x4 const& 				knee_world_matrix
	// float3 						up_dir
	// float 						up_leg_to_fixed_foot_dist
	// float4x4 const& 				foot_world_matrix
	// float4x4 const& 				toe_world_matrix
	// float3 const& 				left_dir
	// float3 						finish
	// float 						knee_len
	// float4x4 const& 				up_leg_world_matrix
	// float4x4 					foot_center_transform
	// float 						position_iterpolation_koef<1>
	// float3 const& 				position<1>
	// ******

	// STATICS
	// static float 				dist_to_test = <0xa8e368>;
	// ******

	// SKIPPED BLOCKS
	// <0x6fc554><1>
	// ******

	// CALL SITE INFO
	// <0x6fc0b7> -> float <unknown>(float) const
	// <0x6fc1a1> -> float <unknown>(float) const
	// <0x6fc28b> -> float <unknown>(float) const
	// <0x6fc578> -> float <unknown>(float) const
	// ******

	return vostok::math::float4x4();

	// FUNCTION BODY
	// <0x6fbaf1>|0x011|+0x03b:'295'
	// <0x6fbb2c>|0x04c|+0x03e:'296'
	// <0x6fbb6a>|0x08a|+0x03e:'297'
	// <0x6fbba8>|0x0c8|+0x03d:'298'
	// <0x6fbbe5>|0x105|+0x03e:'299'
	// <0x6fbc23>|0x143|+0x078:'300'
	// <0x6fbc9b>|0x1bb|+0x018:'301'
	// <0>
	// <0x6fbcb3>|0x1d3|+0x04a:'303'
	// <0x6fbcfd>|0x21d|+0x044:'304'
	// <0x6fbd41>|0x261|+0x034:'305'
	// <0>
	// <0x6fbd75>|0x295|+0x008:'307'
	// <0x6fbd7d>|0x29d|+0x008:'308'
	// <0x6fbd85>|0x2a5|+0x01e:'309'
	// <0x6fbda3>|0x2c3|+0x01e:'310'
	// <0x6fbdc1>|0x2e1|+0x042:'311'
	// <0>
	// <0x6fbe03>|0x323|+0x015:'313'
	// <0x6fbe18>|0x338|+0x03e:'314'
	// <0x6fbe56>|0x376|+0x012:'315'
	// <0x6fbe68>|0x388|+0x012:'316'
	// <0x6fbe7a>|0x39a|+0x012:'317'
	// <0x6fbe8c>|0x3ac|+0x028:'318'
	// <0>
	// <0x6fbeb4>|0x3d4|+0x025:'320'
	// <0x6fbed9>|0x3f9|+0x02f:'321'
	// <0>
	// <0x6fbf08>|0x428|+0x029:'323'
	// <0x6fbf31>|0x451|+0x01d:'324'
	// <0>
	// <0x6fbf4e>|0x46e|+0x016:'326'
	// <0x6fbf64>|0x484|+0x032:'327'
	// <0x6fbf96>|0x4b6|+0x010:'328'
	// <0x6fbfa6>|0x4c6|+0x053:'329'
	// <0>
	// <0x6fbff9>|0x519|+0x050:'331'
	// <0x6fc049>|0x569|+0x050:'332'
	// <0x6fc099>|0x5b9|+0x02f:'333'
	// <0>
	// <0x6fc0c8>|0x5e8|+0x01b:'335'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6fc0e3>|0x603|+0x050:'340'
	// <0x6fc133>|0x653|+0x050:'341'
	// <0x6fc183>|0x6a3|+0x02f:'342'
	// <0>
	// <0x6fc1b2>|0x6d2|+0x01b:'344'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6fc1cd>|0x6ed|+0x050:'349'
	// <0x6fc21d>|0x73d|+0x050:'350'
	// <0x6fc26d>|0x78d|+0x026:'351'
	// <0>
	// <0x6fc293>|0x7b3|+0x005:'353'
	// <0>
	// <0x6fc298>|0x7b8|+0x050:'355'
	// <0x6fc2e8>|0x808|+0x022:'356'
	// <0x6fc30a>|0x82a|+0x013:'357'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6fc31d>|0x83d|+0x026:'364'
	// <0>
	// <0x6fc343>|0x863|+0x017:'366'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6fc35a>|0x87a|+0x022:'371'
	// <0>
	// <1>
	// <2>
	// <0x6fc37c>|0x89c|+0x030:'375'
	// <0>
	// <0x6fc3ac>|0x8cc|+0x017:'377'
	// <0>
	// <0x6fc3c3>|0x8e3|+0x022:'379'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6fc3e5>|0x905|+0x01a:'384'
	// <0>
	// <0x6fc3ff>|0x91f|+0x02f:'386'
	// <0x6fc42e>|0x94e|+0x02f:'387'
	// <0x6fc45d>|0x97d|+0x02e:'388'
	// <0>
	// <0x6fc48b>|0x9ab|+0x039:'390'
	// <0x6fc4c4>|0x9e4|+0x027:'391'
	// <0x6fc4eb>|0xa0b|+0x03e:'392'
	// <0x6fc529>|0xa49|+0x031:'393'
	// <0>
	// <0x6fc55a>|0xa7a|+0x026:'395'
	// <0x6fc580>|0xaa0|+0x080:'396'
	// <0x6fc600>|0xb20|+0x021:'397'
	// <0>
	// <0x6fc621>|0xb41|+0x013:'399'
	// ******
}

// STATE[STUB]
// void survarium::legs_ik_processor::set_left_heel_on_ground(bool)
void legs_ik_processor::set_left_heel_on_ground( bool value )
{
	// FUNCTION BODY
	// <0x6fad57>|0x007|+0x014:'404'
	// ******
}

// STATE[STUB]
// void survarium::legs_ik_processor::set_left_toe_on_ground(bool)
void legs_ik_processor::set_left_toe_on_ground( bool value )
{
	// FUNCTION BODY
	// <0x6fad27>|0x007|+0x014:'409'
	// ******
}

// STATE[STUB]
// void survarium::legs_ik_processor::set_right_heel_on_ground(bool)
void legs_ik_processor::set_right_heel_on_ground( bool value )
{
	// FUNCTION BODY
	// <0x6facf7>|0x007|+0x014:'414'
	// ******
}

// STATE[STUB]
// void survarium::legs_ik_processor::set_right_toe_on_ground(bool)
void legs_ik_processor::set_right_toe_on_ground( bool value )
{
	// FUNCTION BODY
	// <0x6facc7>|0x007|+0x014:'419'
	// ******
}

// STATE[STUB]
// void survarium::legs_ik_processor::set_heel_on_ground(survarium::legs_ik_processor::leg_params&, bool)
void legs_ik_processor::set_heel_on_ground( legs_ik_processor::leg_params& params, bool value )
{
	// FUNCTION BODY
	// <0x6fab79>|0x009|+0x015:'424'
	// <0x6fab8e>|0x01e|+0x005:'425'
	// <0x6fab93>|0x023|+0x00d:'426'
	// <0>
	// <0x6faba0>|0x030|+0x008:'428'
	// <0>
	// <0x6faba8>|0x038|+0x00f:'430'
	// <0x6fabb7>|0x047|+0x013:'431'
	// <0>
	// <0x6fabca>|0x05a|+0x005:'433'
	// <0>
	// <0x6fabcf>|0x05f|+0x020:'435'
	// <0x6fabef>|0x07f|+0x022:'436'
	// <0x6fac11>|0x0a1|+0x018:'437'
	// <0x6fac29>|0x0b9|+0x018:'438'
	// <0x6fac41>|0x0d1|+0x049:'439'
	// <0x6fac8a>|0x11a|+0x00f:'440'
	// <0x6fac99>|0x129|+0x013:'441'
	// <0>
	// ******
}

// STATE[STUB]
// void survarium::legs_ik_processor::set_toe_on_ground(survarium::legs_ik_processor::leg_params&, bool)
void legs_ik_processor::set_toe_on_ground( legs_ik_processor::leg_params& params, bool value )
{
	// FUNCTION BODY
	// <0x6faa79>|0x009|+0x015:'447'
	// <0x6faa8e>|0x01e|+0x005:'448'
	// <0x6faa93>|0x023|+0x00d:'449'
	// <0x6faaa0>|0x030|+0x00c:'450'
	// <0>
	// <0x6faaac>|0x03c|+0x020:'452'
	// <0x6faacc>|0x05c|+0x022:'453'
	// <0x6faaee>|0x07e|+0x018:'454'
	// <0x6fab06>|0x096|+0x018:'455'
	// <0x6fab1e>|0x0ae|+0x049:'456'
	// <0>
	// ******
}

// STATE[STUB]
// void survarium::legs_ik_processor::tick(const unsigned int)
void legs_ik_processor::tick( u32 current_time_in_ms )
{
	// LOCALS
	// float 						dt_sec<1>
	// ******

	// SKIPPED BLOCKS
	// <0x6fa9e0><1>
	// ******

	// FUNCTION BODY
	// <0x6fa9d9>|0x009|+0x009:'462'
	// <0>
	// <0x6fa9e2>|0x012|+0x01f:'464'
	// <0x6faa01>|0x031|+0x012:'465'
	// <0x6faa13>|0x043|+0x012:'466'
	// <0x6faa25>|0x055|+0x01e:'467'
	// <0x6faa43>|0x073|+0x01e:'468'
	// <0>
	// <0x6faa61>|0x091|+0x009:'470'
	// ******
}

} // namespace survarium
