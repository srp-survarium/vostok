// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/game_net_defines.h>
#include "player_logic_stand_state.h"
#include <vostok/game_core/base_player.h>
#include <vostok/game_core/weapon_core.h>
#include <vostok/game_core/weapon_user_animations_container.h>
#include <vostok/game_core/weapon_user_animations_selector.h>
#include <vostok/animation/mixing_addition_lexeme.h>
#include <vostok/animation/mixing_animation_lexeme_parameters.h>
#include <vostok/animation/linear_interpolator.h>
#include <vostok/animation/instant_interpolator.h>

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

// claude@NOTE: structure faithful; residual is the module-wide animation_lexeme_parameters
// builder-chain inline-vs-call wall (the VOSTOK_ANIMATION_API setters / mixing::operator+ /
// expression::operator= go out-of-line in the target but inline here). Same ceiling as
// weapon_core_aimed_fire_state::get_user_hands_expression.
animation::mixing::animation_lexeme player_logic_stand_state::movement_lexeme(
	mutable_buffer&						buffer,
	u32 const							animation_index,
	animation::body_part_masks_enum const	bones_mask,
	bool const							is_aimed,
	bool const							is_third_view,
	bool const							is_firing
) const
{
	u32 const							main_animation_index	= is_firing ? animation_index + 1 : animation_index;

	ASSERT( UNKNOWN_EXPRESSION_T( main_animation_index < 33 ) );

	animation::linear_interpolator		interpolator( 0.3f );

	animation::mixing::animation_lexeme	movement_lexeme(
		animation::mixing::animation_lexeme_parameters(
			buffer,
			m_owner.animations( ).get_stand_animation_caption( is_aimed, main_animation_index ),
			m_owner.animations( ).get_stand_animation( is_aimed, animation_index, is_third_view ),
			0,
			0
		)
		.weight_synchronization_group_id	( 0 )
		.time_synchronization_group_id		( animation_index ? 0 : u32(-1) )
		.weight_interpolator				( interpolator )
		.time_scale_interpolator			( interpolator )
		.time_scale							( m_user->get_movement_speed_factor( ) )
		.animated_object					( m_user )
		.bones_mask							( bones_mask )
		.user_data							( 1 )
	);

	return movement_lexeme;
}

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
	pcstr const							additive_animation_id	= m_owner.animations( ).get_stand_animation_caption( aimed, animation_index );

	resources::managed_resource_ptr		additive_animation		= m_owner.animations( ).get_stand_animation( aimed, animation_index, is_third_view );

	animation::mixing::animation_lexeme_parameters	recoil_lexeme_parameters( buffer, additive_animation_id, additive_animation, 0, 0 );

	float const							start_animation_interval_time	= recoil_lexeme_parameters.animation_intervals( )[ 0 ].length( ) * coeff;

	animation::mixing::animation_lexeme	lexeme(
		recoil_lexeme_parameters
		.start_animation_interval_time	( start_animation_interval_time )
		.animated_object				( m_user )
		.additivity_priority			( additivity_priority )
		.weight_interpolator			( interpolator )
		.time_scale_interpolator		( interpolator )
		.time_calculator				( time_calculator )
	);

	return animation::mixing::expression( lexeme );
}

animation::mixing::expression player_logic_stand_state::look_expression(
	mutable_buffer&						buffer,
	u32 const							movement_animation_index,
	bool const							is_aimed,
	bool const							is_third_view,
	weapon_animation_parameters const&	weapon_parameters,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	animation::instant_interpolator		interpolator;
	animation::linear_interpolator		l_interpolator( 0.3f );

	animation_type_enum const			animation_type	= animation_type_enum( movement_animation_index + 2 );

	pcstr const							look_animation_id	= m_owner.animations( ).get_stand_animation_caption( is_aimed, animation_type );

	resources::managed_resource_ptr		look_animation		= m_owner.animations( ).get_stand_animation( is_aimed, animation_type, is_third_view );

	animation::mixing::animation_lexeme_parameters	look_lexeme_parameters( buffer, look_animation_id, look_animation, NULL, &weight_driving_animation );

	float const							start_animation_interval_time	= look_lexeme_parameters.animation_intervals( )[ 0 ].length( ) * m_owner.look_time_factor( );

	animation::mixing::animation_lexeme	look_lexeme(
		look_lexeme_parameters
		.start_animation_interval_time	( start_animation_interval_time )
		.animated_object				( m_user )
		.additivity_priority			( 4 )
		.time_scale_interpolator		( interpolator )
		.time_calculator				( m_owner.look_time_calculator( ) )
	);

	animation::mixing::expression		result( look_lexeme );

	weapon_core&						weapon	= static_cast< weapon_core& >( *m_user->current_active_object( ) );

	if ( weapon_parameters.recoil_backward != 0 )
	{
		animation::mixing::expression	expression	= get_recoil_animation_lexeme( recoil_back_anim, is_aimed, weapon_parameters.recoil_backward, interpolator, buffer, is_third_view, 2, weapon.backward_recoil_time_calculator( ) );
		result	= result + expression;
	}

	if ( weapon_parameters.recoil_horizontal != 0 )
	{
		animation::mixing::expression	expression	= get_recoil_animation_lexeme( recoil_horizontal, is_aimed, weapon_parameters.recoil_horizontal, interpolator, buffer, is_third_view, 3, weapon.horizontal_recoil_time_calculator( ) );
		result	= result + expression;
	}

	if ( weapon_parameters.recoil_vertical != 0 )
	{
		animation::mixing::expression	expression	= get_recoil_animation_lexeme( recoil_vertical, is_aimed, weapon_parameters.recoil_vertical, interpolator, buffer, is_third_view, 3, weapon.vertical_recoil_time_calculator( ) );
		result	= result + expression;
	}

	return result;
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
