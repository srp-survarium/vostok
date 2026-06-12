////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\game\sources\main_menu.h"

namespace survarium {

// STATE[STUB]
explicit main_menu::main_menu( game& g )
{
	// FUNCTION BODY[0x5d9cb0]: 1
	// <0x5d9cb8>|0x008|+0x01d:'43'
	// ******
}

// STATE[STUB]
 main_menu::~main_menu( )
{
	// FUNCTION BODY[0x5d9aa0]: 0
	// <0x5d9aa0>|0x000|+0x017:'48'	{
	// <0x5d9ab7>|0x017|      :'49'	}
	// ******
}

// STATE[STUB]
void main_menu::on_deactivate( )
{
	// CALL SITE INFO
	// <0x5d9a8b> -> input::world& < unknown >()
	// <0x5d9a95> -> void < unknown >( input::handler& )
	// ******

	// FUNCTION BODY[0x5d9a60]: 2
	// <0x5d9a63>|0x003|+0x017:'53'
	// <0x5d9a7a>|0x01a|+0x01f:'54'
	// ******
}

// STATE[STUB]
void main_menu::clear_resources( )
{
	// FUNCTION BODY[0x5d99c0]: 1
	// <0x5d99c0>|0x000|+0x000:'58'	{
	// <0>
	// <0x5d99c0>|0x000|      :'60'	}
	// ******
}

// STATE[STUB]
void main_menu::on_activate( )
{
	// CALL SITE INFO
	// <0x5d9a42> -> input::world& < unknown >()
	// <0x5d9a4c> -> void < unknown >( input::handler& )
	// ******

	// FUNCTION BODY[0x5d9a20]: 2
	// <0x5d9a24>|0x004|+0x005:'64'
	// <0x5d9a29>|0x009|+0x027:'65'
	// ******
}

// STATE[STUB]
void main_menu::tick(
	const u32		frame_delta_in_ms,
	const u32		current_time_in_ms,
	const bool		is_game_paused
)
{
	// FUNCTION BODY[0x5d99d0]: 4
	// <0x5d99d3>|0x003|+0x041:'70'
	// <0>
	// <1>
	// <2>
	// ******
}

// STATE[STUB]
void main_menu::query_resources( )
{
	// LOCALS
	// variant< 32 > const*[3] 			data
	// variant< 32 > 					temp_data
	// resources::request[2] 			requests
	// render::scene_configuration 		render_configuration
	// ******

	// FUNCTION BODY[0x5d9bc0]: 24
	// <0>
	// <1>
	// <0x5d9bc7>|0x007|+0x004:'80'
	// <0>
	// <0x5d9bcb>|0x00b|+0x00c:'82'
	// <0x5d9bd7>|0x017|+0x005:'83'
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
	// <0x5d9bdc>|0x01c|+0x0ae:'100'
	// <0>
	// ******
}

// STATE[STUB]
void main_menu::on_resources_ready( resources::queries_result& data )
{
	// FUNCTION BODY[0x5d9ac0]: 8
	// <0x5d9ac4>|0x004|+0x07a:'106'
	// <0x5d9b3e>|0x07e|+0x06d:'107'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
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
