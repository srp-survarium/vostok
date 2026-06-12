////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\game\sources\human_npc_cook.h"

namespace survarium {

// STATE[STUB]
explicit human_npc_cook::human_npc_cook( game_world& world )
{
	// FUNCTION BODY[0x7681a0]: 0
	// <0x7681a0>|0x000|+0x06c:'19'	{
	// <0x76820c>|0x06c|      :'20'	}
	// ******
}

// STATE[STUB]
void human_npc_cook::translate_query( resources::query_result_for_cook& parent )
{
	// FUNCTION BODY[0x7688a0]: 23
	// <0x7688ab>|0x00b|+0x023:'24'
	// <0>
	// <0x7688ce>|0x02e|+0x004:'26'
	// <0>
	// <0x7688d2>|0x032|+0x00e:'28'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x7688e0>|0x040|+0x044:'36'
	// <0x768924>|0x084|+0x002:'37'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x768926>|0x086|+0x0a3:'46'
	// ******
}

// STATE[STUB]
void human_npc_cook::delete_resource( resources::resource_base* resource )
{
	// FUNCTION BODY[0x768220]: 1
	// <0x768220>|0x000|+0x045:'51'
	// ******
}

// STATE[STUB]
void human_npc_cook::on_queried_data_received( resources::queries_result& data )
{
	// LOCALS
	// configs::binary_config_ptr 		config
	// ******

	// FUNCTION BODY[0x768800]: 10
	// <0x768800>|0x000|+0x001:'55'	{
	// <0x768801>|0x001|+0x007:'56'
	// <0x768808>|0x008|+0x00d:'57'
	// <0>
	// <1>
	// <0x768815>|0x015|+0x012:'60'
	// <0>
	// <1>
	// <2>
	// <0x768827>|0x027|+0x048:'64'
	// <0x76886f>|0x06f|-0x04f:'65'
	// <0x768820>|0x020|+0x061:'66'
	// <0x768881>|0x081|      :'66'	}
	// ******
}

// STATE[STUB]
void human_npc_cook::on_npc_options_received(
	configs::binary_config_value const&		config_value,
	resources::query_result_for_cook&		parent
)
{
	// LOCALS
	// pcstr 							brain_unit_path
	// pcstr 							space_graph_path
	// pcstr 							model_path
	// ai::brain_unit_cook_params 		cook_brain_unit_params
	// variant< 32 > 					brain_unit_params
	// variant< 32 > const*[4] 			params
	// configs::binary_config_value* 	project_config
	// variant< 32 > 					physics_world
	// resources::request[4] 			requests
	// human_npc* const 				human
	// ******

	// CALL SITE INFO
	// <0x768621> -> sound::world_user& < unknown >() const
	// ******

	// FUNCTION BODY[0x768560]: 50
	// <0x76856f>|0x00f|+0x00e:'70'
	// <0x76857d>|0x01d|+0x024:'71'
	// <0>
	// <0x7685a1>|0x041|+0x034:'73'
	// <0>
	// <1>
	// <0x7685d5>|0x075|+0x00e:'76'
	// <0>
	// <1>
	// <0x7685e3>|0x083|+0x014:'79'
	// <0>
	// <1>
	// <0x7685f7>|0x097|+0x012:'82'
	// <0>
	// <1>
	// <0x768609>|0x0a9|+0x01a:'85'
	// <0x768623>|0x0c3|+0x01f:'86'
	// <0>
	// <0x768642>|0x0e2|+0x004:'88'
	// <0>
	// <1>
	// <0x768646>|0x0e6|+0x027:'91'
	// <0>
	// <1>
	// <0x76866d>|0x10d|+0x01c:'94'
	// <0>
	// <1>
	// <2>
	// <0x768689>|0x129|+0x004:'98'
	// <0x76868d>|0x12d|+0x016:'99'
	// <0>
	// <0x7686a3>|0x143|+0x008:'101'
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
	// <13>
	// <14>
	// <15>
	// <16>
	// <0x7686ab>|0x14b|+0x0e6:'119'
	// ******
}

// STATE[STUB]
void human_npc_cook::on_subresources_loaded( resources::queries_result& data, human_npc* const human )
{
	// LOCALS
	// resources::unmanaged_resource_ptr brain_unit_ptr
	// animated_model_instance_ptr 		model_ptr
	// animation_space_graph_ptr 		new_graph
	// resources::query_result_for_cook* const parent
	// resources::managed_resource_ptr 	default_animation
	// configs::binary_config_value 	human_attributes_config
	// ******

	// FUNCTION BODY[0x768270]: 31
	// <0x768270>|0x000|+0x00a:'123'	{
	// <0x76827a>|0x00a|+0x00e:'124'
	// <0>
	// <0x768288>|0x018|+0x025:'126'
	// <0x7682ad>|0x03d|+0x010:'127'
	// <0>
	// <0x7682bd>|0x04d|+0x024:'129'
	// <0>
	// <1>
	// <0x7682e1>|0x071|+0x016:'132'
	// <0>
	// <1>
	// <2>
	// <0x7682f7>|0x087|+0x041:'136'
	// <0x768338>|0x0c8|+0x049:'137'
	// <0>
	// <0x768381>|0x111|+0x046:'139'
	// <0x7683c7>|0x157|+0x009:'140'
	// <0>
	// <0x7683d0>|0x160|+0x02f:'142'
	// <0x7683ff>|0x18f|+0x034:'143'
	// <0>
	// <0x768433>|0x1c3|+0x04a:'145'
	// <0x76847d>|0x20d|+0x00b:'146'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x768488>|0x218|+0x03a:'151'
	// <0x7684c2>|0x252|+0x00f:'152'
	// <0>
	// <0x7684d1>|0x261|-0x1e3:'154'
	// <0x7682ee>|0x07e|+0x202:'155'
	// <0x7684f0>|0x280|      :'155'	}
	// ******
}

	// TYPEDEFS
	// typedef
	// 	survarium::base_project::resolve_link_object*
	// 	iterator_type;

	// typedef
	// 	survarium::scheduler::record*
	// 	iterator_type;

	// typedef
	// 	vostok::collision::bone_collision_data const*
	// 	iterator_type;

	// typedef
	// 	vostok::collision::bone_collision_data*
	// 	iterator_type;

	// ******

} // namespace survarium
