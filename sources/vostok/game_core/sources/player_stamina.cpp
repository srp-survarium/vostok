////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/player_stamina.h>

#include <vostok/game_core/player_stamina_subscriber.h>
#include <vostok/network_core/packet_reader.h>

namespace survarium {

// STATE[100%|DONE]
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

// STATE[100%|DONE]
player_stamina::player_stamina( player_stamina const& other )
{
	*this = other;
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

	// FUNCTION BODY
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

// STATE[0.00%|PARTIAL]: anchored in temp_include_all (was DCE'd/unpaired); every statement is
// the r<T> cross-module wall - base inlines packet_reader::r<T>, target keeps it out-of-line -
// so the fuzzy score bottoms out despite the aligned structure (same wall as
// hand_to_weapon_ik_processor::deserialize at 10.34%).
void player_stamina::deserialize( network_core::packet_reader& packet )
{
	m_value							= packet.r< float >( );
	m_last_spending_time_in_ms		= packet.r< u32 >( );
	m_last_tick_time_in_ms			= packet.r< u32 >( );
	m_lower_threshold_was_reached	= packet.r< bool >( );

	// STRUCTURE DIFF: target 4 stmts / base 4 stmts
	// SIZE +0x1b | 86 | m_value                       = packet.r< float >( );
	// SIZE +0x15 | 87 | m_last_spending_time_in_ms    = packet.r< u32 >( );
	// SIZE +0x15 | 88 | m_last_tick_time_in_ms        = packet.r< u32 >( );
	// SIZE +0x15 | 89 | m_lower_threshold_was_reached = packet.r< bool >( );
	// VERDICT: STRUCTURE MATCH (4/4) - all SIZE rows are r<T> kept out-of-line in target vs LTCG-inlined in base; cross-module wall, non-steerable.
}

// STATE[100%|DONE]
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
}

// STATE[100%|DONE]
void player_stamina::reset( )
{
	m_value							= m_max_value * m_max_value_factor;
	m_lower_threshold_was_reached	= false;
	m_last_spending_time_in_ms		= 0;

	// FUNCTION BODY
	// <0x5aac97>|0x007|+0x018:'102'
	// <0x5aacaf>|0x01f|+0x007:'103'
	// <0x5aacb6>|0x026|+0x00a:'104'
	// ******
}

// STATE[100%|DONE]
void player_stamina::set_regeneration_speed( float new_regeneration_speed )
{
	m_regeneration_speed = new_regeneration_speed;

	// FUNCTION BODY
	// <0x5aac77>|0x007|+0x00d:'119'
	// ******
}

// STATE[100%|DONE]
void player_stamina::set_regeneration_speed_factor( float new_regeneration_speed_factor )
{
	m_regeneration_speed_factor = new_regeneration_speed_factor;

	// FUNCTION BODY
	// <0x5aac57>|0x007|+0x00d:'134'
	// ******
}

// STATE[100%|DONE]
void player_stamina::increase_value( float amount )
{
	m_value = vostok::math::clamp_r( m_value + amount, 0.0f, m_max_value * m_max_value_factor );
	if ( m_value > m_spending_threshold )
		m_lower_threshold_was_reached = false;

	// FUNCTION BODY
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

	// FUNCTION BODY
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


	// FUNCTION BODY
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

	// FUNCTION BODY
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

	// FUNCTION BODY
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

	// FUNCTION BODY
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

	// FUNCTION BODY
	// <0x5aac19>|0x009|+0x02e:'203'
	// ******
}

// STATE[99.75%|DONE]: push_back call reloc ICF fold-name + 4B frame pad, non-steerable
void player_stamina::subscribe_on_depletion( player_stamina_subscriber* const subscriber )
{
	m_subscribers.push_back( subscriber );

	// STRUCTURE DIFF: target 1 stmts / base 1 stmts
	// VERDICT: STRUCTURE MATCH - identical stream 0x1d both; residual is base frame sub esp,14h vs 10h + push_back reloc ICF-folded onto a sibling instantiation, non-steerable.
}

// STATE[100%|DONE]
void player_stamina::unsubscribe_from_depletion( player_stamina_subscriber* const subscriber )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_subscribers.contains_object( subscriber ) ) );
	m_subscribers.erase( subscriber );

	// FUNCTION BODY
	// <0x5ab089>|0x009|+0x00c:'213'
	// <0x5ab095>|0x015|+0x00c:'214'
	// ******
}

} // namespace survarium
