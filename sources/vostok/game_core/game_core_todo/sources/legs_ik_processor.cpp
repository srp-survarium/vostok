////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "legs_ik_processor.h"

namespace survarium {

// STATE[STUB]
// void survarium::`dynamic initializer for 's_ik_legs_debug_draw_cc''()
void `dynamic initializer for 's_ik_legs_debug_draw_cc''( )
{
}

// STATE[STUB]
// void survarium::`dynamic initializer for 's_ik_foot_capsule_radius_cc''()
void `dynamic initializer for 's_ik_foot_capsule_radius_cc''( )
{
}

// STATE[STUB]
// void survarium::`dynamic initializer for 's_ik_legs_rot_axis_cc''()
void `dynamic initializer for 's_ik_legs_rot_axis_cc''( )
{
}

// STATE[STUB]
// void survarium::`dynamic atexit destructor for 's_ik_legs_debug_draw_cc''()
void `dynamic atexit destructor for 's_ik_legs_debug_draw_cc''( )
{
}

// STATE[STUB]
// survarium::legs_ik_processor::leg_params::leg_params()
legs_ik_processor::leg_params::leg_params( )
{
}

// STATE[STUB]
// void survarium::legs_ik_processor::leg_params::activate(vostok::animation::skeleton const&, char const*)
void legs_ik_processor::leg_params::activate( vostok::animation::skeleton const& skeleton, pcstr foot_bone_name )
{
	// LOCALS
	// vostok::animation::skeleton_bone const& foot_bone
	// ******

	// FUNCTION BODY
	// <0x6fad89>|0x000|0x000:'43'
	// <0x6fad9f>|0x016|0x016:'44'
	// <0x6fadcd>|0x044|0x02e:'45'
	// <0x6fae07>|0x07e|0x03a:'46'
	// <0x6fae3f>|0x0b6|0x038:'47'
	// <0x6fae80>|0x0f7|0x041:'48'
	// ******
}

// STATE[STUB]
// void survarium::legs_ik_processor::~legs_ik_processor()
void legs_ik_processor::~legs_ik_processor( )
{
	// FUNCTION BODY
	// <0x6faf49>|0x000|0x000:'53'
	// ******
}

// STATE[STUB]
// void survarium::legs_ik_processor::leg_params::tick(float)
void legs_ik_processor::leg_params::tick( float dt )
{
	// FUNCTION BODY
	// <0x6fa969>|0x000|0x000:'64'
	// <0x6fa98b>|0x022|0x022:'65'
	// <0x6fa9ad>|0x044|0x022:'66'
	// ******
}

// STATE[STUB]
// void survarium::legs_ik_processor::leg_params::set_heel_transition_time(float)
void legs_ik_processor::leg_params::set_heel_transition_time( float tr_time )
{
	// FUNCTION BODY
	// <0x6fa939>|0x000|0x000:'71'
	// ******
}

// STATE[STUB]
// void survarium::legs_ik_processor::leg_params::set_toe_transition_time(float)
void legs_ik_processor::leg_params::set_toe_transition_time( float tr_time )
{
	// FUNCTION BODY
	// <0x6fa909>|0x000|0x000:'76'
	// ******
}

// STATE[STUB]
// void survarium::legs_ik_processor::leg_params::set_heel_on_ground(const bool)
void legs_ik_processor::leg_params::set_heel_on_ground( bool value )
{
	// FUNCTION BODY
	// <0x6fa899>|0x000|0x000:'81'
	// <0x6fa8a2>|0x009|0x009:'82'
	// <0x6fa8dc>|0x043|0x03a:'83'
	// ******
}

// STATE[STUB]
// void survarium::legs_ik_processor::leg_params::set_toe_on_ground(const bool)
void legs_ik_processor::leg_params::set_toe_on_ground( bool value )
{
	// FUNCTION BODY
	// <0x6fa829>|0x000|0x000:'88'
	// <0x6fa832>|0x009|0x009:'89'
	// <0x6fa86c>|0x043|0x03a:'90'
	// ******
}

// STATE[STUB]
// survarium::legs_ik_processor::legs_ik_processor()
legs_ik_processor::legs_ik_processor( )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6e97ea }, type_index: TypeIndex(0x688a) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6e9804 }, type_index: TypeIndex(0x688a) })
	// ******

}

// STATE[STUB]
// void survarium::legs_ik_processor::activate(vostok::animation::skeleton const&)
void legs_ik_processor::activate( vostok::animation::skeleton const& skeleton )
{
	// FUNCTION BODY
	// <0x6faed9>|0x000|0x000:'105'
	// <0x6faee5>|0x00c|0x00c:'106'
	// <0x6faef9>|0x020|0x014:'107'
	// <0x6faf0d>|0x034|0x014:'108'
	// ******
}

// STATE[STUB]
// float survarium::get_additional_length(vostok::math::float3 const&, vostok::math::float3 const&, float)
float get_additional_length( vostok::math::float3 const& upleg_dir, vostok::math::float3 const& leg_dir, float knee_len )
{
	// LOCALS
	// float 						knee_angle_cos
	// ******

	return 0.0f;
	// FUNCTION BODY
	// <0xcb1f6>|0x000|0x000:'113'
	// <0xcb20e>|0x018|0x018:'114'
	// ******
}

// STATE[STUB]
// void survarium::legs_ik_processor::process(vostok::math::float4x4*, vostok::math::float4x4 const&)
void legs_ik_processor::process( vostok::math::float4x4* matrices, vostok::math::float4x4 const& transform )
{
	// LOCALS
	// vostok::math::float4x4 		inverted_transform
	// float 						right_delta_len
	// float 						left_delta_len
	// vostok::math::float4x4 		hip_obj_matrix
	// vostok::math::float4x4 const& right_foot_fixed_transform
	// vostok::math::float4x4 const& hip_world_matrix
	// bool 						success
	// vostok::math::float4x4 const& left_foot_fixed_transform
	// ******

	// FUNCTION BODY
	// <0x6fc651>|0x000|0x000:'119'
	// <0x6fc67a>|0x029|0x029:'120'
	// <0x6fc69d>|0x04c|0x023:'121'
	// <0x6fc6ad>|0x05c|0x010:'122'
	// <0x6fc6db>|0x08a|0x02e:'123'
	// <0x6fc6eb>|0x09a|0x010:'124'
	// 1
	// <0x6fc722>|0x0d1|0x037:'126'
	// <0x6fc72d>|0x0dc|0x00b:'127'
	// <0x6fc73f>|0x0ee|0x012:'128'
	// 1
	// <0x6fc767>|0x116|0x028:'130'
	// 1
	// <0x6fc79c>|0x14b|0x035:'132'
	// <0x6fc7db>|0x18a|0x03f:'133'
	// 1
	// <0x6fc822>|0x1d1|0x047:'135'
	// 1
	// 2
	// <0x6fc8b4>|0x263|0x092:'138'
	// 1
	// <0x6fc8cc>|0x27b|0x018:'140'
	// 1
	// 2
	// <0x6fc904>|0x2b3|0x038:'143'
	// <0x6fc943>|0x2f2|0x03f:'144'
	// 1
	// <0x6fc98a>|0x339|0x047:'146'
	// 1
	// 2
	// <0x6fca1c>|0x3cb|0x092:'149'
	// 1
	// <0x6fca34>|0x3e3|0x018:'151'
	// 1
	// 2
	// <0x6fca6c>|0x41b|0x038:'154'
	// <0x6fcaab>|0x45a|0x03f:'155'
	// 1
	// <0x6fcaf2>|0x4a1|0x047:'157'
	// 1
	// 2
	// 3
	// ******
}

// STATE[STUB]
// void survarium::legs_ik_processor::process_leg(survarium::legs_ik_processor::leg_params&, vostok::math::float4x4 const&, vostok::math::float4x4 const&, vostok::math::float4x4*, vostok::math::float4x4 const&)
void legs_ik_processor::process_leg(
	legs_ik_processor::leg_params&		params,
	vostok::math::float4x4 const&		target_foot_obj_matrix,
	vostok::math::float4x4 const&		hip_obj_matrix,
	vostok::math::float4x4*				matrices,
	vostok::math::float4x4 const&		transform)
{
	// LOCALS
	// vostok::math::float4x4 		up_leg_obj_matrix
	// vostok::math::float4x4 		foot_obj_matrix
	// vostok::math::float3 const& 	target_up_leg_to_foot_dir
	// u32 							up_leg_matrix_index
	// u32 							leg_matrix_index
	// vostok::math::float4x4 		knee_obj_matrix
	// vostok::math::float4x4 		toe_obj_matrix
	// u32 							foot_matrix_index
	// float 						leg_len
	// u32 							toe_matrix_index
	// vostok::math::float3 		foot_pos
	// float 						up_leg_len
	// vostok::math::float4x4 		leg_obj_matrix
	// u32 							knee_matrix_index
	// float 						knee_len
	// vostok::math::float3 const& 	original_up_leg_to_foot_dir<1>
	// float 						up_leg_alpha_angle<1>
	// float 						additive_len<1>
	// float 						up_leg_to_foot_len<1>
	// vostok::math::float3 const& 	original_up_leg_dir<1>
	// vostok::math::float3 const& 	target_up_leg_dir<1>
	// vostok::math::float4x4 const& rotation_matrix<1>
	// vostok::math::float4x4 const& alpha_rotation_matrix<1>
	// vostok::math::float4x4 const& rotation_matrix<1>
	// vostok::math::float3 const& 	original_knee_dir<1>
	// vostok::math::float3 const& 	target_leg_dir<1>
	// vostok::math::float4x4 const& rotation_matrix<1>
	// vostok::math::float3 const& 	original_leg_dir<1>
	// vostok::math::float3 const& 	foot_pos<1>
	// vostok::math::float3 const& 	toe_pos<1>
	// u32 							toe_matrix_index<1>
	// ******

	// SKIPPED BLOCKS
	// <0x6fb8a8><1>
	// <0x6fb91d><1>
	// ******

	// FUNCTION BODY
	// <0x6fafb1>|0x000|0x000:'166'
	// <0x6fafc9>|0x018|0x018:'167'
	// <0x6fafe0>|0x02f|0x017:'168'
	// <0x6faffb>|0x04a|0x01b:'169'
	// <0x6fb013>|0x062|0x018:'170'
	// 1
	// <0x6fb02e>|0x07d|0x01b:'172'
	// <0x6fb048>|0x097|0x01a:'173'
	// <0x6fb065>|0x0b4|0x01d:'174'
	// 1
	// <0x6fb07f>|0x0ce|0x01a:'176'
	// <0x6fb09f>|0x0ee|0x020:'177'
	// <0x6fb0bf>|0x10e|0x020:'178'
	// <0x6fb0df>|0x12e|0x020:'179'
	// <0x6fb0fc>|0x14b|0x01d:'180'
	// 1
	// 2
	// 3
	// 4
	// <0x6fb119>|0x168|0x01d:'185'
	// <0x6fb1f0>|0x23f|0x0d7:'186'
	// 1
	// <0x6fb1f5>|0x244|0x005:'188'
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
	// <0x6fb214>|0x263|0x01f:'200'
	// 1
	// 2
	// <0x6fb2df>|0x32e|0x0cb:'203'
	// 1
	// <0x6fb31d>|0x36c|0x03e|[1]:'205'
	// 1
	// 2
	// 3
	// 4
	// <0x6fb353>|0x3a2|0x036:'210'
	// 1
	// <0x6fb3d7>|0x426|0x084:'212'
	// 1
	// <0x6fb415>|0x464|0x03e:'214'
	// <0x6fb459>|0x4a8|0x044:'215'
	// <0x6fb49d>|0x4ec|0x044:'216'
	// <0x6fb4c2>|0x511|0x025:'217'
	// 1
	// <0x6fb50c>|0x55b|0x04a:'219'
	// <0x6fb536>|0x585|0x02a:'220'
	// <0x6fb55a>|0x5a9|0x024:'221'
	// <0x6fb583>|0x5d2|0x029:'222'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// 8
	// <0x6fb599>|0x5e8|0x016|[1]:'231'
	// <0x6fb5c8>|0x617|0x02f:'232'
	// <0x6fb5f7>|0x646|0x02f:'233'
	// <0x6fb638>|0x687|0x041:'234'
	// <0x6fb661>|0x6b0|0x029:'235'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// 8
	// 9
	// <0x6fb677>|0x6c6|0x016|[1]:'245'
	// <0x6fb6a6>|0x6f5|0x02f:'246'
	// <0x6fb6d2>|0x721|0x02c:'247'
	// <0x6fb713>|0x762|0x041:'248'
	// <0x6fb754>|0x7a3|0x041:'249'
	// <0x6fb77d>|0x7cc|0x029:'250'
	// <0x6fb790>|0x7df|0x013:'251'
	// 1
	// <0x6fb7a0>|0x7ef|0x010:'253'
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
	// <0x6fb7bf>|0x80e|0x01f:'265'
	// 1
	// <0x6fb893>|0x8e2|0x0d4:'267'
	// 1
	// <0x6fb8aa>|0x8f9|0x017:'269'
	// <0x6fb8d2>|0x921|0x028:'270'
	// 1
	// <0x6fb908>|0x957|0x036:'272'
	// 1
	// <0x6fb923>|0x972|0x01b:'274'
	// <0x6fb95f>|0x9ae|0x03c:'275'
	// <0x6fb9a4>|0x9f3|0x045:'276'
	// 1
	// 2
	// 3
	// <0x6fb9d5>|0xa24|0x031:'280'
	// <0x6fba04>|0xa53|0x02f:'281'
	// <0x6fba33>|0xa82|0x02f:'282'
	// <0x6fba62>|0xab1|0x02f:'283'
	// <0x6fba85>|0xad4|0x023:'284'
	// <0x6fbaae>|0xafd|0x029:'285'
	// 1
	// 2
	// 3
	// 4
	// 5
	// ******
}

// STATE[STUB]
// vostok::math::float4x4 survarium::legs_ik_processor::get_foot_fixed_transform(survarium::legs_ik_processor::leg_params const&, vostok::math::float4x4 const&, vostok::math::float4x4 const*, float&) const
vostok::math::float4x4 legs_ik_processor::get_foot_fixed_transform(
	legs_ik_processor::leg_params const&	params,
	vostok::math::float4x4 const&		hip_world_matrix,
	vostok::math::float4x4 const*		matrices,
	float&								delta_len) const
{
	// LOCALS
	// vostok::math::float4x4 const& leg_world_matrix
	// vostok::math::float3 const& 	foot_to_toe_dir
	// vostok::math::float3 const& 	foot_to_leg_dir
	// vostok::math::float4x4 		result
	// float 						rotation_angle
	// vostok::math::float3 		foot_to_cube_center_offset
	// vostok::math::float3 		capsule_size
	// vostok::math::float4x4 const& foot_to_center_rel
	// vostok::math::float3 		start
	// vostok::math::color 			original_color
	// float 						leg_len
	// vostok::math::color 			fixed_color
	// float 						up_leg_to_original_foot_dist_sqr
	// float 						rotation_interpolation_koef
	// float 						up_leg_len
	// vostok::math::float4x4 const& knee_world_matrix
	// vostok::math::float3 		up_dir
	// float 						up_leg_to_fixed_foot_dist
	// vostok::math::float4x4 const& foot_world_matrix
	// vostok::math::float4x4 const& toe_world_matrix
	// vostok::math::float3 const& 	left_dir
	// vostok::math::float3 		finish
	// float 						knee_len
	// vostok::math::float4x4 const& up_leg_world_matrix
	// vostok::math::float4x4 		foot_center_transform
	// float 						position_iterpolation_koef<1>
	// vostok::math::float3 const& 	position<1>
	// ******

	// STATICS
	// static float 				dist_to_test = <0xa8e368>;
	// ******

	// SKIPPED BLOCKS
	// <0x6fc554><1>
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6eb0b7 }, type_index: TypeIndex(0x6889) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6eb1a1 }, type_index: TypeIndex(0x6889) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6eb28b }, type_index: TypeIndex(0x6889) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6eb578 }, type_index: TypeIndex(0x6889) })
	// ******

	return vostok::math::float4x4();
	// FUNCTION BODY
	// <0x6fbaf1>|0x000|0x000:'295'
	// <0x6fbb2c>|0x03b|0x03b:'296'
	// <0x6fbb6a>|0x079|0x03e:'297'
	// <0x6fbba8>|0x0b7|0x03e:'298'
	// <0x6fbbe5>|0x0f4|0x03d:'299'
	// <0x6fbc23>|0x132|0x03e:'300'
	// <0x6fbc9b>|0x1aa|0x078:'301'
	// 1
	// <0x6fbcb3>|0x1c2|0x018:'303'
	// <0x6fbcfd>|0x20c|0x04a:'304'
	// <0x6fbd41>|0x250|0x044:'305'
	// 1
	// <0x6fbd75>|0x284|0x034:'307'
	// <0x6fbd7d>|0x28c|0x008:'308'
	// <0x6fbd85>|0x294|0x008:'309'
	// <0x6fbda3>|0x2b2|0x01e:'310'
	// <0x6fbdc1>|0x2d0|0x01e:'311'
	// 1
	// <0x6fbe03>|0x312|0x042:'313'
	// <0x6fbe18>|0x327|0x015:'314'
	// <0x6fbe56>|0x365|0x03e:'315'
	// <0x6fbe68>|0x377|0x012:'316'
	// <0x6fbe7a>|0x389|0x012:'317'
	// <0x6fbe8c>|0x39b|0x012:'318'
	// 1
	// <0x6fbeb4>|0x3c3|0x028:'320'
	// <0x6fbed9>|0x3e8|0x025:'321'
	// 1
	// <0x6fbf08>|0x417|0x02f:'323'
	// <0x6fbf31>|0x440|0x029:'324'
	// 1
	// <0x6fbf4e>|0x45d|0x01d:'326'
	// <0x6fbf64>|0x473|0x016:'327'
	// <0x6fbf96>|0x4a5|0x032:'328'
	// <0x6fbfa6>|0x4b5|0x010:'329'
	// 1
	// <0x6fbff9>|0x508|0x053:'331'
	// <0x6fc049>|0x558|0x050:'332'
	// <0x6fc099>|0x5a8|0x050:'333'
	// 1
	// <0x6fc0c8>|0x5d7|0x02f:'335'
	// 1
	// 2
	// 3
	// 4
	// <0x6fc0e3>|0x5f2|0x01b:'340'
	// <0x6fc133>|0x642|0x050:'341'
	// <0x6fc183>|0x692|0x050:'342'
	// 1
	// <0x6fc1b2>|0x6c1|0x02f:'344'
	// 1
	// 2
	// 3
	// 4
	// <0x6fc1cd>|0x6dc|0x01b:'349'
	// <0x6fc21d>|0x72c|0x050:'350'
	// <0x6fc26d>|0x77c|0x050:'351'
	// 1
	// <0x6fc293>|0x7a2|0x026:'353'
	// 1
	// <0x6fc298>|0x7a7|0x005:'355'
	// <0x6fc2e8>|0x7f7|0x050:'356'
	// <0x6fc30a>|0x819|0x022:'357'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// <0x6fc31d>|0x82c|0x013:'364'
	// 1
	// <0x6fc343>|0x852|0x026:'366'
	// 1
	// 2
	// 3
	// 4
	// <0x6fc35a>|0x869|0x017:'371'
	// 1
	// 2
	// 3
	// <0x6fc37c>|0x88b|0x022:'375'
	// 1
	// <0x6fc3ac>|0x8bb|0x030:'377'
	// 1
	// <0x6fc3c3>|0x8d2|0x017:'379'
	// 1
	// 2
	// 3
	// 4
	// <0x6fc3e5>|0x8f4|0x022:'384'
	// 1
	// <0x6fc3ff>|0x90e|0x01a:'386'
	// <0x6fc42e>|0x93d|0x02f:'387'
	// <0x6fc45d>|0x96c|0x02f:'388'
	// 1
	// <0x6fc48b>|0x99a|0x02e:'390'
	// <0x6fc4c4>|0x9d3|0x039:'391'
	// <0x6fc4eb>|0x9fa|0x027:'392'
	// <0x6fc529>|0xa38|0x03e:'393'
	// 1
	// <0x6fc55a>|0xa69|0x031:'395'
	// <0x6fc580>|0xa8f|0x026:'396'
	// <0x6fc600>|0xb0f|0x080:'397'
	// 1
	// <0x6fc621>|0xb30|0x021:'399'
	// ******
}

// STATE[STUB]
// void survarium::legs_ik_processor::set_left_heel_on_ground(bool)
void legs_ik_processor::set_left_heel_on_ground( bool value )
{
	// FUNCTION BODY
	// <0x6fad57>|0x000|0x000:'404'
	// ******
}

// STATE[STUB]
// void survarium::legs_ik_processor::set_left_toe_on_ground(bool)
void legs_ik_processor::set_left_toe_on_ground( bool value )
{
	// FUNCTION BODY
	// <0x6fad27>|0x000|0x000:'409'
	// ******
}

// STATE[STUB]
// void survarium::legs_ik_processor::set_right_heel_on_ground(bool)
void legs_ik_processor::set_right_heel_on_ground( bool value )
{
	// FUNCTION BODY
	// <0x6facf7>|0x000|0x000:'414'
	// ******
}

// STATE[STUB]
// void survarium::legs_ik_processor::set_right_toe_on_ground(bool)
void legs_ik_processor::set_right_toe_on_ground( bool value )
{
	// FUNCTION BODY
	// <0x6facc7>|0x000|0x000:'419'
	// ******
}

// STATE[STUB]
// void survarium::legs_ik_processor::set_heel_on_ground(survarium::legs_ik_processor::leg_params&, bool)
void legs_ik_processor::set_heel_on_ground( legs_ik_processor::leg_params& params, bool value )
{
	// FUNCTION BODY
	// <0x6fab79>|0x000|0x000:'424'
	// <0x6fab8e>|0x015|0x015:'425'
	// <0x6fab93>|0x01a|0x005:'426'
	// 1
	// <0x6faba0>|0x027|0x00d:'428'
	// 1
	// <0x6faba8>|0x02f|0x008:'430'
	// <0x6fabb7>|0x03e|0x00f:'431'
	// 1
	// <0x6fabca>|0x051|0x013:'433'
	// 1
	// <0x6fabcf>|0x056|0x005:'435'
	// <0x6fabef>|0x076|0x020:'436'
	// <0x6fac11>|0x098|0x022:'437'
	// <0x6fac29>|0x0b0|0x018:'438'
	// <0x6fac41>|0x0c8|0x018:'439'
	// <0x6fac8a>|0x111|0x049:'440'
	// <0x6fac99>|0x120|0x00f:'441'
	// 1
	// ******
}

// STATE[STUB]
// void survarium::legs_ik_processor::set_toe_on_ground(survarium::legs_ik_processor::leg_params&, bool)
void legs_ik_processor::set_toe_on_ground( legs_ik_processor::leg_params& params, bool value )
{
	// FUNCTION BODY
	// <0x6faa79>|0x000|0x000:'447'
	// <0x6faa8e>|0x015|0x015:'448'
	// <0x6faa93>|0x01a|0x005:'449'
	// <0x6faaa0>|0x027|0x00d:'450'
	// 1
	// <0x6faaac>|0x033|0x00c:'452'
	// <0x6faacc>|0x053|0x020:'453'
	// <0x6faaee>|0x075|0x022:'454'
	// <0x6fab06>|0x08d|0x018:'455'
	// <0x6fab1e>|0x0a5|0x018:'456'
	// 1
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
	// <0x6fa9d9>|0x000|0x000:'462'
	// 1
	// <0x6fa9e2>|0x009|0x009:'464'
	// <0x6faa01>|0x028|0x01f:'465'
	// <0x6faa13>|0x03a|0x012:'466'
	// <0x6faa25>|0x04c|0x012:'467'
	// <0x6faa43>|0x06a|0x01e:'468'
	// 1
	// <0x6faa61>|0x088|0x01e:'470'
	// ******
}

	/* TYPEDEFS

	typedef
		vostok::render::terrain_data*
		iterator_type;

} // namespace survarium
