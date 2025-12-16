////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/victory_item_core.h>

namespace survarium {

// STATE[STUB]
// survarium::victory_item_core::victory_item_core()
victory_item_core::victory_item_core( )
{
	// FUNCTION BODY
	// <0x59c450>|0x000|+0x074:'20'	{
	// <0x59c4c4>|0x074|+0x00e:'21'
	// <0x59c4d2>|0x082|      :'22'	}
	// ******
}

// STATE[STUB]
// void survarium::victory_item_core::~victory_item_core()
victory_item_core::~victory_item_core( )
{
	// FUNCTION BODY
	// <0x59c3f0>|0x000|+0x027:'25'	{
	// <0x59c417>|0x027|+0x019:'26'
	// <0x59c430>|0x040|      :'27'	}
	// ******
}

// STATE[STUB]
// void survarium::victory_item_core::unload()
void victory_item_core::unload( )
{
	// FUNCTION BODY
	// <0x59c390>|0x000|+0x007:'30'	{
	// <0x59c397>|0x007|+0x008:'31'
	// <0x59c39f>|0x00f|      :'32'	}
	// ******
}

// STATE[STUB]
// void survarium::victory_item_core::load(vostok::configs::binary_config_value const&)
void victory_item_core::load( configs::binary_config_value const& cfg )
{
	// FUNCTION BODY
	// <0x59c4e0>|0x000|+0x009:'35'	{
	// <0x59c4e9>|0x009|+0x00c:'36'
	// <0>
	// <0x59c4f5>|0x015|+0x047:'38'
	// <0x59c53c>|0x05c|+0x024:'39'
	// <0x59c560>|0x080|      :'40'	}
	// ******
}

// STATE[STUB]
// bool survarium::victory_item_core::use_initialize(survarium::usable_object_user_data*)
bool victory_item_core::use_initialize( usable_object_user_data* user )
{
	// CALL SITE INFO
	// <0x59c361> -> void <unknown>(victory_item_core*)
	// ******

	return false;
	// FUNCTION BODY
	// <0x59c330>|0x000|+0x007:'43'	{
	// <0x59c337>|0x007|+0x013:'44'
	// <0x59c34a>|0x01a|+0x004:'45'
	// <0>
	// <0x59c34e>|0x01e|+0x015:'47'
	// <0>
	// <0x59c363>|0x033|+0x002:'49'
	// <0x59c365>|0x035|      :'50'	}
	// ******
}

// STATE[STUB]
// bool survarium::victory_item_core::use_execute(survarium::usable_object_user_data*)
bool victory_item_core::use_execute( usable_object_user_data* user )
{
	return false;
	// FUNCTION BODY
	// <0x59c310>|0x000|+0x009:'53'	{
	// <0x59c319>|0x009|+0x00c:'54'
	// <0x59c325>|0x015|+0x002:'55'
	// <0x59c327>|0x017|      :'56'	}
	// ******
}

// STATE[STUB]
// bool survarium::victory_item_core::use_finalize(survarium::usable_object_user_data*)
bool victory_item_core::use_finalize( usable_object_user_data* user )
{
	return false;
	// FUNCTION BODY
	// <0x59c2f0>|0x000|+0x009:'59'	{
	// <0x59c2f9>|0x009|+0x00c:'60'
	// <0x59c305>|0x015|+0x002:'61'
	// <0x59c307>|0x017|      :'62'	}
	// ******
}

// STATE[STUB]
// void survarium::victory_item_core::put(vostok::physics::world*, vostok::math::float4x4 const&)
void victory_item_core::put( physics::world* world, float4x4 const& transform )
{
	// CALL SITE INFO
	// <0x59c3d2> -> void <unknown>(float4x4 const&)
	// ******

	// FUNCTION BODY
	// <0x59c3b0>|0x000|+0x007:'65'	{
	// <0x59c3b7>|0x007|+0x00c:'66'
	// <0x59c3c3>|0x013|+0x011:'67'
	// <0x59c3d4>|0x024|+0x00a:'68'
	// <0x59c3de>|0x02e|      :'69'	}
	// ******
}

// STATE[STUB]
// void survarium::victory_item_core::take()
void victory_item_core::take( )
{
	// FUNCTION BODY
	// <0x59c370>|0x000|+0x007:'72'	{
	// <0x59c377>|0x007|+0x008:'73'
	// <0x59c37f>|0x00f|+0x00a:'74'
	// <0x59c389>|0x019|      :'75'	}
	// ******
}

// STATE[STUB]
// void survarium::victory_item_core::set_transform(vostok::math::float4x4 const&)
void victory_item_core::set_transform( float4x4 const& transform )
{
	// CALL SITE INFO
	// <0x59c2e5> -> void <unknown>(float4x4 const&)
	// ******

	// FUNCTION BODY
	// <0x59c2b0>|0x000|+0x009:'78'	{
	// <0x59c2b9>|0x009|+0x013:'79'
	// <0x59c2cc>|0x01c|+0x01b:'80'
	// <0x59c2e7>|0x037|      :'81'	}
	// ******
}

// STATE[STUB]
// vostok::math::float4x4 survarium::victory_item_core::get_transform()
float4x4 victory_item_core::get_transform( )
{
	return vostok::math::float4x4();
	// FUNCTION BODY
	// <0x59c280>|0x000|+0x009:'83'	{
	// <0x59c289>|0x009|+0x016:'84'
	// <0x59c29f>|0x01f|      :'85'	}
	// ******
}

} // namespace survarium
