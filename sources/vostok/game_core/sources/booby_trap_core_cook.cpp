////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include  <vostok/game_core/booby_trap_core_cook.h>

namespace survarium {

// STATE[STUB]
// survarium::booby_trap_core_cook::booby_trap_core_cook()
booby_trap_core_cook::booby_trap_core_cook( ) : resources::translate_query_cook( resources::booby_trap_class, reuse_false, use_current_thread_id )
{
	resources::register_cook( this );

	// FUNCTION BODY
	// <0x761d85>|0x035|+0x00c:'17'
	// ******
}

// STATE[STUB]
// void survarium::booby_trap_core_cook::translate_query(vostok::resources::query_result_for_cook&)
void booby_trap_core_cook::translate_query( resources::query_result_for_cook& parent )
{
	// LOCALS
	// pcstr 						model
	// resources::resource_ptr<configs::binary_config,resources::unmanaged_intrusive_base> config
	// resources::request[1] 		requests
	// fixed_string<260> 			aabb_path
	// ******

	// FUNCTION BODY
	// <0x761ef0>|0x010|+0x00b:'22'
	// <0x761efb>|0x01b|+0x01b:'23'
	// <0>
	// <0x761f16>|0x036|+0x012:'25'
	// <0x761f28>|0x048|+0x00c:'26'
	// <0x761f34>|0x054|+0x010:'27'
	// <0>
	// <1>
	// <0x761f44>|0x064|+0x012:'30'
	// <0x761f56>|0x076|+0x033:'31'
	// <0>
	// <1>
	// <2>
	// <0x761f89>|0x0a9|+0x00b:'35'
	// <0x761f94>|0x0b4|+0x01b:'36'
	// <0>
	// <0x761faf>|0x0cf|+0x01a:'38'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x761fc9>|0x0e9|+0x090:'47'
	// ******
}

// STATE[STUB]
// void survarium::booby_trap_core_cook::on_subresources_loaded(vostok::resources::queries_result&, vostok::resources::resource_ptr<vostok::configs::binary_config,vostok::resources::unmanaged_intrusive_base>)
void booby_trap_core_cook::on_subresources_loaded( resources::queries_result& data, resources::resource_ptr<configs::binary_config,resources::unmanaged_intrusive_base> config )
{
	// LOCALS
	// resources::resource_ptr<configs::binary_config,resources::unmanaged_intrusive_base> aabb
	// booby_trap_core* 			resource
	// ******

	// CALL SITE INFO
	// <0x761e4f> -> booby_trap_core* <unknown>()
	// <0x761e83> -> void <unknown>(configs::binary_config_value const&)
	// <0x761ebf> -> void <unknown>(resources::query_result_for_cook*, booby_trap_core*, resources::resource_ptr<configs::binary_config,resources::unmanaged_intrusive_base>)
	// ******

	// FUNCTION BODY
	// <0x761e0a>|0x00a|+0x00c:'52'
	// <0x761e16>|0x016|+0x00c:'53'
	// <0>
	// <0x761e22>|0x022|+0x022:'55'
	// <0>
	// <1>
	// <0x761e44>|0x044|+0x010:'58'
	// <0x761e54>|0x054|+0x031:'59'
	// <0x761e85>|0x085|+0x016:'60'
	// <0>
	// <1>
	// <0x761e9b>|0x09b|+0x026:'63'
	// ******
}

// STATE[STUB]
// void survarium::booby_trap_core_cook::finish_query(vostok::resources::query_result_for_cook*, survarium::booby_trap_core*)
void booby_trap_core_cook::finish_query( resources::query_result_for_cook* parent, booby_trap_core* resource )
{
	// CALL SITE INFO
	// <0x761db4> -> u32 <unknown>()
	// ******

	// FUNCTION BODY
	// <0x761da9>|0x009|+0x036:'68'
	// <0x761ddf>|0x03f|+0x00c:'69'
	// ******
}

} // namespace survarium
