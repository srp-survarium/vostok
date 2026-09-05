// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "game_options.h"

#include "base_game_scene.h"
#include "base_network_client.h"
#include "game.h"
#include "game_action_descr.h"
#include "key_binder.h"
#include "options_tab.h"
#include "options_item_base.h"
#include "text_translator.h"

#include "graphic_preset.h"
#include "options_name_to_label.h"
#include "main_menu_button_name_to_action.h"
#include "key_bind_descr.h"

#include <vostok/console_command.h>
#include <vostok/console_command_processor.h>
#include <vostok/strings_concatenations.h>
#include <vostok/strings_functions.h>

#include <vostok/scaleform/sources/flash_movie.h>
#include <vostok/scaleform/sources/flash_value.h>
#include <vostok/input/world.h>
#include <vostok/math_constants.h>
#include <vostok/math_functions.h>
#include <vostok/math_uint2.h>
#include <vostok/resources.h>
#include <vostok/resources_classes.h>
#include <vostok/resources_queries_result.h>
#include <boost/bind.hpp>

namespace survarium {

graphic_preset default_graphic_preset[10] =
{
	{ texture_quality_option, 4 }, { geometry_quality_option, 4 },
	{ shadow_quality_option, 4 }, { lightning_quality_option, 4 },
	{ shading_quality_option, 4 }, { decorations_option, 4 },
	{ post_process_option, 4 }, { ambient_occlusion_option, 4 },
	{ particles_quality_option, 4 }, { motion_blur_amount_option, 4 },
};

key_bind_descr key_bind_descriptions[33] =
{
	{ kJUMP, "st_jump_action", game_type, unlocked_action },
	{ kCROUCH, "st_crouch_action", game_type, unlocked_action },
	{ kACCEL, "st_sprint_action", game_type, unlocked_action },
	{ kSET_NEXT_FIRE_QUEUE_TYPE, "st_next_fire_queue_action", game_type, unlocked_action },
	{ kSET_NEXT_AMMO_TYPE, "st_next_ammo_action", game_type, unlocked_action },
	{ kFWD, "st_move_forward_action", game_type, unlocked_action },
	{ kBACK, "st_move_backward_action", game_type, unlocked_action },
	{ kL_STRAFE, "st_left_strafe_action", game_type, unlocked_action },
	{ kR_STRAFE, "st_right_strafe_action", game_type, unlocked_action },
	{ kWPN_1, "st_first_weapon_action", game_type, unlocked_action },
	{ kWPN_2, "st_second_weapon_action", game_type, unlocked_action },
	{ kWPN_FIRE, "st_weapon_fire_action", game_type, unlocked_action },
	{ kWPN_RELOAD, "st_weapon_reload_action", game_type, unlocked_action },
	{ kWPN_AIM, "st_weapon_aim_action", game_type, unlocked_action },
	{ kDELAY_BREATH, "st_dealy_breath_action", game_type, unlocked_action },
	{ kQUICK_USE_1, "st_quick_use_action_1", game_type, unlocked_action },
	{ kQUICK_USE_2, "st_quick_use_action_2", game_type, unlocked_action },
	{ kQUICK_USE_3, "st_quick_use_action_3", game_type, unlocked_action },
	{ kQUICK_USE_4, "st_quick_use_action_4", game_type, unlocked_action },
	{ kQUICK_USE_5, "st_quick_use_action_5", game_type, unlocked_action },
	{ kQUICK_USE_6, "st_quick_use_action_6", game_type, unlocked_action },
	{ kBACK_SLOT_USE, "st_back_use_action", game_type, unlocked_action },
	{ kCHAT, "st_start_chat_action", game_type, unlocked_action },
	{ kUSE, "st_use_action", game_type, unlocked_action },
	{ kDROP, "st_drop_action", game_type, unlocked_action },
	{ kCHARACTER, "st_show_character_action", lobby_type, unlocked_action },
	{ kINVENTORY, "st_show_inventory_action", lobby_type, unlocked_action },
	{ kSHOP, "st_show_shop_action", lobby_type, unlocked_action },
	{ kOPTIONS, "st_show_options_action", lobby_type, unlocked_action },
	{ kFRIENDS, "st_show_friends_action", lobby_type, unlocked_action },
	{ kSEND_MESSAGE, "st_send_message_action", chat_type, locked_action },
	{ kSELECT_SEND_TO, "st_select_send_to_action", chat_type, locked_action },
	{ kPTT, "st_ptt_action", chat_type, locked_action },
};

void game_options::apply_default_graphic( )
{
	for ( u32 i = 0; i < 10; ++i )
	{
		flash_value new_resolution_data[4];
		new_resolution_data[0].SetUInt( 2 );
		new_resolution_data[1].SetUInt( default_graphic_preset[i].option_id );
		new_resolution_data[2].SetUInt( default_graphic_preset[i].option_value );
		new_resolution_data[3].SetUInt( 0 );

		m_options_ui->movie->Invoke( "root.set_value", NULL, new_resolution_data, 4 );
	}
}

void game_options::callback(
	flash_movie*			pmovieView,
	pcstr					methodName,
	flash_value const*		args,
	u32						argCount
)
{
	if ( strings::equal( methodName, "menu_button" ) )
	{
		pcstr button = args[0].GetString( );

		if ( strings::equal( button, "back" ) )
			m_game.deactivate_main_menu( );
		else if ( strings::equal( button, "exit_to_os" ) )
			m_game.exit( "quit" );
		else if ( strings::equal( button, "settings" ) )
			show_options( true );
		else if ( strings::equal( button, "leave_match" ) )
			m_game.get_network_client( )->close_current_match( true );
	}
	else if ( strings::equal( methodName, "accept_changes" ) )
	{
		u32 tab_id = args[0].GetInt( );

		if ( tab_id == 1 )
			apply_key_bindings( );

		m_options[tab_id]->apply( m_options_ui );
	}
	else if ( strings::equal( methodName, "cancel_changes" ) )
	{
		u32 tab_id = args[0].GetInt( );

		m_options[tab_id]->revert( m_options_ui );

		if ( tab_id == 1 )
			reset_bindings( true );
	}
	else if ( strings::equal( methodName, "button_optimal_video" ) )
	{
	}
	else if ( strings::equal( methodName, "button_default_video" ) )
		apply_default_graphic( );
	else if ( strings::equal( methodName, "button_default_controls" ) )
		reset_bindings_to_defaults( );
	else if ( strings::equal( methodName, "start_bind_key" ) )
	{
		m_waiting_for_bind_action = game_action_id( args[0].GetInt( ) );
		m_parent_scene->hide_movie( m_cursor_ui );
	}
	else if ( strings::equal( methodName, "reassign_ok_clicked" ) )
	{
		assign_binding( m_conflicted_action_to_bind, m_conflicted_key_name );

		for ( vector< game_action_id >::iterator it = m_conflicted_action_ids.begin( ); it != m_conflicted_action_ids.end( ); ++it )
			assign_binding( *it, "" );
	}
}

void game_options::show_options( bool b_val )
{
	flash_value b_show_value;
	b_show_value.SetBoolean( b_val );
	m_options_ui->movie->Invoke( "root.show_settings", NULL, &b_show_value, 1 );
}

game_options::game_options( game& g ) :
	m_game( g ),
	m_mouse_pos( 0, 0 ),
	m_waiting_for_bind_action( kLASTACTION )
{
}

game_options::~game_options( )
{
	for ( u32 i = 0; i < 4; ++i )
		VOSTOK_DELETE_IMPL( ::survarium::g_allocator, m_options[i] );
}

void game_options::initialize( )
{
	for ( u32 i = 0; i < options_count; ++i )
		m_options[i] = VOSTOK_NEW_IMPL( ::survarium::g_allocator, options_tab )( m_game, m_options_ui, options_enum( i ) );

	resources::request requests[2];
	requests[0].set( "resources/flash_movies/main_menu.swf", resources::flash_movie_class );
	requests[1].set( "resources/flash_movies/cursor.swf", resources::flash_movie_class );

	resources::query_resources( requests, boost::bind( &game_options::on_resources_ready, this, _1 ), g_allocator );
}

void game_options::on_resources_ready( resources::queries_result& data )
{
	m_options_ui = static_cast_resource_ptr< flash_movie_resource_ptr >( data[0].get_unmanaged_resource( ) );

	m_options_ui->movie->SetBackgroundAlpha( 0.f );
	m_options_ui->movie->SetViewAlignment( flash_movie::Align_TopLeft );
	m_options_ui->movie->SetViewScaleMode( flash_movie::SM_NoScale );

	m_cursor_ui = static_cast_resource_ptr< flash_movie_resource_ptr >( data[1].get_unmanaged_resource( ) );

	m_cursor_ui->movie->SetBackgroundAlpha( 0.f );
	m_cursor_ui->movie->SetViewAlignment( flash_movie::Align_TopLeft );
	m_cursor_ui->movie->SetViewScaleMode( flash_movie::SM_NoScale );

	m_options_ui->movie->SetExternalInterface( this );

	fill_labels( );
	fill_settings_data( );

	for ( u32 i = 0; i < options_count; ++i )
		m_options[i]->initialize_data( m_options_ui );

	initialize_bindings( );
}

void game_options::fill_labels( )
{
	options_name_to_label names_to_label[] = {
		{ "s_friends",					"st_options_friends"			},
		{ "s_cross",					"st_options_cross_type"			},
		{ "s_chat",						"st_options_chat"				},
		{ "s_lobby",					"st_options_lobby"				},
		{ "s_game",						"st_options_game"				},
		{ "s_chat_keys",				"st_options_chat_keys"			},
		{ "gameplay_options_type",		"st_options_gameplay"			},
		{ "controllers_options_type",	"st_options_controller"			},
		{ "video_options_type",			"st_options_video"				},
		{ "sound_options_type",			"st_options_sound"				},
		{ "adjust_gamma",				"st_monitor_ajust"				},
		{ "button_default_video",		"st_options_default_video"		},
		{ "button_default_controls",	"st_options_default_controls"	},
		{ "button_optimal_video",		"st_options_optimal_video"		},
		{ "button_ok",					"st_options_ok"					},
		{ "button_apply",				"st_options_apply"				},
		{ "button_cancel",				"st_options_cancel"				},
		{ "apply_changes_msg",			"st_apply_changes_msg"			},
		{ "s_voice_chat",				"st_options_voice"				},
		{ "s_sound_volume",				"st_options_volume"				},
		{ "bind_text",					"st_bind_text"					},
		{ "s_minimap",					"st_minimap"					},
	};

	flash_value labels_array;
	m_options_ui->movie->CreateArray( &labels_array );

	for ( u32 i = 0; i < 22; ++i )
	{
		flash_value label;
		m_options_ui->movie->CreateObject( &label );

		flash_value label_member;

		label_member.SetString( names_to_label[i].name.c_str( ) );
		label.SetMember( "name", label_member );

		wchar_t label_txt[512];
		m_game.text_translator( ).translate_text( names_to_label[i].label.c_str( ), label_txt );
		label_member.SetStringW( label_txt );

		label.SetMember( "label", label_member );

		labels_array.PushBack( label );
	}
	m_options_ui->movie->Invoke( "root.set_labels", NULL, &labels_array, 1 );
}

void game_options::fill_menu_buttons( bool in_game_world )
{
	main_menu_button_name_to_action name_to_action_in_game_world[] = {
		{ "st_mm_button_back",			"back"			},
		{ "st_mm_button_settings",		"settings"		},
		{ "st_mm_button_leave_match",	"leave_match"	},
		{ "st_mm_button_exit_to_os",	"exit_to_os"	},
	};


	main_menu_button_name_to_action name_to_action_in_lobby_menu[] = {
		{ "st_mm_button_back",			"back"			},
		{ "st_mm_button_settings",		"settings"		},
		{ "st_mm_button_exit_to_os",	"exit_to_os"	},
	};

	flash_value buttons_array;
	m_options_ui->movie->CreateArray( &buttons_array );

	main_menu_button_name_to_action* name_to_action;
	u32 buttons_count;

	if ( in_game_world )
	{
		name_to_action = name_to_action_in_game_world;
		buttons_count = 4;
	}
	else
	{
		name_to_action = name_to_action_in_lobby_menu;
		buttons_count = 3;
	}

	for ( u32 i = 0; i < buttons_count; ++i )
	{
		flash_value button;
		m_options_ui->movie->CreateObject( &button );

		flash_value button_member;

		button_member.SetString( name_to_action[i].action.c_str( ) );
		button.SetMember( "action", button_member );

		wchar_t button_txt[512];
		m_game.text_translator( ).translate_text( name_to_action[i].name.c_str( ), button_txt );
		button_member.SetStringW( button_txt );

		button.SetMember( "label", button_member );

		buttons_array.PushBack( button );
	}
	m_options_ui->movie->Invoke( "root.set_options", NULL, &buttons_array, 1 );
}

void game_options::fill_settings_data( )
{
	fixed_string< 64 >* gameplay_options_labels = VOSTOK_NEW_ARRAY_IMPL( ::survarium::g_allocator, fixed_string< 64 >, 9 );
	gameplay_options_labels[0] = "st_invite_from_friends_option";
	gameplay_options_labels[1] = "st_friends_signin_notification_option";
	gameplay_options_labels[2] = "st_messages_censor_option";
	gameplay_options_labels[3] = "st_messages_only_from_friends_option";
	gameplay_options_labels[4] = "st_private_messages_in_game_option";
	gameplay_options_labels[5] = "st_hide_spam_option";
	gameplay_options_labels[6] = "st_crosshair_type_option";
	gameplay_options_labels[7] = "st_crosshair_static_option";
	gameplay_options_labels[8] = "st_minimap_rotable_option";

	fixed_string< 64 >* video_options_labels = VOSTOK_NEW_ARRAY_IMPL( ::survarium::g_allocator, fixed_string< 64 >, 19 );
	video_options_labels[0] = "st_monitor_count_option";
	video_options_labels[1] = "st_resolution_option";
	video_options_labels[2] = "st_fullscreen_option";
	video_options_labels[3] = "st_vsync_option";
	video_options_labels[4] = "st_antialiasing_option";
	video_options_labels[5] = "st_aniso_filtering_option";
	video_options_labels[6] = "st_options_adjust_gamma";
	video_options_labels[7] = "st_fov_option";
	video_options_labels[8] = "st_graphics_quality_option";
	video_options_labels[9] = "st_texture_quality_option";
	video_options_labels[10] = "st_geometry_quality_option";
	video_options_labels[11] = "st_shadow_quality_option";
	video_options_labels[12] = "st_lightning_quality_option";
	video_options_labels[13] = "st_shading_quality_option";
	video_options_labels[14] = "st_decorations_options";
	video_options_labels[15] = "st_post_process_options";
	video_options_labels[16] = "st_ambient_occlusion_options";
	video_options_labels[17] = "st_particles_quality_options";
	video_options_labels[18] = "st_motion_blur_amount_options";

	fixed_string< 64 >* sound_options_labels = VOSTOK_NEW_ARRAY_IMPL( ::survarium::g_allocator, fixed_string< 64 >, 7 );
	sound_options_labels[0] = "st_general_volume_option";
	sound_options_labels[1] = "st_ingame_volume_option";
	sound_options_labels[2] = "st_music_volume_option";
	sound_options_labels[3] = "st_voice_chat_volume_option";
	sound_options_labels[4] = "st_use_microphone_option";
	sound_options_labels[5] = "st_microphone_sensitivity_option";
	sound_options_labels[6] = "st_ptt_button_option";

	fixed_string< 64 >* controllers_options_labels = VOSTOK_NEW_ARRAY_IMPL( ::survarium::g_allocator, fixed_string< 64 >, 2 );
	controllers_options_labels[0] = "st_mouse_invertion_option";
	controllers_options_labels[1] = "st_mouse_sensitivity_option";

	for ( u32 i = 0; i < 4; ++i )
	{
		fixed_string< 64 >* options_labels = NULL;
		u8 options_count = 0;
		switch ( i )
		{
			case 0:
				options_labels = gameplay_options_labels, options_count = 9;
				break;
			case 1:
				options_labels = video_options_labels, options_count = 19;
				break;
			case 2:
				options_labels = sound_options_labels, options_count = 7;
				break;
			case 3:
				options_labels = controllers_options_labels, options_count = 2;
				break;
		}

		flash_value options_args[2];
		options_args[0].SetUInt( i );
		m_options_ui->movie->CreateArray( &options_args[1] );

		for ( u8 j = 0; j < options_count; ++j )
		{
			flash_value options_item;
			m_options_ui->movie->CreateObject( &options_item );

			wchar_t label_txt[512];
			m_game.text_translator( ).translate_text( options_labels[j].c_str( ), label_txt );

			flash_value options_item_member;
			options_item_member.SetStringW( label_txt );
			options_item.SetMember( "label", options_item_member );

			if ( m_options[i] )
			{
				options_item_base* option = m_options[i]->option_by_id( j );
				bool has_data_provider = true;

				m_options_ui->movie->CreateFunction( &options_item_member, option );
				options_item.SetMember( "setter", options_item_member );

				if ( option->option_type( ) == string_selector )
					m_options_ui->movie->CreateArray( &options_item_member );
				else if ( option->option_type( ) == slider_selector )
					m_options_ui->movie->CreateObject( &options_item_member );
				else
					has_data_provider = false;

				if ( has_data_provider )
				{
					option->fill_data( options_item_member );
					options_item.SetMember( "dataProvider", options_item_member );
				}
			}

			options_args[1].SetElement( j, options_item );
		}

		m_options_ui->movie->Invoke( "root.set_settings", NULL, options_args, 2 );
	}

	VOSTOK_DELETE_ARRAY_IMPL( ::survarium::g_allocator, gameplay_options_labels );
	VOSTOK_DELETE_ARRAY_IMPL( ::survarium::g_allocator, video_options_labels );
	VOSTOK_DELETE_ARRAY_IMPL( ::survarium::g_allocator, sound_options_labels );
	VOSTOK_DELETE_ARRAY_IMPL( ::survarium::g_allocator, controllers_options_labels );
}

void game_options::activate( base_game_scene* parent_scene )
{
	if ( m_is_active )
		return;

	m_parent_scene = parent_scene;
	m_parent_scene->show_movie( m_options_ui );
	m_parent_scene->show_movie( m_cursor_ui );

	m_is_active = true;
	m_game.input_world( ).add_handler( *this );

	fill_menu_buttons( parent_scene == &m_game.get_game_world( ) );
}

void game_options::deactivate( )
{
	if ( !m_is_active )
		return;

	m_parent_scene->hide_movie( m_options_ui );
	m_parent_scene->hide_movie( m_cursor_ui );

	m_parent_scene = NULL;
	m_is_active = false;
	m_game.input_world( ).remove_handler( *this );
}

void game_options::tick( const u32 frame_delta, const u32, const bool )	// PDB: last two params __formal, genuinely unused
{
	float deltaTime = frame_delta * math::epsilon_3;

	m_options_ui->movie->Advance( deltaTime, 0 );
	m_cursor_ui->movie->Advance( deltaTime, 0 );
}

bool game_options::process_key_input( s32 dik )
{
	if ( dik == input::key_escape )
		return true;

	key_binder& binder = m_game.get_key_binder( );
	pcstr key_name = binder.dik_to_keyname( dik );
	if ( !key_name )
		return false;

	m_conflicted_action_ids.clear( );
	for ( u32 i = 0; i < 33; ++i )
	{
		if ( m_waiting_for_bind_action != key_bind_descriptions[i].action_id )
		{
			if ( !strings::compare( key_bind_descriptions[i].new_binded_key.c_str( ), key_name ) &&
				( binder.m_key_bindings[key_bind_descriptions[i].action_id].m_action->key_group &
				binder.m_key_bindings[m_waiting_for_bind_action].m_action->key_group ) )
			{
				m_conflicted_action_ids.push_back( key_bind_descriptions[i].action_id );
			}
		}
	}

	if ( !m_conflicted_action_ids.empty( ) )
	{
		m_conflicted_key_name = key_name;
		m_conflicted_action_to_bind = m_waiting_for_bind_action;

		flash_value message_txt;
		wchar_t w_text[512];
		m_game.text_translator( ).translate_text( "st_conflict_message", w_text );
		wcscat_s( w_text, sizeof( w_text ), L"\n" );

		for ( vector< game_action_id >::iterator it = m_conflicted_action_ids.begin( ); it != m_conflicted_action_ids.end( ); ++it )
		{
			wchar_t action_txt[512];
			for ( u32 i = 0; i < 33; ++i )
			{
				if ( *it == key_bind_descriptions[i].action_id )
					m_game.text_translator( ).translate_text( key_bind_descriptions[i].str_description, action_txt );
			}

			wcscat_s( w_text, sizeof( w_text ), L"\"" );
			wcscat_s( w_text, sizeof( w_text ), action_txt );
			wcscat_s( w_text, sizeof( w_text ), L"\"" );

			if ( it != m_conflicted_action_ids.end( ) - 1 )
				wcscat_s( w_text, sizeof( w_text ), L",\n" );
		}

		message_txt.SetStringW( w_text );
		m_options_ui->movie->Invoke( "root.show_reassign_message", NULL, &message_txt, 1 );
		finish_binding( );
		return false;
	}

	assign_binding( m_waiting_for_bind_action, key_name );
	return true;
}

void game_options::assign_binding( game_action_id action_id, pcstr key )
{
	bool is_default = false;
	for ( u8 i = 0; i < 33; ++i )
	{
		if ( key_bind_descriptions[i].action_id == action_id )
		{
			if ( !strings::compare( key_bind_descriptions[i].new_binded_key.c_str( ), key ) )
			{
				is_default = true;
				break;
			}

			key_bind_descriptions[i].new_binded_key.assignf( "%s", key );
		}
	}

	wchar_t w_key_name_txt[512];
	m_game.text_translator( ).translate_text( key, w_key_name_txt );

	flash_value bind_value[3];
	bind_value[0].SetStringW( w_key_name_txt );
	bind_value[1].SetUInt( action_id );
	bind_value[2].SetBoolean( is_default );
	m_options_ui->movie->Invoke( "root.set_keybind", NULL, bind_value, 3 );
}

void game_options::finish_binding( )
{
	m_options_ui->movie->Invoke( "root.end_keybind", NULL, NULL, 0 );
	m_parent_scene->show_movie( m_cursor_ui );
	m_waiting_for_bind_action = kLASTACTION;
}

bool game_options::on_keyboard_action(
	input::world*					input_world,
	input::enum_keyboard			key,
	input::enum_keyboard_action		action
)
{
	if ( m_waiting_for_bind_action != kLASTACTION && action == input::kb_key_down )
	{
		if ( process_key_input( key ) )
			finish_binding( );
	}
	else
		m_parent_scene->get_game( ).input_translator( ).process_keyboard( input_world, key, action, m_options_ui->movie, m_game.game_time_ms( ) );

	return true;
}

bool game_options::on_gamepad_action(
	input::world*					input_world,
	input::gamepad_button			button,
	input::enum_gamepad_action		action
)
{
	return true;
}

bool game_options::on_mouse_key_action(
	input::world*					input_world,
	input::mouse_button				button,
	input::enum_mouse_key_action	action
)
{
	if ( m_waiting_for_bind_action != kLASTACTION && action == input::ms_key_down )
	{
		if ( process_key_input( button ) )
		{
			m_options_ui->movie->Invoke( "root.end_keybind", NULL, NULL, 0 );
			m_parent_scene->show_movie( m_cursor_ui );
			m_waiting_for_bind_action = kLASTACTION;
		}
	}
	else
	{
		u32 mouse_btn = 0;
		switch ( button )
		{
			case input::mouse_button_left:		mouse_btn = 0; break;
			case input::mouse_button_right:		mouse_btn = 1; break;
			case input::mouse_button_middle:	mouse_btn = 2; break;
		}

		m_options_ui->movie->HandleMouseBtn(
			( flash_movie::mouse_btn_action )action,
			mouse_btn,
			( float )m_mouse_pos.x,
			( float )m_mouse_pos.y );
	}

	return true;
}

bool game_options::on_mouse_move(
	input::world*		input_world,
	s32					x,
	s32					y,
	s32					z
)
{
	if ( m_waiting_for_bind_action == kLASTACTION )
	{
		m_mouse_pos.x += x;
		m_mouse_pos.y += y;

		math::uint2 const& window_size = m_parent_scene->output_window_size( );
		math::clamp( m_mouse_pos.x, 0, ( s32 )window_size.x );
		math::clamp( m_mouse_pos.y, 0, ( s32 )window_size.y );

		float scroll_delta = z * ( 1.f / 120.f );
		m_cursor_ui->movie->HandleMouseMove( ( float )m_mouse_pos.x, ( float )m_mouse_pos.y, scroll_delta );
		m_options_ui->movie->HandleMouseMove( ( float )m_mouse_pos.x, ( float )m_mouse_pos.y, scroll_delta );
	}

	return true;
}

void game_options::refill_item_data( u8 options_tab_id, u8 options_item_id )
{
	flash_value options_item_data[3];
	options_item_data[0].SetUInt( options_tab_id );
	options_item_data[1].SetUInt( options_item_id );
	m_options_ui->movie->CreateArray( &options_item_data[2] );
	get_options_item( options_tab_id, options_item_id )->fill_data( options_item_data[2] );
	m_options_ui->movie->Invoke( "root.set_data_provider", NULL, options_item_data, 3 );
}

void game_options::initialize_bindings( )
{
	flash_value keybinds_value_prop;
	flash_value keybinds_array;
	m_options_ui->movie->CreateArray( &keybinds_array );

	for ( u32 i = 0; i < 33; ++i )
	{
		flash_value keybinds_value;
		m_options_ui->movie->CreateObject( &keybinds_value );

		keybinds_value_prop.SetUInt( key_bind_descriptions[i].action_id );
		keybinds_value.SetMember( "action_id", keybinds_value_prop );

		keybinds_value_prop.SetUInt( key_bind_descriptions[i].group_id );
		keybinds_value.SetMember( "group_id", keybinds_value_prop );

		keybinds_value_prop.SetUInt( key_bind_descriptions[i].type );
		keybinds_value.SetMember( "type", keybinds_value_prop );

		wchar_t label_txt[512];
		m_game.text_translator( ).translate_text( key_bind_descriptions[i].str_description, label_txt );
		keybinds_value_prop.SetStringW( label_txt );
		keybinds_value.SetMember( "label", keybinds_value_prop );

		keybinds_array.PushBack( keybinds_value );
	}

	m_options_ui->movie->Invoke( "root.set_keybindings", NULL, &keybinds_array, 1 );

	reset_bindings( true );
}

void game_options::reset_bindings( bool is_default )
{
	for ( u32 i = 0; i < 33; ++i )
	{
		pcstr key_name = m_game.get_key_binder( ).dik_to_keyname( m_game.get_key_binder( ).get_action_dik( key_bind_descriptions[i].action_id, -1 ) );

		flash_value bind_value[3];
		bind_value[1].SetUInt( key_bind_descriptions[i].action_id );

		if ( key_name )
		{
			wchar_t w_key_name_txt[512];
			m_game.text_translator( ).translate_text( key_name, w_key_name_txt );
			bind_value[0].SetStringW( w_key_name_txt );
			key_bind_descriptions[i].old_binded_key.assignf( "%s", key_name );
		}
		else
		{
			key_bind_descriptions[i].old_binded_key.assignf( "%s", "" );
			bind_value[0].SetStringW( L"" );
		}

		bind_value[2].SetBoolean( is_default );
		key_bind_descriptions[i].new_binded_key.assignf( "%s", key_bind_descriptions[i].old_binded_key.c_str( ) );
		m_options_ui->movie->Invoke( "root.set_keybind", NULL, bind_value, 3 );
	}
}

void game_options::apply_key_bindings( )
{
	for ( u32 i = 0; i < 33; ++i )
	{
		if ( strings::compare( key_bind_descriptions[i].new_binded_key.c_str( ), key_bind_descriptions[i].old_binded_key.c_str( ) ) )
		{
			key_bind_descriptions[i].old_binded_key.assignf( "%s", key_bind_descriptions[i].new_binded_key.c_str( ) );

			pstr command;
			if ( !strings::compare( key_bind_descriptions[i].new_binded_key.c_str( ), "" ) )
			{
				console_commands::console_command* console_command = console_commands::find( "unbind" );
				STR_JOINA( command, m_game.get_key_binder( ).id_to_action_name( key_bind_descriptions[i].action_id ) );
				console_command->execute( command );
			}
			else
			{
				console_commands::console_command* console_command = console_commands::find( "bind" );
				STR_JOINA( command, m_game.get_key_binder( ).id_to_action_name( key_bind_descriptions[i].action_id ), " ", key_bind_descriptions[i].new_binded_key.c_str( ) );
				console_command->execute( command );
			}
		}
	}
}

void game_options::reset_bindings_to_defaults( )
{
	m_game.get_key_binder( ).set_default_controls( );
	reset_bindings( false );
	apply_key_bindings( );
}


} // namespace survarium
