////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "stats.h"

namespace survarium {

// STATE[STUB]
 stats::stats( ui::world& ui_world ) :
	m_ui_world( ui_world ),			// ref member - owner is the obvious source
	m_odd_row_color( 0 ),			// buildability: matcher supplies real value
	m_even_row_color( 0 )			// buildability: matcher supplies real value
{
	// FUNCTION BODY[0x5daf20]: 1
	// <0x5daf3b>|0x01b|+0x005:'22'
	// ******
}

// STATE[STUB]
 stats::~stats( )
{
	// CALL SITE INFO
	// <0x5da32b> -> void < unknown >( ui::window* )
	// ******

	// FUNCTION BODY[0x5da320]: 1
	// <0x5da320>|0x000|+0x00d:'27'
	// ******
}

// STATE[STUB]
void stats::draw( render::ui::renderer& w, render::scene_view_ptr const& scene_view )
{
	// CALL SITE INFO
	// <0x5da2fe> -> void < unknown >( render::ui::renderer&, render::base_scene_view_ptr const& )
	// <0x5da308> -> ui::window* < unknown >()
	// <0x5da319> -> void < unknown >( bool )
	// ******

	// FUNCTION BODY[0x5da2f0]: 1
	// <0x5da2f0>|0x000|+0x010:'32'
	// ******
}

// STATE[STUB]
void stats::set_fps_stats( const float fps )
{
	// LOCALS
	// char[64] 						buff
	// ******

	// CALL SITE INFO
	// <0x5daf10> -> void < unknown >( pcstr )
	// ******

	// FUNCTION BODY[0x5daee0]: 3
	// <0>
	// <0x5daee3>|0x003|+0x01a:'39'
	// <0x5daefd>|0x01d|+0x015:'40'
	// ******
}

// STATE[STUB]
void stats::set_player_angular_speed( const float speed )
{
	// LOCALS
	// char[64] 						buff
	// ******

	// CALL SITE INFO
	// <0x5daed0> -> void < unknown >( pcstr )
	// ******

	// FUNCTION BODY[0x5daea0]: 3
	// <0>
	// <0x5daea3>|0x003|+0x01a:'46'
	// <0x5daebd>|0x01d|+0x015:'47'
	// ******
}

// STATE[STUB]
void stats::set_player_linear_speed( const float speed )
{
	// LOCALS
	// char[64] 						buff
	// ******

	// CALL SITE INFO
	// <0x5dae90> -> void < unknown >( pcstr )
	// ******

	// FUNCTION BODY[0x5dae60]: 3
	// <0>
	// <0x5dae63>|0x003|+0x01a:'71'
	// <0x5dae7d>|0x01d|+0x015:'72'
	// ******
}

// STATE[STUB]
void stats::set_camera_stats( float3 const& pos, float3 const& dir )
{
	// LOCALS
	// char[64] 						buff3
	// char[64] 						buff1
	// char[64] 						buff2
	// ******

	// CALL SITE INFO
	// <0x5dadd5> -> void < unknown >( pcstr )
	// <0x5dae1f> -> void < unknown >( pcstr )
	// <0x5dae55> -> void < unknown >( pcstr )
	// ******

	// FUNCTION BODY[0x5dad80]: 11
	// <0>
	// <0x5dad86>|0x006|+0x03f:'85'
	// <0x5dadc5>|0x045|+0x012:'86'
	// <0>
	// <1>
	// <0x5dadd7>|0x057|+0x039:'89'
	// <0x5dae10>|0x090|+0x011:'90'
	// <0>
	// <1>
	// <0x5dae21>|0x0a1|+0x021:'93'
	// <0x5dae42>|0x0c2|+0x015:'94'
	// ******
}

// STATE[STUB]
void stats::set_crosshair_info( const float dist )
{
	// FUNCTION BODY[0x5da2e0]: 1
	// <0x5da2e0>|0x000|+0x005:'99'
	// ******
}

// STATE[STUB]
void stats::set_resources_stats( pcstr str )
{
	// CALL SITE INFO
	// <0x5da2d8> -> void < unknown >( pcstr )
	// ******

	// FUNCTION BODY[0x5da2d0]: 0
	// <0x5da2d0>|0x000|+0x000:'103'	{
	// <0x5da2d0>|0x000|      :'104'	}
	// ******
}

// STATE[STUB]
void stats::create( )
{
	// CALL SITE INFO
	// <0x5da33b> -> ui::window* < unknown >()
	// <0x5da349> -> void < unknown >( bool )
	// <0x5da366> -> void < unknown >( float2 const& )
	// <0x5da391> -> void < unknown >( float2 const& )
	// <0x5da39a> -> ui::text* < unknown >()
	// <0x5da3a6> -> ui::window* < unknown >()
	// <0x5da3b1> -> void < unknown >( bool )
	// <0x5da3bb> -> ui::window* < unknown >()
	// <0x5da3d7> -> void < unknown >( float2 const& )
	// <0x5da3e1> -> ui::window* < unknown >()
	// <0x5da40b> -> void < unknown >( float2 const& )
	// <0x5da416> -> void < unknown >( ui::enum_font )
	// <0x5da422> -> void < unknown >( ui::enum_text_mode )
	// <0x5da430> -> void < unknown >( u32 )
	// <0x5da441> -> ui::window* < unknown >()
	// <0x5da44a> -> void < unknown >( ui::window*, bool )
	// <0x5da453> -> ui::text* < unknown >()
	// <0x5da45f> -> ui::window* < unknown >()
	// <0x5da46a> -> void < unknown >( bool )
	// <0x5da474> -> ui::window* < unknown >()
	// <0x5da498> -> void < unknown >( float2 const& )
	// <0x5da4a2> -> ui::window* < unknown >()
	// <0x5da4cc> -> void < unknown >( float2 const& )
	// <0x5da4d7> -> void < unknown >( ui::enum_font )
	// <0x5da4e3> -> void < unknown >( ui::enum_text_mode )
	// <0x5da4f1> -> void < unknown >( u32 )
	// <0x5da502> -> ui::window* < unknown >()
	// <0x5da50b> -> void < unknown >( ui::window*, bool )
	// <0x5da514> -> ui::text* < unknown >()
	// <0x5da520> -> ui::window* < unknown >()
	// <0x5da52b> -> void < unknown >( bool )
	// <0x5da535> -> ui::window* < unknown >()
	// <0x5da559> -> void < unknown >( float2 const& )
	// <0x5da563> -> ui::window* < unknown >()
	// <0x5da58d> -> void < unknown >( float2 const& )
	// <0x5da598> -> void < unknown >( ui::enum_font )
	// <0x5da5a4> -> void < unknown >( ui::enum_text_mode )
	// <0x5da5b2> -> void < unknown >( u32 )
	// <0x5da5c3> -> ui::window* < unknown >()
	// <0x5da5cc> -> void < unknown >( ui::window*, bool )
	// <0x5da5d5> -> ui::text* < unknown >()
	// <0x5da5e1> -> ui::window* < unknown >()
	// <0x5da5ec> -> void < unknown >( bool )
	// <0x5da5f6> -> ui::window* < unknown >()
	// <0x5da61a> -> void < unknown >( float2 const& )
	// <0x5da624> -> ui::window* < unknown >()
	// <0x5da64e> -> void < unknown >( float2 const& )
	// <0x5da659> -> void < unknown >( ui::enum_font )
	// <0x5da665> -> void < unknown >( ui::enum_text_mode )
	// <0x5da673> -> void < unknown >( u32 )
	// <0x5da684> -> ui::window* < unknown >()
	// <0x5da68d> -> void < unknown >( ui::window*, bool )
	// <0x5da696> -> ui::text* < unknown >()
	// <0x5da6a2> -> ui::window* < unknown >()
	// <0x5da6ad> -> void < unknown >( bool )
	// <0x5da6b7> -> ui::window* < unknown >()
	// <0x5da6db> -> void < unknown >( float2 const& )
	// <0x5da6e5> -> ui::window* < unknown >()
	// <0x5da70f> -> void < unknown >( float2 const& )
	// <0x5da71a> -> void < unknown >( ui::enum_font )
	// <0x5da726> -> void < unknown >( ui::enum_text_mode )
	// <0x5da734> -> void < unknown >( u32 )
	// <0x5da745> -> ui::window* < unknown >()
	// <0x5da74e> -> void < unknown >( ui::window*, bool )
	// <0x5da757> -> ui::text* < unknown >()
	// <0x5da763> -> ui::window* < unknown >()
	// <0x5da76e> -> void < unknown >( bool )
	// <0x5da778> -> ui::window* < unknown >()
	// <0x5da79c> -> void < unknown >( float2 const& )
	// <0x5da7a6> -> ui::window* < unknown >()
	// <0x5da7d0> -> void < unknown >( float2 const& )
	// <0x5da7db> -> void < unknown >( ui::enum_font )
	// <0x5da7e7> -> void < unknown >( ui::enum_text_mode )
	// <0x5da7f5> -> void < unknown >( u32 )
	// <0x5da806> -> ui::window* < unknown >()
	// <0x5da80f> -> void < unknown >( ui::window*, bool )
	// <0x5da818> -> ui::text* < unknown >()
	// <0x5da824> -> ui::window* < unknown >()
	// <0x5da82f> -> void < unknown >( bool )
	// <0x5da839> -> ui::window* < unknown >()
	// <0x5da85d> -> void < unknown >( float2 const& )
	// <0x5da867> -> ui::window* < unknown >()
	// <0x5da891> -> void < unknown >( float2 const& )
	// <0x5da89c> -> void < unknown >( ui::enum_font )
	// <0x5da8a8> -> void < unknown >( ui::enum_text_mode )
	// <0x5da8b6> -> void < unknown >( u32 )
	// <0x5da8c7> -> ui::window* < unknown >()
	// <0x5da8d0> -> void < unknown >( ui::window*, bool )
	// <0x5da8d9> -> ui::text* < unknown >()
	// <0x5da8e5> -> ui::window* < unknown >()
	// <0x5da8f0> -> void < unknown >( bool )
	// <0x5da8fa> -> ui::window* < unknown >()
	// <0x5da91e> -> void < unknown >( float2 const& )
	// <0x5da928> -> ui::window* < unknown >()
	// <0x5da952> -> void < unknown >( float2 const& )
	// <0x5da95d> -> void < unknown >( ui::enum_font )
	// <0x5da969> -> void < unknown >( ui::enum_text_mode )
	// <0x5da977> -> void < unknown >( u32 )
	// <0x5da988> -> ui::window* < unknown >()
	// <0x5da991> -> void < unknown >( ui::window*, bool )
	// <0x5da99a> -> ui::text* < unknown >()
	// <0x5da9a6> -> ui::window* < unknown >()
	// <0x5da9b1> -> void < unknown >( bool )
	// <0x5da9bb> -> ui::window* < unknown >()
	// <0x5da9df> -> void < unknown >( float2 const& )
	// <0x5da9e9> -> ui::window* < unknown >()
	// <0x5daa13> -> void < unknown >( float2 const& )
	// <0x5daa1e> -> void < unknown >( ui::enum_font )
	// <0x5daa2a> -> void < unknown >( ui::enum_text_mode )
	// <0x5daa38> -> void < unknown >( u32 )
	// <0x5daa49> -> ui::window* < unknown >()
	// <0x5daa52> -> void < unknown >( ui::window*, bool )
	// <0x5daa5b> -> ui::text* < unknown >()
	// <0x5daa67> -> ui::window* < unknown >()
	// <0x5daa72> -> void < unknown >( bool )
	// <0x5daa7c> -> ui::window* < unknown >()
	// <0x5daaa0> -> void < unknown >( float2 const& )
	// <0x5daaaa> -> ui::window* < unknown >()
	// <0x5daad4> -> void < unknown >( float2 const& )
	// <0x5daadf> -> void < unknown >( ui::enum_font )
	// <0x5daaeb> -> void < unknown >( ui::enum_text_mode )
	// <0x5daaf9> -> void < unknown >( u32 )
	// <0x5dab0a> -> ui::window* < unknown >()
	// <0x5dab13> -> void < unknown >( ui::window*, bool )
	// <0x5dab1c> -> ui::text* < unknown >()
	// <0x5dab28> -> ui::window* < unknown >()
	// <0x5dab33> -> void < unknown >( bool )
	// <0x5dab3d> -> ui::window* < unknown >()
	// <0x5dab61> -> void < unknown >( float2 const& )
	// <0x5dab6b> -> ui::window* < unknown >()
	// <0x5dab95> -> void < unknown >( float2 const& )
	// <0x5daba0> -> void < unknown >( ui::enum_font )
	// <0x5dabac> -> void < unknown >( ui::enum_text_mode )
	// <0x5dabba> -> void < unknown >( u32 )
	// <0x5dabcb> -> ui::window* < unknown >()
	// <0x5dabd4> -> void < unknown >( ui::window*, bool )
	// <0x5dabdd> -> ui::text* < unknown >()
	// <0x5dabe9> -> ui::window* < unknown >()
	// <0x5dabf4> -> void < unknown >( bool )
	// <0x5dabfe> -> ui::window* < unknown >()
	// <0x5dac22> -> void < unknown >( float2 const& )
	// <0x5dac2c> -> ui::window* < unknown >()
	// <0x5dac56> -> void < unknown >( float2 const& )
	// <0x5dac61> -> void < unknown >( ui::enum_font )
	// <0x5dac6d> -> void < unknown >( ui::enum_text_mode )
	// <0x5dac7b> -> void < unknown >( u32 )
	// <0x5dac8c> -> ui::window* < unknown >()
	// <0x5dac95> -> void < unknown >( ui::window*, bool )
	// <0x5dac9f> -> ui::window* < unknown >()
	// <0x5daca8> -> float2 const& < unknown >() const
	// <0x5dacce> -> ui::text* < unknown >()
	// <0x5dacda> -> ui::window* < unknown >()
	// <0x5dace5> -> void < unknown >( bool )
	// <0x5dacef> -> ui::window* < unknown >()
	// <0x5dacfc> -> void < unknown >( float2 const& )
	// <0x5dad06> -> ui::window* < unknown >()
	// <0x5dad30> -> void < unknown >( float2 const& )
	// <0x5dad3b> -> void < unknown >( ui::enum_font )
	// <0x5dad47> -> void < unknown >( ui::enum_text_mode )
	// <0x5dad55> -> void < unknown >( u32 )
	// <0x5dad66> -> ui::window* < unknown >()
	// <0x5dad6f> -> void < unknown >( ui::window*, bool )
	// ******

	// FUNCTION BODY[0x5da330]: 125
	// <0x5da333>|0x003|+0x00d:'126'
	// <0x5da340>|0x010|+0x00b:'127'
	// <0x5da34b>|0x01b|+0x01d:'128'
	// <0x5da368>|0x038|+0x02b:'129'
	// <0>
	// <0x5da393>|0x063|+0x00c:'131'
	// <0x5da39f>|0x06f|+0x014:'132'
	// <0x5da3b3>|0x083|+0x026:'133'
	// <0x5da3d9>|0x0a9|+0x034:'134'
	// <0x5da40d>|0x0dd|+0x00b:'135'
	// <0x5da418>|0x0e8|+0x00c:'136'
	// <0x5da424>|0x0f4|+0x00e:'137'
	// <0x5da432>|0x102|+0x01a:'138'
	// <0>
	// <0x5da44c>|0x11c|+0x00c:'140'
	// <0x5da458>|0x128|+0x014:'141'
	// <0x5da46c>|0x13c|+0x02e:'142'
	// <0x5da49a>|0x16a|+0x034:'143'
	// <0x5da4ce>|0x19e|+0x00b:'144'
	// <0x5da4d9>|0x1a9|+0x00c:'145'
	// <0x5da4e5>|0x1b5|+0x00e:'146'
	// <0x5da4f3>|0x1c3|+0x01a:'147'
	// <0>
	// <0x5da50d>|0x1dd|+0x00c:'149'
	// <0x5da519>|0x1e9|+0x014:'150'
	// <0x5da52d>|0x1fd|+0x02e:'151'
	// <0x5da55b>|0x22b|+0x034:'152'
	// <0x5da58f>|0x25f|+0x00b:'153'
	// <0x5da59a>|0x26a|+0x00c:'154'
	// <0x5da5a6>|0x276|+0x00e:'155'
	// <0x5da5b4>|0x284|+0x01a:'156'
	// <0>
	// <0x5da5ce>|0x29e|+0x00c:'158'
	// <0x5da5da>|0x2aa|+0x014:'159'
	// <0x5da5ee>|0x2be|+0x02e:'160'
	// <0x5da61c>|0x2ec|+0x034:'161'
	// <0x5da650>|0x320|+0x00b:'162'
	// <0x5da65b>|0x32b|+0x00c:'163'
	// <0x5da667>|0x337|+0x00e:'164'
	// <0x5da675>|0x345|+0x01a:'165'
	// <0>
	// <0x5da68f>|0x35f|+0x00c:'167'
	// <0x5da69b>|0x36b|+0x014:'168'
	// <0x5da6af>|0x37f|+0x02e:'169'
	// <0x5da6dd>|0x3ad|+0x034:'170'
	// <0x5da711>|0x3e1|+0x00b:'171'
	// <0x5da71c>|0x3ec|+0x00c:'172'
	// <0x5da728>|0x3f8|+0x00e:'173'
	// <0x5da736>|0x406|+0x01a:'174'
	// <0>
	// <0x5da750>|0x420|+0x00c:'176'
	// <0x5da75c>|0x42c|+0x014:'177'
	// <0x5da770>|0x440|+0x02e:'178'
	// <0x5da79e>|0x46e|+0x034:'179'
	// <0x5da7d2>|0x4a2|+0x00b:'180'
	// <0x5da7dd>|0x4ad|+0x00c:'181'
	// <0x5da7e9>|0x4b9|+0x00e:'182'
	// <0x5da7f7>|0x4c7|+0x01a:'183'
	// <0>
	// <0x5da811>|0x4e1|+0x00c:'185'
	// <0x5da81d>|0x4ed|+0x014:'186'
	// <0x5da831>|0x501|+0x02e:'187'
	// <0x5da85f>|0x52f|+0x034:'188'
	// <0x5da893>|0x563|+0x00b:'189'
	// <0x5da89e>|0x56e|+0x00c:'190'
	// <0x5da8aa>|0x57a|+0x00e:'191'
	// <0x5da8b8>|0x588|+0x01a:'192'
	// <0>
	// <0x5da8d2>|0x5a2|+0x00c:'194'
	// <0x5da8de>|0x5ae|+0x014:'195'
	// <0x5da8f2>|0x5c2|+0x02e:'196'
	// <0x5da920>|0x5f0|+0x034:'197'
	// <0x5da954>|0x624|+0x00b:'198'
	// <0x5da95f>|0x62f|+0x00c:'199'
	// <0x5da96b>|0x63b|+0x00e:'200'
	// <0x5da979>|0x649|+0x01a:'201'
	// <0>
	// <0x5da993>|0x663|+0x00c:'203'
	// <0x5da99f>|0x66f|+0x014:'204'
	// <0x5da9b3>|0x683|+0x02e:'205'
	// <0x5da9e1>|0x6b1|+0x034:'206'
	// <0x5daa15>|0x6e5|+0x00b:'207'
	// <0x5daa20>|0x6f0|+0x00c:'208'
	// <0x5daa2c>|0x6fc|+0x00e:'209'
	// <0x5daa3a>|0x70a|+0x01a:'210'
	// <0>
	// <0x5daa54>|0x724|+0x00c:'212'
	// <0x5daa60>|0x730|+0x014:'213'
	// <0x5daa74>|0x744|+0x02e:'214'
	// <0x5daaa2>|0x772|+0x034:'215'
	// <0x5daad6>|0x7a6|+0x00b:'216'
	// <0x5daae1>|0x7b1|+0x00c:'217'
	// <0x5daaed>|0x7bd|+0x00e:'218'
	// <0x5daafb>|0x7cb|+0x01a:'219'
	// <0>
	// <0x5dab15>|0x7e5|+0x00c:'221'
	// <0x5dab21>|0x7f1|+0x014:'222'
	// <0x5dab35>|0x805|+0x02e:'223'
	// <0x5dab63>|0x833|+0x034:'224'
	// <0x5dab97>|0x867|+0x00b:'225'
	// <0x5daba2>|0x872|+0x00c:'226'
	// <0x5dabae>|0x87e|+0x00e:'227'
	// <0x5dabbc>|0x88c|+0x01a:'228'
	// <0>
	// <0x5dabd6>|0x8a6|+0x00c:'230'
	// <0x5dabe2>|0x8b2|+0x014:'231'
	// <0x5dabf6>|0x8c6|+0x02e:'232'
	// <0x5dac24>|0x8f4|+0x034:'233'
	// <0x5dac58>|0x928|+0x00b:'234'
	// <0x5dac63>|0x933|+0x00c:'235'
	// <0x5dac6f>|0x93f|+0x00e:'236'
	// <0x5dac7d>|0x94d|+0x01a:'237'
	// <0>
	// <0x5dac97>|0x967|+0x024:'239'
	// <0x5dacbb>|0x98b|+0x018:'240'
	// <0x5dacd3>|0x9a3|+0x014:'241'
	// <0x5dace7>|0x9b7|+0x017:'242'
	// <0x5dacfe>|0x9ce|+0x034:'243'
	// <0x5dad32>|0xa02|+0x00b:'244'
	// <0x5dad3d>|0xa0d|+0x00c:'245'
	// <0x5dad49>|0xa19|+0x00e:'246'
	// <0x5dad57>|0xa27|+0x01a:'247'
	// <0>
	// <1>
	// <2>
	// ******
}

} // namespace survarium
