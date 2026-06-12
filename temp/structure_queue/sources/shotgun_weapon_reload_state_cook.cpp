////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\game\sources\shotgun_weapon_reload_state_cook.h"

namespace survarium {

// STATE[STUB]
 shotgun_weapon_reload_state_cook::shotgun_weapon_reload_state_cook( )
{
	// FUNCTION BODY[0x5bd6f0]: 1
	// <0x5bd728>|0x038|+0x026:'25'
	// ******
}

// STATE[STUB]
 shotgun_weapon_reload_state_cook::~shotgun_weapon_reload_state_cook( )
{
	// FUNCTION BODY[0x5bd6e0]: 1
	// <0x5bd6e0>|0x000|+0x000:'29'	{
	// <0>
	// <0x5bd6e0>|0x000|      :'31'	}
	// ******
}

// STATE[STUB]
mutable_buffer shotgun_weapon_reload_state_cook::allocate_resource(
	resources::query_result_for_cook&		in_query,
	const_buffer							raw_file_data,
	bool									file_exist
)
{
	// FUNCTION BODY[0x5bd780]: 2
	// <0>
	// <0x5bd787>|0x007|+0x023:'36'
	// ******
}

// STATE[STUB]
void shotgun_weapon_reload_state_cook::deallocate_resource( void* buffer )
{
	// FUNCTION BODY[0x5bd760]: 1
	// <0x5bd760>|0x000|+0x01c:'41'
	// ******
}

// STATE[STUB]
void shotgun_weapon_reload_state_cook::create_resource(
	resources::query_result_for_cook&		parent,
	const_buffer							raw_file_data,
	mutable_buffer							in_out_unmanaged_resource_buffer
)
{
	// LOCALS
	// variant< 32 > const*[3] 			user_data_ptrs
	// configs::binary_config_value 	cfg
	// variant< 32 >[3] 				user_data
	// weapon_state_creation_params const* params
	// resources::creation_request[3] 	requests
	// ******

	// FUNCTION BODY[0x5bd9a0]: 36
	// <0x5bd9a0>|0x000|+0x015:'45'	{
	// <0x5bd9b5>|0x015|+0x00c:'46'
	// <0x5bd9c1>|0x021|+0x01a:'47'
	// <0x5bd9db>|0x03b|+0x028:'48'
	// <0x5bda03>|0x063|+0x001:'49'
	// <0x5bda04>|0x064|+0x016:'50'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x5bda1a>|0x07a|+0x006:'60'
	// <0>
	// <1>
	// <2>
	// <0x5bda20>|0x080|+0x090:'64'
	// <0>
	// <0x5bdab0>|0x110|+0x069:'66'
	// <0>
	// <1>
	// <0x5bdb19>|0x179|+0x00b:'69'
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
	// <0x5bdb24>|0x184|+0x0e2:'80'
	// <0x5bdc06>|0x266|-0x1f5:'81'
	// <0x5bda11>|0x071|+0x1ff:'82'
	// <0x5bdc10>|0x270|      :'82'	}
	// ******
}

// STATE[STUB]
void shotgun_weapon_reload_state_cook::on_substates_ready(
	resources::queries_result&				data,
	mutable_buffer							buffer,
	weapon_state_creation_params const*		params
)
{
	// LOCALS
	// weapon_core_shotgun_reload_base_substate_ptr reload_one_substate
	// weapon_core_shotgun_reload_base_substate_ptr start_substate
	// weapon_core_shotgun_reload_base_substate_ptr finish_substate
	// ******

	// FUNCTION BODY[0x5bd7c0]: 10
	// <0>
	// <0x5bd7c9>|0x009|+0x058:'87'
	// <0x5bd821>|0x061|+0x050:'88'
	// <0x5bd871>|0x0b1|+0x04e:'89'
	// <0>
	// <1>
	// <2>
	// <0x5bd8bf>|0x0ff|+0x022:'93'
	// <0x5bd8e1>|0x121|+0x03c:'94'
	// <0x5bd91d>|0x15d|+0x00e:'95'
	// ******
}

// STATE[STUB]
void shotgun_weapon_reload_state_cook::destroy_resource( resources::unmanaged_resource* resource )
{
	// CALL SITE INFO
	// <0x5bd6db> -> void* < unknown >( u32 )
	// ******

	// FUNCTION BODY[0x5bd6c0]: 1
	// <0x5bd6c0>|0x000|+0x00f:'100'
	// ******
}

} // namespace survarium
