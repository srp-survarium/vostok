// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/player_stamina.h>

#include <vostok/game_core/player_stamina_subscriber.h>
#include <vostok/network_core/packet_reader.h>

namespace survarium {

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

player_stamina::player_stamina( player_stamina const& other )
{
	*this = other;
}

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
}

// claude@NOTE: structure is correct (one read per member, offsets 0x34/0x54/0x58/0x5c,
// types float/u32/u32/bool, order matches target asm). Residual is the inline-vs-call
// wall: target out-lines packet_reader::r<T>() as self-contained COMDATs and CALLs them
// (r<float> rva 0x7e960 reads m_pointer directly, no r(void*,u32,u32) delegation); under
// game_core's /Od /Ob2 /GL our header's r<T>() (which delegates to r(void*,..)) is inlined
// into every caller instead. Not forceable from this caller - the fix lives in
// network_core/packet_reader_inline.h (different unit). See patterns/inline-vs-call-template-comdat.md.
void player_stamina::deserialize( network_core::packet_reader& packet )
{
	m_value							= packet.r< float >( );
	m_last_spending_time_in_ms		= packet.r< u32 >( );
	m_last_tick_time_in_ms			= packet.r< u32 >( );
	m_lower_threshold_was_reached	= packet.r< bool >( );
}

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

void player_stamina::reset( )
{
	m_value							= m_max_value * m_max_value_factor;
	m_lower_threshold_was_reached	= false;
	m_last_spending_time_in_ms		= 0;
}

void player_stamina::set_regeneration_speed( const float new_regeneration_speed )
{
	m_regeneration_speed = new_regeneration_speed;
}

void player_stamina::set_regeneration_speed_factor( const float new_regeneration_speed_factor )
{
	m_regeneration_speed_factor = new_regeneration_speed_factor;
}

void player_stamina::increase_value( const float amount )
{
	m_value = vostok::math::clamp_r( m_value + amount, 0.0f, m_max_value * m_max_value_factor );
	if ( m_value > m_spending_threshold )
		m_lower_threshold_was_reached = false;
}

struct stamina_depletion_predicate : private boost::noncopyable {
	inline	void	operator()					( player_stamina_subscriber* const subscriber ) const
	{
		subscriber->subscription_callback( );
	}

}; // struct stamina_depletion_predicate

void player_stamina::decrease_value( const float amount )
{
	m_value = vostok::math::clamp_r( m_value - amount, 0.0f, m_max_value * m_max_value_factor );

	if ( m_regeneration_threshold >= m_value )
	{
		m_lower_threshold_was_reached = true;
		m_subscribers.for_each( stamina_depletion_predicate() );
	}
}

void player_stamina::tick( const u32 current_time_in_ms, const bool is_sprinting )
{
	if ( is_sprinting )
		sprint( current_time_in_ms );

	const u32 regeneration_interval_in_ms = 1000;
	if ( m_max_value * m_max_value_factor > m_value
		&& ( m_value == 0.0f || m_last_spending_time_in_ms && m_last_spending_time_in_ms + regeneration_interval_in_ms < current_time_in_ms ) )
		regenerate( current_time_in_ms );

	m_last_tick_time_in_ms = current_time_in_ms;
}

void player_stamina::regenerate( const u32 current_time_in_ms )
{
	if ( !m_last_tick_time_in_ms )
		return;

	float time_delta_in_sec = ( current_time_in_ms - m_last_tick_time_in_ms ) / 1000.0f;
	increase_value( time_delta_in_sec * m_regeneration_speed * m_regeneration_speed_factor );
}

void player_stamina::spend( const float amount )
{
	decrease_value( amount );
	m_last_spending_time_in_ms = m_last_tick_time_in_ms;
}

void player_stamina::sprint( const u32 current_time_in_ms )
{
	float time_delta_in_sec = ( current_time_in_ms - m_last_tick_time_in_ms ) / 1000.0f;
	decrease_value( m_spending_speed * m_spending_speed_factor * time_delta_in_sec );
	m_last_spending_time_in_ms = current_time_in_ms;
}

bool player_stamina::can_be_spent( ) const
{
	// return m_spending_threshold < m_value || !m_lower_threshold_was_reached;
	return m_spending_threshold >= m_value ? !m_lower_threshold_was_reached : true;
}

void player_stamina::subscribe_on_depletion( player_stamina_subscriber* const subscriber )
{
	m_subscribers.push_back( subscriber );
}

void player_stamina::unsubscribe_from_depletion( player_stamina_subscriber* const subscriber )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_subscribers.contains_object( subscriber ) ) );
	m_subscribers.erase( subscriber );
}

} // namespace survarium
