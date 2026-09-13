// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/weapon_core.h>

#include <vostok/animation/sources/mixing_base_lexeme.h>
#include <vostok/animation/mixing_addition_lexeme.h>

#include <vostok/ai/fsm.h>
#include <boost/bind.hpp>
#include <vostok/network_core/udp_match_packet.h>
#include <vostok/network_core/packet_reader.h>
#include <vostok/game_core/base_player.h>
#include <vostok/game_core/player_input.h>
#include <vostok/game_core/weapon_ammo_info.h>
#include <vostok/game_core/weapon_animation_parameters.h>
#include <vostok/game_core/bullet_manager.h>
#include <vostok/game_core/ik_utils.h>
#include <vostok/physics/api.h>
#include <vostok/physics/character_controller.h>
#include <vostok/console_command.h>

bool g_is_server = false;

namespace vostok { namespace animation {
math::float4x4 calculated_head_matrix( math::float4x4 const& head_matrix, math::float4x4 const& object_matrix );
} } // namespace vostok::animation

namespace survarium {

static bool s_ik_enable_on_hands_value		= true;
static console_commands::cc_bool s_ik_use_cc				( "ik_enable_on_hands", s_ik_enable_on_hands_value, true, console_commands::command_type_engine_internal );
static bool s_ik_enable_on_legs_value		= true;
static console_commands::cc_bool s_ik_use_on_legs_cc		( "ik_enable_on_legs", s_ik_enable_on_legs_value, true, console_commands::command_type_engine_internal );
static bool s_recoil_vertical_enable_value	= true;
static console_commands::cc_bool s_recoil_vertical_eanble_cc	( "recoil_vertical_enable", s_recoil_vertical_enable_value, false, console_commands::command_type_user_specific );
static bool s_recoil_horizontal_enable_value	= true;
static console_commands::cc_bool s_recoil_horizontal_eanble_cc	( "recoil_horizontal_enable", s_recoil_horizontal_enable_value, false, console_commands::command_type_user_specific );
static bool s_recoil_back_enable_value		= true;
static console_commands::cc_bool s_recoil_back_eanble_cc		( "recoil_back_enable", s_recoil_back_enable_value, false, console_commands::command_type_user_specific );
static bool s_recoil_enable_value			= true;
static console_commands::cc_bool s_recoil_enable_cc			( "recoil_enable", s_recoil_enable_value, true, console_commands::command_type_user_specific );

weapon_core::weapon_core( ) :
	inventory_item						( inventory_item::inventory_active_item ),
	m_ammunition						( NULL ),
	m_initiator_holder					( NULL ),
	m_receiver_holder					( NULL ),
	m_logic								( VOSTOK_NEW_IMPL( g_allocator, ai::fsm ) ),
	m_bullet_manager					( NULL ),
	m_user								( NULL ),
	m_weapon_fire_queue_types			( NULL ),
	m_bullet_damage						( 0.0f ),
	m_bullet_pierce						( 0.0f ),
	m_target							( weapon_target_idle ),
	m_old_actions_mask					( 0 ),
	m_magazine_capacity					( 0 ),
	m_ammo_in_magazine					( 0 ),
	m_bullets_in_queue					( 0 ),
	m_fire_queue_type					( 0 ),
	m_ammo_slot							( invalid_slot ),
	m_weapon_id							( 0 ),
	m_weapon_fire_queue_types_count		( 0 ),
	m_is_shown							( false ),
	m_aimed								( false ),
	m_ready_for_fire					( false ),
	m_is_double_handed					( true ),
	m_is_in_sprint_transition			( false ),
	m_is_firing							( false ),
	m_is_there_chamber_a_round_state	( false ),
	m_is_round_chambered				( false ),
	m_chamber_a_round_on_reload			( false ),
	m_load_ammo_on_next_activate		( true ),
	m_aiming_state_transition			( false ),
	m_is_idle							( false ),
	m_deserializing						( false ),
	m_is_toggling						( false )
{
}

weapon_core::~weapon_core( )
{
	VOSTOK_DELETE_ARRAY_IMPL( g_allocator, m_weapon_fire_queue_types );

	m_logic->clear_transitions( );
	while ( ai::fsm_state* state = m_logic->pop_state( ) ) { }
	VOSTOK_DELETE_IMPL( g_allocator, m_logic );
}

// The target source is inline, but the remaining real consumer set does not retain
// this COMDAT yet. Keep the call boundary until that consumer is reconstructed.
u16 weapon_core::ammo_in_magazine( ) const
{
	return m_ammo_in_magazine;
}

bool weapon_core::round_is_chambered( ) const
{
	return m_is_round_chambered;
}

void weapon_core::set_magazine_capacity( u16 magazine_capacity )
{
	ASSERT( UNKNOWN_EXPRESSION_T( magazine_capacity ) );
	m_magazine_capacity = magazine_capacity;
}

u16 weapon_core::fire_queue_length( ) const
{
	return m_weapon_fire_queue_types[m_fire_queue_type];
}

profile_slot_enum weapon_core::ammo_slot( )
{
	return m_ammo_slot;
}

bool weapon_core::is_aimed( ) const
{
	return m_aimed;
}

bool weapon_core::is_double_handed( ) const
{
	return m_is_double_handed;
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

#line 284
bool weapon_core::target_and_animation_ended_predicate( const weapon_targets target ) const
{
	return m_target == target && current_base_state( ).has_animation_ended( );
}

#line 284
animation::mixing::expression weapon_core::get_weapon_and_hands_animation_expression(
	mutable_buffer&						buffer,
	const bool								is_third_view,
	const weapon_user_state_enum				weapon_user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	return current_base_state( ).weapon_and_hands_expression( buffer, is_third_view, weapon_user_state_id, weight_driving_animation );
}

animation::body_part_masks_enum weapon_core::get_body_part_mask_for_user( ) const
{
	return current_base_state( ).get_body_part_mask_for_user( );
}

#line 304
static float const	c_anim_center	= 0.5f;
static float		epsilon			= math::epsilon_7;
static float const	clear_value		= 1.0f;
static float const	offset			= c_anim_center;

#line 310
float weapon_core::horizontal_recoil_value( ) const
{
	float const total_horizontal_coeff	= is_aimed( ) ? m_breath_vibration_calculator.get_horizontal_value( ) + m_recoil_calculator.get_horizontal_coeff( ) : m_recoil_calculator.get_horizontal_coeff( );
	float const result					= c_anim_center - math::clamp_r( total_horizontal_coeff, -c_anim_center + epsilon, c_anim_center - epsilon );
	return result;
}

#line 319
float weapon_core::vertical_recoil_value( ) const
{
	float const total_vertical_coeff	= is_aimed( ) ? m_breath_vibration_calculator.get_vertical_value( ) + m_recoil_calculator.get_vertical_coeff( ) : m_recoil_calculator.get_vertical_coeff( );

	float const result					= math::clamp_r( total_vertical_coeff, -c_anim_center + epsilon, c_anim_center - epsilon ) + c_anim_center;
	return result;
}

#line 330
animation::mixing::expression weapon_core::selected_animations( mutable_buffer& buffer, const bool is_third_view ) const
{
	m_is_third_view = is_third_view;

	std::pair<animation::mixing::expression,animation::mixing::animation_lexeme> result = m_user_animations_selector.selected_animations(
		buffer,
		weapon_animation_parameters(
			s_recoil_enable_value && s_recoil_horizontal_enable_value	? horizontal_recoil_value( )	: offset,
			s_recoil_enable_value && s_recoil_vertical_enable_value		? vertical_recoil_value( )		: offset,
			s_recoil_enable_value && s_recoil_back_enable_value			? math::clamp_r( m_recoil_calculator.get_back_coeff( ), epsilon, clear_value - epsilon ) : offset,
			get_body_part_mask_for_user( ),
			is_aimed( ),
			m_is_firing
		),
		is_third_view
#line 346
	);

	animation::mixing::expression const& expression_for_weapon_and_hands = get_weapon_and_hands_animation_expression( buffer, is_third_view, m_user_animations_selector.get_current_state_id( ), result.second );
	return result.first + expression_for_weapon_and_hands;
}

void weapon_core::update_recoil( u32 current_time_in_ms, const float time_scale )
{
	m_recoil_calculator.tick( m_user_animations_selector.get_current_state_id( ), is_aimed( ), current_time_in_ms, time_scale );
}

#line 357
void weapon_core::update_dispersion( const bool is_moving, u32 current_time_in_ms )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_user ) );
#line 367
	m_dispersion_calculator.tick( m_user_animations_selector.get_current_state_id( ), is_moving, is_aimed( ), ( *m_user->damage_model( ) ).broken_hands_count( ), m_is_double_handed, current_time_in_ms );
}

void weapon_core::update_breath_vibration( const bool is_holding_breath, u32 current_time_in_ms, const float time_scale )
{
	m_breath_vibration_calculator.hold_breath( is_holding_breath );
	m_breath_vibration_calculator.set_character_multiplier( 0.0f );
	m_breath_vibration_calculator.tick( current_time_in_ms, time_scale );
}

#line 379
void weapon_core::tick( )
{
	player_input const&																		input					= m_user->input( );
	resources::resource_ptr<interactive_object,resources::unmanaged_intrusive_base>		target_active_object	= m_user->target_active_object( );

	user_animations_selector( ).tick( );

	if ( m_logic->current_state( ) && is_idle( ) && !( input.actions_mask & 0x20 ) )
	{
		if ( input.actions_mask & 0x400 )
			set_next_fire_queue_type( );
		else if ( input.actions_mask & 0x800 )
			set_next_ammo_type( );
		else
			reset_fire_queue( );
	}

	if ( target_active_object.c_ptr( ) == this && m_user_animations_selector.is_ready_to_be_deactivated( ) )
		set_target( weapon_target_inactive );
	else if ( !could_be_used( *get_user( ) ) )
		set_target( weapon_target_idle );
	else if ( input.actions_mask & 0x20 )
		set_target( is_trying_to_aim( ) ? weapon_target_aim_fire : weapon_target_fire );
	else if ( input.actions_mask & 0x40 )
		set_target( weapon_target_reload );
	else
		set_target( is_trying_to_aim( ) ? weapon_target_aim : weapon_target_idle );

#line 406
	m_logic->tick( );

#line 409
	if ( m_aiming_state_transition )
	{
#line 413
		user_animations_selector( ).tick( );
#line 415
		m_aiming_state_transition = false;
	}

#line 419
	m_old_actions_mask = input.actions_mask;
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

// claude@NOTE: Target inlines the faithful math::min< u16 > helper here; this
// /GL context keeps it out-of-line.
#line 433
void weapon_core::load_magazine( )
{
	u16 const amount = ammunition( )->amount( );
	u16 load = math::min< u16 >( amount, (u16)( m_magazine_capacity - m_ammo_in_magazine ) );
	ammunition( )->set_amount( amount - load );
	m_ammo_in_magazine += load;

	if ( m_chamber_a_round_on_reload )
		chamber_a_round( );
}

#line 444
void weapon_core::chamber_a_round( )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_ammo_in_magazine ) );
	ASSERT( UNKNOWN_EXPRESSION_T( m_ammo_in_magazine ) );
	ASSERT( UNKNOWN_EXPRESSION_T( m_ammo_in_magazine ) );

	--m_ammo_in_magazine;
	m_is_round_chambered = true;
}

// claude@NOTE: The remaining bytes are resource_ptr accessor inline-depth.
#line 454
void weapon_core::unload_ammo( )
{
	if ( !ammunition( ) )
		return;

	u16 ammo_to_add = m_ammo_in_magazine;
	m_ammo_in_magazine = 0;
	if ( m_is_round_chambered )

		++ammo_to_add;
	m_is_round_chambered = false;


	ammunition( )->set_amount( ammunition( )->amount( ) + ammo_to_add );
}

#line 471
void weapon_core::instant_reload( )
{
	m_aimed = false;

	load_magazine( );

	on_reload( );

	m_recoil_calculator.reload( );
	m_dispersion_calculator.reload( );

	reset_fire_queue( );
}

#line 485
void weapon_core::instant_chamber_a_round( )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_ammo_in_magazine ) );
	ASSERT( UNKNOWN_EXPRESSION_T( m_ammo_in_magazine ) );

	m_aimed = false;

	chamber_a_round( );

	on_chamber_a_round( );

	m_recoil_calculator.chamber_a_round( );
}

// claude@NOTE: The remaining bytes are resource_ptr accessor inline-depth.
#line 500
void weapon_core::reload_one_round( )
{
	if ( m_ammo_in_magazine != m_magazine_capacity && ammunition( )->amount( ) != 0 ) {
		++m_ammo_in_magazine;
		ammunition( )->set_amount( ammunition( )->amount( ) - 1 );
	}
	on_reload( );
}

#line 509
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

// claude@NOTE: The final two bytes are argument-evaluation register scheduling.
#line 525
float3 weapon_core::get_dispersed_bullet_dir( )
{
	float const dispersion_angle	= math::clamp_r( m_normal_random.rand_n( 1.f ), -1.f, 1.f );
	float const dispersion_amount	= m_dispersion_calculator.get_dispersion( ) * dispersion_angle;
	float const random_k			= m_random.random_f( math::pi_x2 );
	float3 const& rot_axis			= math::create_rotation( m_fire_bullet_transform.k.xyz( ), random_k ).transform_direction( m_fire_bullet_transform.i.xyz( ) );
	float3 bullet_direction			= math::create_rotation( m_fire_bullet_transform.k.xyz( ), math::deg2rad( dispersion_amount ) ).transform_direction( rot_axis );

#line 539
	return bullet_direction;
}

#line 542
float weapon_core::get_dispersion( ) const
{
	return m_dispersion_calculator.get_dispersion( );
}

#line 547
void weapon_core::instant_fire( const u32 current_time_in_ms )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_bullets_in_queue ) );
	ASSERT( UNKNOWN_EXPRESSION_T( m_ammunition ) );
	--m_bullets_in_queue;

	ASSERT( UNKNOWN_EXPRESSION_T( m_ammunition ) );
	for ( s32 i = 0; i < ( *m_ammunition ).buck_shot( ); ++i ) {
		float3 const& bullet_direction	= get_dispersed_bullet_dir( );

		float3 const& velocity			= bullet_direction * ( *m_ammunition ).muzzle_speed( );

#line 568
		m_bullet_manager->fire( m_fire_bullet_transform.c.xyz( ), velocity, m_ammunition, *this, current_time_in_ms, m_initiator_holder, m_receiver_holder, ( *m_ammunition ).tracer( ) );
	}

	if ( m_is_there_chamber_a_round_state )
		m_is_round_chambered = false;
	else
		--m_ammo_in_magazine;

#line 578
	on_after_fire( );

	m_recoil_calculator.fire( );
	m_dispersion_calculator.fire( );

	m_initiator_holder->on_fire( );
}

void weapon_core::set_fire_bullet_transform( float4x4 const& fire_bullet_transform )
{
	m_ready_for_fire = true;
	m_fire_bullet_transform = fire_bullet_transform;
}

// claude@NOTE: Target keeps pointer_cast< pbyte > as an identity call here;
// this /GL context inlines it.
#line 592
animation::callback_return_type_enum weapon_core::on_sprint_animation_ended( animation::animation_callback_params& params )
{
	params.interrupt_animation_player_tick = true;

#line 599
	get_user( )->unsubscribe_animation_player( animation::channel_id_on_animation_lexeme_end, this );
#line 603
	get_user( )->unsubscribe_animation_player( animation::channel_id_on_animation_lexeme_end, pointer_cast< pbyte >( this ) + 1 );

#line 607
	m_is_in_sprint_transition = false;
#line 609
	return animation::callback_return_type_dont_call_me_anymore;
}

// claude@NOTE: The second sprint subscription retains pointer_cast in target
// and inlines that identity helper in base.
#line 612
void weapon_core::set_target( weapon_targets target )
{
	if ( target == weapon_target_fire || target == weapon_target_aim_fire )
	{
		if ( ammo_in_weapon( ) == 0 )
			target = weapon_target_reload;
		else if ( !is_ready_to_shoot( ) )
		{
			if ( target == weapon_target_fire )
				target = weapon_target_idle;
			else if ( target == weapon_target_aim_fire )
				target = weapon_target_aim;
		}
	}

	if ( !m_is_in_sprint_transition )
		if ( is_sprinting( ) ) {
		m_is_in_sprint_transition = true;

#line 636
		get_user( )->subscribe_animation_player( animation::channel_id_on_animation_lexeme_end, boost::bind( &weapon_core::on_sprint_animation_ended, this, _1 ), get_user( ), m_user_animations_selector.animations( ).get_sprint_animation( 0, false ), this );
#line 643
		get_user( )->subscribe_animation_player( animation::channel_id_on_animation_lexeme_end, boost::bind( &weapon_core::on_sprint_animation_ended, this, _1 ), get_user( ), m_user_animations_selector.animations( ).get_sprint_animation( 1, false ), pointer_cast< pbyte >( this ) + 1 );
	}

#line 649
	if ( m_is_in_sprint_transition || m_user_animations_selector.is_in_jump( ) ) {
		if ( target == weapon_target_fire )
			target = weapon_target_idle;
		else if ( target == weapon_target_aim_fire )
			target = weapon_target_aim;
	}

	if ( target == weapon_target_reload )
	{
		if ( !ready_to_reload( ) )
		{
			if ( !ammunition( ) || ammunition( )->amount( ) == 0 )
				on_ammo_empty( );

			if ( m_target == weapon_target_aim_fire || m_target == weapon_target_aim )
				target = weapon_target_aim;
			else
				target = weapon_target_idle;
		}
	}

	m_target = target;
}

// claude@NOTE: The two fire_queue_length calls use an LTCG-selected custom
// this register (EAX in target, ECX in base).
#line 675
void weapon_core::reset_fire_queue( )
{
	if ( fire_queue_length( ) == 0xff )
	{
		m_bullets_in_queue = m_ammo_in_magazine;
		if ( m_is_round_chambered )
			++m_bullets_in_queue;
	}
	else
		m_bullets_in_queue = math::min( fire_queue_length( ), ammo_in_weapon( ) );
}

#line 689
void weapon_core::set_next_fire_queue_type( )
{
	if ( m_fire_queue_type == m_weapon_fire_queue_types_count - 1 )
		m_fire_queue_type = 0;
	else
		++m_fire_queue_type;
}

// claude@NOTE: Target keeps get_inventory and resource_ptr::operator== calls
// out-of-line at these sites; base inlines them.
#line 700
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

	m_ammo_slot		= next_slot;
	m_ammunition	= static_cast_resource_ptr< weapon_ammunition_ptr >( get_inventory( ).item_in_slot( m_ammo_slot ) );
	m_target		= weapon_target_reload;
}

#line 725
void weapon_core::set_ammunition( resources::resource_ptr<weapon_ammunition,resources::unmanaged_intrusive_base> const& ammunition_to_set )
{
	m_ammunition = ammunition_to_set;
}

// claude@NOTE: The remaining bytes are resource_ptr/accessor inline-depth.
#line 730
void weapon_core::load_ammo( )
{
	if ( ammunition( ) && ammunition( )->amount( ) != 0 )
	{
		if ( m_ammo_in_magazine == 0 )
			load_magazine( );

		if ( m_is_there_chamber_a_round_state && !m_chamber_a_round_on_reload )
		{
			ASSERT( UNKNOWN_EXPRESSION_T( m_is_round_chambered ) );

			if ( ammunition( )->amount( ) != 0 )
			{
				m_is_round_chambered = true;
				ammunition( )->set_amount( ammunition( )->amount( ) - 1 );

			} } else if ( m_ammo_in_magazine != 0 )
		{
			m_is_round_chambered = true;
			--m_ammo_in_magazine;
		}
	}
}

#line 754
void weapon_core::on_reload_started( )
{
}

#line 760
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

#line 778
void weapon_core::set_animation_callback( pcstr channel_id, pcvoid callback_uid, boost::function<enum animation::callback_return_type_enum(animation::animation_callback_params &)> const& animation_callback )
{
	m_user->subscribe_animation_player( channel_id, animation_callback, callback_uid, NULL, 0xff, this );
}

#line 783
void weapon_core::remove_animation_callback( pcstr channel_id, pcvoid callback_uid )
{
	m_user->unsubscribe_animation_player( channel_id, callback_uid );
}

#line 788
void weapon_core::set_animation_callback( animation::reserved_channel_ids_enum channel_id, pcvoid callback_uid, boost::function<enum animation::callback_return_type_enum(animation::animation_callback_params &)> const& animation_callback )
{
	m_user->subscribe_animation_player( channel_id, animation_callback, callback_uid, NULL, this );
}

#line 793
void weapon_core::remove_animation_callback( animation::reserved_channel_ids_enum channel_id, pcvoid callback_uid )
{
	m_user->unsubscribe_animation_player( channel_id, callback_uid );
}

// claude@NOTE: The remaining statement calls profile_slot_id in target and
// inlines its member load in base.
#line 798
void weapon_core::activate( base_player& user, engine& engine )
{
	m_dispersion_calculator.set_character_dispersion_params( &user.get_dispersion_params( ) );
	m_recoil_calculator.set_character_recoil_params( &user.get_recoil_params( ) );
	m_breath_vibration_calculator.set_user( &user );
	m_breath_vibration_calculator.set_breath_holding_params( &user.get_breath_holding_params( ) );
	m_is_shown = false;
	m_bullet_manager = &engine.get_bullet_manager( );
	m_ready_for_fire = false;
	m_is_in_sprint_transition = false;
	m_initiator_holder = &user;
	m_receiver_holder = &user;
	m_is_firing = false;

	m_dispersion_calculator.set_weapon( this );
	m_recoil_calculator.set_weapon( this );

#line 816
	m_transform = user.get_transform( );
	set_fire_bullet_transform( user.get_transform( ) );

	m_user = &user;

	m_user->subscribe_animation_player( "Left toe", boost::bind( &weapon_core::on_animation_ik_interval, this, _1 ), this, NULL, 0xff, NULL );
	m_user->subscribe_animation_player( "Left heel", boost::bind( &weapon_core::on_animation_ik_interval, this, _1 ), this, NULL, 0xff, NULL );
	m_user->subscribe_animation_player( "Right toe", boost::bind( &weapon_core::on_animation_ik_interval, this, _1 ), this, NULL, 0xff, NULL );
	m_user->subscribe_animation_player( "Right heel", boost::bind( &weapon_core::on_animation_ik_interval, this, _1 ), this, NULL, 0xff, NULL );
	get_user( )->subscribe_animation_player( "left_hand_ik", boost::bind( &weapon_core::on_hand_ik_event, this, _1, hand_to_weapon_ik_processor::left ), this, NULL, 0xff, NULL );
	get_user( )->subscribe_animation_player( "right_hand_ik", boost::bind( &weapon_core::on_hand_ik_event, this, _1, hand_to_weapon_ik_processor::right ), this, NULL, 0xff, NULL );
	m_hand_ik_processor.activate( user.skeleton( ), *m_skeleton );
	m_legs_ik_processor.activate( user.skeleton( ) );

	m_legs_ik_processor.set_character_controller( &m_user->physics_controller( ) );

#line 833
	m_user_animations_selector.activate( user, boost::bind( &weapon_core::on_user_sprint, this, true ), boost::bind( &weapon_core::on_user_sprint, this, false ) );

#line 836
	m_logic->set_initial_state( m_logic->states( ).front( ) );
	m_logic->tick( );

#line 840
	profile_slot_enum ammo1_slot = get_ammo_slot( first_ammo );
	profile_slot_enum ammo2_slot = get_ammo_slot( second_ammo );

	resources::resource_ptr<inventory_item,resources::unmanaged_intrusive_base> const& ammo1 = m_inventory->item_in_slot( ammo1_slot );
	resources::resource_ptr<inventory_item,resources::unmanaged_intrusive_base> const& ammo2 = m_inventory->item_in_slot( ammo2_slot );
	if ( ammo1 )
		set_ammunition( static_cast_resource_ptr< weapon_ammunition_ptr >( ammo1 ) );

	else if ( ammo2 )
		set_ammunition( static_cast_resource_ptr< weapon_ammunition_ptr >( ammo2 ) );

	else {
		set_ammunition( NULL );
		LOGI_INFO( "weapon_core", "There is no ammo in both slots (%s)", request_path( ) );
	}

	m_ammo_slot = m_ammunition ? ( *m_ammunition ).profile_slot_id( ) : invalid_slot;

	if ( m_load_ammo_on_next_activate )
	{
		load_ammo( );
		m_load_ammo_on_next_activate = false;
	}

	reset_fire_queue( );
	instant_show( );

	if ( g_is_server )
	{
		get_user( )->subscribe_animation_player( "sound_events", boost::bind( &weapon_core::on_animation_ik_interval, this, _1 ), get_user( ), NULL, 0xff, NULL );
		get_user( )->subscribe_animation_player( "shell_extraction", boost::bind( &weapon_core::on_animation_ik_interval, this, _1 ), this, NULL, 0xff, NULL );
		get_user( )->subscribe_animation_player( "left_hand_corrector", boost::bind( &weapon_core::on_animation_ik_interval, this, _1 ), this, NULL, 0xff, NULL );
		get_user( )->subscribe_animation_player( "right_hand_corrector", boost::bind( &weapon_core::on_animation_ik_interval, this, _1 ), this, NULL, 0xff, NULL );
	}
}
// claude@NOTE: Sprint cleanup retains pointer_cast in target and inlines it in base.
#line 876
void weapon_core::deactivate( )
{
	if ( g_is_server )
	{
		get_user( )->unsubscribe_animation_player( "sound_events", get_user( ) );
		get_user( )->unsubscribe_animation_player( "shell_extraction", this );
		get_user( )->unsubscribe_animation_player( "left_hand_corrector", this );
		get_user( )->unsubscribe_animation_player( "right_hand_corrector", this );
	}

	m_dispersion_calculator.set_character_dispersion_params( NULL );
	m_recoil_calculator.set_character_recoil_params( NULL );
	m_breath_vibration_calculator.set_user( NULL );
	m_breath_vibration_calculator.set_breath_holding_params( NULL );

	instant_hide( );

	m_user->unsubscribe_animation_player( "Right heel", this );
	m_user->unsubscribe_animation_player( "Right toe", this );
	m_user->unsubscribe_animation_player( "Left heel", this );
	m_user->unsubscribe_animation_player( "Left toe", this );
	m_user->unsubscribe_animation_player( "left_hand_ik", this );
	m_user->unsubscribe_animation_player( "right_hand_ik", this );

	if ( m_is_in_sprint_transition )
	{
#line 904
		get_user( )->unsubscribe_animation_player( animation::channel_id_on_animation_lexeme_end, this );
#line 908
		get_user( )->unsubscribe_animation_player( animation::channel_id_on_animation_lexeme_end, pointer_cast< pbyte >( this ) + 1 );
	}

#line 913
	m_user_animations_selector.deactivate( );
#line 915
	m_logic->set_initial_state( NULL );
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

void weapon_core::update_bones_matrices(
	resources::resource_ptr<animation::skeleton,resources::unmanaged_intrusive_base> const&	user_skeleton,
	float4x4* const						user_matrices,
	const u32									user_matrices_count,
	const u32									current_time_in_ms,
	float4x4&							character_head_transform,
	float4x4&							character_transform,
	animation::animation_player const&	user_animation_player
)
{
	static bool draw_bones_matrices = false;

#line 943
	m_last_tick_time_in_ms = current_time_in_ms;
	update_dispersion( ( m_user->input( ).actions_mask & 0x1 ) != ( m_user->input( ).actions_mask & 0x2 ) || ( m_user->input( ).actions_mask & 0x8 ) != ( m_user->input( ).actions_mask & 0x4 ), current_time_in_ms );

	float4x4 const&	weapon_transform		= m_user->get_transform( );

	u32 const		weapon_matrices_count	= ( *m_skeleton ).get_non_root_bones_count( );
	float4x4* const	weapon_matrices			= (float4x4*)alloca( weapon_matrices_count * sizeof( float4x4 ) );

	user_animation_player.compute_bones_matrices( *m_skeleton, weapon_matrices, weapon_matrices + weapon_matrices_count, this, NULL );

	u32 const		weapon_bone_index		= ( *user_skeleton ).get_bone_index( "Weapon" ) - ( *user_skeleton ).get_root_bones_count( );

	user_animation_player.compute_bones_local_matrices( *user_skeleton, user_matrices, user_matrices + weapon_matrices_count, m_user, NULL );
	change_matrix_orientation( math::create_rotation( float3( math::pi, 0.0f, 0.0f ) ), user_matrices[weapon_bone_index] );

#line 961
	if ( s_ik_enable_on_hands_value )
		m_hand_ik_processor.process( current_time_in_ms, weapon_matrices, user_matrices );

#line 966
	if ( s_ik_enable_on_legs_value && m_user->physics_controller( ).on_ground( ) )
	{
		m_legs_ik_processor.tick( current_time_in_ms );
		m_legs_ik_processor.process( user_matrices, weapon_transform );
	}

#line 972
	process_finger_correction( current_time_in_ms, user_matrices );
	user_animation_player.convert_to_object_matrices( *user_skeleton, user_matrices, user_matrices + user_matrices_count, m_user );

#line 993
	if ( draw_bones_matrices )
	{
		for ( float4x4* i = user_matrices, *e = user_matrices + user_matrices_count; i != e; ++i )
		{
#line 998
			for ( u32 j = 0; j < 4; ++j ) {
			}
		}
	}

#line 1005
	float4x4 const&	user_transform			= weapon_transform * user_matrices[weapon_bone_index];
	set_transform( user_transform );

#line 1017
	on_skeleton_matrices_changed( current_time_in_ms, weapon_transform, weapon_matrices, weapon_matrices + weapon_matrices_count, user_transform, user_matrices, user_matrices + user_matrices_count, weapon_transform );

#line 1023
	u32 const		head_bone_index			= ( *user_skeleton ).get_bone_index( "Head" ) - ( *user_skeleton ).get_root_bones_count( );
	character_head_transform = animation::calculated_head_matrix( user_matrices[head_bone_index], weapon_transform );
	set_fire_bullet_transform( character_head_transform );

	u32 const		root_bone_index			= ( *user_skeleton ).get_bone_index( "Root" ) - ( *user_skeleton ).get_root_bones_count( );
	character_transform = weapon_transform * user_matrices[root_bone_index];
}

bool weapon_core::is_sprinting( ) const
{
	return m_user_animations_selector.is_sprinting( );
}

void weapon_core::on_skeleton_matrices_changed(
	const u32						current_time_in_ms,
	float4x4 const&			weapon_transform,
	float4x4 const* const	weapon_matrices_begin,
	float4x4 const* const	weapon_matrices_end,
	float4x4 const&			user_transform,
	float4x4* const			user_matrices_begin,
	float4x4* const			user_matrices_end,
	float4x4 const&			user_weapon_transform
)
{
	VOSTOK_UNREFERENCED_PARAMETERS(
		current_time_in_ms,
		weapon_transform,
		weapon_matrices_begin,
		weapon_matrices_end,
		user_transform,
		user_matrices_begin,
		user_matrices_end,
		user_weapon_transform
	);
}

void weapon_core::process_finger_correction( const u32 current_time_in_ms, float4x4* const user_matrices )
{
	VOSTOK_UNREFERENCED_PARAMETERS( current_time_in_ms, user_matrices );
}

// claude@NOTE: Packet append and FSM accessors cross different /GL inline cuts.
#line 1058
void weapon_core::serialize( network_core::udp_match_packet& packet, u32 client_offset ) const
{
	inventory_item::serialize( packet, client_offset );

#line 1066
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

	if ( !!m_logic->current_state( ) )
	{
		packet.append( m_is_shown );
#line 1086
		m_hand_ik_processor.serialize( packet, client_offset );

		u8							state_id	= 0;
		bool						found		= false;
		ai::fsm_state const* const	current		= m_logic->current_state( );
		for ( ai::fsm_state const* i = m_logic->states( ).front( ); i; i = i->next, ++state_id ) {
			if ( i == current ) {
				found	= true;
				break;
			}
		}

		ASSERT( UNKNOWN_EXPRESSION_T( found ) );

		packet.append( state_id );
		static_cast_checked< weapon_core_base_state const* >( m_logic->current_state( ) )->serialize( packet );

		m_user_animations_selector.serialize( packet );
	}
}

// claude@NOTE: packet_reader::r<T> remains out-of-line in target and inlines in base.
#line 1107
void weapon_core::deserialize( network_core::packet_reader& reader )
{
	inventory_item::deserialize( reader );
	m_deserializing	= true;

#line 1116
	m_random.seed( reader.r< u32 >( ) );
	m_normal_random.set_seed( reader.r< s32 >( ) );
	m_target			= (weapon_targets)reader.r< bool >( );

	m_old_actions_mask	= reader.r< u32 >( );

	m_ammo_in_magazine	= reader.r< u16 >( );
	m_bullets_in_queue	= reader.r< u16 >( );
	m_fire_queue_type	= reader.r< bool >( );
	m_ammo_slot			= (profile_slot_enum)reader.r< bool >( );

	if ( m_ammo_slot != invalid_slot )
		m_ammunition	= static_cast_resource_ptr< weapon_ammunition_ptr >( get_inventory( ).item_in_slot( m_ammo_slot ) );
	else
		m_ammunition	= NULL;

	if ( m_is_there_chamber_a_round_state )
		m_is_round_chambered	= reader.r< bool >( );

	if ( !!m_logic->current_state( ) )
	{
		m_is_shown	= reader.r< bool >( );
#line 1140
		m_hand_ik_processor.deserialize( reader );

		u8 const			target_state_id	= reader.r< bool >( );
		u8					state_id		= 0;
		ai::fsm_state*		current			= NULL;
		for ( ai::fsm_state* i = m_logic->states( ).front( ); i; i = i->next, ++state_id ) {
			if ( state_id == target_state_id ) {
				current	= i;
				break;
			}
		}

		ASSERT( UNKNOWN_EXPRESSION_T( current ) );
		m_logic->set_initial_state( current );
		static_cast_checked< weapon_core_base_state* >( m_logic->current_state( ) )->deserialize( reader );

		m_user_animations_selector.deserialize( reader );

		m_user->force_animation_selection( );
	}

	m_deserializing	= false;
}

bool weapon_core::instant_idle_predicate( ) const
{
	return m_user_animations_selector.sprint_predicate( ) || m_user_animations_selector.is_in_jump( );
}

#line 1169
bool weapon_core::could_be_used( base_player const& user ) const
{
	u8 const broken_hands_count = ( *user.damage_model( ) ).broken_hands_count( );
	return !( broken_hands_count == 2 && is_double_handed( ) );
}

#line 1175
bool weapon_core::could_be_aimed( base_player const& user ) const
{
	u8 const broken_hands_count = ( *user.damage_model( ) ).broken_hands_count( );
	return broken_hands_count != 2;
}

// claude@NOTE: The recoil trio has identical statements, CFG, size, and opcodes;
// only anonymous stack slots and the folded empty-stub alias differ.
#line 1194
float weapon_core::computed_backward_recoil_time(
	const float		animation_length,
	const float		animation_time_before_time_scale_starts,
	const u32			time_scale_start_time_in_ms,
	const u32			current_time_in_ms,
	const u32			target_time_in_ms,
	const float		time_scale
)
{
	VOSTOK_UNREFERENCED_PARAMETERS( animation_time_before_time_scale_starts, time_scale_start_time_in_ms, current_time_in_ms );
	update_recoil( target_time_in_ms, time_scale );
	update_breath_vibration( m_user->input( ).is_holding_breath( ), target_time_in_ms, time_scale );
	return math::clamp_r( m_recoil_calculator.get_back_coeff( ), epsilon, clear_value - epsilon ) * animation_length;
}

float weapon_core::computed_horizontal_recoil_time(
	const float		animation_length,
	const float		animation_time_before_time_scale_starts,
	const u32			time_scale_start_time_in_ms,
	const u32			current_time_in_ms,
	const u32			target_time_in_ms,
	const float		time_scale
)
{
	VOSTOK_UNREFERENCED_PARAMETERS( animation_time_before_time_scale_starts, time_scale_start_time_in_ms, current_time_in_ms );
	update_recoil( target_time_in_ms, time_scale );
	update_breath_vibration( m_user->input( ).is_holding_breath( ), target_time_in_ms, time_scale );
	return horizontal_recoil_value( ) * animation_length;
}

float weapon_core::computed_vertical_recoil_time(
	const float		animation_length,
	const float		animation_time_before_time_scale_starts,
	const u32			time_scale_start_time_in_ms,
	const u32			current_time_in_ms,
	const u32			target_time_in_ms,
	const float		time_scale
)
{
	VOSTOK_UNREFERENCED_PARAMETERS( animation_time_before_time_scale_starts, time_scale_start_time_in_ms, current_time_in_ms );
	update_recoil( target_time_in_ms, time_scale );
	update_breath_vibration( m_user->input( ).is_holding_breath( ), target_time_in_ms, time_scale );
	return vertical_recoil_value( ) * animation_length;
}

fastdelegate::FastDelegate< float( float, float, u32, u32, u32, float ) > weapon_core::backward_recoil_time_calculator( )
{
	return fastdelegate::FastDelegate< float( float, float, u32, u32, u32, float ) >( this, &weapon_core::computed_backward_recoil_time );
}

fastdelegate::FastDelegate< float( float, float, u32, u32, u32, float ) > weapon_core::horizontal_recoil_time_calculator( )
{
	return fastdelegate::FastDelegate< float( float, float, u32, u32, u32, float ) >( this, &weapon_core::computed_horizontal_recoil_time );
}

fastdelegate::FastDelegate< float( float, float, u32, u32, u32, float ) > weapon_core::vertical_recoil_time_calculator( )
{
	return fastdelegate::FastDelegate< float( float, float, u32, u32, u32, float ) >( this, &weapon_core::computed_vertical_recoil_time );
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

// claude@NOTE: The two slot totals differ only in resource_ptr safe-bool inline depth.
void weapon_core::get_ammo_info( weapon_ammo_info& info )
{
#line 1274
	info.current_ammo_type		= ( ammo_slot( ) != get_ammo_slot( first_ammo ) ) + 1;
	info.fire_queue_size		= fire_queue_length( );
	info.ammo_in_magazine		= ammo_in_magazine( );

	inventory& inv				= get_inventory( );
	resources::resource_ptr<inventory_item,resources::unmanaged_intrusive_base> slot1_itm = inv.item_in_slot( get_ammo_slot( first_ammo ) );
	resources::resource_ptr<inventory_item,resources::unmanaged_intrusive_base> slot2_itm = inv.item_in_slot( get_ammo_slot( second_ammo ) );

	info.ammo1_total			= slot1_itm == NULL ? 0 : ( *slot1_itm ).amount( );
	info.ammo2_total			= slot2_itm == NULL ? 0 : ( *slot2_itm ).amount( );

	info.round_is_chambered		= m_is_round_chambered;
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

bool weapon_core::can_and_must_reload_predicate( ) const
{
	return ready_to_reload( ) && m_ammo_in_magazine == 0 && !m_is_round_chambered;
}

#line 1338
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
