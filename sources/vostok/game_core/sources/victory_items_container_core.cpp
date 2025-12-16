////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/victory_items_container_core.h>

namespace survarium {

// STATE[STUB]
victory_items_container_core::victory_items_container_core( ) : m_victory_items( g_allocator )
{
	// FUNCTION BODY
	// <0x59dee0>|0x000|+0x06f:'21'	{
	// <0>
	// <0x59df4f>|0x06f|      :'23'	}
	// ******
}

// STATE[STUB]
// void survarium::victory_items_container_core::load(vostok::configs::binary_config_value const&)
void victory_items_container_core::load( configs::binary_config_value const& cfg )
{
	// FUNCTION BODY
	// <0x59df69>|0x009|+0x00c:'27'
	// <0x59df75>|0x015|+0x01b:'28'
	// <0x59df90>|0x030|+0x018:'29'
	// ******
}

// STATE[STUB]
// bool survarium::victory_items_container_core::use_initialize(survarium::usable_object_user_data*)
bool victory_items_container_core::use_initialize( usable_object_user_data* user )
{
	// CALL SITE INFO
	// <0x59de11> -> void <unknown>(victory_items_container_core*)
	// ******

	return false;

	// FUNCTION BODY
	// <0x59dde7>|0x007|+0x013:'34'
	// <0x59ddfa>|0x01a|+0x004:'35'
	// <0>
	// <0x59ddfe>|0x01e|+0x015:'37'
	// <0>
	// <0x59de13>|0x033|+0x002:'39'
	// ******
}

// STATE[STUB]
// char const* survarium::victory_items_container_core::use_info(survarium::usable_object_user_data*)
pcstr victory_items_container_core::use_info( usable_object_user_data* __formal )
{
	return NULL;

	// FUNCTION BODY
	// <0x59ddc7>|0x007|+0x005:'44'
	// ******
}

// STATE[STUB]
// bool survarium::victory_items_container_core::use_execute(survarium::usable_object_user_data*)
bool victory_items_container_core::use_execute( usable_object_user_data* user )
{
	return false;

	// FUNCTION BODY
	// <0x59de89>|0x009|+0x00c:'49'
	// <0x59de95>|0x015|+0x00c:'50'
	// <0>
	// <1>
	// <0x59dea1>|0x021|+0x002:'53'
	// ******
}

// STATE[STUB]
// bool survarium::victory_items_container_core::use_finalize(survarium::usable_object_user_data*)
bool victory_items_container_core::use_finalize( usable_object_user_data* __formal )
{
	return false;

	// FUNCTION BODY
	// <0x59ddb7>|0x007|+0x002:'58'
	// ******
}

// STATE[STUB]
// void survarium::victory_items_container_core::put_item(survarium::victory_item_core*)
void victory_items_container_core::put_item( victory_item_core* item )
{
	// FUNCTION BODY
	// <0x59deb9>|0x009|+0x01b:'63'
	// ******
}

// STATE[STUB]
// survarium::victory_item_core* survarium::victory_items_container_core::take_item()
victory_item_core* victory_items_container_core::take_item( )
{
	// LOCALS
	// victory_item_core* 			last_item
	// ******

	return NULL;

	// FUNCTION BODY
	// <0x59de29>|0x009|+0x022:'67'
	// <0x59de4b>|0x02b|+0x027:'68'
	// <0x59de72>|0x052|+0x003:'69'
	// ******
}

} // namespace survarium
