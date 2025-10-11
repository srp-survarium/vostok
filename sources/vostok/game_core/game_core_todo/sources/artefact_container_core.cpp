////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "artefact_container_core.h"

namespace survarium {

// STATE[STUB]
// survarium::artefact_container_core::artefact_container_core()
artefact_container_core::artefact_container_core( )
{
}

// STATE[STUB]
// void survarium::artefact_container_core::load(vostok::configs::binary_config_value const&)
void artefact_container_core::load( vostok::configs::binary_config_value const& cfg )
{
	// FUNCTION BODY
	// <0x73dee9>|0x000|0x000:'21'
	// <0x73def5>|0x00c|0x00c:'22'
	// ******
}

// STATE[STUB]
// void survarium::artefact_container_core::activate(survarium::generic_anomaly_core*, vostok::physics::world*, survarium::scheduler&)
void artefact_container_core::activate( generic_anomaly_core* owner, vostok::physics::world* world, scheduler& __formal )
{
	// FUNCTION BODY
	// <0x73deb7>|0x000|0x000:'27'
	// <0x73dec0>|0x009|0x009:'28'
	// ******
}

// STATE[STUB]
// void survarium::artefact_container_core::deactivate()
void artefact_container_core::deactivate( )
{
	// FUNCTION BODY
	// <0x73de97>|0x000|0x000:'33'
	// <0x73de9f>|0x008|0x008:'34'
	// ******
}

// STATE[STUB]
// bool survarium::artefact_container_core::use_initialize(survarium::usable_object_user_data*)
bool artefact_container_core::use_initialize( usable_object_user_data* user )
{
	return false;
	// FUNCTION BODY
	// <0x73de39>|0x000|0x000:'39'
	// <0x73de4c>|0x013|0x013:'40'
	// 1
	// <0x73de50>|0x017|0x004:'42'
	// <0x73de5c>|0x023|0x00c:'43'
	// <0x73de6d>|0x034|0x011:'44'
	// <0x73de76>|0x03d|0x009:'45'
	// <0x73de82>|0x049|0x00c:'46'
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

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x72d1ba }, type_index: TypeIndex(0xa8f1) })
	// ******

	return false;
	// FUNCTION BODY
	// <0x73e099>|0x000|0x000:'51'
	// <0x73e0a5>|0x00c|0x00c:'52'
	// <0x73e0b1>|0x018|0x00c:'53'
	// 1
	// <0x73e0c0>|0x027|0x00f:'55'
	// 1
	// <0x73e0dc>|0x043|0x01c:'57'
	// 1
	// <0x73e11a>|0x081|0x03e:'59'
	// 1
	// <0x73e154>|0x0bb|0x03a:'61'
	// 1
	// <0x73e15c>|0x0c3|0x008:'63'
	// <0x73e165>|0x0cc|0x009:'64'
	// 1
	// <0x73e174>|0x0db|0x00f:'66'
	// <0x73e180>|0x0e7|0x00c:'67'
	// <0x73e1a9>|0x110|0x029:'68'
	// 1
	// <0x73e1c5>|0x12c|0x01c:'70'
	// ******
}

// STATE[STUB]
// bool survarium::artefact_container_core::use_finalize(survarium::usable_object_user_data*)
bool artefact_container_core::use_finalize( usable_object_user_data* user )
{
	return false;
	// FUNCTION BODY
	// <0x73ddd9>|0x000|0x000:'75'
	// <0x73dde5>|0x00c|0x00c:'76'
	// 1
	// <0x73ddf1>|0x018|0x00c:'78'
	// <0x73ddfd>|0x024|0x00c:'79'
	// <0x73de07>|0x02e|0x00a:'80'
	// <0x73de11>|0x038|0x00a:'81'
	// <0x73de20>|0x047|0x00f:'82'
	// ******
}

// STATE[STUB]
// void survarium::artefact_container_core::artefact_spawned(vostok::resources::queries_result&)
void artefact_container_core::artefact_spawned( vostok::resources::queries_result& data )
{
	// FUNCTION BODY
	// <0x73df3a>|0x000|0x000:'87'
	// <0x73df46>|0x00c|0x00c:'88'
	// <0x73dfb2>|0x078|0x06c:'89'
	// ******
}

// STATE[STUB]
// void survarium::artefact_container_core::spawn_artefact()
void artefact_container_core::spawn_artefact( )
{
	// LOCALS
	// vostok::variant<32> 			ud
	// ******

	// FUNCTION BODY
	// <0x73e1e3>|0x000|0x000:'94'
	// <0x73e1ec>|0x009|0x009:'95'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// <0x73e204>|0x021|0x018:'103'
	// ******
}

// STATE[STUB]
// void survarium::artefact_container_core::transfer_artefact(survarium::inventory_holder*)
void artefact_container_core::transfer_artefact( inventory_holder* holder )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x72d064 }, type_index: TypeIndex(0xa826) })
	// ******

	// FUNCTION BODY
	// <0x73e039>|0x000|0x000:'108'
	// <0x73e06e>|0x035|0x035:'109'
	// ******
}

	/* TYPEDEFS

	typedef
		scheduler::record*
		iterator_type;

	typedef
		void**
		iterator_type;

	typedef
		zone_group::zone_wrapper*
		iterator_type;

} // namespace survarium
