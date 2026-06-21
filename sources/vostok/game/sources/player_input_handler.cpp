////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "player_input_handler.h"
#include "game_world.h"	// game_camera base needs game_world complete (upcast to base_game_scene)
#include "game.h"
#include "key_binder.h"
#include <vostok/input/world.h>
#include <vostok/input/keyboard.h>
#include <vostok/console_command.h>
#include <vostok/console_command_processor.h>

namespace survarium {

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

bool player_input_handler::on_keyboard_action(
	input::world* const				input_world,
	input::enum_keyboard			key,
	input::enum_keyboard_action		actions_mask
)
{
	VOSTOK_UNREFERENCED_PARAMETER( input_world );

	toggle_action_enum	actions_mask_type;
	game_action_id const	game_action = m_game_world.get_game().get_key_binder( ).get_binded_action( key, actions_mask_type, m_key_binder_context );

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
	game_action_id const	game_action = m_game_world.get_game().get_key_binder( ).get_binded_action( button, actions_mask_type, m_key_binder_context );

	if( game_action == kNOTBINDED )
		return false;

	if( actions_mask == input::ms_key_down )
	{
		if( actions_mask_type == hold_action || actions_mask_type == toggle_action )
			m_game_actions.push_back( std::make_pair( game_action, down ) );
	}
	else if( actions_mask == input::ms_key_hold )
	{
		if( actions_mask_type == hold_action )
			m_game_actions.push_back( std::make_pair( game_action, hold ) );
	}

	return false;
}

// claude@NOTE: blocked by key_binder.cpp (a still-stub TU): the body reads the
// namespace-scope globals survarium::g_mouse_sensitivity / g_mouse_invert, whose
// definitions (set_mouse_sensitivity_cc dynamic-init) are owned by key_binder.cpp.
// Referencing them as externs here would leave them undefined at link until
// key_binder.cpp is matched. Kept a buildable stub; re-match once key_binder.cpp
// defines the globals.
bool player_input_handler::on_mouse_move(
	input::world*		input_world,
	s32					x,
	s32					y,
	s32					z
)
{
	VOSTOK_UNREFERENCED_PARAMETERS( input_world, x, y, z );
	return false;
}

void player_input_handler::on_before_processing( input::world* const input_world, const u32 current_time_in_ms )
{
	VOSTOK_UNREFERENCED_PARAMETER( input_world );

	m_game_actions.clear( );

	m_rotation_delta	= float2( 0.0f, 0.0f );

	m_time_delta_in_ms		= current_time_in_ms - m_current_time_in_ms;

	m_current_time_in_ms	= current_time_in_ms;
}

// claude@NOTE: instruction stream matches the target; the only residual is
// register allocation around the process_*_person_mode tail calls (target keeps
// `this` in eax + edi-as-callee-save, base keeps ecx + esi) - an LTCG scheduling
// choice, not source-steerable. The `const float2&` binds the velocity temporary
// so it is computed ONCE (no phantom local; --view info still reports 2 locals).
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

// claude@NOTE: PARKED. 144-statement first-person camera/input integrator: reads
// g_mouse_sensitivity (key_binder.cpp-owned global, see on_mouse_move cap), walks
// m_game_actions with a first_predicate<game_action_id> __find_if for each bound
// action, and folds rotation/move into m_input. Two caps stack here: (1) the
// g_mouse_sensitivity global is undefined until key_binder.cpp is matched, and (2)
// the bulk depends on m_game_actions content which on_keyboard/on_mouse_key only
// populate once key_binder's get_binded_action returns a real (non-constant) action.
// Next step: match key_binder.cpp first, then reconstruct the per-action find_if.
// STATE[STUB]
void player_input_handler::process_first_person_mode( const bool use_mouse_move )
{
	VOSTOK_UNREFERENCED_PARAMETER( use_mouse_move );
}

// claude@NOTE: callee-stub-blocked. The two process_first_person_mode() calls fold
// away (process_first_person_mode is an empty STUB until key_binder/player land, so
// LTCG inlines its {} ), which the target keeps as real calls (TRGT_ONLY in the
// structure-diff). The fuzzy % here is unstable regalloc noise around those folded
// calls; it cannot reach 100% until process_first_person_mode has its real body.
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

// claude@NOTE: PARKED. Builds new_inverted_view = player_head_transform composed
// with create_rotation_y( m_yaw ) and create_rotation( pitch axis ) scaled by
// m_distance_to_focus_point, then writes it into game_camera::m_inverted_view_matrix
// and clears m_input_mode_changed. Blocked: the store targets game_camera's PRIVATE
// m_inverted_view_matrix (offset 0x8) directly - there is no public/protected setter
// on game_camera (only get_inverted_view_matrix() const), so the exact write needs a
// game_camera mutator that game_camera.cpp (not this TU) must expose. Next step:
// confirm the game_camera setter / friend when game_camera.cpp is matched.
// STATE[STUB]
void player_input_handler::update_inverted_view( float4x4 const& player_head_transform )
{
	VOSTOK_UNREFERENCED_PARAMETER( player_head_transform );
}

// claude@NOTE: blocked by player.cpp (a still-stub TU): the body ignores its 3
// params and copies the namespace-scope death-camera config statics
// s_death_camera_yaw / _pitch / _distance into m_yaw / m_pitch /
// m_distance_to_focus_point. Those statics (cc_death_camera_* dynamic-init) are
// owned by player.cpp; referencing them as externs would be undefined at link
// until player.cpp is matched. Kept a buildable stub; re-match after player.cpp.
void player_input_handler::set_yaw_pitch_distance( const float yaw, const float arg_1, const float arg_2 )
{
	VOSTOK_UNREFERENCED_PARAMETERS( yaw, arg_1, arg_2 );
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
