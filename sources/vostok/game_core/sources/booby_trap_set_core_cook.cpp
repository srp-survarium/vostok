////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include  <vostok/game_core/booby_trap_set_core_cook.h>

namespace survarium {

// STATE[STUB]
// survarium::booby_trap_set_core_cook::booby_trap_set_core_cook()
booby_trap_set_core_cook::booby_trap_set_core_cook( ) : resources::translate_query_cook( resources::booby_trap_set_class, reuse_false, use_current_thread_id )
{
	resources::register_cook( this );

	// FUNCTION BODY
	// <0x762345>|0x035|+0x00c:'18'
	// ******
}

// STATE[STUB]
// void survarium::booby_trap_set_core_cook::translate_query(vostok::resources::query_result_for_cook&)
void booby_trap_set_core_cook::translate_query( resources::query_result_for_cook& parent )
{
	// LOCALS
	// booby_trap_set_cook_data 	cook_data
	// fs_new::virtual_path_string 	config_name
	// ******

	// FUNCTION BODY
	// <0x76275f>|0x00f|+0x00b:'23'
	// <0x76276a>|0x01a|+0x01d:'24'
	// <0>
	// <1>
	// <0x762787>|0x037|+0x00c:'27'
	// <0>
	// <0x762793>|0x043|+0x01d:'29'
	// <0>
	// <0x7627b0>|0x060|+0x012:'31'
	// <0x7627c2>|0x072|+0x00c:'32'
	// <0x7627ce>|0x07e|+0x005:'33'
	// <0>
	// <1>
	// <0x7627d3>|0x083|+0x002:'36'
	// <0>
	// <1>
	// <0x7627d5>|0x085|+0x007:'39'
	// <0x7627dc>|0x08c|+0x007:'40'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x7627e3>|0x093|+0x126:'50'
	// ******
}

// STATE[STUB]
// void survarium::booby_trap_set_core_cook::on_config_ready(vostok::resources::queries_result&, survarium::booby_trap_set_cook_data)
void booby_trap_set_core_cook::on_config_ready( resources::queries_result& data, booby_trap_set_cook_data cook_data )
{
	// LOCALS
	// variant<32> 					udv
	// buffer_vector<variant<32> const *> user_data
	// configs::binary_config_value data_root
	// booby_trap_set_core* 		resource
	// configs::binary_config_ptr config
	// buffer_vector<resources::request> requests
	// u8 							i
	// ******

	// CALL SITE INFO
	// <0x76256d> -> booby_trap_set_core* <unknown>()
	// ******

	// FUNCTION BODY
	// <0x7624e0>|0x020|+0x00c:'55'
	// <0x7624ec>|0x02c|+0x022:'56'
	// <0x76250e>|0x04e|+0x00c:'57'
	// <0>
	// <0x76251a>|0x05a|+0x03c:'59'
	// <0x762556>|0x096|+0x00c:'60'
	// <0>
	// <0x762562>|0x0a2|+0x010:'62'
	// <0x762572>|0x0b2|+0x00d:'63'
	// <0x76257f>|0x0bf|+0x00c:'64'
	// <0>
	// <0x76258b>|0x0cb|+0x008:'66'
	// <0>
	// <0x762593>|0x0d3|+0x015:'68'
	// <0x7625a8>|0x0e8|+0x00d:'69'
	// <0>
	// <1>
	// <0x7625b5>|0x0f5|+0x023:'72'
	// <0x7625d8>|0x118|+0x023:'73'
	// <0>
	// <0x7625fb>|0x13b|+0x008:'75'
	// <0x762603>|0x143|+0x00c:'76'
	// <0x76260f>|0x14f|+0x027|[1]:'77'
	// <0>
	// <0x762636>|0x176|+0x029:'79'
	// <0x76265f>|0x19f|+0x017:'80'
	// <0x762676>|0x1b6|+0x002:'81'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x762678>|0x1b8|+0x0a4:'90'
	// ******
}

// STATE[STUB]
// void survarium::booby_trap_set_core_cook::on_subresources_loaded(vostok::resources::queries_result&, survarium::booby_trap_set_core*, survarium::booby_trap_set_cook_data, vostok::resources::resource_ptr<vostok::configs::binary_config,vostok::resources::unmanaged_intrusive_base>)
void booby_trap_set_core_cook::on_subresources_loaded(
	resources::queries_result&			data,
	booby_trap_set_core*				resource,
	booby_trap_set_cook_data			cook_data,
	configs::binary_config_ptr	config
)
{
	// LOCALS
	// u8 							i<1>
	// resources::resource_ptr<booby_trap_core,resources::unmanaged_intrusive_base> trap<2>
	// ******

	// CALL SITE INFO
	// <0x7624ae> -> void <unknown>(resources::query_result_for_cook*, booby_trap_set_core*, booby_trap_set_cook_data const&, configs::binary_config_ptr)
	// ******

	// FUNCTION BODY
	// <0x7623ca>|0x00a|+0x00c:'98'
	// <0>
	// <0x7623d6>|0x016|+0x023|[1]:'100'
	// <0>
	// <0x7623f9>|0x039|+0x044|[2]:'102'
	// <0x76243d>|0x07d|+0x00c:'103'
	// <0x762449>|0x089|+0x012:'104'
	// <0x76245b>|0x09b|+0x01e:'105'
	// <0x762479>|0x0b9|+0x00d:'106'
	// <0>
	// <0x762486>|0x0c6|+0x02a:'108'
	// ******
}

// STATE[STUB]
// void survarium::booby_trap_set_core_cook::finish_query(vostok::resources::query_result_for_cook*, survarium::booby_trap_set_core*)
void booby_trap_set_core_cook::finish_query( resources::query_result_for_cook* parent, booby_trap_set_core* resource )
{
	// CALL SITE INFO
	// <0x762374> -> u32 <unknown>()
	// ******

	// FUNCTION BODY
	// <0x762369>|0x009|+0x036:'113'
	// <0x76239f>|0x03f|+0x00c:'114'
	// ******
}

} // namespace survarium
