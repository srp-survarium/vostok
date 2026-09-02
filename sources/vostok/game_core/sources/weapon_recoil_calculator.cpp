// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/weapon_recoil_calculator.h>

#include <vostok/game_core/weapon_core.h>
#include <vostok/console_command.h>

namespace survarium {

static bool s_recoil_use_pseudo_random_value = false;
static console_commands::cc_bool s_recoil_use_pseudo_random_cc( "recoil_use_pseudo_random", s_recoil_use_pseudo_random_value, false, console_commands::command_type_engine_internal );

float pseudo_random::random_f( const float range )
{
	float pi_x24	= math::pi * 24.0f;
	float t			= fmod( m_time, pi_x24 );
	float k			= math::pow( 2.73f, math::sin( t ) ) - 2.0f * math::cos( 4.0f * t ) + math::pow( math::sin( ( t - math::pi_d2 ) / 12.0f ), 5 );
	float result	= math::abs( fmod( k, 1.0f ) ) * range;
	return result;
}

weapon_recoil_calculator::weapon_recoil_calculator( ) :
	m_random							( 0 ),
	m_pseudo_random						( 0 ),
	m_interpolator						( 0.1f ),
	m_weapon							( NULL ),
	m_player_recoil_multiplier			( 1.0f ),
	m_player_compensation_multiplier	( 1.0f ),
	m_time_since_shoot					( 0.0f ),
	m_additive_recoil_timer				( 0.0f ),
	m_time_since_last_dispersion_change	( 0.0f ),
	m_vertical_koef						( 0.0f ),
	m_horizontal_koef					( 0.0f ),
	m_back_koef							( 0.0f ),
	m_target_vertical_koef				( 0.0f ),
	m_target_horizontal_koef			( 0.0f ),
	m_target_recoil_koef				( 0.0f ),
	m_last_time_in_ms					( 0 )
{
}

void weapon_recoil_calculator::tick( const u32 current_time_in_ms, const float time_scale )
{
	if ( !m_last_time_in_ms )
	{
		m_last_time_in_ms = current_time_in_ms;
		return;
	}

	if ( m_last_time_in_ms >= current_time_in_ms )
		return;

	if ( m_weapon )
	{
		u32 time_delta_in_ms = current_time_in_ms - m_last_time_in_ms;
		m_last_time_in_ms = current_time_in_ms;

		float dt_sec = time_delta_in_ms * 0.001f * time_scale;
		m_time_since_shoot += dt_sec;
		m_time_since_last_dispersion_change += dt_sec;
		m_time_since_last_dispersion_change = std::min( m_time_since_last_dispersion_change, m_interpolator.transition_time( ) );

		if ( m_additive_recoil_timer != 0.0f )
		{
			if ( dt_sec < m_additive_recoil_timer )
			{
				m_additive_recoil_timer -= dt_sec;
			}
			else
			{
				weapon_recoil_params const& weapon_params = m_weapon->get_recoil_params( );
				float additive_dispersion_angle_deg = weapon_params.additive_recoil_min_angle + get_random_angle( weapon_params.additive_recoil_angle_range );
				float additive_dispersion_angle_rad = math::deg2rad( additive_dispersion_angle_deg );
				float force_koef = get_random_amount( 1.0f );
				float additive_dispersion_amount = m_player_recoil_multiplier * force_koef * weapon_params.additive_side_recoil;

				m_target_vertical_koef		= m_vertical_koef + math::cos( additive_dispersion_angle_rad ) * additive_dispersion_amount;
				m_target_horizontal_koef	= m_horizontal_koef + math::sin( additive_dispersion_angle_rad ) * additive_dispersion_amount;

				float total_square_amount = math::sqr( m_target_vertical_koef ) + math::sqr( m_target_horizontal_koef );
				if ( total_square_amount > 1.0f )
				{
					float total_amount = math::sqrt( total_square_amount );
					m_target_vertical_koef		/= total_amount;
					m_target_horizontal_koef	/= total_amount;
				}

				float additive_recoil_amount = weapon_params.additive_back_recoil * force_koef;
				m_target_recoil_koef	= std::max( m_target_recoil_koef - additive_recoil_amount, 0.0f );

				m_additive_recoil_timer = 0.0f;
				m_back_koef				= std::max( m_back_koef - additive_dispersion_amount, 0.0f );
			}

			float interpolated_value			= m_interpolator.interpolated_value( m_time_since_last_dispersion_change );
			float one_minus_interpolated_value	= 1.0f - interpolated_value;
			m_vertical_koef		= ( m_target_vertical_koef * interpolated_value )	+ m_vertical_koef * one_minus_interpolated_value;
			m_horizontal_koef	= ( m_target_horizontal_koef * interpolated_value ) + m_horizontal_koef * one_minus_interpolated_value;
			m_back_koef			= ( m_target_recoil_koef * interpolated_value )		+ m_back_koef * one_minus_interpolated_value;

			process_compensation( dt_sec );
		}
	}
}

// claude@TODO: Why two recoil types? m_target_vertical_koef & m_target_horizontal_koef vs. m_target_recoil_koef
void weapon_recoil_calculator::fire( )
{
	weapon_recoil_params const& weapon_params = m_weapon->get_recoil_params( );
	float recoil_angle_deg	= weapon_params.shoot_recoil_min_angle + get_random_angle( weapon_params.shoot_recoil_angle_range );
	float force_koef		= get_random_amount( 1.0f );
	bool const first_shoot	= m_target_vertical_koef == 0.0f && m_target_horizontal_koef == 0.0f;

	float recoil			= m_player_recoil_multiplier * force_koef * ( first_shoot ? weapon_params.first_shoot_side_recoil : weapon_params.shoot_side_recoil );
	float recoil_angle_rad	= math::deg2rad( recoil_angle_deg );

	m_target_vertical_koef		= m_vertical_koef + math::cos( recoil_angle_rad ) * recoil;
	m_target_horizontal_koef	= m_horizontal_koef + math::sin( recoil_angle_rad ) * recoil;

	float total_square_amount = math::sqr( m_target_vertical_koef ) + math::sqr( m_target_horizontal_koef );
	if ( total_square_amount > 1.0f )
	{
		float total_amount = math::sqrt( total_square_amount );
		m_target_vertical_koef	 /= total_amount;
		m_target_horizontal_koef /= total_amount;
	}

	float recoil_amount		= m_player_recoil_multiplier * force_koef * ( first_shoot ? weapon_params.first_shoot_back_recoil : weapon_params.shoot_back_recoil );
	m_target_recoil_koef	= std::min( m_target_recoil_koef + recoil_amount, 1.0f );

	m_time_since_last_dispersion_change = 0.0f;
	m_time_since_shoot					= 0.0f;
	m_additive_recoil_timer				= weapon_params.additive_recoil_time;
}

// delinker pairs it and report.json now reads 100. claude@NOTE: the source duplicates the
// vertical/horizontal koef stores (0x34/0x38 written twice) - matches the target's 7 stores
// exactly, reproduced as-is per the #1 rule.
void weapon_recoil_calculator::reset( )
{
	m_time_since_last_dispersion_change	= 0.0f;
	m_time_since_shoot					= 0.0f;
	m_additive_recoil_timer 			= 0.0f;
	m_target_vertical_koef				= 0.0f;
	m_target_horizontal_koef 			= 0.0f;
	m_target_vertical_koef 				= 0.0f;
	m_target_horizontal_koef 			= 0.0f;
}

void weapon_recoil_calculator::reload( )
{
	reset( );
}

// sushi@TODO: What does that mean
void weapon_recoil_calculator::chamber_a_round( )
{
	reset( );
}

void weapon_recoil_calculator::process_compensation( const float dt_sec )
{
	weapon_recoil_params const& weapon_params = m_weapon->get_recoil_params( );
	float additive_compensation_speed = math::sqrt( math::sqr( m_target_vertical_koef ) + math::sqr( m_target_horizontal_koef ) );
	float compenstion_amount = ( weapon_params.side_compensation_speed + additive_compensation_speed ) * dt_sec * m_player_compensation_multiplier;

	m_target_vertical_koef = math::abs( m_target_vertical_koef ) > compenstion_amount
		? m_target_vertical_koef - ( math::sign( m_target_vertical_koef ) * compenstion_amount )
		: 0.0f;
	m_target_horizontal_koef = math::abs( m_target_horizontal_koef ) > compenstion_amount
		? m_target_horizontal_koef - ( math::sign( m_target_horizontal_koef ) * compenstion_amount )
		: 0.0f;

	float additive_recoil_compensation_speed = math::sqrt( math::sqr( m_target_recoil_koef ) + math::sqr( m_target_recoil_koef ) );
	float recoil_compensation_amount = ( weapon_params.back_compensation_speed + additive_recoil_compensation_speed ) * dt_sec * m_player_compensation_multiplier;

	m_target_recoil_koef = recoil_compensation_amount < m_target_recoil_koef
		? m_target_recoil_koef - recoil_compensation_amount
		: 0.0f;
}

float weapon_recoil_calculator::get_random_angle( const float range )
{
	if ( s_recoil_use_pseudo_random_value )
	{
		m_pseudo_random.set_time( m_weapon->get_user( )->local_time( m_last_time_in_ms ) * 0.01f );
		return m_pseudo_random.random_f( range );
	}
	else
	{
		return m_random.random_f( range );
	}
}

float weapon_recoil_calculator::get_random_amount( const float range )
{
	float k = s_recoil_use_pseudo_random_value
		? ( 1.0f + math::sin( m_weapon->get_user( )->local_time( m_last_time_in_ms ) * 0.01f ) ) * 0.5f
		: m_random.random_f( 1.0f );
	float c_min_amaunt = 0.25f;
	return math::max( c_min_amaunt, k ) * range;
}

void weapon_recoil_calculator::set_weapon( weapon_core* weapon )
{
	m_weapon = weapon;
}

} // namespace survarium
