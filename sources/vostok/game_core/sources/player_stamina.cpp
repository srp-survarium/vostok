////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/player_stamina.h>

#include <vostok/game_core/player_stamina_subscriber.h>

namespace survarium {

// STATE[94.32%|DONE]: LTCG for mutex
player_stamina::player_stamina( ) :
	m_max_value_factor				( 1.0f ),
	m_spending_speed_factor			( 1.0f ),
	m_regeneration_speed_factor		( 1.0f ),
	m_last_spending_time_in_ms		( 0 ),
	m_last_tick_time_in_ms			( 0 ),
	m_lower_threshold_was_reached	( false ),
	m_max_carried_weight			( 0.0f )
{
}

// STATE[90.87%|DONE]: LTCG for mutex
player_stamina::player_stamina( player_stamina const& other )
{
	*this = other;

	// FUNCTION BODY[0x5aae50]: 1
	// <0x5aae86>|0x036|+0x00c:'28'
	// ******
}

// STATE[100%|DONE]
player_stamina& player_stamina::operator=( player_stamina const& other )
{
	if ( this != &other )
	{
		m_max_value						= other.m_max_value;
		m_value							= other.m_value;
		m_spending_speed				= other.m_spending_speed;
		m_regeneration_speed			= other.m_regeneration_speed;
		m_max_value_factor				= other.m_max_value_factor;
		m_spending_speed_factor			= other.m_spending_speed_factor;
		m_last_tick_time_in_ms			= other.m_last_tick_time_in_ms;
		m_spending_threshold			= other.m_spending_threshold;
		m_regeneration_speed_factor		= other.m_regeneration_speed_factor;
		m_regeneration_threshold		= other.m_regeneration_threshold;
		m_last_spending_time_in_ms		= other.m_last_spending_time_in_ms;
		m_lower_threshold_was_reached	= other.m_lower_threshold_was_reached;
	}
	return *this;

	// FUNCTION BODY[0x5aacd0]: 17
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
	// ******
}

// STATE[BLOCKED]
void player_stamina::deserialize( network_core::packet_reader& packet )
{
	// FUNCTION BODY[0x5ab030]: 4
	// <0x5ab030>|0x000|+0x009:'67'	{
	// <0x5ab039>|0x009|+0x010:'68'
	// <0x5ab049>|0x019|+0x00e:'69'
	// <0x5ab057>|0x027|+0x00e:'70'
	// <0x5ab065>|0x035|+0x00e:'71'
	// <0x5ab073>|0x043|      :'72'	}
	// ******
}

// STATE[99.45%|DONE] LTCG for `binary_config_value::operator[]`.
void player_stamina::load( configs::binary_config_value const& config )
{
	m_spending_threshold		= (float)config["spending_threshold"];
	m_regeneration_threshold	= (float)config["regeneration_threshold"];

	float max_value				= (float)config["max_value"];
	m_max_value					= max_value;
	m_value						= m_max_value * m_max_value_factor;

	float spending_speed		= (float)config["spending_speed"];
	m_spending_speed			= spending_speed;

	float regeneration_speed	= (float)config["regeneration_speed"];
	m_regeneration_speed		= regeneration_speed;

	m_max_carried_weight		= (float)config["max_carried_weight"];


	// FUNCTION BODY[0x5aaf40]: 14
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
	// ******
}

// STATE[100%|DONE]
void player_stamina::reset( )
{
	m_value							= m_max_value * m_max_value_factor;
	m_lower_threshold_was_reached	= false;
	m_last_spending_time_in_ms		= 0;

	// FUNCTION BODY[0x5aac90]: 3
	// <0x5aac97>|0x007|+0x018:'102'
	// <0x5aacaf>|0x01f|+0x007:'103'
	// <0x5aacb6>|0x026|+0x00a:'104'
	// ******
}

// STATE[100%|DONE]
void player_stamina::set_regeneration_speed( float new_regeneration_speed )
{
	m_regeneration_speed = new_regeneration_speed;

	// FUNCTION BODY[0x5aac70]: 1
	// <0x5aac77>|0x007|+0x00d:'119'
	// ******
}

// STATE[100%|DONE]
void player_stamina::set_regeneration_speed_factor( float new_regeneration_speed_factor )
{
	m_regeneration_speed_factor = new_regeneration_speed_factor;

	// FUNCTION BODY[0x5aac50]: 1
	// <0x5aac57>|0x007|+0x00d:'134'
	// ******
}

// STATE[100%|DONE]
void player_stamina::increase_value( float amount )
{
	m_value = vostok::math::clamp_r( m_value + amount, 0.0f, m_max_value * m_max_value_factor );
	if ( m_value > m_spending_threshold )
		m_lower_threshold_was_reached = false;

	// FUNCTION BODY[0x5aad80]: 3
	// <0x5aad89>|0x009|+0x03b:'144'
	// <0x5aadc4>|0x044|+0x011:'145'
	// <0x5aadd5>|0x055|+0x007:'146'
	// ******
}

struct stamina_depletion_predicate : public boost::noncopyable {
	// STATE[INLINED]
	inline	void	operator()					( player_stamina_subscriber* subscriber ) const
	{
		subscriber->subscription_callback( );
	}

}; // struct stamina_depletion_predicate

// STATE[100%|DONE]
void player_stamina::decrease_value( float amount )
{
	m_value = vostok::math::clamp_r( m_value - amount, 0.0f, m_max_value * m_max_value_factor );

	if ( m_regeneration_threshold >= m_value )
	{
		m_lower_threshold_was_reached = true;
		m_subscribers.for_each( stamina_depletion_predicate() );
	}

	// FUNCTION BODY[0x5ab0d0]: 6
	// <0x5ab0df>|0x00f|+0x047:'159'
	// <0x5ab126>|0x056|+0x017:'160'
	// <0>
	// <0x5ab13d>|0x06d|+0x00a:'162'
	// <0x5ab147>|0x077|+0x01f:'163'
	// <0>
	// ******
}

// STATE[100%|DONE]
void player_stamina::tick( u32 current_time_in_ms, bool is_sprinting )
{
	if ( is_sprinting )
		sprint( current_time_in_ms );

	const u32 regeneration_interval_in_ms = 1000;
	if ( m_max_value * m_max_value_factor > m_value
		&& ( m_value == 0.0f || m_last_spending_time_in_ms && m_last_spending_time_in_ms + regeneration_interval_in_ms < current_time_in_ms ) )
		regenerate( current_time_in_ms );

	m_last_tick_time_in_ms = current_time_in_ms;


	// FUNCTION BODY[0x5ab200]: 8
	// <0x5ab209>|0x009|+0x008:'169'
	// <0x5ab211>|0x011|+0x00c:'170'
	// <0>
	// <0x5ab21d>|0x01d|+0x007:'172'
	// <0x5ab224>|0x024|+0x048:'173'
	// <0x5ab26c>|0x06c|+0x00c:'174'
	// <0>
	// <0x5ab278>|0x078|+0x009:'176'
	// ******
}

// STATE[100%|DONE]
void player_stamina::regenerate( u32 current_time_in_ms )
{
	if ( !m_last_tick_time_in_ms )
		return;

	float time_delta_in_sec = ( current_time_in_ms - m_last_tick_time_in_ms ) / 1000.0f;
	increase_value( time_delta_in_sec * m_regeneration_speed * m_regeneration_speed_factor );

	// FUNCTION BODY[0x5aadf0]: 5
	// <0x5aadf9>|0x009|+0x009:'181'
	// <0x5aae02>|0x012|+0x002:'182'
	// <0>
	// <0x5aae04>|0x014|+0x01f:'184'
	// <0x5aae23>|0x033|+0x023:'185'
	// ******
}

// STATE[100%|DONE]
void player_stamina::spend( float amount )
{
	decrease_value( amount );
	m_last_spending_time_in_ms = m_last_tick_time_in_ms;

	// FUNCTION BODY[0x5ab1d0]: 2
	// <0x5ab1d7>|0x007|+0x00f:'190'
	// <0x5ab1e6>|0x016|+0x00c:'191'
	// ******
}

// STATE[100%|DONE]
void player_stamina::sprint( u32 current_time_in_ms )
{
	float time_delta_in_sec = ( current_time_in_ms - m_last_tick_time_in_ms ) / 1000.0f;
	decrease_value( m_spending_speed * m_spending_speed_factor * time_delta_in_sec );
	m_last_spending_time_in_ms = current_time_in_ms;

	// FUNCTION BODY[0x5ab170]: 3
	// <0x5ab179>|0x009|+0x01f:'196'
	// <0x5ab198>|0x028|+0x023:'197'
	// <0x5ab1bb>|0x04b|+0x009:'198'
	// ******
}

// STATE[100%|DONE]
bool player_stamina::can_be_spent( ) const
{
	// return m_spending_threshold < m_value || !m_lower_threshold_was_reached;
	return m_spending_threshold >= m_value ? !m_lower_threshold_was_reached : true;

	// FUNCTION BODY[0x5aac10]: 1
	// <0x5aac19>|0x009|+0x02e:'203'
	// ******
}

// STATE[99.75%|DONE]
void player_stamina::subscribe_on_depletion( player_stamina_subscriber* const subscriber )
{
	m_subscribers.push_back( subscriber );

	// FUNCTION BODY[0x5ab0b0]: 1
	// <0x5ab0b9>|0x009|+0x00e:'208'
	// ******
}

// STATE[100%|DONE]
void player_stamina::unsubscribe_from_depletion( player_stamina_subscriber* const subscriber )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_subscribers.contains_object( subscriber ) ) );
	m_subscribers.erase( subscriber );

	// FUNCTION BODY[0x5ab080]: 2
	// <0x5ab089>|0x009|+0x00c:'213'
	// <0x5ab095>|0x015|+0x00c:'214'
	// ******
}

} // namespace survarium
