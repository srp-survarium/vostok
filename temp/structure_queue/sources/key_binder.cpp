////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\game\sources\key_binder.h"

namespace survarium {

// STATE[STUB]
 console_command_bind::console_command_bind( key_binder* binder, s32 type )
{
	// FUNCTION BODY[0x91980]: 1
	// <0x91980>|0x000|+0x0ab:'32'	{
	// <0>
	// <0x91a2b>|0x0ab|      :'34'	}
	// ******
}

// STATE[STUB]
void console_command_bind::save_to( console_commands::save_storage& f, memory::base_allocator* a ) const
{
	// LOCALS
	// strings::detail::tuples 			STR_JOINA_tuples_unique_identifier
	// ******

	// FUNCTION BODY[0x91a40]: 11
	// <0>
	// <0x91a4b>|0x00b|+0x002:'42'
	// <0>
	// <0x91a4d>|0x00d|+0x003:'44'
	// <0x91a50>|0x010|+0x017:'45'
	// <0>
	// <1>
	// <0x91a67>|0x027|+0x042:'48'
	// <0x91aa9>|0x069|+0x014:'49'
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
void `dynamic initializer for 'set_mouse_sensitivity_cc''( )
{
	// FUNCTION BODY[0x7d8400]
	// <0x7d8400>|0x000|      :'218'	{
	// ******
}

// STATE[STUB]
void `dynamic atexit destructor for 'set_mouse_sensitivity_cc''( )
{
	// FUNCTION BODY[0x7f01d0]
	// <0x7d8470>|0x000|      :'220'	{
	// ******
}

// STATE[STUB]
explicit key_binder::key_binder( game& g )
{
	// STATICS
	// static console_commands::cc_delegate s_unbind_key_command = <0x4c2b208>;
	// static console_command_bind 		s_bind_key_command = <0x4c2b2d0>;
	// static console_command_bind 		s_bind_sec_key_command = <0x4c2b268>;
	// static console_commands::cc_delegate s_unbind_second_key_command = <0x4c2b1a8>;
	// ******

	// FUNCTION BODY[0x5db6e0]: 16
	// <0x5db6e5>|0x005|+0x017:'225'
	// <0>
	// <1>
	// <0x5db6fc>|0x01c|+0x005:'228'
	// <0>
	// <1>
	// <0x5db701>|0x021|+0x013:'231'
	// <0>
	// <1>
	// <0x5db714>|0x034|+0x02c:'234'
	// <0x5db740>|0x060|+0x029:'235'
	// <0>
	// <0x5db769>|0x089|+0x153:'237'
	// <0x5db8bc>|0x1dc|+0x153:'238'
	// <0>
	// <0x5dba0f>|0x32f|+0x006:'240'
	// ******
}

// STATE[STUB]
void key_binder::set_default_controls( )
{
	// LOCALS
	// strings::detail::tuples 			STR_JOINA_tuples_unique_identifier
	// ******

	// FUNCTION BODY[0x5db670]: 10
	// <0>
	// <0x5db679>|0x009|+0x002:'246'
	// <0>
	// <0x5db67b>|0x00b|+0x00a:'248'
	// <0>
	// <1>
	// <0x5db685>|0x015|+0x035:'251'
	// <0x5db6ba>|0x04a|+0x016:'252'
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
void key_binder::remap_keys( )
{
	// LOCALS
	// char[128] 						buff
	// ******

	// CALL SITE INFO
	// <0x5db020> -> input::world& < unknown >()
	// <0x5db029> -> input::keyboard const* < unknown >()
	// <0x5db040> -> bool < unknown >( int, char*, int ) const
	// ******

	// FUNCTION BODY[0x5daff0]: 10
	// <0x5dafff>|0x00f|+0x002:'259'
	// <0>
	// <0x5db001>|0x011|+0x00f:'261'
	// <0>
	// <1>
	// <2>
	// <0x5db010>|0x020|+0x032:'265'
	// <0x5db042>|0x052|+0x019:'266'
	// <0x5db05b>|0x06b|+0x01a:'267'
	// <0>
	// ******
}

// STATE[STUB]
pcstr key_binder::id_to_action_name( game_action_id _id ) const
{
	return NULL;

	// FUNCTION BODY[0x5db350]: 8
	// <0x5db350>|0x000|+0x00c:'272'	{
	// <0>
	// <0x5db35c>|0x00c|+0x009:'274'
	// <0>
	// <0x5db365>|0x015|+0x06c:'276'
	// <0x5db3d1>|0x081|-0x059:'277'
	// <0>
	// <0x5db378>|0x028|+0x069:'279'
	// <0x5db3e1>|0x091|-0x006:'279'
	// <0>
	// <0x5db3db>|0x08b|+0x06c:'281'
	// <0x5db447>|0x0f7|      :'281'	}
	// ******
}

// STATE[STUB]
game_action_id key_binder::action_name_to_id( pcstr _name )
{
	// FUNCTION BODY[0x5db330]: 5
	// <0x5db330>|0x000|+0x000:'284'	{
	// <0x5db330>|0x000|+0x006:'285'
	// <0x5db336>|0x006|+0x004:'286'
	// <0x5db33a>|0x00a|+0x004:'287'
	// <0>
	// <0x5db33e>|0x00e|-0x001:'289'
	// <0x5db33d>|0x00d|+0x006:'290'
	// <0x5db343>|0x013|      :'290'	}
	// ******
}

// STATE[STUB]
game_action_descr* key_binder::action_name_to_ptr( pcstr _name )
{
	return NULL;

	// FUNCTION BODY[0x5db210]: 8
	// <0x5db210>|0x000|+0x00e:'293'	{
	// <0>
	// <0x5db21e>|0x00e|+0x008:'295'
	// <0>
	// <0x5db226>|0x016|+0x07e:'297'
	// <0x5db2a4>|0x094|-0x05b:'298'
	// <0>
	// <0x5db249>|0x039|+0x06e:'300'
	// <0x5db2b7>|0x0a7|-0x009:'300'
	// <0>
	// <0x5db2ae>|0x09e|+0x073:'302'
	// <0x5db321>|0x111|      :'302'	}
	// ******
}

// STATE[STUB]
pcstr key_binder::dik_to_keyname( s32 _dik )
{
	return NULL;

	// FUNCTION BODY[0x5db1f0]: 5
	// <0x5db1f0>|0x000|+0x001:'305'	{
	// <0x5db1f1>|0x001|+0x005:'306'
	// <0x5db1f6>|0x006|+0x004:'307'
	// <0x5db1fa>|0x00a|+0x004:'308'
	// <0>
	// <0x5db1fe>|0x00e|-0x001:'310'
	// <0x5db1fd>|0x00d|+0x004:'311'
	// <0x5db201>|0x011|      :'311'	}
	// ******
}

// STATE[STUB]
keyboard_key_descr* key_binder::dik_to_ptr( s32 _dik, bool bSafe )
{
	return NULL;

	// FUNCTION BODY[0x5db1b0]: 11
	// <0x5db1b0>|0x000|+0x001:'314'	{
	// <0x5db1b1>|0x001|+0x009:'315'
	// <0x5db1ba>|0x00a|+0x00a:'316'
	// <0>
	// <1>
	// <0x5db1c4>|0x014|+0x01e:'319'
	// <0x5db1e2>|0x032|-0x016:'320'
	// <0x5db1cc>|0x01c|+0x012:'321'
	// <0>
	// <1>
	// <2>
	// <0x5db1de>|0x02e|+0x002:'325'
	// <0x5db1e0>|0x030|+0x00d:'326'
	// <0x5db1ed>|0x03d|      :'326'	}
	// ******
}

// STATE[STUB]
keyboard_key_descr* key_binder::keyname_to_ptr( pcstr _name )
{
	return NULL;

	// FUNCTION BODY[0x5db080]: 11
	// <0x5db080>|0x000|+0x007:'335'	{
	// <0x5db087>|0x007|+0x006:'336'
	// <0x5db08d>|0x00d|+0x013:'337'
	// <0>
	// <1>
	// <0x5db0a0>|0x020|+0x080:'340'
	// <0x5db120>|0x0a0|-0x070:'341'
	// <0x5db0b0>|0x030|+0x017:'342'
	// <0>
	// <1>
	// <0x5db0c7>|0x047|+0x06c:'345'
	// <0x5db133>|0x0b3|-0x006:'345'
	// <0>
	// <0x5db12d>|0x0ad|+0x06c:'347'
	// <0x5db199>|0x119|      :'347'	}
	// ******
}

// STATE[STUB]
s32 key_binder::get_action_dik( game_action_id _action_id, s32 idx )
{
	return 0;

	// FUNCTION BODY[0x5dafd0]: 15
	// <0x5dafd0>|0x000|+0x000:'367'	{
	// <0x5dafd0>|0x000|+0x006:'368'
	// <0>
	// <1>
	// <2>
	// <0x5dafd6>|0x006|+0x007:'372'
	// <0x5dafdd>|0x00d|+0x004:'373'
	// <0>
	// <0x5dafe1>|0x011|+0x007:'375'
	// <0x5dafe8>|0x018|+0x004:'376'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5dafec>|0x01c|-0x00c:'382'
	// <0x5dafe0>|0x010|+0x00b:'383'
	// <0x5dafeb>|0x01b|+0x003:'383'
	// <0x5dafee>|0x01e|      :'383'	}
	// ******
}

// STATE[STUB]
game_action_id key_binder::get_binded_action( s32 _dik, toggle_action_enum& actions_mask_type, s32 key_group_mask ) const
{
	// FUNCTION BODY[0x5daf60]: 20
	// <0x5daf60>|0x000|+0x005:'386'	{
	// <0x5daf65>|0x005|+0x02e:'387'
	// <0x5daf93>|0x033|-0x023:'387'
	// <0>
	// <1>
	// <0x5daf70>|0x010|+0x00a:'390'
	// <0>
	// <0x5daf7a>|0x01a|+0x002:'392'
	// <0>
	// <1>
	// <0x5daf7c>|0x01c|+0x029:'395'
	// <0x5dafa5>|0x045|+0x006:'396'
	// <0x5dafab>|0x04b|-0x023:'397'
	// <0>
	// <1>
	// <0x5daf88>|0x028|+0x02c:'400'
	// <0x5dafb4>|0x054|+0x006:'401'
	// <0x5dafba>|0x05a|-0x01e:'402'
	// <0>
	// <1>
	// <2>
	// <0x5daf9c>|0x03c|+0x006:'406'
	// <0x5dafa2>|0x042|+0x00f:'407'
	// <0x5dafb1>|0x051|+0x00f:'407'
	// <0x5dafc0>|0x060|      :'407'	}
	// ******
}

// STATE[STUB]
void key_binder::bind_key( pcstr args, s32 bind_number )
{
	// LOCALS
	// char[256] 						action
	// char[256] 						key
	// ******

	// FUNCTION BODY[0x5db4a0]: 47
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5db4ac>|0x00c|+0x03a:'444'
	// <0x5db4e6>|0x046|+0x00b:'445'
	// <0>
	// <1>
	// <0x5db4f1>|0x051|+0x00e:'448'
	// <0>
	// <1>
	// <0x5db4ff>|0x05f|+0x009:'451'
	// <0x5db508>|0x068|+0x006:'452'
	// <0x5db50e>|0x06e|+0x00a:'453'
	// <0>
	// <1>
	// <0x5db518>|0x078|+0x012:'456'
	// <0>
	// <1>
	// <0x5db52a>|0x08a|+0x015:'459'
	// <0x5db53f>|0x09f|+0x009:'460'
	// <0>
	// <1>
	// <0x5db548>|0x0a8|+0x00c:'463'
	// <0x5db554>|0x0b4|+0x0f6:'464'
	// <0>
	// <1>
	// <0x5db64a>|0x1aa|-0x0ec:'467'
	// <0>
	// <0x5db55e>|0x0be|+0x0ef:'469'
	// <0>
	// <1>
	// <0x5db64d>|0x1ad|-0x0d7:'472'
	// <0>
	// <1>
	// <0x5db576>|0x0d6|+0x036:'475'
	// <0x5db5ac>|0x10c|+0x034:'475'
	// <0x5db5e0>|0x140|+0x035:'475'
	// <0x5db615>|0x175|-0x098:'475'
	// <0>
	// <0x5db57d>|0x0dd|+0x036:'477'
	// <0x5db5b3>|0x113|+0x034:'477'
	// <0x5db5e7>|0x147|+0x035:'477'
	// <0x5db61c>|0x17c|-0x088:'477'
	// <0>
	// <0x5db594>|0x0f4|+0x036:'479'
	// <0x5db5ca>|0x12a|+0x033:'479'
	// <0x5db5fd>|0x15d|+0x035:'479'
	// <0x5db632>|0x192|-0x095:'479'
	// <0x5db59d>|0x0fd|+0x036:'480'
	// <0x5db5d3>|0x133|+0x033:'480'
	// <0x5db606>|0x166|+0x035:'480'
	// <0x5db63b>|0x19b|-0x09b:'480'
	// <0>
	// <0x5db5a0>|0x100|+0x036:'482'
	// <0x5db5d6>|0x136|+0x033:'482'
	// <0x5db609>|0x169|+0x035:'482'
	// <0x5db63e>|0x19e|-0x095:'482'
	// <0x5db5a9>|0x109|+0x035:'483'
	// <0x5db5de>|0x13e|+0x034:'483'
	// <0x5db612>|0x172|+0x035:'483'
	// <0x5db647>|0x1a7|+0x011:'483'
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
void key_binder::unbind_key( pcstr args, s32 bind_number )
{
	// FUNCTION BODY[0x5db450]: 3
	// <0x5db450>|0x000|+0x000:'489'	{
	// <0x5db450>|0x000|+0x014:'490'
	// <0>
	// <0x5db464>|0x014|+0x015:'492'
	// <0x5db479>|0x029|-0x004:'492'
	// <0x5db475>|0x025|+0x01a:'493'
	// <0x5db48f>|0x03f|      :'493'	}
	// ******
}

// STATE[STUB]
s32 key_binder::get_binding_group( game_action_id _id )
{
	return 0;

	// FUNCTION BODY[0x5daf50]: 3
	// <0x5daf50>|0x000|+0x003:'497'
	// <0>
	// <0x5daf53>|0x003|+0x006:'499'
	// ******
}

	// TYPEDEFS
	// typedef
	// 	survarium::base_project::resolve_link_object*
	// 	iterator_type;

	// typedef
	// 	survarium::scheduler::record*
	// 	iterator_type;

	// typedef
	// 	vostok::collision::bone_collision_data const*
	// 	iterator_type;

	// typedef
	// 	vostok::collision::bone_collision_data*
	// 	iterator_type;

	// ******

} // namespace survarium
