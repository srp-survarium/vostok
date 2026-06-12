////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "camera_director.h"

namespace survarium {

// the ctor-local console-command static 'cc_cam_pos' emits a compiler-generated
// atexit destructor; a matcher recovers it with the ctor body.
/*
// STATE[STUB]
void `survarium::camera_director::camera_director'::`2'::`dynamic atexit destructor for 'cc_cam_pos''( )
{
	// FUNCTION BODY[0x7f0840]
	// <0x7d8b50>|0x000|      :'17'	{
	// ******
}
*/

// STATE[STUB]
 camera_director::camera_director( base_game_scene& w ) :
	// ref member; the same-named param is the obvious source - a matcher
	// confirms when this TU is enabled
	m_game_scene( w )
{
	// STATICS
	// static console_commands::cc_float3 cc_cam_pos = <0x4c2ac60>;
	// ******

	// FUNCTION BODY[0x5ccad0]: 8
	// <0x5ccade>|0x00e|+0x0b6:'23'
	// <0>
	// <0x5ccb94>|0x0c4|+0x071:'25'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5ccc05>|0x135|+0x050:'30'
	// ******
}

// STATE[STUB]
void camera_director::set_position_direction( float3 const& p, float3 const& d )
{
	// FUNCTION BODY[0x5cca00]: 1
	// <0x5cca06>|0x006|+0x0b9:'35'
	// ******
}

// STATE[STUB]
void camera_director::tick( )
{
	// CALL SITE INFO
	// <0x5cc9f4> -> void < unknown >()
	// ******

	// FUNCTION BODY[0x5cc9e0]: 2
	// <0x5cc9e0>|0x000|+0x009:'40'
	// <0x5cc9e9>|0x009|+0x00d:'41'
	// ******
}

// STATE[STUB]
void camera_director::apply( )
{
	// LOCALS
	// float2 							window_size
	// ******

	// CALL SITE INFO
	// <0x5ccddf> -> float2 < unknown >() const
	// <0x5ccdf5> -> float4x4 < unknown >( float2 const& ) const
	// ******

	// FUNCTION BODY[0x5ccda0]: 7
	// <0x5ccdad>|0x00d|+0x00c:'46'
	// <0>
	// <0x5ccdb9>|0x019|+0x003:'48'
	// <0x5ccdbc>|0x01c|+0x025:'49'
	// <0x5ccde1>|0x041|+0x022:'50'
	// <0>
	// <0x5cce03>|0x063|+0x00e:'52'
	// ******
}

// STATE[STUB]
void camera_director::switch_to_camera( game_camera* c, pcstr camera_name )
{
	// CALL SITE INFO
	// <0x5ccd57> -> void < unknown >( bool )
	// <0x5ccd64> -> void < unknown >()
	// <0x5ccd78> -> void < unknown >( camera_director* )
	// <0x5ccd8b> -> void < unknown >( bool )
	// ******

	// FUNCTION BODY[0x5ccc60]: 24
	// <0x5ccc69>|0x009|+0x0c6:'57'
	// <0>
	// <1>
	// <2>
	// <0x5ccd2f>|0x0cf|+0x012:'61'
	// <0>
	// <0x5ccd41>|0x0e1|+0x00b:'63'
	// <0>
	// <1>
	// <0x5ccd4c>|0x0ec|+0x004:'66'
	// <0x5ccd50>|0x0f0|+0x009:'67'
	// <0>
	// <0x5ccd59>|0x0f9|+0x00d:'69'
	// <0>
	// <1>
	// <0x5ccd66>|0x106|+0x006:'72'
	// <0>
	// <0x5ccd6c>|0x10c|+0x004:'74'
	// <0>
	// <0x5ccd70>|0x110|+0x00a:'76'
	// <0>
	// <0x5ccd7a>|0x11a|+0x004:'78'
	// <0x5ccd7e>|0x11e|+0x00f:'79'
	// <0>
	// ******
}

// STATE[STUB]
void camera_director::on_focus( bool b_focus_enter )
{
	// CALL SITE INFO
	// <0x5cc9d4> -> void < unknown >( bool )
	// ******

	// FUNCTION BODY[0x5cc9c0]: 2
	// <0x5cc9c0>|0x000|+0x009:'85'
	// <0x5cc9c9>|0x009|+0x00d:'86'
	// ******
}

} // namespace survarium
