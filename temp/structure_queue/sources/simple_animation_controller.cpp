////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\game\sources\simple_animation_controller.h"

namespace survarium {

// STATE[STUB]
explicit simple_animation_controller::simple_animation_controller( human_npc& owner )
{
	// FUNCTION BODY[0x5bbff0]: 0
	// <0x5bbff0>|0x000|+0x023:'22'	{
	// <0x5bc013>|0x023|      :'23'	}
	// ******
}

// STATE[STUB]
 simple_animation_controller::~simple_animation_controller( )
{
	// FUNCTION BODY[0x5bbf90]: 0
	// <0x5bbf90>|0x000|+0x009:'26'	{
	// <0x5bbf99>|0x009|      :'27'	}
	// ******
}

// STATE[STUB]
void simple_animation_controller::initialize( )
{
	// FUNCTION BODY[0x5bbed0]: 0
	// <0x5bbed0>|0x000|+0x000:'30'	{
	// <0x5bbed0>|0x000|      :'31'	}
	// ******
}

// STATE[STUB]
void simple_animation_controller::set_target( animation_controller_parameters const& target )
{
	// FUNCTION BODY[0x5bbf40]: 1
	// <0x5bbf40>|0x000|+0x045:'35'
	// ******
}

// STATE[STUB]
void simple_animation_controller::query_new_target_if_needed( )
{
	// CALL SITE INFO
	// <0x5bbf2a> -> void < unknown >()
	// <0x5bbf35> -> void < unknown >()
	// ******

	// FUNCTION BODY[0x5bbf10]: 6
	// <0x5bbf13>|0x003|+0x006:'40'
	// <0>
	// <0x5bbf19>|0x009|+0x009:'42'
	// <0x5bbf22>|0x012|+0x00a:'43'
	// <0x5bbf2c>|0x01c|+0x00c:'44'
	// <0>
	// ******
}

// STATE[STUB]
animation::mixing::expression simple_animation_controller::selected_animations( mutable_buffer& buffer )
{
	// CALL SITE INFO
	// <0x5bc06d> -> animation::mixing::expression < unknown >( mutable_buffer&, bool& ) const
	// ******

	// FUNCTION BODY[0x5bc020]: 11
	// <0x5bc020>|0x000|+0x004:'49'	{
	// <0>
	// <0x5bc024>|0x004|+0x008:'51'
	// <0>
	// <1>
	// <0x5bc02c>|0x00c|+0x00f:'54'
	// <0>
	// <1>
	// <0x5bc03b>|0x01b|+0x009:'57'
	// <0x5bc044>|0x024|+0x016:'58'
	// <0>
	// <0x5bc05a>|0x03a|-0x003:'60'
	// <0x5bc057>|0x037|+0x01c:'61'
	// <0x5bc073>|0x053|      :'61'	}
	// ******
}

// STATE[STUB]
animation::mixing::expression simple_animation_controller::try_finalize( base_animation_controller& next_controller, mutable_buffer& buffer )
{
	// FUNCTION BODY[0x5bbef0]: 2
	// <0>
	// <0x5bbef0>|0x000|+0x011:'66'
	// ******
}

// STATE[STUB]
void simple_animation_controller::debug_draw( render::game::renderer& render, render::base_scene_ptr const& scene ) const
{
	// FUNCTION BODY[0x5bbee0]: 1
	// <0x5bbee0>|0x000|+0x000:'70'	{
	// <0>
	// <0x5bbee0>|0x000|      :'72'	}
	// ******
}

	// TYPEDEFS
	// typedef
	// 	vostok::collision::bone_collision_data const*
	// 	iterator_type;

	// typedef
	// 	vostok::collision::bone_collision_data*
	// 	iterator_type;

	// ******

} // namespace survarium
