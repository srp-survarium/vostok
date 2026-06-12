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
#include <vostok/game_core/weapon_ammo_info.h>

namespace survarium {

static float const c_anim_center = 0.5f;
static float const epsilon       = 0.001f;

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
}

// STATE[DONE]: STRUCTURE MATCH (0/0 stmts). Empty body - all init in member-init list. Residual SIZE 0x2b1 vs 0xee is base-ctor call + member-init expansion, non-steerable.
weapon_core::~weapon_core( )
{
	ai::fsm_state* state;
	vostok::memory::delete_array_helper( *survarium::g_allocator, m_weapon_fire_queue_types );
	m_logic->clear_transitions( );
	while ( (state = m_logic->pop_state( )) )		;
	vostok::memory::delete_helper( *survarium::g_allocator, m_logic ); m_logic_states.clear( );
}

// STATE[100%|DONE]
// claude@NOTE: out-of-line so the idle-state getters emit `call ammo_in_magazine`
// instead of inlining `m_ammo_in_magazine` (matches the target's out-of-line call).
u16 weapon_core::ammo_in_magazine( ) const
{
	return m_ammo_in_magazine;
}

// STATE[100%|DONE]
// claude@NOTE: out-of-line so reload_state_base::initialize emits `call round_is_chambered`
// instead of inlining `m_is_round_chambered` (matches the target's out-of-line call @0x09b360).
bool weapon_core::round_is_chambered( ) const
{
	return m_is_round_chambered;
}

// STATE[100%|DONE]
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

// STATE[99.83%|DONE]
// STRUCTURE MATCH (122/122 stmts) — residual 2-byte diff is boost::bind/function transition-temp
// register rename allocator nondeterminism, non-steerable.
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

	// STRUCTURE DIFF: target 122 stmts / base 122 stmts (clean)
	// VERDICT: STRUCTURE MATCH - residual is the boost::function transition-temp register rename
	// (esi vs ecx) cascading over the add_transition calls; allocator nondeterminism, non-steerable.
}

// STATE[100%|DONE]
void weapon_core::set_skeleton( resources::resource_ptr<animation::skeleton,resources::unmanaged_intrusive_base> const& skeleton )
{
	m_skeleton = skeleton;
}

// STATE[DONE]: STRUCTURE MATCH 1/1 stmts. SIZE -0x7 is current_base_state/has_animation_ended LTCG temp-chain depth, non-steerable.
bool weapon_core::target_and_animation_ended_predicate( weapon_targets target ) const
{
	return m_target == target && current_base_state( ).has_animation_ended( );

	// STRUCTURE DIFF: target 1 stmt / base 1 stmt
	// SIZE -0x7 | 309 | return m_target == target && current_base_state( ).has_animation_ended( );
	// VERDICT: STRUCTURE MATCH - has_animation_ended out-of-line in target vs our inline; documented non-steerable wall.
}

// STATE[DONE]: STRUCTURE MATCH 1/1 stmts. SIZE -0x6 LTCG temp-chain depth, non-steerable.
animation::mixing::expression weapon_core::get_weapon_and_hands_animation_expression(
	mutable_buffer&						buffer,
	bool								is_third_view,
	weapon_user_state_enum				weapon_user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	return current_base_state( ).weapon_and_hands_expression( buffer, is_third_view, weapon_user_state_id, weight_driving_animation );
}

// STATE[DONE]: STRUCTURE MATCH 1/1 stmts - faithful one-liner (target has no locals). SIZE -0xc LTCG temp-chain depth, non-steerable.
animation::body_part_masks_enum weapon_core::get_body_part_mask_for_user( ) const
{
	return current_base_state( ).get_body_part_mask_for_user( );

	// STRUCTURE DIFF: target 1 stmt / base 1 stmt
	// SIZE -0xc | 340 | return current_base_state( ).get_body_part_mask_for_user( );
	// VERDICT: STRUCTURE MATCH - quantity fixed (was 2 stmts); SIZE residual is /Od ref-temp depth, not source-steerable.
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
// STATE[DONE]: STRUCTURE MATCH 3/3 stmts. SIZE +0x5 LTCG is_aimed inline direction, non-steerable.
float weapon_core::horizontal_recoil_value( ) const
{
	float const total_horizontal_coeff = is_aimed( ) ?
		m_breath_vibration_calculator.get_horizontal_value( ) + m_recoil_calculator.get_horizontal_coeff( ) :
		m_recoil_calculator.get_horizontal_coeff( );
	float const result = c_anim_center - math::clamp_r( total_horizontal_coeff, -c_anim_center + epsilon, c_anim_center - epsilon );
	return result;
}

// STATE[DONE]: STRUCTURE MATCH 3/3 stmts. SIZE +0x5 LTCG is_aimed inline direction, non-steerable.
float weapon_core::vertical_recoil_value( ) const
{
	float const total_vertical_coeff = is_aimed( ) ?
		m_breath_vibration_calculator.get_vertical_value( ) + m_recoil_calculator.get_vertical_coeff( ) :
		m_recoil_calculator.get_vertical_coeff( );
	float const result = math::clamp_r( total_vertical_coeff, -c_anim_center + epsilon, c_anim_center - epsilon ) + c_anim_center;
	return result;
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

// STATE[DONE]: STRUCTURE MATCH 1/1 stmts. SIZE +0x5 LTCG is_aimed inline-vs-call difference, non-steerable (out-lining is_aimed would FPO-break 26 other callers).
void weapon_core::update_recoil( u32 current_time_in_ms, float time_scale )
{
	m_recoil_calculator.tick( m_user_animations_selector.get_current_state_id( ), is_aimed( ), current_time_in_ms, time_scale );

	// STRUCTURE DIFF: target 1 stmt / base 1 stmt
	// SIZE +0x5 | 442 | m_recoil_calculator.tick( ... is_aimed( ) ... );
	// VERDICT: STRUCTURE MATCH - is_aimed() call in target vs our inline read; out-lining it FPO-breaks
	// many other callers (net -26 exact), so the inline stays.
}

// STATE[DONE]: STRUCTURE MATCH 2/2 stmts. SIZE +0x14 LTCG operator*/is_aimed inline-vs-call, non-steerable.
void weapon_core::update_dispersion( bool is_moving, u32 current_time_in_ms )
{
	ASSERT( UNKNOWN_EXPRESSION );
	m_dispersion_calculator.tick(
		m_user_animations_selector.get_current_state_id( ),
		is_moving,
		is_aimed( ),
		( *m_user->damage_model( ) ).broken_hands_count( ),
		m_is_double_handed,
		current_time_in_ms
	);
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

// STATE[DONE]: STRUCTURE MATCH 6/6 stmts. Residual SIZE on intrusive_ptr ctor/ASSERT is LTCG inline-vs-call, non-steerable.
void weapon_core::load_magazine( )
{
	weapon_ammunition_ptr ammo( m_ammunition ); ASSERT( UNKNOWN_EXPRESSION_T( ammo ) ); u16 const amount = ammo->amount( );

	u16 load = amount < u16( m_magazine_capacity - m_ammo_in_magazine ) ? amount : u16( m_magazine_capacity - m_ammo_in_magazine );

	weapon_ammunition_ptr ammo2( m_ammunition ); ASSERT( UNKNOWN_EXPRESSION_T( ammo2 ) ); ammo2->set_amount( amount - load );

	m_ammo_in_magazine += load;

	if ( m_chamber_a_round_on_reload )
		chamber_a_round( );

	// STRUCTURE DIFF: target 6 / base 6 stmts
	// SIZE +0x45 | 0 | weapon_ammunition_ptr ammo( m_ammunition ); ASSERT( UNKNOWN_EXPRESSION_T( ammo ) ); u16 const amount = ammo->amount( );
	// SIZE +0xd  | +2 | u16 load = amount < u16( m_magazine_capacity - m_ammo_in_magazine ) ? amount : u16( m_magazine_capacity - m_ammo_in_magazine );
	// SIZE +0x5  | +4 | weapon_ammunition_ptr ammo2( m_ammunition ); ASSERT( UNKNOWN_EXPRESSION_T( ammo2 ) ); ammo2->set_amount( amount - load );
	// VERDICT: STRUCTURE MATCH - SIZE residuals are LTCG intrusive_ptr::set out-of-line call vs inline + ASSERT expansion, non-steerable.
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

// STATE[DONE]: STRUCTURE MATCH 8/8 stmts. Residual SIZE on intrusive_ptr ctor/ASSERT is LTCG inline-vs-call, non-steerable.
void weapon_core::unload_ammo( )
{
	if ( !weapon_ammunition_ptr( m_ammunition ) )
		return;

	u16 ammo_to_add = m_ammo_in_magazine;

	m_ammo_in_magazine = 0;

	if ( m_is_round_chambered )
	{
		++ammo_to_add;
		m_is_round_chambered = false;
	}

	weapon_ammunition_ptr ammo2( m_ammunition ); ASSERT( UNKNOWN_EXPRESSION_T( ammo2 ) ); ammo2->set_amount( ammo2->amount( ) + ammo_to_add );

	// STRUCTURE DIFF: target 8 / base 8 stmts
	// SIZE +0x41 | 0 | if ( !weapon_ammunition_ptr( m_ammunition ) )
	// SIZE -0x1d | +13 | weapon_ammunition_ptr ammo2( m_ammunition ); ASSERT( UNKNOWN_EXPRESSION_T( ammo2 ) ); ammo2->set_amount( ammo2->amount( ) + ammo_to_add );
	// VERDICT: STRUCTURE MATCH - SIZE residuals are LTCG intrusive_ptr::set out-of-line call vs inline + scope guard shape, non-steerable.
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

// STATE[DONE]: STRUCTURE MATCH 4/4 stmts. Residual SIZE on intrusive_ptr/ASSERT is LTCG inline-vs-call, non-steerable.
void weapon_core::reload_one_round( )
{
	bool has_ammo = false; if ( m_ammo_in_magazine != m_magazine_capacity ) { weapon_ammunition_ptr ammo( m_ammunition ); ASSERT( UNKNOWN_EXPRESSION_T( ammo ) ); has_ammo = ammo->amount( ) != 0; }

	if ( has_ammo ) ++m_ammo_in_magazine;

	weapon_ammunition_ptr ammo2( m_ammunition ); ASSERT( UNKNOWN_EXPRESSION_T( ammo2 ) ); ammo2->set_amount( ammo2->amount( ) - 1 );

	on_reload( );

	// STRUCTURE DIFF: target 4 / base 4 stmts
	// SIZE +0x27 | 0 | bool has_ammo = false; if ( m_ammo_in_magazine != m_magazine_capacity ) { weapon_ammunition_ptr ammo( m_ammunition ); ASSERT( UNKNOWN_EXPRESSION_T( ammo ) ); has_ammo = ammo->amount( ) != 0; }
	// SIZE +0x8  | +2 | if ( has_ammo ) ++m_ammo_in_magazine;
	// SIZE -0x1d | +4 | weapon_ammunition_ptr ammo2( m_ammunition ); ASSERT( UNKNOWN_EXPRESSION_T( ammo2 ) ); ammo2->set_amount( ammo2->amount( ) - 1 );
	// VERDICT: STRUCTURE MATCH - SIZE residuals are LTCG intrusive_ptr::set out-of-line call vs inline + scope guard shape, non-steerable.
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

// STATE[DONE]: STRUCTURE MATCH 5/5 stmts. Residual SIZE +0x1/-0x3 is LTCG inline-vs-call of is_aimed(), non-steerable.
animation::callback_return_type_enum weapon_core::on_sprint_animation_ended( animation::animation_callback_params& params )
{
	params.interrupt_animation_player_tick = true;

	base_player* const user = get_user( ); user->unsubscribe_animation_player( (animation::reserved_channel_ids_enum)3, this );

	user->unsubscribe_animation_player( (animation::reserved_channel_ids_enum)3, (pcvoid)( u32( is_aimed( ) ) + 1 ) );

	m_is_in_sprint_transition = false;
	return animation::callback_return_type_dont_call_me_anymore;
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

// STATE[99.65%|PARTIAL]: instructions byte-identical modulo `this` register at the promoted
// fire_queue_length call (eax vs ecx) + slot numbering. OPEN QUESTION: target's line table has
// the else branch as ONE statement (6 stmts, NO named local recorded) yet keeps the named-local
// byte shape ([ebp-2] store + [ebp-6] param spill); every local-free spelling tried merges the
// temps and LOSES 8 bytes (u16(...) cast -> 85.8%, explicit min<u16> -> out-of-line call 79.0%,
// by-value min template -> C2668 ambiguity engine-wide). Keeping the named local for the bytes.
void weapon_core::reset_fire_queue( )
{
	if ( fire_queue_length( ) == 0xff )
	{
		m_bullets_in_queue = m_ammo_in_magazine;
		if ( m_is_round_chambered )
			++m_bullets_in_queue;
	}
	else { u16 bullets_in_queue = m_ammo_in_magazine + ( m_is_round_chambered != 0 ); m_bullets_in_queue = math::min( fire_queue_length( ), bullets_in_queue ); }

	// STRUCTURE MATCH 6/6 stmts. Merged else-body onto one line to match target PDB (no local recorded for else branch). Residual SIZE from LTCG inline-vs-call of fire_queue_length/min, non-steerable.
}

// STATE[100%|DONE]
void weapon_core::set_next_fire_queue_type( )
{
	if ( m_fire_queue_type == m_weapon_fire_queue_types_count - 1 )
		m_fire_queue_type = 0;
	else
		++m_fire_queue_type;
}

// STATE[DONE]: STRUCTURE MATCH 13/13 stmts. Residual: SIZE +0xb/+0x15 from LTCG item_in_slot/static_cast_resource_ptr inline-vs-call differences, non-steerable.
void weapon_core::set_next_ammo_type( )
{
	profile_slot_enum next_slot = invalid_slot;

	if ( m_ammo_slot == get_ammo_slot( first_ammo ) )
		next_slot = get_ammo_slot( second_ammo );
	else if ( m_ammo_slot == get_ammo_slot( second_ammo ) )
		next_slot = get_ammo_slot( first_ammo );
	else
		return;

	if ( get_inventory( ).item_in_slot( next_slot ) == NULL )
		return;

	unload_ammo( );
	m_ammo_slot = next_slot;
	{ inventory_item_ptr const& slot_item = get_inventory( ).item_in_slot( m_ammo_slot ); m_ammunition = static_cast_resource_ptr< weapon_ammunition_ptr >( slot_item ); }
	m_target = weapon_target_reload;
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

// STATE[DONE]: STRUCTURE MATCH 11/11 stmts. Frame-size diff (push ecx vs sub esp,0Ch) is LTCG register alloc, non-steerable.
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

// STATE[DONE]: STRUCTURE MATCH 1/1 stmts. SIZE -0xc LTCG trivial-getter inline-vs-call, non-steerable.
bool weapon_core::is_ready_to_be_deactivated( ) const
{
	return current_base_state( ).is_ready_to_be_deactivated( ) && m_user_animations_selector.is_ready_to_be_deactivated( );

	// STRUCTURE DIFF: target 1 stmt / base 1 stmt
	// SIZE -0xc | 1205 | return current_base_state( ).is_ready_to_be_deactivated( ) && ...;
	// VERDICT: STRUCTURE MATCH - the two trivial getters out-of-line in target vs our inline; non-steerable LTCG class.
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

// STATE[100%|DONE]
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

// STATE[DONE]: STRUCTURE MATCH 1/1 stmts. SIZE +0x7 is ASSERT dead-code empty_stub vs vtable call, non-steerable.
void weapon_core::process_finger_correction( u32 current_time_in_ms, float4x4* const user_matrices )
{
	ASSERT( UNKNOWN_EXPRESSION ); if ( user_matrices != 0 ) { process_finger_correction( current_time_in_ms, user_matrices ); }
}

// STATE[DONE]: STRUCTURE MATCH 26/26 stmts. SIZE differences are network_core packet::append / fsm accessor inline-vs-call (LTCG), not steerable from this TU.
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

	// STRUCTURE DIFF: target 26 stmts / base 26 stmts
	// SIZE +0x8 | 1400/1401 | packet.append( m_random/m_normal_random.seed( ) ); (x2)
	// SIZE +0x7 | 1402-1407,1410,1414 | the u8/u16 appends (aligner shows BASE_ONLY/TRGT_ONLY drift over the same 6 stmts)
	// SIZE -0xa | 1412 | if ( m_logic->current_state( ) ) (target materializes the bool, setne+movzx)
	// SIZE +0x7 | append( state_id ); SIZE -0x6 | forwarded ->serialize( packet )
	// VERDICT: STRUCTURE MATCH - set/order identical; every row is packet<T>::append / fsm accessor
	// inline-vs-call (network_core/ai headers), not steerable from this TU.
}

// STATE[DONE]: STRUCTURE MATCH 33/33 stmts. SIZE differences are network_core packet::r<T> / inventory accessor inline-vs-call (LTCG), not steerable from this TU.
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

	// STRUCTURE DIFF: target 33 stmts / base 33 stmts
	// SIZE +0x12..+0x15 | 1461-1483 | every reader.r<T>( ) read (base inlines read+advance, target calls r<T>;
	//                                 aligner shows BASE_ONLY/TRGT_ONLY drift over the same stmts)
	// SIZE +0x52 | 1471 | m_ammunition = ...item_in_slot( m_ammo_slot ).c_ptr( ); (inline-vs-call mix)
	// SIZE -0xa | 1478 | if ( m_logic->current_state( ) ) (target materializes the bool)
	// SIZE -0x6 | 1499 | forwarded ->deserialize( reader )
	// VERDICT: STRUCTURE MATCH - set/order identical; every row is the network_core r<T>/inventory
	// accessor inline-vs-call class, not steerable from this TU.
}

// STATE[100%|DONE]
bool weapon_core::instant_idle_predicate( ) const
{
	return m_user_animations_selector.sprint_predicate( ) || m_user_animations_selector.is_in_jump( );
}

// STATE[DONE]: STRUCTURE MATCH 2/2 stmts. SIZE +0xf/+0x6 LTCG intrusive_ptr::operator* / is_double_handed inline-vs-call, non-steerable (shared header).
bool weapon_core::could_be_used( base_player const& user ) const
{
	u8 const broken_hands_count = ( *user.damage_model( ) ).broken_hands_count( );
	return !( broken_hands_count == 2 && is_double_handed( ) );

	// STRUCTURE DIFF: target 2 stmts / base 2 stmts
	// SIZE +0xf | 1533 | u8 broken_hands_count = ( *user.damage_model( ) ).broken_hands_count( );
	// SIZE +0x6 | 1534 | return !( broken_hands_count == 2 && is_double_handed( ) );
	// VERDICT: STRUCTURE MATCH - intrusive_ptr::operator* / is_double_handed inline-vs-call wall; non-steerable.
}

// STATE[DONE]: STRUCTURE MATCH 2/2 stmts. SIZE +0xf LTCG intrusive_ptr::operator* inline-vs-call, non-steerable (shared header).
bool weapon_core::could_be_aimed( base_player const& user ) const
{
	u8 const broken_hands_count = ( *user.damage_model( ) ).broken_hands_count( );
	return broken_hands_count != 2;

	// STRUCTURE DIFF: target 2 stmts / base 2 stmts
	// SIZE +0xf | 1545 | u8 broken_hands_count = ( *user.damage_model( ) ).broken_hands_count( );
	// VERDICT: STRUCTURE MATCH - same intrusive_ptr::operator* inline-vs-call wall as could_be_used; non-steerable.
}

// STATE[69.62%|DONE]
// STRUCTURE MATCH (4 stmts) - residual SIZE on ASSERT (-0x24, UNKNOWN_EXPRESSION_T empty_stub vs
// real expression), update_breath_vibration (+0xb, inline-vs-call), return clamp_r (-0x10, inline-vs-call).
// All non-steerable LTCG differences.
float weapon_core::computed_backward_recoil_time(
	float		animation_length,
	float		animation_time_before_time_scale_starts,
	u32			time_scale_start_time_in_ms,
	u32			current_time_in_ms,
	u32			target_time_in_ms,
	float		time_scale
)
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_user->input( ).is_sprinting( ) ) );
	update_recoil( target_time_in_ms, time_scale );
	update_breath_vibration( m_user->input( ).is_sprinting( ), target_time_in_ms, time_scale );
	return math::clamp_r( m_recoil_calculator.get_back_coeff( ), 0.0f, 1.0f ) * animation_length;

	// STRUCTURE DIFF: target 4 stmts / base 4 stmts
	// SIZE -0x24 | ASSERT (UNKNOWN_EXPRESSION_T vs real expression)
	// SIZE +0xb  | update_breath_vibration (inline-vs-call)
	// SIZE -0x10 | return clamp_r (inline-vs-call)
	// VERDICT: STRUCTURE MATCH - all SIZE differences are LTCG inline-vs-call; assert expression unknown.
}

// STATE[76.09%|DONE]
// STRUCTURE MATCH (4 stmts) - residual SIZE on ASSERT (-0x24, UNKNOWN_EXPRESSION_T empty_stub)
// and update_breath_vibration (+0xb, inline-vs-call). Non-steerable LTCG differences.
float weapon_core::computed_horizontal_recoil_time(
	float		animation_length,
	float		animation_time_before_time_scale_starts,
	u32			time_scale_start_time_in_ms,
	u32			current_time_in_ms,
	u32			target_time_in_ms,
	float		time_scale
)
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_user->input( ).is_sprinting( ) ) );
	update_recoil( target_time_in_ms, time_scale );
	update_breath_vibration( m_user->input( ).is_sprinting( ), target_time_in_ms, time_scale );
	return horizontal_recoil_value( ) * animation_length;

	// STRUCTURE DIFF: target 4 stmts / base 4 stmts
	// SIZE -0x24 | ASSERT (UNKNOWN_EXPRESSION_T vs real expression)
	// SIZE +0xb  | update_breath_vibration (inline-vs-call)
	// VERDICT: STRUCTURE MATCH - all SIZE differences are LTCG inline-vs-call; assert expression unknown.
}

// STATE[76.09%|DONE]
// STRUCTURE MATCH (4 stmts) - residual SIZE on ASSERT (-0x24, UNKNOWN_EXPRESSION_T empty_stub)
// and update_breath_vibration (+0xb, inline-vs-call). Non-steerable LTCG differences.
float weapon_core::computed_vertical_recoil_time(
	float		animation_length,
	float		animation_time_before_time_scale_starts,
	u32			time_scale_start_time_in_ms,
	u32			current_time_in_ms,
	u32			target_time_in_ms,
	float		time_scale
)
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_user->input( ).is_sprinting( ) ) );
	update_recoil( target_time_in_ms, time_scale );
	update_breath_vibration( m_user->input( ).is_sprinting( ), target_time_in_ms, time_scale );
	return vertical_recoil_value( ) * animation_length;

	// STRUCTURE DIFF: target 4 stmts / base 4 stmts
	// SIZE -0x24 | ASSERT (UNKNOWN_EXPRESSION_T vs real expression)
	// SIZE +0xb  | update_breath_vibration (inline-vs-call)
	// VERDICT: STRUCTURE MATCH - all SIZE differences are LTCG inline-vs-call; assert expression unknown.
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

// STATE[100%|DONE]
void weapon_core::set_inventory( inventory* inv, profile_slot_enum slot )
{
	inventory_item::set_inventory( inv, slot );
}

// STATE[99.77%|DONE]: byte-identical stream; sole residual = call-boundary register for the
// `this` of profile_slot_id() (target passes it in eax, ours in ecx) - LTCG custom calling
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

	// STRUCTURE DIFF: target 4 stmts / base 4 stmts (clean, 0x45 bytes both)
	// VERDICT: STRUCTURE MATCH - sole residual is the `this` register at the promoted profile_slot_id call (eax vs ecx); LTCG.
}

// STATE[DONE]: STRUCTURE MATCH 9/9 stmts. SIZE on intrusive_ptr ctor/operator-> is LTCG inline-vs-call, non-steerable (shared header).
void weapon_core::get_ammo_info( weapon_ammo_info& info )
{
	info.current_ammo_type		= ( ammo_slot( ) != get_ammo_slot( first_ammo ) ) + 1;
	info.fire_queue_size		= fire_queue_length( );
	info.ammo_in_magazine		= ammo_in_magazine( );

	inventory&									inv			= get_inventory( );
	resources::resource_ptr<inventory_item,resources::unmanaged_intrusive_base>	slot1_itm	= inv.item_in_slot( get_ammo_slot( first_ammo ) );
	resources::resource_ptr<inventory_item,resources::unmanaged_intrusive_base>	slot2_itm	= inv.item_in_slot( get_ammo_slot( second_ammo ) );

	info.ammo1_total			= slot1_itm ? slot1_itm->amount( ) : 0;
	info.ammo2_total			= slot2_itm ? slot2_itm->amount( ) : 0;

	info.round_is_chambered		= m_is_round_chambered;

	// STRUCTURE DIFF: target 9 stmts / base 9 stmts
	// SIZE +0x4d | slot1_itm = inv.item_in_slot(...) (intrusive_ptr ctor inline-vs-call, LTCG)
	// SIZE +0x4d | slot2_itm = inv.item_in_slot(...) (intrusive_ptr ctor inline-vs-call, LTCG)
	// SIZE +0x18 | info.ammo1_total ternary (intrusive_ptr operator-> inline-vs-call, LTCG)
	// SIZE +0x18 | info.ammo2_total ternary (intrusive_ptr operator-> inline-vs-call, LTCG)
	// VERDICT: STRUCTURE MATCH - all SIZE are LTCG inline-vs-call, non-steerable.
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

// STATE[DONE]: STRUCTURE MATCH 2/2 stmts. SIZE -0x7 LTCG has_animation_ended inline-vs-call, non-steerable (shared header).
bool weapon_core::must_chamber_a_round_and_animation_ended_predicate( ) const
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_is_round_chambered ) );
	return must_chamber_a_round_predicate( ) && current_base_state( ).has_animation_ended( );

	// STRUCTURE DIFF: target 2 stmts / base 2 stmts
	// SIZE -0x7 | 1721 | return must_chamber_a_round_predicate( ) && current_base_state( ).has_animation_ended( );
	// VERDICT: STRUCTURE MATCH - has_animation_ended out-of-line in target vs our inline; documented non-steerable wall.
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

// STATE[DONE]: STRUCTURE MATCH 3/3 stmts. SIZE +0x36 LTCG player_input::is_sprinting inline-vs-call, non-steerable (shared header).
bool weapon_core::is_trying_to_aim( ) const
{
	player_input const&	input			= m_user->input( );
	u32					just_toggled	= ~m_old_actions_mask & input.actions_mask;

	return could_be_aimed( *get_user( ) )
		&& ( input.actions_mask & 0x80 ) != 0	// sushi@TODO: aim action bit
		&& !( input.is_sprinting( ) && ( just_toggled & 0x200 ) != 0 )	// sushi@TODO: sprint action bit
		&& m_user_animations_selector.get_current_state_id( ) != type_jump;

	// STRUCTURE DIFF: target 3 stmts / base 3 stmts
	// SIZE +0x36 | 1753 | && m_user_animations_selector.get_current_state_id( ) != type_jump; (the return stmt)
	// VERDICT: STRUCTURE MATCH - player_input::is_sprinting standalone call in target vs our inline of its mask body; non-steerable LTCG.
}

// STATE[100%|DONE]
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

// STATE[DONE]: STRUCTURE MATCH 1/1 stmts. Residual frame-size diff 0x30 vs 0x08 is target's dead-dword slot with no PDB record, not source-pinnable.
bool weapon_core::can_and_must_reload_predicate( ) const
{
	return ready_to_reload( ) && m_ammo_in_magazine == 0 && !m_is_round_chambered;

	// STRUCTURE DIFF: target 1 stmt / base 1 stmt
	// SIZE -0x7 | 1785 | return ready_to_reload( ) && m_ammo_in_magazine == 0 && !m_is_round_chambered;
	// VERDICT: STRUCTURE MATCH - target reserves a 0x30 frame and zeroes a dead dword; not source-pinnable.
}

// STATE[DONE]: STRUCTURE MATCH 1/1 stmts. SIZE -0x7 LTCG has_animation_ended inline-vs-call, non-steerable (shared header).
bool weapon_core::can_and_must_reload_and_animation_ended_predicate( ) const
{
	return current_base_state( ).has_animation_ended( ) && can_and_must_reload_predicate( );

	// STRUCTURE DIFF: target 1 stmt / base 1 stmt
	// SIZE -0x7 | 1799 | return current_base_state( ).has_animation_ended( ) && can_and_must_reload_predicate( );
	// VERDICT: STRUCTURE MATCH - has_animation_ended out-of-line in target vs our inline; documented non-steerable wall.
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

// STATE[100%|DONE]: `bool const` local (per target PDB) flips the && temp to a byte store,
// and `!= 0` in the addend emits the neg;sbb;neg normalize - both target shapes.
u16 weapon_core::maximum_ammo_in_weapon( ) const
{
	bool const chamber_a_round_but_not_on_reload = m_is_there_chamber_a_round_state && !m_chamber_a_round_on_reload;
	return m_magazine_capacity + ( chamber_a_round_but_not_on_reload != 0 );
}

// STATE[100%|DONE]: `bool const active` (per target PDB) drops the xor-before-sete; a plain
// `bool` local zeroes the full register first.
animation::callback_return_type_enum weapon_core::on_hand_ik_event( animation::animation_callback_params& params, hand_to_weapon_ik_processor::hands_enum const hand )
{
	ASSERT( UNKNOWN_EXPRESSION_T( params.domain_data ) );
	ASSERT( UNKNOWN_EXPRESSION_T( params.callback_time_in_ms ) );

	bool const active = params.domain_data == 9;
	m_hand_ik_processor.activate_hand( hand, active, params.callback_time_in_ms );
	return animation::callback_return_type_call_me_again;
}

// STATE[DONE]: STRUCTURE MATCH 2/2 stmts. SIZE +0x5 LTCG is_double_handed inline-vs-call, non-steerable (shared header).
void weapon_core::on_user_sprint( bool user_is_sprinting )
{
	bool const left_hand_ik_is_active = is_double_handed( ) || !user_is_sprinting;
	m_hand_ik_processor.activate_hand( hand_to_weapon_ik_processor::left, left_hand_ik_is_active, m_last_tick_time_in_ms );

	// STRUCTURE DIFF: target 2 stmts / base 2 stmts
	// SIZE +0x5 | 1862 | bool const left_hand_ik_is_active = is_double_handed( ) || !user_is_sprinting;
	// VERDICT: STRUCTURE MATCH - is_double_handed call in target vs our inline read; same wall as is_trying_to_aim.
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

