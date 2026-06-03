////////////////////////////////////////////////////////////////////////////
//	Created 	: 14.08.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/damage_model.h>

#include <vostok/game_core/body_part_parameters.h>
#include <vostok/game_core/bullet.h>

#include <vostok/ai/npc_statistics.h>


namespace survarium {

// STATE[97.55%|DONE]
damage_model::damage_model( affects_applying_type_enum affects_applying_type ) :
	m_affects_applying_type					( affects_applying_type ),
	m_last_tick_time_in_ms					( 0 ),
	m_last_hit_initiator					( 255 )
{
	m_broken_legs_count[0] = 0;
	m_broken_legs_count[1] = 0;

	m_broken_hands_count[0] = 0;
	m_broken_hands_count[1] = 0;

	m_leg_damaged_subscriber.subscription_callback = boost::bind( &damage_model::on_broken_limb_affect, this, _1, _2, _3 );
	m_hand_damaged_subscriber.subscription_callback = boost::bind( &damage_model::on_broken_limb_affect, this, _1, _2, _3 );

	subscribe_on_affect( affects_type_leg_damage, &m_leg_damaged_subscriber );
	subscribe_on_affect( affects_type_hand_damage, &m_hand_damaged_subscriber );
}

// STATE[83.38%|DONE]: Difference in compiler generated code.
damage_model::~damage_model( )
{
	while ( booster_damage_protector* p = m_damage_protectors.pop_front( ) )
		VOSTOK_DELETE_IMPL( g_allocator, p );

	unsubscribe_from_affect( affects_type_leg_damage, &m_leg_damaged_subscriber );
	unsubscribe_from_affect( affects_type_hand_damage, &m_hand_damaged_subscriber );
}

// STATE[100%|DONE]
body_part_parameters* damage_model::pop_body_part( )
{
	return m_body_parts.empty( ) ? NULL : m_body_parts.pop_front( ); // <0x6ff629>|0x009|+0x033:'46'
}

struct regenerate_body_parts_predicate : public boost::noncopyable {
public:
	// STATE[BLOCKED]
	explicit			regenerate_body_parts_predicate		( u32 time_delta_ms, u32 current_time_in_ms ) :
							time_delta_ms		( time_delta_ms ),
							current_time_in_ms	( current_time_in_ms ) {}

	// STATE[BLOCKED]
	inline		void	operator()							( body_part_parameters * body_part ) const {
		body_part->regenerate( time_delta_ms, current_time_in_ms );
	}

public:
	/* 0x0000 */	u32		time_delta_ms;
	/* 0x0004 */	u32		current_time_in_ms;
}; // struct regenerate_body_parts_predicate

STATIC_SIZE_ASSERT(regenerate_body_parts_predicate, 0x8);


// STATE[100%|DONE]
void damage_model::tick( u32 time_delta_ms, u32 current_time_in_ms )
{
	if ( !m_last_tick_time_in_ms )	// <0x6ffc69>|0x009|+0x00c:'68'
	{
		m_last_tick_time_in_ms = current_time_in_ms;
		return;
	}

	regenerate_body_parts_predicate regeneration_predicate( time_delta_ms, current_time_in_ms );
	m_body_parts.for_each( regeneration_predicate );
	m_last_tick_time_in_ms = current_time_in_ms;
}

// STATE[100%|DONE]
void damage_model::add_body_part( body_part_parameters* const new_body_part )
{
	m_body_parts.push_back( new_body_part );	// <0x6ff5f9>|0x009|+0x014:'81'
}

struct find_body_part_by_name_predicate : public boost::noncopyable {
public:
	// STATE[BLOCKED]
	explicit	find_body_part_by_name_predicate	( pcstr body_part_name ) : body_part_name( body_part_name ) {}

	// STATE[BLOCKED]
	bool		operator()							( body_part_parameters* params ) const {
		return strings::equal( params->get_name( ), body_part_name );
	}

public:
	/* 0x0000 */	pcstr		body_part_name;
}; // struct find_body_part_by_name_predicate


// STATE[100%|DONE]
body_part_parameters* damage_model::get_body_part( pcstr part_name ) const
{
	find_body_part_by_name_predicate find_predicate( part_name );	// <0x6ff739>|0x009|+0x00e:'101': why twice, isn't it boost::noncopy?, it looks like our asm has a copy :shrug:
	return m_body_parts.find_if<find_body_part_by_name_predicate>( find_predicate );
}

struct find_by_damage_type_predicate {
	// STATE[BLOCKED]
	explicit		find_by_damage_type_predicate	( pcstr damage_type ) {
		strings::copy( m_damage_type, 16, damage_type );
	}

	// STATE[BLOCKED]: sushi@TODO: m_hit_type is getter
	inline	bool	operator()						( booster_damage_protector * protector ) const {
		return strings::equal( protector->m_hit_type, m_damage_type );
	}

private:
	/* 0x0000 */	char	m_damage_type[16];
}; // struct find_by_damage_type_predicate


// STATE[99.82%|DONE]
bool damage_model::hit_body_part(
	u8			initiator,
	pcstr		part_name,
	pcstr		damage_type,
	float		amount,
	float		armor_piercing,
	u32			time_in_ms,
	bullet*		const bullet
)
{
	body_part_parameters* last_hitted_body_part = bullet ? m_body_parts.find( bullet->last_hitted_body_part( ) ) : NULL;	// <0x6ffafa>|0x00a|+0x033:'145'

	body_part_parameters* part = get_body_part( part_name );
	if ( last_hitted_body_part )
		if ( last_hitted_body_part == part
			|| last_hitted_body_part->damage_group( ) != u8(-1)
			&& last_hitted_body_part->damage_group( ) == part->damage_group( ) )
		return false;

	ASSERT( UNKNOWN_EXPRESSION );
	pcstr hit_type = damage_type;
	m_last_hit_initiator = initiator;
	damage_protector* prot = m_damage_protectors.find_if( find_by_damage_type_predicate( damage_type ) );
	part->hit_by_type( hit_type, time_in_ms, amount, armor_piercing, true, prot );

	if ( bullet )
		bullet->last_hitted_body_part( part );

	return true;
}

// STATE[100%|DONE]
void damage_model::apply_med_kit( pcstr part_name, float amount )
{
	body_part_parameters* part = get_body_part( part_name );
	ASSERT( UNKNOWN_EXPRESSION );
	part->increase_health( amount );
}

struct dump_npc_body_part_state_predicate : boost::noncopyable {
public:
	// STATE[UNVERIFIED]
	explicit		dump_npc_body_part_state_predicate	( vostok::ai::npc_statistics& npc_stats, u32 current_time ) :
						npc_stats		( npc_stats ),
						current_time	( current_time ) {}

	// STATE[UNVERIFIED]
	inline	void	operator()							( body_part_parameters* params ) const {
		params->dump_state( npc_stats, current_time );
	}

public:
	/* 0x0000 */ ai::npc_statistics&		npc_stats;
	/* 0x0004 */ u32						current_time;
}; // struct dump_npc_body_part_state_predicate


// STATE[BLOCKED]
void damage_model::fill_stats( ai::npc_statistics& stats, u32 current_time_in_ms ) const
{
	typedef ai::statistics_item<46, 16> content_type;
	content_type new_stats_item;	// <0x6ff800>|0x010|+0x00b:'211'
	new_stats_item.caption = "damage status: ";
	stats.selectors_state.push_back( new_stats_item );	// sushi@TODO: This is wrong, npc_statistics has changed, this should be `body_state`.

	dump_npc_body_part_state_predicate dump_predicate( stats, current_time_in_ms );
	m_body_parts.for_each( dump_predicate );
}

// STATE[78.97%|PARTIAL]
void damage_model::dump_stats( boost::function<void( u32, float, float, pcstr )> callback )
{
	body_part_parameters*		body_part = m_body_parts.front( );	// <0x6ff92a>|0x00a|+0x00c:'231'
	u32							body_part_index = 0;
	while ( body_part )
	{
		body_part->dump_state( callback, body_part_index++ ); // sushi@NOTE: Seems like callback copy has inlined differently
		body_part = m_body_parts.get_next_of_object( body_part );
	}
}

// STATE[100%|DONE]
void damage_model::subscribe_on_affect( hit_affects_type_enum affect_type, affect_subscriber* const subscriber )
{
	m_affect_subscriptions[affect_type].push_back( subscriber );	// <0x6ffa39>|0x009|+0x01b:'242'
}

// STATE[100%|DONE]
void damage_model::unsubscribe_from_affect( hit_affects_type_enum affect_type, affect_subscriber* const subscriber )
{
	affect_subscriptions_list* subscribers = &m_affect_subscriptions[affect_type];
	ASSERT( UNKNOWN_EXPRESSION );
	subscribers->erase( subscriber );
}

struct affect_event_predicate : public boost::noncopyable {
public:
	// STATE[BLOCKED]
	explicit		affect_event_predicate	( pcstr body_part_name, hit_affects_type_enum affect_type, affect_event_type_enum event_type ) :
						body_part_name		( body_part_name ),
						affect_type			( affect_type ),
						event_type			( event_type ) { }

	// STATE[BLOCKED]
	inline	void	operator()				( affect_subscriber* subscriber ) const {
		subscriber->subscription_callback( body_part_name, affect_type, event_type );
	}


	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	pcstr						body_part_name;
	/* 0x0004 */	hit_affects_type_enum		affect_type;
	/* 0x0008 */	affect_event_type_enum		event_type;
}; // struct affect_event_predicate

STATIC_SIZE_ASSERT(affect_event_predicate, 0xC);

// STATE[100%|DONE]
void damage_model::notify_on_affect_event( pcstr body_part_name, hit_affects_type_enum affect_type, affect_event_type_enum event_type )
{
	affect_subscriptions_list* subscribers = &m_affect_subscriptions[affect_type];	// <0x6ff99f>|0x00f|+0x016|[1]:'294'
	subscribers->for_each( affect_event_predicate( body_part_name, affect_type, event_type ) );
}

struct reset_predicate : boost::noncopyable {
public:
	explicit			reset_predicate	( ) {}

	// STATE[BLOCKED]
	inline		void	operator()		( body_part_parameters* params ) const {
		params->reset( );
	}
}; // struct reset_predicate

// STATE[100%|DONE]
void damage_model::reset( )
{
	m_last_hit_initiator = u8(-1);

	m_broken_legs_count[0] = 0;
	m_broken_legs_count[1] = 0;

	m_broken_hands_count[0] = 0;
	m_broken_hands_count[1] = 0;

	m_body_parts.for_each( reset_predicate( ) );
}

// STATE[100%|DONE]
void damage_model::apply_affect( pcstr part_name, hit_affects_type_enum affect, affect_event_type_enum event_type )
{
	body_part_parameters* part = get_body_part( part_name );
	ASSERT( UNKNOWN_EXPRESSION );
	part->apply_affect_by_force( affect, event_type, m_last_tick_time_in_ms );
}

// STATE[100%|DONE]
u8 damage_model::get_total_health( )
{
	u8 result_value = 100;

	for ( body_part_parameters* it_body_part = m_body_parts.front( ) ; it_body_part ; it_body_part = m_body_parts.get_next_of_object( it_body_part ) )
	{
		if ( it_body_part->can_affect_death( ) && it_body_part->get_health_in_percentage( ) < result_value )
			result_value = it_body_part->get_health_in_percentage( );
	}
	return result_value;
}

// STATE[100%|DONE]
void damage_model::cancel_affect( pcstr part_name, hit_affects_type_enum affect )
{
	body_part_parameters* part = get_body_part( part_name );
	ASSERT( UNKNOWN_EXPRESSION );
	part->cancel_affect_by_force( affect );
}

// STATE[100%|DONE]
void damage_model::register_body_part_damage_protector( pcstr part_name, damage_protector* protector )
{
	body_part_parameters* part = get_body_part( part_name );
	ASSERT( UNKNOWN_EXPRESSION );
	part->add_damage_protector( protector );
}

// STATE[100%|DONE]
void damage_model::unregister_body_part_damage_protector( pcstr part_name, damage_protector* protector )
{
	body_part_parameters* part = get_body_part( part_name );
	ASSERT( UNKNOWN_EXPRESSION );
	part->remove_damage_protector( protector );
}

// STATE[92.21%|DONE]
void damage_model::add_damage_protector( pcstr damage_type, float reduce, float absorb )
{
	booster_damage_protector* protector = m_damage_protectors.find_if(
		find_by_damage_type_predicate( damage_type ) );
	if ( protector )
	{
		protector->m_reduce *= reduce; // damage protector is multiplicative: 0.5 * 0.5
		protector->m_absorb += absorb;
	}
	else
	{
		protector = VOSTOK_NEW_IMPL( g_allocator, booster_damage_protector )( damage_type, reduce, absorb );
		m_damage_protectors.push_back( protector );
	}
}

// STATE[98.95%|DONE]
void damage_model::on_broken_limb_affect( pcstr bodypart, hit_affects_type_enum affect, affect_event_type_enum type )
{
	if ( affect == affects_type_leg_damage && strings::equal( "left_leg", bodypart ) )
	{
		m_broken_legs_count[0] = ( type == affect_applying ) ? 1 : 0;
	}
	else if ( affect == affects_type_leg_damage && strings::equal( "right_leg", bodypart ) )
	{
		m_broken_legs_count[1] = ( type == affect_applying ) ? 1 : 0;
	}
	else if ( affect == affects_type_hand_damage && strings::equal( "left_hand", bodypart ) )
	{
		m_broken_hands_count[0] = ( type == affect_applying ) ? 1 : 0;
	}
	else if ( affect == affects_type_hand_damage && strings::equal( "right_hand", bodypart ) )
	{
		m_broken_hands_count[1] = ( type == affect_applying ) ? 1 : 0;
	}
}

// STATE[BLOCKED]
void damage_model::deserialize( network_core::packet_reader& reader )
{
	// FUNCTION BODY[0x6ffc10]: 1
	// <0x6ffc10>|0x000|+0x009:'433'	{
	// <0x6ffc19>|0x009|+0x041:'434'
	// <0x6ffc5a>|0x04a|      :'435'	}
	// ******
}

} // namespace survarium
