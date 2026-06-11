////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_recoil_calculator.h>

#include <vostok/game_core/weapon_core.h>
#include <vostok/console_command.h>

namespace survarium {

static bool s_recoil_use_pseudo_random_value = true; // <0x7db780>
static console_commands::cc_bool s_recoil_use_pseudo_random_cc( "recoil_use_pseudo_random", s_recoil_use_pseudo_random_value, false, console_commands::command_type_engine_internal );

// STATE[21.57%|PARTIAL]: math::pow(float,int) inline-vs-call wall (target inlines the int-pow
// sign-dispatch, base calls out-of-line); the % collapse from the old 60.74 is reloc/fold-name
// pairing churn on the float-const pool, not a structure change - 5/5 statements align.
float pseudo_random::random_f( const float range )
{
	float pi_x24	= math::pi * 24.0f;
	float t			= fmod( m_time, pi_x24 );
	float k			= math::pow( 2.73f, math::sin( t ) ) - math::cos( 4.0f * t ) * 2.0f + math::pow( math::sin( ( t - math::pi_d2 ) / 12.0f ), 5 );
	float result	= math::abs( fmod( k, 1.0f ) ) * range;
	return result;

	// STRUCTURE DIFF: target 5 / base 5 stmts
	// SIZE -0x50 | 21 | float k			= math::pow( 2.73f, math::sin( t ) ) - math::cos( 4.0f * t ) * 2.0f + math::pow( math::sin( ( t - math::pi_d2 ) / 12.0f ), 5 );
	// SIZE -0x4  | 22 | float result	= math::abs( fmod( k, 1.0f ) ) * range;
	// VERDICT: STRUCTURE MATCH (shape ok) - target inlines pow(float,int) (int literal 5 binds the int overload; the inlined block IS base's out-of-line pow body), base calls it; per-call-site LTCG, non-steerable.
}

// STATE[100%|DONE]: target ctor is empty (0 stmts) - LTCG dead-store-eliminated the constant-init
// list because the only caller is the synthetic anchor (same class as weapon_recoil_params ctor);
// base matches it byte-for-byte (target 0 / base 0 stmts, 0 diffs).
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

// STATE[93.90%|PARTIAL]: 38/38 statements align; sole residual is std::min inline-vs-call
// (base inlines stlp_std::min<float>, target calls the promoted out-of-line instantiation).
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

	// STRUCTURE DIFF: target 38 / base 38 stmts
	// SIZE +0x1f | 82 | m_time_since_last_dispersion_change = std::min( m_time_since_last_dispersion_change, m_interpolator.transition_time( ) );
	// VERDICT: STRUCTURE MATCH (shape ok) - base INLINES stlp_std::min<float> (comiss/branch ptr-select), target calls the LTCG-promoted out-of-line instantiation (ecx/eax by-address); per-call-site inline-vs-call, non-steerable.
}

// STATE[89.99%|PARTIAL]: bool const first_shoot closed the &&-temp-width row (the old BASE_ONLY
// first_shoot quantity diff was that one size change sliding the aligner); min restored to
// std::min - the target provably calls stlp_std::min<float> (by-ref temps + lea/lea/call), the
// old math::min was byte-closer (91.69) but the WRONG function. Faithful source over %.
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

	// STRUCTURE DIFF: target 18 / base 18 stmts
	// BASE_ONLY | 158 | float recoil_angle_rad	= math::deg2rad( recoil_angle_deg );
	// TRGT_ONLY | --  | L129 (0xf, same size+offset as the base row - aligner refused the pair on reloc content)
	// SIZE +0x17 | 172 | m_target_recoil_koef	= std::min( m_target_recoil_koef + recoil_amount, 1.0f );
	// VERDICT: STRUCTURE MATCH (shape ok) - the deg2rad pair is both-sides 0xf at offset 0xb8, a pairing refusal not a divergence; the min row is base INLINING stlp_std::min<float> where target calls the promoted instantiation (same wall as tick). Non-steerable.
}

// STATE[100%|DONE]: the old None was the 0.0f constant-pool reloc-name artifact; the current
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

// STATE[100%|DONE]
void weapon_recoil_calculator::reload( )
{
	reset( );
}

// STATE[100%|DONE]: sushi@TODO: What does that mean
void weapon_recoil_calculator::chamber_a_round( )
{
	reset( );
}

// STATE[93.74%|PARTIAL]: instruction stream matches; residual is the two
// math::abs(float) comparisons emitting x87 (fld/fcomip) in our build vs SSE
// (comiss) in the target - a return-register difference at the abs() call
// boundary (abs returns ST0 here, xmm0 in the target LTCG build).
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

	// STRUCTURE DIFF: target 8 / base 8 stmts
	// SIZE +0x5 | 237 | : 0.0f;
	// SIZE +0x5 | 240 | : 0.0f;
	// VERDICT: STRUCTURE MATCH (shape ok) - the two abs-compare ternaries: base compares via x87 (fld/fcomip) where target uses SSE comiss (abs() return-register at the LTCG call boundary); non-steerable.
}

// STATE[89.02%|PARTIAL]: cross-unit wall. The target keeps an out-of-line
// `call weapon_core::get_user`; our build inlines get_user (weapon_core.h declares
// it as an in-class inline reading m_user at +0x44C), shifting the frame +4 and
// replacing the call with a mov chain. weapon_core.h is owned by another unit;
// making get_user out-of-line there would close this. Otherwise byte-identical.
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

	// STRUCTURE DIFF: target 5 / base 5 stmts
	// SIZE +0x7 | 259 | m_pseudo_random.set_time( m_weapon->get_user( )->local_time( m_last_time_in_ms ) * 0.01f );
	// VERDICT: STRUCTURE MATCH (shape ok) - target calls weapon_core::get_user out-of-line, our build inlines the accessor (cross-unit header; out-lining it there is forbidden); non-steerable from here.
}

// STATE[91.0%|PARTIAL]: statement split recovered (80.75 -> 91.0): target inits c_min_amaunt
// to 0.25f in its OWN statement and does max+multiply in the return; only the cross-unit
// get_user inline wall remains.
float weapon_recoil_calculator::get_random_amount( const float range )
{
	float k = s_recoil_use_pseudo_random_value
		? ( 1.0f + math::sin( m_weapon->get_user( )->local_time( m_last_time_in_ms ) * 0.01f ) ) * 0.5f
		: m_random.random_f( 1.0f );
	float c_min_amaunt = 0.25f;
	return math::max( c_min_amaunt, k ) * range;

	// STRUCTURE DIFF: target 3 / base 3 stmts
	// SIZE +0x7 | 274 | : m_random.random_f( 1.0f );
	// VERDICT: STRUCTURE MATCH (shape ok) - sole row is the get_user accessor inline-vs-call (same cross-unit wall as get_random_angle); non-steerable from here.
}

// STATE[100%|DONE]
void weapon_recoil_calculator::set_weapon( weapon_core* weapon )
{
	m_weapon = weapon;
}

} // namespace survarium
