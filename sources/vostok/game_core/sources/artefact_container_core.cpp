////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/artefact_container_core.h>

namespace survarium {

// STATE[STUB]
// survarium::artefact_container_core::artefact_container_core()
artefact_container_core::artefact_container_core( )
{
	// FUNCTION BODY
	// <0x73dfe0>|0x000|      :'17'	{
	// ******
}

// STATE[STUB]
// void survarium::artefact_container_core::load(vostok::configs::binary_config_value const&)
void artefact_container_core::load( configs::binary_config_value const& cfg )
{
	// FUNCTION BODY
	// <0x73dee0>|0x000|+0x009:'20'	{
	// <0x73dee9>|0x009|+0x00c:'21'
	// <0x73def5>|0x015|+0x02e:'22'
	// <0x73df23>|0x043|      :'23'	}
	// ******
}

// STATE[STUB]
// void survarium::artefact_container_core::activate(survarium::generic_anomaly_core*, vostok::physics::world*, survarium::scheduler&)
void artefact_container_core::activate( generic_anomaly_core* owner, physics::world* world, scheduler& __formal )
{
	// FUNCTION BODY
	// <0x73deb0>|0x000|+0x007:'26'	{
	// <0x73deb7>|0x007|+0x009:'27'
	// <0x73dec0>|0x010|+0x00c:'28'
	// <0x73decc>|0x01c|      :'29'	}
	// ******
}

// STATE[STUB]
// void survarium::artefact_container_core::deactivate()
void artefact_container_core::deactivate( )
{
	// FUNCTION BODY
	// <0x73de90>|0x000|+0x007:'32'	{
	// <0x73de97>|0x007|+0x008:'33'
	// <0x73de9f>|0x00f|+0x00a:'34'
	// <0x73dea9>|0x019|      :'35'	}
	// ******
}

// STATE[STUB]
// bool survarium::artefact_container_core::use_initialize(survarium::usable_object_user_data*)
bool artefact_container_core::use_initialize( usable_object_user_data* user )
{
	return false;
	// FUNCTION BODY
	// <0x73de30>|0x000|+0x009:'38'	{
	// <0x73de39>|0x009|+0x013:'39'
	// <0x73de4c>|0x01c|+0x004:'40'
	// <0>
	// <0x73de50>|0x020|+0x00c:'42'
	// <0x73de5c>|0x02c|+0x011:'43'
	// <0x73de6d>|0x03d|+0x009:'44'
	// <0x73de76>|0x046|+0x00c:'45'
	// <0x73de82>|0x052|+0x002:'46'
	// <0x73de84>|0x054|      :'47'	}
	// ******
}

// STATE[STUB]
// bool survarium::artefact_container_core::use_execute(survarium::usable_object_user_data*)
bool artefact_container_core::use_execute( usable_object_user_data* user )
{
	// LOCALS
	// u32 							left_ms
	// float 						artsearch_time
	// u32 							artefact_search_time_ms
	// ******

	// CALL SITE INFO
	// <0x73e1ba> -> inventory_holder* <unknown>()
	// ******

	return false;
	// FUNCTION BODY
	// <0x73e090>|0x000|+0x009:'50'	{
	// <0x73e099>|0x009|+0x00c:'51'
	// <0x73e0a5>|0x015|+0x00c:'52'
	// <0x73e0b1>|0x021|+0x00f:'53'
	// <0>
	// <0x73e0c0>|0x030|+0x01c:'55'
	// <0>
	// <0x73e0dc>|0x04c|+0x03e:'57'
	// <0>
	// <0x73e11a>|0x08a|+0x03a:'59'
	// <0>
	// <0x73e154>|0x0c4|+0x008:'61'
	// <0>
	// <0x73e15c>|0x0cc|+0x009:'63'
	// <0x73e165>|0x0d5|+0x00f:'64'
	// <0>
	// <0x73e174>|0x0e4|+0x00c:'66'
	// <0x73e180>|0x0f0|+0x029:'67'
	// <0x73e1a9>|0x119|+0x01c:'68'
	// <0>
	// <0x73e1c5>|0x135|+0x002:'70'
	// <0x73e1c7>|0x137|      :'71'	}
	// ******
}

// STATE[STUB]
// bool survarium::artefact_container_core::use_finalize(survarium::usable_object_user_data*)
bool artefact_container_core::use_finalize( usable_object_user_data* user )
{
	return false;
	// FUNCTION BODY
	// <0x73ddd0>|0x000|+0x009:'74'	{
	// <0x73ddd9>|0x009|+0x00c:'75'
	// <0x73dde5>|0x015|+0x00c:'76'
	// <0>
	// <0x73ddf1>|0x021|+0x00c:'78'
	// <0x73ddfd>|0x02d|+0x00a:'79'
	// <0x73de07>|0x037|+0x00a:'80'
	// <0x73de11>|0x041|+0x00f:'81'
	// <0x73de20>|0x050|+0x002:'82'
	// <0x73de22>|0x052|      :'83'	}
	// ******
}

// STATE[STUB]
// void survarium::artefact_container_core::artefact_spawned(vostok::resources::queries_result&)
void artefact_container_core::artefact_spawned( resources::queries_result& data )
{
	// FUNCTION BODY
	// <0x73df30>|0x000|+0x00a:'86'	{
	// <0x73df3a>|0x00a|+0x00c:'87'
	// <0x73df46>|0x016|+0x06c:'88'
	// <0x73dfb2>|0x082|+0x022:'89'
	// <0x73dfd4>|0x0a4|      :'90'	}
	// ******
}

// STATE[STUB]
// void survarium::artefact_container_core::spawn_artefact()
void artefact_container_core::spawn_artefact( )
{
	// LOCALS
	// variant<32> 					ud
	// ******

	// FUNCTION BODY
	// <0x73e1d0>|0x000|+0x013:'93'	{
	// <0x73e1e3>|0x013|+0x009:'94'
	// <0x73e1ec>|0x01c|+0x018:'95'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x73e204>|0x034|+0x0eb:'103'
	// <0x73e2ef>|0x11f|      :'104'	}
	// ******
}

// STATE[STUB]
// void survarium::artefact_container_core::transfer_artefact(survarium::inventory_holder*)
void artefact_container_core::transfer_artefact( inventory_holder* holder )
{
	// CALL SITE INFO
	// <0x73e064> -> void <unknown>(resources::resource_ptr<inventory_item,resources::unmanaged_intrusive_base> const&)
	// ******

	// FUNCTION BODY
	// <0x73e030>|0x000|+0x009:'107'	{
	// <0x73e039>|0x009|+0x035:'108'
	// <0x73e06e>|0x03e|+0x013:'109'
	// <0x73e081>|0x051|      :'110'	}
	// ******
}

} // namespace survarium
