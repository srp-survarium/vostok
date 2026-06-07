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

// STATE[93.97%|DONE]
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

	// FUNCTION BODY[0x596fd0]: 1
	// <0x597105>|0x135|+0x024:'29'
	// ******
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

// STATE[99%|DONE]: Stack size
hit_type_parameters* body_part_parameters::get_hit_parameters( pcstr hit_type ) const
{
	find_hit_parameters_by_type_predicate find_predicate( hit_type );
	return m_hit_types.find_if<find_hit_parameters_by_type_predicate>( find_predicate );

	// FUNCTION BODY[0x5971b0]: 2
	// <0x5971b9>|0x009|+0x00e:'69'
	// <0x5971c7>|0x017|+0x01d:'70'
	// ******
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

// STATE[99.83%|DONE]
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

	// FUNCTION BODY[0x597910]: 36
	// <0x59791f>|0x00f|+0x012:'112'
	// <0x597931>|0x021|+0x00c:'113'
	// <0>
	// <0x59793d>|0x02d|+0x00a:'115'
	// <0>
	// <1>
	// <2>
	// <0x597947>|0x037|+0x01f:'119'	if ( params->m_armor == 0.0f )
	// <0x597966>|0x056|+0x00d:'120'
	// <0x597973>|0x063|+0x002:'121'	else
	// <0x597975>|0x065|+0x031:'122'
	// <0>
	// <0x5979a6>|0x096|+0x017:'124'	float e_wnd = math::max( 0.0f, arp_arm_coeff );
	// <0>
	// <0x5979bd>|0x0ad|+0x072:'126'
	// <0>
	// <0x597a2f>|0x11f|+0x039:'128'
	// <0x597a68>|0x158|+0x02b:'129'
	// <0x597a93>|0x183|+0x017:'130'
	// <0>
	// <0x597aaa>|0x19a|+0x006:'132'
	// <0x597ab0>|0x1a0|+0x04c:'133'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x597afc>|0x1ec|+0x012:'140'
	// <0x597b0e>|0x1fe|+0x018:'141'
	// <0x597b26>|0x216|+0x00f:'142'
	// <0>
	// <0x597b35>|0x225|+0x01e:'144'
	// <0x597b53>|0x243|+0x00f:'145'
	// <0>
	// <0x597b62>|0x252|+0x013:'147'
	// ******
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

// STATE[91.78%|BLOCKED]: body + control structure are an exact statement-for-statement match
// (deltas agree vs target --view structure 0x0ba3c0). Sole residual: target inlines the
// fixed_string<46>(char const*) ctor at the "none" leaf while base keeps it out-of-line (the
// const-char* ctor exists out-of-line in base @0x030ae0 but NOT in target). That inline is a
// whole-program COMDAT decision on the fixed_string<46> type - not steerable from this function's
// source - so this is BLOCKED on fixed_string<46>'s emission, not a banked LTCG/PARTIAL residual.
// The +0x10 frame delta and all reg/slot renaming cascade from that one inline. See the .md.
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

	// STRUCTURE DIFF[target 0x0ba3c0 | base 0x085aa0]: target 22 / base 23 stmts
	// .. same ..
	// 0x010 <0x11> | 0x010 <0x12> | new_stats_item.caption = m_name;                                  SIZE
	// .. same ..
	// 0x1f0 <0x31> | 0x1f1 <0x19> | new_stats_item.content.push_back( fixed_string<46>( "none" ) );   SIZE
	// --          | <0>          |                                                                     EMPTY only base
	// ; aligned 20, size-diffs 2, quantity-diffs 1
	//
	// STRUCTURE MATCH (statement-for-statement; 22 vs 23 is an EMPTY blank-line gap, not a control diff).
	// Both SIZE diffs cascade from one whole-program COMDAT inline at the "none" leaf (L306): target
	// inlines fixed_string<46>(char const*) (mov 2Eh; call buffer_string::buffer_string), base calls
	// fixed_string<46>::fixed_string<46> out-of-line. That widens the frame 0x10 (0xE8 vs 0xD8) and
	// cascades the reg/slot renaming (incl. the L286 caption= operator= 0x11 vs 0x12). Not steerable
	// from this function's source -> BLOCKED on fixed_string<46>'s emission, not PARTIAL/LTCG.
}

// claude@TODO: this explicit instantiation's PLACEMENT may be wrong - the original likely
//   instantiates fill_new_stats_item where the template is defined/used, not at file scope here.
//   Warrants verification against the target.
template void body_part_parameters::fill_new_stats_item<vostok::ai::statistics_item<46,16> >(
	vostok::ai::statistics_item<46,16>& new_stats_item, const u32 current_time_in_ms ) const;

// STATE[INPROGRESS]: unblocked - fill_new_stats_item is now matched and wired in (this calls
// it). Remaining: the trailing stats.<member>.push_back (commented below) needs npc_statistics's
// body-state member.
void body_part_parameters::dump_state( vostok::ai::npc_statistics& stats, u32 current_time_in_ms ) const
{
	typedef vostok::ai::statistics_item<46,16> content_type;
	content_type new_stats_item = content_type( );					// <0x59714f>
	fill_new_stats_item( new_stats_item, current_time_in_ms );		// <0x597165>
	// stats->body_state.push_back(new_stats_item);					// <0x59717b>
}

// STATE[51.42%|PARTIAL]
void body_part_parameters::dump_state( boost::function<void(u32, float, float, pcstr)> callback, u32 index ) const
{
	vostok::fixed_string<512> affects_str;
	for ( u32 i = 0 ; i < m_affects.size( ) ; ++i ) {
		affects_str.appendf( "%s ", affects_captions[m_affects.at( i ).first] );
	}
	callback( index, m_health, m_max_health, affects_str.c_str( ) ); // sushi@MATCH: Callback execution is inlined, which is not true in target

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
