////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.08.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/body_part_parameters.h>

#include <vostok/game_core/affects_applying_type_enum.h>
#include <vostok/game_core/damage_model.h>
#include <vostok/game_core/damage_protector.h>

#include <vostok/ai/npc_statistics.h>

namespace survarium {

// STATE[PARTIAL]: inlining was done completely differently
body_part_parameters::body_part_parameters(
	pcstr				name,
	float				health,
	float				regeneration_speed,
	float				regeneration_timeout,
	bool				can_be_assigned,
	damage_model&		owner,
	u8					damage_group
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
	m_regeneration_timeout = math::floor( 1000.0f * regeneration_timeout ); // <0x597105>
}

// STATE[99%|DONE]
hit_type_parameters* body_part_parameters::pop_hit_type( )
{
	return m_hit_types.empty( ) ? NULL : m_hit_types.pop_front( ); // <0x596f99>
}

// STATE[99%|DONE]
affects_threshold* body_part_parameters::pop_threshold( )
{
	return m_thresholds.empty( ) ? NULL : m_thresholds.pop_front( ); // <0x596f59>
}

// STATE[99%|DONE]
void body_part_parameters::add_hit_type( hit_type_parameters* const new_hit_type )
{
	m_hit_types.push_back( new_hit_type ); // <0x596f39>
}

// STATE[99%|DONE]
void body_part_parameters::add_threshold( affects_threshold* const new_threshold )
{
	m_thresholds.push_back( new_threshold ); // <0x596f19>
}

struct find_hit_parameters_by_type_predicate : public boost::noncopyable {
public:
	explicit		find_hit_parameters_by_type_predicate	( pcstr hit_t ) : hit_type( hit_t ) {  }

	// STATE[INLINED]: sushi@TODO: This method seems to be inlined into `find_if`, so we need to match that instead
	inline	bool	operator()								( hit_type_parameters* hit_type_params ) const {
		return strings::equal( hit_type, hit_type_params->get_type( ) );
	}

private:
	/* 0x0000 */	pcstr	hit_type;
}; // struct find_hit_parameters_by_type_predicate

// STATE[99%|DONE]
hit_type_parameters* body_part_parameters::get_hit_parameters( pcstr hit_type ) const
{
	find_hit_parameters_by_type_predicate find_predicate( hit_type );						// <0x5971b9>
	return m_hit_types.find_if<find_hit_parameters_by_type_predicate>( find_predicate );	// <0x5971c7>
}

struct protect_damage_predicate : boost::noncopyable {
public:
	// STATE[BLOCKED]
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

	// STATE[99%|DONE]
	inline		void	operator()					( damage_protector* const protector ) {
					if ( m_amount > 0.0f && protector->reduce_damage_functor )	// <0xc9eff>
						m_amount = protector->reduce_damage_functor( m_body_type_name, m_damage_type, m_amount, m_armor_piercing );
				}

public:
	/* 0x0000 */	pcstr		m_body_type_name;
	/* 0x0004 */	pcstr		m_damage_type;
	/* 0x0008 */	float		m_armor_piercing;
	/* 0x000c */	float		m_amount;
}; // struct protect_damage_predicate

STATIC_SIZE_ASSERT(protect_damage_predicate, 0x10);

static float g_arp_arm_coeff = 1.0;

// STATE[PENDING]
void body_part_parameters::hit_by_type(
	pcstr					hit_type,
	u32						time_in_ms,
	float					amount,
	float					armor_piercing,
	bool					__formal,
	damage_protector*		prot
)
{
	hit_type_parameters * params = get_hit_parameters( hit_type );
	ASSERT(params);

	float delta = amount;

	float arp_arm_coeff;
	if ( params->m_armor == 0.0f )
	{
		arp_arm_coeff = g_arp_arm_coeff;
	}
	else
	{
		arp_arm_coeff = math::min( ( armor_piercing / params->m_armor ) - g_arp_arm_coeff, g_arp_arm_coeff );
	}

	float e_wnd = math::max( 0.0f, arp_arm_coeff );

	delta = delta * e_wnd
		+ math::max(
			0.0f,
			( g_arp_arm_coeff - params->m_reduce ) * delta * ( g_arp_arm_coeff - e_wnd ) - params->m_absorption_amount );

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

// STATE[99%|DONE]
void body_part_parameters::increase_health( float amount )
{
	m_health = math::clamp_r<float>( m_health + amount, 0.0f, m_max_health ); // <0x596ec9>
}

// STATE[99%|DONE]
void body_part_parameters::decrease_health( float amount )
{
	m_health = math::clamp_r<float>( m_health - amount, 0.0f, m_max_health ); // <0x596e79>
}

// Regenerates health and removes negative affects if they have passed.
//
// # Arguments
// * `time_delta_ms` - frame duration.
//
// STATE[100%|DONE]
void body_part_parameters::regenerate( u32 time_delta_ms, u32 current_time_in_ms )
{
	u32 regenerate_delta = time_delta_ms;

	if ( m_regeneration_timeout )
	{
		u32 next_regen_time = m_last_hit_time + m_regeneration_timeout;

		if ( current_time_in_ms <= next_regen_time )
			return;

		u32 regen_allowed = current_time_in_ms - next_regen_time;
		regenerate_delta = math::min( regen_allowed, time_delta_ms );
		// sushi@NOTE: There is an ifdef of some kind
	}

	float amount = regenerate_delta * m_regeneration_speed / 1000.0f;
	increase_health( amount );

	if ( m_damage_model.get_affects_applying_type( ) == type_apply_directly ) // sushi@TODO: Needs getter
		update_affects( current_time_in_ms );
}

// STATE[99.84%|DONE]
void body_part_parameters::update_affects( u32 current_time_in_ms )
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

	// LOCALS
	// s32 							i<1>
	// std::pair<enum hit_affects_type_enum,u32>* it_affect<2>
	// ******

	// FUNCTION BODY
	// <0x5977a9>|0x009|+0x02e|[1]:'192'	for ( s32 i = m_affects.size( ) - 1 ; i >= 0; --i )
	// <0>									{
	// <0x5977d7>|0x037|+0x00f|[2]:'194'		std::pair<hit_affects_type_enum, u32> * it_affect = m_affects.begin( );
	// <0x5977e6>|0x046|+0x01e:'195'			ASSERT
	// <0>
	// <0x597804>|0x064|+0x00c:'197'
	// <0x597810>|0x070|+0x01f:'198'			m_damage_model.notify_on_affect_event( m_name.c_str( ), it_affect->first, affect_recalling );
	// <0x59782f>|0x08f|+0x01c:'199'			m_affects.erase( it_affect )
	// <0x59784b>|0x0ab|+0x005:'200'
	// <0>
	// ******
}

// STATE[99.84%|DONE]
void body_part_parameters::cancel_affect_by_force( hit_affects_type_enum affect )
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

// STATE[99%|DONE]: sushi@NOTE: check_affects doesn't checks them but applies them .
void body_part_parameters::check_affects( u32 current_time_in_ms )
{
	for ( affects_threshold* it_threshold = m_thresholds.front( ) ; it_threshold ; it_threshold = m_thresholds.get_next_of_object( it_threshold ) )
		if ( m_health <= m_max_health * it_threshold->value( ) )
		{
			it_threshold->bodypart( )->apply_affects( it_threshold, current_time_in_ms );
		}
}

// STATE[100%|DONE]
bool body_part_parameters::is_affect_applied( hit_affects_type_enum	affect )
{
	for ( u32 i = 0; i < m_affects.size( ); ++i )
	{
		if ( m_affects.at( i ).first == affect ) return true;
	}

	return false;
}

struct protect_affect_predicate : boost::noncopyable {
public:
	explicit				protect_affect_predicate	( pcstr body_type_name, hit_affects_type_enum affect_type ) :
								m_body_type_name	( body_type_name ),
								m_affect_type		( affect_type ),
								m_result			( false ) { }

	// STATE[UNVERIFIED]
	inline		void		operator()					( damage_protector* protector ) {
		if ( !m_result && protector->protect_affect_functor )
			m_result = protector->protect_affect_functor( m_body_type_name, m_affect_type );
	}

public:
	/* 0x0000 */	pcstr						m_body_type_name;
	/* 0x0004 */	hit_affects_type_enum		m_affect_type;
	/* 0x0008 */	bool						m_result;
}; // struct protect_affect_predicate

STATIC_SIZE_ASSERT(protect_affect_predicate, 0xC);


// STATE[99%|DONE]
bool body_part_parameters::has_affect_protector( hit_affects_type_enum affect )
{
	protect_affect_predicate p( m_name.c_str( ), affect ); // <0x5973e9>
	m_damage_protectors.for_each( p );
	return p.m_result;
}

// STATE[99%|DONE]
void body_part_parameters::apply_affects( affects_threshold const* threshold_reached, u32 current_time_in_ms )
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

// STATE[PARTIAL]: sushi@TODO MATCHING IS INCORERCT
void body_part_parameters::apply_affect_by_force( hit_affects_type_enum affect, affect_event_type_enum event_type, u32 current_time_in_ms )
{
	if ( event_type == affect_applying && !is_affect_applied( affect ) )
	{
		m_damage_model.notify_on_affect_event( m_name.c_str( ), affect, affect_applying );
		m_affects.push_back( std::make_pair( affect, current_time_in_ms + 1000 * affects_durations[affect] ) );	// <0x597491>: @TODO: Most likely inlined helper. Used in multiple places

	}
	else if ( event_type == affect_recalling )
	{
		for ( u32 i = 0 ; i < m_affects.size() ; ++i )
		{
			std::pair<hit_affects_type_enum, u32> & it_affect = m_affects.at( i );
			if ( it_affect.first == affect )
			{
				m_damage_model.notify_on_affect_event( m_name.c_str( ), affect, affect_recalling );
				m_affects.erase( &it_affect );
				return;
			}
		}
	}

	// LOCALS
	// std::pair<enum hit_affects_type_enum,u32>* it_affect<1>
	// u32 							i<2>
	// ******

	// FUNCTION BODY
	// <0x597459>|0x009|+0x019:'260'
	// <0>
	// <0x597472>|0x022|+0x01f:'262'
	// <0x597491>|0x041|+0x03c:'263'
	// <0>
	// <1>
	// <0x5974cd>|0x07d|+0x00a:'266'
	// <0>
	// <0x5974d7>|0x087|+0x00f|[1]:'268'
	// <0x5974e6>|0x096|+0x02e|[2]:'269'
	// <0>
	// <0x597514>|0x0c4|+0x01d:'271'
	// <0>
	// <0x597531>|0x0e1|+0x00c:'273'
	// <0x59753d>|0x0ed|+0x01d:'274'
	// <0x59755a>|0x10a|+0x01c:'275'
	// <0>
	// <0x597576>|0x126|+0x002:'277'
	// <0>
	// <0x597578>|0x128|+0x005:'279'
	// <0>
	// ******

}

// sushi@TODO: This one is very strange, since there is no such function in the header?
// It does exists in IDA. Maybe it was generated by something? The code seems completely different from what is generally used.
#if 0
// STATE[STUB]
// void survarium::body_part_parameters::fill_new_stats_item<vostok::ai::statistics_item<46,16> >(vostok::ai::statistics_item<46,16>&, const unsigned int) const
void body_part_parameters::fill_new_stats_item<vostok::ai::statistics_item<46,16> >(
	vostok::ai::statistics_item<46,16>&  new_stats_item,
	u32                                current_time_in_ms)
{
	// LOCALS
	// vostok::fixed_string<46>          new_item
	// u32                             i<1>
	// u32                             remaining_time_in_ms<2>
	// ******

	// SKIPPED BLOCKS
	// <0xca49d><2>
	// ******

	// FUNCTION BODY
	// <0xca3d0>
	// <0xca3e1>

	// <0xca3ee>
	// <0xca3f6>
	// <0xca432>

	// <0xca441>
	// <0xca449>
	// <0xca456>

	// <0xca465> <block><1>

	// <0xca4a3>
	// <0xca4ab>
	// <0xca512>
	// <0xca570>
	// <0xca57f>

	// <0xca584>
	// <0xca5b0>
	// ******
}
#endif

// STATE[BLOCKED] sushi@TODO: Requires fill_new_stats_item
void body_part_parameters::dump_state( vostok::ai::npc_statistics& stats, u32 current_time_in_ms ) const
{
	typedef vostok::ai::statistics_item<46,16> content_type;
	content_type new_stats_item = content_type( );					// <0x59714f>
	// fill_new_stats_item(&new_stats_item, current_time_in_ms);	// <0x597165>
	// stats->body_state.push_back(new_stats_item);					// <0x59717b>
}

// STATE[51.16%|PARTIAL]
void body_part_parameters::dump_state( boost::function<void(u32, float, float, pcstr)> callback, u32 index ) const
{
	vostok::fixed_string<512> affects_str;
	for ( u32 i = 0 ; i < m_affects.size( ) ; ++i ) {
		affects_str.appendf( "%s ", affects_captions[m_affects.at( i ).first] );
	}
	callback( index, m_health, m_max_health, affects_str.c_str( ) ); // sushi@TODO: Callback execution is inlined, which is not true in target

	// LOCALS
	// fixed_string<512> 			affects_str
	// u32 							i<1>
	// ******

	// FUNCTION BODY
	// <0x597300>|0x010|+0x00b:'326'
	// <0x59730b>|0x01b|+0x046|[1]:'327'
	// <0x597351>|0x061|+0x03f:'328'
	// <0x597390>|0x0a0|+0x002:'329'
	// <0x597392>|0x0a2|+0x038:'330'
	// ******
}

// STATE[100%|DONE]
void body_part_parameters::reset( )
{
	m_health = m_max_health;	// <0x596d49>
	m_last_hit_time = 0;		// <0x596d5b>
	m_affects.clear();			// <0x596d68> sushi@TODO:  for ( i = this->m_affects.m_begin; i != this->m_affects.m_end; ++i ) { ; } this->m_affects.m_end = this->m_affects.m_begin;
}

// STATE[100%|DONE]
bool body_part_parameters::can_affect_death( )
{
	for ( affects_threshold * it_threshold = m_thresholds.front( ) ; it_threshold ; it_threshold = m_thresholds.get_next_of_object( it_threshold ) ) // <0x596cc9> <block><1>
	{
		hit_affects_type_enum const* threshold_affects = it_threshold->get_affects( );	// <0x596ce2> <block><2>
		u32 threshold_affects_count = it_threshold->get_affects_count( );				// <0x596cf7>
		for ( u32 i = 0 ; i < threshold_affects_count ; ++i )							// <0x596d00> <block><3>
		{
			if ( threshold_affects[i] == affects_type_death )							// <0x596d1a>
				return true;															// <0x596d26>
		}																				// <0x596d2a>
	}																					// <0x596d2c>
	return false;																		// <0x596d2e>
}

// STATE[100%|DONE]
u8 body_part_parameters::get_health_in_percentage( )
{
	// return (u8)( 100 * ( m_health / m_max_health ) ); // sushi@NOTE: Seems like the target impl is incorrect and should be like this.
	return 100 * (u8)( m_health / m_max_health );
}

// STATE[99%|DONE]
void body_part_parameters::add_damage_protector( damage_protector* protector )
{
	ASSERT( protector );						// <0x596e49>
	m_damage_protectors.push_back( protector );	// <0x596e55>
}

// STATE[99%|DONE]
void body_part_parameters::remove_damage_protector( damage_protector* protector )
{
	ASSERT( protector );					// <0x596e19>
	m_damage_protectors.erase( protector );	// <0x596e25>
}

// STATE[100%|DONE]
void body_part_parameters::set_parameters( float max_health, float regeneration_speed )
{
	m_max_health = max_health;					// <0x596c67>
	m_regeneration_speed = regeneration_speed ; // <0x596c77>
}

// STATE[BLOCKED]
void serialize_affect( network_core::udp_match_packet& packet, std::pair<enum hit_affects_type_enum,u32> const& affect, s32 client_offset )
{
}

// STATE[BLOCKED]
void deserialize_affect( network_core::packet_reader& reader, std::pair<enum hit_affects_type_enum,u32>& affect )
{
}

// STATE[BLOCKED]
void body_part_parameters::serialize( network_core::udp_match_packet& packet, s32 client_offset ) const
{
}

// STATE[BLOCKED]
void body_part_parameters::deserialize( network_core::packet_reader& reader )
{
}

} // namespace survarium
