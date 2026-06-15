////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "player_cook.h"
#include "profile_skin_visual_cook.h"
#include "player_creation_params.h"

namespace survarium {

// STATE[STUB]
 player_cook::player_cook( ) :
	// base args are the legacy prior (translate_query_cook has no default ctor);
	// a matcher confirms when this TU is enabled
	translate_query_cook( resources::player_class, reuse_true, use_any_thread_id )
{
	// FUNCTION BODY[0x5dcbe0]: 0
	// <0x5dcbe0>|0x000|+0x057:'27'	{
	// <0x5dcc37>|0x057|      :'28'	}
	// ******
}

// STATE[STUB]
void player_cook::translate_query( resources::query_result_for_cook& parent )
{
	// LOCALS
	// fs_new::virtual_path_string 		path
	// ******

	// FUNCTION BODY[0x5de400]: 11
	// <0>
	// <0x5de40d>|0x00d|+0x04c:'33'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x5de459>|0x059|+0x090:'42'
	// ******
}

// STATE[STUB]
void player_cook::delete_resource( resources::resource_base* resource )
{
	// CALL SITE INFO
	// <0x5dcc72> -> void* < unknown >( u32 )
	// ******

	// FUNCTION BODY[0x5dcc40]: 2
	// <0x5dcc42>|0x002|+0x012:'47'
	// <0x5dcc54>|0x014|+0x035:'48'
	// ******
}

// STATE[STUB]
void player_cook::on_config_loaded( resources::queries_result& data )
{
	// LOCALS
	// const u32 						requests_count
	// resources::query_result_for_cook* const parent
	// configs::binary_config_value const& root
	// fs_new::virtual_path_string 		damage_config_path
	// player_parameters_cooker_data* 	player_parameters_cook_data
	// configs::binary_config_ptr 		config
	// vectora< resources::request > 	requests
	// fs_new::virtual_path_string 		model_settings_config_path
	// variant< 32 > 					ud_skin_visual
	// variant< 32 > 					id
	// variant< 32 > 					pd
	// ******

	// FUNCTION BODY[0x5dde10]: 77
	// <0x5dde2c>|0x01c|+0x00b:'53'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5dde37>|0x027|+0x046:'58'
	// <0x5dde7d>|0x06d|+0x015:'59'
	// <0>
	// <0x5dde92>|0x082|+0x005:'61'
	// <0>
	// <0x5dde97>|0x087|+0x02e:'63'
	// <0>
	// <1>
	// <0x5ddec5>|0x0b5|+0x035:'66'
	// <0>
	// <0x5ddefa>|0x0ea|+0x014:'68'
	// <0x5ddf0e>|0x0fe|+0x03a:'69'
	// <0x5ddf48>|0x138|+0x036:'70'
	// <0>
	// <0x5ddf7e>|0x16e|+0x014:'72'
	// <0x5ddf92>|0x182|+0x03a:'73'
	// <0x5ddfcc>|0x1bc|+0x036:'74'
	// <0>
	// <1>
	// <0x5de002>|0x1f2|+0x032:'77'
	// <0x5de034>|0x224|+0x02f:'78'
	// <0>
	// <0x5de063>|0x253|+0x040:'80'
	// <0>
	// <0x5de0a3>|0x293|+0x00e:'82'
	// <0>
	// <0x5de0b1>|0x2a1|+0x003:'84'
	// <0x5de0b4>|0x2a4|+0x017:'85'
	// <0x5de0cb>|0x2bb|+0x014:'86'
	// <0x5de0df>|0x2cf|+0x01b:'87'
	// <0x5de0fa>|0x2ea|+0x017:'88'
	// <0x5de111>|0x301|+0x018:'89'
	// <0x5de129>|0x319|+0x018:'90'
	// <0x5de141>|0x331|+0x018:'91'
	// <0>
	// <1>
	// <0x5de159>|0x349|+0x01e:'94'
	// <0x5de177>|0x367|+0x03b:'95'
	// <0>
	// <0x5de1b2>|0x3a2|+0x006:'97'
	// <0x5de1b8>|0x3a8|+0x02e:'98'
	// <0>
	// <0x5de1e6>|0x3d6|+0x00f:'100'
	// <0x5de1f5>|0x3e5|+0x004:'101'
	// <0x5de1f9>|0x3e9|+0x006:'102'
	// <0x5de1ff>|0x3ef|+0x008:'103'
	// <0>
	// <0x5de207>|0x3f7|+0x00c:'105'
	// <0x5de213>|0x403|+0x019:'106'
	// <0x5de22c>|0x41c|+0x01d:'107'
	// <0>
	// <1>
	// <0x5de249>|0x439|+0x00e:'110'
	// <0x5de257>|0x447|+0x011:'111'
	// <0x5de268>|0x458|+0x01e:'112'
	// <0>
	// <0x5de286>|0x476|+0x00d:'114'
	// <0x5de293>|0x483|+0x009:'115'
	// <0x5de29c>|0x48c|+0x007:'116'
	// <0>
	// <0x5de2a3>|0x493|+0x008:'118'
	// <0x5de2ab>|0x49b|+0x00e:'119'
	// <0x5de2b9>|0x4a9|+0x015:'120'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x5de2ce>|0x4be|+0x09b:'129'
	// ******
}

// STATE[STUB]
void player_cook::on_subresources_loaded(
	resources::queries_result&			data,
	player_creation_params*				params,
	inventory_cooker_data*				inventory_cook_data,
	player_parameters_cooker_data*		player_parameters_cook_data
)
{
	// LOCALS
	// configs::binary_config_ptr 		damage_cfg
	// resources::query_result_for_cook* const parent
	// configs::binary_config_ptr 		settings_cfg
	// variant< 32 > 					new_ud
	// fs_new::virtual_path_string 		hit_params_config_path
	// ******

	// FUNCTION BODY[0x5dd790]: 36
	// <0x5dd79f>|0x00f|+0x00c:'134'
	// <0x5dd7ab>|0x01b|+0x020:'135'
	// <0x5dd7cb>|0x03b|+0x018:'136'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5dd7e3>|0x053|+0x0ac:'141'
	// <0x5dd88f>|0x0ff|+0x0af:'142'
	// <0x5dd93e>|0x1ae|+0x04c:'143'
	// <0x5dd98a>|0x1fa|+0x049:'144'
	// <0x5dd9d3>|0x243|+0x0ad:'145'
	// <0x5dda80>|0x2f0|+0x0af:'146'
	// <0>
	// <1>
	// <2>
	// <0x5ddb2f>|0x39f|+0x11f:'150'
	// <0>
	// <0x5ddc4e>|0x4be|+0x028:'152'
	// <0>
	// <0x5ddc76>|0x4e6|+0x010:'154'
	// <0x5ddc86>|0x4f6|+0x051:'155'
	// <0>
	// <1>
	// <0x5ddcd7>|0x547|+0x010:'158'
	// <0x5ddce7>|0x557|+0x00c:'159'
	// <0x5ddcf3>|0x563|+0x009:'160'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x5ddcfc>|0x56c|+0x0b9:'169'
	// ******
}

// STATE[STUB]
void player_cook::on_hit_params_loaded( resources::queries_result& data, player_creation_params* params )
{
	// FUNCTION BODY[0x5dcd00]: 18
	// <0x5dcd00>|0x000|+0x001:'173'	{
	// <0x5dcd01>|0x001|+0x00b:'174'
	// <0>
	// <1>
	// <2>
	// <0x5dcd0c>|0x00c|+0x0b0:'178'
	// <0>
	// <0x5dcdbc>|0x0bc|+0x01e:'180'
	// <0x5dcdda>|0x0da|+0x004:'181'
	// <0>
	// <1>
	// <0x5dcdde>|0x0de|+0x02b:'184'
	// <0>
	// <1>
	// <2>
	// <0x5dce09>|0x109|+0x01d:'188'
	// <0>
	// <0x5dce26>|0x126|-0x038:'190'
	// <0x5dcdee>|0x0ee|+0x05e:'191'
	// <0x5dce4c>|0x14c|-0x04b:'191'
	// <0x5dce01>|0x101|+0x058:'192'
	// <0x5dce59>|0x159|      :'192'	}
	// ******
}

// STATE[STUB]
 profile_skin_visual_cook::profile_skin_visual_cook( game& g ) :
	// base args are the legacy prior (translate_query_cook has no default ctor);
	// a matcher confirms when this TU is enabled
	translate_query_cook( resources::player_skin_visual_class, reuse_true, use_any_thread_id ),
	m_game( g )
{
	// FUNCTION BODY[0x5dcb60]: 1
	// <0x5dcba1>|0x041|+0x029:'200'
	// ******
}

// STATE[STUB]
void profile_skin_visual_cook::translate_query( resources::query_result_for_cook& parent )
{
	// LOCALS
	// player_profile const* 			profile
	// items_dictionary& 				items_dictionary
	// vectora< resources::request > 	requests
	// slot_def 						current
	// strings::detail::tuples 			STR_JOINA_tuples_unique_identifier
	// ******

	// FUNCTION BODY[0x5dd580]: 29
	// <0>
	// <0x5dd58d>|0x00d|+0x01b:'222'
	// <0>
	// <1>
	// <0x5dd5a8>|0x028|+0x009:'225'
	// <0x5dd5b1>|0x031|+0x006:'226'
	// <0>
	// <0x5dd5b7>|0x037|+0x0f0:'228'
	// <0>
	// <0x5dd6a7>|0x127|-0x0b2:'230'
	// <0>
	// <0x5dd5f5>|0x075|+0x005:'232'
	// <0x5dd5fa>|0x07a|+0x00c:'233'
	// <0x5dd606>|0x086|+0x009:'234'
	// <0>
	// <1>
	// <0x5dd60f>|0x08f|+0x069:'237'
	// <0x5dd678>|0x0f8|+0x042:'238'
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
	// <0x5dd6ba>|0x13a|+0x0ab:'249'
	// ******
}

// STATE[STUB]
void profile_skin_visual_cook::on_configs_loaded(
	resources::queries_result&				data,
	resources::query_result_for_cook*		parent,
	player_profile const*					profile
)
{
	// LOCALS
	// fixed_string< 260 > 				path
	// pcstr 							torso_table_id
	// variant< 32 > 					ud
	// configs::binary_config_value 	default_root
	// pcstr 							pants_table_id
	// configs::binary_config_ptr 		default_config
	// configs::binary_config_value 	current
	// slot_def 						current_slot_def
	// bool 							has_part
	// configs::binary_config_ptr 		cfg
	// ******

	// FUNCTION BODY[0x5dce70]: 94
	// <0>
	// <1>
	// <0x5dce83>|0x013|+0x024:'258'
	// <0x5dcea7>|0x037|+0x00e:'259'
	// <0x5dceb5>|0x045|+0x005:'260'
	// <0>
	// <0x5dceba>|0x04a|+0x010:'262'
	// <0>
	// <0x5dceca>|0x05a|+0x088:'264'
	// <0x5dcf52>|0x0e2|+0x02e:'265'
	// <0>
	// <0x5dcf80>|0x110|+0x007:'267'
	// <0>
	// <1>
	// <0x5dcf87>|0x117|+0x005:'270'
	// <0>
	// <1>
	// <0x5dcf8c>|0x11c|+0x036:'273'
	// <0>
	// <1>
	// <2>
	// <0x5dcfc2>|0x152|+0x016:'277'
	// <0x5dcfd8>|0x168|+0x002:'278'
	// <0x5dcfda>|0x16a|+0x043:'279'
	// <0x5dd01d>|0x1ad|+0x025:'280'
	// <0x5dd042>|0x1d2|+0x02a:'281'
	// <0x5dd06c>|0x1fc|+0x020:'282'
	// <0x5dd08c>|0x21c|+0x02a:'283'
	// <0>
	// <0x5dd0b6>|0x246|+0x386:'285'
	// <0x5dd43c>|0x5cc|-0x358:'285'
	// <0>
	// <1>
	// <2>
	// <0x5dd0e4>|0x274|+0x016:'289'
	// <0x5dd0fa>|0x28a|+0x010:'290'
	// <0>
	// <1>
	// <0x5dd10a>|0x29a|+0x00b:'293'
	// <0x5dd115>|0x2a5|+0x005:'294'
	// <0x5dd11a>|0x2aa|+0x008:'295'
	// <0>
	// <0x5dd122>|0x2b2|+0x05e:'297'
	// <0x5dd180>|0x310|+0x05c:'298'
	// <0>
	// <0x5dd1dc>|0x36c|+0x0ca:'300'
	// <0>
	// <0x5dd2a6>|0x436|+0x00e:'302'
	// <0>
	// <0x5dd2b4>|0x444|+0x042:'304'
	// <0x5dd2f6>|0x486|-0x0ef:'305'
	// <0>
	// <1>
	// <0x5dd207>|0x397|+0x0f4:'308'
	// <0x5dd2fb>|0x48b|-0x0f0:'308'
	// <0x5dd20b>|0x39b|+0x015:'309'
	// <0>
	// <0x5dd220>|0x3b0|+0x00b:'311'
	// <0>
	// <0x5dd22b>|0x3bb|+0x02c:'313'
	// <0>
	// <0x5dd257>|0x3e7|+0x020:'315'
	// <0x5dd277>|0x407|+0x02a:'316'
	// <0x5dd2a1>|0x431|+0x061:'317'
	// <0>
	// <0x5dd302>|0x492|+0x03c:'319'
	// <0x5dd33e>|0x4ce|+0x059:'320'
	// <0>
	// <1>
	// <0x5dd397>|0x527|+0x012:'323'
	// <0x5dd3a9>|0x539|+0x055:'324'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5dd3fe>|0x58e|+0x009:'330'
	// <0>
	// <0x5dd407>|0x597|+0x014:'332'
	// <0>
	// <0x5dd41b>|0x5ab|+0x007:'334'
	// <0>
	// <0x5dd422>|0x5b2|+0x014:'336'
	// <0>
	// <0x5dd436>|0x5c6|+0x016:'338'
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
	// <0x5dd44c>|0x5dc|+0x0e8:'349'
	// ******
}

// STATE[STUB]
void profile_skin_visual_cook::on_visual_loaded(
	resources::queries_result&				data,
	resources::query_result_for_cook*		parent,
	render::skeleton_combined_cook_data*	cook_data
)
{
	// FUNCTION BODY[0x5dcc90]: 5
	// <0>
	// <1>
	// <0x5dcc93>|0x003|+0x037:'358'
	// <0x5dccca>|0x03a|+0x00b:'359'
	// <0x5dccd5>|0x045|+0x022:'360'
	// ******
}

// STATE[STUB]
void profile_skin_visual_cook::delete_resource( resources::resource_base* __formal )
{
	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5cbb50 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN3") })
	// ******

	// FUNCTION BODY[0x5dcb50]
	// <0x5dcb50>|0x000|      :'364'	{
	// ******
}

} // namespace survarium
