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

// STATE[60.74%|PARTIAL]: source matches; sole divergence is math::pow(float,int) inline-vs-call - target INLINES pow(float,int) (5 is an int -> binds the int overload), base keeps it an out-of-line call. Non-steerable LTCG. See structure/weapon_recoil_calculator-random_f.md
float pseudo_random::random_f( const float range )
{
	float pi_x24	= math::pi * 24.0f;
	float t			= fmod( m_time, pi_x24 );
	float k			= math::pow( 2.73f, math::sin( t ) ) - math::cos( 4.0f * t ) * 2.0f + math::pow( math::sin( ( t - math::pi_d2 ) / 12.0f ), 5 );
	float result	= math::abs( fmod( k, 1.0f ) ) * range;
	return result;

	// STRUCTURE DIFF:
	// target: 0x57e420            base: 0x448d00
	// ; float survarium::pseudo_random::random_f(const float) ; target 5 stmts / base 5 stmts
	// .. same ..
	// 0x039 <0xd5> | 0x039 <0x85> | float k			= math::pow( 2.73f, math::sin( t ) ) - math::cos( 4.0f * t ) * 2.0f + math::pow( math::sin( ( t - math::pi_d2 ) / 12.0f ), 5 );   SIZE
	// 0x10e <0x2a> | 0x0be <0x26> | float result	= math::abs( fmod( k, 1.0f ) ) * range;   SIZE
	// .. same ..
	// ; aligned 3, size-diffs 2, quantity-diffs 0
	// VERDICT: STRUCTURE MATCH (shape ok) - all 5 statements present, 0 quantity-diffs. The k-row SIZE is math::pow(float,int) inline-vs-call: target inlines pow(float,int) (the test/jne sign-dispatch + pow_impl block, identical to base's out-of-line pow @0x4aaa0) while base emits `mov ecx,5; call math::pow`; that inline also reorders operand eval and shifts result-row codegen (movss vs x87). Non-steerable per-call-site LTCG, same class as the operator|/is_aimed/fixed_string entries. trail: weapon_recoil_calculator-random_f.md
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

// STATE[93.37%|PARTIAL]: body/control-flow/offsets match (disassembles fine now; the old
// BLOCKED "Ghidra didn't generate" note was stale). Residual SIZE rows are inline-vs-call of
// std::min/m_interpolator.transition_time() and the m_additive_recoil_timer compare - LTCG, not
// source-steerable. See structure embed below.
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

	// STRUCTURE DIFF:
	// target: 0x57e820            base: 0x4490b0
	// ; void survarium::weapon_recoil_calculator::tick(const unsigned int, const float) ; target 51 stmts / base 55 stmts
	// .. same ..
	// --          | <0>         |    EMPTY only base
	// .. same ..
	// 0x098 <0x2a> | 0x098 <0x49> | m_time_since_last_dispersion_change = std::min( m_time_since_last_dispersion_change, m_interpolator.transition_time( ) );   SIZE
	// --          | <0>         |    EMPTY only base
	// 0x0c2 <0x19> | 0x0e1 <0x1a> | if ( m_additive_recoil_timer != 0.0 )   SIZE
	// .. same ..
	// --          | <0>         |    EMPTY only base
	// .. same ..
	// --          | <0>         |    EMPTY only base
	// .. same ..
	// ; aligned 49, size-diffs 2, quantity-diffs 4
	// VERDICT: STRUCTURE MATCH (shape ok) - 49 statements align; the 2 SIZE rows are inline-vs-call of std::min / m_interpolator.transition_time() and the FPU 0.0-compare lowering, both LTCG. Quantity-diffs are EMPTY source-line gaps. Non-steerable. trail: weapon_recoil_calculator-tick.md
}

// STATE[91.69%|PARTIAL]: fixed a source precedence bug (79.80% -> 91.69%): target tests `first_shoot`
// ALONE as the ternary condition (movzx; test; je), so recoil/recoil_amount are
// `mult * koef * ( first_shoot ? a : b )` (parenthesized), not the `(mult*koef*first_shoot) ? a : b`
// we had. Residual is non-steerable: the `&&` bool temp width (target byte vs base dword), a frame-
// slot shift, and deg2rad/math::min inline-vs-call. claude@TODO: Why two recoil types?
// m_target_vertical_koef & m_target_horizontal_koef vs. m_target_recoil_koef
void weapon_recoil_calculator::fire( )
{
	weapon_recoil_params const& weapon_params = m_weapon->get_recoil_params( );
	float recoil_angle_deg	= weapon_params.shoot_recoil_min_angle + get_random_angle( weapon_params.shoot_recoil_angle_range );
	float force_koef		= get_random_amount( 1.0f );
	bool first_shoot		= m_target_vertical_koef == 0.0f && m_target_horizontal_koef == 0.0f;

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
	m_target_recoil_koef	= math::min( m_target_recoil_koef + recoil_amount, 1.0f );

	m_time_since_last_dispersion_change = 0.0f;
	m_time_since_shoot					= 0.0f;
	m_additive_recoil_timer				= weapon_params.additive_recoil_time;

	// STRUCTURE DIFF:
	// target: 0x57e600            base: 0x448e80
	// ; void survarium::weapon_recoil_calculator::fire() ; target 22 stmts / base 24 stmts
	// .. same ..
	// --          | 0x044 <0x40> | bool first_shoot		= m_target_vertical_koef == 0.0f && m_target_horizontal_koef == 0.0f;   ONLY base
	// --          | <0>         |    EMPTY only base
	// .. same ..
	// 0x07e <0x3a> | 0x0be <0xf> | float recoil_angle_rad	= math::deg2rad( recoil_angle_deg );   SIZE
	// .. same ..
	// 0x0b8 <0xf> | --          | L129   ONLY target
	// .. same ..
	// --          | <0>         |    EMPTY only base
	// .. same ..
	// 0x1b1 <0x32> | 0x1b7 <0x22> | m_target_recoil_koef	= math::min( m_target_recoil_koef + recoil_amount, 1.0f );   SIZE
	// .. same ..
	// ; aligned 19, size-diffs 2, quantity-diffs 4
	// VERDICT: STRUCTURE MATCH (shape ok) - after the precedence fix the recoil/recoil_amount ternaries align (no more ONLY base/target on those rows). Residual: the && bool temp is a byte in target vs dword in base + a frame-slot shift (the first_shoot ONLY base row), plus deg2rad/math::min inline-vs-call SIZE rows. Non-steerable. trail: weapon_recoil_calculator-fire.md
}

// STATE[None|DONE]: body byte-identical to target (7 stores align 7/7, 0 diffs); objdiff scores
// None only because the 0.0f constant-pool reloc resolves to different delinker symbol names
// (base out_of_range_reward vs target offset) - both are 0.0f. Non-steerable. claude@NOTE: the
// source duplicates the vertical/horizontal koef stores (0x34/0x38 written twice) - matches the
// target's 7 stores exactly, so reproduced as-is per the #1 rule.
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
}

// STATE[80.75%|PARTIAL]: same cross-unit get_user inline wall as get_random_angle
// (target keeps the out-of-line call; our build inlines get_user).
float weapon_recoil_calculator::get_random_amount( const float range )
{
	float k = s_recoil_use_pseudo_random_value
		? ( 1.0f + math::sin( m_weapon->get_user( )->local_time( m_last_time_in_ms ) * 0.01f ) ) * 0.5f
		: m_random.random_f( 1.0f );
	float c_min_amaunt = math::max( 0.25f, k ) * range;
	return c_min_amaunt;
}

// STATE[100%|DONE]
void weapon_recoil_calculator::set_weapon( weapon_core* weapon )
{
	m_weapon = weapon;
}

} // namespace survarium
