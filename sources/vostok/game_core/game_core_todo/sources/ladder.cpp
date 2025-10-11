////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "ladder.h"

namespace survarium {

// STATE[STUB]
// survarium::ladder::ladder(vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const&, vostok::math::plane const&)
ladder::ladder( vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const& main_animation, vostok::math::plane const& p )
{
}

// STATE[STUB]
// void survarium::ladder::~ladder()
void ladder::~ladder( )
{
	// FUNCTION BODY
	// <0x59666c>|0x000|0x000:'23'
	// ******
}

// STATE[STUB]
// void survarium::ladder::load(vostok::configs::binary_config_value const&)
void ladder::load( vostok::configs::binary_config_value const& cfg_val )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x585636 }, type_index: TypeIndex(0xa90c) })
	// ******

	// FUNCTION BODY
	// <0x596579>|0x000|0x000:'28'
	// <0x596585>|0x00c|0x00c:'29'
	// <0x59659b>|0x022|0x016:'30'
	// 1
	// <0x5965b3>|0x03a|0x018:'32'
	// <0x596617>|0x09e|0x064:'33'
	// 1
	// ******
}

// STATE[STUB]
// void survarium::ladder::resolve_links(survarium::base_project*, vostok::configs::binary_config_value)
void ladder::resolve_links( base_project* p, vostok::configs::binary_config_value cfg )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x585566 }, type_index: TypeIndex(0x3d85e) })
	// ******

	// FUNCTION BODY
	// <0x5964e7>|0x000|0x000:'39'
	// <0x59651b>|0x034|0x034:'40'
	// <0x596524>|0x03d|0x009:'41'
	// ******
}

// STATE[STUB]
// void survarium::ladder::add_landing_point(survarium::landing_point* const)
void ladder::add_landing_point( landing_point* new_point )
{
	// FUNCTION BODY
	// <0x596479>|0x000|0x000:'102'
	// ******
}

// STATE[STUB]
// survarium::landing_point* survarium::ladder::pop_landing_point()
landing_point* ladder::pop_landing_point( )
{
	return NULL;
	// FUNCTION BODY
	// <0x596459>|0x000|0x000:'107'
	// ******
}

// STATE[STUB]
// bool survarium::ladder::use_initialize(survarium::usable_object_user_data*)
bool ladder::use_initialize( usable_object_user_data* user )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x58543f }, type_index: TypeIndex(0xa8eb) })
	// ******

	return false;
	// FUNCTION BODY
	// <0x596427>|0x000|0x000:'112'
	// <0x596441>|0x01a|0x01a:'113'
	// ******
}

// STATE[STUB]
// bool survarium::ladder::use_execute(survarium::usable_object_user_data*)
bool ladder::use_execute( usable_object_user_data* user )
{
	return false;
	// FUNCTION BODY
	// 1
	// <0x596417>|0x000|0x000:'119'
	// ******
}

// STATE[STUB]
// bool survarium::ladder::use_finalize(survarium::usable_object_user_data*)
bool ladder::use_finalize( usable_object_user_data* user )
{
	return false;
	// FUNCTION BODY
	// 1
	// <0x596407>|0x000|0x000:'125'
	// ******
}

// STATE[STUB]
// void survarium::ladder::activate(vostok::physics::world*)
void ladder::activate( vostok::physics::world* world )
{
	// FUNCTION BODY
	// <0x5964c7>|0x000|0x000:'130'
	// ******
}

// STATE[STUB]
// void survarium::ladder::deactivate()
void ladder::deactivate( )
{
	// FUNCTION BODY
	// <0x5964a7>|0x000|0x000:'135'
	// ******
}

} // namespace survarium
