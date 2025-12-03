////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/player_stamina.h>

namespace survarium {

// STATE[STUB]
// survarium::player_stamina::player_stamina()
player_stamina::player_stamina( )
{
	// FUNCTION BODY
	// <0x5aaea0>|0x000|+0x091:'23'	{
	// <0x5aaf31>|0x091|      :'24'	}
	// ******
}

// STATE[STUB]
// survarium::player_stamina::player_stamina(survarium::player_stamina const&)
player_stamina::player_stamina( player_stamina const& other )
{
	// FUNCTION BODY
	// <0x5aae50>|0x000|+0x036:'27'	{
	// <0x5aae86>|0x036|+0x00c:'28'
	// <0x5aae92>|0x042|      :'29'	}
	// ******
}

// STATE[STUB]
// survarium::player_stamina& survarium::player_stamina::operator=(survarium::player_stamina const&)
player_stamina& player_stamina::operator=( player_stamina const& other )
{
	// FUNCTION BODY
	// <0x5aacd0>|0x000|+0x007:'32'	{
	// <0x5aacd7>|0x007|+0x00c:'33'
	// <0>
	// <0x5aace3>|0x013|+0x00c:'35'
	// <0x5aacef>|0x01f|+0x00c:'36'
	// <0x5aacfb>|0x02b|+0x00c:'37'
	// <0x5aad07>|0x037|+0x00c:'38'
	// <0x5aad13>|0x043|+0x00c:'39'
	// <0x5aad1f>|0x04f|+0x00c:'40'
	// <0x5aad2b>|0x05b|+0x00c:'41'
	// <0x5aad37>|0x067|+0x00c:'42'
	// <0x5aad43>|0x073|+0x00c:'43'
	// <0x5aad4f>|0x07f|+0x00c:'44'
	// <0x5aad5b>|0x08b|+0x00c:'45'
	// <0x5aad67>|0x097|+0x00c:'46'
	// <0>
	// <1>
	// <0x5aad73>|0x0a3|+0x003:'49'
	// <0x5aad76>|0x0a6|      :'50'	}
	// ******
}

// STATE[STUB]
// void survarium::player_stamina::deserialize(vostok::network_core::packet_reader&)
void player_stamina::deserialize( network_core::packet_reader& packet )
{
	// FUNCTION BODY
	// <0x5ab030>|0x000|+0x009:'67'	{
	// <0x5ab039>|0x009|+0x010:'68'
	// <0x5ab049>|0x019|+0x00e:'69'
	// <0x5ab057>|0x027|+0x00e:'70'
	// <0x5ab065>|0x035|+0x00e:'71'
	// <0x5ab073>|0x043|      :'72'	}
	// ******
}

// STATE[STUB]
// void survarium::player_stamina::load(vostok::configs::binary_config_value const&)
void player_stamina::load( configs::binary_config_value const& config )
{
	// LOCALS
	// float 						spending_speed
	// float 						max_value
	// float 						regeneration_speed
	// ******

	// FUNCTION BODY
	// <0x5aaf40>|0x000|+0x009:'83'	{
	// <0x5aaf49>|0x009|+0x01a:'84'
	// <0x5aaf63>|0x023|+0x01a:'85'
	// <0>
	// <0x5aaf7d>|0x03d|+0x017:'87'
	// <0x5aaf94>|0x054|+0x00d:'88'
	// <0x5aafa1>|0x061|+0x018:'89'
	// <0>
	// <0x5aafb9>|0x079|+0x017:'91'
	// <0x5aafd0>|0x090|+0x00d:'92'
	// <0>
	// <0x5aafdd>|0x09d|+0x017:'94'
	// <0x5aaff4>|0x0b4|+0x00d:'95'
	// <0>
	// <0x5ab001>|0x0c1|+0x01a:'97'
	// <0x5ab01b>|0x0db|      :'98'	}
	// ******
}

// STATE[STUB]
// void survarium::player_stamina::reset()
void player_stamina::reset( )
{
	// FUNCTION BODY
	// <0x5aac90>|0x000|+0x007:'101'	{
	// <0x5aac97>|0x007|+0x018:'102'
	// <0x5aacaf>|0x01f|+0x007:'103'
	// <0x5aacb6>|0x026|+0x00a:'104'
	// <0x5aacc0>|0x030|      :'105'	}
	// ******
}

// STATE[STUB]
// void survarium::player_stamina::set_regeneration_speed(const float)
void player_stamina::set_regeneration_speed( float new_regeneration_speed )
{
	// FUNCTION BODY
	// <0x5aac70>|0x000|+0x007:'118'	{
	// <0x5aac77>|0x007|+0x00d:'119'
	// <0x5aac84>|0x014|      :'120'	}
	// ******
}

// STATE[STUB]
// void survarium::player_stamina::set_regeneration_speed_factor(const float)
void player_stamina::set_regeneration_speed_factor( float new_regeneration_speed_factor )
{
	// FUNCTION BODY
	// <0x5aac50>|0x000|+0x007:'133'	{
	// <0x5aac57>|0x007|+0x00d:'134'
	// <0x5aac64>|0x014|      :'135'	}
	// ******
}

// STATE[STUB]
// void survarium::player_stamina::increase_value(const float)
void player_stamina::increase_value( float amount )
{
	// FUNCTION BODY
	// <0x5aad80>|0x000|+0x009:'143'	{
	// <0x5aad89>|0x009|+0x03b:'144'
	// <0x5aadc4>|0x044|+0x011:'145'
	// <0x5aadd5>|0x055|+0x007:'146'
	// <0x5aaddc>|0x05c|      :'147'	}
	// ******
}

// STATE[STUB]
// void survarium::player_stamina::decrease_value(const float)
void player_stamina::decrease_value( float amount )
{
	// FUNCTION BODY
	// <0x5ab0d0>|0x000|+0x00f:'158'	{
	// <0x5ab0df>|0x00f|+0x047:'159'
	// <0x5ab126>|0x056|+0x017:'160'
	// <0>
	// <0x5ab13d>|0x06d|+0x00a:'162'
	// <0x5ab147>|0x077|+0x01f:'163'
	// <0>
	// <0x5ab166>|0x096|      :'165'	}
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
	// <0x5ab200>|0x000|+0x009:'168'	{
	// <0x5ab209>|0x009|+0x008:'169'
	// <0x5ab211>|0x011|+0x00c:'170'
	// <0>
	// <0x5ab21d>|0x01d|+0x007:'172'
	// <0x5ab224>|0x024|+0x048:'173'
	// <0x5ab26c>|0x06c|+0x00c:'174'
	// <0>
	// <0x5ab278>|0x078|+0x009:'176'
	// <0x5ab281>|0x081|      :'177'	}
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
	// <0x5aadf0>|0x000|+0x009:'180'	{
	// <0x5aadf9>|0x009|+0x009:'181'
	// <0x5aae02>|0x012|+0x002:'182'
	// <0>
	// <0x5aae04>|0x014|+0x01f:'184'
	// <0x5aae23>|0x033|+0x023:'185'
	// <0x5aae46>|0x056|      :'186'	}
	// ******
}

// STATE[STUB]
// void survarium::player_stamina::spend(const float)
void player_stamina::spend( float amount )
{
	// FUNCTION BODY
	// <0x5ab1d0>|0x000|+0x007:'189'	{
	// <0x5ab1d7>|0x007|+0x00f:'190'
	// <0x5ab1e6>|0x016|+0x00c:'191'
	// <0x5ab1f2>|0x022|      :'192'	}
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
	// <0x5ab170>|0x000|+0x009:'195'	{
	// <0x5ab179>|0x009|+0x01f:'196'
	// <0x5ab198>|0x028|+0x023:'197'
	// <0x5ab1bb>|0x04b|+0x009:'198'
	// <0x5ab1c4>|0x054|      :'199'	}
	// ******
}

// STATE[STUB]
// bool survarium::player_stamina::can_be_spent() const
bool player_stamina::can_be_spent( ) const
{
	return false;
	// FUNCTION BODY
	// <0x5aac10>|0x000|+0x009:'202'	{
	// <0x5aac19>|0x009|+0x02e:'203'
	// <0x5aac47>|0x037|      :'204'	}
	// ******
}

// STATE[STUB]
// void survarium::player_stamina::subscribe_on_depletion(survarium::player_stamina_subscriber* const)
void player_stamina::subscribe_on_depletion( player_stamina_subscriber* subscriber )
{
	// FUNCTION BODY
	// <0x5ab0b0>|0x000|+0x009:'207'	{
	// <0x5ab0b9>|0x009|+0x00e:'208'
	// <0x5ab0c7>|0x017|      :'209'	}
	// ******
}

// STATE[STUB]
// void survarium::player_stamina::unsubscribe_from_depletion(survarium::player_stamina_subscriber* const)
void player_stamina::unsubscribe_from_depletion( player_stamina_subscriber* subscriber )
{
	// FUNCTION BODY
	// <0x5ab080>|0x000|+0x009:'212'	{
	// <0x5ab089>|0x009|+0x00c:'213'
	// <0x5ab095>|0x015|+0x00c:'214'
	// <0x5ab0a1>|0x021|      :'215'	}
	// ******
}

} // namespace survarium
