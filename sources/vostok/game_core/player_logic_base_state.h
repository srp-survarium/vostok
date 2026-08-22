////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef PLAYER_LOGIC_BASE_STATE_H_INCLUDED
#define PLAYER_LOGIC_BASE_STATE_H_INCLUDED

#include <vostok/ai/fsm_state.h>
#include <vostok/animation/mixing_animation_lexeme.h>
#include <vostok/animation/mixing_expression.h>

#include <vostok/game_core/weapon_user_state_enum.h>
#include <vostok/game_core/weapon_animation_parameters.h>

namespace vostok {
namespace network_core {
	class udp_match_packet;
	class packet_reader;
}
}

namespace survarium {

struct base_player;
class weapon_user_animations_selector;
struct player_input;

//	weapon_user_dead_state::`scalar deleting destructor'(unsigned int)
//	inventory_item::action

class player_logic_base_state : public ai::fsm_state {
public:
	explicit							player_logic_base_state		( weapon_user_animations_selector& owner, const weapon_user_state_enum weapon_user_state_id );

	virtual	void						set_user					( base_player& user );

	virtual	std::pair< animation::mixing::expression, animation::mixing::animation_lexeme >
										selected_animations			(
											mutable_buffer&						buffer,
											weapon_animation_parameters const&	weapon_parameters,
											bool								is_third_view
										) const = 0;

	// claude@NOTE: stub body must return a value or the abstract vtable fails LTCG
	// codegen (C4716/LNK1257) once an obj carrying this vtable is kept. Not matched.
	virtual	float4x4					get_attachment_transform	( ) const { return float4x4( ); }

	virtual	void						serialize					( network_core::udp_match_packet& arg_0 ) const { /* no source */ }
	virtual	void						deserialize					( network_core::packet_reader& arg_0 ) { /* no source */ }

	inline	weapon_user_state_enum		id							( ) const { return m_weapon_user_state_id; }
	inline	bool						is_ready_to_be_deactivated	( ) const { return m_is_ready_to_be_deactivated; }
	virtual	bool						is_ready_for_transition		( ) const override { return true; }

	inline	bool						is_weapon_visible			( ) const { /* no source */ }
	inline	bool						is_smoothing_needed			( ) const { /* no source */ }
	inline	bool						is_physics_transform_allowed( ) const { /* no source */ }

	virtual								~player_logic_base_state	( ) { }


protected:
	// claude@MATCH: protected static -> mangled `K` (target). private would be `C`,
	// public `S`; either mismatches the target's symbol so objdiff can't pair it.
	static	u32							movement_animation_index	( player_input const& input );

	// claude@NOTE: protected (not private) so derived state overrides
	// (e.g. player_logic_jump_state::is_ready_for_transition) can read m_user
	// directly, as the target codegen does (no accessor call). Access level does
	// not affect base_state's own byte output.
	/* 0x0000 */	/* ai::fsm_state */
	/* 0x0018 */	weapon_user_animations_selector&	m_owner;
	/* 0x001c */	base_player*						m_user;


private:

	/* 0x0020 */	weapon_user_state_enum				m_weapon_user_state_id;
	/* 0x0024 */	bool								m_is_weapon_weapon_visible;
	/* 0x0025 */	bool								m_is_smoothing_needed;
	/* 0x0026 */	bool								m_is_physics_transform_allowed;
	/* 0x0027 */	bool								m_is_ready_to_be_deactivated;
}; // class player_logic_base_state

STATIC_SIZE_ASSERT(player_logic_base_state, 0x28);

enum animation_type_enum {
	idle_anim					= 0x0,
	shoot_anim					= 0x1,
	idle_look_anim				= 0x2,
	move_fwd_anim				= 0x3,
	shoot_fwd_anim				= 0x4,
	move_fwd_look_anim			= 0x5,
	move_fwd_right_anim			= 0x6,
	shoot_fwd_right_anim		= 0x7,
	move_fwd_right_look_anim	= 0x8,
	move_right_anim				= 0x9,
	shoot_right_anim			= 0xA,
	move_right_look_anim		= 0xB,
	move_bwd_right_anim			= 0xC,
	shoot_bwd_right_anim		= 0xD,
	move_bwd_right_look_anim	= 0xE,
	move_bwd_anim				= 0xF,
	shoot_bwd_anim				= 0x10,
	move_bwd_look_anim			= 0x11,
	move_bwd_left_anim			= 0x12,
	shoot_bwd_left_anim			= 0x13,
	move_bwd_left_look_anim		= 0x14,
	move_left_anim				= 0x15,
	shoot_left_anim				= 0x16,
	move_left_look_anim			= 0x17,
	move_fwd_left_anim			= 0x18,
	shoot_fwd_left_anim			= 0x19,
	move_fwd_left_look_anim		= 0x1A,
	recoil_vertical				= 0x1B,
	recoil_horizontal			= 0x1C,
	recoil_back_anim			= 0x1D,
	throw_idle_anim				 = 0x1E,
	throw_start_anim			= 0x1F,
	throw_end_anim				= 0x20,
	animation_types_count		= 0x21,
	invalid_animation_type		= 0xFFFFFFFF,
};



} // namespace survarium

#endif // #ifndef PLAYER_LOGIC_BASE_STATE_H_INCLUDED
