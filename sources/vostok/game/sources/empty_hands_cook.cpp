////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "empty_hands_cook.h"

namespace survarium {

// STATE[STUB]
 empty_hands_cook::empty_hands_cook( ) :
	// base args are buildability placeholders (the game_core cook convention);
	// a matcher confirms when this TU is enabled
	resources::translate_query_cook( resources::empty_hands_class, reuse_false, use_current_thread_id )
{
	// FUNCTION BODY[0x766830]: 1
	// <0x766868>|0x038|+0x029:'17'
	// ******
}

// STATE[STUB]
void empty_hands_cook::translate_query( resources::query_result_for_cook& parent )
{
	// LOCALS
	// fs_new::virtual_path_string 		empty_hands_config_name
	// ******

	// FUNCTION BODY[0x766c70]: 11
	// <0>
	// <0x766c7d>|0x00d|+0x04c:'23'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x766cc9>|0x059|+0x090:'32'
	// ******
}

// STATE[STUB]
void empty_hands_cook::on_empty_hands_config_loaded( resources::queries_result& data )
{
	// LOCALS
	// configs::binary_config_value 	animations_node
	// configs::binary_config_ptr 		config
	// ******

	// FUNCTION BODY[0x766ac0]: 22
	// <0>
	// <0x766ac6>|0x006|+0x046:'38'
	// <0>
	// <1>
	// <2>
	// <0x766b0c>|0x04c|+0x030:'42'
	// <0x766b3c>|0x07c|+0x020:'43'
	// <0>
	// <1>
	// <2>
	// <0x766b5c>|0x09c|+0x010:'47'
	// <0x766b6c>|0x0ac|+0x007:'48'
	// <0x766b73>|0x0b3|+0x01b:'49'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x766b8e>|0x0ce|+0x0af:'58'
	// ******
}

// STATE[STUB]
void empty_hands_cook::on_empty_hands_animations_loaded( resources::queries_result& data )
{
	// LOCALS
	// const u32 						buffer_size
	// resources::query_result_for_cook* const parent
	// const u32 						animations_count
	// ******

	// FUNCTION BODY[0x766900]: 17
	// <0>
	// <0x766909>|0x009|+0x003:'64'
	// <0>
	// <1>
	// <2>
	// <0x76690c>|0x00c|+0x013:'68'
	// <0>
	// <1>
	// <0x76691f>|0x01f|+0x024:'71'
	// <0>
	// <0x766943>|0x043|+0x082:'73'
	// <0>
	// <0x7669c5>|0x0c5|+0x079:'75'
	// <0>
	// <0x766a3e>|0x13e|+0x006:'77'
	// <0x766a44>|0x144|+0x05f:'78'
	// <0x766aa3>|0x1a3|+0x00e:'79'
	// ******
}

// STATE[STUB]
void empty_hands_cook::delete_resource( resources::resource_base* resource )
{
	// CALL SITE INFO
	// <0x7668bb> -> void* < unknown >( u32 )
	// ******

	// FUNCTION BODY[0x7668a0]: 10
	// <0>
	// <0x7668a3>|0x003|+0x004:'85'
	// <0>
	// <1>
	// <0x7668a7>|0x007|+0x016:'88'
	// <0x7668bd>|0x01d|+0x004:'89'
	// <0x7668c1>|0x021|+0x00f:'90'
	// <0>
	// <0x7668d0>|0x030|+0x009:'92'
	// <0x7668d9>|0x039|+0x019:'93'
	// ******
}

} // namespace survarium
