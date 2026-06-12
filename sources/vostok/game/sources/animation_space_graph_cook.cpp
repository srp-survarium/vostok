////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "animation_space_graph_cook.h"
#include "animation_space_graph.h"
#include "animation_space_vertex.h"

namespace survarium {

// STATE[STUB]
 animation_space_graph_cook::animation_space_graph_cook( ai::navigation::world& navigation_world ) :
	// ref member; the same-named param is the obvious source - a matcher confirms
	m_navigation_world( navigation_world )
{
	// FUNCTION BODY[0x7677a0]: 0
	// <0x7677a0>|0x000|+0x06c:'20'	{
	// <0x76780c>|0x06c|      :'21'	}
	// ******
}

// STATE[STUB]
void animation_space_graph_cook::translate_request_path( pcstr request, fs_new::virtual_path_string& new_request ) const
{
	// FUNCTION BODY[0x767900]: 1
	// <0x767900>|0x000|+0x017:'25'
	// ******
}

// STATE[STUB]
void animation_space_graph_cook::translate_query( resources::query_result_for_cook& parent )
{
	// FUNCTION BODY[0x7680e0]: 8
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x7680ea>|0x00a|+0x0a4:'37'
	// ******
}

// STATE[STUB]
void animation_space_graph_cook::delete_resource( resources::resource_base* resource )
{
	// CALL SITE INFO
	// <0x767959> -> void* < unknown >( u32 )
	// ******

	// FUNCTION BODY[0x767920]: 19
	// <0>
	// <1>
	// <0x767921>|0x001|+0x005:'44'
	// <0x767926>|0x006|+0x015:'45'
	// <0x76793b>|0x01b|+0x005:'46'
	// <0x767940>|0x020|+0x011:'47'
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
	// <0x767951>|0x031|+0x00a:'59'
	// <0x76795b>|0x03b|+0x021:'60'
	// ******
}

// STATE[STUB]
u32 get_animation_vertices_count( configs::binary_config_value const& groups_config )
{
	return 0;

	// FUNCTION BODY[0x7678a0]: 14
	// <0>
	// <0x7678a2>|0x002|+0x002:'66'
	// <0x7678a4>|0x004|+0x00d:'67'
	// <0>
	// <0x7678b1>|0x011|+0x00f:'69'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x7678c0>|0x020|+0x00c:'74'
	// <0x7678cc>|0x02c|+0x029:'75'
	// <0>
	// <1>
	// <0x7678f5>|0x055|+0x003:'78'
	// ******
}

// STATE[STUB]
std::pair< u32, u32 > get_animation_mixes_count( configs::binary_config_value const& groups_config )
{
	// FUNCTION BODY[0x767820]: 18
	// <0>
	// <0x767823>|0x003|+0x002:'86'
	// <0x767825>|0x005|+0x017:'87'
	// <0>
	// <0x76783c>|0x01c|+0x004:'89'
	// <0>
	// <1>
	// <0x767840>|0x020|+0x00e:'92'
	// <0>
	// <0x76784e>|0x02e|+0x010:'94'
	// <0>
	// <0x76785e>|0x03e|+0x00c:'96'
	// <0x76786a>|0x04a|+0x020:'97'
	// <0x76788a>|0x06a|+0x00e:'98'
	// <0>
	// <1>
	// <2>
	// <3>
	// ******
}

// STATE[STUB]
void animation_space_graph_cook::on_options_received( resources::queries_result& data )
{
	// LOCALS
	// configs::binary_config_value const* it_end_groups
	// resources::query_result_for_cook* const parent
	// configs::binary_config_ptr 		config
	// ******

	// FUNCTION BODY[0x767f20]: 53
	// <0x767f20>|0x000|+0x006:'106'	{
	// <0x767f26>|0x006|+0x008:'107'
	// <0x767f2e>|0x00e|+0x00f:'108'
	// <0>
	// <1>
	// <0x767f3d>|0x01d|+0x017:'111'
	// <0>
	// <1>
	// <2>
	// <0x767f54>|0x034|+0x03d:'115'
	// <0x767f91>|0x071|+0x009:'116'
	// <0>
	// <1>
	// <0x767f9a>|0x07a|+0x00a:'119'
	// <0>
	// <1>
	// <0x767fa4>|0x084|+0x00e:'122'
	// <0>
	// <0x767fb2>|0x092|+0x005:'124'
	// <0x767fb7>|0x097|+0x00b:'125'
	// <0>
	// <1>
	// <0x767fc2>|0x0a2|+0x014:'128'
	// <0>
	// <0x767fd6>|0x0b6|+0x043:'130'
	// <0x768019>|0x0f9|-0x039:'130'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x767fe0>|0x0c0|+0x00c:'135'
	// <0x767fec>|0x0cc|+0x002:'136'
	// <0x767fee>|0x0ce|+0x00a:'137'
	// <0>
	// <0x767ff8>|0x0d8|+0x01e:'139'
	// <0x768016>|0x0f6|-0x01a:'139'
	// <0>
	// <1>
	// <0x767ffc>|0x0dc|+0x005:'142'
	// <0>
	// <1>
	// <0x768001>|0x0e1|+0x002:'145'
	// <0>
	// <0x768003>|0x0e3|+0x01d:'147'
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
	// <0x768020>|0x100|-0x0d8:'159'
	// <0x767f48>|0x028|+0x161:'160'
	// <0x7680a9>|0x189|      :'160'	}
	// ******
}

// STATE[STUB]
void animation_space_graph_cook::generate_graph_edges( animation_space_graph* graph )
{
	// LOCALS
	// u32 								current_offset
	// animation_space_edge* 			it_edges
	// std::pair< animation_space_vertex const*, animation_space_vertex const* > const* it_begin
	// animation::animation_player 		player
	// const u32 						mixes_count
	// const float 						left_weight
	// ******

	// TYPEDEFS
	// typedef
	// 	buffer_vector< u32 >
	// 	offsets_type;

	// ******

	// FUNCTION BODY[0x767980]: 43
	// <0>
	// <0x767991>|0x011|+0x007:'165'
	// <0x767998>|0x018|+0x00f:'166'
	// <0>
	// <0x7679a7>|0x027|+0x013:'168'
	// <0x7679ba>|0x03a|+0x003:'169'
	// <0x7679bd>|0x03d|+0x00d:'170'
	// <0>
	// <0x7679ca>|0x04a|+0x028:'172'
	// <0x7679f2>|0x072|-0x01f:'172'
	// <0>
	// <0x7679d3>|0x053|+0x006:'174'
	// <0x7679d9>|0x059|+0x01c:'175'
	// <0>
	// <1>
	// <0x7679f5>|0x075|+0x03b:'178'
	// <0x767a30>|0x0b0|-0x026:'178'
	// <0x767a0a>|0x08a|+0x00e:'179'
	// <0x767a18>|0x098|+0x132:'180'
	// <0x767b4a>|0x1ca|-0x114:'180'
	// <0>
	// <0x767a36>|0x0b6|+0x028:'182'
	// <0>
	// <1>
	// <0x767a5e>|0x0de|+0x004:'185'
	// <0x767a62>|0x0e2|+0x004:'186'
	// <0x767a66>|0x0e6|+0x004:'187'
	// <0x767a6a>|0x0ea|+0x002:'188'
	// <0x767a6c>|0x0ec|+0x002:'189'
	// <0x767a6e>|0x0ee|+0x005:'190'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x767a73>|0x0f3|+0x00b:'196'
	// <0>
	// <0x767a7e>|0x0fe|+0x018:'198'
	// <0x767a96>|0x116|-0x016:'198'
	// <0x767a80>|0x100|+0x01e:'199'
	// <0x767a9e>|0x11e|+0x010:'199'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x767aae>|0x12e|+0x0a6:'205'
	// <0>
	// ******
}

// STATE[STUB]
void animation_space_graph_cook::on_animations_loaded( resources::queries_result& data, configs::binary_config_ptr config )
{
	// LOCALS
	// std::pair< animation_space_vertex const*, animation_space_vertex const* >* it_edges
	// configs::binary_config_value const* it_end_groups
	// animation_space_graph* 			graph
	// configs::binary_config_value const* it_groups
	// resources::query_result_for_cook* const parent
	// configs::binary_config_value const& groups
	// animation_space_vertex* 			it_animations
	// std::pair< u32, u32 > 			mixes_count
	// configs::binary_config_value const* it_end_vertices
	// configs::binary_config_value const* it_end_mix
	// animation_space_vertex const* const first_mixable
	// pcstr 							second_path
	// ******

	// FUNCTION BODY[0x767b80]: 88
	// <0x767b8b>|0x00b|+0x003:'211'
	// <0x767b8e>|0x00e|+0x01c:'212'
	// <0>
	// <1>
	// <0x767baa>|0x02a|+0x006:'215'
	// <0x767bb0>|0x030|+0x005:'216'
	// <0>
	// <1>
	// <0x767bb5>|0x035|+0x009:'219'
	// <0x767bbe>|0x03e|+0x016:'220'
	// <0>
	// <0x767bd4>|0x054|+0x003:'222'
	// <0x767bd7>|0x057|+0x00d:'223'
	// <0>
	// <1>
	// <2>
	// <0x767be4>|0x064|+0x008:'227'
	// <0x767bec>|0x06c|+0x02f:'228'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x767c1b>|0x09b|+0x02e:'236'
	// <0>
	// <0x767c49>|0x0c9|+0x027:'238'
	// <0x767c70>|0x0f0|-0x021:'238'
	// <0x767c4f>|0x0cf|+0x025:'239'
	// <0x767c74>|0x0f4|+0x06f:'240'
	// <0>
	// <0x767ce3>|0x163|+0x00b:'242'
	// <0>
	// <0x767cee>|0x16e|+0x006:'244'
	// <0x767cf4>|0x174|+0x016:'245'
	// <0>
	// <0x767d0a>|0x18a|+0x151:'247'
	// <0x767e5b>|0x2db|-0x149:'247'
	// <0>
	// <1>
	// <2>
	// <0x767d12>|0x192|+0x010:'251'
	// <0x767d22>|0x1a2|+0x01a:'252'
	// <0>
	// <0x767d3c>|0x1bc|+0x012:'254'
	// <0>
	// <1>
	// <0x767d4e>|0x1ce|+0x010:'257'
	// <0>
	// <0x767d5e>|0x1de|+0x02c:'259'
	// <0x767d8a>|0x20a|-0x028:'259'
	// <0>
	// <1>
	// <2>
	// <0x767d62>|0x1e2|+0x00b:'263'
	// <0>
	// <0x767d6d>|0x1ed|+0x004:'265'
	// <0x767d71>|0x1f1|+0x01d:'266'
	// <0>
	// <1>
	// <0x767d8e>|0x20e|+0x014:'269'
	// <0>
	// <0x767da2>|0x222|+0x00c:'271'
	// <0>
	// <0x767dae>|0x22e|+0x002:'273'
	// <0x767db0>|0x230|+0x00e:'274'
	// <0>
	// <0x767dbe>|0x23e|+0x099:'276'
	// <0x767e57>|0x2d7|-0x087:'276'
	// <0>
	// <1>
	// <0x767dd0>|0x250|+0x019:'279'
	// <0x767de9>|0x269|+0x021:'280'
	// <0>
	// <0x767e0a>|0x28a|+0x015:'282'
	// <0x767e1f>|0x29f|+0x011:'283'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x767e30>|0x2b0|+0x040:'288'
	// <0>
	// <1>
	// <2>
	// <0x767e70>|0x2f0|+0x007:'292'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x767e77>|0x2f7|+0x065:'297'
	// <0x767edc>|0x35c|+0x00b:'298'
	// ******
}

} // namespace survarium
