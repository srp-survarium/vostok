////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/dispersion_calculator.h>

#include <vostok/game_core/weapon_core.h>
#include <vostok/game_core/weapon_ammunition.h>
#include <vostok/console_command.h>

namespace survarium {

static bool s_dispersion_enabled_value = true; // <0x7db?>
static console_commands::cc_bool s_dispersion_enabled_cc( "dispersion_enabled", s_dispersion_enabled_value, false, console_commands::command_type_engine_internal );

/*
// STATE[STUB]
// void `dynamic atexit destructor for 's_dispersion_enabled_cc''()
void `dynamic atexit destructor for 's_dispersion_enabled_cc''( )
{
	// FUNCTION BODY
	// <0x7dafa0>|0x000|      :'14'	{
	// ******
}
*/

// STATE[100%|DONE]
dispersion_calculator::dispersion_calculator( ) :
	m_weapon				( NULL ),
	m_shooting_skill_coeff	( 1.0f ),
	m_aiming_speed_coeff	( 1.0f )
{
}

// STATE[87.49%|PARTIAL]: body/offsets/order all match; residual diff is LTCG only -
// claude@NOTE: bigger base frame (sub esp,58h vs 4Ch) -> [ebp-XX] slot reassignment;
// claude@NOTE: target calls weapon_core::is_aimed() out-of-line, base inlines it as
//   [this+0x488] (m_aimed) - the inline-vs-call COMDAT decision, not steerable here;
// claude@NOTE: the m_weapon->ammunition() safe-bool test (operator unspecified_bool_type)
//   lowers via an extra bool slot in base vs a direct cmp/sete in target - LTCG materialization.
// STRUCTURE DIFF:
// target: 0x586970            base: 0x45c300
// ; float survarium::dispersion_calculator::get_dispersion() const ; target 5 stmts / base 8 stmts
// 0x010 <0x7b> | 0x010 <0x87> | if ( !( m_weapon && m_weapon->ammunition( ) && s_dispersion_enabled_value ) )   SIZE
// .. same ..
// --          | <0>         |    EMPTY only base
// 0x092 <0xf> | 0x09e <0xe> | weapon_dispersion_params const& weapon_params = m_weapon->get_dispersion_params( );   SIZE
// --          | <0>         |    EMPTY only base
// --          | 0x0ac <0x3d> | : weapon_params.from_the_hip_multiplier;   ONLY base
// .. same ..
// 0x0a1 <0xef> | 0x0e9 <0xc1> | + ( m_weapon_calculator.get_value( ) + m_character_calculator.get_value( ) ) * m_shooting_skill_coeff;   SIZE
// ; aligned 2, size-diffs 3, quantity-diffs 3
// VERDICT: STRUCTURE MATCH - source ternary/guard shape mirrors target; stmt delta is is_aimed() inline-vs-call (ternary-else split) + safe-bool slot, LTCG only  trail: dispersion_calculator-get_dispersion.md
// float survarium::dispersion_calculator::get_dispersion() const
float dispersion_calculator::get_dispersion( ) const
{
	if ( !( m_weapon && m_weapon->ammunition( ) && s_dispersion_enabled_value ) )
		return 0.0f;

	weapon_dispersion_params const& weapon_params = m_weapon->get_dispersion_params( );
	float aim_or_hip_multiplier = m_weapon->is_aimed( )
		? weapon_params.aim_multiplier
		: weapon_params.from_the_hip_multiplier;

	return weapon_params.base_dispersion * m_weapon->ammunition( )->dispersion( ) * aim_or_hip_multiplier
		+ ( m_weapon_calculator.get_value( ) + m_character_calculator.get_value( ) ) * m_shooting_skill_coeff;
}

// STATE[UNCHECKED]
void dispersion_calculator::set_shooting_skill_coeff( float const shooting_skill_coef )
{
	m_shooting_skill_coeff = shooting_skill_coef;

	// FUNCTION BODY
	// <0x596787>|0x007|+0x00d:'70'
	// ******
}

// STATE[UNCHECKED]
void dispersion_calculator::set_aiming_speed_coeff( float const aiming_speed_coeff )
{
	m_aiming_speed_coeff = aiming_speed_coeff;
	apply_aim_speed( );

	// FUNCTION BODY
	// <0x596817>|0x007|+0x00d:'75'
	// <0x596824>|0x014|+0x008:'76'
	// ******
}

// STATE[100%|DONE]
void dispersion_calculator::apply_aim_speed( )
{
	float const speed_of_aiming = m_weapon
		? m_weapon->get_dispersion_params( ).speed_of_aiming * m_aiming_speed_coeff
		: 0.0f;

	m_character_calculator.set_aiming_speed( speed_of_aiming );
	m_weapon_calculator.set_aiming_speed( speed_of_aiming );
}

// STATE[100%|DONE]
void dispersion_calculator::set_weapon( weapon_core* weapon )
{
	m_weapon = weapon;
	if ( m_weapon )
	{
		m_weapon_calculator.set_one_shoot_dispersion_amount( m_weapon->get_dispersion_params( ).one_shoot_dispersion_amount );
		m_weapon_calculator.set_reload_dispersion_amount( m_weapon->get_dispersion_params( ).reload_dispersion_amount );
	}
	apply_aim_speed( );
}

// STATE[UNCHECKED]
void dispersion_calculator::tick(
	weapon_user_state_enum const	character_state,
	bool const						is_moving,
	bool const						is_aiming,
	u8 const						broken_hands_count,
	bool const						using_double_handed_weapon,
	u32	const						current_time_in_ms
)
{
	m_weapon_calculator.tick( current_time_in_ms );
	m_character_calculator.tick( character_state, is_moving, is_aiming, broken_hands_count, using_double_handed_weapon, current_time_in_ms );

	// FUNCTION BODY
	// <0x596887>|0x007|+0x00c:'100'
	// <0x596893>|0x013|+0x027:'101'
	// ******
}

// STATE[UNCHECKED]
void dispersion_calculator::reload( )
{
	m_weapon_calculator.reload( );

	// FUNCTION BODY
	// <0x596867>|0x007|+0x008:'106'
	// ******
}

// STATE[UNCHECKED]
void dispersion_calculator::fire( )
{
	m_weapon_calculator.fire( );

	// FUNCTION BODY
	// <0x596847>|0x007|+0x008:'111'
	// ******
}

} // namespace survarium
