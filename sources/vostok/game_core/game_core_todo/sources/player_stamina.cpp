////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "player_stamina.h"

namespace survarium {

// STATE[STUB]
// survarium::player_stamina::player_stamina()
player_stamina::player_stamina( )
{
}

// STATE[STUB]
// survarium::player_stamina::player_stamina(survarium::player_stamina const&)
player_stamina::player_stamina( player_stamina const& other )
{
	// FUNCTION BODY
	// <0x5aae86>|0x000|0x000:'28'
	// ******
}

// STATE[STUB]
// survarium::player_stamina& survarium::player_stamina::operator=(survarium::player_stamina const&)
player_stamina& player_stamina::operator=( player_stamina const& other )
{
	// FUNCTION BODY
	// <0x5aacd7>|0x000|0x000:'33'
	// 1
	// <0x5aace3>|0x00c|0x00c:'35'
	// <0x5aacef>|0x018|0x00c:'36'
	// <0x5aacfb>|0x024|0x00c:'37'
	// <0x5aad07>|0x030|0x00c:'38'
	// <0x5aad13>|0x03c|0x00c:'39'
	// <0x5aad1f>|0x048|0x00c:'40'
	// <0x5aad2b>|0x054|0x00c:'41'
	// <0x5aad37>|0x060|0x00c:'42'
	// <0x5aad43>|0x06c|0x00c:'43'
	// <0x5aad4f>|0x078|0x00c:'44'
	// <0x5aad5b>|0x084|0x00c:'45'
	// <0x5aad67>|0x090|0x00c:'46'
	// 1
	// 2
	// <0x5aad73>|0x09c|0x00c:'49'
	// ******
}

// STATE[STUB]
// void survarium::player_stamina::deserialize(vostok::network_core::packet_reader&)
void player_stamina::deserialize( vostok::network_core::packet_reader& packet )
{
	// FUNCTION BODY
	// <0x5ab039>|0x000|0x000:'68'
	// <0x5ab049>|0x010|0x010:'69'
	// <0x5ab057>|0x01e|0x00e:'70'
	// <0x5ab065>|0x02c|0x00e:'71'
	// ******
}

// STATE[STUB]
// void survarium::player_stamina::load(vostok::configs::binary_config_value const&)
void player_stamina::load( vostok::configs::binary_config_value const& config )
{
	// LOCALS
	// float 						spending_speed
	// float 						max_value
	// float 						regeneration_speed
	// ******

	// FUNCTION BODY
	// <0x5aaf49>|0x000|0x000:'84'
	// <0x5aaf63>|0x01a|0x01a:'85'
	// 1
	// <0x5aaf7d>|0x034|0x01a:'87'
	// <0x5aaf94>|0x04b|0x017:'88'
	// <0x5aafa1>|0x058|0x00d:'89'
	// 1
	// <0x5aafb9>|0x070|0x018:'91'
	// <0x5aafd0>|0x087|0x017:'92'
	// 1
	// <0x5aafdd>|0x094|0x00d:'94'
	// <0x5aaff4>|0x0ab|0x017:'95'
	// 1
	// <0x5ab001>|0x0b8|0x00d:'97'
	// ******
}

// STATE[STUB]
// void survarium::player_stamina::reset()
void player_stamina::reset( )
{
	// FUNCTION BODY
	// <0x5aac97>|0x000|0x000:'102'
	// <0x5aacaf>|0x018|0x018:'103'
	// <0x5aacb6>|0x01f|0x007:'104'
	// ******
}

// STATE[STUB]
// void survarium::player_stamina::set_regeneration_speed(const float)
void player_stamina::set_regeneration_speed( float new_regeneration_speed )
{
	// FUNCTION BODY
	// <0x5aac77>|0x000|0x000:'119'
	// ******
}

// STATE[STUB]
// void survarium::player_stamina::set_regeneration_speed_factor(const float)
void player_stamina::set_regeneration_speed_factor( float new_regeneration_speed_factor )
{
	// FUNCTION BODY
	// <0x5aac57>|0x000|0x000:'134'
	// ******
}

// STATE[STUB]
// void survarium::player_stamina::increase_value(const float)
void player_stamina::increase_value( float amount )
{
	// FUNCTION BODY
	// <0x5aad89>|0x000|0x000:'144'
	// <0x5aadc4>|0x03b|0x03b:'145'
	// <0x5aadd5>|0x04c|0x011:'146'
	// ******
}

// STATE[STUB]
// void survarium::player_stamina::decrease_value(const float)
void player_stamina::decrease_value( float amount )
{
	// FUNCTION BODY
	// <0x5ab0df>|0x000|0x000:'159'
	// <0x5ab126>|0x047|0x047:'160'
	// 1
	// <0x5ab13d>|0x05e|0x017:'162'
	// <0x5ab147>|0x068|0x00a:'163'
	// 1
	// ******
}

// STATE[STUB]
// void survarium::player_stamina::tick(const unsigned int, const bool)
void player_stamina::tick( u32 current_time_in_ms, bool is_sprinting )
{
	// LOCALS
	// u32 							regeneration_interval_in_ms
	// ******

	// FUNCTION BODY
	// <0x5ab209>|0x000|0x000:'169'
	// <0x5ab211>|0x008|0x008:'170'
	// 1
	// <0x5ab21d>|0x014|0x00c:'172'
	// <0x5ab224>|0x01b|0x007:'173'
	// <0x5ab26c>|0x063|0x048:'174'
	// 1
	// <0x5ab278>|0x06f|0x00c:'176'
	// ******
}

// STATE[STUB]
// void survarium::player_stamina::regenerate(const unsigned int)
void player_stamina::regenerate( u32 current_time_in_ms )
{
	// LOCALS
	// float 						time_delta_in_sec
	// ******

	// FUNCTION BODY
	// <0x5aadf9>|0x000|0x000:'181'
	// <0x5aae02>|0x009|0x009:'182'
	// 1
	// <0x5aae04>|0x00b|0x002:'184'
	// <0x5aae23>|0x02a|0x01f:'185'
	// ******
}

// STATE[STUB]
// void survarium::player_stamina::spend(const float)
void player_stamina::spend( float amount )
{
	// FUNCTION BODY
	// <0x5ab1d7>|0x000|0x000:'190'
	// <0x5ab1e6>|0x00f|0x00f:'191'
	// ******
}

// STATE[STUB]
// void survarium::player_stamina::sprint(const unsigned int)
void player_stamina::sprint( u32 current_time_in_ms )
{
	// LOCALS
	// float 						time_delta_in_sec
	// ******

	// FUNCTION BODY
	// <0x5ab179>|0x000|0x000:'196'
	// <0x5ab198>|0x01f|0x01f:'197'
	// <0x5ab1bb>|0x042|0x023:'198'
	// ******
}

// STATE[STUB]
// bool survarium::player_stamina::can_be_spent() const
bool player_stamina::can_be_spent( ) const
{
	return false;
	// FUNCTION BODY
	// <0x5aac19>|0x000|0x000:'203'
	// ******
}

// STATE[STUB]
// void survarium::player_stamina::subscribe_on_depletion(survarium::player_stamina_subscriber* const)
void player_stamina::subscribe_on_depletion( player_stamina_subscriber* subscriber )
{
	// FUNCTION BODY
	// <0x5ab0b9>|0x000|0x000:'208'
	// ******
}

// STATE[STUB]
// void survarium::player_stamina::unsubscribe_from_depletion(survarium::player_stamina_subscriber* const)
void player_stamina::unsubscribe_from_depletion( player_stamina_subscriber* subscriber )
{
	// FUNCTION BODY
	// <0x5ab089>|0x000|0x000:'213'
	// <0x5ab095>|0x00c|0x00c:'214'
	// ******
}

} // namespace survarium
