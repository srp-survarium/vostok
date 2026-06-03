////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_inactive_state_cook.h>

namespace survarium {

// STATE[STUB]
// survarium::weapon_core_inactive_state_cook::weapon_core_inactive_state_cook()
weapon_core_inactive_state_cook::weapon_core_inactive_state_cook( ) :
	resources::unmanaged_cook( resources::weapon_inactive_state_class, reuse_false, use_current_thread_id )
{
	resources::register_cook( this );

	// FUNCTION BODY[0x59eee0]: 1
	// <0x59ef17>|0x037|+0x00c:'18'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_inactive_state_cook::~weapon_core_inactive_state_cook()
weapon_core_inactive_state_cook::~weapon_core_inactive_state_cook( )
{
	// FUNCTION BODY[0x59eec0]: 1
	// ******
}

// STATE[STUB]
// vostok::mutable_buffer survarium::weapon_core_inactive_state_cook::allocate_resource(vostok::resources::query_result_for_cook&, vostok::const_buffer, bool)
mutable_buffer weapon_core_inactive_state_cook::allocate_resource( resources::query_result_for_cook& in_query, const_buffer raw_file_data, bool file_exist )
{
	// FUNCTION BODY[0x59ef60]: 2
	// <0x59ef6b>|0x00b|+0x03d:'28'
	// <0x59efa8>|0x048|+0x02e:'29'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_inactive_state_cook::deallocate_resource(void*)
void weapon_core_inactive_state_cook::deallocate_resource( void* buffer )
{
	// FUNCTION BODY[0x59ef30]: 1
	// <0x59ef38>|0x008|+0x014:'34'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_inactive_state_cook::create_resource(vostok::resources::query_result_for_cook&, vostok::const_buffer, vostok::mutable_buffer)
void weapon_core_inactive_state_cook::create_resource( resources::query_result_for_cook& parent, const_buffer raw_file_data, mutable_buffer in_out_unmanaged_resource_buffer )
{
	// LOCALS
	// weapon_core_inactive_state* 	object_to_cook
	// weapon_state_creation_params const* params
	// ******

	// FUNCTION BODY[0x59efe0]: 4
	// <0x59efe9>|0x009|+0x006:'39'
	// <0x59efef>|0x00f|+0x068:'40'
	// <0x59f057>|0x077|+0x045:'41'
	// <0x59f09c>|0x0bc|+0x00c:'42'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_inactive_state_cook::destroy_resource(vostok::resources::unmanaged_resource*)
void weapon_core_inactive_state_cook::destroy_resource( resources::unmanaged_resource* resource )
{
	// LOCALS
	// weapon_core_inactive_state* 	wpn_state
	// ******

	// CALL SITE INFO
	// <0x59eeb3> -> void* <unknown>(u32)
	// ******

	// FUNCTION BODY[0x59ee80]: 2
	// <0x59ee89>|0x009|+0x01e:'47'
	// <0x59eea7>|0x027|+0x00e:'48'
	// ******
}

} // namespace survarium
