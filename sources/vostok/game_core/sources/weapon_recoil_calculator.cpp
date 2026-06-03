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

// STATE[60.74%|PARTIAL]: pow inlined in base, after that is fixed further things need to be verified
float pseudo_random::random_f( const float range )
{
	float pi_x24	= math::pi * 24.0f;
	float t			= fmod( m_time, pi_x24 );
	float k			= math::pow( 2.73f, math::sin( t ) ) - math::cos( 4.0f * t ) * 2.0f + math::pow( math::sin( ( t - math::pi_d2 ) / 12.0f ), 5 );
	float result	= math::abs( fmod( k, 1.0f ) ) * range;
	return result;

	// FUNCTION BODY
	// <0x58e429>|0x009|+0x015:'22'
	// <0x58e43e>|0x01e|+0x01b:'23'
	// <0x58e459>|0x039|+0x0d5:'24'
	// <0x58e52e>|0x10e|+0x02a:'25'
	// <0x58e558>|0x138|+0x003:'26'
	// ******
}

// STATE[BLOCKED]: Ghidra script didn't generate anything for `weapon_recoil_calculator`. Error is not returned at all.
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
	// FUNCTION BODY
	// ******
}

// STATE[BLOCKED]
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

		if ( m_additive_recoil_timer != 0.0 )
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

				float total_square_amount = math::sqr( m_target_horizontal_koef ) + math::sqr( m_target_vertical_koef );
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

	// FUNCTION BODY
	// <0x58e829>|0x009|+0x009:'58'
	// <0>
	// <0x58e832>|0x012|+0x009:'60'
	// <0x58e83b>|0x01b|+0x005:'61'
	// <0>
	// <1>
	// <0x58e840>|0x020|+0x00b:'64'
	// <0x58e84b>|0x02b|+0x005:'65'
	// <0>
	// <0x58e850>|0x030|+0x00d:'67'	if ( m_weapon )
	// <0>
	// <0x58e85d>|0x03d|+0x00c:'69'		u32 time_delta_in_ms = current_time_in_ms - m_last_time_in_ms;
	// <0x58e869>|0x049|+0x009:'70'		m_last_time_in_ms = current_time_in_ms;
	// <0x58e872>|0x052|+0x01c:'71'		float dt_sec = time_delta_in_ms * 0.001 * time_scale;
	// <0x58e88e>|0x06e|+0x015:'72'		m_time_since_shoot += dt_sec;
	// <0x58e8a3>|0x083|+0x015:'73'		m_time_since_last_dispersion_change += dt_sec;
	// <0x58e8b8>|0x098|+0x02a:'74'		m_time_since_last_dispersion_change = std::min( m_time_since_last_dispersion_change, m_interpolator.transition_time( ) );
	// <0x58e8e2>|0x0c2|+0x019:'75'		if ( m_additive_recoil_timer != 0.0 )
	// <0>
	// <0x58e8fb>|0x0db|+0x00e:'77'			if ( dt_sec < m_additive_recoil_timer )
	// <0>
	// <0x58e909>|0x0e9|+0x015:'79'
	// <0>
	// <0x58e91e>|0x0fe|+0x005:'81'			else
	// <0>									{
	// <0x58e923>|0x103|+0x00f|[2]:'83'
	// <0x58e932>|0x112|+0x01b:'84'				float additive_dispersion_angle_deg = weapon_params.additive_recoil_min_angle + get_random_angle( weapon_params.additive_recoil_angle_range );
	// <0x58e94d>|0x12d|+0x00f:'85'				float additive_dispersion_angle_rad = math::deg2rad( additive_dispersion_angle_deg );
	// <0x58e95c>|0x13c|+0x011:'86'				float force_coef = get_random_amount( 1.0f );
	// <0x58e96d>|0x14d|+0x01a:'87'				float additive_dispersion_amount = m_player_recoil_multiplier * force_koef * weapon_params.additive_side_recoil );
	// <0x58e987>|0x167|+0x01e:'88'				m_target_vertical_koef		= m_vertical_koef + math::cos( additive_dispersion_angle_rad ) * additive_dispersion_amount;
	// <0x58e9a5>|0x185|+0x01e:'89'				m_target_horizontal_koef	= m_horizontal_koef + math::sin( additive_dispersion_angle_rad ) * additive_dispersion_amount;
	// <0x58e9c3>|0x1a3|+0x029:'90'				float total_square_amount	= math::sqr( m_target_horizontal_koef ) + math::sqr( m_target_vertical_koef );
	// <0x58e9ec>|0x1cc|+0x00e:'91'
	// <0>
	// <0x58e9fa>|0x1da|+0x012:'93'					float total_amount = math::sqrt( total_square_amount );
	// <0x58ea0c>|0x1ec|+0x015:'94'					m_target_vertical_koef		/= total_amount;
	// <0x58ea21>|0x201|+0x015:'95'					m_target_horizontal_koef	/= total_amount;
	// <0>
	// <1>
	// <0x58ea36>|0x216|+0x012:'98'
	// <0x58ea48>|0x228|+0x032:'99'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x58ea7a>|0x25a|+0x010:'104'
	// <0x58ea8a>|0x26a|+0x032:'105'			m_back_koef				= std::max( m_back_koef - additive_dispersion_amount, 0.0f );
	// <0>
	// <1>
	// <0x58eabc>|0x29c|+0x01d:'108'
	// <0x58ead9>|0x2b9|+0x012:'109'
	// <0x58eaeb>|0x2cb|+0x026:'110'
	// <0x58eb11>|0x2f1|+0x026:'111'
	// <0x58eb37>|0x317|+0x026:'112'
	// <0>
	// <0x58eb5d>|0x33d|+0x00f:'114'
	// <0>
	// <1>
	// ******
}

// STATE[BLOCKED]: sushi@TODO: Why two recoil types? m_target_vertical_koef & m_target_horizontal_koef vs. m_target_recoil_koef
void weapon_recoil_calculator::fire( )
{
	weapon_recoil_params const& weapon_params = m_weapon->get_recoil_params( );
	float recoil_angle_deg	= weapon_params.shoot_recoil_min_angle + get_random_angle( weapon_params.shoot_recoil_angle_range );
	float force_koef		= get_random_amount( 1.0f );
	bool first_shoot		= m_target_vertical_koef == 0.0f && m_target_horizontal_koef == 0.0f;

	float recoil			= m_player_recoil_multiplier * force_koef * first_shoot ? weapon_params.first_shoot_side_recoil : weapon_params.shoot_side_recoil;
	float recoil_angle_rad	= math::deg2rad( recoil_angle_deg );

	m_target_vertical_koef		= m_vertical_koef + math::cos( recoil_angle_rad ) * recoil;
	m_target_horizontal_koef	= m_horizontal_koef + math::sin( recoil_angle_rad ) * recoil;

	float total_square_amount = math::sqr( m_target_horizontal_koef ) + math::sqr( m_target_vertical_koef );
	if ( total_square_amount > 1.0f )
	{
		float total_amount = math::sqrt( total_square_amount );
		m_target_vertical_koef	 /= total_amount;
		m_target_horizontal_koef /= total_amount;
	}

	float recoil_amount		= m_player_recoil_multiplier * force_koef * first_shoot ? weapon_params.first_shoot_back_recoil : weapon_params.shoot_back_recoil;
	m_target_recoil_koef	= math::min( m_target_recoil_koef + recoil_amount, 1.0f );

	m_time_since_last_dispersion_change = 0.0f;
	m_time_since_shoot					= 0.0f;
	m_additive_recoil_timer				= weapon_params.additive_recoil_time;

	// FUNCTION BODY
	// <0x58e609>|0x009|+0x00f:'121'
	// <0x58e618>|0x018|+0x01b:'122'
	// <0x58e633>|0x033|+0x011:'123'
	// <0x58e644>|0x044|+0x03a:'124'
	// <0x58e67e>|0x07e|+0x03a:'125'
	// <0>
	// <1>
	// <2>
	// <0x58e6b8>|0x0b8|+0x00f:'129'	float recoil_angle_deg	= math::deg2rad( recoil_angle_deg );
	// <0x58e6c7>|0x0c7|+0x01e:'130'	m_target_vertical_koef		= m_vertical_koef + math::cos( recoil_angle_rad ) * recoil;
	// <0x58e6e5>|0x0e5|+0x01e:'131'	m_target_horizontal_koef	= m_horizontal_koef + math::sin( recoil_angle_rad ) * recoil;
	// <0x58e703>|0x103|+0x029:'132'	float total_square_amount = math::sqr( m_target_horizontal_koe f) + math::sqr( m_target_vertical_koef );
	// <0x58e72c>|0x12c|+0x00e:'133'	if ( total_square_amount > 1.0f )
	// <0>
	// <0x58e73a>|0x13a|+0x012:'135'
	// <0x58e74c>|0x14c|+0x015:'136'
	// <0x58e761>|0x161|+0x015:'137'
	// <0>
	// <1>
	// <2>
	// <0x58e776>|0x176|+0x03b:'141'
	// <0x58e7b1>|0x1b1|+0x032:'142'
	// <0>
	// <0x58e7e3>|0x1e3|+0x010:'144'
	// <0x58e7f3>|0x1f3|+0x010:'145'
	// <0x58e803>|0x203|+0x00c:'146'
	// ******
}

// STATE[BLOCKED]
void weapon_recoil_calculator::reset( )
{
	m_time_since_last_dispersion_change	= 0.0f;
	m_time_since_shoot					= 0.0f;
	m_additive_recoil_timer 			= 0.0f;
	m_target_vertical_koef				= 0.0f;
	m_target_horizontal_koef 			= 0.0f;
	m_target_vertical_koef 				= 0.0f;
	m_target_horizontal_koef 			= 0.0f;


	// FUNCTION BODY
	// <0x58dfe7>|0x007|+0x010:'151'
	// <0x58dff7>|0x017|+0x010:'152'
	// <0x58e007>|0x027|+0x010:'153'
	// <0x58e017>|0x037|+0x010:'154'
	// <0x58e027>|0x047|+0x010:'155'
	// <0x58e037>|0x057|+0x010:'156'
	// <0x58e047>|0x067|+0x010:'157'
	// ******
}

// STATE[BLOCKED]
void weapon_recoil_calculator::reload( )
{
	reset( );

	// FUNCTION BODY
	// <0x58e087>|0x007|+0x008:'162'
	// ******
}

// STATE[BLOCKED]: sushi@TODO: What does that mean
void weapon_recoil_calculator::chamber_a_round( )
{
	reset( );

	// FUNCTION BODY
	// <0x58e067>|0x007|+0x008:'167'
	// ******
}

// STATE[BLOCKED]
void weapon_recoil_calculator::process_compensation( const float dt_sec )
{
	weapon_recoil_params const& weapon_params = m_weapon->get_recoil_params( );
	float additive_compensation_speed = math::sqrt( math::sqr( m_target_vertical_koef ) * math::sqr( m_target_horizontal_koef ) );
	float compenstion_amount = m_player_compensation_multiplier * dt_sec * ( weapon_params.side_compensation_speed + additive_compensation_speed );

	m_target_vertical_koef = compenstion_amount < math::abs( m_target_vertical_koef )
		? m_target_vertical_koef - ( math::sign( m_target_vertical_koef ) * compenstion_amount )
		: 0.0f;
	m_target_horizontal_koef = compenstion_amount < math::abs( m_target_horizontal_koef )
		? m_target_horizontal_koef - ( math::sign( m_target_horizontal_koef ) * compenstion_amount )
		: 0.0f;

	float additive_recoil_compensation_speed = math::sqrt( math::sqr( m_target_recoil_koef ) + math::sqr( m_target_recoil_koef ) );
	float recoil_compensation_amount = m_player_compensation_multiplier * dt_sec * ( weapon_params.back_compensation_speed + additive_recoil_compensation_speed );

	m_target_recoil_koef = recoil_compensation_amount < m_target_recoil_koef
		? m_target_recoil_koef - recoil_compensation_amount
		: 0.0f;

	// FUNCTION BODY
	// <0x58e159>|0x009|+0x00f:'172'
	// <0x58e168>|0x018|+0x035:'173'
	// <0x58e19d>|0x04d|+0x01f:'174'
	// <0x58e1bc>|0x06c|+0x05e:'175'
	// <0x58e21a>|0x0ca|+0x05e:'176'
	// <0>
	// <0x58e278>|0x128|+0x035:'178'
	// <0x58e2ad>|0x15d|+0x01f:'179'
	// <0x58e2cc>|0x17c|+0x03c:'180'
	// ******
}

// STATE[BLOCKED]
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

	// FUNCTION BODY
	// <0x58e579>|0x009|+0x00b:'185'
	// <0>
	// <0x58e584>|0x014|+0x045:'187'
	// <0x58e5c9>|0x059|+0x014:'188'
	// <0>
	// <0x58e5dd>|0x06d|+0x002:'190' else
	// <0>
	// <0x58e5df>|0x06f|+0x00f:'192'
	// <0>
	// ******
}

// STATE[BLOCKED]
float weapon_recoil_calculator::get_random_amount( const float range )
{
	float k = s_recoil_use_pseudo_random_value
		? ( 1.0f + math::sin( m_weapon->get_user( )->local_time( m_last_time_in_ms ) * 0.01f ) ) * 0.5f
		: m_random.random_f( 1.0f );
	float c_min_amaunt = math::max( 0.25, k ) * range;
	return c_min_amaunt;

	// FUNCTION BODY
	// <0x58e0a9>|0x009|+0x076:'198'
	// <0x58e11f>|0x07f|+0x00d:'199'
	// <0x58e12c>|0x08c|+0x01a:'200'
	// ******
}

// STATE[BLOCKED]
void weapon_recoil_calculator::set_weapon( weapon_core* weapon )
{
	m_weapon = weapon;

	// FUNCTION BODY
	// <0x58dfc7>|0x007|+0x009:'205'
	// ******
}

} // namespace survarium
