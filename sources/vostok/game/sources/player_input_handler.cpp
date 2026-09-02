// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/game_net_defines.h>
#include "player_input_handler.h"
#include "game_world.h"	// game_camera base needs game_world complete (upcast to base_game_scene)
#include "game.h"
#include "key_binder.h"
#include <vostok/input/world.h>
#include <vostok/input/keyboard.h>
#include <vostok/console_command.h>
#include <vostok/console_command_processor.h>

namespace survarium {

// owned by key_binder.cpp (cc_float/cc_bool console-command init); the mouse-input
// consumers here read them at namespace scope.
extern float	g_mouse_sensitivity;
extern bool		g_mouse_invert;

 player_input_handler::player_input_handler( game_world& world ) :
	game_camera( world ),		// base needs base_game_scene& (game_world derives from it)
	m_game_world( world ),		// ref member - the owner
	m_input( ),
	m_rotation_delta( 0.0f, 0.0f ),
	m_z_mouse_axis( 0.0f ),
	m_current_time_in_ms( 0 ),
	m_yaw( 0.0f ),
	m_pitch( 0.0f ),
	m_distance_to_focus_point( 1.0f ),
	m_input_mode( ),
	m_input_mode_changed( true ),
	m_key_binder_context( 1 )
{
}

// Target inlines all three action pushes; this compiler retains only the key-up call.


bool player_input_handler::on_keyboard_action(
	input::world* const				input_world,
	input::enum_keyboard			key,
	input::enum_keyboard_action		actions_mask
)
{
	VOSTOK_UNREFERENCED_PARAMETER( input_world );

	toggle_action_enum	actions_mask_type;
	game&				current_game = m_game_world.get_game( );
	game_action_id const	game_action = current_game.get_key_binder( ).get_binded_action( key, actions_mask_type, m_key_binder_context );

	if( game_action == kNOTBINDED )
		return false;

	if( actions_mask == input::kb_key_down )
	{
		switch( game_action )
		{
			case kPAUSE:
				m_game_world.get_game( ).toggle_pause( );
				break;
			case kSERIALIZE_PLAYER_STATE:
				vostok::console_commands::execute( "serialize_player_state", vostok::console_commands::execution_filter_all );
				break;
			case kDESERIALIZE_PLAYER_STATE:
				vostok::console_commands::execute( "deserialize_player_state", vostok::console_commands::execution_filter_all );
				break;
		}

		if( actions_mask_type == hold_action || actions_mask_type == toggle_action )
			m_game_actions.push_back( std::make_pair( game_action, down ) );
	}
	else if( actions_mask == input::kb_key_hold )
	{
		if( actions_mask_type == hold_action )
			m_game_actions.push_back( std::make_pair( game_action, hold ) );
	}
	else if( actions_mask == input::kb_key_up )
	{
		if( actions_mask_type == hold_action )
			m_game_actions.push_back( std::make_pair( game_action, up ) );
	}

	return false;
}

bool player_input_handler::on_gamepad_action(
	input::world*					input_world,
	input::gamepad_button			button,
	input::enum_gamepad_action		actions_mask
)
{
	VOSTOK_UNREFERENCED_PARAMETERS( input_world, button, actions_mask );
	return false;
}

bool player_input_handler::on_mouse_key_action(
	input::world*					input_world,
	input::mouse_button				button,
	input::enum_mouse_key_action	actions_mask
)
{
	VOSTOK_UNREFERENCED_PARAMETER( input_world );


	toggle_action_enum	actions_mask_type;
	game_action_id const	game_action = m_game_world.get_game().get_key_binder( ).get_binded_action( button, actions_mask_type, 1 );

	if( game_action == kNOTBINDED )
		return false;

	if( actions_mask == input::ms_key_down && ( actions_mask_type == hold_action || actions_mask_type == toggle_action ) )
		m_game_actions.push_back( std::make_pair( game_action, down ) );
	else if( actions_mask == input::ms_key_hold && actions_mask_type == hold_action )
		m_game_actions.push_back(
			std::make_pair( game_action, hold )
		);

	return false;
}

bool player_input_handler::on_mouse_move(
	input::world*		input_world,
	s32					x,
	s32					y,
	s32					z
)
{
	VOSTOK_UNREFERENCED_PARAMETER( input_world );

	const float horizontal_sensitivity	= m_fov_factor * g_mouse_sensitivity * 0.1f;
	float vertical_sensitivity			= ( m_game_world.get_game( ).engine( ).get_render_window_size( ).y / m_game_world.get_game( ).engine( ).get_render_window_size( ).x ) * horizontal_sensitivity * 0.95492965f;
	if ( g_mouse_invert )	vertical_sensitivity	= -vertical_sensitivity;

	m_rotation_delta.x	-= ( ( float( x ) / 180.0f ) * math::pi ) * horizontal_sensitivity;
	m_rotation_delta.y	-= ( ( float( y ) / 180.0f ) * math::pi ) * vertical_sensitivity;
	m_z_mouse_axis		-= float( z ) * math::epsilon_3;

	return false;
}

void player_input_handler::on_before_processing( input::world* const input_world, const u32 current_time_in_ms )
{
	VOSTOK_UNREFERENCED_PARAMETER( input_world );

	m_game_actions.clear( );

	m_rotation_delta	= float2( 0.0f, 0.0f );

	m_z_mouse_axis = 0.0f;
	m_input.actions_mask = 0;
	m_time_delta_in_ms		= current_time_in_ms - m_current_time_in_ms;

	m_current_time_in_ms	= current_time_in_ms;
}

void player_input_handler::on_after_processing( input::world* input_world )
{
	VOSTOK_UNREFERENCED_PARAMETER( input_world );

	const float time_delta = m_time_delta_in_ms / 1000.0f;

	const float2& angular_velocity = m_rotation_delta / time_delta;
	m_input.angular_acceleration	= ( angular_velocity - m_input.angular_velocity ) * 2.0f / time_delta;
	m_input.angular_velocity		= angular_velocity;

	if( m_input_mode == first_person_mode )
		process_first_person_mode( true );
	else
		process_third_person_mode( );
}

bool player_input_handler::alt_is_held( ) const
{
	input::keyboard const& keyboard = *m_game_world.get_game( ).input_world( ).get_keyboard( );
	return keyboard.is_key_down( input::key_rmenu ) ||
		   keyboard.is_key_down( input::key_lmenu );
}

// TU-local primary template (canonical "headers/first_predicate_enum
// survarium__game_action_id_.h" - the PDB only records the <game_action_id>
// monomorphisation; its consumer is process_first_person_mode's __find_if over
// m_game_actions, per the rich index)
template < typename T >
struct first_predicate {
	inline	explicit	first_predicate	( T const& arg_0 ) :
		parameter( arg_0 ) { /* no source */ }

	// inlined away in the __find_if COMDAT (no PDB record); pair.first shape
	// per the instantiation's argument types
	template < typename P >
	inline	bool	operator()	( P const& arg_0 ) const { /* no source */ return arg_0.first == parameter; }

	/* 0x0000 */	T		parameter;
}; // struct first_predicate

STATIC_SIZE_ASSERT(first_predicate< enum game_action_id >, 0x4);

// inlined into process_first_person_mode (no standalone symbol); first_predicate is
// TU-local so the body lives here, not in the header
inline bool player_input_handler::action_present( const game_action_id action, action_state_enum& state ) const
{
	fixed_vector< std::pair< game_action_id, action_state_enum >, 32 >::const_iterator const found =
		std::find_if( m_game_actions.begin( ), m_game_actions.end( ), first_predicate< game_action_id >( action ) );

	if ( found == m_game_actions.end( ) )
		return false;

	state	= found->second;
	return true;
}

void player_input_handler::process_first_person_mode( const bool use_mouse_move )
{
	if ( m_game_actions.begin( ) == m_game_actions.end( ) && m_input.is_empty( ) )
		return;

	if ( std::find_if( m_game_actions.begin( ), m_game_actions.end( ), first_predicate< game_action_id >( kFWD ) ) != m_game_actions.end( ) )
		m_input.actions_mask	|= 0x1;
	if ( std::find_if( m_game_actions.begin( ), m_game_actions.end( ), first_predicate< game_action_id >( kBACK ) ) != m_game_actions.end( ) )
		m_input.actions_mask	|= 0x2;
	if ( std::find_if( m_game_actions.begin( ), m_game_actions.end( ), first_predicate< game_action_id >( kR_STRAFE ) ) != m_game_actions.end( ) )
		m_input.actions_mask	|= 0x8;
	if ( std::find_if( m_game_actions.begin( ), m_game_actions.end( ), first_predicate< game_action_id >( kL_STRAFE ) ) != m_game_actions.end( ) )
		m_input.actions_mask	|= 0x4;

	if ( use_mouse_move )
	{
		const float horizontal_sensitivity	= m_fov_factor * g_mouse_sensitivity * 0.1f;
		float vertical_sensitivity			= ( m_game_world.get_game( ).engine( ).get_render_window_size( ).y / m_game_world.get_game( ).engine( ).get_render_window_size( ).x ) * horizontal_sensitivity * 0.95492965f;
		if ( g_mouse_invert )	vertical_sensitivity	= -vertical_sensitivity;

		if ( std::find_if( m_game_actions.begin( ), m_game_actions.end( ), first_predicate< game_action_id >( kUP ) ) != m_game_actions.end( ) )
			m_rotation_delta.x	+= vertical_sensitivity * 0.0174532924f;
		if ( std::find_if( m_game_actions.begin( ), m_game_actions.end( ), first_predicate< game_action_id >( kDOWN ) ) != m_game_actions.end( ) )
			m_rotation_delta.x	-= vertical_sensitivity * 0.0174532924f;
		if ( std::find_if( m_game_actions.begin( ), m_game_actions.end( ), first_predicate< game_action_id >( kRIGHT ) ) != m_game_actions.end( ) )
			m_rotation_delta.y	-= horizontal_sensitivity * 0.0174532924f;
		if ( std::find_if( m_game_actions.begin( ), m_game_actions.end( ), first_predicate< game_action_id >( kLEFT ) ) != m_game_actions.end( ) )
			m_rotation_delta.y	+= horizontal_sensitivity * 0.0174532924f;
	}

	if ( std::find_if( m_game_actions.begin( ), m_game_actions.end( ), first_predicate< game_action_id >( kJUMP ) ) != m_game_actions.end( ) )
		m_input.actions_mask	|= 0x10;

	if ( std::find_if( m_game_actions.begin( ), m_game_actions.end( ), first_predicate< game_action_id >( kWPN_FIRE ) ) != m_game_actions.end( ) )
	{
		if ( alt_is_held( ) )
			vostok::console_commands::execute( "remove_player", vostok::console_commands::execution_filter_all );
		else
			m_input.actions_mask	|= 0x20;
	}

	if ( std::find_if( m_game_actions.begin( ), m_game_actions.end( ), first_predicate< game_action_id >( kWPN_RELOAD ) ) != m_game_actions.end( ) )
		m_input.actions_mask	|= 0x40;
	if ( std::find_if( m_game_actions.begin( ), m_game_actions.end( ), first_predicate< game_action_id >( kWPN_AIM ) ) != m_game_actions.end( ) )
		m_input.actions_mask	|= 0x80;
	if ( std::find_if( m_game_actions.begin( ), m_game_actions.end( ), first_predicate< game_action_id >( kCROUCH ) ) != m_game_actions.end( ) )
		m_input.actions_mask	|= 0x100;
	if ( std::find_if( m_game_actions.begin( ), m_game_actions.end( ), first_predicate< game_action_id >( kACCEL ) ) != m_game_actions.end( ) )
		m_input.actions_mask	|= 0x200;
	if ( std::find_if( m_game_actions.begin( ), m_game_actions.end( ), first_predicate< game_action_id >( kSET_NEXT_FIRE_QUEUE_TYPE ) ) != m_game_actions.end( ) )
		m_input.actions_mask	|= 0x400;
	if ( std::find_if( m_game_actions.begin( ), m_game_actions.end( ), first_predicate< game_action_id >( kSET_NEXT_AMMO_TYPE ) ) != m_game_actions.end( ) )
		m_input.actions_mask	|= 0x800;
	if ( std::find_if( m_game_actions.begin( ), m_game_actions.end( ), first_predicate< game_action_id >( kWPN_1 ) ) != m_game_actions.end( ) )
		m_input.actions_mask	|= 0x1000;
	if ( std::find_if( m_game_actions.begin( ), m_game_actions.end( ), first_predicate< game_action_id >( kWPN_2 ) ) != m_game_actions.end( ) )
		m_input.actions_mask	|= 0x2000;

	action_state_enum	state;
	if ( action_present( kQUICK_USE_1, state ) )
	{
		if ( state == down )
			m_input.actions_mask	|= 0x4000;
		else if ( state == up )
			m_input.actions_mask	|= 0x8000;
	}
	if ( action_present( kQUICK_USE_2, state ) )
	{
		if ( state == down )
			m_input.actions_mask	|= 0x10000;
		else if ( state == up )
			m_input.actions_mask	|= 0x20000;
	}
	if ( action_present( kQUICK_USE_3, state ) )
	{
		if ( state == down )
			m_input.actions_mask	|= 0x40000;
		else if ( state == up )
			m_input.actions_mask	|= 0x80000;
	}
	if ( action_present( kQUICK_USE_4, state ) )
	{
		if ( state == down )
			m_input.actions_mask	|= 0x100000;
		else if ( state == up )
			m_input.actions_mask	|= 0x200000;
	}
	if ( action_present( kQUICK_USE_5, state ) )
	{
		if ( state == down )
			m_input.actions_mask	|= 0x400000;
		else if ( state == up )
			m_input.actions_mask	|= 0x800000;
	}
	if ( action_present( kQUICK_USE_6, state ) )
	{
		if ( state == down )
			m_input.actions_mask	|= 0x1000000;
		else if ( state == up )
			m_input.actions_mask	|= 0x2000000;
	}

	if ( std::find_if( m_game_actions.begin( ), m_game_actions.end( ), first_predicate< game_action_id >( kBACK_SLOT_USE ) ) != m_game_actions.end( ) )
		m_input.actions_mask	|= 0x4000000;
	if ( std::find_if( m_game_actions.begin( ), m_game_actions.end( ), first_predicate< game_action_id >( kDELAY_BREATH ) ) != m_game_actions.end( ) )
		m_input.actions_mask	|= 0x8000000;
	if ( std::find_if( m_game_actions.begin( ), m_game_actions.end( ), first_predicate< game_action_id >( kUSE ) ) != m_game_actions.end( ) )
		m_input.actions_mask	|= 0x10000000;
	if ( std::find_if( m_game_actions.begin( ), m_game_actions.end( ), first_predicate< game_action_id >( kDROP ) ) != m_game_actions.end( ) )
		m_input.actions_mask	|= 0x40000000;
	if ( std::find_if( m_game_actions.begin( ), m_game_actions.end( ), first_predicate< game_action_id >( kMISSILE_WPN ) ) != m_game_actions.end( ) )
		m_input.actions_mask	|= 0x20000000;
}

void player_input_handler::process_third_person_mode( )
{
	m_distance_to_focus_point	+= m_z_mouse_axis;

	if( alt_is_held( ) ||
		m_key_binder_context == 0x10 )
	{
		m_yaw	+= m_rotation_delta.x;
		m_pitch	+= m_rotation_delta.y;

		process_first_person_mode( false );

		m_rotation_delta				= float2( 0.0f, 0.0f );
		m_input.angular_velocity		= float2( 0.0f, 0.0f );
		m_input.angular_acceleration	= float2( 0.0f, 0.0f );
	}
	else
		process_first_person_mode( true );
}

// Branch order follows the target; the third-person offset keeps its sign-mask evaluation.

void player_input_handler::update_inverted_view( float4x4 const& player_head_transform )
{
	if ( m_input_mode == first_person_mode )
		m_inverted_view_matrix = player_head_transform;
	else
	{
		float4x4 new_inverted_view;
		new_inverted_view	= player_head_transform;
		new_inverted_view	= math::mul4x3( new_inverted_view, math::create_rotation_y( m_yaw ) );
		new_inverted_view	= math::mul4x3( new_inverted_view, math::create_rotation( new_inverted_view.i.xyz( ), m_pitch ) );

		new_inverted_view.c.xyz( )	= player_head_transform.c.xyz( ) + m_distance_to_focus_point * ( -new_inverted_view.k.xyz( ) + new_inverted_view.i.xyz( ) * 0.2f );

		m_inverted_view_matrix		= new_inverted_view;
	}

	m_input_mode_changed		= false;
}

void player_input_handler::set_yaw_pitch_distance( const float yaw, const float arg_1, const float arg_2 )
{
	m_yaw						= yaw;
	m_pitch						= arg_1;
	m_distance_to_focus_point	= arg_2;
}

void player_input_handler::set_input_mode(
	const input_mode_type_enum		input_mode
)
{
	m_input_mode_changed	= m_input_mode_changed || m_input_mode != input_mode;
	m_input_mode			= input_mode;
}

void player_input_handler::on_focus( bool b_focus_enter )
{
	if( b_focus_enter )
		m_game_world.get_game( ).input_world( ).add_handler( *this );
	else
		m_game_world.get_game( ).input_world( ).remove_handler( *this );
}

} // namespace survarium
