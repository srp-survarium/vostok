#include "pch.h"
#include "scene_cook.h"

namespace vostok {
namespace render {

// STATE[STUB]
scene_cook::scene_cook( ) :
	resources::translate_query_cook(
		resources::scene_class,
		reuse_false,
		use_current_thread_id
	)
{
	// FUNCTION BODY[0x76af90]
	// ******
}

// STATE[STUB]
void scene_cook::translate_query( resources::query_result_for_cook& in_out_query )
{
	// LOCALS
	// scene_configuration 				renderer_configuration
	// variant< 32 > 					data
	// ******

	// FUNCTION BODY[0x76b140]: 26
	// <0>
	// <1>
	// <0x76b14a>|0x00a|+0x020:'30'
	// <0x76b16a>|0x02a|+0x00a:'31'
	// <0>
	// <0x76b174>|0x034|+0x026:'33'
	// <0x76b19a>|0x05a|+0x029:'34'
	// <0>
	// <0x76b1c3>|0x083|+0x00b:'36'
	// <0>
	// <0x76b1ce>|0x08e|+0x010:'38'
	// <0x76b1de>|0x09e|+0x00b:'39'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x76b1e9>|0x0a9|+0x0ee:'48'
	// <0x76b2d7>|0x197|+0x020:'49'
	// <0>
	// <0x76b2f7>|0x1b7|+0x020:'51'
	// <0x76b317>|0x1d7|-0x029:'52'
	// <0>
	// <0x76b2ee>|0x1ae|+0x036:'54'
	// ******
}

// STATE[STUB]
void scene_cook::on_particle_world_created(
	resources::queries_result&				result,
	scene*									created_resource,
	resources::query_result_for_cook*		in_out_query
)
{
	// FUNCTION BODY[0x76b070]: 4
	// <0x76b071>|0x001|+0x089:'58'
	// <0x76b0fa>|0x08a|+0x006:'59'
	// <0x76b100>|0x090|+0x02a:'60'
	// <0x76b12a>|0x0ba|+0x00b:'61'
	// ******
}

// STATE[STUB]
void scene_cook::delete_resource( resources::resource_base* resource )
{
	// CALL SITE INFO
	// <0x76b053> -> void* < unknown >( u32 )
	// ******

	// FUNCTION BODY[0x76aff0]: 5
	// <0>
	// <1>
	// <0x76aff0>|0x000|+0x045:'68'
	// <0>
	// <0x76b035>|0x045|+0x036:'70'
	// ******
}
} // namespace render
} // namespace vostok
