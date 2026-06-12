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

// STATE[99.72%|DONE]: structure-diff 1/1 stmt clean (target 0x586fd0); residual is stack-size/reg-slot noise, non-steerable
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
	m_regeneration_timeout = math::floor( 1000.0f * regeneration_timeout );

	// STRUCTURE DIFF: target 1 stmts / base 1 stmts (0x162 bytes BOTH; no diverging rows)
	// VERDICT: STRUCTURE MATCH - residual is reg-slot/reloc noise only, non-steerable.
}

// STATE[100%|DONE]
hit_type_parameters* body_part_parameters::pop_hit_type( )
{
	return m_hit_types.empty( ) ? NULL : m_hit_types.pop_front( );
}

// STATE[100%|DONE]
affects_threshold* body_part_parameters::pop_threshold( )
{
	return m_thresholds.empty( ) ? NULL : m_thresholds.pop_front( );
}

// STATE[100%|DONE]
void body_part_parameters::add_hit_type( hit_type_parameters* const new_hit_type )
{
	m_hit_types.push_back( new_hit_type );
}

// STATE[100%|DONE]
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

// STATE[99.85%|DONE]: structure-diff 2/2 stmt clean (target 0x5871b0); residual is stack-size, non-steerable
hit_type_parameters* body_part_parameters::get_hit_parameters( pcstr hit_type ) const
{
	find_hit_parameters_by_type_predicate find_predicate( hit_type );
	return m_hit_types.find_if<find_hit_parameters_by_type_predicate>( find_predicate );

	// STRUCTURE DIFF: target 2 stmts / base 2 stmts (0x3a bytes BOTH; no diverging rows)
	// VERDICT: STRUCTURE MATCH - residual is stack-slot noise only, non-steerable.
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

	// STATE[100%|DONE]
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

// STATE[99.83%|DONE]: structure-diff 29/29 stmt clean (target 0x587910); residual is stack-size/reg-slot noise, non-steerable
void body_part_parameters::hit_by_type(
	pcstr				hit_type,
	u32					time_in_ms,
	float				amount,
	float				armor_piercing,
	bool				__formal,
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

	// STRUCTURE DIFF: target 20 stmts / base 20 stmts (0x273 bytes BOTH; no diverging rows)
	// VERDICT: STRUCTURE MATCH - residual is reg-slot noise only, non-steerable.
}

// STATE[100%|DONE]
void body_part_parameters::increase_health( float amount )
{
	m_health = math::clamp_r<float>( m_health + amount, 0.0f, m_max_health );
}

// STATE[100%|DONE]
void body_part_parameters::decrease_health( float amount )
{
	m_health = math::clamp_r<float>( m_health - amount, 0.0f, m_max_health );
}

// Regenerates health and removes negative affects if they have passed.
//
// # Arguments
// * `time_delta_ms` - frame duration.
//
// STATE[72.61%|PARTIAL]: math::min(u32,u32) inline-vs-call (LTCG), non-steerable from this TU.
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

	// STRUCTURE DIFF: target 11 stmts / base 11 stmts
	// SIZE +0x3  | 200 | 			return;
	// SIZE +0x18 | 203 | 		regenerate_delta = math::min( regen_allowed, time_delta_ms );
	// VERDICT: STRUCTURE MATCH (shape ok) - target CALLs the LTCG-promoted math::min(u32,u32)
	// (ecx/edx, exists out-of-line in BOTH builds), base inlines min_integral at this site;
	// the return; +0x3 is the rel8->rel32 cascade. Callee identity verified: math::min, NOT
	// stlp_std::min<u32> (which takes const&). Whole-program inline budget, non-steerable here.
}

// STATE[100%|DONE]
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
}

// STATE[100%|DONE]
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

// Applies affects to all bodyparts. sushi@TODO: There is apply_affects also.
//
// STATE[100%|DONE]
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

	// STATE[100%|DONE]: target param is `damage_protector* const` (mangled), matching it pairs the symbol
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


// STATE[100%|DONE]
bool body_part_parameters::has_affect_protector( hit_affects_type_enum affect )
{
	protect_affect_predicate p( m_name.c_str( ), affect );
	m_damage_protectors.for_each( p );
	return p.m_result;
}

// STATE[100%|DONE]
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

// STATE[100%|DONE]
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

// STATE[91.79%|BLOCKED]: 16/16 stmt-for-stmt; blocked on the fixed_string<46>(pcstr) ctor's
// whole-program emission (target inlines it at the "none" leaf, base keeps it out-of-line).
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

	// STRUCTURE DIFF: target 16 stmts / base 16 stmts
	// SIZE +0x1  | 349 | new_stats_item.caption = m_name;
	// SIZE -0x18 | 369 | 		new_stats_item.content.push_back( vostok::fixed_string<46>( "none" ) );
	// VERDICT: STRUCTURE MATCH (shape ok) - the "none" leaf: target INLINES the fixed_string<46>
	// (pcstr) ctor (forwarding to an out-of-line buffer_string ctor, count bound by ref through a
	// u32 temp = the 0x2E store) while base keeps the whole ctor out-of-line; the +0x1 is the
	// caption operator= promoted-call (target regs) vs plain call (base). BLOCKED on the
	// fixed_string<46> ctor's whole-program emission, not steerable from this function.
}

// claude@TODO: this explicit instantiation's PLACEMENT may be wrong - the original likely
//   instantiates fill_new_stats_item where the template is defined/used, not at file scope here.
//   Warrants verification against the target.
template void body_part_parameters::fill_new_stats_item<vostok::ai::statistics_item<46,16> >(
	vostok::ai::statistics_item<46,16>& new_stats_item, const u32 current_time_in_ms ) const;

// STATE[17.04%|BLOCKED]: needs npc_statistics::body_state member (absent from our ai header)
void body_part_parameters::dump_state( vostok::ai::npc_statistics& stats, const u32 current_time_in_ms ) const
{
	typedef vostok::ai::statistics_item<46,16> content_type;
	content_type new_stats_item = content_type( );
	fill_new_stats_item( new_stats_item, current_time_in_ms );
	// stats.body_state.push_back( new_stats_item );	// claude@TODO: body_state @0x2798 missing from npc_statistics.h

	// STRUCTURE DIFF: target 3 stmts / base 2 stmts
	// TRGT_ONLY | -- | L<push_back> (0x15: stats.body_state.push_back( new_stats_item ))
	// VERDICT: STRUCTURE MISMATCH (quantity) - target's 3rd statement is
	// stats.body_state.push_back( new_stats_item ) via buffer_vector<statistics_item<46,16>>::push_back
	// at member offset 0x2798; our npc_statistics has NO body_state there (sensors_state 0 +
	// selectors_state 0x13CC land exactly at 0x2798; fill_npc_stats uses 0x7378, far past our 0x4470
	// layout). BLOCKED on adding body_state to vostok/ai/npc_statistics.h - a cross-module ai header
	// change affecting every dump_state/fill_npc_stats consumer; out of scope here.
}

// STATE[55.04%|PARTIAL]: boost::function4::operator() inline-vs-call (LTCG), non-steerable
void body_part_parameters::dump_state( boost::function<void(u32, float, float, pcstr)> callback, const u32 index ) const
{
	vostok::fixed_string<512> affects_str;
	for ( u32 i = 0 ; i < m_affects.size( ) ; ++i ) {
		affects_str.appendf( "%s ", affects_captions[m_affects.at( i ).first] );
	}
	callback( index, m_health, m_max_health, affects_str.c_str( ) );

	// STRUCTURE DIFF: target 5 stmts / base 5 stmts
	// SIZE +0x64 | 415 | callback( index, m_health, m_max_health, affects_str.c_str( ) );
	// VERDICT: STRUCTURE MATCH (shape ok) - target CALLs function4<void,u32,float,float,pcstr>
	// ::operator() out-of-line (the symbol exists ONLY in target); base inlines it (empty-check +
	// bad_function_call throw + invoker, forcing the float args through pre-evaluated temps).
	// Whole-program LTCG keep/inline decision on the boost header operator, non-steerable.
}

// STATE[100%|DONE]
void body_part_parameters::reset( )
{
	m_health = m_max_health;
	m_last_hit_time = 0;
	m_affects.clear();
}

// STATE[100%|DONE]
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

// STATE[100%|DONE]
u8 body_part_parameters::get_health_in_percentage( )
{
	// return (u8)( 100 * ( m_health / m_max_health ) ); // sushi@NOTE: Seems like the target impl is incorrect and should be like this.
	return 100 * (u8)( m_health / m_max_health );
}

// STATE[100%|DONE]
void body_part_parameters::add_damage_protector( damage_protector* protector )
{
	ASSERT( UNKNOWN_EXPRESSION_T( protector ) );
	m_damage_protectors.push_back( protector );
}

// STATE[100%|DONE]
void body_part_parameters::remove_damage_protector( damage_protector* protector )
{
	ASSERT( UNKNOWN_EXPRESSION_T( protector ) );
	m_damage_protectors.erase( protector );
}

// STATE[100%|DONE]
void body_part_parameters::set_parameters( float max_health, float regeneration_speed )
{
	m_max_health = max_health;
	m_regeneration_speed = regeneration_speed ;
}

// STATE[59.35%|PARTIAL]: append affect type (u8) then its expiry time biased by client_offset.
void serialize_affect( network_core::udp_match_packet& packet, std::pair<enum hit_affects_type_enum,u32> const& affect, s32 client_offset )
{
	packet.append( (u8)affect.first );
	packet.append( affect.second - client_offset );

	// STRUCTURE DIFF: target 2 stmts / base 2 stmts
	// SIZE +0x7 | 484 | packet.append( (u8)affect.first );
	// SIZE +0x8 | 485 | packet.append( affect.second - client_offset );
	// VERDICT: STRUCTURE MATCH (shape ok) - both SIZE are packet<T>::append promoted-call
	// (target) vs plain call (base), LTCG, non-steerable.
}

// STATE[0%|PARTIAL]: read affect type (r<bool>) and expiry time (r<u32>), then an eaten assert.
void deserialize_affect( network_core::packet_reader& reader, std::pair<enum hit_affects_type_enum,u32>& affect )
{
	affect.first	= (hit_affects_type_enum)reader.r< bool >( );
	affect.second	= reader.r< u32 >( );

	ASSERT( UNKNOWN_EXPRESSION );

	// STRUCTURE DIFF: target 3 stmts / base 3 stmts
	// SIZE +0x13 | 500 | affect.first	= (hit_affects_type_enum)reader.r< bool >( );
	// SIZE +0x15 | 501 | affect.second	= reader.r< u32 >( );
	// VERDICT: STRUCTURE MATCH (fixed quantity) - the TRGT_ONLY 0xc row was a MASTER_GOLD assert
	// eater (old "inlined r<u32> tail" verdict was wrong), ASSERT added -> 3/3. SIZE rows are
	// r<bool>/r<u32> promoted-call (target) vs plain call (base), LTCG, non-steerable.
	// NOTE: report.json shows None - the delinker attributes the base copy to another unit
	// (ICF); the rich-index pairing above is by mangled symbol and verifies fine.
}

// STATE[87.65%|PARTIAL]: append health, last-hit time (biased by client_offset, 0 when unset),
// the active-affect count, then each affect via serialize_affect. Was DCE'd; anchored from
// temp_include_all use_game_core_serialization.
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

	// STRUCTURE DIFF: target 4 stmts / base 4 stmts
	// SIZE +0x9 | 519 | packet.append( m_health );
	// SIZE +0x8 | 520 | packet.append( m_last_hit_time ? m_last_hit_time - client_offset : 0 );
	// SIZE +0x8 | 521 | packet.append( (u32)m_affects.size( ) );
	// VERDICT: STRUCTURE MATCH (shape ok) - all rows are packet<T>::append promoted-call (target)
	// vs plain call (base), LTCG, non-steerable; the for_each statement matches byte-for-byte.
}

// STATE[29.30%|PARTIAL]: read health + last-hit time, then a count of affects, applying each by force.
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

	// STRUCTURE DIFF: target 9 stmts / base 9 stmts
	// SIZE +0x1b | 536 | m_health		= reader.r< float >( );
	// SIZE +0x15 | 537 | m_last_hit_time	= reader.r< u32 >( );
	// SIZE +0x15 | 539 | u8 affects_count = reader.r< bool >( );
	// VERDICT: STRUCTURE MATCH (fixed quantity 4 -> 0; 17.60 -> 29.30) - the two TRGT_ONLY 0xc
	// rows were MASTER_GOLD assert eaters (ASSERTs added); the while+`--affects_count;`+`}` trio
	// (0x8+0x9+0x2) equals the target's single 0x13 loop head = `for ( ; cond ; --count )`
	// (jmp-over-decrement entry shape confirmed in the slice). Remaining SIZE rows are r<T>
	// promoted-call (target) vs plain call (base), LTCG, non-steerable.
}

} // namespace survarium
