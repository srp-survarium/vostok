////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "object_wire.h"

namespace vostok {
namespace memory {
	class writer;
} // namespace memory
} // namespace vostok

namespace survarium {

// STATE[STUB]
 object_wire::object_wire( base_game_scene& w ) :
	game_object_static( w )
{
	// FUNCTION BODY[0x7813f0]
	// <0x7813f0>|0x000|      :'19'	{
	// ******
}

// STATE[STUB]
 object_wire::~object_wire( )
{
	// FUNCTION BODY[0x781380]: 1
	// <0x781389>|0x009|+0x028:'23'
	// ******
}

// STATE[STUB]
void create_wire_visual_source(
	memory::writer&		writer,
	float3*				points,
	u32					points_count,
	pcstr				material_name,
	float				wire_width
)
{
	// LOCALS
	// render::model_header 			hdr
	// ******

	// CALL SITE INFO
	// <0x78129a> -> void < unknown >( pcvoid, u32 )
	// <0x7812cc> -> void < unknown >( pcvoid, u32 )
	// <0x7812e8> -> void < unknown >( pcvoid, u32 )
	// <0x781321> -> void < unknown >( pcvoid, u32 )
	// ******

	// FUNCTION BODY[0x781210]: 25
	// <0>
	// <1>
	// <2>
	// <0x781313>|0x103|-0x100:'35'
	// <0x781213>|0x003|+0x033:'36'
	// <0x781246>|0x036|+0x028:'37'
	// <0>
	// <1>
	// <0x78126e>|0x05e|+0x01e:'40'
	// <0x78128c>|0x07c|+0x010:'41'
	// <0x78129c>|0x08c|+0x007:'42'
	// <0>
	// <0x7812a3>|0x093|+0x00a:'44'
	// <0x7812ad>|0x09d|+0x021:'45'
	// <0x7812ce>|0x0be|+0x01c:'46'
	// <0x7812ea>|0x0da|+0x007:'47'
	// <0>
	// <0x7812f1>|0x0e1|+0x00a:'49'
	// <0>
	// <0x7812fb>|0x0eb|+0x014:'51'
	// <0>
	// <0x78130f>|0x0ff|+0x008:'53'
	// <0x781317>|0x107|+0x014:'54'
	// <0>
	// <0x78132b>|0x11b|+0x04b:'56'
	// ******
}

// STATE[STUB]
void object_wire::load(
	configs::binary_config_value const&		t,
	pcstr									__formal,
	boost::function< void( game_object_& ) >&	cb
)
{
	// LOCALS
	// memory::writer 					writer
	// const_buffer 					creation_buffer
	// fixed_string< 32 > 				wire_name
	// ******

	// FUNCTION BODY[0x7815b0]: 45
	// <0x7815be>|0x00e|+0x01d:'62'
	// <0x7815db>|0x02b|+0x01d:'63'
	// <0x7815f8>|0x048|+0x01c:'64'
	// <0>
	// <0x781614>|0x064|+0x004:'66'
	// <0>
	// <0x781618>|0x068|+0x023:'68'
	// <0x78163b>|0x08b|-0x00a:'68'
	// <0x781631>|0x081|+0x002:'69'
	// <0x781633>|0x083|+0x044:'70'
	// <0x781677>|0x0c7|-0x037:'70'
	// <0>
	// <0x781640>|0x090|+0x016:'72'
	// <0x781656>|0x0a6|+0x012:'73'
	// <0x781668>|0x0b8|+0x011:'74'
	// <0>
	// <1>
	// <2>
	// <0x781679>|0x0c9|+0x00d:'78'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x781686>|0x0d6|+0x045:'83'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x7816cb>|0x11b|+0x015:'89'
	// <0>
	// <1>
	// <0x7816e0>|0x130|+0x016:'92'
	// <0x7816f6>|0x146|+0x006:'93'
	// <0x7816fc>|0x14c|+0x02e:'94'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x78172a>|0x17a|+0x0b6:'101'
	// <0x7817e0>|0x230|+0x05a:'102'
	// <0>
	// <0x78183a>|0x28a|+0x0bc:'104'
	// <0x7818f6>|0x346|-0x114:'105'
	// <0>
	// ******
}

// STATE[STUB]
void object_wire::insert( )
{
	// FUNCTION BODY[0x781530]: 2
	// <0x781530>|0x000|+0x018:'112'
	// <0x781548>|0x018|+0x026:'113'
	// ******
}

// STATE[STUB]
void object_wire::remove( )
{
	// FUNCTION BODY[0x781570]: 2
	// <0x781570>|0x000|+0x018:'118'
	// <0x781588>|0x018|+0x020:'119'
	// ******
}

// STATE[STUB]
void object_wire::resources_ready( resources::queries_result& data, boost::function< void( game_object_& ) >& cb )
{
	// LOCALS
	// const_buffer 					user_data_to_create
	// ******

	// FUNCTION BODY[0x781420]: 8
	// <0>
	// <0x78142c>|0x00c|+0x015:'125'
	// <0x781441>|0x021|+0x011:'126'
	// <0x781452>|0x032|+0x016:'127'
	// <0>
	// <0x781468>|0x048|+0x0a9:'129'
	// <0x781511>|0x0f1|+0x009:'130'
	// <0>
	// ******
}

} // namespace survarium
