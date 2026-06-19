////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "base_game_scene.h"
#include "game.h"
#include <vostok/render/facade/game_renderer.h>

namespace survarium {

// TU static 's_freeze_culling' (compiler-generated atexit destructor); a
// matcher recovers its type/initializer from the init asm.
/*
// STATE[STUB]
void `dynamic atexit destructor for 's_freeze_culling''( )
{
	// FUNCTION BODY[0x7f02f0]
	// <0x7d85d0>|0x000|      :'24'	{
	// ******
}
*/

// STATE[STUB]
 base_game_scene::base_game_scene( game& g ) :
	// ref member; the same-named param is the obvious source - a matcher
	// confirms when this TU is enabled
	m_game( g )
{
	// FUNCTION BODY[0x5d6ea0]: 1
	// <0x5d6ea0>|0x000|+0x000:'37'	{
	// <0x5d6ea0>|0x000|+0x06c:'38'
	// <0x5d6f0c>|0x06c|-0x004:'38'
	// <0x5d6f08>|0x068|+0x00a:'39'
	// <0x5d6f12>|0x072|      :'39'	}
	// ******
}

// STATE[STUB]
 base_game_scene::~base_game_scene( )
{
	// FUNCTION BODY[0x5d6f20]: 1
	// <0x5d6f22>|0x002|+0x043:'59'
	// ******
}

// STATE[STUB]
math::uint2 const& base_game_scene::output_window_size( ) const
{
	return *( math::uint2 const* )NULL;	// buildability return

	// FUNCTION BODY[0x5d6fa0]: 1
	// <0x5d6fa1>|0x001|+0x045:'64'
	// ******
}

// STATE[STUB]
void base_game_scene::show_movie( flash_movie_resource_ptr& movie )
{
	// FUNCTION BODY[0x5d7240]: 6
	// <0>
	// <1>
	// <2>
	// <0x5d724b>|0x00b|+0x00c:'72'
	// <0x5d7257>|0x017|+0x00f:'73'
	// <0x5d7266>|0x026|+0x035:'74'
	// ******
}

// STATE[STUB]
void base_game_scene::hide_movie( flash_movie_resource_ptr& movie )
{
	// FUNCTION BODY[0x5d7200]: 5
	// <0x5d7201>|0x001|+0x005:'79'
	// <0>
	// <1>
	// <2>
	// <0x5d7206>|0x006|+0x036:'83'
	// ******
}

// STATE[STUB]
void base_game_scene::show_text_manager( flash_text_manager* tm )
{
	// FUNCTION BODY[0x5d73b0]: 6
	// <0>
	// <1>
	// <0x5d73b3>|0x003|+0x009:'90'
	// <0>
	// <0x5d73bc>|0x00c|+0x008:'92'
	// <0x5d73c4>|0x014|+0x017:'93'
	// ******
}

// STATE[STUB]
void base_game_scene::hide_text_manager( flash_text_manager* tm )
{
	// FUNCTION BODY[0x5d7390]: 2
	// <0>
	// <0x5d7390>|0x000|+0x017:'99'
	// ******
}

// STATE[STUB]
void base_game_scene::init_physics( )
{
	// CALL SITE INFO
	// <0x5d6e99> -> void < unknown >()
	// ******

	// FUNCTION BODY[0x5d6e60]: 9
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5d6e61>|0x001|+0x030:'108'
	// <0>
	// <1>
	// <2>
	// <3>
	// ******
}

// STATE[STUB]
void base_game_scene::destroy_physics( )
{
	// FUNCTION BODY[0x5d6e20]: 9
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5d6e21>|0x001|+0x038:'122'
	// <0>
	// <1>
	// <2>
	// <3>
	// ******
}

// STATE[STUB]
scheduler& base_game_scene::scheduler( )
{
	return *( survarium::scheduler* )NULL;	// buildability return

	// FUNCTION BODY[0x5d6e00]: 1
	// <0x5d6e00>|0x000|+0x00b:'130'
	// ******
}

// STATE[STUB]
void base_game_scene::tick( const u32 __formal, const u32 current_time_in_ms, const bool is_game_paused )
{
	// CALL SITE INFO
	// <0x5d6df1> -> void < unknown >( const u32 )
	// ******

	// FUNCTION BODY[0x5d6db0]: 8
	// <0x5d6db3>|0x003|+0x01e:'135'
	// <0>
	// <0x5d6dd1>|0x021|+0x010:'137'
	// <0>
	// <1>
	// <0x5d6de1>|0x031|+0x013:'140'
	// <0>
	// <1>
	// ******
}

render::game::renderer& base_game_scene::renderer( ) const
{
	return m_game.renderer( );
}

// STATE[STUB]
render::scene_renderer& base_game_scene::scene_renderer( ) const
{
	return *( render::scene_renderer* )NULL;	// buildability return

	// FUNCTION BODY[0x5d6e10]: 1
	// <0x5d6e10>|0x000|+0x00f:'151'
	// ******
}

// STATE[STUB]
void base_game_scene::on_activate( )
{
	// CALL SITE INFO
	// <0x5d6d66> -> sound::world_user& < unknown >() const
	// <0x5d6d7c> -> void < unknown >( bool )
	// ******

	// FUNCTION BODY[0x5d6d40]: 7
	// <0>
	// <0x5d6d43>|0x003|+0x02c:'157'
	// <0>
	// <1>
	// <2>
	// <0x5d6d6f>|0x02f|+0x00f:'161'
	// <0x5d6d7e>|0x03e|+0x012:'162'
	// ******
}

// STATE[STUB]
void base_game_scene::on_deactivate( )
{
	// CALL SITE INFO
	// <0x5d6d24> -> void < unknown >( bool )
	// ******

	// FUNCTION BODY[0x5d6d10]: 5
	// <0x5d6d10>|0x000|+0x007:'167'
	// <0>
	// <1>
	// <2>
	// <0x5d6d17>|0x007|+0x00f:'171'
	// ******
}

// STATE[STUB]
void base_game_scene::apply_camera( camera_director& cd )
{
	// FUNCTION BODY[0x5d72b0]: 11
	// <0x5d72b8>|0x008|+0x012:'176'
	// <0x5d72ca>|0x01a|+0x00f:'177'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5d72d9>|0x029|+0x092:'184'
	// <0>
	// <0x5d736b>|0x0bb|+0x017:'186'
	// ******
}

// STATE[STUB]
bool base_game_scene::point_to_screen( float3 const& p, float2& result )
{
	// LOCALS
	// float3 							pos
	// float4x4 						vp
	// float4x4 						view
	// ******

	return false;

	// FUNCTION BODY[0x5d6ff0]: 12
	// <0x5d6ff0>|0x000|+0x006:'190'	{
	// <0x5d6ff6>|0x006|+0x019:'191'
	// <0x5d700f>|0x01f|+0x00c:'192'
	// <0>
	// <1>
	// <0x5d701b>|0x02b|+0x018:'195'
	// <0x5d7033>|0x043|+0x018:'196'
	// <0x5d704b>|0x05b|+0x0db:'197'
	// <0>
	// <0x5d7126>|0x136|+0x039:'199'
	// <0x5d715f>|0x16f|+0x01e:'200'
	// <0>
	// <0x5d717d>|0x18d|+0x065:'202'
	// <0x5d71e2>|0x1f2|-0x00d:'202'
	// <0x5d71d5>|0x1e5|+0x011:'203'
	// <0x5d71e6>|0x1f6|      :'203'	}
	// ******
}

// STATE[STUB]
swf_input_translator& base_game_scene::input_translator( )
{
	return *( swf_input_translator* )NULL;	// buildability return

	// FUNCTION BODY[0x5d6d30]: 1
	// <0x5d6d30>|0x000|+0x00b:'206'
	// ******
}

// STATE[STUB]
void base_game_scene::create_text_manager( )
{
	// FUNCTION BODY[0x5d73f0]: 2
	// <0x5d73f1>|0x001|+0x02b:'211'
	// <0x5d741c>|0x02c|+0x02e:'212'
	// ******
}

// STATE[STUB]
void base_game_scene::on_after_tick( )
{
	// FUNCTION BODY[0x5d73e0]: 1
	// <0x5d73e0>|0x000|+0x00c:'217'
	// ******
}

} // namespace survarium
