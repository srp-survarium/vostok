////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "key_binder.h"
#include "game.h"
#include "game_action_descr.h"
#include "keyboard_key_descr.h"
#include <vostok/console_command.h>
#include <vostok/input/world.h>
#include <vostok/input/keyboard.h>
#include <vostok/input/mouse.h>

namespace survarium {

// TU-local key-name table (lineage-preserved from the legacy module); the
// ctor wires it into the per-action bindings (a matcher recovers the actions[]
// side - reshaped to the canonical 5-field game_action_descr - when enabled)
keyboard_key_descr keyboards[] = {
	{ "kESCAPE",		input::key_escape		},	{ "k1",				input::key_1			},
	{ "k2",				input::key_2			},	{ "k3",				input::key_3			},
	{ "k4",				input::key_4			},	{ "k5",				input::key_5			},
	{ "k6",				input::key_6			},	{ "k7",				input::key_7			},
	{ "k8",				input::key_8			},	{ "k9",				input::key_9			},
	{ "k0",				input::key_0			},	{ "kMINUS",			input::key_minus		},
	{ "kEQUALS",		input::key_equals		},	{ "kBACK",			input::key_back			},
	{ "kTAB",			input::key_tab			},	{ "kQ",				input::key_q			},
	{ "kW",				input::key_w			},	{ "kE",				input::key_e			},
	{ "kR",				input::key_r			},	{ "kT",				input::key_t			},
	{ "kY",				input::key_y			},	{ "kU",				input::key_u			},
	{ "kI",				input::key_i			},	{ "kO",				input::key_o			},
	{ "kP",				input::key_p			},	{ "kLBRACKET",		input::key_lbracket		},
	{ "kRBRACKET",		input::key_rbracket		},	{ "kRETURN",		input::key_return		},
	{ "kLCONTROL",		input::key_lcontrol		},	{ "kA",				input::key_a			},
	{ "kS",				input::key_s			},	{ "kD",				input::key_d			},
	{ "kF",				input::key_f			},	{ "kG",				input::key_g			},
	{ "kH",				input::key_h			},	{ "kJ",				input::key_j			},
	{ "kK",				input::key_k			},	{ "kL",				input::key_l			},
	{ "kSEMICOLON",		input::key_semicolon	},	{ "kAPOSTROPHE",	input::key_apostrophe	},
	{ "kGRAVE",			input::key_grave		},	{ "kLSHIFT",		input::key_lshift		},
	{ "kBACKSLASH",		input::key_backslash	},	{ "kZ",				input::key_z			},
	{ "kX",				input::key_x			},	{ "kC",				input::key_c			},
	{ "kV",				input::key_v			},	{ "kB",				input::key_b			},
	{ "kN",				input::key_n			},	{ "kM",				input::key_m			},
	{ "kCOMMA",			input::key_comma		},	{ "kPERIOD",		input::key_period		},
	{ "kSLASH",			input::key_slash		},	{ "kRSHIFT",		input::key_rshift		},
	{ "kMULTIPLY",		input::key_multiply		},	{ "kLMENU",			input::key_lmenu		},
	{ "kSPACE",			input::key_space		},	{ "kCAPITAL",		input::key_capital		},
	{ "kF1",			input::key_f1			},	{ "kF2",			input::key_f2			},
	{ "kF3",			input::key_f3			},	{ "kF4",			input::key_f4			},
	{ "kF5",			input::key_f5			},	{ "kF6",			input::key_f6			},
	{ "kF7",			input::key_f7			},	{ "kF8",			input::key_f8			},
	{ "kF9",			input::key_f9			},	{ "kF10",			input::key_f10			},
	{ "kNUMLOCK",		input::key_numlock		},	{ "kSCROLL",		input::key_scroll		},
	{ "kNUMPAD7",		input::key_numpad7		},	{ "kNUMPAD8",		input::key_numpad8		},
	{ "kNUMPAD9",		input::key_numpad9		},	{ "kSUBTRACT",		input::key_subtract		},
	{ "kNUMPAD4",		input::key_numpad4		},	{ "kNUMPAD5",		input::key_numpad5		},
	{ "kNUMPAD6",		input::key_numpad6		},	{ "kADD",			input::key_add			},
	{ "kNUMPAD1",		input::key_numpad1		},	{ "kNUMPAD2",		input::key_numpad2		},
	{ "kNUMPAD3",		input::key_numpad3		},	{ "kNUMPAD0",		input::key_numpad0		},
	{ "kDECIMAL",		input::key_decimal		},	{ "kF11",			input::key_f11			},
	{ "kF12",			input::key_f12			},	{ "kF13",			input::key_f13			},
	{ "kF14",			input::key_f14			},	{ "kF15",			input::key_f15			},
	{ "kKANA",			input::key_kana			},	{ "kCONVERT",		input::key_convert		},
	{ "kNOCONVERT",		input::key_noconvert	},	{ "kYEN",			input::key_yen			},
	{ "kNUMPADEQUALS",	input::key_numpadequals	},	{ "kCIRCUMFLEX",	input::key_circumflex	},
	{ "kAT",			input::key_at			},	{ "kCOLON",			input::key_colon		},
	{ "kUNDERLINE",		input::key_underline	},	{ "kKANJI",			input::key_kanji		},
	{ "kSTOP",			input::key_stop			},	{ "kAX",			input::key_ax			},
	{ "kUNLABELED",		input::key_unlabeled	},	{ "kNUMPADENTER",	input::key_numpadenter	},
	{ "kRCONTROL",		input::key_rcontrol		},	{ "kNUMPADCOMMA",	input::key_numpadcomma	},
	{ "kDIVIDE",		input::key_divide		},	{ "kSYSRQ",			input::key_sysrq		},
	{ "kRMENU",			input::key_rmenu		},	{ "kHOME",			input::key_home			},
	{ "kUP",			input::key_up			},	{ "kPRIOR",			input::key_prior		},
	{ "kLEFT",			input::key_left			},	{ "kRIGHT",			input::key_right		},
	{ "kEND",			input::key_end			},	{ "kDOWN",			input::key_down			},
	{ "kNEXT",			input::key_next			},	{ "kINSERT",		input::key_insert		},
	{ "kDELETE",		input::key_delete		},	{ "kLWIN",			input::key_lwin			},
	{ "kRWIN",			input::key_rwin			},	{ "kAPPS",			input::key_apps			},
	{ "kPAUSE",			input::key_pause		},
	{ "mouse1",			input::mouse_button_left		},
	{ "mouse2",			input::mouse_button_right		},
	{ "mouse3",			input::mouse_button_middle		},
	{ "mouse4",			input::mouse_button_extended0	},
	{ "mouse5",			input::mouse_button_extended1	},
	{ "mouse6",			input::mouse_button_extended2	},
	{ "mouse7",			input::mouse_button_extended3	},
	{ "mouse8",			input::mouse_button_extended4	},
	{ NULL,				0						}
};

// TU-local (canonical headers/console_command_bind.h; owner mapping in
// temp/triage_log.md) - the type of the ctor's s_bind_*_command statics
class console_command_bind : public console_commands::cc_delegate {
public:
					console_command_bind	( key_binder* binder, s32 type );

	virtual	void	save_to					( console_commands::save_storage& f, memory::base_allocator* a ) const override;

	virtual			~console_command_bind	( ) { /* no source */ }

private:
	/* 0x0000 */	/* console_commands::cc_delegate */
	/* 0x0060 */	s32				m_type;
	/* 0x0064 */	key_binder*		m_binder;
}; // class console_command_bind

STATIC_SIZE_ASSERT(console_command_bind, 0x68);

// STATE[STUB]
 console_command_bind::console_command_bind( key_binder* binder, s32 type )
	: console_commands::cc_delegate(
		( type == 0 ) ? "bind" : "bind_sec",
		boost::bind( &key_binder::bind_key, binder, _1, type ),
		true
	),
	m_type		( type ),
	m_binder	( binder )
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

// TU static 'set_mouse_sensitivity_cc' (compiler-generated dynamic
// initializer + atexit destructor); a matcher recovers its type/initializer
// from the asm.
/*
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
*/

// STATE[STUB]
 key_binder::key_binder( game& g )
	: m_game( g ) // buildability: ref member must be init'd
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
	s32			idx = 0;
	string128	buff;
	while ( keyboards[idx].key_name ) {
		buff[0]						= 0;
		keyboard_key_descr&	kb		= keyboards[idx];
		bool res					= m_game.input_world().get_keyboard()->get_dik_name( kb.dik, buff, sizeof( buff ) );
		if ( res )
			strings::copy	( kb.key_local_name, buff );
		else
			strings::copy	( kb.key_local_name, kb.key_name );
		++idx;
	}

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
	keyboard_key_descr* kb = key_binder::dik_to_ptr( _dik, true );
	if ( kb )
		return kb->key_name;
	else
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
	s32 idx = 0;
	while ( keyboards[idx].key_name ) {
		keyboard_key_descr& kb = keyboards[idx];
		if ( kb.dik == _dik )
			return &keyboards[idx];
		++idx;
	}
	if ( !bSafe )
		LOG_INFO	( "! cant find corresponding [keyboard_key_descr] for dik" );
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
	s32 idx = 0;
	while ( keyboards[idx].key_name ) {
		keyboard_key_descr& kb = keyboards[idx];
		if ( !_stricmp( _name, kb.key_name ) )
			return &keyboards[idx];
		++idx;
	}

	LOG_INFO	( "! cant find corresponding [keyboard_key_descr*] for keyname %s", _name );
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
	// claude@NOTE: buildability return for the still-STUB body (free_fly_camera
	// forces this to codegen under LTCG). Sourced through volatiles so LTCG cannot
	// constant-fold the value into free_fly_camera::on_keyboard_action's switch.
	// A matcher recovers the real lookup.
	static volatile s32 s_action = kNOTBINDED;
	static volatile s32 s_mask	= hold_action;
	actions_mask_type = (toggle_action_enum)s_mask;
	return (game_action_id)s_action;

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

} // namespace survarium
