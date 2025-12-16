////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_recoil_calculator.h>

namespace survarium {

/*
// STATE[STUB]
// void survarium::`dynamic initializer for 's_recoil_use_pseudo_random_cc''()
void `dynamic initializer for 's_recoil_use_pseudo_random_cc''( )
{
	// FUNCTION BODY
	// <0x7db780>|0x000|      :'18'	{
	// ******
}
*/

// STATE[STUB]
// float survarium::pseudo_random::random_f(const float)
float pseudo_random::random_f( float range )
{
	// LOCALS
	// float 						t
	// float 						result
	// float 						k
	// float 						pi_x24
	// ******

	return 0.0f;

	// FUNCTION BODY
	// <0x58e429>|0x009|+0x015:'22'
	// <0x58e43e>|0x01e|+0x01b:'23'
	// <0x58e459>|0x039|+0x0d5:'24'
	// <0x58e52e>|0x10e|+0x02a:'25'
	// <0x58e558>|0x138|+0x003:'26'
	// ******
}

// STATE[STUB]
// survarium::weapon_recoil_calculator::weapon_recoil_calculator()
weapon_recoil_calculator::weapon_recoil_calculator( ) : m_pseudo_random( 0.0f ), m_interpolator( 0.0f )
{
	// FUNCTION BODY
	// <0x58e310>|0x000|+0x102:'46'	{
	// <0>
	// <0x58e412>|0x102|      :'48'	}
	// ******
}

// STATE[STUB]
// void survarium::weapon_recoil_calculator::tick(const unsigned int, const float)
void weapon_recoil_calculator::tick( u32 current_time_in_ms, float time_scale )
{
	// LOCALS
	// u32 							time_delta_in_ms<1>
	// float 						interpolated_value<1>
	// float 						one_minus_interpolated_value<1>
	// float 						dt_sec<1>
	// float 						additive_dispersion_angle_rad<2>
	// float 						additive_recoil_amount<2>
	// weapon_recoil_params const& 	weapon_params<2>
	// float 						total_square_amount<2>
	// float 						additive_dispersion_amount<2>
	// float 						additive_dispersion_angle_deg<2>
	// float 						force_koef<2>
	// float 						total_amount<3>
	// ******

	// SKIPPED BLOCKS
	// <0x58e857><1>
	// <0x58e9f8><3>
	// ******

	// CALL SITE INFO
	// <0x58e8c7> -> float <unknown>() const
	// <0x58ead4> -> float <unknown>(float) const
	// ******

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
	// <0x58e850>|0x030|+0x00d:'67'
	// <0>
	// <0x58e85d>|0x03d|+0x00c:'69'
	// <0x58e869>|0x049|+0x009:'70'
	// <0x58e872>|0x052|+0x01c:'71'
	// <0x58e88e>|0x06e|+0x015:'72'
	// <0x58e8a3>|0x083|+0x015:'73'
	// <0x58e8b8>|0x098|+0x02a:'74'
	// <0x58e8e2>|0x0c2|+0x019:'75'
	// <0>
	// <0x58e8fb>|0x0db|+0x00e:'77'
	// <0>
	// <0x58e909>|0x0e9|+0x015:'79'
	// <0>
	// <0x58e91e>|0x0fe|+0x005:'81'
	// <0>
	// <0x58e923>|0x103|+0x00f|[2]:'83'
	// <0x58e932>|0x112|+0x01b:'84'
	// <0x58e94d>|0x12d|+0x00f:'85'
	// <0x58e95c>|0x13c|+0x011:'86'
	// <0x58e96d>|0x14d|+0x01a:'87'
	// <0x58e987>|0x167|+0x01e:'88'
	// <0x58e9a5>|0x185|+0x01e:'89'
	// <0x58e9c3>|0x1a3|+0x029:'90'
	// <0x58e9ec>|0x1cc|+0x00e:'91'
	// <0>
	// <0x58e9fa>|0x1da|+0x012:'93'
	// <0x58ea0c>|0x1ec|+0x015:'94'
	// <0x58ea21>|0x201|+0x015:'95'
	// <0>
	// <1>
	// <0x58ea36>|0x216|+0x012:'98'
	// <0x58ea48>|0x228|+0x032:'99'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x58ea7a>|0x25a|+0x010:'104'
	// <0x58ea8a>|0x26a|+0x032:'105'
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

// STATE[STUB]
// void survarium::weapon_recoil_calculator::fire()
void weapon_recoil_calculator::fire( )
{
	// LOCALS
	// bool 						first_shoot
	// float 						recoil_angle_rad
	// weapon_recoil_params const& 	weapon_params
	// float 						total_square_amount
	// float 						force_koef
	// float 						recoil
	// float 						recoil_angle_deg
	// float 						recoil_amount
	// float 						total_amount<1>
	// ******

	// SKIPPED BLOCKS
	// <0x58e738><1>
	// ******

	// FUNCTION BODY
	// <0x58e609>|0x009|+0x00f:'121'
	// <0x58e618>|0x018|+0x01b:'122'
	// <0x58e633>|0x033|+0x011:'123'
	// <0x58e644>|0x044|+0x03a:'124'
	// <0x58e67e>|0x07e|+0x03a:'125'
	// <0>
	// <1>
	// <2>
	// <0x58e6b8>|0x0b8|+0x00f:'129'
	// <0x58e6c7>|0x0c7|+0x01e:'130'
	// <0x58e6e5>|0x0e5|+0x01e:'131'
	// <0x58e703>|0x103|+0x029:'132'
	// <0x58e72c>|0x12c|+0x00e:'133'
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

// STATE[STUB]
// void survarium::weapon_recoil_calculator::reset()
void weapon_recoil_calculator::reset( )
{
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

// STATE[STUB]
// void survarium::weapon_recoil_calculator::reload()
void weapon_recoil_calculator::reload( )
{
	// FUNCTION BODY
	// <0x58e087>|0x007|+0x008:'162'
	// ******
}

// STATE[STUB]
// void survarium::weapon_recoil_calculator::chamber_a_round()
void weapon_recoil_calculator::chamber_a_round( )
{
	// FUNCTION BODY
	// <0x58e067>|0x007|+0x008:'167'
	// ******
}

// STATE[STUB]
// void survarium::weapon_recoil_calculator::process_compensation(const float)
void weapon_recoil_calculator::process_compensation( float dt_sec )
{
	// LOCALS
	// weapon_recoil_params const& 	weapon_params
	// float 						additive_compensation_speed
	// float 						compenstion_amount
	// float 						recoil_compensation_amount
	// float 						additive_recoil_compensation_speed
	// ******

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

// STATE[STUB]
// float survarium::weapon_recoil_calculator::get_random_angle(const float)
float weapon_recoil_calculator::get_random_angle( float range )
{
	// CALL SITE INFO
	// <0x58e5a4> -> u32 <unknown>(const u32) const
	// ******

	return 0.0f;

	// FUNCTION BODY
	// <0x58e579>|0x009|+0x00b:'185'
	// <0>
	// <0x58e584>|0x014|+0x045:'187'
	// <0x58e5c9>|0x059|+0x014:'188'
	// <0>
	// <0x58e5dd>|0x06d|+0x002:'190'
	// <0>
	// <0x58e5df>|0x06f|+0x00f:'192'
	// <0>
	// ******
}

// STATE[STUB]
// float survarium::weapon_recoil_calculator::get_random_amount(const float)
float weapon_recoil_calculator::get_random_amount( float range )
{
	// LOCALS
	// float 						k
	// float 						c_min_amaunt
	// ******

	// CALL SITE INFO
	// <0x58e0d4> -> u32 <unknown>(const u32) const
	// ******

	return 0.0f;

	// FUNCTION BODY
	// <0x58e0a9>|0x009|+0x076:'198'
	// <0x58e11f>|0x07f|+0x00d:'199'
	// <0x58e12c>|0x08c|+0x01a:'200'
	// ******
}

// STATE[STUB]
// void survarium::weapon_recoil_calculator::set_weapon(survarium::weapon_core*)
void weapon_recoil_calculator::set_weapon( weapon_core* weapon )
{
	// FUNCTION BODY
	// <0x58dfc7>|0x007|+0x009:'205'
	// ******
}

} // namespace survarium
