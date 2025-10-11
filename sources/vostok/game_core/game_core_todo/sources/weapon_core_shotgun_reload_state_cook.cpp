////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "weapon_core_shotgun_reload_state_cook.h"

namespace survarium {

// STATE[STUB]
// void survarium::weapon_core_shotgun_reload_state_cook::~weapon_core_shotgun_reload_state_cook()
void weapon_core_shotgun_reload_state_cook::~weapon_core_shotgun_reload_state_cook( )
{
	// FUNCTION BODY
	// 1
	// ******
}

// STATE[STUB]
// vostok::mutable_buffer survarium::weapon_core_shotgun_reload_state_cook::allocate_resource(vostok::resources::query_result_for_cook&, vostok::const_buffer, bool)
vostok::mutable_buffer weapon_core_shotgun_reload_state_cook::allocate_resource( vostok::resources::query_result_for_cook& in_query, vostok::const_buffer raw_file_data, bool file_exist )
{
	// FUNCTION BODY
	// <0x5901ab>|0x000|0x000:'35'
	// <0x5901e8>|0x03d|0x03d:'36'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_shotgun_reload_state_cook::deallocate_resource(void*)
void weapon_core_shotgun_reload_state_cook::deallocate_resource( void* buffer )
{
	// FUNCTION BODY
	// <0x590178>|0x000|0x000:'41'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_shotgun_reload_state_cook::create_resource(vostok::resources::query_result_for_cook&, vostok::const_buffer, vostok::mutable_buffer)
void weapon_core_shotgun_reload_state_cook::create_resource( vostok::resources::query_result_for_cook& parent, vostok::const_buffer raw_file_data, vostok::mutable_buffer in_out_unmanaged_resource_buffer )
{
	// LOCALS
	// vostok::configs::binary_config_value cfg
	// vostok::configs::binary_config_value start_user_anim_cfg
	// vostok::configs::binary_config_value finish_weapon_anim_cfg
	// vostok::configs::binary_config_value start_weapon_anim_cfg
	// vostok::configs::binary_config_value reload_one_weapon_anim_cfg
	// vostok::configs::binary_config_value reload_one_user_anim_cfg
	// vostok::configs::binary_config_value finish_user_anim_cfg
	// weapon_state_creation_params const* params
	// vostok::fixed_vector<vostok::resources::request,24> requests
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
	// 	vostok::fixed_vector<vostok::resources::request,24>
	// 	requests_fixed_type;

	// ******

	// FUNCTION BODY
	// <0x590701>|0x000|0x000:'50'
	// <0x59070a>|0x009|0x009:'51'
	// <0x590715>|0x014|0x00b:'52'
	// <0x590731>|0x030|0x01c:'53'
	// <0x590732>|0x031|0x001:'54'
	// <0x59073e>|0x03d|0x00c:'55'
	// 1
	// 2
	// <0x590743>|0x042|0x005:'58'
	// <0x590755>|0x054|0x012:'59'
	// <0x590767>|0x066|0x012:'60'
	// <0x590779>|0x078|0x012:'61'
	// <0x59078b>|0x08a|0x012:'62'
	// 1
	// <0x59079d>|0x09c|0x012:'64'
	// <0x5907af>|0x0ae|0x012:'65'
	// <0x5907c1>|0x0c0|0x012:'66'
	// <0x5907d3>|0x0d2|0x012:'67'
	// <0x5907e5>|0x0e4|0x012:'68'
	// 1
	// <0x5907f7>|0x0f6|0x012:'70'
	// <0x590809>|0x108|0x012:'71'
	// <0x59081b>|0x11a|0x012:'72'
	// <0x59082d>|0x12c|0x012:'73'
	// <0x59083f>|0x13e|0x012:'74'
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
	// <0x590851>|0x150|0x012:'87'
	// 1
	// 2
	// <0x590870>|0x16f|0x01f:'90'
	// <0x5908c1>|0x1c0|0x051|[1]:'91'
	// <0x5908e5>|0x1e4|0x024:'92'
	// <0x590922>|0x221|0x03d:'93'
	// <0x590924>|0x223|0x002:'94'
	// <0x590975>|0x274|0x051|[2]:'95'
	// <0x590999>|0x298|0x024:'96'
	// <0x5909d6>|0x2d5|0x03d:'97'
	// 1
	// 2
	// <0x5909d8>|0x2d7|0x002:'100'
	// <0x590a29>|0x328|0x051|[3]:'101'
	// <0x590a4d>|0x34c|0x024:'102'
	// <0x590a8a>|0x389|0x03d:'103'
	// <0x590a8c>|0x38b|0x002:'104'
	// <0x590add>|0x3dc|0x051|[4]:'105'
	// <0x590b01>|0x400|0x024:'106'
	// <0x590b3e>|0x43d|0x03d:'107'
	// 1
	// 2
	// <0x590b40>|0x43f|0x002:'110'
	// <0x590b91>|0x490|0x051|[5]:'111'
	// <0x590bb5>|0x4b4|0x024:'112'
	// <0x590bf2>|0x4f1|0x03d:'113'
	// <0x590bf4>|0x4f3|0x002:'114'
	// <0x590c45>|0x544|0x051|[6]:'115'
	// <0x590c69>|0x568|0x024:'116'
	// <0x590ca6>|0x5a5|0x03d:'117'
	// <0x590ca8>|0x5a7|0x002:'118'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// 8
	// <0x590cba>|0x5b9|0x012:'127'
	// <0x590d9f>|0x69e|0x0e5:'128'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_shotgun_reload_state_cook::on_subresources_ready(vostok::resources::queries_result&, vostok::mutable_buffer, survarium::weapon_state_creation_params const*)
void weapon_core_shotgun_reload_state_cook::on_subresources_ready( vostok::resources::queries_result& data, vostok::mutable_buffer buffer, weapon_state_creation_params const* params )
{
	// LOCALS
	// vostok::fixed_vector<vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base>,8> start_animations
	// weapon_core_shotgun_reload_state* object_to_cook
	// float 						animations_timescale
	// u32 							resource_index
	// vostok::fixed_vector<vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base>,8> finish_animations
	// vostok::fixed_vector<vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base>,8> reload_one_animations
	// weapon_core_shotgun_reload_start_substate* reload_start
	// weapon_core_shotgun_reload_one_round_substate* reload_one_round
	// weapon_core_shotgun_reload_finish_substate* reload_finish
	// u32 							i<1>
	// u32 							i<2>
	// u32 							i<3>
	// ******

	// TYPEDEFS
	// typedef
	// 	vostok::fixed_vector<vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base>,8>
	// 	start_animations_fixed_type;

	// typedef
	// 	vostok::fixed_vector<vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base>,8>
	// 	finish_animations_fixed_type;

	// typedef
	// 	vostok::fixed_vector<vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base>,8>
	// 	reload_one_animations_fixed_type;

	// ******

	// FUNCTION BODY
	// <0x590231>|0x000|0x000:'133'
	// 1
	// <0x590243>|0x012|0x012:'135'
	// 1
	// <0x59024a>|0x019|0x007:'137'
	// <0x590255>|0x024|0x00b|[1]:'138'
	// <0x590279>|0x048|0x024:'139'
	// <0x5902de>|0x0ad|0x065:'140'
	// 1
	// 2
	// <0x5902e0>|0x0af|0x002:'143'
	// <0x5902e8>|0x0b7|0x008|[2]:'144'
	// <0x59030c>|0x0db|0x024:'145'
	// <0x59036e>|0x13d|0x062:'146'
	// 1
	// 2
	// <0x590370>|0x13f|0x002:'149'
	// <0x590378>|0x147|0x008|[3]:'150'
	// <0x59039c>|0x16b|0x024:'151'
	// <0x5903fe>|0x1cd|0x062:'152'
	// 1
	// 2
	// 3
	// 4
	// 5
	// <0x590400>|0x1cf|0x002:'158'
	// 1
	// 2
	// 3
	// <0x59044a>|0x219|0x04a:'162'
	// 1
	// 2
	// 3
	// <0x5904de>|0x2ad|0x094:'166'
	// 1
	// 2
	// 3
	// <0x59056d>|0x33c|0x08f:'170'
	// 1
	// 2
	// 3
	// <0x5905fb>|0x3ca|0x08e:'174'
	// <0x590659>|0x428|0x05e:'175'
	// <0x5906b2>|0x481|0x059:'176'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_shotgun_reload_state_cook::destroy_resource(vostok::resources::unmanaged_resource*)
void weapon_core_shotgun_reload_state_cook::destroy_resource( vostok::resources::unmanaged_resource* resource )
{
	// LOCALS
	// weapon_core_shotgun_reload_state* wpn_state
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x57f143 }, type_index: TypeIndex(0x2524f) })
	// ******

	// FUNCTION BODY
	// <0x590119>|0x000|0x000:'181'
	// <0x590137>|0x01e|0x01e:'182'
	// ******
}

} // namespace survarium
