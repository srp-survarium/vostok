// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef PLAYER_INPUT_HANDLER_H_INCLUDED
#define PLAYER_INPUT_HANDLER_H_INCLUDED

#include <vostok/input/handler.h>
#include <vostok/game_core/player_input.h>

#include "game_action_id.h"
#include "game_camera.h"
// the canonical dump re-prints input_mode_type_enum here; batch 1 already
// rebuilt it standalone
#include "input_mode_type_enum.h"

namespace survarium {

class game_world;

// void* player_input_handler::`scalar deleting destructor'( u32 ) // FUNCTION BODY[0x96900]: <0x968f0>|0x000|      :'39'	{

class player_input_handler : public input::handler , public game_camera {
public:
			explicit				player_input_handler		( game_world& world );
	virtual							~player_input_handler		( ) { /* no source */ }

			void					set_yaw_pitch_distance		( const float yaw, const float arg_1, const float arg_2 );
			void					set_input_mode				(
										const input_mode_type_enum		arg_0 /* input_mode_type_enum input_mode */
									);
	inline	void					set_key_binder_context		( s32 arg_0 ) { m_key_binder_context = arg_0; }

			void					update_inverted_view		( float4x4 const& player_head_transform );

	inline	player_input const&		get_input					( ) const { /* no source */ return m_input; }

	inline	input_mode_type_enum	input_mode					( ) const { /* no source */ return m_input_mode; }

			bool					alt_is_held					( ) const;

private:
	virtual	bool					on_keyboard_action			(
										input::world*					input_world,
										input::enum_keyboard			key,
										input::enum_keyboard_action		actions_mask
									) override;
	virtual	bool					on_gamepad_action			(
										input::world*					input_world,
										input::gamepad_button			button,
										input::enum_gamepad_action		actions_mask
									) override;
	virtual	bool					on_mouse_key_action			(
										input::world*					input_world,
										input::mouse_button				button,
										input::enum_mouse_key_action	actions_mask
									) override;
	virtual	bool					on_mouse_move				(
										input::world*		input_world,
										s32					x,
										s32					y,
										s32					z
									) override;

	virtual	s32						input_priority				( ) override { /* no source */ return 0; }

	// target mangling: ?on_before_processing@...@@EAEXPAUworld@input@vostok@@I@Z -
	// the shipped input::handler interface passes (world*, u32); our legacy
	// handler.h still has the one-param shape, so no `override` here (this and
	// lobby_camera both carry the two-param virtual; revisit at TU enablement)
	virtual	void					on_before_processing		( input::world* input_world, const u32 current_time_in_ms );
	virtual	void					on_after_processing			( input::world* input_world ) override;
	virtual	void					on_focus					( bool b_focus_enter ) override;

			void					process_first_person_mode	( const bool use_mouse_move );
			void					process_third_person_mode	( );

	enum action_state_enum
	{
		down	= 0x0,
		up		= 0x1,
		hold	= 0x2,
	};

	inline	bool					action_present				( const game_action_id arg_0, action_state_enum& arg_1 ) const;

	typedef std::pair< game_action_id, action_state_enum > generated_action_type;
	typedef fixed_vector< generated_action_type, 32 > actions_type;

	/* 0x0000 */	/* input::handler */
	/* 0x0004 */	/* game_camera */
	/* 0x0058 */	actions_type			m_game_actions;
	/* 0x0160 */	game_world&				m_game_world;
	/* 0x0164 */	player_input			m_input;
	/* 0x0178 */	u32						m_time_delta_in_ms;
	/* 0x017c */	float2					m_rotation_delta;
	/* 0x0184 */	float					m_z_mouse_axis;
	/* 0x0188 */	u32						m_current_time_in_ms;
	/* 0x018c */	float					m_yaw;
	/* 0x0190 */	float					m_pitch;
	/* 0x0194 */	float					m_distance_to_focus_point;
	/* 0x0198 */	input_mode_type_enum	m_input_mode;
	/* 0x019c */	bool					m_input_mode_changed;
	/* 0x01a0 */	s32						m_key_binder_context;
}; // class player_input_handler

STATIC_SIZE_ASSERT(player_input_handler, 0x1A4);

} // namespace survarium

#endif // #ifndef PLAYER_INPUT_HANDLER_H_INCLUDED
