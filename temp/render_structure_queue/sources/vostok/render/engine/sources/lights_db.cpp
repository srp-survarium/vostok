////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\engine\sources\lights_db.h"

namespace vostok {
namespace render {

// STATE[STUB]
 lights_db::lights_db( )
{
	// FUNCTION BODY[0x602f80]: 3
	// <0>
	// <1>
	// <0x602f80>|0x000|+0x02c:'28'
	// ******
}

// STATE[STUB]
 lights_db::~lights_db( )
{
	// FUNCTION BODY[0x602ec0]: 4
	// <0x602ec7>|0x007|+0x009:'33'
	// <0x602ed0>|0x010|+0x010:'34'
	// <0>
	// <0x602ee0>|0x020|+0x02c:'36'
	// ******
}

// STATE[STUB]
light_ptr lights_db::get_sun( ) const
{
	// FUNCTION BODY[0x602e10]: 4
	// <0x602e11>|0x001|+0x01c:'41'
	// <0x602e2d>|0x01d|+0x006:'42'
	// <0>
	// <0x602e33>|0x023|+0x004:'44'
	// <0x602e37>|0x027|-0x001:'44'
	// <0x602e36>|0x026|+0x011:'45'
	// ******
}

// STATE[STUB]
light* lights_db::create(
	const lights_db::tree_operation_enum	arg_0 /* lights_db::tree_operation_enum operation */
) const
{
	return NULL;

	// FUNCTION BODY[0x602de0]: 1
	// <0x602de1>|0x001|+0x026:'49'
	// <0x602e07>|0x027|-0x005:'49'
	// <0x602e02>|0x022|+0x007:'50'
	// ******
}

// STATE[STUB]
void lights_db::initialize_sun( light_data& light_to_add )
{
	// FUNCTION BODY[0x602ea0]: 10
	// <0>
	// <0x602ea1>|0x001|+0x00b:'55'
	// <0>
	// <0x602eac>|0x00c|+0x009:'57'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// ******
}

// STATE[STUB]
void fill_light( light& light, light_props* props )
{
	// LOCALS
	// float4x4 						transform
	// float3 							translation
	// float3 							translation
	// ******

	// FUNCTION BODY[0x602fb0]: 105
	// <0x602fbc>|0x00c|+0x016:'143'
	// <0x602fd2>|0x022|+0x58e:'144'
	// <0>
	// <1>
	// <2>
	// <0x603560>|0x5b0|+0x006:'148'
	// <0>
	// <0x603566>|0x5b6|+0x068:'150'
	// <0>
	// <1>
	// <0x6035ce>|0x61e|+0x049:'153'
	// <0>
	// <0x603617>|0x667|-0x603:'155'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x603014>|0x064|+0x006:'161'
	// <0>
	// <0x60301a>|0x06a|+0x068:'163'
	// <0>
	// <1>
	// <0x603082>|0x0d2|+0x052:'166'
	// <0x6030d4>|0x124|+0x015:'167'
	// <0x6030e9>|0x139|-0x0f5:'168'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x602ff4>|0x044|+0x009:'173'
	// <0x602ffd>|0x04d|+0x009:'174'
	// <0x603006>|0x056|+0x009:'175'
	// <0x60300f>|0x05f|+0x1a2:'176'
	// <0x6031b1>|0x201|+0x003:'176'
	// <0>
	// <1>
	// <2>
	// <0x6031b4>|0x204|+0x003:'180'
	// <0>
	// <0x6031b7>|0x207|+0x05f:'182'
	// <0x603216>|0x266|+0x00f:'183'
	// <0x603225>|0x275|+0x010:'184'
	// <0x603235>|0x285|+0x089:'185'
	// <0x6032be>|0x30e|+0x00d:'186'
	// <0>
	// <0x6032cb>|0x31b|+0x0ec:'188'
	// <0x6033b7>|0x407|+0x009:'189'
	// <0x6033c0>|0x410|+0x00d:'190'
	// <0>
	// <0x6033cd>|0x41d|+0x009:'192'
	// <0x6033d6>|0x426|+0x00d:'193'
	// <0x6033e3>|0x433|+0x00c:'194'
	// <0x6033ef>|0x43f|+0x009:'195'
	// <0x6033f8>|0x448|+0x009:'196'
	// <0x603401>|0x451|+0x00c:'197'
	// <0x60340d>|0x45d|+0x00d:'198'
	// <0>
	// <0x60341a>|0x46a|+0x009:'200'
	// <0>
	// <0x603423>|0x473|+0x007:'202'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x60342a>|0x47a|+0x00d:'209'
	// <0x603437>|0x487|+0x00c:'210'
	// <0>
	// <0x603443>|0x493|+0x004:'212'
	// <0x603447>|0x497|+0x01b:'213'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x603462>|0x4b2|+0x01a:'220'
	// <0>
	// <0x60347c>|0x4cc|+0x014:'222'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x603490>|0x4e0|+0x052:'230'
	// <0x6034e2>|0x532|+0x059:'231'
	// <0>
	// <1>
	// <0x60353b>|0x58b|+0x006:'234'
	// <0>
	// <1>
	// <0x603541>|0x591|+0x009:'237'
	// <0x60354a>|0x59a|+0x009:'238'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x603553>|0x5a3|+0x006:'247'
	// ******
}

// STATE[STUB]
void lights_db::add_light( const u32 id, light_props* props )
{
	// LOCALS
	// light_data 						light_to_add
	// ******

	// FUNCTION BODY[0x603660]: 14
	// <0>
	// <1>
	// <0x60366c>|0x00c|+0x041:'254'
	// <0>
	// <1>
	// <0x6036ad>|0x04d|+0x02a:'257'
	// <0x6036d7>|0x077|+0x00e:'258'
	// <0x6036e5>|0x085|+0x035:'259'
	// <0>
	// <0x60371a>|0x0ba|+0x00e:'261'
	// <0>
	// <0x603728>|0x0c8|+0x016:'263'
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
void lights_db::update_light( u32 id, light_props* props )
{
	// FUNCTION BODY[0x603620]: 3
	// <0x603623>|0x003|+0x023:'270'
	// <0>
	// <0x603646>|0x026|+0x010:'272'
	// ******
}

// STATE[STUB]
void lights_db::tick( float time_delta )
{
	// FUNCTION BODY[0x602db0]: 5
	// <0x602db1>|0x001|+0x003:'277'
	// <0x602db4>|0x004|+0x003:'278'
	// <0>
	// <0x602db7>|0x007|+0x004:'280'
	// <0x602dbb>|0x00b|+0x017:'281'
	// ******
}

// STATE[STUB]
void lights_db::remove_light( const u32 id )
{
	// FUNCTION BODY[0x602e50]: 8
	// <0x602e55>|0x005|+0x012:'286'
	// <0>
	// <1>
	// <0x602e67>|0x017|+0x010:'289'
	// <0x602e77>|0x027|+0x00b:'290'
	// <0x602e82>|0x032|+0x008:'291'
	// <0x602e8a>|0x03a|+0x008:'292'
	// <0>
	// ******
}

	// TYPEDEFS
	// typedef
	// 	pbyte
	// 	iterator_type;

	// typedef
	// 	pcvoid*
	// 	iterator_type;

	// typedef
	// 	void**
	// 	iterator_type;

	// typedef
	// 	vostok::fixed_string< 32 >*
	// 	iterator_type;

	// typedef
	// 	vostok::render::light_data*
	// 	iterator_type;

	// typedef
	// 	vostok::render::shader_constant_binding*
	// 	iterator_type;

	// typedef
	// 	vostok::render::signature_layout_pair*
	// 	iterator_type;

	// ******

} // namespace render
} // namespace vostok
