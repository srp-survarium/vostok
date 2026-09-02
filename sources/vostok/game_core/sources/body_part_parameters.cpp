// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.08.2025
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include <vostok/game_core/body_part_parameters.h>

#include <vostok/game_core/affects_applying_type_enum.h>
#include <vostok/game_core/damage_model.h>
#include <vostok/game_core/damage_protector.h>

#include <vostok/ai/npc_statistics.h>
#include <vostok/network_core/udp_match_packet.h>
#include <vostok/network_core/packet_reader.h>
#include <boost/bind.hpp>

namespace survarium {

body_part_parameters::body_part_parameters(
	pcstr				name,
	const float				health,
	const float				regeneration_speed,
	const float				regeneration_timeout,
	const bool				can_be_assigned,
	damage_model&		owner,
	const u8					damage_group
) :
	next					( NULL ),
	m_damage_model			( owner ),
	m_name					( name ), // TODO: check
	m_max_health			( health ),
	m_health				( health ),
	m_regeneration_speed	( regeneration_speed ),
	m_last_hit_time			( 0 ),
	m_last_hit_health		( health ),
	m_assignable			( can_be_assigned ),
	m_damage_group			( damage_group )
{
	m_regeneration_timeout = math::floor( 1000.0f * regeneration_timeout );
}

hit_type_parameters* body_part_parameters::pop_hit_type( )
{
	return m_hit_types.empty( ) ? NULL : m_hit_types.pop_front( );
}

affects_threshold* body_part_parameters::pop_threshold( )
{
	return m_thresholds.empty( ) ? NULL : m_thresholds.pop_front( );
}

void body_part_parameters::add_hit_type( hit_type_parameters* const new_hit_type )
{
	m_hit_types.push_back( new_hit_type );
}

void body_part_parameters::add_threshold( affects_threshold* const new_threshold )
{
	m_thresholds.push_back( new_threshold );
}

struct find_hit_parameters_by_type_predicate : private boost::noncopyable {
public:
	explicit		find_hit_parameters_by_type_predicate	( pcstr hit_t ) : hit_type( hit_t ) {  }

	// sushi@TODO: This method seems to be inlined into `find_if`, so we need to match that instead
	inline	bool	operator()								( hit_type_parameters* const hit_type_params ) const {
		return strings::equal( hit_type, hit_type_params->get_type( ) );
	}

public:
	/* 0x0000 */	pcstr	hit_type;
}; // struct find_hit_parameters_by_type_predicate

hit_type_parameters* body_part_parameters::get_hit_parameters( pcstr hit_type ) const
{
	find_hit_parameters_by_type_predicate find_predicate( hit_type );
	return m_hit_types.find_if<find_hit_parameters_by_type_predicate>( find_predicate );
}

struct protect_damage_predicate : private boost::noncopyable {
public:
	explicit			protect_damage_predicate	(
							float		armor_piercing,
							pcstr		damage_type,
							pcstr		body_type_name,
							float		amount
						) :
							m_body_type_name	( body_type_name ),
							m_damage_type		( damage_type ),
							m_armor_piercing	( armor_piercing ),
							m_amount			( amount ) {}

	inline		void	operator()					( damage_protector* const protector ) {
					if ( m_amount > 0.0f && protector->reduce_damage_functor )
						m_amount = protector->reduce_damage_functor( m_body_type_name, m_damage_type, m_amount, m_armor_piercing );
				}

public:
	/* 0x0000 */	pcstr		m_body_type_name;
	/* 0x0004 */	pcstr		m_damage_type;
	/* 0x0008 */	const float	m_armor_piercing;
	/* 0x000c */	float		m_amount;
}; // struct protect_damage_predicate

STATIC_SIZE_ASSERT(protect_damage_predicate, 0x10);

struct protect_affect_predicate : private boost::noncopyable {
public:
	explicit				protect_affect_predicate	( pcstr body_type_name, hit_affects_type_enum affect_type ) :
								m_body_type_name	( body_type_name ),
								m_affect_type		( affect_type ),
								m_result			( false ) { }

	inline		void		operator()					( damage_protector* const protector ) {
		if ( !m_result && protector->protect_affect_functor )
			m_result = protector->protect_affect_functor( m_body_type_name, m_affect_type );
	}

public:
	/* 0x0000 */	pcstr						m_body_type_name;
	/* 0x0004 */	hit_affects_type_enum		m_affect_type;
	/* 0x0008 */	bool						m_result;
}; // struct protect_affect_predicate

STATIC_SIZE_ASSERT(protect_affect_predicate, 0xC);

static float g_arp_arm_coeff = 1.0;

void body_part_parameters::hit_by_type(
	pcstr				hit_type,
	const u32					time_in_ms,
	const float				amount,
	const float				armor_piercing,
	const bool				__formal,
	damage_protector*	prot
)
{
	hit_type_parameters* params = get_hit_parameters( hit_type );
	ASSERT( UNKNOWN_EXPRESSION_T( params ) );

	float delta = amount;

	float arp_arm_coeff;
	if ( params->get_armor( ) == 0.0f )
		arp_arm_coeff = 1.0f; // g_arp_arm_coeff;
	else
		arp_arm_coeff = math::min( 1.0f, ( armor_piercing / params->get_armor( ) ) - 1.0f );

	float e_wnd = math::max( 0.0f, arp_arm_coeff );

	delta = delta * e_wnd
	  + math::max(
		0.0f,
		( ( 1.0f - params->get_reduce( ) ) * delta ) * ( 1.0f - e_wnd ) - params->get_absorption( )
	);

	protect_damage_predicate p( armor_piercing, hit_type, m_name.c_str( ), delta );
	m_damage_protectors.for_each( p );
	delta = math::max( 0.0f, p.m_amount );

	if ( prot )
	{
		delta = math::max( 0.0f, prot->reduce_damage_functor( m_name.c_str(), hit_type, delta, armor_piercing ) );
	}

	decrease_health( delta );
	m_last_hit_health = m_health;
	m_last_hit_time = time_in_ms;

	if ( m_damage_model.get_affects_applying_type( ) == type_apply_directly )
		check_affects( time_in_ms );

	params->apply_damage( delta, time_in_ms );
}

void body_part_parameters::increase_health( const float amount )
{
	m_health = math::clamp_r<float>( m_health + amount, 0.0f, m_max_health );
}

void body_part_parameters::decrease_health( const float amount )
{
	m_health = math::clamp_r<float>( m_health - amount, 0.0f, m_max_health );
}

// Regenerates health and removes negative affects if they have passed.
//
// # Arguments
// * `time_delta_ms` - frame duration.
//
void body_part_parameters::regenerate( const u32 time_delta_ms, const u32 current_time_in_ms )
{
	u32 regenerate_delta = time_delta_ms;

	if ( m_regeneration_timeout )
	{
		const u32 next_regen_time = m_last_hit_time + m_regeneration_timeout;

		if ( current_time_in_ms <= next_regen_time )
			return;

		const u32 regen_allowed = current_time_in_ms - next_regen_time;
		regenerate_delta = math::min( regen_allowed, time_delta_ms );
		// sushi@NOTE: There is an ifdef of some kind
	}

	const float amount = regenerate_delta * m_regeneration_speed / 1000.0f;
	increase_health( amount );

	if ( m_damage_model.get_affects_applying_type( ) == type_apply_directly ) // sushi@TODO: Needs getter
		update_affects( current_time_in_ms );
}

void body_part_parameters::update_affects( const u32 current_time_in_ms )
{
	for ( s32 i = m_affects.size( ) - 1 ; i >= 0; --i )
	{
		std::pair<hit_affects_type_enum, u32>* it_affect = m_affects.begin( ); // Why write it this way
		if ( m_affects[i].second <= current_time_in_ms )
		{
			it_affect += i;
			m_damage_model.notify_on_affect_event( m_name.c_str( ), it_affect->first, affect_recalling );
			m_affects.erase( it_affect );
		}
	}
}

void body_part_parameters::cancel_affect_by_force( const hit_affects_type_enum affect )
{
	for ( s32 i = m_affects.size( ) - 1 ; i >= 0 ; --i )
	{
		std::pair<hit_affects_type_enum, u32>* it_affect = m_affects.begin( ); // Why write it this way
		if ( m_affects[i].first == affect )
		{
			it_affect += i;
			m_damage_model.notify_on_affect_event( m_name.c_str( ), it_affect->first, affect_canceling );
			m_affects.erase( it_affect );
		}
	}
}

// Applies affects to all bodyparts. sushi@TODO: There is apply_affects also.
//
void body_part_parameters::check_affects( const u32 current_time_in_ms )
{
	for ( affects_threshold* it_threshold = m_thresholds.front( ) ; it_threshold ; it_threshold = m_thresholds.get_next_of_object( it_threshold ) )
		if ( m_health <= m_max_health * it_threshold->value( ) )
		{
			it_threshold->bodypart( )->apply_affects( it_threshold, current_time_in_ms );
		}
}

bool body_part_parameters::is_affect_applied( const hit_affects_type_enum	affect )
{
	for ( u32 i = 0; i < m_affects.size( ); ++i )
	{
		if ( m_affects.at( i ).first == affect ) return true;
	}

	return false;
}

bool body_part_parameters::has_affect_protector( const hit_affects_type_enum affect )
{
	protect_affect_predicate p( m_name.c_str( ), affect );
	m_damage_protectors.for_each( p );
	return p.m_result;
}

void body_part_parameters::apply_affects( affects_threshold const* threshold_reached, const u32 current_time_in_ms )
{
	hit_affects_type_enum const* it_begin = threshold_reached->get_affects( );
	hit_affects_type_enum const* it_end = it_begin + threshold_reached->get_affects_count( );
	for ( hit_affects_type_enum const* it = it_begin ; it != it_end ; ++it )
		if ( !is_affect_applied( *it ) && !has_affect_protector( *it ) )
		{
			m_damage_model.notify_on_affect_event( m_name.c_str( ), *it, affect_applying );
			m_affects.push_back( std::make_pair( *it, current_time_in_ms + 1000 * affects_durations[*it] ) );
		}
}

void body_part_parameters::apply_affect_by_force(
	const hit_affects_type_enum		affect,
	const affect_event_type_enum	event_type,
	const u32						current_time_in_ms
)
{
	if ( event_type == affect_applying && !is_affect_applied( affect ) )
	{
		m_damage_model.notify_on_affect_event( m_name.c_str( ), affect, event_type );
		m_affects.push_back( std::make_pair( affect, current_time_in_ms + 1000 * affects_durations[affect] ) );	// sushi@TODO: Most likely inlined helper. Used in multiple places

	}
	else if ( event_type == affect_recalling )
	{
		std::pair< enum hit_affects_type_enum, u32 >* it_affect = m_affects.begin( );
		for ( u32 i = 0 ; i < m_affects.size() ; ++i )
		{
			if ( m_affects[i].first == affect )
			{
				it_affect += i;
				m_damage_model.notify_on_affect_event( m_name.c_str( ), affect, affect_recalling );
				m_affects.erase( it_affect );
				return;
			}
		}
	}
}

template < class stats_item_type >
void body_part_parameters::fill_new_stats_item( stats_item_type& new_stats_item, const u32 current_time_in_ms ) const
{
	new_stats_item.caption = m_name;
	new_stats_item.caption.append( " state:" );

	vostok::fixed_string<46> new_item;
	new_item.appendf( "HP: %4.2f/%4.2f", m_health, m_max_health );
	new_stats_item.content.push_back( new_item );

	new_item.clear( );
	new_item.append( "affects: " );
	new_stats_item.content.push_back( new_item );

	for ( u32 i = 0 ; i < m_affects.size( ) ; ++i )
	{
		new_item.clear( );
		const u32 remaining_time_in_ms = m_affects[i].second >= current_time_in_ms ? m_affects[i].second - current_time_in_ms : 0;
		new_item.appendf( "%s [%4.2f seconds left]", affects_captions[m_affects[i].first], remaining_time_in_ms / 1000.0f );
		new_stats_item.content.push_back( new_item );
	}

	if ( m_affects.empty( ) )
		new_stats_item.content.push_back( vostok::fixed_string<46>( "none" ) );
}

template void body_part_parameters::fill_new_stats_item<vostok::ai::statistics_item<46,16> >(
	vostok::ai::statistics_item<46,16>& new_stats_item, const u32 current_time_in_ms ) const;

void body_part_parameters::dump_state( vostok::ai::npc_statistics& stats, const u32 current_time_in_ms ) const
{
	typedef vostok::ai::statistics_item<46,16> content_type;
	content_type new_stats_item = content_type( );
	fill_new_stats_item( new_stats_item, current_time_in_ms );
	stats.body_state.push_back( new_stats_item );
}

void body_part_parameters::dump_state( boost::function<void(u32, float, float, pcstr)> callback, const u32 index ) const
{
	vostok::fixed_string<512> affects_str;
	for ( u32 i = 0 ; i < m_affects.size( ) ; ++i ) {
		affects_str.appendf( "%s ", affects_captions[m_affects.at( i ).first] );
	}
	callback( index, m_health, m_max_health, affects_str.c_str( ) );
}

void body_part_parameters::reset( )
{
	m_health = m_max_health;
	m_last_hit_time = 0;
	m_affects.clear();
}

bool body_part_parameters::can_affect_death( )
{
	for ( affects_threshold * it_threshold = m_thresholds.front( ) ; it_threshold ; it_threshold = m_thresholds.get_next_of_object( it_threshold ) )
	{
		hit_affects_type_enum const* threshold_affects = it_threshold->get_affects( );
		u32 threshold_affects_count = it_threshold->get_affects_count( );
		for ( u32 i = 0 ; i < threshold_affects_count ; ++i )
		{
			if ( threshold_affects[i] == affects_type_death )
				return true;
		}
	}
	return false;
}

u8 body_part_parameters::get_health_in_percentage( )
{
	// return (u8)( 100 * ( m_health / m_max_health ) ); // sushi@NOTE: Seems like the target impl is incorrect and should be like this.
	return 100 * (u8)( m_health / m_max_health );
}

void body_part_parameters::add_damage_protector( damage_protector* protector )
{
	ASSERT( UNKNOWN_EXPRESSION_T( protector ) );
	m_damage_protectors.push_back( protector );
}

void body_part_parameters::remove_damage_protector( damage_protector* protector )
{
	ASSERT( UNKNOWN_EXPRESSION_T( protector ) );
	m_damage_protectors.erase( protector );
}

void body_part_parameters::set_parameters( float max_health, float regeneration_speed )
{
	m_max_health = max_health;
	m_regeneration_speed = regeneration_speed ;
}

void serialize_affect( network_core::udp_match_packet& packet, std::pair<enum hit_affects_type_enum,u32> const& affect, s32 client_offset )
{
	packet.append( (u8)affect.first );
	packet.append( affect.second - client_offset );
}

void deserialize_affect( network_core::packet_reader& reader, std::pair<enum hit_affects_type_enum,u32>& affect )
{
	affect.first	= (hit_affects_type_enum)reader.r< bool >( );
	affect.second	= reader.r< u32 >( );

	ASSERT( UNKNOWN_EXPRESSION );
}

void body_part_parameters::serialize( network_core::udp_match_packet& packet, s32 client_offset ) const
{
	packet.append( m_health );
	packet.append( m_last_hit_time ? m_last_hit_time - client_offset : 0 );
	packet.append( (u32)m_affects.size( ) );

	std::for_each(
		m_affects.begin( ),
		m_affects.end( ),
		boost::bind( serialize_affect, boost::ref( packet ), _1, client_offset )
	);
}

void body_part_parameters::deserialize( network_core::packet_reader& reader )
{
	m_health		= reader.r< float >( );
	m_last_hit_time	= reader.r< u32 >( );
	u8 affects_count = reader.r< bool >( );

	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );

	for ( ; affects_count != 0 ; --affects_count )
	{
		std::pair< hit_affects_type_enum, u32 >	affect;
		deserialize_affect( reader, affect );

		apply_affect_by_force( affect.first, (affect_event_type_enum)0, affect.second );
	}
}

} // namespace survarium
