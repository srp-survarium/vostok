////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "player_logic_stand_state.h"
#include <vostok/game_core/base_player.h>
#include <vostok/game_core/weapon_user_animations_container.h>
#include <vostok/animation/mixing_addition_lexeme.h>

namespace survarium {

// claude@NOTE: extern definition needed by weapon_user_animations_container::
// get_stand_animation_caption (inlined into jump_logic::get_move_look_caption);
// content read from the target exe .rdata @va 0x89d090 (33 entries, single copy).
// 3 captions per direction: move, shoot, look - indexed by direction * 3 + part.
pcstr const stand_animations_captions[] = {
	"idle",
	"shoot_on_site",
	"idle_look",
	"move_fwd",
	"shoot_fwd",
	"move_fwd_look",
	"move_fwd_right",
	"shoot_fwd_right",
	"move_fwd_right_look",
	"move_right",
	"shoot_right",
	"move_right_look",
	"move_bwd_right",
	"shoot_bwd_right",
	"move_bwd_right_look",
	"move_bwd",
	"shoot_bwd",
	"move_bwd_look",
	"move_bwd_left",
	"shoot_bwd_left",
	"move_bwd_left_look",
	"move_left",
	"shoot_left",
	"move_left_look",
	"move_fwd_left",
	"shoot_fwd_left",
	"move_fwd_left_look",
	"recoil_vertical",
	"recoil_horizontal",
	"recoil_back",
	"throw_idle",
	"throw_start",
	"throw_end",
};

player_logic_stand_state::player_logic_stand_state( weapon_user_animations_selector& owner ) :
	player_logic_base_state	( owner, type_stand )
{
}

// STATE[STUB]
// claude@NOTE: PARKED on the weapon_user_animations_container::get_stand_animation wall
// (its template get_animation_impl<27,6> is declared but never DEFINED in any TU, so a real
// body LNK2001s unresolved - the same class of cross-keystone block as create_animation_interval).
// Recovered structure (target @0x780960, 5 statements, locals interpolator/main_animation_index/
// movement_lexeme):
//   u32 const main_animation_index = is_firing ? animation_index + 1 : animation_index;   // 44
//   ASSERT( UNKNOWN_EXPRESSION );                                                          // 45
//   linear_interpolator interpolator( s_aim_transition_time );                             // 46
//   return animation_lexeme( animation_lexeme_parameters( buffer,                          // 61
//       stand_animations_captions[ main_animation_index ],
//       m_user->animations()->get_stand_animation( is_aimed, animation_index, is_third_view ),
//       0, 0 )
//       .weight_synchronization_group_id( 0 )
//       .time_synchronization_group_id( animation_index ? 0 : u32(-1) )
//       .weight_interpolator( interpolator ).time_scale_interpolator( interpolator )
//       .time_scale( m_user-><float@+0x114> ).animated_object( m_user )
//       .bones_mask( bones_mask ).user_data( 1 ) );                                        // 63
// NEXT STEP: implement weapon_user_animations_container::get_stand_animation + get_animation_impl
// (the animation-container keystone), then land this body.
animation::mixing::animation_lexeme player_logic_stand_state::movement_lexeme(
	mutable_buffer&						buffer,
	u32 const							animation_index,
	animation::body_part_masks_enum const	bones_mask,
	bool const							is_aimed,
	bool const							is_third_view,
	bool const							is_firing
) const
{
	UNREACHABLE_CODE( );
}

// STATE[STUB]
// claude@NOTE: PARKED on the same weapon_user_animations_container::get_stand_animation /
// get_animation_impl<27,6> wall (undefined template -> LNK2001). Target @0x780710, 10 statements;
// locals recoil_lexeme_parameters / lexeme / start_animation_interval_time / additive_animation_id
// / additive_animation. Reached only by look_expression (also parked). NEXT STEP: land the
// animation-container keystone (get_stand_animation), then reconstruct from --view target.
animation::mixing::expression player_logic_stand_state::get_recoil_animation_lexeme(
	animation_type_enum					animation_index,
	bool const							aimed,
	float const							coeff,
	animation::base_interpolator const&	interpolator,
	mutable_buffer&						buffer,
	bool const							is_third_view,
	u32 const							additivity_priority,
	fastdelegate::FastDelegate<float(float,float,u32,u32,u32,float)> const&	time_calculator
) const
{
	UNREACHABLE_CODE( );
}

// STATE[STUB]
// claude@NOTE: PARKED on the weapon_user_animations_container::get_stand_animation /
// get_animation_impl<27,6> wall (undefined template -> LNK2001), same block as movement_lexeme.
// Large body (target @0x780bc0, ~0x46f bytes; locals result/look_animation_id/weapon/interpolator/
// look_lexeme_parameters/start_animation_interval_time/look_lexeme/look_animation/l_interpolator/
// animation_type + 3 SKIPPED expression branches at 0x780e51/0x780ef1/0x780f91). It also calls the
// parked get_recoil_animation_lexeme. NEXT STEP: land the animation-container keystone
// (get_stand_animation), then reconstruct from --view target/structure.
animation::mixing::expression player_logic_stand_state::look_expression(
	mutable_buffer&						buffer,
	u32 const							movement_animation_index,
	bool const							is_aimed,
	bool const							is_third_view,
	weapon_animation_parameters const&	weapon_parameters,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	UNREACHABLE_CODE( );
}

std::pair<animation::mixing::expression,animation::mixing::animation_lexeme> player_logic_stand_state::selected_animations( mutable_buffer& buffer, weapon_animation_parameters const& weapon_parameters, bool const is_third_view ) const
{
	u32 const							movement_animation_index	= player_logic_base_state::movement_animation_index( m_user->input( ) );

	animation::mixing::animation_lexeme	main_lexeme	= movement_lexeme(
		buffer,
		movement_animation_index,
		weapon_parameters.body_part_mask,
		weapon_parameters.is_aimed,
		is_third_view,
		weapon_parameters.is_firing
	);

	return std::make_pair< animation::mixing::expression, animation::mixing::animation_lexeme >(
		animation::mixing::expression( main_lexeme ) +
			look_expression( buffer, movement_animation_index, weapon_parameters.is_aimed, is_third_view, weapon_parameters, main_lexeme ),
		main_lexeme
	);
}

} // namespace survarium
