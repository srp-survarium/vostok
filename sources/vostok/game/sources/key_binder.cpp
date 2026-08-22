////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "key_binder.h"
#include "game.h"
#include "game_action_descr.h"
#include "keyboard_key_descr.h"
#include <vostok/console_command.h>
#include <vostok/strings_concatenations.h>
#include <vostok/input/world.h>
#include <vostok/input/keyboard.h>
#include <vostok/input/mouse.h>

namespace survarium {

enum keyboard_key_group
{
	game_world_group	= 0x01,
	lobby_group			= 0x02,
	weapon_aim_group	= 0x04,
	chat_group			= 0x08,
	player_dead_group	= 0x10,
};

// TU-local console-command type (canonical headers/console_command_bind.h); the
// ctor wires the per-binder s_bind_*_command statics through it.
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

console_command_bind::console_command_bind( key_binder* binder, s32 type )
	: console_commands::cc_delegate(
		( type == 0 ) ? "bind" : "bind_sec",
		boost::bind( &key_binder::bind_key, binder, _1, type ),
		true
	),
	m_type		( type ),
	m_binder	( binder )
{
}

void console_command_bind::save_to( console_commands::save_storage& f, memory::base_allocator* a ) const
{
	VOSTOK_UNREFERENCED_PARAMETER( a );
	for ( s32 idx = 0; idx < 64; ++idx ) {
		key_binding* binding	= &m_binder->m_key_bindings[idx];
		if ( binding->m_keyboard[m_type] == NULL )
			continue;
		if ( binding->m_keyboard[m_type]->key_name == NULL )
			continue;
		pcstr out_str	= NULL;
		STR_JOINA		( out_str, m_name, " ", binding->m_action->action_name, " ", binding->m_keyboard[m_type]->key_name );
		f.add_line		( out_str );
	}
}

// This TU owns the namespace-scope mouse-input globals; recovered defaults from
// .data/.bss: g_mouse_sensitivity = 1.0f (VA 0x9c8950, raw 0000803f),
// g_mouse_invert = false (zero-init .bss). Consumed by lobby_camera::on_mouse_move
// and player_input_handler::on_mouse_move / process_first_person_mode (parked on
// this definition's absence until now).
float	g_mouse_sensitivity	= 1.0f;
bool	g_mouse_invert		= false;

// claude@NOTE: PARKED - file-scope mouse console-command statics
// (set_mouse_sensitivity_cc / set_mouse_invert_cc) and their compiler-emitted
// dynamic init + atexit. The cc_float dynamic init proves min=0.01f
// (__real@3c23d70a), max=10.0f (the shared .rdata constant the delinker labels
// c_fUncompressWindScalar), value=&g_mouse_sensitivity, serializable=true; the
// cc_bool init proves value=&g_mouse_invert, serializable=true. The command-NAME
// string for each does not appear in either init body (it is stored by the
// cc_value/console_command base ctor which the linker placed out of the init),
// so the registered name is unrecoverable from this TU's asm alone. Reconstruct
// once the name strings surface (a sibling consumer or the cc_value base ctor).
// The two GLOBALS above are independently faithful and unblock the mouse-input
// consumers without the cc statics.

// TU-local action table; the ctor populates per-action m_keyboard slots through it.
game_action_descr	actions[] = {
	{ "left",						kLEFT,						game_world_group | weapon_aim_group,	hold_action,	"kLEFT"		},
	{ "right",						kRIGHT,						game_world_group | weapon_aim_group,	hold_action,	"kRIGHT"	},
	{ "up",							kUP,						game_world_group | weapon_aim_group,	hold_action,	"kUP"		},
	{ "down",						kDOWN,						game_world_group | weapon_aim_group,	hold_action,	"kDOWN"		},
	{ "jump",						kJUMP,						game_world_group | weapon_aim_group,	hold_action,	"kSPACE"	},
	{ "crouch",						kCROUCH,					game_world_group | weapon_aim_group,	hold_action,	"kLCONTROL"	},
	{ "accel",						kACCEL,						game_world_group,	hold_action,	"kLSHIFT"	},
	{ "sprint_toggle",				kSPRINT_TOGGLE,				game_world_group | weapon_aim_group,	toggle_action,	"kX"		},
	{ "set_next_fire_queue_type",	kSET_NEXT_FIRE_QUEUE_TYPE,	game_world_group | weapon_aim_group,	toggle_action,	"kN"		},
	{ "set_next_ammo_type",			kSET_NEXT_AMMO_TYPE,		game_world_group | weapon_aim_group,	toggle_action,	"kM"		},
	{ "forward",					kFWD,						game_world_group | weapon_aim_group,	hold_action,	"kW"		},
	{ "back",						kBACK,						game_world_group | weapon_aim_group,	hold_action,	"kS"		},
	{ "lstrafe",					kL_STRAFE,					game_world_group | weapon_aim_group,	hold_action,	"kA"		},
	{ "rstrafe",					kR_STRAFE,					game_world_group | weapon_aim_group,	hold_action,	"kD"		},
	{ "llookout",					kL_LOOKOUT,					game_world_group | weapon_aim_group,	hold_action,	"kQ"		},
	{ "rlookout",					kR_LOOKOUT,					game_world_group | weapon_aim_group,	hold_action,	"kE"		},
	{ "cam_1",						kCAM_1,						game_world_group | weapon_aim_group,	toggle_action,	"kF1"		},
	{ "torch",						kTORCH,						game_world_group | weapon_aim_group,	toggle_action,	NULL		},
	{ "night_vision",				kNIGHT_VISION,				game_world_group | weapon_aim_group,	toggle_action,	NULL		},
	{ "show_detector",				kDETECTOR,					game_world_group | weapon_aim_group,	toggle_action,	NULL		},
	{ "wpn_1",						kWPN_1,						game_world_group | weapon_aim_group,	toggle_action,	"k1"		},
	{ "wpn_2",						kWPN_2,						game_world_group | weapon_aim_group,	toggle_action,	"k2"		},
	{ "missile_wpn",				kMISSILE_WPN,				game_world_group | weapon_aim_group,	hold_action,	"kG"		},
	{ "artefact",					kARTEFACT,					game_world_group | weapon_aim_group,	toggle_action,	NULL		},
	{ "wpn_next",					kWPN_NEXT,					game_world_group | weapon_aim_group,	toggle_action,	NULL		},
	{ "wpn_fire",					kWPN_FIRE,					game_world_group | weapon_aim_group,	hold_action,	"mouse1"	},
	{ "wpn_reload",					kWPN_RELOAD,				game_world_group | weapon_aim_group,	hold_action,	"kR"		},
	{ "wpn_aim",					kWPN_AIM,					game_world_group | weapon_aim_group,	hold_action,	"mouse2"	},
	{ "delay_breath",				kDELAY_BREATH,				weapon_aim_group,	hold_action,	"kLSHIFT"	},
	{ "pause",						kPAUSE,						game_world_group | weapon_aim_group,	toggle_action,	"kPAUSE"	},
	{ "serialize_player_state",		kSERIALIZE_PLAYER_STATE,	game_world_group | weapon_aim_group,	toggle_action,	"kO"		},
	{ "deserialize_player_state",	kDESERIALIZE_PLAYER_STATE,	game_world_group | weapon_aim_group,	toggle_action,	"kP"		},
	{ "quick_use_1",				kQUICK_USE_1,				game_world_group | weapon_aim_group,	hold_action,	"k3"		},
	{ "quick_use_2",				kQUICK_USE_2,				game_world_group | weapon_aim_group,	hold_action,	"k4"		},
	{ "quick_use_3",				kQUICK_USE_3,				game_world_group | weapon_aim_group,	hold_action,	"k5"		},
	{ "quick_use_4",				kQUICK_USE_4,				game_world_group | weapon_aim_group,	hold_action,	"k6"		},
	{ "quick_use_5",				kQUICK_USE_5,				game_world_group | weapon_aim_group,	hold_action,	"k7"		},
	{ "quick_use_6",				kQUICK_USE_6,				game_world_group | weapon_aim_group,	hold_action,	"k8"		},
	{ "back_slot_use",				kBACK_SLOT_USE,				game_world_group | weapon_aim_group,	toggle_action,	"k9"		},
	{ "chat",						kCHAT,						game_world_group | weapon_aim_group,	toggle_action,	"kT"		},
	{ "use",						kUSE,						game_world_group | weapon_aim_group,	hold_action,	"kE"		},
	{ "drop",						kDROP,						game_world_group | weapon_aim_group,	hold_action,	"kV"		},
	{ "character",					kCHARACTER,					lobby_group,	hold_action,	"kC"		},
	{ "inventory",					kINVENTORY,					lobby_group,	hold_action,	"kI"		},
	{ "shop",						kSHOP,						lobby_group,	hold_action,	"kS"		},
	{ "options",					kOPTIONS,					lobby_group,	hold_action,	"kO"		},
	{ "friends",					kFRIENDS,					lobby_group,	hold_action,	"kF"		},
	{ "send_message",				kSEND_MESSAGE,				chat_group,	hold_action,	"kRETURN"	},
	{ "send_to",					kSELECT_SEND_TO,			chat_group,	hold_action,	"kTAB"		},
	{ "ptt",						kPTT,						chat_group,	hold_action,	"kP"		},
};

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
	{ "kO",				input::key_o			},	{ "kP",				input::key_p			},
	{ "kN",				input::key_n			},	{ "kM",				input::key_m			},
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

s32 const bindings_count = 50;

int bRemapped = FALSE;

key_binder::key_binder( game& g )
	: m_game( g )
{
	memset( m_key_bindings, 0, sizeof( m_key_bindings ) );

	for ( game_action_descr* action = actions; action != actions + bindings_count; ++action )
	{
		m_key_bindings[action->id].m_action =
			action;
	}

	static console_command_bind s_bind_key_command( this, 0 );
	static console_command_bind s_bind_sec_key_command( this, 1 );

	static console_commands::cc_delegate s_unbind_key_command(
		"unbind",
		boost::bind( &key_binder::unbind_key, this, _1, 0 ),
		true
	);
	static console_commands::cc_delegate s_unbind_second_key_command(
		"unbind_sec",
		boost::bind( &key_binder::unbind_key, this, _1, 1 ),
		true
	);

	set_default_controls( );
}

void key_binder::set_default_controls( )
{
	for ( u32 idx = 0; idx < bindings_count; ++idx ) {
		if ( actions[idx].default_key ) {
			pcstr arg	= NULL;
			STR_JOINA	( arg, actions[idx].action_name, " ", actions[idx].default_key );
			bind_key	( arg, 0 );
		}
	}
}

void key_binder::remap_keys( )
{
	s32			idx = 0;
	string128	buff;
	while ( keyboards[idx].key_name ) {
		buff[0]						= 0;
		bool res					= m_game.input_world( ).get_keyboard( )->get_dik_name( keyboards[idx].dik, buff, sizeof( buff ) );
		strings::copy	( keyboards[idx].key_local_name, res ? buff : keyboards[idx].key_name );
		++idx;
	}
}

pcstr key_binder::id_to_action_name( game_action_id _id ) const
{
	for ( u32 idx = 0; idx < bindings_count; ++idx ) {
		if ( _id == actions[idx].id )
			return actions[idx].action_name;
	}
	LOG_INFO	( "can't find corresponding [action_name] for id" );

	return NULL;
}

game_action_id key_binder::action_name_to_id( pcstr _name )
{
	game_action_descr* descr = action_name_to_ptr( _name );
	if ( descr )
		return descr->id;
	else
		return kNOTBINDED;
}

game_action_descr* key_binder::action_name_to_ptr( pcstr _name )
{
	for ( u32 idx = 0; idx < bindings_count; ++idx ) {
		if ( !_stricmp( _name, actions[idx].action_name ) )
			return &actions[idx];
	}

	LOG_INFO	( "! cant find corresponding [id] for action_name", _name );

	return NULL;
}
pcstr key_binder::dik_to_keyname( s32 _dik )
{
	keyboard_key_descr* kb = key_binder::dik_to_ptr( _dik, true );
	if ( kb )
		return kb->key_name;
	else
		return NULL;
}

keyboard_key_descr* key_binder::dik_to_ptr( s32 _dik, bool bSafe )
{
	VOSTOK_UNREFERENCED_PARAMETER( bSafe );
	s32 idx = 0;
	while ( keyboards[idx].key_name ) {
		if ( keyboards[idx].dik == _dik )
			return &keyboards[idx];
		++idx;
	}

	return NULL;
}









keyboard_key_descr* key_binder::keyname_to_ptr( pcstr _name )
{
	s32 idx = 0;
	while ( keyboards[idx].key_name )
	{

		if ( !_stricmp( _name, keyboards[idx].key_name ) )
			return &keyboards[idx];
		++idx;
	}

	LOG_INFO	( "! cant find corresponding [keyboard_key_descr*] for keyname %s", _name );

	return NULL;
}

s32 key_binder::get_action_dik( game_action_id _action_id, s32 idx )
{
	VOSTOK_UNREFERENCED_PARAMETER( idx );
	key_binding* pbinding = &m_key_bindings[_action_id];

	if ( pbinding->m_keyboard[0] )
		return pbinding->m_keyboard[0]->dik;

	if ( pbinding->m_keyboard[1] )
		return pbinding->m_keyboard[1]->dik;

	return 0;
}

game_action_id key_binder::get_binded_action( s32 _dik, toggle_action_enum& actions_mask_type, s32 key_group_mask ) const
{
	for ( s32 idx = 0; idx < 64; ++idx )
	{
		key_binding const& binding	= m_key_bindings[idx];
		if ( !binding.m_action || !( binding.m_action->key_group & key_group_mask ) )
			continue;

		if ( binding.m_keyboard[0] && binding.m_keyboard[0]->dik == _dik )
		{
			actions_mask_type	= binding.m_action->actions_mask_type;
			return binding.m_action->id;
		}
		if ( binding.m_keyboard[1] && binding.m_keyboard[1]->dik == _dik )
		{
			actions_mask_type	= binding.m_action->actions_mask_type;
			return binding.m_action->id;
		}
	}
	return kNOTBINDED;
}

void key_binder::bind_key( pcstr args, s32 bind_number )
{
	char	action[256];
	char	key[256];
	*action								= 0;
	*key								= 0;

	sscanf_s							( args, "%255s %255s", action, sizeof( action ), key, sizeof( key ) );
	if ( !*action )
		return;

	if ( !*key )
		return;

	if ( !bRemapped ) {
		remap_keys	( );
		bRemapped	= TRUE;
	}

	if ( !action_name_to_ptr( action ) )
		return;

	s32 action_id						= action_name_to_id( action );
	if ( action_id == kNOTBINDED )
		return;

	keyboard_key_descr*	pkeyboard		= keyname_to_ptr( key );
	if ( !pkeyboard )
		return;

	key_binding*	curr_pbinding		= &m_key_bindings[action_id];

	curr_pbinding->m_keyboard[bind_number]	= pkeyboard;

	for ( s32 idx = 0; idx < 64; ++idx )
	{
		key_binding*	binding			= &m_key_bindings[idx];
		if ( binding == curr_pbinding )	continue;

		bool b_conflict = binding->m_action && ( binding->m_action->key_group & curr_pbinding->m_action->key_group );

		if ( binding->m_keyboard[0] == pkeyboard && b_conflict )
			binding->m_keyboard[0] = NULL;

		if ( binding->m_keyboard[1] == pkeyboard && b_conflict )
			binding->m_keyboard[1] = NULL;
	}
}

void key_binder::unbind_key( pcstr args, s32 bind_number )
{
	game_action_id	action_id	= action_name_to_id( args );
	m_key_bindings[action_id].m_keyboard[bind_number]	= NULL;
}

s32 key_binder::get_binding_group( game_action_id _id )
{
	return m_key_bindings[_id].m_action->key_group;
}

} // namespace survarium
