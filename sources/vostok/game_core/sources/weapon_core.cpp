////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core.h>

#include <vostok/animation/sources/mixing_base_lexeme.h>

#include <vostok/ai/fsm.h>
#include <boost/bind.hpp>
#include <vostok/network_core/udp_match_packet.h>
#include <vostok/network_core/packet_reader.h>
#include <vostok/game_core/base_player.h>
#include <vostok/game_core/player_input.h>

namespace survarium {
/*
// STATE[STUB]
// void survarium::`dynamic initializer for 's_ik_use_cc''()
void `dynamic initializer for 's_ik_use_cc''( )
{
}

// STATE[STUB]
// void survarium::`dynamic initializer for 's_ik_use_on_legs_cc''()
void `dynamic initializer for 's_ik_use_on_legs_cc''( )
{
}

// STATE[STUB]
// void survarium::`dynamic initializer for 's_recoil_vertical_eanble_cc''()
void `dynamic initializer for 's_recoil_vertical_eanble_cc''( )
{
}

// STATE[STUB]
// void survarium::`dynamic initializer for 's_recoil_horizontal_eanble_cc''()
void `dynamic initializer for 's_recoil_horizontal_eanble_cc''( )
{
}

// STATE[STUB]
// void survarium::`dynamic initializer for 's_recoil_back_eanble_cc''()
void `dynamic initializer for 's_recoil_back_eanble_cc''( )
{
}

// STATE[STUB]
// void survarium::`dynamic initializer for 's_recoil_enable_cc''()
void `dynamic initializer for 's_recoil_enable_cc''( )
{
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
// claude@NOTE: out-of-line so reload_state_base::initialize emits `call round_is_chambered`
// instead of inlining `m_is_round_chambered` (matches the target's out-of-line call @0x09b360).
bool weapon_core::round_is_chambered( ) const
{
	return m_is_round_chambered;
}

// STATE[STUB]
// claude@NOTE: out-of-line so the double-barreled ctor's ASSERT_CMP_U emits
// `call get_magazine_capacity` instead of inlining `m_magazine_capacity`
// (matches the target's out-of-line call; symbol @0x09cc20).
u16 weapon_core::get_magazine_capacity( ) const
{
	return m_magazine_capacity;
}

void weapon_core::set_magazine_capacity( u16 magazine_capacity )
{
	ASSERT( UNKNOWN_EXPRESSION_T( magazine_capacity ) );
	m_magazine_capacity = magazine_capacity;
}

// claude@NOTE: out-of-line so reset_fire_queue emits `call fire_queue_length`
// instead of inlining the m_weapon_fire_queue_types[m_fire_queue_type] read
// (matches the target's standalone symbol @0x09b290).
u16 weapon_core::fire_queue_length( ) const
{
	return m_weapon_fire_queue_types[m_fire_queue_type];
}

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

	weapon_core_base_state* const inactive				= inactive_state.c_ptr( );
	weapon_core_base_state* const show					= show_state.c_ptr( );
	weapon_core_base_state* const hide					= hide_state.c_ptr( );
	weapon_core_base_state* const idle					= idle_state.c_ptr( );
	weapon_core_base_state* const reload				= reload_state.c_ptr( );
	weapon_core_base_state* const fire					= fire_state.c_ptr( );
	weapon_core_base_state* const aim					= aim_state.c_ptr( );
	weapon_core_base_state* const aim_fire				= aim_fire_state.c_ptr( );
	weapon_core_base_state* const chamber_a_round		= chamber_a_round_state.c_ptr( );
	weapon_core_base_state* const chamber_a_round_aimed	= chamber_a_round_aimed_state.c_ptr( );

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
	if ( m_is_there_chamber_a_round_state && !m_chamber_a_round_on_reload )
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

void weapon_core::set_skeleton( resources::resource_ptr<animation::skeleton,resources::unmanaged_intrusive_base> const& skeleton )
{
	m_skeleton = skeleton;
}

bool weapon_core::target_and_animation_ended_predicate( weapon_targets target ) const
{
	return m_target == target && current_base_state( ).has_animation_ended( );
}

animation::mixing::expression weapon_core::get_weapon_and_hands_animation_expression(
	mutable_buffer&						buffer,
	bool								is_third_view,
	weapon_user_state_enum				weapon_user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	return current_base_state( ).weapon_and_hands_expression( buffer, is_third_view, weapon_user_state_id, weight_driving_animation );
}

animation::body_part_masks_enum weapon_core::get_body_part_mask_for_user( ) const
{
	return current_base_state( ).get_body_part_mask_for_user( );
}
/*
// STATE[STUB]
// void survarium::`dynamic initializer for 'epsilon''()
void `dynamic initializer for 'epsilon''( )
{
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

void weapon_core::update_recoil( u32 current_time_in_ms, float time_scale )
{
	m_recoil_calculator.tick( m_user_animations_selector.get_current_state_id( ), is_aimed( ), current_time_in_ms, time_scale );
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

void weapon_core::instant_show( )
{
	m_aimed = false;
	on_show( );
}

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

void weapon_core::instant_reload( )
{
	m_aimed = false;
	load_magazine( );
	on_reload( );
	m_recoil_calculator.reload( );
	m_dispersion_calculator.reload( );
	reset_fire_queue( );
}

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

void weapon_core::instant_aim_start( )
{
	if ( !is_firing( ) && !( m_user->input( ).actions_mask & 0x20 ) )
		reset_fire_queue( );

	m_aimed = true;
	m_aiming_state_transition = true;
}

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

void weapon_core::reset_fire_queue( )
{
	if ( fire_queue_length( ) == 0xff )
	{
		m_bullets_in_queue = m_ammo_in_magazine;
		if ( m_is_round_chambered )
			++m_bullets_in_queue;
	}
	else
		m_bullets_in_queue = math::min( fire_queue_length( ), u16( m_ammo_in_magazine + ( m_is_round_chambered != 0 ) ) );
}

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

void weapon_core::on_reload_started( )
{
}

animation::callback_return_type_enum weapon_core::on_animation_ik_interval( animation::animation_callback_params& params )
{
	params.interrupt_animation_player_tick = false;

	if ( params.animated_object == m_user )
	{
		if ( strings::equal( params.channel_id, "Left heel" ) )
			m_legs_ik_processor.set_left_heel_on_ground( params.domain_data != 0xFF );
		else if ( strings::equal( params.channel_id, "Left toe" ) )
			m_legs_ik_processor.set_left_toe_on_ground( params.domain_data != 0xFF );
		else if ( strings::equal( params.channel_id, "Right heel" ) )
			m_legs_ik_processor.set_right_heel_on_ground( params.domain_data != 0xFF );
		else if ( strings::equal( params.channel_id, "Right toe" ) )
			m_legs_ik_processor.set_right_toe_on_ground( params.domain_data != 0xFF );
	}

	return animation::callback_return_type_call_me_again;
}

void weapon_core::set_animation_callback( pcstr channel_id, pcvoid callback_uid, boost::function<enum animation::callback_return_type_enum(animation::animation_callback_params &)> const& animation_callback )
{
	m_user->subscribe_animation_player( channel_id, animation_callback, callback_uid, resources::managed_resource_ptr( NULL ), 0xff, this );
}

void weapon_core::remove_animation_callback( pcstr channel_id, pcvoid callback_uid )
{
	m_user->unsubscribe_animation_player( channel_id, callback_uid );
}

void weapon_core::set_animation_callback( animation::reserved_channel_ids_enum channel_id, pcvoid callback_uid, boost::function<enum animation::callback_return_type_enum(animation::animation_callback_params &)> const& animation_callback )
{
	m_user->subscribe_animation_player( channel_id, animation_callback, callback_uid, resources::managed_resource_ptr( NULL ), this );
}

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

bool weapon_core::is_ready_to_be_deactivated( ) const
{
	return current_base_state( ).is_ready_to_be_deactivated( ) && m_user_animations_selector.is_ready_to_be_deactivated( );
}

void weapon_core::on_player_model_added( )
{
	instant_show( );
}

void weapon_core::on_player_model_removed( )
{
	instant_hide( );
}

// STATE[STUB]
// void survarium::weapon_core::update_bones_matrices(vostok::resources::resource_ptr<vostok::animation::skeleton,vostok::resources::unmanaged_intrusive_base> const&, vostok::math::float4x4* const, const unsigned int, const unsigned int, vostok::math::float4x4&, vostok::math::float4x4&, vostok::animation::animation_player const&)
void weapon_core::update_bones_matrices(
	resources::resource_ptr<animation::skeleton,resources::unmanaged_intrusive_base> const&	user_skeleton,
	float4x4* const						user_matrices,
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

bool weapon_core::is_sprinting( ) const
{
	return m_user_animations_selector.is_sprinting( );
}

// STATE[STUB]
// void survarium::weapon_core::on_skeleton_matrices_changed(const unsigned int, vostok::math::float4x4 const&, vostok::math::float4x4 const* const, vostok::math::float4x4 const* const, vostok::math::float4x4 const&, vostok::math::float4x4* const, vostok::math::float4x4* const, vostok::math::float4x4 const&)
void weapon_core::on_skeleton_matrices_changed(
	u32						current_time_in_ms,
	float4x4 const&			weapon_transform,
	float4x4 const* const	weapon_matrices_begin,
	float4x4 const* const	weapon_matrices_end,
	float4x4 const&			user_transform,
	float4x4* const			user_matrices_begin,
	float4x4* const			user_matrices_end,
	float4x4 const&			user_weapon_transform
)
{
	// FUNCTION BODY
	// <0x5a2e8b>|0x00b|+0x05f:'1047'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::process_finger_correction(const unsigned int, vostok::math::float4x4* const)
void weapon_core::process_finger_correction( u32 current_time_in_ms, float4x4* const user_matrices )
{
	// FUNCTION BODY
	// <0x5a2e49>|0x009|+0x023:'1055'
	// ******
}

void weapon_core::serialize( network_core::udp_match_packet& packet, u32 client_offset ) const
{
	inventory_item::serialize( packet, client_offset );

	packet.append( m_random.seed( ) );
	packet.append( m_normal_random.seed( ) );
	packet.append( (u8)m_target );
	packet.append( m_old_actions_mask );
	packet.append( m_ammo_in_magazine );
	packet.append( m_bullets_in_queue );
	packet.append( m_fire_queue_type );
	packet.append( (u8)m_ammo_slot );

	if ( m_is_there_chamber_a_round_state )
		packet.append( m_is_round_chambered );

	if ( m_logic->current_state( ) )
	{
		packet.append( m_is_shown );
		m_hand_ik_processor.serialize( packet, client_offset );

		u8							state_id	= 0;
		bool						found		= false;
		ai::fsm_state const* const	current		= m_logic->current_state( );

		for ( ai::fsm_state const* i = m_logic->states( ).front( ); i; i = i->next, ++state_id )
		{
			if ( i == current )
			{
				found	= true;
				break;
			}
		}

		ASSERT( UNKNOWN_EXPRESSION_T( found ) );

		packet.append( state_id );
		static_cast< weapon_core_base_state const* >( m_logic->current_state( ) )->serialize( packet );
		m_user_animations_selector.serialize( packet );
	}
}

void weapon_core::deserialize( network_core::packet_reader& reader )
{
	inventory_item::deserialize( reader );
	m_deserializing	= true;

	m_random.seed( reader.r< u32 >( ) );
	m_normal_random.set_seed( reader.r< s32 >( ) );
	m_target			= (weapon_targets)reader.r< bool >( );
	m_old_actions_mask	= reader.r< u32 >( );
	m_ammo_in_magazine	= reader.r< u16 >( );
	m_bullets_in_queue	= reader.r< u16 >( );
	m_fire_queue_type	= reader.r< bool >( );
	m_ammo_slot			= (profile_slot_enum)reader.r< bool >( );

	if ( m_ammo_slot != invalid_slot )
		m_ammunition	= static_cast< weapon_ammunition* >( get_inventory( ).item_in_slot( m_ammo_slot ).c_ptr( ) );
	else
		m_ammunition	= NULL;

	if ( m_is_there_chamber_a_round_state )
		m_is_round_chambered	= reader.r< bool >( );

	if ( m_logic->current_state( ) )
	{
		m_is_shown	= reader.r< bool >( );
		m_hand_ik_processor.deserialize( reader );

		u8 const			target_state_id	= reader.r< bool >( );
		u8					state_id		= 0;
		ai::fsm_state*		current			= NULL;

		for ( ai::fsm_state* i = m_logic->states( ).front( ); i; i = i->next, ++state_id )
		{
			if ( state_id == target_state_id )
			{
				current	= i;
				break;
			}
		}

		ASSERT( UNKNOWN_EXPRESSION_T( current ) );

		m_logic->set_initial_state( current );
		static_cast< weapon_core_base_state* >( m_logic->current_state( ) )->deserialize( reader );
		m_user_animations_selector.deserialize( reader );

		m_user->force_animation_selection( );
	}

	m_deserializing	= false;
}

bool weapon_core::instant_idle_predicate( ) const
{
	return m_user_animations_selector.sprint_predicate( ) || m_user_animations_selector.is_in_jump( );
}

bool weapon_core::could_be_used( base_player const& user ) const
{
	u8 const broken_hands_count = ( *user.damage_model( ) ).broken_hands_count( );
	return !( broken_hands_count == 2 && is_double_handed( ) );
}

bool weapon_core::could_be_aimed( base_player const& user ) const
{
	u8 const broken_hands_count = ( *user.damage_model( ) ).broken_hands_count( );
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

weapon_core::calculator_functor weapon_core::backward_recoil_time_calculator( )
{
	return calculator_functor( this, &weapon_core::computed_backward_recoil_time );
}

weapon_core::calculator_functor weapon_core::horizontal_recoil_time_calculator( )
{
	return calculator_functor( this, &weapon_core::computed_horizontal_recoil_time );
}

weapon_core::calculator_functor weapon_core::vertical_recoil_time_calculator( )
{
	return calculator_functor( this, &weapon_core::computed_vertical_recoil_time );
}

void weapon_core::set_inventory( inventory* inv, profile_slot_enum slot )
{
	inventory_item::set_inventory( inv, slot );
}

// convention on the callee, not source-steerable. claude@NOTE
profile_slot_enum weapon_core::get_ammo_slot( ammo_id_enum slot_id )
{
	static profile_slot_enum const weapon_ammo_slots[2][2] = {
		{ ammo1_weapon1_slot, ammo2_weapon1_slot },
		{ ammo1_weapon2_slot, ammo2_weapon2_slot },
	};

	switch ( profile_slot_id( ) )
	{
		case weapon1_slot:	return weapon_ammo_slots[0][slot_id];
		case weapon2_slot:	return weapon_ammo_slots[1][slot_id];
		default:			return invalid_slot;
	}
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

bool weapon_core::must_chamber_a_round_predicate( ) const
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_is_round_chambered ) );
	return !m_is_round_chambered && m_ammo_in_magazine != 0 && !m_user_animations_selector.is_in_jump( );
}

bool weapon_core::must_chamber_a_round_aimed_predicate( ) const
{
	return must_chamber_a_round_predicate( ) && is_trying_to_aim( );
}

bool weapon_core::must_chamber_a_round_and_animation_ended_predicate( ) const
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_is_round_chambered ) );
	return must_chamber_a_round_predicate( ) && current_base_state( ).has_animation_ended( );
}

bool weapon_core::must_chamber_a_round_aimed_and_animation_ended_predicate( ) const
{
	return must_chamber_a_round_and_animation_ended_predicate( ) && is_trying_to_aim( );
}

bool weapon_core::is_ready_to_shoot( ) const
{
	return ( m_is_there_chamber_a_round_state ? m_is_round_chambered : m_ammo_in_magazine > 0 ) && m_bullets_in_queue != 0 && m_ready_for_fire;
}

bool weapon_core::is_trying_to_aim( ) const
{
	player_input const&	input			= m_user->input( );
	u32					just_toggled	= ~m_old_actions_mask & input.actions_mask;

	return could_be_aimed( *get_user( ) )
		&& ( input.actions_mask & 0x80 ) != 0	// sushi@TODO: aim action bit
		&& !( input.is_sprinting( ) && ( just_toggled & 0x200 ) != 0 )	// sushi@TODO: sprint action bit
		&& m_user_animations_selector.get_current_state_id( ) != type_jump;
}

bool weapon_core::is_not_trying_to_aim_predicate( ) const
{
	return !is_trying_to_aim( );
}

// STATE[STUB]: out-of-line stub (target @0x0ac370 has the real "A LOT OF LOGIC" body, not matched
// here). Defined so callers like can_and_must_reload_predicate emit `call ready_to_reload`.
bool weapon_core::ready_to_reload( ) const
{
	return true;
}

// record and no read - not source-pinnable. claude@NOTE
bool weapon_core::can_and_must_reload_predicate( ) const
{
	return ready_to_reload( ) && m_ammo_in_magazine == 0 && !m_is_round_chambered;
}

// shared getter would regress other inlined call sites. claude@NOTE
bool weapon_core::can_and_must_reload_and_animation_ended_predicate( ) const
{
	return current_base_state( ).has_animation_ended( ) && can_and_must_reload_predicate( );
}

void weapon_core::unload_chambered_round( )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_is_round_chambered ) );
	ASSERT( UNKNOWN_EXPRESSION_T( m_is_round_chambered ) );
	ASSERT( UNKNOWN_EXPRESSION_T( m_is_round_chambered ) );

	++m_ammo_in_magazine;
	m_is_round_chambered = false;
	on_unload_chambered_round( );
}

u16 weapon_core::maximum_ammo_in_weapon( ) const
{
	bool const chamber_a_round_but_not_on_reload = m_is_there_chamber_a_round_state && !m_chamber_a_round_on_reload;
	return m_magazine_capacity + ( chamber_a_round_but_not_on_reload != 0 );
}

animation::callback_return_type_enum weapon_core::on_hand_ik_event( animation::animation_callback_params& params, hand_to_weapon_ik_processor::hands_enum const hand )
{
	ASSERT( UNKNOWN_EXPRESSION_T( params.domain_data ) );
	ASSERT( UNKNOWN_EXPRESSION_T( params.callback_time_in_ms ) );

	bool const active = params.domain_data == 9;
	m_hand_ik_processor.activate_hand( hand, active, params.callback_time_in_ms );
	return animation::callback_return_type_call_me_again;
}

void weapon_core::on_user_sprint( bool user_is_sprinting )
{
	bool const left_hand_ik_is_active = is_double_handed( ) || !user_is_sprinting;
	m_hand_ik_processor.activate_hand( hand_to_weapon_ik_processor::left, left_hand_ik_is_active, m_last_tick_time_in_ms );
}

void weapon_core::instant_idle_start( )
{
	m_is_idle = true;
	if ( !( m_user->input( ).actions_mask & 0x20 ) )
		reset_fire_queue( );
}

void weapon_core::instant_idle_end( )
{
	m_is_idle = false;
}

void weapon_core::instant_toggle_start( )
{
	m_is_toggling = true;
}

void weapon_core::instant_toggle_end( )
{
	m_is_toggling = false;
}

} // namespace survarium
