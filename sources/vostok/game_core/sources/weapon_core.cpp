////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core.h>

#include <vostok/animation/sources/mixing_base_lexeme.h>

#include <vostok/game_core/base_player.h>
#include <vostok/game_core/player_input.h>

#include <vostok/ai/fsm.h>
#include <boost/bind.hpp>

namespace survarium {
/*
// STATE[STUB]
// void survarium::`dynamic initializer for 's_ik_use_cc''()
void `dynamic initializer for 's_ik_use_cc''( )
{
	// FUNCTION BODY
	// <0x7d95b0>|0x000|      :'38'	{
	// ******
}

// STATE[STUB]
// void survarium::`dynamic initializer for 's_ik_use_on_legs_cc''()
void `dynamic initializer for 's_ik_use_on_legs_cc''( )
{
	// FUNCTION BODY
	// <0x7d95f0>|0x000|      :'41'	{
	// ******
}

// STATE[STUB]
// void survarium::`dynamic initializer for 's_recoil_vertical_eanble_cc''()
void `dynamic initializer for 's_recoil_vertical_eanble_cc''( )
{
	// FUNCTION BODY
	// <0x7d9630>|0x000|      :'44'	{
	// ******
}

// STATE[STUB]
// void survarium::`dynamic initializer for 's_recoil_horizontal_eanble_cc''()
void `dynamic initializer for 's_recoil_horizontal_eanble_cc''( )
{
	// FUNCTION BODY
	// <0x7d9670>|0x000|      :'47'	{
	// ******
}

// STATE[STUB]
// void survarium::`dynamic initializer for 's_recoil_back_eanble_cc''()
void `dynamic initializer for 's_recoil_back_eanble_cc''( )
{
	// FUNCTION BODY
	// <0x7d96b0>|0x000|      :'50'	{
	// ******
}

// STATE[STUB]
// void survarium::`dynamic initializer for 's_recoil_enable_cc''()
void `dynamic initializer for 's_recoil_enable_cc''( )
{
	// FUNCTION BODY
	// <0x7d96f0>|0x000|      :'53'	{
	// ******
}
*/
// STATE[STUB]
// survarium::weapon_core::weapon_core()
weapon_core::weapon_core( ) : inventory_item( inventory_item::inventory_active_item )
{
	// FUNCTION BODY
	// <0x5aa750>|0x000|+0x2aa:'95'	{
	// <0x5aa9fa>|0x2aa|      :'96'	}
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::~weapon_core()
weapon_core::~weapon_core( )
{
	// LOCALS
	// ai::fsm_state* 				state<1>
	// ******

	// FUNCTION BODY
	// <0x5a4ce2>|0x012|+0x013:'100'
	// <0x5a4cf5>|0x025|+0x00e:'101'
	// <0x5a4d03>|0x033|+0x019|[1]:'102'
	// <0>
	// <1>
	// <0x5a4d1c>|0x04c|+0x02c:'105'
	// ******
}

// STATE[STUB]
// claude@NOTE: out-of-line so the idle-state getters emit `call ammo_in_magazine`
// instead of inlining `m_ammo_in_magazine` (matches the target's out-of-line call).
u16 weapon_core::ammo_in_magazine( ) const
{
	return m_ammo_in_magazine;
}

// STATE[STUB]
// claude@NOTE: out-of-line so the double-barreled ctor's ASSERT_CMP_U emits
// `call get_magazine_capacity` instead of inlining `m_magazine_capacity`
// (matches the target's out-of-line call; symbol @0x09cc20).
u16 weapon_core::get_magazine_capacity( ) const
{
	return m_magazine_capacity;
}

// STATE[100%|DONE]
void weapon_core::set_magazine_capacity( u16 magazine_capacity )
{
	ASSERT( UNKNOWN_EXPRESSION_T( magazine_capacity ) );
	m_magazine_capacity = magazine_capacity;
}

// STATE[100%|DONE]
// claude@NOTE: out-of-line so reset_fire_queue emits `call fire_queue_length`
// instead of inlining the m_weapon_fire_queue_types[m_fire_queue_type] read
// (matches the target's standalone symbol @0x09b290).
u16 weapon_core::fire_queue_length( ) const
{
	return m_weapon_fire_queue_types[m_fire_queue_type];
}

// STATE[95.69%|INPROGRESS]: full FSM body written (10 add_state + 72 add_transition via boost::bind); residual is the boost::function Form-A vs Form-B construction shape across transitions; see weapon_core_initialize_weapon_logic.md
void weapon_core::initialize_weapon_logic(
	weapon_core_base_state_ptr const&	inactive_state,
	weapon_core_base_state_ptr const&	show_state,
	weapon_core_base_state_ptr const&	hide_state,
	weapon_core_base_state_ptr const&	idle_state,
	weapon_core_base_state_ptr const&	reload_state,
	weapon_core_base_state_ptr const&	fire_state,
	weapon_core_base_state_ptr const&	aim_state,
	weapon_core_base_state_ptr const&	aim_fire_state,
	weapon_core_base_state_ptr const&	chamber_a_round_state,
	weapon_core_base_state_ptr const&	chamber_a_round_aimed_state
)
{
	m_is_there_chamber_a_round_state = chamber_a_round_state.c_ptr( ) != NULL;
	ASSERT( UNKNOWN_EXPRESSION_T( m_logic ) );

	m_logic_states.push_back( inactive_state );
	m_logic_states.push_back( show_state );
	m_logic_states.push_back( hide_state );
	m_logic_states.push_back( idle_state );
	m_logic_states.push_back( reload_state );
	m_logic_states.push_back( fire_state );
	m_logic_states.push_back( aim_state );
	m_logic_states.push_back( aim_fire_state );
	if ( m_is_there_chamber_a_round_state )
		m_logic_states.push_back( chamber_a_round_state );
	if ( chamber_a_round_aimed_state )
		m_logic_states.push_back( chamber_a_round_aimed_state );

	weapon_core_base_state* inactive				= inactive_state.c_ptr( );
	weapon_core_base_state* show					= show_state.c_ptr( );
	weapon_core_base_state* hide					= hide_state.c_ptr( );
	weapon_core_base_state* idle					= idle_state.c_ptr( );
	weapon_core_base_state* reload				= reload_state.c_ptr( );
	weapon_core_base_state* fire					= fire_state.c_ptr( );
	weapon_core_base_state* aim					= aim_state.c_ptr( );
	weapon_core_base_state* aim_fire				= aim_fire_state.c_ptr( );
	weapon_core_base_state* chamber_a_round		= chamber_a_round_state.c_ptr( );
	weapon_core_base_state* chamber_a_round_aimed	= chamber_a_round_aimed_state.c_ptr( );

	fire->set_is_firing_ptr( &m_is_firing );
	aim_fire->set_is_firing_ptr( &m_is_firing );

	m_logic->add_state( inactive );
	m_logic->add_state( show );
	m_logic->add_state( hide );
	m_logic->add_state( idle );
	m_logic->add_state( reload );
	m_logic->add_state( fire );
	m_logic->add_state( aim );
	m_logic->add_state( aim_fire );
	if ( m_is_there_chamber_a_round_state )
		m_logic->add_state( chamber_a_round );
	if ( chamber_a_round_aimed )
		m_logic->add_state( chamber_a_round_aimed );

	m_logic->add_transition( inactive, show, boost::bind( &weapon_core::target_predicate, this, weapon_target_idle ) );
	m_logic->add_transition( inactive, show, boost::bind( &weapon_core::target_predicate, this, weapon_target_reload ) );
	m_logic->add_transition( inactive, show, boost::bind( &weapon_core::target_predicate, this, weapon_target_fire ) );
	m_logic->add_transition( inactive, show, boost::bind( &weapon_core::target_predicate, this, weapon_target_aim ) );
	m_logic->add_transition( inactive, show, boost::bind( &weapon_core::target_predicate, this, weapon_target_aim_fire ) );
	m_logic->add_transition( show, hide, boost::bind( &weapon_core::target_predicate, this, weapon_target_inactive ) );
	if ( chamber_a_round_aimed )
		m_logic->add_transition( show, chamber_a_round_aimed, boost::bind( &weapon_core::must_chamber_a_round_aimed_predicate, this ) );
	if ( m_is_there_chamber_a_round_state )
		m_logic->add_transition( show, chamber_a_round, boost::bind( &weapon_core::must_chamber_a_round_predicate, this ) );
	m_logic->add_transition( show, reload, boost::bind( &weapon_core::can_and_must_reload_predicate, this ) );
	m_logic->add_transition( show, idle, boost::bind( &weapon_core::target_predicate, this, weapon_target_idle ) );
	m_logic->add_transition( show, reload, boost::bind( &weapon_core::target_predicate, this, weapon_target_reload ) );
	m_logic->add_transition( show, fire, boost::bind( &weapon_core::target_predicate, this, weapon_target_fire ) );
	m_logic->add_transition( show, aim, boost::bind( &weapon_core::target_predicate, this, weapon_target_aim ) );
	m_logic->add_transition( show, aim_fire, boost::bind( &weapon_core::target_predicate, this, weapon_target_aim_fire ) );
	m_logic->add_transition( hide, inactive, boost::bind( &weapon_core::target_predicate, this, weapon_target_inactive ) );
	m_logic->add_transition( hide, show, boost::bind( &weapon_core::target_predicate, this, weapon_target_idle ) );
	m_logic->add_transition( hide, show, boost::bind( &weapon_core::target_predicate, this, weapon_target_reload ) );
	m_logic->add_transition( hide, show, boost::bind( &weapon_core::target_predicate, this, weapon_target_fire ) );
	m_logic->add_transition( hide, show, boost::bind( &weapon_core::target_predicate, this, weapon_target_aim ) );
	m_logic->add_transition( hide, show, boost::bind( &weapon_core::target_predicate, this, weapon_target_aim_fire ) );
	m_logic->add_transition( idle, hide, boost::bind( &weapon_core::target_predicate, this, weapon_target_inactive ) );
	if ( chamber_a_round_aimed )
		m_logic->add_transition( idle, chamber_a_round_aimed, boost::bind( &weapon_core::must_chamber_a_round_aimed_predicate, this ) );
	if ( m_is_there_chamber_a_round_state )
		m_logic->add_transition( idle, chamber_a_round, boost::bind( &weapon_core::must_chamber_a_round_predicate, this ) );
	m_logic->add_transition( idle, reload, boost::bind( &weapon_core::target_predicate, this, weapon_target_reload ) );
	m_logic->add_transition( idle, fire, boost::bind( &weapon_core::target_predicate, this, weapon_target_fire ) );
	m_logic->add_transition( idle, aim, boost::bind( &weapon_core::target_predicate, this, weapon_target_aim ) );
	m_logic->add_transition( idle, aim_fire, boost::bind( &weapon_core::target_predicate, this, weapon_target_aim_fire ) );
	m_logic->add_transition( reload, hide, boost::bind( &weapon_core::target_predicate, this, weapon_target_inactive ) );
	if ( m_is_there_chamber_a_round_state )
		if ( !m_chamber_a_round_on_reload )
			m_logic->add_transition( reload, chamber_a_round, boost::bind( &weapon_core::must_chamber_a_round_and_animation_ended_predicate, this ) );
	m_logic->add_transition( reload, idle, boost::bind( &weapon_core::target_and_animation_ended_predicate, this, weapon_target_idle ) );
	m_logic->add_transition( reload, idle, boost::bind( &weapon_core::instant_idle_predicate, this ) );
	m_logic->add_transition( reload, fire, boost::bind( &weapon_core::target_and_animation_ended_predicate, this, weapon_target_fire ) );
	m_logic->add_transition( reload, aim, boost::bind( &weapon_core::target_and_animation_ended_predicate, this, weapon_target_aim ) );
	m_logic->add_transition( reload, aim_fire, boost::bind( &weapon_core::target_and_animation_ended_predicate, this, weapon_target_aim_fire ) );
	m_logic->add_transition( fire, hide, boost::bind( &weapon_core::target_and_animation_ended_predicate, this, weapon_target_inactive ) );
	if ( chamber_a_round_aimed )
		m_logic->add_transition( fire, chamber_a_round_aimed, boost::bind( &weapon_core::must_chamber_a_round_aimed_and_animation_ended_predicate, this ) );
	if ( m_is_there_chamber_a_round_state )
		m_logic->add_transition( fire, chamber_a_round, boost::bind( &weapon_core::must_chamber_a_round_and_animation_ended_predicate, this ) );
	m_logic->add_transition( fire, reload, boost::bind( &weapon_core::can_and_must_reload_and_animation_ended_predicate, this ) );
	m_logic->add_transition( fire, idle, boost::bind( &weapon_core::target_and_animation_ended_predicate, this, weapon_target_idle ) );
	m_logic->add_transition( fire, reload, boost::bind( &weapon_core::target_and_animation_ended_predicate, this, weapon_target_reload ) );
	m_logic->add_transition( fire, aim, boost::bind( &weapon_core::target_and_animation_ended_predicate, this, weapon_target_aim ) );
	m_logic->add_transition( fire, aim_fire, boost::bind( &weapon_core::target_and_animation_ended_predicate, this, weapon_target_aim_fire ) );
	m_logic->add_transition( fire, aim_fire, boost::bind( &weapon_core::is_trying_to_aim, this ) );
	m_logic->add_transition( aim, hide, boost::bind( &weapon_core::target_predicate, this, weapon_target_inactive ) );
	m_logic->add_transition( aim, idle, boost::bind( &weapon_core::target_predicate, this, weapon_target_idle ) );
	m_logic->add_transition( aim, idle, boost::bind( &weapon_core::instant_idle_predicate, this ) );
	m_logic->add_transition( aim, reload, boost::bind( &weapon_core::target_predicate, this, weapon_target_reload ) );
	m_logic->add_transition( aim, fire, boost::bind( &weapon_core::target_predicate, this, weapon_target_fire ) );
	m_logic->add_transition( aim, aim_fire, boost::bind( &weapon_core::target_predicate, this, weapon_target_aim_fire ) );
	m_logic->add_transition( aim_fire, hide, boost::bind( &weapon_core::target_and_animation_ended_predicate, this, weapon_target_inactive ) );
	if ( chamber_a_round_aimed )
		m_logic->add_transition( aim_fire, chamber_a_round_aimed, boost::bind( &weapon_core::must_chamber_a_round_aimed_and_animation_ended_predicate, this ) );
	if ( m_is_there_chamber_a_round_state )
		m_logic->add_transition( aim_fire, chamber_a_round, boost::bind( &weapon_core::must_chamber_a_round_and_animation_ended_predicate, this ) );
	m_logic->add_transition( aim_fire, reload, boost::bind( &weapon_core::can_and_must_reload_and_animation_ended_predicate, this ) );
	m_logic->add_transition( aim_fire, idle, boost::bind( &weapon_core::target_and_animation_ended_predicate, this, weapon_target_idle ) );
	m_logic->add_transition( aim_fire, reload, boost::bind( &weapon_core::target_and_animation_ended_predicate, this, weapon_target_reload ) );
	m_logic->add_transition( aim_fire, fire, boost::bind( &weapon_core::target_and_animation_ended_predicate, this, weapon_target_fire ) );
	m_logic->add_transition( aim_fire, aim, boost::bind( &weapon_core::target_and_animation_ended_predicate, this, weapon_target_aim ) );
	m_logic->add_transition( aim_fire, fire, boost::bind( &weapon_core::is_not_trying_to_aim_predicate, this ) );
	if ( m_is_there_chamber_a_round_state )
	{
		m_logic->add_transition( chamber_a_round, hide, boost::bind( &weapon_core::target_predicate, this, weapon_target_inactive ) );
		m_logic->add_transition( chamber_a_round, idle, boost::bind( &weapon_core::target_and_animation_ended_predicate, this, weapon_target_idle ) );
		m_logic->add_transition( chamber_a_round, idle, boost::bind( &weapon_core::instant_idle_predicate, this ) );
		m_logic->add_transition( chamber_a_round, fire, boost::bind( &weapon_core::target_and_animation_ended_predicate, this, weapon_target_fire ) );
		m_logic->add_transition( chamber_a_round, aim, boost::bind( &weapon_core::target_and_animation_ended_predicate, this, weapon_target_aim ) );
		m_logic->add_transition( chamber_a_round, aim_fire, boost::bind( &weapon_core::target_and_animation_ended_predicate, this, weapon_target_aim_fire ) );
		if ( chamber_a_round_aimed )
			m_logic->add_transition( chamber_a_round, chamber_a_round_aimed, boost::bind( &weapon_core::is_trying_to_aim, this ) );
	}
	if ( chamber_a_round_aimed )
	{
		m_logic->add_transition( chamber_a_round_aimed, hide, boost::bind( &weapon_core::target_predicate, this, weapon_target_inactive ) );
		m_logic->add_transition( chamber_a_round_aimed, idle, boost::bind( &weapon_core::target_and_animation_ended_predicate, this, weapon_target_idle ) );
		m_logic->add_transition( chamber_a_round_aimed, idle, boost::bind( &weapon_core::instant_idle_predicate, this ) );
		m_logic->add_transition( chamber_a_round_aimed, fire, boost::bind( &weapon_core::target_and_animation_ended_predicate, this, weapon_target_fire ) );
		m_logic->add_transition( chamber_a_round_aimed, aim, boost::bind( &weapon_core::target_and_animation_ended_predicate, this, weapon_target_aim ) );
		m_logic->add_transition( chamber_a_round_aimed, aim_fire, boost::bind( &weapon_core::target_and_animation_ended_predicate, this, weapon_target_aim_fire ) );
		m_logic->add_transition( chamber_a_round_aimed, chamber_a_round, boost::bind( &weapon_core::is_not_trying_to_aim_predicate, this ) );
	}
}

// FUNCTION BODY (target carcass, kept inline per MATCHING.md - non-100%; <N> = no-address)
	// <0>
	// <0x5a7034>|0x014|+0x023:'128'
	// <0x5a7057>|0x037|+0x00c:'129'
	// <0>
	// <0x5a7063>|0x043|+0x015:'131'
	// <0x5a7078>|0x058|+0x015:'132'
	// <0x5a708d>|0x06d|+0x015:'133'
	// <0x5a70a2>|0x082|+0x015:'134'
	// <0x5a70b7>|0x097|+0x015:'135'
	// <0x5a70cc>|0x0ac|+0x015:'136'
	// <0x5a70e1>|0x0c1|+0x015:'137'
	// <0x5a70f6>|0x0d6|+0x015:'138'
	// <0>
	// <0x5a710b>|0x0eb|+0x011:'140'
	// <0x5a711c>|0x0fc|+0x015:'141'
	// <0>
	// <0x5a7131>|0x111|+0x027:'143'
	// <0x5a7158>|0x138|+0x015:'144'
	// <0>
	// <0x5a716d>|0x14d|+0x008:'146'
	// <0x5a7175>|0x155|+0x008:'147'
	// <0x5a717d>|0x15d|+0x008:'148'
	// <0x5a7185>|0x165|+0x008:'149'
	// <0x5a718d>|0x16d|+0x008:'150'
	// <0x5a7195>|0x175|+0x008:'151'
	// <0x5a719d>|0x17d|+0x008:'152'
	// <0x5a71a5>|0x185|+0x008:'153'
	// <0x5a71ad>|0x18d|+0x008:'154'
	// <0>
	// <1>
	// <0x5a71b5>|0x195|+0x008:'157'
	// <0>
	// <0x5a71bd>|0x19d|+0x015:'159'
	// <0x5a71d2>|0x1b2|+0x015:'160'
	// <0>
	// <0x5a71e7>|0x1c7|+0x015:'162'
	// <0x5a71fc>|0x1dc|+0x015:'163'
	// <0x5a7211>|0x1f1|+0x015:'164'
	// <0x5a7226>|0x206|+0x015:'165'
	// <0x5a723b>|0x21b|+0x015:'166'
	// <0x5a7250>|0x230|+0x015:'167'
	// <0x5a7265>|0x245|+0x015:'168'
	// <0x5a727a>|0x25a|+0x015:'169'
	// <0x5a728f>|0x26f|+0x011:'170'
	// <0x5a72a0>|0x280|+0x015:'171'
	// <0x5a72b5>|0x295|+0x006:'172'
	// <0x5a72bb>|0x29b|+0x015:'173'
	// <0>
	// <0x5a72d0>|0x2b0|+0x0ab:'175'
	// <0x5a737b>|0x35b|+0x0c6:'176'
	// <0x5a7441>|0x421|+0x0c6:'177'
	// <0x5a7507>|0x4e7|+0x0c6:'178'
	// <0x5a75cd>|0x5ad|+0x0c6:'179'
	// <0>
	// <0x5a7693>|0x673|+0x0c6:'181'
	// <0x5a7759>|0x739|+0x00a:'182'
	// <0x5a7763>|0x743|+0x0c4:'183'
	// <0x5a7827>|0x807|+0x015:'184'
	// <0x5a783c>|0x81c|+0x0c4:'185'
	// <0x5a7900>|0x8e0|+0x0c4:'186'
	// <0x5a79c4>|0x9a4|+0x0c6:'187'
	// <0x5a7a8a>|0xa6a|+0x0c6:'188'
	// <0x5a7b50>|0xb30|+0x0c6:'189'
	// <0x5a7c16>|0xbf6|+0x0c6:'190'
	// <0x5a7cdc>|0xcbc|+0x0c6:'191'
	// <0>
	// <0x5a7da2>|0xd82|+0x0c6:'193'
	// <0x5a7e68>|0xe48|+0x0c6:'194'
	// <0x5a7f2e>|0xf0e|+0x0c6:'195'
	// <0x5a7ff4>|0xfd4|+0x0c6:'196'
	// <0x5a80ba>|0x109a|+0x0c6:'197'
	// <0x5a8180>|0x1160|+0x0c6:'198'
	// <0>
	// <0x5a8246>|0x1226|+0x0c6:'200'
	// <0x5a830c>|0x12ec|+0x00a:'201'
	// <0x5a8316>|0x12f6|+0x0c4:'202'
	// <0x5a83da>|0x13ba|+0x015:'203'
	// <0x5a83ef>|0x13cf|+0x0c4:'204'
	// <0x5a84b3>|0x1493|+0x0c6:'205'
	// <0x5a8579>|0x1559|+0x0c6:'206'
	// <0x5a863f>|0x161f|+0x0c6:'207'
	// <0x5a8705>|0x16e5|+0x0c6:'208'
	// <0>
	// <0x5a87cb>|0x17ab|+0x0c6:'210'
	// <0x5a8891>|0x1871|+0x02a:'211'
	// <0x5a88bb>|0x189b|+0x0c4:'212'
	// <0x5a897f>|0x195f|+0x0c6:'213'
	// <0x5a8a45>|0x1a25|+0x0c4:'214'
	// <0x5a8b09>|0x1ae9|+0x0c6:'215'
	// <0x5a8bcf>|0x1baf|+0x0c6:'216'
	// <0x5a8c95>|0x1c75|+0x0c6:'217'
	// <0>
	// <0x5a8d5b>|0x1d3b|+0x0c6:'219'
	// <0x5a8e21>|0x1e01|+0x00a:'220'
	// <0x5a8e2b>|0x1e0b|+0x0c4:'221'
	// <0x5a8eef>|0x1ecf|+0x015:'222'
	// <0x5a8f04>|0x1ee4|+0x0c4:'223'
	// <0x5a8fc8>|0x1fa8|+0x0c4:'224'
	// <0x5a908c>|0x206c|+0x0c6:'225'
	// <0x5a9152>|0x2132|+0x0c6:'226'
	// <0x5a9218>|0x21f8|+0x0c6:'227'
	// <0x5a92de>|0x22be|+0x0c6:'228'
	// <0x5a93a4>|0x2384|+0x0c4:'229'
	// <0>
	// <0x5a9468>|0x2448|+0x0c6:'231'
	// <0x5a952e>|0x250e|+0x0c6:'232'
	// <0x5a95f4>|0x25d4|+0x0c4:'233'
	// <0x5a96b8>|0x2698|+0x0c6:'234'
	// <0x5a977e>|0x275e|+0x0c6:'235'
	// <0x5a9844>|0x2824|+0x0c6:'236'
	// <0>
	// <0x5a990a>|0x28ea|+0x0c6:'238'
	// <0x5a99d0>|0x29b0|+0x00a:'239'
	// <0x5a99da>|0x29ba|+0x0c4:'240'
	// <0x5a9a9e>|0x2a7e|+0x015:'241'
	// <0x5a9ab3>|0x2a93|+0x0c4:'242'
	// <0x5a9b77>|0x2b57|+0x0c4:'243'
	// <0x5a9c3b>|0x2c1b|+0x0c6:'244'
	// <0x5a9d01>|0x2ce1|+0x0c6:'245'
	// <0x5a9dc7>|0x2da7|+0x0c6:'246'
	// <0x5a9e8d>|0x2e6d|+0x08a:'247'
	// <0x5a9f17>|0x2ef7|+0x085:'248'
	// <0>
	// <0x5a9f9c>|0x2f7c|+0x015:'250'
	// <0>
	// <0x5a9fb1>|0x2f91|+0x08a:'252'
	// <0x5aa03b>|0x301b|+0x08a:'253'
	// <0x5aa0c5>|0x30a5|+0x085:'254'
	// <0x5aa14a>|0x312a|+0x08a:'255'
	// <0x5aa1d4>|0x31b4|+0x08a:'256'
	// <0x5aa25e>|0x323e|+0x08a:'257'
	// <0x5aa2e8>|0x32c8|+0x00a:'258'
	// <0x5aa2f2>|0x32d2|+0x085:'259'
	// <0>
	// <1>
	// <0x5aa377>|0x3357|+0x00a:'262'
	// <0>
	// <0x5aa381>|0x3361|+0x08a:'264'
	// <0x5aa40b>|0x33eb|+0x08a:'265'
	// <0x5aa495>|0x3475|+0x085:'266'
	// <0x5aa51a>|0x34fa|+0x08a:'267'
	// <0x5aa5a4>|0x3584|+0x08a:'268'
	// <0x5aa62e>|0x360e|+0x08a:'269'
	// <0x5aa6b8>|0x3698|+0x085:'270'
	// <0>
	// ******

// STATE[100%|DONE]
void weapon_core::set_skeleton( resources::resource_ptr<animation::skeleton,resources::unmanaged_intrusive_base> const& skeleton )
{
	m_skeleton = skeleton;
}

// STATE[85.68%|PARTIAL]: control-flow byte-identical; residual = LTCG inline-vs-call of trivial
// weapon_core_base_state::has_animation_ended() (target /Od `call`, our /GL inlines the [+135h]
// read). Same class as must_chamber_a_round_and_animation_ended_predicate.
bool weapon_core::target_and_animation_ended_predicate( weapon_targets target ) const
{
	return m_target == target && current_base_state( ).has_animation_ended( );

	// FUNCTION BODY
	// <0xbc609>|0x009|+0x04b:'286'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::weapon_core::get_weapon_and_hands_animation_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
animation::mixing::expression weapon_core::get_weapon_and_hands_animation_expression(
	mutable_buffer&						buffer,
	bool								is_third_view,
	weapon_user_state_enum				weapon_user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	// CALL SITE INFO
	// <0x5a37a4> -> animation::mixing::expression <unknown>(mutable_buffer&, const bool, const weapon_user_state_enum, animation::mixing::animation_lexeme&) const
	// ******

	// FUNCTION BODY
	// <0x5a3769>|0x009|+0x040:'291'
	// ******
}

// STATE[83.89%|INPROGRESS]: body matches; base emits 3 temp slots, target 4 (one
// extra reference-materialization between current_base_state() and the member read).
// The ref local closed most of the gap (73%->83.89%); the last temp depends on the
// original expression nesting of current_base_state(). See weapon_core_get_body_part_mask_for_user.md
animation::body_part_masks_enum weapon_core::get_body_part_mask_for_user( ) const
{
	weapon_core_base_state const& state = current_base_state( );
	return state.get_body_part_mask_for_user( );

	// FUNCTION BODY
	// <0x5a3459>|0x009|+0x02a:'296'
	// ******
}
/*
// STATE[STUB]
// void survarium::`dynamic initializer for 'epsilon''()
void `dynamic initializer for 'epsilon''( )
{
	// FUNCTION BODY
	// <0x7d9730>|0x000|      :'300'	{
	// ******
}
*/
// STATE[STUB]
// float survarium::weapon_core::horizontal_recoil_value() const
float weapon_core::horizontal_recoil_value( ) const
{
	// LOCALS
	// float 						result
	// float 						total_horizontal_coeff
	// ******

	return 0.0f;

	// FUNCTION BODY
	// <0>
	// <1>
	// <0xbc669>|0x009|+0x050:'312'
	// <0xbc6b9>|0x059|+0x04b:'313'
	// <0xbc704>|0x0a4|+0x003:'314'
	// ******
}

// STATE[STUB]
// float survarium::weapon_core::vertical_recoil_value() const
float weapon_core::vertical_recoil_value( ) const
{
	// LOCALS
	// float 						result
	// float 						total_vertical_coeff
	// ******

	return 1.0f;

	// FUNCTION BODY
	// <0>
	// <1>
	// <0xbc719>|0x009|+0x050:'321'
	// <0>
	// <0xbc769>|0x059|+0x047:'323'
	// <0xbc7b0>|0x0a0|+0x003:'324'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::weapon_core::selected_animations(vostok::mutable_buffer&, const bool) const
animation::mixing::expression weapon_core::selected_animations( mutable_buffer& buffer, bool is_third_view ) const
{

	VOSTOK_UNREACHABLE_CODE( );

	// LOCALS
	// animation::mixing::expression const& expression_for_weapon_and_hands
	// std::pair<animation::mixing::expression,animation::mixing::animation_lexeme> result
	// ******

	// FUNCTION BODY
	// <0>
	// <1>
	// <2>
	// <0x5a3a50>|0x010|+0x00f:'332'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <12>
	// <0x5a3a5f>|0x01f|+0x1bd:'346'
	// <0>
	// <0x5a3c1c>|0x1dc|+0x040:'348'
	// <0x5a3c5c>|0x21c|+0x033:'349'
	// ******
}

// STATE[72.04%|INPROGRESS]: body matches; sole diff is is_aimed() - target emits a
// `call is_aimed` (standalone FPO symbol) but under /Od our inline is_aimed() expands
// to `mov [this+0x488]` inline. Out-lining is_aimed reaches 99.78% here but FPO-breaks
// many other inline callers (net -26 exact). Next: revisit once a per-call-site
// inline/out-of-line story exists. See docs/.../weapon_core_update_recoil.md
void weapon_core::update_recoil( u32 current_time_in_ms, float time_scale )
{
	m_recoil_calculator.tick( m_user_animations_selector.get_current_state_id( ), is_aimed( ), current_time_in_ms, time_scale );

	// FUNCTION BODY
	// <0x5a3717>|0x007|+0x034:'354'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::update_dispersion(const bool, unsigned int)
void weapon_core::update_dispersion( bool is_moving, u32 current_time_in_ms )
{
	// CALL SITE INFO
	// <0x5a33dc> -> resources::resource_ptr<damage_model,resources::unmanaged_intrusive_base> const& <unknown>() const
	// ******

	// FUNCTION BODY
	// <0x5a33b9>|0x009|+0x00c:'359'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x5a33c5>|0x015|+0x07c:'367'
	// ******
}

// STATE[100%|DONE]
void weapon_core::update_breath_vibration( bool is_holding_breath, u32 current_time_in_ms, float time_scale )
{
	m_breath_vibration_calculator.hold_breath( is_holding_breath );
	m_breath_vibration_calculator.set_character_multiplier( 0.0f );
	m_breath_vibration_calculator.tick( current_time_in_ms, time_scale );
}

// STATE[STUB]
// void survarium::weapon_core::tick()
void weapon_core::tick( )
{
	// LOCALS
	// player_input const& 			input
	// resources::resource_ptr<interactive_object,resources::unmanaged_intrusive_base> target_active_object
	// ******

	// CALL SITE INFO
	// <0x5a58c2> -> player_input const& <unknown>() const
	// <0x5a5978> -> void <unknown>()
	// <0x5a5998> -> void <unknown>()
	// <0x5a59d6> -> void <unknown>(const weapon_targets)
	// <0x5a5a05> -> void <unknown>(const weapon_targets)
	// <0x5a5a38> -> void <unknown>(const weapon_targets)
	// <0x5a5a57> -> void <unknown>(const weapon_targets)
	// <0x5a5a7c> -> void <unknown>(const weapon_targets)
	// ******

	// FUNCTION BODY
	// <0>
	// <1>
	// <0x5a58a9>|0x009|+0x01e:'381'
	// <0x5a58c7>|0x027|+0x01a:'382'
	// <0>
	// <0x5a58e1>|0x041|+0x00f:'384'
	// <0>
	// <0x5a58f0>|0x050|+0x070:'386'
	// <0>
	// <0x5a5960>|0x0c0|+0x00d:'388'
	// <0x5a596d>|0x0cd|+0x00f:'389'
	// <0x5a597c>|0x0dc|+0x00e:'390'
	// <0x5a598a>|0x0ea|+0x010:'391'
	// <0x5a599a>|0x0fa|+0x002:'392'
	// <0x5a599c>|0x0fc|+0x008:'393'
	// <0>
	// <1>
	// <0x5a59a4>|0x104|+0x022:'396'
	// <0>
	// <0x5a59c6>|0x126|+0x017:'398'
	// <0x5a59dd>|0x13d|+0x018:'399'
	// <0x5a59f5>|0x155|+0x014:'400'
	// <0x5a5a09>|0x169|+0x00b:'401'
	// <0x5a5a14>|0x174|+0x028:'402'
	// <0x5a5a3c>|0x19c|+0x00b:'403'
	// <0x5a5a47>|0x1a7|+0x012:'404'
	// <0x5a5a59>|0x1b9|+0x002:'405'
	// <0x5a5a5b>|0x1bb|+0x023:'406'
	// <0>
	// <1>
	// <0x5a5a7e>|0x1de|+0x00e:'409'
	// <0>
	// <1>
	// <2>
	// <0x5a5a8c>|0x1ec|+0x00e:'413'
	// <0>
	// <0x5a5a9a>|0x1fa|+0x00f:'415'
	// <0x5a5aa9>|0x209|+0x00a:'416'
	// <0>
	// <1>
	// <0x5a5ab3>|0x213|+0x00f:'419'
	// ******
}

// STATE[100%|DONE]
void weapon_core::instant_show( )
{
	m_aimed = false;
	on_show( );
}

// STATE[100%|DONE]
void weapon_core::instant_hide( )
{
	on_hide( );
}

// STATE[STUB]
// void survarium::weapon_core::load_magazine()
void weapon_core::load_magazine( )
{
	// LOCALS
	// u16 							load
	// u16 							amount
	// ******

	// FUNCTION BODY
	// <0x5a4b99>|0x009|+0x03e:'435'
	// <0x5a4bd7>|0x047|+0x040:'436'
	// <0x5a4c17>|0x087|+0x07f:'437'
	// <0x5a4c96>|0x106|+0x01a:'438'
	// <0>
	// <0x5a4cb0>|0x120|+0x00e:'440'
	// <0x5a4cbe>|0x12e|+0x008:'441'
	// ******
}

// STATE[100%|DONE]
void weapon_core::chamber_a_round( )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_ammo_in_magazine ) );
	ASSERT( UNKNOWN_EXPRESSION_T( m_ammo_in_magazine ) );
	ASSERT( UNKNOWN_EXPRESSION_T( m_ammo_in_magazine ) );

	--m_ammo_in_magazine;
	m_is_round_chambered = true;
}

// STATE[STUB]
// void survarium::weapon_core::unload_ammo()
void weapon_core::unload_ammo( )
{
	// LOCALS
	// u16 							ammo_to_add
	// ******

	// FUNCTION BODY
	// <0x5a4a49>|0x009|+0x034:'456'
	// <0x5a4a7d>|0x03d|+0x005:'457'
	// <0>
	// <0x5a4a82>|0x042|+0x00e:'459'
	// <0x5a4a90>|0x050|+0x00c:'460'
	// <0x5a4a9c>|0x05c|+0x00e:'461'
	// <0>
	// <0x5a4aaa>|0x06a|+0x00c:'463'
	// <0x5a4ab6>|0x076|+0x00a:'464'
	// <0>
	// <1>
	// <0x5a4ac0>|0x080|+0x0c0:'467'
	// ******
}

// STATE[100%|DONE]
void weapon_core::instant_reload( )
{
	m_aimed = false;
	load_magazine( );
	on_reload( );
	m_recoil_calculator.reload( );
	m_dispersion_calculator.reload( );
	reset_fire_queue( );
}

// STATE[100%|DONE]
void weapon_core::instant_chamber_a_round( )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_ammo_in_magazine ) );
	ASSERT( UNKNOWN_EXPRESSION_T( m_ammo_in_magazine ) );

	m_aimed = false;
	chamber_a_round( );
	on_chamber_a_round( );
	m_recoil_calculator.chamber_a_round( );
}

// STATE[STUB]
// void survarium::weapon_core::reload_one_round()
void weapon_core::reload_one_round( )
{
	// CALL SITE INFO
	// <0x5a4a2f> -> void <unknown>()
	// ******

	// FUNCTION BODY
	// <0x5a48c0>|0x010|+0x08c:'502'
	// <0x5a494c>|0x09c|+0x018:'503'
	// <0x5a4964>|0x0b4|+0x0bd:'504'
	// <0>
	// <0x5a4a21>|0x171|+0x010:'506'
	// ******
}

// STATE[100%|DONE]
void weapon_core::instant_aim_start( )
{
	if ( !is_firing( ) && !( m_user->input( ).actions_mask & 0x20 ) )
		reset_fire_queue( );

	m_aimed = true;
	m_aiming_state_transition = true;
}

// STATE[100%|DONE]
void weapon_core::instant_aim_end( )
{
	m_aimed = false;
	m_aiming_state_transition = true;
}

// STATE[STUB]
// vostok::math::float3 survarium::weapon_core::get_dispersed_bullet_dir()
float3 weapon_core::get_dispersed_bullet_dir( )
{
	// LOCALS
	// float 						dispersion_angle
	// float 						dispersion_amount
	// float3 const& 				rot_axis
	// float 						random_k
	// float3 						bullet_direction
	// ******

	return vostok::math::float3(1., 1., 1.);

	// FUNCTION BODY
	// <0>
	// <0x5a4792>|0x012|+0x040:'527'
	// <0x5a47d2>|0x052|+0x017:'528'
	// <0x5a47e9>|0x069|+0x01e:'529'
	// <0x5a4807>|0x087|+0x047:'530'
	// <0x5a484e>|0x0ce|+0x03e:'531'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x5a488c>|0x10c|+0x017:'539'
	// ******
}

// STATE[100%|DONE]
float weapon_core::get_dispersion( ) const
{
	return m_dispersion_calculator.get_dispersion( );
}

// STATE[STUB]
// void survarium::weapon_core::instant_fire(const unsigned int)
void weapon_core::instant_fire( u32 current_time_in_ms )
{
	// LOCALS
	// s32 							i<1>
	// float3 const& 				velocity<2>
	// float3 const& 				bullet_direction<2>
	// ******

	// SKIPPED BLOCKS
	// <0x5a56e4><2>
	// ******

	// CALL SITE INFO
	// <0x5a57f9> -> void <unknown>()
	// <0x5a582e> -> void <unknown>()
	// ******

	// FUNCTION BODY
	// <0x5a5669>|0x009|+0x00c:'549'
	// <0x5a5675>|0x015|+0x00c:'550'
	// <0x5a5681>|0x021|+0x018:'551'
	// <0>
	// <0x5a5699>|0x039|+0x00c:'553'
	// <0x5a56a5>|0x045|+0x045|[1]:'554'
	// <0x5a56ea>|0x08a|+0x012:'555'
	// <0>
	// <0x5a56fc>|0x09c|+0x04a:'557'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <0x5a5746>|0x0e6|+0x06e:'568'
	// <0x5a57b4>|0x154|+0x005:'569'
	// <0>
	// <0x5a57b9>|0x159|+0x00e:'571'
	// <0x5a57c7>|0x167|+0x00a:'572'
	// <0x5a57d1>|0x171|+0x002:'573'
	// <0x5a57d3>|0x173|+0x018:'574'
	// <0>
	// <1>
	// <2>
	// <0x5a57eb>|0x18b|+0x010:'578'
	// <0>
	// <0x5a57fb>|0x19b|+0x00e:'580'
	// <0x5a5809>|0x1a9|+0x00e:'581'
	// <0>
	// <0x5a5817>|0x1b7|+0x019:'583'
	// ******
}

// STATE[100%|DONE]
void weapon_core::set_fire_bullet_transform( float4x4 const& fire_bullet_transform )
{
	m_ready_for_fire = true;
	m_fire_bullet_transform = fire_bullet_transform;
}

// STATE[STUB]
// vostok::animation::callback_return_type_enum survarium::weapon_core::on_sprint_animation_ended(vostok::animation::animation_callback_params&)
animation::callback_return_type_enum weapon_core::on_sprint_animation_ended( animation::animation_callback_params& params )
{
	// CALL SITE INFO
	// <0x5a302c> -> void <unknown>(animation::reserved_channel_ids_enum, pcvoid)
	// <0x5a3052> -> void <unknown>(animation::reserved_channel_ids_enum, pcvoid)
	// ******

	// FUNCTION BODY
	// <0x5a3009>|0x009|+0x007:'594'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5a3010>|0x010|+0x01e:'599'
	// <0>
	// <1>
	// <2>
	// <0x5a302e>|0x02e|+0x026:'603'
	// <0>
	// <1>
	// <2>
	// <0x5a3054>|0x054|+0x00a:'607'
	// <0>
	// <0x5a305e>|0x05e|+0x005:'609'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::set_target(const survarium::weapon_targets)
void weapon_core::set_target( weapon_targets target )
{
	// CALL SITE INFO
	// <0x5a521c> -> bool <unknown>() const
	// <0x5a5350> -> void <unknown>(animation::reserved_channel_ids_enum, boost::function<enum animation::callback_return_type_enum(animation::animation_callback_params &)> const&, pcvoid, resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base> const&, pcvoid const)
	// <0x5a5487> -> void <unknown>(animation::reserved_channel_ids_enum, boost::function<enum animation::callback_return_type_enum(animation::animation_callback_params &)> const&, pcvoid, resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base> const&, pcvoid const)
	// <0x5a560e> -> void <unknown>()
	// ******

	// FUNCTION BODY
	// <0x5a518a>|0x01a|+0x00c:'614'
	// <0>
	// <0x5a5196>|0x026|+0x029:'616'
	// <0x5a51bf>|0x04f|+0x009:'617'
	// <0x5a51c8>|0x058|+0x012:'618'
	// <0>
	// <0x5a51da>|0x06a|+0x006:'620'
	// <0x5a51e0>|0x070|+0x009:'621'
	// <0x5a51e9>|0x079|+0x006:'622'
	// <0x5a51ef>|0x07f|+0x007:'623'
	// <0>
	// <1>
	// <2>
	// <0x5a51f6>|0x086|+0x015:'627'
	// <0x5a520b>|0x09b|+0x01e:'628'
	// <0x5a5229>|0x0b9|+0x00d:'629'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5a5236>|0x0c6|+0x12c:'636'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5a5362>|0x1f2|+0x137:'643'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5a5499>|0x329|+0x029:'649'
	// <0x5a54c2>|0x352|+0x006:'650'
	// <0x5a54c8>|0x358|+0x009:'651'
	// <0x5a54d1>|0x361|+0x006:'652'
	// <0x5a54d7>|0x367|+0x007:'653'
	// <0>
	// <1>
	// <0x5a54de>|0x36e|+0x00a:'656'
	// <0>
	// <0x5a54e8>|0x378|+0x016:'658'
	// <0>
	// <0x5a54fe>|0x38e|+0x0fc:'660'
	// <0x5a55fa>|0x48a|+0x016:'661'
	// <0>
	// <0x5a5610>|0x4a0|+0x01e:'663'
	// <0x5a562e>|0x4be|+0x007:'664'
	// <0x5a5635>|0x4c5|+0x002:'665'
	// <0x5a5637>|0x4c7|+0x007:'666'
	// <0>
	// <1>
	// <2>
	// <0x5a563e>|0x4ce|+0x00f:'670'
	// ******
}

// STATE[99.65%|PARTIAL]: every instruction/op/order byte-identical; sole residual = register
// choice (eax vs ecx for `this` at the fire_queue_length call) + stack-slot NUMBERING in the
// else-branch min (target [ebp-2]/[ebp-6]/[ebp-4], base [ebp-4]/[ebp-8]/[ebp-6]); same 3-slot
// structure. Allocator nondeterminism, not source-steerable. See weapon_core_instant_batch6.md
void weapon_core::reset_fire_queue( )
{
	if ( fire_queue_length( ) == 0xff )
	{
		m_bullets_in_queue = m_ammo_in_magazine;
		if ( m_is_round_chambered )
			++m_bullets_in_queue;
	}
	else
	{
		u16 bullets_in_queue = m_ammo_in_magazine + ( m_is_round_chambered != 0 );
		m_bullets_in_queue = math::min( fire_queue_length( ), bullets_in_queue );
	}
}

// STATE[100%|DONE]
void weapon_core::set_next_fire_queue_type( )
{
	if ( m_fire_queue_type == m_weapon_fire_queue_types_count - 1 )
		m_fire_queue_type = 0;
	else
		++m_fire_queue_type;
}

// STATE[STUB]
// void survarium::weapon_core::set_next_ammo_type()
void weapon_core::set_next_ammo_type( )
{
	// LOCALS
	// profile_slot_enum 			next_slot
	// ******

	// FUNCTION BODY
	// <0>
	// <1>
	// <2>
	// <0x5a5089>|0x009|+0x007:'702'
	// <0>
	// <0x5a5090>|0x010|+0x015:'704'
	// <0>
	// <0x5a50a5>|0x025|+0x00f:'706'
	// <0>
	// <0x5a50b4>|0x034|+0x015:'708'
	// <0>
	// <0x5a50c9>|0x049|+0x00d:'710'
	// <0>
	// <0x5a50d6>|0x056|+0x002:'712'
	// <0x5a50d8>|0x058|+0x005:'713'
	// <0>
	// <0x5a50dd>|0x05d|+0x020:'715'
	// <0x5a50fd>|0x07d|+0x002:'716'
	// <0>
	// <0x5a50ff>|0x07f|+0x008:'718'
	// <0>
	// <0x5a5107>|0x087|+0x00c:'720'
	// <0x5a5113>|0x093|+0x045:'721'
	// <0x5a5158>|0x0d8|+0x00d:'722'
	// ******
}

// STATE[100%|DONE]
void weapon_core::set_ammunition( resources::resource_ptr<weapon_ammunition,resources::unmanaged_intrusive_base> const& ammunition_to_set )
{
	m_ammunition = ammunition_to_set;
}

// STATE[STUB]
// void survarium::weapon_core::load_ammo()
void weapon_core::load_ammo( )
{
	// FUNCTION BODY
	// <0x5a4ea0>|0x010|+0x08f:'732'
	// <0>
	// <0x5a4f2f>|0x09f|+0x00e:'734'
	// <0x5a4f3d>|0x0ad|+0x008:'735'
	// <0>
	// <0x5a4f45>|0x0b5|+0x024:'737'
	// <0>
	// <0x5a4f69>|0x0d9|+0x00c:'739'
	// <0>
	// <0x5a4f75>|0x0e5|+0x04a:'741'
	// <0>
	// <0x5a4fbf>|0x12f|+0x00a:'743'
	// <0x5a4fc9>|0x139|+0x07c:'744'
	// <0>
	// <0x5a5045>|0x1b5|+0x010:'746'
	// <0>
	// <0x5a5055>|0x1c5|+0x00a:'748'
	// <0x5a505f>|0x1cf|+0x018:'749'
	// <0>
	// <1>
	// <2>
	// ******
}

// STATE[100%|DONE]
void weapon_core::on_reload_started( )
{
}

// STATE[STUB]
// vostok::animation::callback_return_type_enum survarium::weapon_core::on_animation_ik_interval(vostok::animation::animation_callback_params&)
animation::callback_return_type_enum weapon_core::on_animation_ik_interval( animation::animation_callback_params& params )
{
	// FUNCTION BODY
	// <0>
	// <0x5a35f9>|0x009|+0x007:'762'
	// <0>
	// <0x5a3600>|0x010|+0x014:'764'
	// <0>
	// <0x5a3614>|0x024|+0x017:'766'
	// <0x5a362b>|0x03b|+0x027:'767'
	// <0x5a3652>|0x062|+0x017:'768'
	// <0x5a3669>|0x079|+0x024:'769'
	// <0x5a368d>|0x09d|+0x017:'770'
	// <0x5a36a4>|0x0b4|+0x024:'771'
	// <0x5a36c8>|0x0d8|+0x017:'772'
	// <0x5a36df>|0x0ef|+0x022:'773'
	// <0>
	// <0x5a3701>|0x111|+0x002:'775'
	// ******
}

// STATE[100%|DONE]
void weapon_core::set_animation_callback( pcstr channel_id, pcvoid callback_uid, boost::function<enum animation::callback_return_type_enum(animation::animation_callback_params &)> const& animation_callback )
{
	// claude@MATCH: named local materializes the managed_resource_ptr(NULL) temp ahead of
	// the argument pushes, matching the target's temp scheduling (push 0;ctor before push this).
	resources::managed_resource_ptr tmp( NULL );
	m_user->subscribe_animation_player( channel_id, animation_callback, callback_uid, tmp, 0xff, this );
}

// STATE[100%|DONE]
void weapon_core::remove_animation_callback( pcstr channel_id, pcvoid callback_uid )
{
	m_user->unsubscribe_animation_player( channel_id, callback_uid );
}

// STATE[100%|DONE]
void weapon_core::set_animation_callback( animation::reserved_channel_ids_enum channel_id, pcvoid callback_uid, boost::function<enum animation::callback_return_type_enum(animation::animation_callback_params &)> const& animation_callback )
{
	// claude@MATCH: named local materializes the managed_resource_ptr(NULL) temp ahead of
	// the argument pushes, matching the target's temp scheduling (push 0;ctor before push this).
	resources::managed_resource_ptr tmp( NULL );
	m_user->subscribe_animation_player( channel_id, animation_callback, callback_uid, tmp, this );
}

// STATE[100%|DONE]
void weapon_core::remove_animation_callback( animation::reserved_channel_ids_enum channel_id, pcvoid callback_uid )
{
	m_user->unsubscribe_animation_player( channel_id, callback_uid );
}

// STATE[STUB]
// void survarium::weapon_core::activate(survarium::base_player&, survarium::engine&)
void weapon_core::activate( base_player& user, engine& engine )
{
	// LOCALS
	// resources::resource_ptr<inventory_item,resources::unmanaged_intrusive_base> const& ammo1
	// resources::resource_ptr<inventory_item,resources::unmanaged_intrusive_base> const& ammo2
	// profile_slot_enum 			ammo1_slot
	// profile_slot_enum 			ammo2_slot
	// ******

	// CALL SITE INFO
	// <0x5a5fe1> -> bullet_manager& <unknown>() const
	// <0x5a60b5> -> float4x4 const& <unknown>() const
	// <0x5a60db> -> float4x4 const& <unknown>() const
	// <0x5a60f2> -> void <unknown>(float4x4 const&)
	// <0x5a61cb> -> void <unknown>(pcstr, boost::function<enum animation::callback_return_type_enum(animation::animation_callback_params &)> const&, pcvoid, resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base> const&, const u8, pcvoid const)
	// <0x5a62aa> -> void <unknown>(pcstr, boost::function<enum animation::callback_return_type_enum(animation::animation_callback_params &)> const&, pcvoid, resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base> const&, const u8, pcvoid const)
	// <0x5a63a1> -> void <unknown>(pcstr, boost::function<enum animation::callback_return_type_enum(animation::animation_callback_params &)> const&, pcvoid, resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base> const&, const u8, pcvoid const)
	// <0x5a649e> -> void <unknown>(pcstr, boost::function<enum animation::callback_return_type_enum(animation::animation_callback_params &)> const&, pcvoid, resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base> const&, const u8, pcvoid const)
	// <0x5a65a0> -> void <unknown>(pcstr, boost::function<enum animation::callback_return_type_enum(animation::animation_callback_params &)> const&, pcvoid, resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base> const&, const u8, pcvoid const)
	// <0x5a66a2> -> void <unknown>(pcstr, boost::function<enum animation::callback_return_type_enum(animation::animation_callback_params &)> const&, pcvoid, resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base> const&, const u8, pcvoid const)
	// <0x5a66d6> -> animation::skeleton const& <unknown>() const
	// <0x5a66f5> -> animation::skeleton const& <unknown>() const
	// <0x5a6726> -> physics::bt_character_controller& <unknown>()
	// <0x5a6ce3> -> void <unknown>(pcstr, boost::function<enum animation::callback_return_type_enum(animation::animation_callback_params &)> const&, pcvoid, resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base> const&, const u8, pcvoid const)
	// <0x5a6dec> -> void <unknown>(pcstr, boost::function<enum animation::callback_return_type_enum(animation::animation_callback_params &)> const&, pcvoid, resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base> const&, const u8, pcvoid const)
	// <0x5a6ef5> -> void <unknown>(pcstr, boost::function<enum animation::callback_return_type_enum(animation::animation_callback_params &)> const&, pcvoid, resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base> const&, const u8, pcvoid const)
	// <0x5a6ffe> -> void <unknown>(pcstr, boost::function<enum animation::callback_return_type_enum(animation::animation_callback_params &)> const&, pcvoid, resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base> const&, const u8, pcvoid const)
	// ******

	// FUNCTION BODY
	// <0x5a5f6b>|0x01b|+0x01a:'800'
	// <0x5a5f85>|0x035|+0x01b:'801'
	// <0x5a5fa0>|0x050|+0x00f:'802'
	// <0x5a5faf>|0x05f|+0x01b:'803'
	// <0x5a5fca>|0x07a|+0x00d:'804'
	// <0x5a5fd7>|0x087|+0x018:'805'
	// <0x5a5fef>|0x09f|+0x00d:'806'
	// <0x5a5ffc>|0x0ac|+0x00d:'807'
	// <0x5a6009>|0x0b9|+0x030:'808'
	// <0x5a6039>|0x0e9|+0x030:'809'
	// <0x5a6069>|0x119|+0x00d:'810'
	// <0>
	// <0x5a6076>|0x126|+0x018:'812'
	// <0x5a608e>|0x13e|+0x018:'813'
	// <0>
	// <1>
	// <0x5a60a6>|0x156|+0x026:'816'
	// <0x5a60cc>|0x17c|+0x028:'817'
	// <0>
	// <0x5a60f4>|0x1a4|+0x00f:'819'
	// <0>
	// <0x5a6103>|0x1b3|+0x0da:'821'
	// <0x5a61dd>|0x28d|+0x0df:'822'
	// <0x5a62bc>|0x36c|+0x0fd:'823'
	// <0x5a63b9>|0x469|+0x0fd:'824'
	// <0x5a64b6>|0x566|+0x102:'825'
	// <0x5a65b8>|0x668|+0x102:'826'
	// <0x5a66ba>|0x76a|+0x030:'827'
	// <0x5a66ea>|0x79a|+0x01f:'828'
	// <0>
	// <0x5a6709>|0x7b9|+0x037:'830'
	// <0>
	// <1>
	// <0x5a6740>|0x7f0|+0x16f:'833'
	// <0>
	// <1>
	// <0x5a68af>|0x95f|+0x028:'836'
	// <0x5a68d7>|0x987|+0x011:'837'
	// <0>
	// <1>
	// <0x5a68e8>|0x998|+0x010:'840'
	// <0x5a68f8>|0x9a8|+0x010:'841'
	// <0>
	// <0x5a6908>|0x9b8|+0x017:'843'
	// <0x5a691f>|0x9cf|+0x017:'844'
	// <0x5a6936>|0x9e6|+0x00c:'845'
	// <0x5a6942>|0x9f2|+0x053:'846'
	// <0>
	// <0x5a6995>|0xa45|+0x011:'848'
	// <0x5a69a6>|0xa56|+0x053:'849'
	// <0>
	// <0x5a69f9>|0xaa9|+0x005:'851'
	// <0x5a69fe>|0xaae|+0x08e:'852'
	// <0x5a6a8c>|0xb3c|+0x092:'853'
	// <0>
	// <1>
	// <0x5a6b1e>|0xbce|+0x081:'856'
	// <0>
	// <0x5a6b9f>|0xc4f|+0x011:'858'
	// <0>
	// <0x5a6bb0>|0xc60|+0x00b:'860'
	// <0x5a6bbb>|0xc6b|+0x00d:'861'
	// <0>
	// <1>
	// <0x5a6bc8>|0xc78|+0x00b:'864'
	// <0x5a6bd3>|0xc83|+0x00b:'865'
	// <0>
	// <0x5a6bde>|0xc8e|+0x00f:'867'
	// <0>
	// <0x5a6bed>|0xc9d|+0x10e:'869'
	// <0x5a6cfb>|0xdab|+0x109:'870'
	// <0x5a6e04>|0xeb4|+0x109:'871'
	// <0x5a6f0d>|0xfbd|+0x109:'872'
	// <0>
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::deactivate()
void weapon_core::deactivate( )
{
	// CALL SITE INFO
	// <0x5a404c> -> void <unknown>(pcstr, pcvoid)
	// <0x5a406d> -> void <unknown>(pcstr, pcvoid)
	// <0x5a408e> -> void <unknown>(pcstr, pcvoid)
	// <0x5a40af> -> void <unknown>(pcstr, pcvoid)
	// <0x5a4116> -> void <unknown>(pcstr, pcvoid)
	// <0x5a4138> -> void <unknown>(pcstr, pcvoid)
	// <0x5a415a> -> void <unknown>(pcstr, pcvoid)
	// <0x5a417c> -> void <unknown>(pcstr, pcvoid)
	// <0x5a419e> -> void <unknown>(pcstr, pcvoid)
	// <0x5a41c0> -> void <unknown>(pcstr, pcvoid)
	// <0x5a41ec> -> void <unknown>(animation::reserved_channel_ids_enum, pcvoid)
	// <0x5a4212> -> void <unknown>(animation::reserved_channel_ids_enum, pcvoid)
	// ******

	// FUNCTION BODY
	// <0x5a4019>|0x009|+0x00f:'878'
	// <0>
	// <0x5a4028>|0x018|+0x026:'880'
	// <0x5a404e>|0x03e|+0x021:'881'
	// <0x5a406f>|0x05f|+0x021:'882'
	// <0x5a4090>|0x080|+0x021:'883'
	// <0>
	// <1>
	// <0x5a40b1>|0x0a1|+0x010:'886'
	// <0x5a40c1>|0x0b1|+0x010:'887'
	// <0x5a40d1>|0x0c1|+0x00d:'888'
	// <0x5a40de>|0x0ce|+0x010:'889'
	// <0>
	// <0x5a40ee>|0x0de|+0x008:'891'
	// <0>
	// <0x5a40f6>|0x0e6|+0x022:'893'
	// <0x5a4118>|0x108|+0x022:'894'
	// <0x5a413a>|0x12a|+0x022:'895'
	// <0x5a415c>|0x14c|+0x022:'896'
	// <0x5a417e>|0x16e|+0x022:'897'
	// <0x5a41a0>|0x190|+0x022:'898'
	// <0>
	// <0x5a41c2>|0x1b2|+0x00e:'900'
	// <0>
	// <1>
	// <2>
	// <0x5a41d0>|0x1c0|+0x01e:'904'
	// <0>
	// <1>
	// <2>
	// <0x5a41ee>|0x1de|+0x026:'908'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5a4214>|0x204|+0x00e:'913'
	// <0>
	// <0x5a4222>|0x212|+0x010:'915'
	// ******
}

// STATE[84.77%|PARTIAL]: control-flow byte-identical; residual = LTCG inline-vs-call of the two
// trivial getters (weapon_user_animations_selector::is_ready_to_be_deactivated returns false
// standalone in target; current_base_state ref-copy shape). Same inline-decision class as the
// has_animation_ended predicates.
bool weapon_core::is_ready_to_be_deactivated( ) const
{
	return current_base_state( ).is_ready_to_be_deactivated( ) && m_user_animations_selector.is_ready_to_be_deactivated( );

	// FUNCTION BODY
	// <0x5a32e9>|0x009|+0x057:'920'
	// ******
}

// STATE[100%|DONE]
void weapon_core::on_player_model_added( )
{
	instant_show( );
}

// STATE[100%|DONE]
void weapon_core::on_player_model_removed( )
{
	instant_hide( );
}

// STATE[STUB]
// void survarium::weapon_core::update_bones_matrices(vostok::resources::resource_ptr<vostok::animation::skeleton,vostok::resources::unmanaged_intrusive_base> const&, vostok::math::float4x4* const, const unsigned int, const unsigned int, vostok::math::float4x4&, vostok::math::float4x4&, vostok::animation::animation_player const&)
void weapon_core::update_bones_matrices(
	resources::resource_ptr<animation::skeleton,resources::unmanaged_intrusive_base> const&	user_skeleton,
	float4x4*							user_matrices,
	u32									user_matrices_count,
	u32									current_time_in_ms,
	float4x4&							character_head_transform,
	float4x4&							character_transform,
	animation::animation_player const&	user_animation_player
)
{
	// LOCALS
	// u32 							weapon_bone_index
	// u32 							head_bone_index
	// float4x4* 					weapon_matrices
	// u32 							weapon_matrices_count
	// u32 							root_bone_index
	// float4x4 const& 				weapon_transform
	// float4x4 const& 				user_transform
	// float4x4* 					e<2>
	// float4x4* 					i<2>
	// u32 							j<3>
	// ******

	// STATICS
	// static bool 					draw_bones_matrices = <0x4c26664>;
	// ******

	// SKIPPED BLOCKS
	// <0x5a2a10><1>
	// <0x5a5dbb><2>
	// <0x5a5de0><3>
	// ******

	// CALL SITE INFO
	// <0x5a5b6d> -> player_input const& <unknown>() const
	// <0x5a5bfb> -> float4x4 const& <unknown>() const
	// <0x5a5d3d> -> physics::bt_character_controller& <unknown>()
	// <0x5a5d95> -> void <unknown>(const u32, float4x4* const)
	// <0x5a5e33> -> void <unknown>(float4x4 const&)
	// <0x5a5e7b> -> void <unknown>(const u32, float4x4 const&, float4x4 const* const, float4x4 const* const, float4x4 const&, float4x4* const, float4x4* const, float4x4 const&)
	// <0x5a5ee3> -> void <unknown>(float4x4 const&)
	// ******

	// FUNCTION BODY
	// <0x5a5b41>|0x011|+0x00f:'943'
	// <0x5a5b50>|0x020|+0x08a:'944'
	// <0>
	// <0x5a5bda>|0x0aa|+0x026:'946'
	// <0>
	// <0x5a5c00>|0x0d0|+0x01a:'948'
	// <0x5a5c1a>|0x0ea|+0x01f:'949'
	// <0>
	// <0x5a5c39>|0x109|+0x029:'951'
	// <0>
	// <0x5a5c62>|0x132|+0x026:'953'
	// <0>
	// <0x5a5c88>|0x158|+0x024:'955'
	// <0x5a5cac>|0x17c|+0x03f:'956'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5a5ceb>|0x1bb|+0x00b:'961'
	// <0x5a5cf6>|0x1c6|+0x01d:'962'
	// <0>
	// <1>
	// <2>
	// <0x5a5d13>|0x1e3|+0x038:'966'
	// <0>
	// <0x5a5d4b>|0x21b|+0x015:'968'
	// <0x5a5d60>|0x230|+0x019:'969'
	// <0>
	// <1>
	// <0x5a5d79>|0x249|+0x01e:'972'
	// <0x5a5d97>|0x267|+0x01b:'973'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <12>
	// <13>
	// <14>
	// <15>
	// <16>
	// <17>
	// <18>
	// <0x5a5db2>|0x282|+0x00b:'993'
	// <0>
	// <0x5a5dbd>|0x28d|+0x025:'995'
	// <0>
	// <1>
	// <0x5a5de2>|0x2b2|+0x018:'998'
	// <0x5a5dfa>|0x2ca|+0x002:'999'
	// <0x5a5dfc>|0x2cc|+0x002:'1000'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5a5dfe>|0x2ce|+0x020:'1005'
	// <0x5a5e1e>|0x2ee|+0x017:'1006'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <0x5a5e35>|0x305|+0x048:'1017'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5a5e7d>|0x34d|+0x026:'1023'
	// <0x5a5ea3>|0x373|+0x028:'1024'
	// <0x5a5ecb>|0x39b|+0x01a:'1025'
	// <0>
	// <0x5a5ee5>|0x3b5|+0x026:'1027'
	// <0x5a5f0b>|0x3db|+0x029:'1028'
	// ******
}

// STATE[100%|DONE]: body byte-identical (rich-index diff = 10/10 instrs equal). report.json
// shows 0% only because this 10-byte virtual is ICF-folded with an identical body in BOTH
// binaries, so neither delinked .obj keeps a distinct COMDAT for it (objdiff can't pair it -
// the documented COMDAT-fold 100<->0 artifact, ik_processor-ctor-activate.md). Source is correct.
bool weapon_core::is_sprinting( ) const
{
	return m_user_animations_selector.is_sprinting( );
}

// STATE[STUB]
// void survarium::weapon_core::on_skeleton_matrices_changed(const unsigned int, vostok::math::float4x4 const&, vostok::math::float4x4 const* const, vostok::math::float4x4 const* const, vostok::math::float4x4 const&, vostok::math::float4x4* const, vostok::math::float4x4* const, vostok::math::float4x4 const&)
void weapon_core::on_skeleton_matrices_changed(
	u32					current_time_in_ms,
	float4x4 const&		weapon_transform,
	float4x4 const*		weapon_matrices_begin,
	float4x4 const*		weapon_matrices_end,
	float4x4 const&		user_transform,
	float4x4*			user_matrices_begin,
	float4x4*			user_matrices_end,
	float4x4 const&		user_weapon_transform
)
{
	// FUNCTION BODY
	// <0x5a2e8b>|0x00b|+0x05f:'1047'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::process_finger_correction(const unsigned int, vostok::math::float4x4* const)
void weapon_core::process_finger_correction( u32 current_time_in_ms, float4x4* user_matrices )
{
	// FUNCTION BODY
	// <0x5a2e49>|0x009|+0x023:'1055'
	// ******
}

// STATE[BLOCKED]: udp_match_packet/packet_reader cluster is never-compiled (see game_core/README.md) - body is matchable from asm but cannot compile/diff until that header cluster is built.
// void survarium::weapon_core::serialize(vostok::network_core::udp_match_packet&, unsigned int) const
void weapon_core::serialize( network_core::udp_match_packet& packet, u32 client_offset ) const
{
	// LOCALS
	// u8 							state_id<1>
	// bool 						found<1>
	// ai::fsm_state const* 		current<1>
	// ai::fsm_state const* 		i<2>
	// ******

	// CALL SITE INFO
	// <0x5a39d0> -> void <unknown>(network_core::udp_match_packet&) const
	// ******

	// FUNCTION BODY
	// <0x5a3819>|0x009|+0x010:'1060'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5a3829>|0x019|+0x018:'1066'
	// <0x5a3841>|0x031|+0x018:'1067'
	// <0>
	// <0x5a3859>|0x049|+0x013:'1069'
	// <0>
	// <0x5a386c>|0x05c|+0x012:'1071'
	// <0>
	// <0x5a387e>|0x06e|+0x013:'1073'
	// <0x5a3891>|0x081|+0x013:'1074'
	// <0x5a38a4>|0x094|+0x013:'1075'
	// <0x5a38b7>|0x0a7|+0x013:'1076'
	// <0>
	// <0x5a38ca>|0x0ba|+0x00e:'1078'
	// <0x5a38d8>|0x0c8|+0x013:'1079'
	// <0>
	// <0x5a38eb>|0x0db|+0x023:'1081'
	// <0>
	// <0x5a390e>|0x0fe|+0x013|[1]:'1083'
	// <0>
	// <1>
	// <0x5a3921>|0x111|+0x016:'1086'
	// <0>
	// <0x5a3937>|0x127|+0x004:'1088'
	// <0x5a393b>|0x12b|+0x004:'1089'
	// <0x5a393f>|0x12f|+0x00f:'1090'
	// <0x5a394e>|0x13e|+0x02f|[2]:'1091'
	// <0x5a397d>|0x16d|+0x008:'1092'
	// <0x5a3985>|0x175|+0x004:'1093'
	// <0x5a3989>|0x179|+0x002:'1094'
	// <0>
	// <0x5a398b>|0x17b|+0x002:'1096'
	// <0>
	// <0x5a398d>|0x17d|+0x00c:'1098'
	// <0>
	// <0x5a3999>|0x189|+0x00d:'1100'
	// <0x5a39a6>|0x196|+0x02c:'1101'
	// <0>
	// <0x5a39d2>|0x1c2|+0x012:'1103'
	// <0>
	// ******
}

// STATE[BLOCKED]: udp_match_packet/packet_reader cluster is never-compiled (see game_core/README.md) - body is matchable from asm but cannot compile/diff until that header cluster is built.
// void survarium::weapon_core::deserialize(vostok::network_core::packet_reader&)
void weapon_core::deserialize( network_core::packet_reader& reader )
{
	// LOCALS
	// u8 							target_state_id<1>
	// u8 							state_id<1>
	// ai::fsm_state* 				current<1>
	// ai::fsm_state* 				i<2>
	// ******

	// CALL SITE INFO
	// <0x5a46f3> -> void <unknown>(network_core::packet_reader&)
	// ******

	// FUNCTION BODY
	// <0x5a445b>|0x00b|+0x00c:'1109'
	// <0x5a4467>|0x017|+0x00a:'1110'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5a4471>|0x021|+0x01a:'1116'
	// <0x5a448b>|0x03b|+0x023:'1117'
	// <0x5a44ae>|0x05e|+0x014:'1118'
	// <0>
	// <0x5a44c2>|0x072|+0x011:'1120'
	// <0>
	// <0x5a44d3>|0x083|+0x012:'1122'
	// <0x5a44e5>|0x095|+0x012:'1123'
	// <0x5a44f7>|0x0a7|+0x011:'1124'
	// <0x5a4508>|0x0b8|+0x014:'1125'
	// <0>
	// <0x5a451c>|0x0cc|+0x010:'1127'
	// <0x5a452c>|0x0dc|+0x082:'1128'
	// <0x5a45ae>|0x15e|+0x002:'1129'
	// <0x5a45b0>|0x160|+0x03b:'1130'
	// <0>
	// <0x5a45eb>|0x19b|+0x00e:'1132'
	// <0x5a45f9>|0x1a9|+0x011:'1133'
	// <0>
	// <0x5a460a>|0x1ba|+0x023:'1135'
	// <0>
	// <0x5a462d>|0x1dd|+0x011|[1]:'1137'
	// <0>
	// <1>
	// <0x5a463e>|0x1ee|+0x012:'1140'
	// <0>
	// <0x5a4650>|0x200|+0x00b:'1142'
	// <0x5a465b>|0x20b|+0x004:'1143'
	// <0x5a465f>|0x20f|+0x007:'1144'
	// <0x5a4666>|0x216|+0x02f|[2]:'1145'
	// <0x5a4695>|0x245|+0x00c:'1146'
	// <0x5a46a1>|0x251|+0x006:'1147'
	// <0x5a46a7>|0x257|+0x002:'1148'
	// <0>
	// <0x5a46a9>|0x259|+0x002:'1150'
	// <0>
	// <0x5a46ab>|0x25b|+0x00c:'1152'
	// <0x5a46b7>|0x267|+0x012:'1153'
	// <0x5a46c9>|0x279|+0x02c:'1154'
	// <0>
	// <0x5a46f5>|0x2a5|+0x012:'1156'
	// <0>
	// <0x5a4707>|0x2b7|+0x00e:'1158'
	// <0>
	// <1>
	// <0x5a4715>|0x2c5|+0x00a:'1161'
	// ******
}

// STATE[100%|DONE]
bool weapon_core::instant_idle_predicate( ) const
{
	return m_user_animations_selector.sprint_predicate( ) || m_user_animations_selector.is_in_jump( );
}

// STATE[81.19%|PARTIAL]: control-flow + member reads (33A/33B/48A) byte-identical; sole
// residual = smart-ptr operator* shape on (*user.damage_model()) - target calls operator*
// out-of-line on the returned resource_ptr const& (no copy), our /GL copies the resource_ptr
// (extra `mov byte[ebp-2],0; lea; call <copy>`) and inlines operator* (`mov eax,[edx]`).
// LTCG inline/copy decision at the operator* call boundary. See weapon_core_could_be_predicates.md
bool weapon_core::could_be_used( base_player const& user ) const
{
	u8 broken_hands_count = ( *user.damage_model( ) ).broken_hands_count( );
	return broken_hands_count != 2 || !m_is_double_handed;
}

// STATE[75.88%|PARTIAL]: return-logic byte-identical; same operator* copy/inline residual as
// could_be_used (see weapon_core_could_be_predicates.md).
bool weapon_core::could_be_aimed( base_player const& user ) const
{
	u8 broken_hands_count = ( *user.damage_model( ) ).broken_hands_count( );
	return broken_hands_count != 2;
}

// STATE[STUB]
// float survarium::weapon_core::computed_backward_recoil_time(const float, const float, const unsigned int, const unsigned int, const unsigned int, const float)
float weapon_core::computed_backward_recoil_time(
	float		animation_length,
	float		animation_time_before_time_scale_starts,
	u32			time_scale_start_time_in_ms,
	u32			current_time_in_ms,
	u32			target_time_in_ms,
	float		time_scale
)
{
	// CALL SITE INFO
	// <0x5a3e83> -> player_input const& <unknown>() const
	// ******

	return 0.0f;

	// FUNCTION BODY
	// <0x5a3e29>|0x009|+0x030:'1203'
	// <0x5a3e59>|0x039|+0x013:'1204'
	// <0x5a3e6c>|0x04c|+0x060:'1205'
	// <0x5a3ecc>|0x0ac|+0x047:'1206'
	// ******
}

// STATE[STUB]
// float survarium::weapon_core::computed_horizontal_recoil_time(const float, const float, const unsigned int, const unsigned int, const unsigned int, const float)
float weapon_core::computed_horizontal_recoil_time(
	float		animation_length,
	float		animation_time_before_time_scale_starts,
	u32			time_scale_start_time_in_ms,
	u32			current_time_in_ms,
	u32			target_time_in_ms,
	float		time_scale
)
{
	// CALL SITE INFO
	// <0x5a3dc3> -> player_input const& <unknown>() const
	// ******

	return 0.0f;

	// FUNCTION BODY
	// <0x5a3d69>|0x009|+0x030:'1218'
	// <0x5a3d99>|0x039|+0x013:'1219'
	// <0x5a3dac>|0x04c|+0x060:'1220'
	// <0x5a3e0c>|0x0ac|+0x00b:'1221'
	// ******
}

// STATE[STUB]
// float survarium::weapon_core::computed_vertical_recoil_time(const float, const float, const unsigned int, const unsigned int, const unsigned int, const float)
float weapon_core::computed_vertical_recoil_time(
	float		animation_length,
	float		animation_time_before_time_scale_starts,
	u32			time_scale_start_time_in_ms,
	u32			current_time_in_ms,
	u32			target_time_in_ms,
	float		time_scale
)
{
	// CALL SITE INFO
	// <0x5a3d03> -> player_input const& <unknown>() const
	// ******

	return 0.0f;

	// FUNCTION BODY
	// <0x5a3ca9>|0x009|+0x030:'1233'
	// <0x5a3cd9>|0x039|+0x013:'1234'
	// <0x5a3cec>|0x04c|+0x060:'1235'
	// <0x5a3d4c>|0x0ac|+0x00b:'1236'
	// ******
}

// STATE[100%|DONE]
weapon_core::calculator_functor weapon_core::backward_recoil_time_calculator( )
{
	return calculator_functor( this, &weapon_core::computed_backward_recoil_time );
}

// STATE[100%|DONE]
weapon_core::calculator_functor weapon_core::horizontal_recoil_time_calculator( )
{
	return calculator_functor( this, &weapon_core::computed_horizontal_recoil_time );
}

// STATE[100%|DONE]
weapon_core::calculator_functor weapon_core::vertical_recoil_time_calculator( )
{
	return calculator_functor( this, &weapon_core::computed_vertical_recoil_time );
}

// STATE[UNCHECKED]
void weapon_core::set_inventory( inventory* inv, profile_slot_enum slot )
{
	inventory_item::set_inventory( inv, slot );

	// FUNCTION BODY
	// <0x5a2e27>|0x007|+0x010:'1256'
	// ******
}

// STATE[STUB]
// survarium::profile_slot_enum survarium::weapon_core::get_ammo_slot(survarium::ammo_id_enum)
profile_slot_enum weapon_core::get_ammo_slot( ammo_id_enum slot_id )
{
	// FUNCTION BODY
	// <0x5a2dd9>|0x009|+0x019:'1261'
	// <0>
	// <1>
	// <0x5a2df2>|0x022|+0x00c:'1264'
	// <0>
	// <0x5a2dfe>|0x02e|+0x00c:'1266'
	// <0>
	// <0x5a2e0a>|0x03a|+0x005:'1268'
	// <0>
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::get_ammo_info(survarium::weapon_ammo_info&)
void weapon_core::get_ammo_info( weapon_ammo_info& info )
{
	// LOCALS
	// inventory& 					inv
	// resources::resource_ptr<inventory_item,resources::unmanaged_intrusive_base> slot2_itm
	// resources::resource_ptr<inventory_item,resources::unmanaged_intrusive_base> slot1_itm
	// ******

	// FUNCTION BODY
	// <0x5a432a>|0x00a|+0x024:'1274'
	// <0x5a434e>|0x02e|+0x00f:'1275'
	// <0x5a435d>|0x03d|+0x011:'1276'
	// <0>
	// <0x5a436e>|0x04e|+0x00c:'1278'
	// <0x5a437a>|0x05a|+0x01e:'1279'
	// <0x5a4398>|0x078|+0x01e:'1280'
	// <0>
	// <0x5a43b6>|0x096|+0x035:'1282'
	// <0x5a43eb>|0x0cb|+0x036:'1283'
	// <0>
	// <0x5a4421>|0x101|+0x00f:'1285'
	// ******
}

// STATE[100%|DONE]
bool weapon_core::must_chamber_a_round_predicate( ) const
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_is_round_chambered ) );
	return !m_is_round_chambered && m_ammo_in_magazine != 0 && !m_user_animations_selector.is_in_jump( );
}

// STATE[100%|DONE]
bool weapon_core::must_chamber_a_round_aimed_predicate( ) const
{
	return must_chamber_a_round_predicate( ) && is_trying_to_aim( );
}

// STATE[87.47%|PARTIAL]: ASSERT + control-flow byte-identical; residual = LTCG inline-vs-call
// of trivial weapon_core_base_state::has_animation_ended() (header inline; target /Od emits
// `call has_animation_ended` @0x42 + 3 current_base_state ref copies, our /GL inlines the
// `mov al,[+135h]` read). Same class as is_trying_to_aim / round_is_chambered.
bool weapon_core::must_chamber_a_round_and_animation_ended_predicate( ) const
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_is_round_chambered ) );
	return must_chamber_a_round_predicate( ) && current_base_state( ).has_animation_ended( );

	// FUNCTION BODY
	// <0x5a3549>|0x009|+0x00c:'1301'
	// <0x5a3555>|0x015|+0x04c:'1302'
	// ******
}

// STATE[100%|DONE]
bool weapon_core::must_chamber_a_round_aimed_and_animation_ended_predicate( ) const
{
	return must_chamber_a_round_and_animation_ended_predicate( ) && is_trying_to_aim( );
}

// STATE[100%|DONE]
bool weapon_core::is_ready_to_shoot( ) const
{
	return ( m_is_there_chamber_a_round_state ? m_is_round_chambered : m_ammo_in_magazine > 0 ) && m_bullets_in_queue != 0 && m_ready_for_fire;
}

// STATE[66.75%|PARTIAL]: every member access/mask/order byte-identical; sole diff = LTCG
// inline-vs-call of trivial player_input::is_sprinting() - target keeps it standalone
// (`call player_input::is_sprinting` @0x60), our /GL inlines its (&0x200 && &0x1 && &0x16E==0)
// body. Same inline-decision class as reload_state_base::initialize round_is_chambered.
bool weapon_core::is_trying_to_aim( ) const
{
	player_input const&	input			= m_user->input( );
	u32					just_toggled	= ~m_old_actions_mask & input.actions_mask;

	return could_be_aimed( *get_user( ) )
		&& ( input.actions_mask & 0x80 ) != 0	// sushi@TODO: aim action bit
		&& !( input.is_sprinting( ) && ( just_toggled & 0x200 ) != 0 )	// sushi@TODO: sprint action bit
		&& m_user_animations_selector.get_current_state_id( ) != type_jump;

	// FUNCTION BODY
	// <0x5a3499>|0x009|+0x01e:'1317'
	// <0x5a34b7>|0x027|+0x014:'1318'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5a34cb>|0x03b|+0x065:'1323'
	// ******
}

// STATE[100%|DONE]
bool weapon_core::is_not_trying_to_aim_predicate( ) const
{
	return !is_trying_to_aim( );
}

// STATE[85.32%|PARTIAL]: members/branches/&&-shape byte-identical; sole residual = LTCG
// inline-vs-call of the trivial ready_to_reload() stub - target /Od `call ready_to_reload`
// into a temp (frame 0x30), our /GL inlines its `return true` to `mov eax,1` (frame 0x08).
// Same inline-decision class as must_chamber_a_round_and_animation_ended_predicate.
bool weapon_core::can_and_must_reload_predicate( ) const
{
	return ready_to_reload( ) && m_ammo_in_magazine == 0 && !m_is_round_chambered;

	// FUNCTION BODY
	// <0x5a4df0>|0x010|+0x03e:'1333'
	// ******
}

// STATE[86.17%|PARTIAL]: control-flow byte-identical; residual = same ready_to_reload inline
// (via can_and_must_reload_predicate) + has_animation_ended inline class.
bool weapon_core::can_and_must_reload_and_animation_ended_predicate( ) const
{
	return current_base_state( ).has_animation_ended( ) && can_and_must_reload_predicate( );

	// FUNCTION BODY
	// <0>
	// <1>
	// <0x5a5ad9>|0x009|+0x04c:'1340'
	// ******
}

// STATE[100%|DONE]
void weapon_core::unload_chambered_round( )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_is_round_chambered ) );
	ASSERT( UNKNOWN_EXPRESSION_T( m_is_round_chambered ) );
	ASSERT( UNKNOWN_EXPRESSION_T( m_is_round_chambered ) );

	++m_ammo_in_magazine;
	m_is_round_chambered = false;
	on_unload_chambered_round( );
}

// STATE[88.41%|PARTIAL]: members/branches byte-identical; residual = bool->int codegen: target
// stores the && into a BYTE temp and normalizes it (neg;sbb;neg, the (val!=0) shape) before
// `add eax,ecx`, our base stores a DWORD temp and adds directly (no normalize). Source-shape
// residual (NOT LTCG/arg-passing), not yet diffed to a clean cause.
u16 weapon_core::maximum_ammo_in_weapon( ) const
{
	bool chamber_a_round_but_not_on_reload = m_is_there_chamber_a_round_state && !m_chamber_a_round_on_reload;
	return m_magazine_capacity + chamber_a_round_but_not_on_reload;

	// FUNCTION BODY
	// <0x5a2a79>|0x009|+0x02c:'1357'
	// <0x5a2aa5>|0x035|+0x016:'1358'
	// ******
}

// STATE[96.55%|PARTIAL]: ASSERTs/activate_hand args/return byte-identical; sole diff = `==9`
// comparison codegen - target `cmp eax,9; sete cl`, our base emits an extra `xor ecx,ecx`
// before the cmp. Not the LTCG inline class; an open source-shape residual, not yet diffed
// to a clean cause (register-zeroing is usually source-steerable).
animation::callback_return_type_enum weapon_core::on_hand_ik_event( animation::animation_callback_params& params, hand_to_weapon_ik_processor::hands_enum hand )
{
	ASSERT( UNKNOWN_EXPRESSION_T( params.domain_data ) );
	ASSERT( UNKNOWN_EXPRESSION_T( params.callback_time_in_ms ) );

	bool active = params.domain_data == 9;
	m_hand_ik_processor.activate_hand( hand, active, params.callback_time_in_ms );
	return animation::callback_return_type_call_me_again;

	// FUNCTION BODY
	// <0x5a2d09>|0x009|+0x00c:'1363'
	// <0x5a2d15>|0x015|+0x00c:'1364'
	// <0x5a2d21>|0x021|+0x010:'1365'
	// <0x5a2d31>|0x031|+0x01e:'1366'
	// <0x5a2d4f>|0x04f|+0x002:'1367'
	// ******
}

// STATE[89.72%|PARTIAL]: control-flow/args byte-identical; residual = LTCG inline-vs-call of
// trivial weapon_core::is_double_handed() (header inline; target /Od `call is_double_handed`,
// our /GL inlines the `mov cl,[+48Ah]` read). Same inline-decision class as is_trying_to_aim.
void weapon_core::on_user_sprint( bool user_is_sprinting )
{
	bool left_hand_ik_is_active = is_double_handed( ) || !user_is_sprinting;
	m_hand_ik_processor.activate_hand( hand_to_weapon_ik_processor::left, left_hand_ik_is_active, m_last_tick_time_in_ms );

	// FUNCTION BODY
	// <0x5a2ca9>|0x009|+0x027:'1372'
	// <0x5a2cd0>|0x030|+0x01f:'1373'
	// ******
}

// STATE[100%|DONE]
void weapon_core::instant_idle_start( )
{
	m_is_idle = true;
	if ( !( m_user->input( ).actions_mask & 0x20 ) )
		reset_fire_queue( );
}

// STATE[100%|DONE]
void weapon_core::instant_idle_end( )
{
	m_is_idle = false;
}

// STATE[100%|DONE]
void weapon_core::instant_toggle_start( )
{
	m_is_toggling = true;
}

// STATE[100%|DONE]
void weapon_core::instant_toggle_end( )
{
	m_is_toggling = false;
}

} // namespace survarium
