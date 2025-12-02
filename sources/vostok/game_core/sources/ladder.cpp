////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/ladder.h>

namespace survarium {

// STATE[STUB]
// survarium::ladder::ladder(vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const&, vostok::math::plane const&)
ladder::ladder( resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base> const& main_animation, math::plane const& p )
{
	// FUNCTION BODY
	// <0x5966c0>|0x000|+0x0b7:'18'	{
	// <0x596777>|0x0b7|      :'19'	}
	// ******
}

// STATE[STUB]
// void survarium::ladder::~ladder()
ladder::~ladder( )
{
	// FUNCTION BODY
	// <0x596640>|0x000|+0x02c:'22'	{
	// <0x59666c>|0x02c|+0x01c:'23'
	// <0x596688>|0x048|      :'24'	}
	// ******
}

// STATE[STUB]
// void survarium::ladder::load(vostok::configs::binary_config_value const&)
void ladder::load( configs::binary_config_value const& cfg_val )
{
	// CALL SITE INFO
	// <0x596636> -> void <unknown>(configs::binary_config_value const&)
	// ******

	// FUNCTION BODY
	// <0x596570>|0x000|+0x009:'27'	{
	// <0x596579>|0x009|+0x00c:'28'
	// <0x596585>|0x015|+0x016:'29'
	// <0x59659b>|0x02b|+0x018:'30'
	// <0>
	// <0x5965b3>|0x043|+0x064:'32'
	// <0x596617>|0x0a7|+0x021:'33'
	// <0>
	// <0x596638>|0x0c8|      :'35'	}
	// ******
}

// STATE[STUB]
// void survarium::ladder::resolve_links(survarium::base_project*, vostok::configs::binary_config_value)
void ladder::resolve_links( base_project* p, configs::binary_config_value cfg )
{
	// CALL SITE INFO
	// <0x596566> -> void <unknown>(base_project*, configs::binary_config_value)
	// ******

	// FUNCTION BODY
	// <0x5964e0>|0x000|+0x007:'38'	{
	// <0x5964e7>|0x007|+0x034:'39'
	// <0x59651b>|0x03b|+0x009:'40'
	// <0x596524>|0x044|+0x044:'41'
	// <0x596568>|0x088|      :'42'	}
	// ******
}

// STATE[STUB]
// void survarium::ladder::add_landing_point(survarium::landing_point* const)
void ladder::add_landing_point( landing_point* new_point )
{
	// FUNCTION BODY
	// <0x596470>|0x000|+0x009:'101'	{
	// <0x596479>|0x009|+0x014:'102'
	// <0x59648d>|0x01d|      :'103'	}
	// ******
}

// STATE[STUB]
// survarium::landing_point* survarium::ladder::pop_landing_point()
landing_point* ladder::pop_landing_point( )
{
	return NULL;
	// FUNCTION BODY
	// <0x596450>|0x000|+0x009:'106'	{
	// <0x596459>|0x009|+0x00e:'107'
	// <0x596467>|0x017|      :'108'	}
	// ******
}

// STATE[STUB]
// bool survarium::ladder::use_initialize(survarium::usable_object_user_data*)
bool ladder::use_initialize( usable_object_user_data* user )
{
	// CALL SITE INFO
	// <0x59643f> -> void <unknown>(ladder*)
	// ******

	return false;
	// FUNCTION BODY
	// <0x596420>|0x000|+0x007:'111'	{
	// <0x596427>|0x007|+0x01a:'112'
	// <0x596441>|0x021|+0x002:'113'
	// <0x596443>|0x023|      :'114'	}
	// ******
}

// STATE[STUB]
// bool survarium::ladder::use_execute(survarium::usable_object_user_data*)
bool ladder::use_execute( usable_object_user_data* user )
{
	return false;
	// FUNCTION BODY
	// <0x596410>|0x000|+0x007:'117'	{
	// <0>
	// <0x596417>|0x007|+0x002:'119'
	// <0x596419>|0x009|      :'120'	}
	// ******
}

// STATE[STUB]
// bool survarium::ladder::use_finalize(survarium::usable_object_user_data*)
bool ladder::use_finalize( usable_object_user_data* user )
{
	return false;
	// FUNCTION BODY
	// <0x596400>|0x000|+0x007:'123'	{
	// <0>
	// <0x596407>|0x007|+0x002:'125'
	// <0x596409>|0x009|      :'126'	}
	// ******
}

// STATE[STUB]
// void survarium::ladder::activate(vostok::physics::world*)
void ladder::activate( physics::world* world )
{
	// FUNCTION BODY
	// <0x5964c0>|0x000|+0x007:'129'	{
	// <0x5964c7>|0x007|+0x012:'130'
	// <0x5964d9>|0x019|      :'131'	}
	// ******
}

// STATE[STUB]
// void survarium::ladder::deactivate()
void ladder::deactivate( )
{
	// FUNCTION BODY
	// <0x5964a0>|0x000|+0x007:'134'	{
	// <0x5964a7>|0x007|+0x00e:'135'
	// <0x5964b5>|0x015|      :'136'	}
	// ******
}

} // namespace survarium
