////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "game_generate_shaders.h"

namespace survarium {

// STATE[STUB]
 generate_shaders_world::generate_shaders_world( render::world& render_world ) :
	// ref member; buildability placeholder - the real source pulls the game
	// renderer off render_world (a matcher supplies it)
	m_renderer( *( render::game::renderer* )NULL )
{
	// FUNCTION BODY[0x747c70]: 0
	// <0x747c70>|0x000|+0x013:'22'	{
	// <0x747c83>|0x013|      :'23'	}
	// ******
}

// STATE[STUB]
void generate_shaders_world::generate_renderer_shaders( )
{
	// LOCALS
	// u32[2] 							post_process_quality_values
	// console_commands::console_command* lighting_quality_command
	// u32[2] 							lighting_quality_values
	// u32[2] 							shading_quality_values
	// console_commands::console_command* shading_quality_command
	// u32& 							post_process_quality_command_value
	// u32& 							antialiasing_method_command_value
	// u32& 							lighting_quality_command_value
	// u32& 							shading_quality_command_value
	// u32[2] 							shadow_quality_values
	// u32& 							shadow_quality_command_value
	// u32[3] 							antialiasing_method_values
	// u32 								antialiasing_method
	// u32 								shadow_quality
	// u32 								lighting_quality
	// u32 								shading_quality
	// u32 								post_process_quality
	// long 							waiting_for
	// ******

	// FUNCTION BODY[0x747fd0]: 59
	// <0x747fdf>|0x00f|+0x012:'27'
	// <0x747ff1>|0x021|+0x00c:'28'
	// <0x747ffd>|0x02d|+0x00c:'29'
	// <0x748009>|0x039|+0x00e:'30'
	// <0x748017>|0x047|+0x00e:'31'
	// <0>
	// <0x748025>|0x055|+0x003:'33'
	// <0x748028>|0x058|+0x003:'34'
	// <0>
	// <1>
	// <0x74802b>|0x05b|+0x011:'37'
	// <0>
	// <0x74803c>|0x06c|+0x006:'39'
	// <0x748042>|0x072|+0x006:'40'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x748048>|0x078|+0x01c:'46'
	// <0x748064>|0x094|+0x037:'47'
	// <0x74809b>|0x0cb|+0x008:'48'
	// <0x7480a3>|0x0d3|+0x00e:'49'
	// <0x7480b1>|0x0e1|+0x008:'50'
	// <0>
	// <0x7480b9>|0x0e9|+0x004:'52'
	// <0>
	// <1>
	// <2>
	// <0x7480bd>|0x0ed|+0x320:'56'
	// <0x7483dd>|0x40d|-0x2da:'56'
	// <0>
	// <1>
	// <2>
	// <0x748103>|0x133|+0x018:'60'
	// <0>
	// <0x74811b>|0x14b|+0x015:'62'
	// <0>
	// <0x748130>|0x160|+0x079:'64'
	// <0x7481a9>|0x1d9|+0x01d:'65'
	// <0>
	// <1>
	// <0x7481c6>|0x1f6|+0x008:'68'
	// <0x7481ce>|0x1fe|+0x00a:'69'
	// <0x7481d8>|0x208|+0x00a:'70'
	// <0x7481e2>|0x212|+0x00a:'71'
	// <0x7481ec>|0x21c|+0x00e:'72'
	// <0>
	// <1>
	// <0x7481fa>|0x22a|+0x027:'75'
	// <0>
	// <0x748221>|0x251|+0x024:'77'
	// <0>
	// <0x748245>|0x275|+0x079:'79'
	// <0x7482be>|0x2ee|+0x015:'80'
	// <0x7482d3>|0x303|+0x005:'81'
	// <0>
	// <0x7482d8>|0x308|+0x016:'83'
	// <0x7482ee>|0x31e|+0x14e:'84'
	// <0>
	// ******
}

// STATE[STUB]
void generate_shaders_world::generate_materials_shaders( )
{
	// LOCALS
	// u32[2] 							shading_quality_values
	// u32& 							shading_quality_command_value
	// u32[2] 							shadow_quality_values
	// u32& 							shadow_quality_command_value
	// u32 								shadow_quality
	// u32 								shading_quality
	// long 							waiting_for
	// ******

	// FUNCTION BODY[0x747c90]: 41
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
	// <0x747c9b>|0x00b|+0x058:'103'
	// <0x747cf3>|0x063|+0x2a9:'104'
	// <0x747f9c>|0x30c|-0x29c:'104'
	// <0>
	// <1>
	// <2>
	// <0x747d00>|0x070|+0x01d:'108'
	// <0>
	// <0x747d1d>|0x08d|+0x013:'110'
	// <0>
	// <0x747d30>|0x0a0|+0x077:'112'
	// <0x747da7>|0x117|+0x01d:'113'
	// <0>
	// <1>
	// <0x747dc4>|0x134|+0x008:'116'
	// <0x747dcc>|0x13c|+0x00e:'117'
	// <0>
	// <1>
	// <0x747dda>|0x14a|+0x027:'120'
	// <0>
	// <0x747e01>|0x171|+0x024:'122'
	// <0>
	// <0x747e25>|0x195|+0x077:'124'
	// <0x747e9c>|0x20c|+0x015:'125'
	// <0x747eb1>|0x221|+0x005:'126'
	// <0>
	// <0x747eb6>|0x226|+0x016:'128'
	// <0x747ecc>|0x23c|+0x0f6:'129'
	// <0>
	// ******
}

// STATE[STUB]
void generate_shaders_world::tick( u32 current_frame_id )
{
	// STATICS
	// static u32 						tick_id = <0x4c265c0>;
	// ******

	// FUNCTION BODY[0x748450]: 25
	// <0x748461>|0x011|+0x009:'135'
	// <0>
	// <0x74846a>|0x01a|+0x00c:'137'
	// <0>
	// <0x748476>|0x026|+0x006:'139'
	// <0x74847c>|0x02c|+0x006:'140'
	// <0>
	// <0x748482>|0x032|+0x004:'142'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x748486>|0x036|+0x01b:'147'
	// <0>
	// <0x7484a1>|0x051|+0x016:'149'
	// <0x7484b7>|0x067|+0x0be:'150'
	// <0>
	// <1>
	// <0x748575>|0x125|+0x00d:'153'
	// <0x748582>|0x132|+0x00a:'154'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x74858c>|0x13c|+0x012:'159'
	// ******
}

} // namespace survarium
