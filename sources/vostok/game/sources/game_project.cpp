////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "game_project.h"

#include "render_visual.h"
#include "damage_zone.h"          // complete damage_zone for the damage_zone_ptr vector dtor
#include <vostok/game_core/ladder.h> // complete ladder for the ladder_ptr vector dtor

namespace survarium {

// STATE[STUB]
 simple_game_project::simple_game_project( base_game_scene& s ) :
	// ref member; the same-named param is the obvious source - a matcher
	// confirms when this TU is enabled
	m_game_scene( s )
{
	// FUNCTION BODY[0x5cc560]
	// <0x5cc560>|0x000|      :'23'	{
	// ******
}

// STATE[STUB]
 simple_game_project::~simple_game_project( )
{
	// FUNCTION BODY[0x5cc3c0]: 2
	// <0x5cc3c6>|0x006|+0x01f:'27'
	// <0x5cc3e5>|0x025|+0x016:'28'
	// ******
}

// STATE[STUB]
bool simple_game_project::all_loaded( )
{
	return false;

	// FUNCTION BODY[0x5cc300]: 5
	// <0x5cc300>|0x000|+0x000:'32'	{
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5cc300>|0x000|+0x038:'37'
	// <0x5cc338>|0x038|-0x001:'37'
	// <0x5cc337>|0x037|+0x003:'38'
	// <0x5cc33a>|0x03a|      :'38'	}
	// ******
}

// STATE[STUB]
void simple_game_project::on_npc_loaded( resources::queries_result& data )
{
	// FUNCTION BODY[0x5cc370]: 2
	// <0x5cc370>|0x000|+0x001:'41'	{
	// <0>
	// <0x5cc371>|0x001|+0x038:'43'
	// <0x5cc3a9>|0x039|-0x003:'43'
	// <0x5cc3a6>|0x036|+0x00c:'44'
	// <0x5cc3b2>|0x042|      :'44'	}
	// ******
}

// STATE[STUB]
void simple_game_project::insert( scheduler& scheduler )
{
	// CALL SITE INFO
	// <0x5cc74d> -> void < unknown >()
	// <0x5cc79a> -> void < unknown >( zone_group*, physics::world*, scheduler& )
	// <0x5cc7c8> -> void < unknown >( physics::world*, scheduler& )
	// <0x5cc824> -> void < unknown >( physics::world* )
	// ******

	// FUNCTION BODY[0x5cc6a0]: 49
	// <0x5cc6a7>|0x007|+0x019:'48'
	// <0x5cc6c0>|0x020|+0x042:'49'
	// <0>
	// <0x5cc702>|0x062|+0x00e:'51'
	// <0x5cc710>|0x070|+0x026:'52'
	// <0>
	// <0x5cc736>|0x096|+0x006:'54'
	// <0x5cc73c>|0x09c|+0x006:'55'
	// <0>
	// <0x5cc742>|0x0a2|+0x004:'57'
	// <0>
	// <0x5cc746>|0x0a6|+0x002:'59'
	// <0x5cc748>|0x0a8|+0x00e:'60'
	// <0>
	// <1>
	// <0x5cc756>|0x0b6|+0x006:'63'
	// <0x5cc75c>|0x0bc|+0x006:'64'
	// <0x5cc762>|0x0c2|+0x00e:'65'
	// <0x5cc770>|0x0d0|+0x00b:'66'
	// <0x5cc77b>|0x0db|+0x028:'67'
	// <0>
	// <0x5cc7a3>|0x103|+0x006:'69'
	// <0x5cc7a9>|0x109|+0x006:'70'
	// <0x5cc7af>|0x10f|+0x004:'71'
	// <0x5cc7b3>|0x113|+0x01e:'72'
	// <0>
	// <1>
	// <0x5cc7d1>|0x131|+0x006:'75'
	// <0x5cc7d7>|0x137|+0x006:'76'
	// <0>
	// <0x5cc7dd>|0x13d|+0x004:'78'
	// <0>
	// <1>
	// <0x5cc7e1>|0x141|+0x01b:'81'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x5cc7fc>|0x15c|+0x006:'90'
	// <0x5cc802>|0x162|+0x006:'91'
	// <0x5cc808>|0x168|+0x008:'92'
	// <0x5cc810>|0x170|+0x01d:'93'
	// <0>
	// <1>
	// <2>
	// ******
}

// STATE[STUB]
void simple_game_project::remove( )
{
	// CALL SITE INFO
	// <0x5cc90d> -> void < unknown >()
	// <0x5cc92d> -> void < unknown >()
	// <0x5cc966> -> void < unknown >()
	// <0x5cc987> -> void < unknown >()
	// ******

	// FUNCTION BODY[0x5cc870]: 47
	// <0x5cc872>|0x002|+0x00e:'100'
	// <0x5cc880>|0x010|+0x040:'101'
	// <0>
	// <0x5cc8c0>|0x050|+0x010:'103'
	// <0x5cc8d0>|0x060|+0x026:'104'
	// <0>
	// <0x5cc8f6>|0x086|+0x006:'106'
	// <0x5cc8fc>|0x08c|+0x006:'107'
	// <0x5cc902>|0x092|+0x004:'108'
	// <0x5cc906>|0x096|+0x010:'109'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5cc916>|0x0a6|+0x006:'116'
	// <0x5cc91c>|0x0ac|+0x006:'117'
	// <0x5cc922>|0x0b2|+0x004:'118'
	// <0x5cc926>|0x0b6|+0x010:'119'
	// <0>
	// <0x5cc936>|0x0c6|+0x006:'121'
	// <0x5cc93c>|0x0cc|+0x006:'122'
	// <0x5cc942>|0x0d2|+0x00e:'123'
	// <0x5cc950>|0x0e0|+0x00b:'124'
	// <0x5cc95b>|0x0eb|+0x014:'125'
	// <0>
	// <1>
	// <0x5cc96f>|0x0ff|+0x006:'128'
	// <0x5cc975>|0x105|+0x006:'129'
	// <0>
	// <0x5cc97b>|0x10b|+0x005:'131'
	// <0>
	// <0x5cc980>|0x110|+0x002:'133'
	// <0x5cc982>|0x112|+0x00e:'134'
	// <0>
	// <1>
	// <0x5cc990>|0x120|+0x006:'137'
	// <0x5cc996>|0x126|+0x006:'138'
	// <0>
	// <0x5cc99c>|0x12c|+0x004:'140'
	// <0>
	// <1>
	// <0x5cc9a0>|0x130|+0x00e:'143'
	// <0>
	// <1>
	// <2>
	// ******
}

// STATE[STUB]
void render_visual::insert( base_game_scene& w )
{
	// FUNCTION BODY[0x5cc670]: 4
	// <0x5cc670>|0x000|+0x007:'152'
	// <0>
	// <1>
	// <0x5cc677>|0x007|+0x01f:'155'
	// ******
}

// STATE[STUB]
void render_visual::remove( base_game_scene& w )
{
	// FUNCTION BODY[0x5cc840]: 4
	// <0x5cc840>|0x000|+0x007:'160'
	// <0>
	// <1>
	// <0x5cc847>|0x007|+0x01f:'163'
	// ******
}

// STATE[STUB]
victory_items_container_core* simple_game_project::get_items_container( u8 id )
{
	return NULL;

	// FUNCTION BODY[0x5cc340]: 9
	// <0x5cc340>|0x000|+0x000:'167'	{
	// <0>
	// <0x5cc340>|0x000|+0x014:'169'
	// <0>
	// <0x5cc354>|0x014|+0x00f:'171'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5cc363>|0x023|+0x004:'176'
	// <0x5cc367>|0x027|+0x005:'176'
	// <0x5cc36c>|0x02c|-0x006:'176'
	// <0x5cc366>|0x026|+0x005:'177'
	// <0x5cc36b>|0x02b|+0x004:'177'
	// <0x5cc36f>|0x02f|      :'177'	}
	// ******
}

} // namespace survarium
