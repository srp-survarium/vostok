#include "pch.h"
#include <vostok/input/world.h>
#include <vostok/input/keyboard.h>
#include <vostok/input/mouse.h>
#include "key_binder.h"
#include "game.h"
#include <vostok/console_command.h>


using namespace vostok::input;

namespace survarium {

class console_command_bind: private vostok::console_commands::cc_delegate
{
public:
	// ctor HARVESTED to canonical key_binder.cpp (batch 5)
private:
	int m_type;
public:
	virtual void		save_to					( console_commands::save_storage& f, memory::base_allocator* a)	const
	{
		VOSTOK_UNREFERENCED_PARAMETERS	( a );
		for(int idx=0; idx<bindings_count; ++idx)
		{
			key_binding*	binding = &g_key_bindings[idx];	
			if ( binding->m_keyboard[m_type] == NULL )
				continue;
			pcstr out_str	= NULL;
			STR_JOINA		( out_str, m_name, " ", binding->m_action->action_name, " ", binding->m_keyboard[m_type]->key_name );
			f.add_line		( out_str );
			//	f.w_stringZ_CRLF( out_str );
		}
	};

};

key_binding	g_key_bindings		[bindings_count]; 
keyboard_key_group	g_current_keygroup	= _sp;

game_action_descr  actions[]		= {
	{ "left",				kLEFT					,_both, "kLEFT"},	
	{ "right",				kRIGHT					,_both, "kRIGHT"},	
	{ "up",					kUP						,_both, "kUP"},	
	{ "down",				kDOWN					,_both, "kDOWN"},	
	{ "jump",				kJUMP					,_both, "kSPACE"},	
	{ "crouch",				kCROUCH					,_both, "kC"},	
	{ "accel",				kACCEL					,_both, "kLSHIFT"},	
	{ "sprint_toggle",  	kSPRINT_TOGGLE  		,_both, "kX"},	
																
	{ "forward",			kFWD					,_both, "kW"},	
	{ "back",				kBACK					,_both, "kS"},	
	{ "lstrafe",			kL_STRAFE				,_both, "kA"},	
	{ "rstrafe",			kR_STRAFE				,_both, "kD"},	
 																
 	{ "llookout",			kL_LOOKOUT				,_both, "kQ" },	
 	{ "rlookout",			kR_LOOKOUT				,_both, "kE" },	
 																
 	{ "cam_1",				kCAM_1					,_both},	
 	{ "cam_2",				kCAM_2					,_both},	
 	{ "cam_3",				kCAM_3					,_both},	
 	{ "cam_zoom_in",		kCAM_ZOOM_IN			,_both},	
 	{ "cam_zoom_out",		kCAM_ZOOM_OUT			,_both},	
 															
 	{ "torch",				kTORCH					,_both},	
 	{ "night_vision",		kNIGHT_VISION			,_both},	
 	{ "show_detector",		kDETECTOR				,_sp},		
 
 	{ "wpn_1",				kWPN_1					,_both},	
 	{ "wpn_2",				kWPN_2					,_both},	
 	{ "wpn_3",				kWPN_3					,_both},	
 	{ "wpn_4",				kWPN_4					,_both},	
 	{ "wpn_5",				kWPN_5					,_both},	
 	{ "wpn_6",				kWPN_6					,_both},	
 	{ "artefact",			kARTEFACT				,_both/*_mp*/},		
 	{ "wpn_next",			kWPN_NEXT				,_both},	// means next ammo type
 	{ "wpn_fire",			kWPN_FIRE				,_both, "mouse1"},	
// 	{ "wpn_zoom",			kWPN_ZOOM				,_both},	
// 	{ "wpn_zoom_inc",		kWPN_ZOOM_INC			,_both},	
// 	{ "wpn_zoom_dec",		kWPN_ZOOM_DEC			,_both},	
// 	{ "wpn_reload",			kWPN_RELOAD				,_both},	
// 	{ "wpn_func",			kWPN_FUNC				,_both},	
// 	{ "wpn_firemode_prev",	kWPN_FIREMODE_PREV		,_both},	
// 	{ "wpn_firemode_next",	kWPN_FIREMODE_NEXT		,_both},	
// 															
// 	{ "pause",				kPAUSE					,_both},	
// 	{ "drop",				kDROP					,_both},	
// 	{ "use",				kUSE					,_both},	
// 	{ "scores",				kSCORES					,_both},	
// 	{ "chat",				kCHAT					,_mp},		
// 	{ "chat_team",			kCHAT_TEAM				,_mp},		
// 	{ "screenshot",			kSCREENSHOT				,_both},	
// 	{ "quit",				kQUIT					,_both},	
// 	{ "console",			kCONSOLE				,_both},	
// 	{ "inventory",			kINVENTORY				,_both},	
// 	{ "buy_menu",			kBUY					,_mp},		
// 	{ "skin_menu",			kSKIN					,_mp},		
// 	{ "team_menu",			kTEAM					,_mp},		
// 																
// 	{ "vote_begin",			kVOTE_BEGIN				,_mp},		
// 	{ "vote",				kVOTE					,_mp},		
// 	{ "vote_yes",			kVOTEYES				,_mp},		
// 	{ "vote_no",			kVOTENO					,_mp},		
// 																
// 	{ "next_slot",			kNEXT_SLOT				,_both},	
// 	{ "prev_slot",			kPREV_SLOT				,_both},	
// 															
// 	{ "speech_menu_0",		kSPEECH_MENU_0			,_mp},		
// 	{ "speech_menu_1",		kSPEECH_MENU_1			,_mp},		
// 																
// 	{ "quick_use_1",		kQUICK_USE_1			,_both},		
// 	{ "quick_use_2",		kQUICK_USE_2			,_both},
// 	{ "quick_use_3",		kQUICK_USE_3			,_both},
// 	{ "quick_use_4",		kQUICK_USE_4			,_both},

																
	{ NULL, 				kLASTACTION				,_both}		
};															

// keyboards[] table HARVESTED to canonical key_binder.cpp (batch 5)

key_binder::key_binder	( game& game ):m_game( game )
{
	for(int idx=0; idx<bindings_count; ++idx)
		g_key_bindings[idx].m_action = &actions[idx];
	
	static console_command_bind s_bind_key_command( this , 0); // conmmand bind
	static console_command_bind s_bind_sec_key_command( this , 1); // conmmand bind_sec
	
	static vostok::console_commands::cc_delegate s_unbind_key_command(
		"unbind",
		boost::bind( &key_binder::unbind_key, this, _1, 0 ),
		console_commands::command_type_user_specific
	);
	static vostok::console_commands::cc_delegate s_unbind_second_key_command(
		"unbind_sec",
		boost::bind( &key_binder::unbind_key, this, _1, 1 ),
		console_commands::command_type_user_specific
	);

	set_default_controls( );
}

void key_binder::set_default_controls( )
{
	int idx				= 0;
	while( actions[idx].action_name )
	{
		if ( actions[idx].default_key ){
			pcstr arg = NULL;
			STR_JOINA(arg, actions[idx].action_name, " ", actions[idx].default_key );
			bind_key( arg, 0 );
		}
		idx++;
	}
}

// remap_keys HARVESTED to canonical key_binder.cpp (batch 5)

pcstr key_binder::id_to_action_name(game_action_id _id)
{
	int idx				= 0;
	while( actions[idx].action_name )
	{
		if(_id==actions[idx].id )
			return actions[idx].action_name;
		++idx;
	}
	LOG_INFO				("! cant find corresponding [action_name] for id");
	return			NULL;
}

game_action_id key_binder::action_name_to_id(pcstr _name)
{
	game_action_descr* action = action_name_to_ptr(_name);
	if(action)
		return action->id;
	else
		return	kNOTBINDED;
}

game_action_descr* key_binder::action_name_to_ptr(pcstr _name)
{
	int idx				= 0;
	while( actions[idx].action_name )
	{
		if( !_stricmp(_name,actions[idx].action_name) )
			return &actions[idx];
		++idx;
	}
	LOG_INFO				("! cant find corresponding [id] for action_name", _name);
	return			NULL;
}

// dik_to_keyname + dik_to_ptr HARVESTED to canonical key_binder.cpp (batch 5)

int	key_binder::keyname_to_dik (LPCSTR _name)
{
	keyboard_key_descr* _kb = keyname_to_ptr(_name);
	return _kb->dik;
}

// keyname_to_ptr HARVESTED to canonical key_binder.cpp (batch 5)

bool is_group_not_conflicted(keyboard_key_group g1, keyboard_key_group g2)
{
	return ((g1==_sp && g2==_mp) || (g1==_mp && g2==_sp));
}

bool is_group_matching(keyboard_key_group g1, keyboard_key_group g2)
{
	return ( (g1==g2) || (g1==_both) || (g2==_both) );
}

bool key_binder::is_binded(game_action_id _action_id, int _dik)
{
	key_binding* pbinding = &g_key_bindings[_action_id];
	if(pbinding->m_keyboard[0] && pbinding->m_keyboard[0]->dik==_dik)
		return true;

	if(pbinding->m_keyboard[1] && pbinding->m_keyboard[1]->dik==_dik)
		return true;
	
	return false;
}

int key_binder::get_action_dik(game_action_id _action_id, int idx)
{
	key_binding* pbinding = &g_key_bindings[_action_id];
	
	if(idx==-1)
	{
	if(pbinding->m_keyboard[0])
		return pbinding->m_keyboard[0]->dik;

	if(pbinding->m_keyboard[1])
		return pbinding->m_keyboard[1]->dik;
	}else
	{
	if(pbinding->m_keyboard[idx])
		return pbinding->m_keyboard[idx]->dik;
	}
	return 0;
}

game_action_id key_binder::get_binded_action(int _dik)
{
	for(int idx=0; idx<bindings_count; ++idx)
	{
		key_binding*	binding = &g_key_bindings[idx];

		bool b_is_group_matching	= is_group_matching(binding->m_action->key_group,g_current_keygroup);
		
		if(!b_is_group_matching)	continue;

		if(binding->m_keyboard[0] && binding->m_keyboard[0]->dik==_dik && b_is_group_matching)
			return binding->m_action->id;
		
		if(binding->m_keyboard[1] && binding->m_keyboard[1]->dik==_dik && b_is_group_matching)
			return binding->m_action->id;
	}
	return kNOTBINDED;
}

// void GetActionAllBinding		(LPCSTR _action, char* dst_buff, int dst_buff_sz)
// {
// 	int			action_id	= action_name_to_id(_action);
// 	key_binding*	pbinding	= &g_key_bindings[action_id];
// 
// 	string128	prim;
// 	string128	sec;
// 	prim[0]		= 0;
// 	sec[0]		= 0;
// 
// 	if(pbinding->m_keyboard[0])
// 	{
// 		strings::copy(prim, pbinding->m_keyboard[0]->key_local_name);
// 	}
// 	if(pbinding->m_keyboard[1])
// 	{
// 		strings::copy(sec, pbinding->m_keyboard[1]->key_local_name);
// 	}
// // 	if(NULL==pbinding->m_keyboard[0] && NULL==pbinding->m_keyboard[1])
// // 	{
// // 		xr_sprintf		(dst_buff, dst_buff_sz, "%s", CStringTable().translate("st_key_notbinded").c_str());
// // 	}else
// // 		xr_sprintf		(dst_buff, dst_buff_sz, "%s%s%s", prim[0]?prim:"", (sec[0]&&prim[0])?" , ":"", sec[0]?sec:"");
// // 					
// }

BOOL bRemapped = FALSE;

void key_binder::bind_key(pcstr args, int bind_number) 
{
	string256							action;
	string256							key;
	*action								= 0;
	*key								= 0;

	sscanf_s							(args,"%255s %255s", action,sizeof(action), key, sizeof(key));
	if (!*action)
		return;

	if (!*key)
		return;

	if(!bRemapped) {
		remap_keys	();
		bRemapped	= TRUE;
	}

	if (!action_name_to_ptr(action))
		return;

	int action_id						= action_name_to_id			(action);
	if (action_id==kNOTBINDED)
		return;

	keyboard_key_descr*	pkeyboard				= keyname_to_ptr(key);
	if (!pkeyboard)
		return;

	key_binding*	curr_pbinding			= &g_key_bindings[action_id];

	curr_pbinding->m_keyboard[bind_number]= pkeyboard;
		
	{
		for(int idx=0; idx<bindings_count; ++idx)
		{
			key_binding*	binding			= &g_key_bindings[idx];
			if(binding==curr_pbinding)	continue;

			bool b_conflict = !is_group_not_conflicted(binding->m_action->key_group, curr_pbinding->m_action->key_group);

			if(binding->m_keyboard[0]==pkeyboard && b_conflict)
				binding->m_keyboard[0]=NULL;
			
			if(binding->m_keyboard[1]==pkeyboard && b_conflict)
				binding->m_keyboard[1]=NULL;
		}
	}


/*CStringTable::ReparseKeyBindings();*/
}
 
void key_binder::unbind_key( LPCSTR args, int bind_number )
{
	int action_id						= action_name_to_id			(args);
	key_binding*	pbinding				= &g_key_bindings[action_id];
	pbinding->m_keyboard[bind_number]	= NULL;

	/*CStringTable::ReparseKeyBindings();*/
} 
}; //namespace survarium