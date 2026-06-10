////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_cook.h>

#include <vostok/game_core/weapon_core.h>
#include "game_core_memory.h"

namespace survarium {

// STATE[100%|DONE]
weapon_core_cook::weapon_core_cook( ) :
	resources::translate_query_cook( resources::weapon_class, reuse_false, use_current_thread_id )
{
	resources::register_cook( this );
}

// STATE[STUB]
// void survarium::weapon_core_cook::translate_query(vostok::resources::query_result_for_cook&)
void weapon_core_cook::translate_query( resources::query_result_for_cook& parent )
{
	// LOCALS
	// fs_new::virtual_path_string 	weapon_config_name
	// ******

	// FUNCTION BODY
	// <0x5a068f>|0x00f|+0x00b:'123'
	// <0x5a069a>|0x01a|+0x01d:'124'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x5a06b7>|0x037|+0x11e:'133'
	// ******
}

// STATE[100%|DONE]
void weapon_core_cook::on_weapon_config_loaded( resources::queries_result& data )
{
	resources::query_result_for_cook*	parent			= data.get_parent_query( );
	ASSERT( UNKNOWN_EXPRESSION );
	configs::binary_config_ptr			config_ptr		= static_cast_resource_ptr< configs::binary_config_ptr >( data[0].get_unmanaged_resource( ) );
	weapon_core*						object_to_cook	= VOSTOK_NEW_IMPL( g_allocator, weapon_core );

	process_loading_weapon_core( parent, config_ptr, object_to_cook );
}

// STATE[STUB]
// void survarium::weapon_core_cook::load_weapon_parameters(vostok::resources::resource_ptr<vostok::configs::binary_config,vostok::resources::unmanaged_intrusive_base>, survarium::weapon_core*)
void weapon_core_cook::load_weapon_parameters( configs::binary_config_ptr config_ptr, weapon_core* object_to_cook )
{
	// LOCALS
	// configs::binary_config_value const& weapon_fire_queue_types_cfg
	// u16 							magazine_capacity
	// float 						aim_near_plane_factor
	// configs::binary_config_value const& parameters
	// float 						aim_fov_factor
	// float 						bullet_pierce
	// configs::binary_config_value const& cfg_root
	// u8 							queue_types_count
	// pbyte 						weapon_fire_queue_types
	// float 						bullet_damage
	// configs::binary_config_value const* it_e<1>
	// configs::binary_config_value const* it<1>
	// ******

	// FUNCTION BODY
	// <0x59f161>|0x011|+0x010:'149'
	// <0x59f171>|0x021|+0x00c:'150'
	// <0>
	// <0x59f17d>|0x02d|+0x010:'152'
	// <0>
	// <0x59f18d>|0x03d|+0x016:'154'
	// <0x59f1a3>|0x053|+0x00d:'155'
	// <0>
	// <0x59f1b0>|0x060|+0x017:'157'
	// <0x59f1c7>|0x077|+0x022:'158'
	// <0>
	// <0x59f1e9>|0x099|+0x017:'160'
	// <0x59f200>|0x0b0|+0x022:'161'
	// <0>
	// <0x59f222>|0x0d2|+0x023:'163'
	// <0x59f245>|0x0f5|+0x010:'164'
	// <0>
	// <0x59f255>|0x105|+0x017:'166'
	// <0x59f26c>|0x11c|+0x010:'167'
	// <0>
	// <0x59f27c>|0x12c|+0x046:'169'
	// <0>
	// <0x59f2c2>|0x172|+0x046:'171'
	// <0>
	// <0x59f308>|0x1b8|+0x01c:'173'
	// <0>
	// <0x59f324>|0x1d4|+0x024:'175'
	// <0x59f348>|0x1f8|+0x004:'176'
	// <0x59f34c>|0x1fc|+0x032|[1]:'177'
	// <0x59f37e>|0x22e|+0x014:'178'
	// <0>
	// <0x59f392>|0x242|+0x018:'180'
	// <0>
	// <0x59f3aa>|0x25a|+0x032:'182'
	// <0x59f3dc>|0x28c|+0x035:'183'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_cook::process_loading_weapon_core(vostok::resources::query_result_for_cook* const, vostok::resources::resource_ptr<vostok::configs::binary_config,vostok::resources::unmanaged_intrusive_base>, survarium::weapon_core*)
void weapon_core_cook::process_loading_weapon_core( resources::query_result_for_cook* parent, configs::binary_config_ptr config_ptr, weapon_core* object_to_cook )
{
	// LOCALS
	// fs_new::virtual_path_string 	skeleton_path
	// resources::request[2] 		requests
	// ******

	// FUNCTION BODY
	// <0x5a0470>|0x010|+0x01b:'188'
	// <0>
	// <0x5a048b>|0x02b|+0x00b:'190'
	// <0x5a0496>|0x036|+0x03f:'191'
	// <0>
	// <1>
	// <0x5a04d5>|0x075|+0x015:'194'
	// <0x5a04ea>|0x08a|+0x028:'195'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x5a0512>|0x0b2|+0x095:'205'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_cook::on_core_subresources_ready(vostok::resources::queries_result&, vostok::resources::resource_ptr<vostok::configs::binary_config,vostok::resources::unmanaged_intrusive_base>, survarium::weapon_core*)
void weapon_core_cook::on_core_subresources_ready( resources::queries_result& data, configs::binary_config_ptr config_ptr, weapon_core* object_to_cook )
{
	// LOCALS
	// resources::resource_ptr<animation::skeleton,resources::unmanaged_intrusive_base> weapon_skeleton
	// u32 							resource_index
	// resources::resource_ptr<weapon_user_animations_container,resources::unmanaged_intrusive_base> user_animations
	// resources::query_result_for_cook* parent
	// ******

	// FUNCTION BODY
	// <0x5a033a>|0x00a|+0x00b:'210'
	// <0x5a0345>|0x015|+0x00c:'211'
	// <0x5a0351>|0x021|+0x00c:'212'
	// <0>
	// <0x5a035d>|0x02d|+0x007:'214'
	// <0x5a0364>|0x034|+0x03b:'215'
	// <0x5a039f>|0x06f|+0x058:'216'
	// <0x5a03f7>|0x0c7|+0x00c:'217'
	// <0x5a0403>|0x0d3|+0x012:'218'
	// <0>
	// <0x5a0415>|0x0e5|+0x021:'220'
	// ******
}

// STATE[33.38%|PARTIAL]: delete_helper<doug_lea_allocator,resource_base> picks
// a different inline shape than the target - target inlines strip_pointer
// (mov ecx,[g_allocator]; push ecx) and passes 2 cdecl args; base out-of-lines
// strip_pointer and passes &resource in edi. Same delta as weapon_ammunition_cook.
void weapon_core_cook::delete_resource( resources::resource_base* resource )
{
	VOSTOK_DELETE_IMPL( g_allocator, resource );

	// STRUCTURE DIFF: target 1 stmts / base 1 stmts
	// SIZE +0x3 | 187 | VOSTOK_DELETE_IMPL( g_allocator, resource );
	// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is the delete_helper strip_pointer
	// inline-vs-call/convention split, non-steerable LTCG.
}

// STATE[STUB]
// void survarium::weapon_core_cook::query_weapon_states(vostok::resources::query_result_for_cook* const, vostok::resources::resource_ptr<vostok::configs::binary_config,vostok::resources::unmanaged_intrusive_base>, survarium::weapon_core*)
void weapon_core_cook::query_weapon_states( resources::query_result_for_cook* parent, configs::binary_config_ptr config_ptr, weapon_core* object_to_cook )
{
	// LOCALS
	// u32 							requests_count
	// configs::binary_config_value states_cfg
	// buffer_vector<variant<32> const *> user_data_ptrs
	// buffer_vector<variant<32> > 	user_data
	// float 						rounds_per_second
	// float 						reload_time
	// weapon_state_creation_params const* params
	// const_buffer 				params_buffer
	// fixed_vector<resources::creation_request,10> requests
	// u32 							i
	// ******

	// FUNCTION BODY
	// <0x59fc01>|0x021|+0x037:'230'
	// <0x59fc38>|0x058|+0x02f:'231'
	// <0x59fc67>|0x087|+0x08b:'232'
	// <0x59fcf2>|0x112|+0x00d:'233'
	// <0>
	// <1>
	// <0x59fcff>|0x11f|+0x03c:'236'
	// <0x59fd3b>|0x15b|+0x022:'237'
	// <0x59fd5d>|0x17d|+0x029:'238'
	// <0x59fd86>|0x1a6|+0x043:'239'
	// <0x59fdc9>|0x1e9|+0x043:'240'
	// <0x59fe0c>|0x22c|+0x043:'241'
	// <0x59fe4f>|0x26f|+0x043:'242'
	// <0x59fe92>|0x2b2|+0x043:'243'
	// <0x59fed5>|0x2f5|+0x043:'244'
	// <0x59ff18>|0x338|+0x043:'245'
	// <0>
	// <0x59ff5b>|0x37b|+0x014:'247'
	// <0x59ff6f>|0x38f|+0x043:'248'
	// <0>
	// <0x59ffb2>|0x3d2|+0x014:'250'
	// <0>
	// <0x59ffc6>|0x3e6|+0x012:'252'
	// <0x59ffd8>|0x3f8|+0x043:'253'
	// <0>
	// <1>
	// <0x5a001b>|0x43b|+0x015:'256'
	// <0x5a0030>|0x450|+0x072:'257'
	// <0x5a00a2>|0x4c2|+0x043:'258'
	// <0>
	// <0x5a00e5>|0x505|+0x02d|[1]:'260'
	// <0x5a0112>|0x532|+0x057:'261'
	// <0x5a0169>|0x589|+0x026:'262'
	// <0x5a018f>|0x5af|+0x005:'263'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x5a0194>|0x5b4|+0x126:'271'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_cook::on_weapon_states_ready(vostok::resources::queries_result&, survarium::weapon_state_creation_params const*, survarium::weapon_core*)
void weapon_core_cook::on_weapon_states_ready( resources::queries_result& data, weapon_state_creation_params const* params, weapon_core* object_to_cook )
{
	// LOCALS
	// resources::resource_ptr<weapon_core_base_state,resources::unmanaged_intrusive_base> reload_state
	// resources::resource_ptr<weapon_core_base_state,resources::unmanaged_intrusive_base> inactive_state
	// resources::resource_ptr<weapon_core_base_state,resources::unmanaged_intrusive_base> idle_state
	// resources::resource_ptr<weapon_core_base_state,resources::unmanaged_intrusive_base> fire_state
	// resources::resource_ptr<weapon_core_base_state,resources::unmanaged_intrusive_base> chamber_a_round_state
	// u32 							resource_index
	// resources::resource_ptr<weapon_core_base_state,resources::unmanaged_intrusive_base> show_state
	// resources::resource_ptr<weapon_core_base_state,resources::unmanaged_intrusive_base> chamber_a_round_aimed_state
	// resources::resource_ptr<weapon_core_base_state,resources::unmanaged_intrusive_base> hide_state
	// resources::resource_ptr<weapon_core_base_state,resources::unmanaged_intrusive_base> aim_state
	// resources::resource_ptr<weapon_core_base_state,resources::unmanaged_intrusive_base> aim_fire_state
	// ******

	// CALL SITE INFO
	// <0x59fb3b> -> u32 <unknown>(weapon_core&) const
	// ******

	// FUNCTION BODY
	// <0x59f447>|0x017|+0x00c:'276'
	// <0x59f453>|0x023|+0x032:'277'
	// <0x59f485>|0x055|+0x007:'278'
	// <0x59f48c>|0x05c|+0x087:'279'
	// <0x59f513>|0x0e3|+0x087:'280'
	// <0x59f59a>|0x16a|+0x087:'281'
	// <0x59f621>|0x1f1|+0x087:'282'
	// <0x59f6a8>|0x278|+0x087:'283'
	// <0x59f72f>|0x2ff|+0x087:'284'
	// <0x59f7b6>|0x386|+0x087:'285'
	// <0x59f83d>|0x40d|+0x087:'286'
	// <0>
	// <0x59f8c4>|0x494|+0x119:'288'
	// <0x59f9dd>|0x5ad|+0x119:'289'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <12>
	// <0x59faf6>|0x6c6|+0x030:'303'
	// <0>
	// <0x59fb26>|0x6f6|+0x04b:'305'
	// <0x59fb71>|0x741|+0x013:'306'
	// ******
}

// STATE[100%|DONE]
u32 weapon_core_cook::cooked_object_size( weapon_core& object_to_cook ) const
{
	return sizeof( weapon_core );
}

} // namespace survarium
