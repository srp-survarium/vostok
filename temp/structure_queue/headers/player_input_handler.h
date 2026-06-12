////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef PLAYER_INPUT_HANDLER_H_INCLUDED
#define PLAYER_INPUT_HANDLER_H_INCLUDED

/* INCLUDES */
enum const survarium::input_mode_type_enum;
class const survarium::game_action_id;
class vostok::fixed_vector<stlp_std::pair<enum survarium::game_action_id,enum survarium::player_input_handler::action_state_enum>,32>;
class survarium::game_camera;
class survarium::game_world;
struct vostok::input::handler;
struct survarium::player_input;
enum survarium::input_mode_type_enum;
class vostok::input::enum_gamepad_action;
class vostok::input::enum_keyboard;
class vostok::input::enum_keyboard_action;
class vostok::input::enum_mouse_key_action;
class vostok::input::gamepad_button;
class vostok::input::mouse_button;

/* FORWARD REFS */
enum survarium::player_input_handler::action_state_enum;
class vostok::input::world;

namespace survarium {

enum survarium::input_mode_type_enum
{
	first_person_mode		= 0x0,
	free_fly_mode			= 0x1,
	third_person_mode		= 0x2,
};

class player_input_handler : public input::handler , public game_camera {
public:
			explicit				player_input_handler		( game_world& world );
	virtual							~player_input_handler		( ) { /* no source */ }

			void					set_yaw_pitch_distance		( const float yaw, const float arg_1, const float arg_2 );
			void					set_input_mode				(
										const input_mode_type_enum		arg_0 /* input_mode_type_enum input_mode */
									);
	inline	void					set_key_binder_context		( s32 arg_0 ) { /* no source */ }

			void					update_inverted_view		( float4x4 const& player_head_transform );

	inline	player_input const&		get_input					( ) const { /* no source */ }

	inline	input_mode_type_enum	input_mode					( ) const { /* no source */ }

			bool					alt_is_held					( ) const;

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

	virtual	s32						input_priority				( ) override { /* no source */ }

	virtual	void					on_before_processing		( input::world* input_world, const u32 current_time_in_ms ) override;
	virtual	void					on_after_processing			( input::world* input_world ) override;
	virtual	void					on_focus					( bool b_focus_enter ) override;

			void					process_first_person_mode	( const bool use_mouse_move );
			void					process_third_person_mode	( );

	inline	bool					action_present				( const game_action_id arg_0, player_input_handler::action_state_enum& arg_1 ) const { /* no source */ }

private:
	/* 0x0000 */	/* input::handler */
	/* 0x0004 */	/* game_camera */
	/* 0x0058 */	fixed_vector< std::pair< enum game_action_id, enum player_input_handler::action_state_enum >, 32 >	m_game_actions;
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
