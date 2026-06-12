////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\game\sources\fingers_to_weapon_corrector.h"

namespace survarium {

// STATE[STUB]
 fingers_to_weapon_corrector::fingers_to_weapon_corrector( )
{
	// FUNCTION BODY[0x5bca00]: 0
	// <0x5bca00>|0x000|+0x03a:'47'	{
	// <0x5bca3a>|0x03a|      :'48'	}
	// ******
}

// STATE[STUB]
void fingers_to_weapon_corrector::activate(
	animation::skeleton const&				character_skeleton,
	render::render_model_instance_ptr		weapon_model,
	const bool								first_person_view
)
{
	// FUNCTION BODY[0x5bc9b0]: 5
	// <0>
	// <1>
	// <2>
	// <0x5bc9b1>|0x001|+0x008:'55'
	// <0x5bc9b9>|0x009|+0x010:'56'
	// ******
}

// STATE[STUB]
void fingers_to_weapon_corrector::initialize_bones_indices( animation::skeleton const& character_skeleton )
{
	// FUNCTION BODY[0x5bc7f0]: 5
	// <0x5bc7fa>|0x00a|+0x026:'61'
	// <0x5bc820>|0x030|+0x010:'62'
	// <0x5bc830>|0x040|+0x07e:'63'
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
void fingers_to_weapon_corrector::initialize_locators( render::render_model_instance const& weapon_model, const bool first_person_view )
{
	// LOCALS
	// float4x4[16] 					matrices
	// float4x4[16] 					inverted_matrices
	// render::model_locator_item 		current_item
	// ******

	// TYPEDEFS
	// typedef
	// 	fixed_string< 256 >
	// 	locator_name_string_type;

	// ******

	// CALL SITE INFO
	// <0x5bc724> -> bool < unknown >( pcstr, render::model_locator_item& ) const
	// ******

	// FUNCTION BODY[0x5bc6d0]: 25
	// <0x5bc6da>|0x00a|+0x006:'70'
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
	// <0x5bc6e0>|0x010|+0x011:'81'
	// <0x5bc6f1>|0x021|+0x002:'82'
	// <0x5bc6f3>|0x023|+0x015:'83'
	// <0x5bc708>|0x038|+0x01e:'84'
	// <0>
	// <0x5bc726>|0x056|+0x017:'86'
	// <0x5bc73d>|0x06d|+0x01b:'87'
	// <0>
	// <1>
	// <0x5bc758>|0x088|+0x01c:'90'
	// <0x5bc774>|0x0a4|+0x037:'91'
	// <0>
	// <0x5bc7ab>|0x0db|+0x02f:'93'
	// <0>
	// ******
}

// STATE[STUB]
void interpolate_hand_matrices(
	float4x4 const*		locator_matrices,
	u32 const*			bone_indices,
	const u32			phalanges_count,
	const float			iterpolation_coeff,
	float4x4*			result_matrices
)
{
	// FUNCTION BODY[0xacde0]: 3
	// <0xacde5>|0x005|+0x008:'107'
	// <0xacded>|0x00d|+0x04c:'108'
	// <0>
	// ******
}

// STATE[STUB]
void fingers_to_weapon_corrector::process( const u32 current_time_in_ms, float4x4* matrices ) const
{
	// CALL SITE INFO
	// <0x5bc963> -> float < unknown >( float ) const
	// <0x5bc96b> -> float < unknown >( float ) const
	// ******

	// FUNCTION BODY[0x5bc8c0]: 10
	// <0x5bc8cc>|0x00c|+0x024:'114'
	// <0x5bc8f0>|0x030|+0x00d:'115'
	// <0x5bc8fd>|0x03d|+0x009:'116'
	// <0x5bc906>|0x046|+0x02a:'117'
	// <0>
	// <0x5bc930>|0x070|+0x00a:'119'
	// <0x5bc93a>|0x07a|+0x033:'120'
	// <0x5bc96d>|0x0ad|+0x033:'121'
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
float fingers_to_weapon_corrector::get_hand_coefficient( const float hand_transition_time, const bool hand_active ) const
{
	// LOCALS
	// const float 						interpolation_coeff
	// ******

	// CALL SITE INFO
	// <0x5bc6b2> -> float < unknown >( float ) const
	// ******

	return 0.0f;

	// FUNCTION BODY[0x5bc6a0]: 4
	// <0>
	// <1>
	// <0x5bc6a0>|0x000|+0x023:'130'
	// <0x5bc6c3>|0x023|+0x006:'131'
	// ******
}

// STATE[STUB]
void fingers_to_weapon_corrector::activate_hand(
	const fingers_to_weapon_corrector::hands_enum	arg_0 /* fingers_to_weapon_corrector::hands_enum hand */,
	const bool		is_active,
	const u32		current_time_in_ms
)
{
	// FUNCTION BODY[0x5bc670]: 4
	// <0x5bc670>|0x000|+0x014:'141'
	// <0x5bc684>|0x014|+0x006:'142'
	// <0x5bc68a>|0x01a|+0x00a:'143'
	// <0>
	// ******
}

} // namespace survarium
