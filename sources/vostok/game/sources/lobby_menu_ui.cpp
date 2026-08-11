////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

// claude@NOTE: every function here drives the Scaleform flash UI through
// flash_value (Set*/SetMember/SetElement/ctor/dtor) and flash_movie
// (Invoke/CreateArray/CreateObject/Advance/Restart). With the scaleform glue now
// compiled /Ox (Master Gold, /GL), those tiny methods INLINE into the call sites
// here under LTCG - the old flash /Od wall is LIFTED (several methods reached 100%,
// most ~doubled). The remaining caps are NOT flash: the // STATE[STUB] bodies still
// need config/cooker/cross-module surfaces (items_dictionary::dict_config
// binary_config navigation, the player-parameters cooked resource, network::
// login_client port/server members, the chat_handler add_message path, the
// lobby_labels/login_labels string tables) that have no header / are not recoverable
// from the binary; the structure-recovered bodies that stay <100% are capped by
// allocator-inline (new/delete routing through global operator new vs the engine
// mspace) and minor LTCG instruction scheduling, not the flash glue.

#include "pch.h"
#include "lobby_menu.h"
#include "lobby_menu_external_handler.h"
#include "game.h"
#include "game_memory.h"
#include "lobby_client.h"
#include "messaging_client.h"
#include "base_network_client.h"
#include "chat_handler.h"
#include <vostok/resources.h>
#include <vostok/resources_queries_result.h>
#include <vostok/math_constants.h>
#include <vostok/vectora.h>
#include <vostok/game_core/game_net_defines.h>
#include <vostok/game_core/inventory_item_instance.h>
#include <vostok/network/login_client.h>
#include <vostok/configs_binary_config_value.h>
#include <vostok/strings_functions.h>
#include "ui_label.h"
#include "price_item.h"
#include "player_leveling_info.h"
#include "profile_player_character.h"
#include <vostok/scaleform/sources/flash_movie.h>
#include <vostok/scaleform/sources/flash_movie_resource.h>
#include <vostok/scaleform/sources/flash_value.h>
#include <vostok/scaleform/sources/flash_function_handler.h>
#include <vostok/scaleform/sources/flash_function_handler_params.h>

namespace survarium {

// TU-local (canonical headers/relocate_item_func.h; owner mapping in
// temp/triage_log.md) - lobby_menu holds it through m_relocate_item_func
class relocate_item_func : public flash_function_handler , public boost::noncopyable {
public:
			explicit	relocate_item_func	( game& g );

	virtual	void		call				( flash_function_handler_params& params ) override;

	inline				~relocate_item_func	( ) { /* no source */ }

private:
	/* 0x0000 */	/* flash_function_handler */
	/* 0x0008 */	/* boost::noncopyable */
	/* 0x0008 */	game&		m_game;
}; // class relocate_item_func

STATIC_SIZE_ASSERT(relocate_item_func, 0xC);

static float const	clear_value		= 1.0f;

relocate_item_func::relocate_item_func( game& g )
	: m_game( g )
{
}

void relocate_item_func::call( flash_function_handler_params& params )
{
	LOG_INFO( "TRY RELOCATE(FLASH)!!!" );

	vector< relocate_item_descr > descriptions;
	lobby_client& lobby = m_game.network_client( ).lobby_client( );

	for ( u8 i = 0; i < params.pArgs[ 0 ].GetArraySize( ); ++i )
	{
		flash_value descr_value;
		params.pArgs[ 0 ].GetElement( i, &descr_value );

		flash_value descr_member_value;
		relocate_item_descr current;

		descr_value.GetMember( "profile", &descr_member_value );
		current.profile_id = descr_member_value.GetUInt( );

		descr_value.GetMember( "id", &descr_member_value );
		current.item_id = descr_member_value.GetUInt( );

		descr_value.GetMember( "dict_id", &descr_member_value );
		current.item_dict_id = descr_member_value.GetUInt( );

		descr_value.GetMember( "sourceSlot", &descr_member_value );
		current.source_slot_id = descr_member_value.GetUInt( );

		descr_value.GetMember( "targetSlot", &descr_member_value );
		current.target_slot_id = descr_member_value.GetUInt( );

		descr_value.GetMember( "count", &descr_member_value );
		current.amount = ( u16 )descr_member_value.GetUInt( );

		dictionary_item current_item = m_game.items_dictionary( ).item_by_id( current.item_dict_id );

		LOG_INFO(
			"try move item %d from %d to %d. ",
			current.item_id,
			current.source_slot_id,
			current.target_slot_id,
			current.amount
		);

		u32 second_item_id = 0;
		if ( current.target_slot_id == ammo1_weapon1_slot || current.target_slot_id == ammo2_weapon1_slot )
			second_item_id = weapon1_slot;
		else if ( current.target_slot_id == ammo1_weapon2_slot || current.target_slot_id == ammo2_weapon2_slot )
			second_item_id = weapon2_slot;

		if ( second_item_id )
		{
			profile_slot_enum const weapon_slot = ( profile_slot_enum )second_item_id;
			second_item_id = lobby.profile( m_game.lobby_menu( ).selected_profile( ) ).slots[ weapon_slot ].item.dict_id;

			for ( vector< relocate_item_descr >::const_iterator j = descriptions.begin( ); j != descriptions.end( ); ++j )
			{
				if ( j->target_slot_id == weapon_slot )
					second_item_id = j->item_dict_id;
			}
		}

		if ( lobby.can_move_item( current_item.item_category, current.target_slot_id ) &&
			( !second_item_id || lobby.check_compatibility( current.item_dict_id, second_item_id ) ) )
			descriptions.push_back( current );
	}

	lobby.move_item( descriptions );
}

// claude@NOTE: flash external-interface dispatch over methodName. The args reads
// (args[N].GetUInt()/GetBool()/GetString()) inline to the GFx Value union field at +8 under
// LTCG (scaleform Master Gold /GL); residual is that GFx-Value-internal inlining + the
// flash_value ctor/dtor scheduling (the start_friend_message flash_value[2] + unlock_perks
// vectora builds) and the LTCG slot reuse of use_premium/faction_id in buy_ok_clicked.
// The method-name string literals are matched from the target rdata.
void lobby_menu_external_handler::callback(
	flash_movie*			pmovieView,
	pcstr					methodName,
	flash_value const*		args,
	u32						argCount
)
{
	if ( strings::equal( methodName, "leave_queue" ) )
	{
		m_game.network_client( ).lobby_client( ).discard_playing_order( );
	}
	else if ( strings::equal( methodName, "play_button_clicked" ) )
	{
		if ( m_game.network_client( ).lobby_client( ).profiles_count( ) )
		{
			u8 faction_id = m_game.lobby_menu( ).selected_profile( );
			m_game.network_client( ).lobby_client( ).set_status_ready_for_match(
				m_game.network_client( ).lobby_client( ).profile( faction_id ).profile_id );
		}
	}
	else if ( strings::equal( methodName, "profile_changed" ) )
	{
		m_game.lobby_menu( ).on_profile_changed( ( u8 )args[ 0 ].GetUInt( ) );
	}
	else if ( strings::equal( methodName, "shop_ready" ) )
	{
		m_game.lobby_menu( ).on_shop_ui_ready( );
	}
	else if ( strings::equal( methodName, "set_mouse_cursor" ) )
	{
		m_game.lobby_menu( ).set_cursor( ( u8 )args[ 0 ].GetUInt( ) );
	}
	else if ( strings::equal( methodName, "buy_ok_clicked" ) )
	{
		u16 item_dict_id	= ( u16 )args[ 0 ].GetUInt( );
		u32 count			= args[ 1 ].GetUInt( );

		u8 faction_id = ( u8 )args[ 2 ].GetUInt( );

		if ( !faction_id )
		{
			for ( u8 i = 1; i <= 4; ++i )
			{
				faction_price const& price = m_game.network_client( ).lobby_client( ).price( i );
				for ( u8 j = 0; j < price.count; ++j )
				{
					if ( price.items[ j ].item_dict_id == item_dict_id )
					{
						faction_id = i;
						break;
					}
				}

				if ( faction_id )
					break;
			}
		}

		m_game.network_client( ).lobby_client( ).buy_item( item_dict_id, count, faction_id, false );
	}
	else if ( strings::equal( methodName, "unlock_perks" ) )
	{
		flash_value skills_array	= args[ 0 ];
		flash_value perks_array		= args[ 1 ];

		vectora< player_skill > skills( g_allocator );

		for ( u8 i = 0; i < skills_array.GetArraySize( ); ++i )
		{
			flash_value branch_value;
			skills_array.GetElement			( i, &branch_value );

			flash_value branch_member_value;
			branch_value.GetMember			( "id", &branch_member_value );

			player_skill current_skill;
			current_skill.skill_id			= ( u8 )branch_member_value.GetUInt( );

			branch_value.GetMember			( "points", &branch_member_value );
			current_skill.skill_points		= ( u8 )branch_member_value.GetUInt( );

			skills.push_back				( current_skill );
		}

		vectora< u8 > perks( g_allocator );

		for ( u8 i = 0; i < perks_array.GetArraySize( ); ++i )
		{
			flash_value perk_value;
			perks_array.GetElement			( i, &perk_value );

			u8 perk = ( u8 )perk_value.GetUInt( );
			perks.push_back					( perk );
		}

		m_game.network_client( ).lobby_client( ).set_player_skills( skills, perks );
	}
	else if ( strings::equal( methodName, "reroll_ok_clicked" ) )
	{
		m_game.network_client( ).lobby_client( ).reroll_player_skills( );
	}
	else if ( strings::equal( methodName, "find_players" ) )
	{
		pcstr player_name = args[ 0 ].GetString( );
		enum { c_min_name_len_to_search = 3 };
		if ( strlen( player_name ) >= c_min_name_len_to_search )
			m_game.network_client( ).messaging_client( ).find_players_by_name( player_name );
	}
	else if ( strings::equal( methodName, "add_friend" ) )
	{
		m_game.network_client( ).messaging_client( ).add_to_friend_list( args[ 0 ].GetUInt( ) );
	}
	else if ( strings::equal( methodName, "remove_friend" ) )
	{
		m_game.network_client( ).messaging_client( ).remove_from_friend_list( args[ 0 ].GetUInt( ) );
	}
	else if ( strings::equal( methodName, "add_ignore" ) )
	{
		m_game.network_client( ).messaging_client( ).add_to_ignore_list( args[ 0 ].GetUInt( ) );
	}
	else if ( strings::equal( methodName, "remove_ignored" ) )
	{
		m_game.network_client( ).messaging_client( ).remove_from_ignore_list( args[ 0 ].GetUInt( ) );
	}
	else if ( strings::equal( methodName, "start_friend_message" ) )
	{
		flash_value ret_args[2];
		ret_args[ 0 ].SetString			( args[ 0 ].GetString( ) );
		ret_args[ 1 ].SetUInt			( 100 );
		m_game.get_chat_handler( ).get_movie( )->movie->Invoke( "root.start_message", NULL, ret_args, 2 );
		m_game.get_chat_handler( ).focus( true );
	}
	else if ( strings::equal( methodName, "show_settings" ) )
	{
		m_game.activate_main_menu( );
	}
}

// claude@NOTE: flash glue now inlines at /Ox (scaleform Master Gold /GL), so the
// inline-vs-call wall is lifted; residual is LTCG statement scheduling around the
// inlined flash_value ctor/dtor (base merges a few of the dtor cleanups the target
// keeps as separate statements). The 7-entry label name/key table and the localized
// wide placeholder strings (set_current_time / set_place / set_status) are
// reconstructed approximately.
void lobby_menu::show_match_making( bool b_show )
{
	if ( m_is_in_match_making != b_show )
	{
		if ( b_show )
		{
			m_level_loading_progress = 0.f;
			m_last_queries_count = 0;
			m_match_making_ui->movie->Restart	( );

			flash_value labels_array;
			m_match_making_ui->movie->CreateArray( &labels_array );

			static const struct { pcstr name; pcstr translate_id; } c_labels[] =
			{
				{ "average_time",	"st_average_search_time" },
				{ "current_time",	"st_current_search_time" },
				{ "players_count",	"st_players_in_queue" },
				{ "place",			"st_place_in_queue" },
				{ "status",			"st_search_status" },
				{ "min_players",	"st_min_players" },
				{ "max_players",	"st_max_players" },
			};

			for ( u32 i = 0; i < 7; ++i )
			{
				flash_value label;
				m_match_making_ui->movie->CreateObject( &label );

				flash_value label_member;
				label_member.SetString			( c_labels[ i ].name );
				label.SetMember					( "name", label_member );

				wchar_t label_txt[512];
				get_game( ).text_translator( ).translate_text( c_labels[ i ].translate_id, label_txt );
				label_member.SetStringW			( label_txt );
				label.SetMember					( "label", label_member );

				labels_array.SetElement			( i, label );
			}

			m_match_making_ui->movie->Invoke	( "root.set_labels", NULL, &labels_array, 1 );

			flash_value text;
			text.SetStringW						( L"0 / 5:" );
			m_match_making_ui->movie->Invoke	( "root.set_current_time", NULL, &text, 1 );

			text.SetStringW						( L"" );
			m_match_making_ui->movie->Invoke	( "root.set_average_time", NULL, &text, 1 );

			text.SetStringW						( L"-" );
			m_match_making_ui->movie->Invoke	( "root.set_place", NULL, &text, 1 );

			text.SetStringW						( L"..." );
			m_match_making_ui->movie->Invoke	( "root.set_status", NULL, &text, 1 );

			show_movie							( m_match_making_ui );
		}
		else
		{
			hide_movie							( m_match_making_ui );
		}

		m_is_in_match_making = b_show;
	}
}

// claude@NOTE: flash glue now inlines at /Ox (scaleform Master Gold /GL); structure
// is faithful and the residual is LTCG scheduling of the inlined flash_value
// ctor/dtor, not the old inline-vs-call wall.
// sushi@TODO: the level-name localization key L"st_loading_level" is a LENGTH-matched
// guess (16 wide chars => the 4x movq + word copy the target emits); content is loaded
// from rdata and does not affect this function's bytes, but the exact key is unverified.
void lobby_menu::update_level_loading_progress( )
{
	char buff[64];
	vostok::sprintf						( buff, "(%d)", resources::pending_queries_count( ) );

	if ( m_last_queries_count > resources::pending_queries_count( ) )
		m_level_loading_progress += ( 1.0f - m_level_loading_progress ) *
			( float )( m_last_queries_count - resources::pending_queries_count( ) ) / ( float )m_last_queries_count;

	m_last_queries_count = resources::pending_queries_count( );

	wchar_t w_text[512] = L"st_loading_level";

	wchar_t level_name[512];
	mbstowcs_s							( NULL, level_name, 512, get_game( ).project_resource_name( ), _TRUNCATE );

	wchar_t queries_count[512];
	mbstowcs_s							( NULL, queries_count, 512, buff, _TRUNCATE );

	wcscat_s							( w_text, level_name );
	wcscat_s							( w_text, L"=" );
	wcscat_s							( w_text, queries_count );

	flash_value text;
	text.SetStringW						( w_text );
	m_match_making_ui->movie->Invoke	( "root.set_status", NULL, &text, 1 );

	if ( clear_value - m_level_loading_progress < math::epsilon_3 )
		m_level_loading_progress = clear_value;

	flash_value progress;
	progress.SetUInt					( ( u32 )( m_level_loading_progress * 100.0f ) );
	m_match_making_ui->movie->Invoke	( "root.set_percent", NULL, &progress, 1 );
}

void lobby_menu::on_ui_destroy( )
{
	DELETE	( m_lobby_menu_external_handler );
	DELETE	( m_relocate_item_func );
}

void lobby_menu::update_ui( const u32 frame_delta_in_ms, const u32 current_time_in_ms )
{
	if ( get_game( ).get_game_world( ).is_loading( ) )
		update_level_loading_progress( );

	if ( is_active( ) )
	{
		flash_value queries_count;
		queries_count.SetUInt			( resources::pending_queries_count( ) );
		m_lobby_menu_ui->movie->Invoke	( "root.set_disk_query", NULL, &queries_count, 1 );
	}

	float deltaTime = frame_delta_in_ms * math::epsilon_3;

	m_cursor_ui->movie->Advance			( deltaTime, 0 );
	m_lobby_menu_ui->movie->Advance		( deltaTime, 0 );
	m_message_ui->movie->Advance		( deltaTime, 0 );

	if ( m_is_in_match_making )
		m_match_making_ui->movie->Advance( deltaTime, 0 );
}

// claude@NOTE: PARKED on cross-module network::login_client accessors. Recovered body:
// fixed_string<128> status_str; flash_value b_val; if(lobby_client().status(status_str)
// == <not ready-for-match>) b_val.SetBoolean(true); login = login_client(); build a
// 4-element account_info array via SetElement: [0]=login.account_name(),
// [1]=login.<browser addr 0x134>, [2]=port (login.<u16 0x174>), [3]=buff.assignf
// ("%s:%d", login.<server 0x28>, login.<u16 0x28+>); Invoke
// "root.lobby_menu.set_account_info"; Invoke "root.lock_play_button" (b_val); Invoke
// "root.set_status_info" (status_str). login_client only exposes account_name() +
// server_browser_address() publicly - the port/server-address members (0x134/0x174/
// 0x28) have no accessor. Recover when network::login_client is matched. Also
// byte-capped by the scaleform flash stubs.
void lobby_menu::update_status( )
{
	flash_value b_val;

	fixed_string< 128 > status_str;
	lobby_client( ).status				( status_str );

	flash_value account_info;
	m_lobby_menu_ui->movie->CreateArray	( &account_info );
	account_info.SetElement				( 0, login_client( ).account_name( ) );

	m_lobby_menu_ui->movie->Invoke		( "root.lobby_menu.set_account_info", NULL, &account_info, 1 );
	m_lobby_menu_ui->movie->Invoke		( "root.lock_play_button", NULL, &b_val, 1 );

	flash_value log_message;
	log_message.SetString				( status_str.c_str( ) );
	m_lobby_menu_ui->movie->Invoke		( "root.set_status_info", NULL, &log_message, 1 );

	return;

	// FUNCTION BODY[0x743190]: 35
	// <0x7431a0>|0x010|+0x010:'446'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x7431b0>|0x020|+0x051:'451'
	// <0x743201>|0x071|+0x004:'452'
	// <0>
	// <1>
	// <2>
	// <0x743205>|0x075|+0x02e:'456'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x743233>|0x0a3|+0x013:'461'
	// <0x743246>|0x0b6|+0x015:'462'
	// <0>
	// <1>
	// <0x74325b>|0x0cb|+0x023:'465'
	// <0x74327e>|0x0ee|+0x013:'466'
	// <0x743291>|0x101|+0x011:'467'
	// <0>
	// <1>
	// <0x7432a2>|0x112|+0x032:'470'
	// <0x7432d4>|0x144|+0x01d:'471'
	// <0x7432f1>|0x161|+0x018:'472'
	// <0x743309>|0x179|+0x015:'473'
	// <0>
	// <0x74331e>|0x18e|+0x021:'475'
	// <0>
	// <1>
	// <0x74333f>|0x1af|+0x018:'478'
	// <0x743357>|0x1c7|+0x021:'479'
	// <0x743378>|0x1e8|+0x01f:'480'
	// ******
}

// claude@NOTE: PARKED - heavy cross-subsystem wall (sound::world_user listener setup,
// render::scene_renderer::add_model, game::create_network_client, flash_external_handler
// ctor + CreateFunction/GetVariable glue) plus the scaleform flash /Od inline wall. The
// camera/scene wiring members have no header here. Recover with the render/sound scene
// surfaces.
// STATE[STUB]
void lobby_menu::on_render_scenes_ready( resources::queries_result& data )
{
	// LOCALS
	// flash_value 						func
	// float3 							pos
	// float3 							dir
	// flash_value 						proxy
	// flash_value 						players_count
	// configs::binary_config_ptr 		player_config
	// ******

	// CALL SITE INFO
	// <0x74729d> -> sound::world_user& < unknown >() const
	// <0x74797f> -> void < unknown >( bool )
	// <0x7479d7> -> void < unknown >()
	// ******

	// FUNCTION BODY[0x747070]: 92
	// <0>
	// <1>
	// <0x74707c>|0x00c|+0x078:'487'
	// <0x7470f4>|0x084|+0x06f:'488'
	// <0x747163>|0x0f3|+0x0a8:'489'
	// <0>
	// <0x74720b>|0x19b|+0x09b:'491'
	// <0>
	// <0x7472a6>|0x236|+0x0a5:'493'
	// <0>
	// <0x74734b>|0x2db|+0x024:'495'
	// <0>
	// <1>
	// <0x74736f>|0x2ff|+0x0a5:'498'
	// <0x747414>|0x3a4|+0x01f:'499'
	// <0x747433>|0x3c3|+0x006:'500'
	// <0>
	// <0x747439>|0x3c9|+0x0af:'502'
	// <0x7474e8>|0x478|+0x01f:'503'
	// <0x747507>|0x497|+0x006:'504'
	// <0>
	// <0x74750d>|0x49d|+0x0af:'506'
	// <0x7475bc>|0x54c|+0x01f:'507'
	// <0x7475db>|0x56b|+0x006:'508'
	// <0>
	// <0x7475e1>|0x571|+0x0af:'510'
	// <0x747690>|0x620|+0x023:'511'
	// <0x7476b3>|0x643|+0x006:'512'
	// <0>
	// <0x7476b9>|0x649|+0x04f:'514'
	// <0>
	// <0x747708>|0x698|+0x03d:'516'
	// <0>
	// <0x747745>|0x6d5|+0x03d:'518'
	// <0>
	// <0x747782>|0x712|+0x026:'520'
	// <0x7477a8>|0x738|+0x026:'521'
	// <0>
	// <0x7477ce>|0x75e|+0x018:'523'
	// <0x7477e6>|0x776|+0x017:'524'
	// <0>
	// <0x7477fd>|0x78d|+0x018:'526'
	// <0x747815>|0x7a5|+0x017:'527'
	// <0>
	// <0x74782c>|0x7bc|+0x018:'529'
	// <0x747844>|0x7d4|+0x017:'530'
	// <0>
	// <0x74785b>|0x7eb|+0x018:'532'
	// <0x747873>|0x803|+0x017:'533'
	// <0x74788a>|0x81a|+0x026:'534'
	// <0>
	// <1>
	// <2>
	// <0x7478b0>|0x840|+0x026:'538'
	// <0x7478d6>|0x866|+0x023:'539'
	// <0>
	// <0x7478f9>|0x889|+0x02e:'541'
	// <0x747927>|0x8b7|+0x02e:'542'
	// <0>
	// <0x747955>|0x8e5|+0x01f:'544'
	// <0>
	// <0x747974>|0x904|+0x00d:'546'
	// <0>
	// <1>
	// <2>
	// <0x747981>|0x911|+0x035:'550'
	// <0>
	// <0x7479b6>|0x946|+0x00c:'552'
	// <0x7479c2>|0x952|+0x006:'553'
	// <0>
	// <0x7479c8>|0x958|+0x008:'555'
	// <0>
	// <0x7479d0>|0x960|+0x002:'557'
	// <0x7479d2>|0x962|+0x00e:'558'
	// <0>
	// <1>
	// <0x7479e0>|0x970|+0x01e:'561'
	// <0x7479fe>|0x98e|+0x036:'562'
	// <0>
	// <0x747a34>|0x9c4|+0x01a:'564'
	// <0x747a4e>|0x9de|+0x01a:'565'
	// <0>
	// <0x747a68>|0x9f8|+0x02d:'567'
	// <0x747a95>|0xa25|+0x03b:'568'
	// <0x747ad0>|0xa60|+0x0ca:'569'
	// <0>
	// <0x747b9a>|0xb2a|+0x008:'571'
	// <0x747ba2>|0xb32|+0x006:'572'
	// <0>
	// <0x747ba8>|0xb38|+0x01f:'574'
	// <0>
	// <0x747bc7>|0xb57|+0x008:'576'
	// ******
}

// claude@NOTE: walks the file-scope survarium::lobby_labels ui_label table (73 {name,label}
// pairs); the table is a fabricated placeholder - the strings are read at runtime (data only,
// zero bytes in this function), so its content does not affect the match, but the exact 73
// pairs are unrecoverable from the binary (same data wall as login_menu::fill_labels).
// Residual is the scaleform flash /Od inline wall (CreateObject/SetMember/SetStringW + the
// flash_value ctor/dtor) and LTCG scheduling.
static ui_label lobby_labels_data[0x49] =
{
	{ "label_0", "st_label_0" },
	{ "label_1", "st_label_1" },
	{ "label_2", "st_label_2" },
	{ "label_3", "st_label_3" },
	{ "label_4", "st_label_4" },
	{ "label_5", "st_label_5" },
	{ "label_6", "st_label_6" },
	{ "label_7", "st_label_7" },
	{ "label_8", "st_label_8" },
	{ "label_9", "st_label_9" },
	{ "label_10", "st_label_10" },
	{ "label_11", "st_label_11" },
	{ "label_12", "st_label_12" },
	{ "label_13", "st_label_13" },
	{ "label_14", "st_label_14" },
	{ "label_15", "st_label_15" },
	{ "label_16", "st_label_16" },
	{ "label_17", "st_label_17" },
	{ "label_18", "st_label_18" },
	{ "label_19", "st_label_19" },
	{ "label_20", "st_label_20" },
	{ "label_21", "st_label_21" },
	{ "label_22", "st_label_22" },
	{ "label_23", "st_label_23" },
	{ "label_24", "st_label_24" },
	{ "label_25", "st_label_25" },
	{ "label_26", "st_label_26" },
	{ "label_27", "st_label_27" },
	{ "label_28", "st_label_28" },
	{ "label_29", "st_label_29" },
	{ "label_30", "st_label_30" },
	{ "label_31", "st_label_31" },
	{ "label_32", "st_label_32" },
	{ "label_33", "st_label_33" },
	{ "label_34", "st_label_34" },
	{ "label_35", "st_label_35" },
	{ "label_36", "st_label_36" },
	{ "label_37", "st_label_37" },
	{ "label_38", "st_label_38" },
	{ "label_39", "st_label_39" },
	{ "label_40", "st_label_40" },
	{ "label_41", "st_label_41" },
	{ "label_42", "st_label_42" },
	{ "label_43", "st_label_43" },
	{ "label_44", "st_label_44" },
	{ "label_45", "st_label_45" },
	{ "label_46", "st_label_46" },
	{ "label_47", "st_label_47" },
	{ "label_48", "st_label_48" },
	{ "label_49", "st_label_49" },
	{ "label_50", "st_label_50" },
	{ "label_51", "st_label_51" },
	{ "label_52", "st_label_52" },
	{ "label_53", "st_label_53" },
	{ "label_54", "st_label_54" },
	{ "label_55", "st_label_55" },
	{ "label_56", "st_label_56" },
	{ "label_57", "st_label_57" },
	{ "label_58", "st_label_58" },
	{ "label_59", "st_label_59" },
	{ "label_60", "st_label_60" },
	{ "label_61", "st_label_61" },
	{ "label_62", "st_label_62" },
	{ "label_63", "st_label_63" },
	{ "label_64", "st_label_64" },
	{ "label_65", "st_label_65" },
	{ "label_66", "st_label_66" },
	{ "label_67", "st_label_67" },
	{ "label_68", "st_label_68" },
	{ "label_69", "st_label_69" },
	{ "label_70", "st_label_70" },
	{ "label_71", "st_label_71" },
	{ "label_72", "st_label_72" },
};

ui_label* lobby_labels = lobby_labels_data;

void lobby_menu::fill_inventory_labels( )
{
	flash_value labels;
	m_lobby_menu_ui->movie->CreateObject( &labels );

	for ( u32 i = 0; i < 0x49; ++i )
	{
		wchar_t label_w[512];
		get_game( ).text_translator( ).translate_text( lobby_labels[ i ].label, label_w );

		flash_value label_translate;
		label_translate.SetStringW		( label_w );

		labels.SetMember				( lobby_labels[ i ].name, label_translate );
	}

	m_lobby_menu_ui->movie->Invoke		( "root.set_localization_data", NULL, &labels, 1 );
}

// claude@NOTE: PARKED - navigates items_dictionary()'s private dict_config binary_config
// (items_dictionary::dict_config has no accessor; game_core class, can't edit here) and
// iterates its dictionary_item map (the dictionary_item type/ctor/dtor/copy live with the
// dict, not yet headered here). Also scaleform flash /Od inline wall. Recover once
// items_dictionary exposes dict_config + dictionary_item.
// STATE[STUB]
void lobby_menu::fill_items_dictionary( )
{
	// LOCALS
	// map< u32, dictionary_item, std::less< u32 > > const& itm_dict
	// std::priv::_Rb_tree_iterator< std::pair< u32 const , dictionary_item >, std::priv::_ConstMapTraitsT< std::pair< u32 const , dictionary_item > > > itm_it
	// fixed_string< 32 >[6] 			sellers_names
	// flash_value 						traders_array_item_property
	// flash_value 						items_descr_array
	// flash_value 						inventory_item_property
	// flash_value 						traders_array
	// u32 								in_array_index
	// u32 								icon
	// flash_value 						inventory_item_propertyies_array
	// flash_value 						item_property_member
	// u8 								current_item_category_id
	// wchar_t[512] 					item_name
	// u32 								current_item_dict_id
	// wchar_t[512] 					item_desc
	// float 							item_weight
	// bool 							current_item_is_stack
	// configs::binary_config_value const* it_end
	// flash_value 						inventory_item_descr
	// dictionary_item 					current_item
	// u32 								j
	// wchar_t[512] 					prop_name
	// flash_value 						item_property
	// u32 								prop_icon
	// flash_value 						traders_array_item
	// wchar_t[512] 					faction_name
	// ******

	// FUNCTION BODY[0x745500]: 144
	// <0x74550f>|0x00f|+0x006:'699'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x745515>|0x015|+0x015:'705'
	// <0>
	// <1>
	// <2>
	// <0x74552a>|0x02a|+0x056:'709'
	// <0>
	// <0x745580>|0x080|+0x013:'711'
	// <0x745593>|0x093|+0x007:'712'
	// <0>
	// <0x74559a>|0x09a|+0x014:'714'
	// <0x7455ae>|0x0ae|+0x00b:'715'
	// <0>
	// <0x7455b9>|0x0b9|+0x01f:'717'
	// <0x7455d8>|0x0d8|+0x0f9:'718'
	// <0x7456d1>|0x1d1|+0x00e:'719'
	// <0>
	// <1>
	// <0x7456df>|0x1df|+0x01a:'722'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x7456f9>|0x1f9|+0x047:'728'
	// <0>
	// <0x745740>|0x240|+0x040:'730'
	// <0>
	// <1>
	// <0x745780>|0x280|+0x026:'733'
	// <0>
	// <1>
	// <2>
	// <0x7457a6>|0x2a6|+0x02b:'737'
	// <0x7457d1>|0x2d1|+0x038:'738'
	// <0>
	// <0x745809>|0x309|+0x010:'740'
	// <0x745819>|0x319|+0x02c:'741'
	// <0>
	// <0x745845>|0x345|+0x00c:'743'
	// <0x745851>|0x351|+0x02a:'744'
	// <0>
	// <0x74587b>|0x37b|+0x02b:'746'
	// <0x7458a6>|0x3a6|+0x03a:'747'
	// <0>
	// <0x7458e0>|0x3e0|+0x028:'749'
	// <0x745908>|0x408|+0x038:'750'
	// <0>
	// <1>
	// <0x745940>|0x440|+0x01f:'753'
	// <0x74595f>|0x45f|+0x03e:'754'
	// <0x74599d>|0x49d|+0x03a:'755'
	// <0x7459d7>|0x4d7|+0x029:'756'
	// <0>
	// <1>
	// <0x745a00>|0x500|+0x00b:'759'
	// <0>
	// <0x745a0b>|0x50b|+0x02e:'761'
	// <0x745a39>|0x539|+0x045:'762'
	// <0>
	// <0x745a7e>|0x57e|+0x029:'764'
	// <0x745aa7>|0x5a7|+0x034:'765'
	// <0>
	// <1>
	// <2>
	// <0x745adb>|0x5db|+0x02d:'769'
	// <0>
	// <0x745b08>|0x608|+0x01a:'771'
	// <0x745b22>|0x622|+0x02b:'772'
	// <0>
	// <0x745b4d>|0x64d|+0x008:'774'
	// <0>
	// <0x745b55>|0x655|+0x2a8:'776'
	// <0x745dfd>|0x8fd|-0x29a:'776'
	// <0>
	// <1>
	// <0x745b63>|0x663|+0x027:'779'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x745b8a>|0x68a|+0x028:'785'
	// <0>
	// <0x745bb2>|0x6b2|+0x00e:'787'
	// <0x745bc0>|0x6c0|+0x02e:'788'
	// <0>
	// <0x745bee>|0x6ee|+0x079:'790'
	// <0x745c67>|0x767|+0x02c:'791'
	// <0>
	// <0x745c93>|0x793|+0x083:'793'
	// <0x745d16>|0x816|+0x02e:'794'
	// <0>
	// <0x745d44>|0x844|+0x02c:'796'
	// <0x745d70>|0x870|+0x038:'797'
	// <0>
	// <0x745da8>|0x8a8|+0x020:'799'
	// <0x745dc8>|0x8c8|+0x037:'800'
	// <0>
	// <0x745dff>|0x8ff|+0x031:'802'
	// <0>
	// <0x745e30>|0x930|+0x023:'804'
	// <0>
	// <0x745e53>|0x953|+0x0dc:'806'
	// <0x745f2f>|0xa2f|+0x029:'807'
	// <0>
	// <1>
	// <2>
	// <0x745f58>|0xa58|+0x02a:'811'
	// <0>
	// <0x745f82>|0xa82|+0x028:'813'
	// <0x745faa>|0xaaa|+0x04b:'814'
	// <0x745ff5>|0xaf5|+0x050:'815'
	// <0x746045>|0xb45|+0x050:'816'
	// <0x746095>|0xb95|+0x050:'817'
	// <0x7460e5>|0xbe5|+0x050:'818'
	// <0x746135>|0xc35|+0x050:'819'
	// <0>
	// <0x746185>|0xc85|+0x015:'821'
	// <0>
	// <0x74619a>|0xc9a|+0x006:'823'
	// <0>
	// <1>
	// <0x7461a0>|0xca0|+0x029:'826'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x7461c9>|0xcc9|+0x01e:'832'
	// <0>
	// <0x7461e7>|0xce7|+0x098:'834'
	// <0x74627f>|0xd7f|+0x031:'835'
	// <0>
	// <0x7462b0>|0xdb0|+0x034:'837'
	// <0x7462e4>|0xde4|+0x046:'838'
	// <0>
	// <0x74632a>|0xe2a|+0x01c:'840'
	// <0x746346>|0xe46|+0x030:'841'
	// <0x746376>|0xe76|+0x029:'842'
	// ******
}

void lobby_menu::fill_inventory_contents( )
{
	inventory_item_instance const* it	= lobby_client( ).inventory_item_instances( ).begin( );
	inventory_item_instance const* it_e	= lobby_client( ).inventory_item_instances( ).end( );

	flash_value inventory_array;
	m_lobby_menu_ui->movie->CreateArray	( &inventory_array );

	u32 i = 0;
	for ( ; it != it_e; ++it )
	{
		flash_value inventory_item;
		m_lobby_menu_ui->movie->CreateObject( &inventory_item );

		flash_value inventory_item_property;
		inventory_item_property.SetInt	( it->id );
		inventory_item.SetMember		( "id", inventory_item_property );

		inventory_item_property.SetInt	( it->dict_id );
		inventory_item.SetMember		( "dictId", inventory_item_property );

		inventory_item_property.SetUInt	( it->condition_or_stack );
		inventory_item.SetMember		( "condition", inventory_item_property );

		inventory_item_property.SetUInt	( it->condition_or_stack );
		inventory_item.SetMember		( "condition_or_stack", inventory_item_property );

		inventory_array.SetElement		( i, inventory_item );
		++i;
	}

	m_lobby_menu_ui->movie->Invoke		( "root.inventory_list.setupInventoryData", NULL, &inventory_array, 1 );
}

void lobby_menu::on_items_compatibility_arrived( )
{
	flash_value slot_restrictions_array;
	m_lobby_menu_ui->movie->CreateArray	( &slot_restrictions_array );

	for ( u8 i = 0; i < lobby_client( ).item_compatibilities_count( ); ++i )
	{
		flash_value items_compatibility_item;
		m_lobby_menu_ui->movie->CreateObject( &items_compatibility_item );

		flash_value items_compatibility_item_property;
		items_compatibility_item_property.SetUInt	( lobby_client( ).get_items_compatibility( i ).first_item_dict_id );
		items_compatibility_item.SetMember			( "first_item_dict_id", items_compatibility_item_property );

		items_compatibility_item_property.SetUInt	( lobby_client( ).get_items_compatibility( i ).second_item_dict_id );
		items_compatibility_item.SetMember			( "second_item_dict_id", items_compatibility_item_property );

		slot_restrictions_array.SetElement			( i, items_compatibility_item );
	}

	m_lobby_menu_ui->movie->Invoke		( "root.player_profile.profileItems.setItemsCompatibility", NULL, &slot_restrictions_array, 1 );
}

void lobby_menu::on_slot_restrictions_arrived( )
{
	flash_value slot_restrictions_array;
	m_lobby_menu_ui->movie->CreateArray	( &slot_restrictions_array );

	for ( u8 i = 0; i < lobby_client( ).slot_restrictions_count( ); ++i )
	{
		flash_value slot_restriction_item;
		m_lobby_menu_ui->movie->CreateObject( &slot_restriction_item );

		flash_value slot_restriction_item_property;
		slot_restriction_item_property.SetUInt	( lobby_client( ).slot_restriction( i ).slot_dict_id );
		slot_restriction_item.SetMember			( "slot_id", slot_restriction_item_property );

		slot_restriction_item_property.SetUInt	( lobby_client( ).slot_restriction( i ).category_dict_id );
		slot_restriction_item.SetMember			( "category_id", slot_restriction_item_property );

		slot_restrictions_array.SetElement		( i, slot_restriction_item );
	}

	m_lobby_menu_ui->movie->Invoke		( "root.player_profile.profileItems.setSlotsRestrictions", NULL, &slot_restrictions_array, 1 );
}

void lobby_menu::fill_profiles( )
{
	flash_value profiles_array;
	m_lobby_menu_ui->movie->CreateArray	( &profiles_array );

	for ( u8 i = 0; i < lobby_client( ).profiles_count( ); ++i )
	{
		flash_value profile_item;
		m_lobby_menu_ui->movie->CreateObject( &profile_item );

		wchar_t profile_name_w[512];
		mbstowcs_s						( NULL, profile_name_w, 512, lobby_client( ).profile( i ).profile_name, _TRUNCATE );

		flash_value profile_item_property;
		profile_item_property.SetStringW( profile_name_w );
		profile_item.SetMember			( "name", profile_item_property );

		profile_item_property.SetInt	( 1 );
		profile_item.SetMember			( "icon", profile_item_property );

		profiles_array.SetElement		( i, profile_item );
	}

	m_lobby_menu_ui->movie->Invoke		( "root.player_profile.setupProfiles", NULL, &profiles_array, 1 );
}

// claude@NOTE: PARKED - builds a player_parameters_query_path + queues a cooked-resource
// query (resources::query_resources with a player_parameters_cooker_data* user_data via
// profile_player_character::query_profile_contents) and walks player_profile slot items;
// the cooked player-parameters resource type + the profile/slot-item layout have no header
// here. Also scaleform flash /Od inline wall. Recover in the player-parameters cooker phase.
// STATE[STUB]
void lobby_menu::on_profile_changed( u8 profile_id )
{
	// LOCALS
	// flash_value 						profile_descriptor
	// variant< 32 > 					user_data
	// flash_value 						profile_items_array
	// player_profile const& 			profile
	// fixed_string< 260 > 				player_parameters_query_path
	// flash_value 						profile_descriptor_property
	// flash_value 						slot_item_property
	// u8 								in_array_index
	// flash_value 						v
	// u32 								condition_or_stack
	// flash_value 						slot_item
	// ******

	// CALL SITE INFO
	// <0x746a1c> -> lobby_client& < unknown >()
	// ******

	// FUNCTION BODY[0x7469f0]: 79
	// <0x746a01>|0x011|+0x003:'982'
	// <0>
	// <0x746a04>|0x014|+0x01a:'984'
	// <0>
	// <1>
	// <0x746a1e>|0x02e|+0x020:'987'
	// <0>
	// <1>
	// <0x746a3e>|0x04e|+0x02c:'990'
	// <0>
	// <1>
	// <2>
	// <0x746a6a>|0x07a|+0x037:'994'
	// <0>
	// <0x746aa1>|0x0b1|+0x1ce:'996'
	// <0>
	// <0x746c6f>|0x27f|-0x1af:'998'
	// <0>
	// <0x746ac0>|0x0d0|+0x003:'1000'
	// <0x746ac3>|0x0d3|+0x008:'1001'
	// <0>
	// <0x746acb>|0x0db|+0x002:'1003'
	// <0>
	// <0x746acd>|0x0dd|+0x016:'1005'
	// <0>
	// <1>
	// <0x746ae3>|0x0f3|+0x02a:'1008'
	// <0>
	// <1>
	// <2>
	// <0x746b0d>|0x11d|+0x025:'1012'
	// <0x746b32>|0x142|+0x03b:'1013'
	// <0>
	// <0x746b6d>|0x17d|+0x029:'1015'
	// <0x746b96>|0x1a6|+0x034:'1016'
	// <0>
	// <0x746bca>|0x1da|+0x028:'1018'
	// <0x746bf2>|0x202|+0x034:'1019'
	// <0>
	// <0x746c26>|0x236|+0x021:'1021'
	// <0>
	// <1>
	// <0x746c47>|0x257|+0x04f:'1024'
	// <0>
	// <1>
	// <0x746c96>|0x2a6|+0x024:'1027'
	// <0>
	// <0x746cba>|0x2ca|+0x031:'1029'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x746ceb>|0x2fb|+0x004:'1034'
	// <0x746cef>|0x2ff|+0x042:'1035'
	// <0>
	// <1>
	// <2>
	// <0x746d31>|0x341|+0x055:'1039'
	// <0>
	// <0x746d86>|0x396|+0x025:'1041'
	// <0x746dab>|0x3bb|+0x040:'1042'
	// <0>
	// <0x746deb>|0x3fb|+0x021:'1044'
	// <0>
	// <1>
	// <0x746e0c>|0x41c|+0x00d:'1047'
	// <0x746e19>|0x429|+0x00e:'1048'
	// <0x746e27>|0x437|+0x009:'1049'
	// <0>
	// <0x746e30>|0x440|+0x00e:'1051'
	// <0x746e3e>|0x44e|+0x00c:'1052'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x746e4a>|0x45a|+0x10e:'1060'
	// ******
}

// claude@NOTE: PARKED on the cooked player-parameters resource type. Recovered body:
// if (cook_data) DELETE(cook_data); take data[0].get_unmanaged_resource() as a
// resource carrying total-items-weight (off 0x124), used/free slot counts (off
// 0x154/0x155); store weight into m_player_total_items_weight; Invoke
// "root.player_profile.updateSlots" (2 uint args = slot counts) and
// "root.player_profile.updateWeight" (2 number args = weight, m_player_max_carried_
// weight). The resource type has no header yet (not profile_player_character, which
// is 8 bytes) - recover its name + accessors in the player-parameters cooker phase,
// then this fills in. Also byte-capped by the scaleform flash stubs.
void lobby_menu::player_parameters_ready( resources::queries_result& data, player_parameters_cooker_data* cook_data )
{
	VOSTOK_UNREFERENCED_PARAMETER	( data );
	VOSTOK_UNREFERENCED_PARAMETER	( cook_data );
}

void lobby_menu::on_profile_arrived( u8 profile_id )
{
	if ( m_selected_profile == profile_id )
	{
		flash_value profile_id_value;
		profile_id_value.SetUInt		( profile_id );
		m_lobby_menu_ui->movie->Invoke	( "_root.player_profile.selectProfile", NULL, &profile_id_value, 1 );
	}
}

// claude@NOTE: PARKED - navigates items_dictionary()'s private dict_config
// (["factions_dict"]["faction_%d"]["levels"]); items_dictionary::dict_config has no
// accessor (game_core class, can't edit here). Also scaleform flash /Od inline wall.
// Recover once items_dictionary exposes dict_config.
// STATE[STUB]
void lobby_menu::on_price_items_arrived( u8 trader_id )
{
	// LOCALS
	// configs::binary_config_value const& faction_levels
	// u8 								levels_count
	// lobby_client& 					lobby_client
	// char[32] 						faction_str
	// u8 								current_reputation_level
	// flash_value 						price_item_property
	// flash_value[5] 					current_level
	// wchar_t[512] 					faction_level_name_w
	// flash_value 						prices_array_item
	// ******

	// CALL SITE INFO
	// <0x744c32> -> lobby_client& < unknown >()
	// ******

	// FUNCTION BODY[0x744c10]: 55
	// <0x744c17>|0x007|+0x0c9:'1093'
	// <0x744ce0>|0x0d0|-0x0ac:'1093'
	// <0>
	// <1>
	// <0x744c34>|0x024|+0x020:'1096'
	// <0x744c54>|0x044|+0x042:'1097'
	// <0>
	// <0x744c96>|0x086|+0x026:'1099'
	// <0>
	// <1>
	// <2>
	// <0x744cbc>|0x0ac|+0x028:'1103'
	// <0>
	// <0x744ce4>|0x0d4|+0x015:'1105'
	// <0>
	// <0x744cf9>|0x0e9|+0x01c:'1107'
	// <0>
	// <0x744d15>|0x105|+0x02d:'1109'
	// <0>
	// <0x744d42>|0x132|+0x028:'1111'
	// <0>
	// <0x744d6a>|0x15a|+0x03f:'1113'
	// <0>
	// <1>
	// <0x744da9>|0x199|+0x01f:'1116'
	// <0x744dc8>|0x1b8|+0x083:'1117'
	// <0>
	// <0x744e4b>|0x23b|+0x019:'1119'
	// <0>
	// <0x744e64>|0x254|+0x058:'1121'
	// <0x744ebc>|0x2ac|-0x025:'1121'
	// <0>
	// <1>
	// <2>
	// <0x744e97>|0x287|+0x029:'1125'
	// <0>
	// <0x744ec0>|0x2b0|+0x013:'1127'
	// <0>
	// <0x744ed3>|0x2c3|+0x00f:'1129'
	// <0>
	// <1>
	// <2>
	// <0x744ee2>|0x2d2|+0x02d:'1133'
	// <0>
	// <0x744f0f>|0x2ff|+0x02c:'1135'
	// <0x744f3b>|0x32b|+0x038:'1136'
	// <0>
	// <0x744f73>|0x363|+0x028:'1138'
	// <0x744f9b>|0x38b|+0x03f:'1139'
	// <0>
	// <0x744fda>|0x3ca|+0x02d:'1141'
	// <0x745007>|0x3f7|+0x036:'1142'
	// <0>
	// <0x74503d>|0x42d|+0x015:'1144'
	// <0x745052>|0x442|+0x03a:'1145'
	// <0x74508c>|0x47c|+0x028:'1146'
	// <0x7450b4>|0x4a4|+0x073:'1147'
	// ******
}

void lobby_menu::reset_account_money( )
{
	flash_value account_info;
	m_lobby_menu_ui->movie->CreateObject( &account_info );

	wchar_t an[256];
	mbstowcs_s						( NULL, an, 256, lobby_client( ).account_name( ), _TRUNCATE );

	flash_value account_info_property;
	account_info_property.SetStringW( an );
	account_info.SetMember			( "nickname", account_info_property );

	account_info_property.SetUInt	( lobby_client( ).get_account_money( ).generic_money );
	account_info.SetMember			( "generic_money", account_info_property );

	account_info_property.SetUInt	( lobby_client( ).get_account_money( ).premium_money );
	account_info.SetMember			( "premium_money", account_info_property );

	m_lobby_menu_ui->movie->Invoke	( "root.setPlayerInfo", NULL, &account_info, 1 );
}

void lobby_menu::on_shop_ui_ready( )
{
	for ( u32 trader_id = 1; trader_id <= 3; ++trader_id )
		lobby_client( ).query_prices( trader_id );
}

// claude@NOTE: PARKED - large config-driven builder navigating items_dictionary()'s
// private dict_config binary_config (skills tree / boosters / perks tables);
// items_dictionary::dict_config has no accessor (game_core class, can't edit here). Also
// scaleform flash /Od inline wall. Recover once items_dictionary exposes dict_config.
// STATE[STUB]
void lobby_menu::fill_skills_tree( )
{
	// LOCALS
	// configs::binary_config_value const& tree
	// configs::binary_config_value const& db_dictionaries
	// flash_value 						skills_tree_value_prop
	// flash_value 						skills_tree_value
	// wchar_t[512] 					branch_descr
	// char[32] 						skill_branch_name
	// u32 								levels_count
	// wchar_t[512] 					branch_name
	// u8 								skill_id
	// flash_value 						skills_tree_level_value_prop
	// configs::binary_config_value const& current_skill_value
	// char[32] 						skill_name
	// u32 								i
	// configs::binary_config_value const* booster_it_e
	// flash_value 						skills_tree_level_value
	// flash_value 						item_property_member
	// configs::binary_config_value const& current_level_cfg
	// char[32] 						skill_level_table_name
	// u32 								booster_index
	// char[32] 						prop_value_str
	// char[32] 						booster_table_name
	// wchar_t[512] 					prop_name
	// float 							prop_value
	// flash_value 						item_property
	// flash_value 						perk_property_member
	// configs::binary_config_value const* perk_it_e
	// configs::binary_config_value const* perk_it
	// u32 								perk_index
	// wchar_t[512] 					perk_name
	// flash_value 						perk_property
	// char[32] 						perk_table_name
	// wchar_t[512] 					perk_descr
	// ******

	// FUNCTION BODY[0x743cd0]: 156
	// <0x743cda>|0x00a|+0x022:'1184'
	// <0>
	// <0x743cfc>|0x02c|+0x01f:'1186'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x743d1b>|0x04b|+0x024:'1191'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x743d3f>|0x06f|+0x019:'1198'
	// <0>
	// <0x743d58>|0x088|+0x019:'1200'
	// <0>
	// <0x743d71>|0x0a1|+0x015:'1202'
	// <0>
	// <1>
	// <0x743d86>|0x0b6|+0x031:'1205'
	// <0>
	// <1>
	// <0x743db7>|0x0e7|+0x01e:'1208'
	// <0>
	// <1>
	// <2>
	// <0x743dd5>|0x105|+0x045:'1212'
	// <0>
	// <0x743e1a>|0x14a|+0x06e:'1214'
	// <0x743e88>|0x1b8|+0x037:'1215'
	// <0>
	// <1>
	// <0x743ebf>|0x1ef|+0x045:'1218'
	// <0>
	// <0x743f04>|0x234|+0x06e:'1220'
	// <0x743f72>|0x2a2|+0x037:'1221'
	// <0>
	// <0x743fa9>|0x2d9|+0x050:'1223'
	// <0x743ff9>|0x329|+0x041:'1224'
	// <0>
	// <0x74403a>|0x36a|+0x028:'1226'
	// <0x744062>|0x392|+0x042:'1227'
	// <0>
	// <0x7440a4>|0x3d4|+0x024:'1229'
	// <0x7440c8>|0x3f8|+0x03f:'1230'
	// <0>
	// <0x744107>|0x437|+0x019:'1232'
	// <0>
	// <1>
	// <2>
	// <0x744120>|0x450|+0x038:'1236'
	// <0x744158>|0x488|+0x9b4:'1237'
	// <0x744b0c>|0xe3c|-0x993:'1237'
	// <0>
	// <1>
	// <0x744179>|0x4a9|+0x019:'1240'
	// <0x744192>|0x4c2|+0x01b:'1241'
	// <0>
	// <1>
	// <0x7441ad>|0x4dd|+0x036:'1244'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x7441e3>|0x513|+0x083:'1249'
	// <0x744266>|0x596|+0x03a:'1250'
	// <0>
	// <0x7442a0>|0x5d0|+0x00c:'1252'
	// <0>
	// <0x7442ac>|0x5dc|+0x039:'1254'
	// <0x7442e5>|0x615|+0x04d:'1255'
	// <0>
	// <0x744332>|0x662|+0x01c:'1257'
	// <0>
	// <0x74434e>|0x67e|+0x00e:'1259'
	// <0x74435c>|0x68c|+0x01c:'1260'
	// <0>
	// <0x744378>|0x6a8|+0x008:'1262'
	// <0>
	// <0x744380>|0x6b0|+0x2fe:'1264'
	// <0x74467e>|0x9ae|-0x2ee:'1264'
	// <0>
	// <1>
	// <0x744390>|0x6c0|+0x024:'1267'
	// <0>
	// <0x7443b4>|0x6e4|+0x01d:'1269'
	// <0x7443d1>|0x701|+0x015:'1270'
	// <0>
	// <1>
	// <0x7443e6>|0x716|+0x017:'1273'
	// <0>
	// <1>
	// <0x7443fd>|0x72d|+0x045:'1276'
	// <0>
	// <1>
	// <0x744442>|0x772|+0x022:'1279'
	// <0>
	// <0x744464>|0x794|+0x074:'1281'
	// <0x7444d8>|0x808|+0x02e:'1282'
	// <0>
	// <0x744506>|0x836|+0x06e:'1284'
	// <0x744574>|0x8a4|+0x02c:'1285'
	// <0>
	// <0x7445a0>|0x8d0|+0x050:'1287'
	// <0x7445f0>|0x920|+0x03d:'1288'
	// <0>
	// <0x74462d>|0x95d|+0x020:'1290'
	// <0x74464d>|0x97d|+0x038:'1291'
	// <0>
	// <0x744685>|0x9b5|+0x03a:'1293'
	// <0x7446bf>|0x9ef|+0x014:'1294'
	// <0x7446d3>|0xa03|+0x01c:'1295'
	// <0>
	// <0x7446ef>|0xa1f|+0x00e:'1297'
	// <0x7446fd>|0xa2d|+0x023:'1298'
	// <0>
	// <0x744720>|0xa50|+0x008:'1300'
	// <0>
	// <0x744728>|0xa58|+0x01c:'1302'
	// <0>
	// <1>
	// <0x744744>|0xa74|+0x024:'1305'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x744768>|0xa98|+0x00e:'1310'
	// <0>
	// <1>
	// <0x744776>|0xaa6|+0x012:'1313'
	// <0>
	// <1>
	// <0x744788>|0xab8|+0x048:'1316'
	// <0>
	// <1>
	// <0x7447d0>|0xb00|+0x045:'1319'
	// <0>
	// <0x744815>|0xb45|+0x083:'1321'
	// <0x744898>|0xbc8|+0x02e:'1322'
	// <0>
	// <0x7448c6>|0xbf6|+0x07e:'1324'
	// <0x744944>|0xc74|+0x02c:'1325'
	// <0>
	// <0x744970>|0xca0|+0x024:'1327'
	// <0x744994>|0xcc4|+0x03a:'1328'
	// <0>
	// <0x7449ce>|0xcfe|+0x020:'1330'
	// <0x7449ee>|0xd1e|+0x03c:'1331'
	// <0x744a2a>|0xd5a|+0x03a:'1332'
	// <0x744a64>|0xd94|+0x021:'1333'
	// <0x744a85>|0xdb5|+0x020:'1334'
	// <0x744aa5>|0xdd5|+0x06c:'1335'
	// <0x744b11>|0xe41|+0x037:'1336'
	// <0>
	// <0x744b48>|0xe78|+0x023:'1338'
	// <0x744b6b>|0xe9b|+0x07b:'1339'
	// ******
}

// claude@NOTE: flash glue now inlines at /Ox (scaleform Master Gold /GL); residual is
// LTCG scheduling of the inlined flash_value ctor/dtor. The "trees" member
// (per-skill perk grouping, total_points_in_tree) + "perks" array are reconstructed
// approximately; the skills array, the leveling-info members (points_available,
// points_unlocked, experience_current/next_level/delta) and the fill_char_info Invoke
// are faithful.
void lobby_menu::fill_character_data( )
{
	flash_value player_skills_value;
	m_lobby_menu_ui->movie->CreateObject( &player_skills_value );

	flash_value player_skills_value_prop;
	flash_value skill_value;
	flash_value skill_value_prop;
	flash_value perk_value;

	for ( u8 i = 0; i < lobby_client( ).player_skills_count( ); ++i )
	{
		m_lobby_menu_ui->movie->CreateObject( &skill_value );

		skill_value_prop.SetUInt		( lobby_client( ).player_skill( i ).skill_id );
		skill_value.SetMember			( "id", skill_value_prop );

		skill_value_prop.SetUInt		( lobby_client( ).player_skill( i ).skill_points );
		skill_value.SetMember			( "points", skill_value_prop );

		u8 total_points_in_tree = 0;
		skill_value_prop.SetUInt		( total_points_in_tree );
		skill_value.SetMember			( "trees", skill_value_prop );

		player_skills_value.SetElement	( i, skill_value );
	}
	player_skills_value.SetMember		( "skills", player_skills_value );

	player_leveling_info const& leveling = lobby_client( ).get_player_leveling( );

	player_skills_value_prop.SetUInt	( leveling.total_skill_points );
	player_skills_value.SetMember		( "points_available", player_skills_value_prop );

	player_skills_value_prop.SetUInt	( leveling.total_skill_points );
	player_skills_value.SetMember		( "points_unlocked", player_skills_value_prop );

	player_skills_value_prop.SetUInt	( leveling.total_experience );
	player_skills_value.SetMember		( "experience_current", player_skills_value_prop );

	player_skills_value_prop.SetUInt	( leveling.next_level_experience );
	player_skills_value.SetMember		( "experience_next_level", player_skills_value_prop );

	player_skills_value_prop.SetUInt	( leveling.next_level_experience - leveling.prev_level_experience );
	player_skills_value.SetMember		( "experience_delta", player_skills_value_prop );

	for ( u8 i = 0; i < lobby_client( ).player_perks_count( ); ++i )
	{
		perk_value.SetUInt				( lobby_client( ).player_perk( i ) );
		player_skills_value.SetElement	( i, perk_value );
	}
	player_skills_value.SetMember		( "perks", player_skills_value );

	m_lobby_menu_ui->movie->Invoke		( "root.fill_char_info", NULL, &player_skills_value, 1 );
}

void lobby_menu::fill_service_prices( )
{
	flash_value reroll_cost_value;
	reroll_cost_value.SetUInt		( lobby_client( ).get_service_prices( ).reroll_cost );
	m_lobby_menu_ui->movie->Invoke	( "root.set_reroll_cost", NULL, &reroll_cost_value, 1 );
}

void lobby_menu::fill_friend_list( )
{
	vectora< account_list_item > const& players_list = messaging_client( ).get_friend_list( );

	flash_value array_value;
	m_lobby_menu_ui->movie->CreateArray	( &array_value );

	for ( u32 i = 0; i < players_list.size( ); ++i )
	{
		flash_value list_item;
		m_lobby_menu_ui->movie->CreateObject( &list_item );

		flash_value value;
		value.SetUInt					( players_list[ i ].account_id );
		list_item.SetMember				( "id", value );

		wchar_t player_name_w[512];
		mbstowcs_s						( NULL, player_name_w, 512, players_list[ i ].account_name.c_str( ), _TRUNCATE );
		value.SetStringW				( player_name_w );
		list_item.SetMember				( "name", value );

		value.SetUInt					( 3 );
		list_item.SetMember				( "icon", value );

		value.SetUInt					( players_list[ i ].online ? 0 : 2 );
		list_item.SetMember				( "status", value );

		array_value.SetElement			( i, list_item );
	}

	m_lobby_menu_ui->movie->Invoke		( "root.set_friends_list", NULL, &array_value, 1 );
}

void lobby_menu::fill_ignore_list( )
{
	flash_value array_value;
	m_lobby_menu_ui->movie->CreateArray	( &array_value );

	for ( u32 i = 0; i < messaging_client( ).get_ignore_list( ).size( ); ++i )
	{
		flash_value list_item;
		m_lobby_menu_ui->movie->CreateObject( &list_item );

		flash_value value;
		value.SetUInt					( messaging_client( ).get_ignore_list( )[ i ].account_id );
		list_item.SetMember				( "id", value );

		value.SetString					( messaging_client( ).get_ignore_list( )[ i ].account_name.c_str( ) );
		list_item.SetMember				( "name", value );

		value.SetUInt					( 3 );
		list_item.SetMember				( "icon", value );

		array_value.SetElement			( i, list_item );
	}

	m_lobby_menu_ui->movie->Invoke		( "root.set_ignored_list", NULL, &array_value, 1 );
}

void lobby_menu::fill_found_players( )
{
	flash_value array_value;
	m_lobby_menu_ui->movie->CreateArray	( &array_value );

	const u32 count = messaging_client( ).get_found_players_list( ).size( );
	for ( u32 i = 0; i < count; ++i )
	{
		flash_value list_item;
		m_lobby_menu_ui->movie->CreateObject( &list_item );

		flash_value value;
		value.SetUInt					( messaging_client( ).get_found_players_list( )[ i ].account_id );
		list_item.SetMember				( "id", value );

		value.SetString					( messaging_client( ).get_found_players_list( )[ i ].account_name.c_str( ) );
		list_item.SetMember				( "name", value );

		array_value.SetElement			( i, list_item );
	}

	m_lobby_menu_ui->movie->Invoke		( "root.fill_players_search", NULL, &array_value, 1 );
}

void lobby_menu::set_cursor( u8 id )
{
	flash_value c_id;
	c_id.SetUInt					( id );
	m_cursor_ui->movie->Invoke		( "root.setCursor", NULL, &c_id, 1 );
}

void lobby_menu::set_fps_stats( float fps )
{
	flash_value f_val;
	f_val.SetUInt					( ( u16 )fps );
	m_lobby_menu_ui->movie->Invoke	( "root.set_fps", NULL, &f_val, 1 );
}

bool lobby_menu::is_mouse_over_ui( )
{
	flash_value is_mouse_over_val;
	m_lobby_menu_ui->movie->Invoke					( "root.get_mouse_over", &is_mouse_over_val, NULL, 0 );
	bool is_over_lobby = is_mouse_over_val.GetBool	( );
	get_game( ).get_chat_handler( ).get_movie( )->movie->Invoke( "root.get_mouse_over", &is_mouse_over_val, NULL, 0 );
	return is_over_lobby || is_mouse_over_val.GetBool( );
}

// claude@NOTE: flash glue now inlines at /Ox (scaleform Master Gold /GL); structure
// (the three independent message blocks) is faithful and the residual is LTCG
// scheduling of the inlined flash_value ctor/dtor + SetStringW/SetMember/SetUInt, not
// the old inline-vs-call wall.
// sushi@TODO: the L"joined"/L"left"/L"in_queue"/L"team" wcsstr prefix literals are
// length-matched guesses (loaded from rdata; do not affect this function's bytes), exact
// keys unverified.
void lobby_menu::on_match_message_arrived( wchar_t const* w_text )
{
	wchar_t const* player_joined_message	= wcsstr( w_text, L"joined" );
	wchar_t const* player_left_message		= wcsstr( w_text, L"joined" );
	wchar_t const* queue_state_message		= wcsstr( w_text, L"in_queue" );

	wchar_t w_player_name[32];
	flash_value player_name_val;

	if ( player_joined_message )
	{
		wcsncpy_s			( w_player_name, player_joined_message + 6, ( wcsstr( player_joined_message, L" =" ) - player_joined_message ) / 2 - 6 );

		wchar_t const* player_team = wcsstr( w_text, L"team" );
		wchar_t w_player_team[8];
		wcsncpy_s			( w_player_team, player_team + 4, ( wcsstr( player_team, L"=" ) - player_team ) / 2 - 4 );

		game_team_id team = ( game_team_id )_wtoi( w_player_team );

		flash_value add_player_args[2];
		m_match_making_ui->movie->CreateObject( &add_player_args[1] );

		player_name_val.SetStringW	( w_player_name );
		add_player_args[1].SetMember( "name", player_name_val );

		flash_value player_member_value;
		player_member_value.SetUInt	( team );
		add_player_args[1].SetMember( "icon", player_member_value );

		add_player_args[0].SetUInt	( team );
		m_match_making_ui->movie->Invoke( "root.add_player", NULL, add_player_args, 2 );
	}

	if ( player_left_message )
	{
		wcsncpy_s			( w_player_name, player_left_message + 6, ( wcsstr( player_left_message, L" =" ) - player_left_message ) / 2 - 6 );

		player_name_val.SetStringW	( w_player_name );
		m_match_making_ui->movie->Invoke( "root.remove_player", NULL, &player_name_val, 1 );
	}

	if ( queue_state_message )
	{
		wchar_t w_player_in_queue[16];
		wcsncpy_s			( w_player_in_queue, queue_state_message + 4, ( wcsstr( queue_state_message, L"=" ) - queue_state_message ) / 2 - 4 );

		flash_value players_in_queue_val;
		players_in_queue_val.SetStringW	( w_player_in_queue );
		m_match_making_ui->movie->Invoke( "root.set_place", NULL, &players_in_queue_val, 1 );
	}
}

// claude@NOTE: chat_handler now headered, so the parse + add_message path is recovered.
// The wcsstr prefix literals (L"player_id"/L"player_exp"/L"player_count" + the L" ="/L"="
// terminators) are length-matched guesses (relocated rdata, do not affect bytes). Residual
// is the scaleform flash /Od inline wall (set_games_online Invoke + flash_value ctor/dtor)
// and LTCG scheduling.
void lobby_menu::on_stats_message_arrived(
	wchar_t const*						w_text,
	wchar_t const*						w_sender_name,
	messaging::message_channel_enum		message_channel
)
{
	wchar_t const* player_id	= wcsstr( w_text, L"player_id" );
	wchar_t const* player_exp	= wcsstr( w_text, L"player_exp" );
	wcsstr( w_text, L"match_count" );
	wchar_t const* player_count	= wcsstr( w_text, L"player_count" );

	if ( player_id )
	{
		wchar_t w_player_id[32];
		wcsncpy_s			( w_player_id, player_id + 9, ( wcsstr( player_id, L" =" ) - player_id ) / 2 - 9 );

		char player_name[32];
		wcstombs_s			( NULL, player_name, 32, w_player_id, _TRUNCATE );

		if ( strings::equal( player_name, lobby_client( ).account_name( ) ) )
		{
			get_game( ).get_chat_handler( ).add_message( message_channel, w_text, w_sender_name );

			if ( player_exp )
			{
				wchar_t w_player_exp[32];
				wcsncpy_s	( w_player_exp, player_exp + 4, ( wcsstr( player_exp, L"=" ) - player_exp ) / 2 - 4 );

				m_match_stats.last_match_exp_delta = _wtoi( w_player_exp );

				if ( lobby_client( ).net_connected( ) )
				{
					lobby_client( ).query_client_status( lobby::q_account_money );	// 7
					lobby_client( ).query_client_status( lobby::q_player_skills );	// 8
				}
			}

			get_game( ).get_chat_handler( ).add_message( message_channel, w_text, w_sender_name );
		}
	}
	else if ( player_count )
	{
		wchar_t w_player_count[8];
		wcsncpy_s			( w_player_count, player_count + 5, ( wcsstr( player_count, L"=" ) - player_count ) / 2 - 5 );

		flash_value player_count_val;
		player_count_val.SetStringW		( w_player_count );
		m_lobby_menu_ui->movie->Invoke	( "root.set_games_online", NULL, &player_count_val, 1 );
	}
}

void lobby_menu::show_disconnected_message( bool b_show )
{
	if ( b_show )
	{
		flash_value v[4];
		v[0].SetUInt					( 13 );
		v[1].SetString					( "noclose" );

		wchar_t message_txt[512];
		get_game( ).text_translator( ).translate_text( "st_disconnected_from_lobby", message_txt );
		v[2].SetStringW					( message_txt );
		v[3].SetBoolean					( true );

		m_message_ui->movie->Invoke		( "root.showMessage", NULL, v, 4 );
	}
	else
	{
		flash_value window_id;
		window_id.SetUInt				( 13 );
		m_message_ui->movie->Invoke		( "root.close", NULL, &window_id, 1 );
	}
}

// claude@NOTE: config part PARKED on items_dictionary::dict_config being private with
// no accessor (game_core, can't edit here). Recovered body: per reputation_id in
// lobby_client().get_player_reputations_count(): faction_str = sprintf("faction_%d",
// reputation.faction_id); faction_levels = items_dictionary().dict_config->get_root()
// ["factions_dict"][faction_str]["levels"]; walk faction_levels, player_reputation_
// level = highest level i where reputation_points >= levels[i]["value"]; build
// player_progress_args[3] (faction/level/points uints) and Invoke
// "root.setup_player_progress". Restore the config nav once items_dictionary exposes
// dict_config (or lobby_menu is friended). Also byte-capped by the scaleform stubs.
void lobby_menu::on_player_reputations_arrived( )
{
	for ( u8 reputation_id = 0; reputation_id < lobby_client( ).get_player_reputations_count( ); ++reputation_id )
	{
		char faction_str[32];
		sprintf_s						( faction_str, "faction_%d", lobby_client( ).get_player_reputation( reputation_id ).faction_id );

		u8 player_reputation_level = 0;

		flash_value player_progress_args[3];
		player_progress_args[0].SetUInt	( reputation_id );
		player_progress_args[1].SetUInt	( player_reputation_level );
		player_progress_args[2].SetUInt	( lobby_client( ).get_player_reputation( reputation_id ).reputation_points );
		m_lobby_menu_ui->movie->Invoke	( "root.setup_player_progress", NULL, player_progress_args, 3 );
	}
}

void lobby_menu::set_ping( u32 ping_val )
{
	flash_value args; args.SetUInt( ping_val ); m_lobby_menu_ui->movie->Invoke( "root.set_ping", NULL, &args, 1 );
}

// /OPT:REF anchor hook for the TU-local relocate_item_func (no public header);
// see anchor_game_lobby_ui.cpp.
void use_game_relocate_item_func( game& g )
{
	static volatile bool s_run = false;
	if( !s_run )
		return;

	relocate_item_func func( g );
	func.call( *( flash_function_handler_params* )NULL );
}

} // namespace survarium
