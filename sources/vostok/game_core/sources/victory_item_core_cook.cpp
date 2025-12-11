////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/victory_item_core_cook.h>

namespace survarium {

// STATE[STUB]
victory_item_core_cook::victory_item_core_cook( ) : resources::translate_query_cook( resources::victory_item_class, reuse_false, use_current_thread_id )
{
	resources::register_cook( this );

	// FUNCTION BODY
	// <0x7620a5>|0x035|+0x00c:'15'
	// ******
}

// STATE[STUB]
// survarium::victory_item_core* survarium::victory_item_core_cook::create_resource()
victory_item_core* victory_item_core_cook::create_resource( )
{
	return NULL;

	// FUNCTION BODY
	// <0x7620f9>|0x009|+0x04e:'20'
	// ******
}

// STATE[STUB]
// void survarium::victory_item_core_cook::translate_query(vostok::resources::query_result_for_cook&)
void victory_item_core_cook::translate_query( resources::query_result_for_cook& parent )
{
	// FUNCTION BODY
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x76222c>|0x00c|+0x0d0:'33'
	// <0>
	// ******
}

// STATE[STUB]
// void survarium::victory_item_core_cook::on_config_loaded(vostok::resources::queries_result&)
void victory_item_core_cook::on_config_loaded( resources::queries_result& data )
{
	// LOCALS
	// resources::resource_ptr<configs::binary_config,resources::unmanaged_intrusive_base> cfg
	// victory_item_core* 			object_to_cook
	// resources::query_result_for_cook* parent
	// ******

	// CALL SITE INFO
	// <0x762192> -> victory_item_core* <unknown>()
	// <0x7621b0> -> void <unknown>(configs::binary_config_value const&)
	// ******

	// FUNCTION BODY
	// <0x76215a>|0x00a|+0x00b:'39'
	// <0>
	// <0x762165>|0x015|+0x022:'41'
	// <0>
	// <0x762187>|0x037|+0x010:'43'
	// <0>
	// <0x762197>|0x047|+0x01b:'45'
	// <0>
	// <0x7621b2>|0x062|+0x045:'47'
	// <0x7621f7>|0x0a7|+0x00c:'48'
	// ******
}

// STATE[STUB]
// void survarium::victory_item_core_cook::delete_resource(vostok::resources::resource_base*)
void victory_item_core_cook::delete_resource( resources::resource_base* resource )
{
	// FUNCTION BODY
	// <0x7620c9>|0x009|+0x013:'53'
	// ******
}

} // namespace survarium
