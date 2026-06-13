////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "object_decal.h"

namespace survarium {

static u32 decal_ids = 0;

// STATE[STUB]
 object_decal::object_decal( base_game_scene& w ) :
	game_object_static( w )
{
	m_decal_id = decal_ids++;

	// FUNCTION BODY[0x78f200]: 1
	// <0x78f213>|0x013|+0x021:'25'
	// ******
}

// STATE[STUB]
 object_decal::~object_decal( )
{
	// FUNCTION BODY[0x78f1c0]: 0
	// <0x78f1c0>|0x000|+0x006:'29'	{
	// <0x78f1c6>|0x006|      :'30'	}
	// ******
}

// STATE[STUB]
void object_decal::load(
	configs::binary_config_value const&		t,
	pcstr									__formal,
	boost::function< void( game_object_& ) >&	cb
)
{
	// LOCALS
	// variant< 32 > 					user_data
	// pcstr 							decal_material_name
	// ******

	// FUNCTION BODY[0x78f390]: 56
	// <0x78f39b>|0x00b|+0x017:'34'
	// <0>
	// <1>
	// <0x78f3b2>|0x022|+0x026:'37'
	// <0>
	// <1>
	// <0x78f3d8>|0x048|+0x02c:'40'
	// <0>
	// <1>
	// <0x78f404>|0x074|+0x02c:'43'
	// <0>
	// <1>
	// <0x78f430>|0x0a0|+0x02c:'46'
	// <0>
	// <1>
	// <0x78f45c>|0x0cc|+0x02c:'49'
	// <0>
	// <1>
	// <0x78f488>|0x0f8|+0x04d:'52'
	// <0>
	// <1>
	// <0x78f4d5>|0x145|+0x045:'55'
	// <0>
	// <1>
	// <0x78f51a>|0x18a|+0x045:'58'
	// <0>
	// <1>
	// <0x78f55f>|0x1cf|+0x045:'61'
	// <0>
	// <1>
	// <0x78f5a4>|0x214|+0x045:'64'
	// <0>
	// <1>
	// <0x78f5e9>|0x259|+0x040:'67'
	// <0>
	// <0x78f629>|0x299|+0x01a:'69'
	// <0>
	// <1>
	// <0x78f643>|0x2b3|+0x02f:'72'
	// <0>
	// <0x78f672>|0x2e2|+0x00a:'74'
	// <0x78f67c>|0x2ec|+0x005:'75'
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
	// <0x78f681>|0x2f1|+0x0c4:'89'
	// ******
}

// STATE[STUB]
void object_decal::insert( )
{
	// FUNCTION BODY[0x78f240]: 17
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
	// <0x78f246>|0x006|+0x113:'110'
	// ******
}

// STATE[STUB]
void object_decal::remove( )
{
	// FUNCTION BODY[0x78f360]: 1
	// <0x78f360>|0x000|+0x026:'115'
	// ******
}

// STATE[STUB]
void object_decal::material_ready(
	resources::queries_result&		data,
	render::material_effects_instance_cook_data*	cook_data,
	boost::function< void( game_object_& ) >&	cb
)
{
	// FUNCTION BODY[0x78f110]: 3
	// <0x78f113>|0x003|+0x012:'120'
	// <0x78f125>|0x015|+0x07e:'121'
	// <0x78f1a3>|0x093|+0x00a:'122'
	// ******
}

} // namespace survarium
