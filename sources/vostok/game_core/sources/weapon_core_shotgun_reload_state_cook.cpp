////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_shotgun_reload_state_cook.h>

namespace survarium {

// STATE[STUB]
// void survarium::weapon_core_shotgun_reload_state_cook::~weapon_core_shotgun_reload_state_cook()
weapon_core_shotgun_reload_state_cook::~weapon_core_shotgun_reload_state_cook( )
{
	// FUNCTION BODY
	// <0x590150>|0x000|+0x010:'29'	{
	// <0>
	// <0x590160>|0x010|      :'31'	}
	// ******
}

// STATE[STUB]
// vostok::mutable_buffer survarium::weapon_core_shotgun_reload_state_cook::allocate_resource(vostok::resources::query_result_for_cook&, vostok::const_buffer, bool)
mutable_buffer weapon_core_shotgun_reload_state_cook::allocate_resource( resources::query_result_for_cook& in_query, const_buffer raw_file_data, bool file_exist )
{
	// FUNCTION BODY
	// <0x5901ab>|0x00b|+0x03d:'35'
	// <0x5901e8>|0x048|+0x02e:'36'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_shotgun_reload_state_cook::deallocate_resource(void*)
void weapon_core_shotgun_reload_state_cook::deallocate_resource( void* buffer )
{
	// FUNCTION BODY
	// <0x590178>|0x008|+0x014:'41'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_shotgun_reload_state_cook::create_resource(vostok::resources::query_result_for_cook&, vostok::const_buffer, vostok::mutable_buffer)
void weapon_core_shotgun_reload_state_cook::create_resource( resources::query_result_for_cook& parent, const_buffer raw_file_data, mutable_buffer in_out_unmanaged_resource_buffer )
{
	// LOCALS
	// configs::binary_config_value cfg
	// configs::binary_config_value start_user_anim_cfg
	// configs::binary_config_value finish_weapon_anim_cfg
	// configs::binary_config_value start_weapon_anim_cfg
	// configs::binary_config_value reload_one_weapon_anim_cfg
	// configs::binary_config_value reload_one_user_anim_cfg
	// configs::binary_config_value finish_user_anim_cfg
	// weapon_state_creation_params const* params
	// fixed_vector<resources::request,24> requests
	// u32 							i<1>
	// u32 							i<2>
	// u32 							i<3>
	// u32 							i<4>
	// u32 							i<5>
	// u32 							i<6>
	// ******

	// CONSTANTS
	// const weapon_core_shotgun_reload_state_cook::create_resource::__l65::<unnamed-tag> requests_count = 24;
	// const weapon_core_shotgun_reload_state_cook::create_resource::__l65::<unnamed-tag> total_weapon_anim_count = 12;
	// const weapon_core_shotgun_reload_state_cook::create_resource::__l65::<unnamed-tag> total_user_anim_count = 12;
	// ******

	// TYPEDEFS
	// typedef
	// 	fixed_vector<resources::request,24>
	// 	requests_fixed_type;

	// ******

	// FUNCTION BODY
	// <0x590701>|0x011|+0x009:'50'
	// <0x59070a>|0x01a|+0x00b:'51'
	// <0x590715>|0x025|+0x01c:'52'
	// <0x590731>|0x041|+0x001:'53'
	// <0x590732>|0x042|+0x00c:'54'
	// <0x59073e>|0x04e|+0x005:'55'
	// <0>
	// <1>
	// <0x590743>|0x053|+0x012:'58'
	// <0x590755>|0x065|+0x012:'59'
	// <0x590767>|0x077|+0x012:'60'
	// <0x590779>|0x089|+0x012:'61'
	// <0x59078b>|0x09b|+0x012:'62'
	// <0>
	// <0x59079d>|0x0ad|+0x012:'64'
	// <0x5907af>|0x0bf|+0x012:'65'
	// <0x5907c1>|0x0d1|+0x012:'66'
	// <0x5907d3>|0x0e3|+0x012:'67'
	// <0x5907e5>|0x0f5|+0x012:'68'
	// <0>
	// <0x5907f7>|0x107|+0x012:'70'
	// <0x590809>|0x119|+0x012:'71'
	// <0x59081b>|0x12b|+0x012:'72'
	// <0x59082d>|0x13d|+0x012:'73'
	// <0x59083f>|0x14f|+0x012:'74'
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
	// <0x590851>|0x161|+0x01f:'87'
	// <0>
	// <1>
	// <0x590870>|0x180|+0x051:'90'
	// <0x5908c1>|0x1d1|+0x024|[1]:'91'
	// <0x5908e5>|0x1f5|+0x03d:'92'
	// <0x590922>|0x232|+0x002:'93'
	// <0x590924>|0x234|+0x051:'94'
	// <0x590975>|0x285|+0x024|[2]:'95'
	// <0x590999>|0x2a9|+0x03d:'96'
	// <0x5909d6>|0x2e6|+0x002:'97'
	// <0>
	// <1>
	// <0x5909d8>|0x2e8|+0x051:'100'
	// <0x590a29>|0x339|+0x024|[3]:'101'
	// <0x590a4d>|0x35d|+0x03d:'102'
	// <0x590a8a>|0x39a|+0x002:'103'
	// <0x590a8c>|0x39c|+0x051:'104'
	// <0x590add>|0x3ed|+0x024|[4]:'105'
	// <0x590b01>|0x411|+0x03d:'106'
	// <0x590b3e>|0x44e|+0x002:'107'
	// <0>
	// <1>
	// <0x590b40>|0x450|+0x051:'110'
	// <0x590b91>|0x4a1|+0x024|[5]:'111'
	// <0x590bb5>|0x4c5|+0x03d:'112'
	// <0x590bf2>|0x502|+0x002:'113'
	// <0x590bf4>|0x504|+0x051:'114'
	// <0x590c45>|0x555|+0x024|[6]:'115'
	// <0x590c69>|0x579|+0x03d:'116'
	// <0x590ca6>|0x5b6|+0x002:'117'
	// <0x590ca8>|0x5b8|+0x012:'118'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x590cba>|0x5ca|+0x0e5:'127'
	// <0x590d9f>|0x6af|+0x00c:'128'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_shotgun_reload_state_cook::on_subresources_ready(vostok::resources::queries_result&, vostok::mutable_buffer, survarium::weapon_state_creation_params const*)
void weapon_core_shotgun_reload_state_cook::on_subresources_ready( resources::queries_result& data, mutable_buffer buffer, weapon_state_creation_params const* params )
{
	// LOCALS
	// fixed_vector<resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base>,8> start_animations
	// weapon_core_shotgun_reload_state* object_to_cook
	// float 						animations_timescale
	// u32 							resource_index
	// fixed_vector<resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base>,8> finish_animations
	// fixed_vector<resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base>,8> reload_one_animations
	// weapon_core_shotgun_reload_start_substate* reload_start
	// weapon_core_shotgun_reload_one_round_substate* reload_one_round
	// weapon_core_shotgun_reload_finish_substate* reload_finish
	// u32 							i<1>
	// u32 							i<2>
	// u32 							i<3>
	// ******

	// TYPEDEFS
	// typedef
	// 	fixed_vector<resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base>,8>
	// 	start_animations_fixed_type;

	// typedef
	// 	fixed_vector<resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base>,8>
	// 	finish_animations_fixed_type;

	// typedef
	// 	fixed_vector<resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base>,8>
	// 	reload_one_animations_fixed_type;

	// ******

	// FUNCTION BODY
	// <0x590231>|0x011|+0x012:'133'
	// <0>
	// <0x590243>|0x023|+0x007:'135'
	// <0>
	// <0x59024a>|0x02a|+0x00b:'137'
	// <0x590255>|0x035|+0x024|[1]:'138'
	// <0x590279>|0x059|+0x065:'139'
	// <0x5902de>|0x0be|+0x002:'140'
	// <0>
	// <1>
	// <0x5902e0>|0x0c0|+0x008:'143'
	// <0x5902e8>|0x0c8|+0x024|[2]:'144'
	// <0x59030c>|0x0ec|+0x062:'145'
	// <0x59036e>|0x14e|+0x002:'146'
	// <0>
	// <1>
	// <0x590370>|0x150|+0x008:'149'
	// <0x590378>|0x158|+0x024|[3]:'150'
	// <0x59039c>|0x17c|+0x062:'151'
	// <0x5903fe>|0x1de|+0x002:'152'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x590400>|0x1e0|+0x04a:'158'
	// <0>
	// <1>
	// <2>
	// <0x59044a>|0x22a|+0x094:'162'
	// <0>
	// <1>
	// <2>
	// <0x5904de>|0x2be|+0x08f:'166'
	// <0>
	// <1>
	// <2>
	// <0x59056d>|0x34d|+0x08e:'170'
	// <0>
	// <1>
	// <2>
	// <0x5905fb>|0x3db|+0x05e:'174'
	// <0x590659>|0x439|+0x059:'175'
	// <0x5906b2>|0x492|+0x013:'176'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_shotgun_reload_state_cook::destroy_resource(vostok::resources::unmanaged_resource*)
void weapon_core_shotgun_reload_state_cook::destroy_resource( resources::unmanaged_resource* resource )
{
	// LOCALS
	// weapon_core_shotgun_reload_state* wpn_state
	// ******

	// CALL SITE INFO
	// <0x590143> -> void* <unknown>(u32)
	// ******

	// FUNCTION BODY
	// <0x590119>|0x009|+0x01e:'181'
	// <0x590137>|0x027|+0x00e:'182'
	// ******
}

} // namespace survarium
