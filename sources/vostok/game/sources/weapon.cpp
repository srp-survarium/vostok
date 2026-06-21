////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "weapon.h"
#include "game_world_ui.h"

#include <vostok/game_core/inventory.h>

#include "weapon_user_dead_state.h"

#include <vostok/math_constants.h>
#include <vostok/console_command.h>

#include "base_game_scene.h"
#include <vostok/render/facade/game_renderer.h>
#include <vostok/render/facade/scene_renderer.h>

// TU-local console-command statics (file scope, no namespace prefix in the PDB).
// finger_corrector_enable gates weapon::process_finger_correction.
static bool s_enable_finger_corrector_value = true;
static vostok::console_commands::cc_bool s_attach_fingers_to_weapon_cc( "finger_corrector_enable", s_enable_finger_corrector_value, false, vostok::console_commands::command_type_user_specific );

namespace survarium {

// claude@NOTE: STUB - 6 body stmts. Init-list builds weapon_core base, m_fingers_corrector (vector
// ctor of 2 hands), m_weapon_fire_light_props (light_props ctor). Body line 86 (big): zeroes the pfx
// list/count/model/ui/scene pointers, sets m_first/third/preview death-animation counts, takes
// m_weapon_fire_light_id from the survarium::light_ids global (post-increment), copies this+0x158
// into m_weapon_fire_light_props (rep movsd), seeds m_fire_light_anim_length=0xC8 and the firing-light
// state floats; lines 88-90 set more light_props fields; 98/99 set m_barrel_transform = m_scope_transform
// = float4x4().identity(). Parked: needs the light_ids global + the light_props field seeds + the named
// float constants (__real@437f0000/40a00000 etc.) pinned across several diff cycles. Const members must
// stay init-list; m_fire_light_anim_length seed is 0xC8.
// STATE[STUB]
 weapon::weapon(
	const u32		first_view_death_animations_count,
	const u32		third_view_death_animations_count,
	u32				preview_animations_count
) :
	m_fire_light_anim_length				( 0 ),
	m_first_view_death_animations_count		( first_view_death_animations_count ),
	m_third_view_death_animations_count		( third_view_death_animations_count ),
	m_preview_animations_count				( preview_animations_count )
{
}

 weapon::~weapon( )
{
	if ( get_game_scene( ) && get_game_scene( )->render_scene( ) )
	{
		render::scene_renderer& scene = get_game_scene( )->renderer( ).scene( );

		for ( u8 i = 0; i < m_fire_pfx_count; ++i )
			if ( particle::is_playing( m_fire_pfx_list[ i ] ) )
				scene.remove_particle_system_instance( get_game_scene( )->render_scene( ), m_fire_pfx_list[ i ] );

		for ( u8 i = 0; i < m_shells_pfx_count; ++i )
			if ( particle::is_playing( m_shells_pfx_list[ i ] ) )
				scene.remove_particle_system_instance( get_game_scene( )->render_scene( ), m_shells_pfx_list[ i ] );
	}
}

void weapon::set_fire_bullet_transform( float4x4 const& transform )
{
	weapon_core::set_fire_bullet_transform( m_is_third_view ? m_barrel_transform : transform );
}

// claude@NOTE: STUB - calls weapon_core::instant_aim_start, then (if m_game_ui) drives the player
// aim FOV transition: reads a sound-emitter aim value (m_rifle_scope, +0x114/+0x118) else
// m_aim_fov_factor/m_aim_near_plane_factor (weapon_core +0x464/+0x468), and writes the player
// fov-transition block (user().m_local_input_controller, player+0x10EF4 -> +0x10EF4/+0x10F0C..F28
// + [+0x1A0]=4). Walled: that block lives in player / player_input_handler, incomplete in this TU
// (forward-declared). Needs player.h + the s_aim_transition_time / satisfaction_equality_tolerance
// file-statics (unknown seeds). Parked until the player layer is includable here.
// STATE[STUB]
void weapon::instant_aim_start( )
{
}

// claude@NOTE: STUB - mirror of instant_aim_start for the end transition (sets the player
// fov-transition block to the idle target with [+0x1A0]=1). Same player/player_input_handler
// incomplete-type wall + s_aim_transition_time/clear_value file-static seeds.
// STATE[STUB]
void weapon::instant_aim_end( )
{
}

void weapon::tick( )
{
	weapon_core::tick( );
}

void weapon::set_transform( float4x4 const& transform )
{
	weapon_core::set_transform( transform );
}

bool is_dead( base_player*& user )
{
	return !user->is_alive( );
}

bool is_alive( base_player*& user )
{
	return user->is_alive( );
}

float freeze_at_end_time_calculator(
	float		animation_length,
	float		animation_time_before_time_scale_starts,
	u32			time_scale_start_time_in_ms,
	u32			current_time_in_ms,
	u32			target_time_in_ms,
	float		time_scale
)
{
	return animation_time_before_time_scale_starts + ( target_time_in_ms - time_scale_start_time_in_ms ) * time_scale * math::epsilon_3;
}

// STATE[STUB]
std::pair< animation::mixing::expression, animation::mixing::animation_lexeme > weapon_user_dead_state::selected_animations(
	mutable_buffer&							buffer,
	weapon_animation_parameters const&		weapon_parameters,
	const bool								is_third_view
) const
{
	// claude@NOTE: STUB - parked. Builds a (expression, animation_lexeme) pair: a death_lexeme local
	// (animation::mixing::animation_lexeme) from the weapon animation parameters + an expression, returned
	// as the pair. VOSTOK_UNREACHABLE_CODE is the buildability device (the pair element types have no
	// default ctor - empty_hands precedent). Needs the animation::mixing lexeme/expression builders +
	// the create_animation_interval lexeme wall (shared with the *_state TUs) before it can be bodied.
	VOSTOK_UNREACHABLE_CODE( );
}

bool weapon_user_dead_state::is_ready_for_transition( ) const
{
	return false;
}

// claude@NOTE: STUB - 16 stmts. Stores model = base_model + m_rifle_scope = rifle_scope, looks up
// the barrel/scope locators (model->get_locator( "...", m_barrel_locator/m_scope_locator ), the two
// CALL SITE `bool(pcstr, model_locator_item&) const`), copies them, derives the toe bone indices,
// and wires the dead player_logic_base_state (local `dead`). Walled: render_model_instance::get_locator
// + the player_logic_base_state plumbing pull model + player-state internals not reachable from this
// TU at the byte level; structure recoverable once those are spelled.
// STATE[STUB]
void weapon::load_weapon(
	render::skeleton_model_ptr const&	base_model,
	rifle_scope_ptr const&		rifle_scope
)
{
}

// STATE[STUB]
// claude@NOTE: STUB - 6 stmts, NRV float4x4 return. Magic-static `add = math::create_rotation_y( pi )`
// (line 290), reads locator.m_bone (+0x60), then composes via math::mul4x3 of locator.m_offset (+0x20),
// `add`, and m_transform (weapon_core +0x158) - with matrices[locator.m_bone] folded in when m_bone !=
// 0xFFFF, else the identity/transform-only path. Parked: the exact mul4x3 nesting + operand order and
// the NRV stack-temp scheduling need several diff cycles to pin; pure math, no cross-module wall.
// STATE[STUB]
float4x4 weapon::calculate_locator(
	render::model_locator_item const&		locator,
	float4x4 const*							matrices,
	const u32								matrices_count
)
{
	return vostok::math::float4x4( );
}

// claude@NOTE: last statement (line 318) is STUB - it sets the player_input_handler aim/input
// state to 1 via user().m_local_input_controller (player+0x10EF4 -> [+0x1A0] = 1). player and
// player_input_handler are incomplete in this TU (forward-declared only), so that member chain
// can't be spelled here without pulling player.h + player_input_handler.h. The other 10 statements
// (model add x2, the m_game_ui UI refresh block) are recovered.
void weapon::on_show( )
{
	m_is_in_scene = true;
	render::scene_ptr scene = get_game_scene( )->render_scene( );

	get_game_scene( )->renderer( ).scene( ).add_model( scene, model->m_render_model, transform( ) );

	if ( m_rifle_scope )
		get_game_scene( )->renderer( ).scene( ).add_model( scene, m_rifle_scope->idle_model( )->m_render_model, transform( ) );

	if ( m_game_ui )
	{
		m_game_ui->set_ammo_type( (u8)( m_ammo_slot != get_ammo_slot( first_ammo ) ) + 1 );
		m_game_ui->set_fire_queue_size( m_weapon_fire_queue_types[ m_fire_queue_type ] );
		m_game_ui->show_ammo_indicator( true );

		if ( m_game_ui )
			m_game_ui->show_crosshair( true );

		set_ui_ammo( true );
	}
}

void weapon::on_hide( )
{
	m_is_in_scene = false;
	render::scene_ptr scene = get_game_scene( )->render_scene( );

	if ( !( m_is_scope_aimed && m_rifle_scope && m_rifle_scope->hide_weapon_on_aim( ) ) )
		get_game_scene( )->renderer( ).scene( ).remove_model( scene, model->m_render_model );

	if ( m_rifle_scope )
		get_game_scene( )->renderer( ).scene( ).remove_model(
			scene,
			( m_is_scope_aimed
				? m_rifle_scope->aimed_model( )
				: m_rifle_scope->idle_model( ) )->m_render_model
		);

	if ( m_game_ui )
	{
		m_is_scope_aimed = false;
		m_game_ui->show_crosshair( false );
	}

	if ( m_game_ui )
		m_game_ui->show_ammo_indicator( false );
}

// claude@NOTE: structure matched (8 stmts, 0 named locals). Capped by inline-vs-call: the target
// INLINES inventory::item_in_slot (direct m_slots[slot].item access) and the inventory_item_ptr
// addref/release, but item_in_slot is parked out-of-line (STATE[STUB] in inventory.h, LTCG custom
// convention) so our base CALLs it + the resource_ptr copy-ctor. Also the set_ammo_in_magazine arg
// is register-passed (16-bit add) target-side vs our push. Both are cross-unit LTCG walls.
void weapon::set_ui_ammo( bool update_total_count )
{
	if ( m_game_ui && m_inventory )
	{
		m_game_ui->set_ammo_in_magazine( ( m_is_round_chambered != 0 ) + m_ammo_in_magazine );

		if ( update_total_count )
		{
			inventory& inv = *m_inventory;

			inventory_item_ptr ammo1 = inv.item_in_slot( get_ammo_slot( first_ammo ) );
			inventory_item_ptr ammo2 = inv.item_in_slot( get_ammo_slot( second_ammo ) );

			m_game_ui->set_ammo_total_count(
				ammo1 ? ( *ammo1 ).amount( ) : 0,
				ammo2 ? ( *ammo2 ).amount( ) : 0
			);
		}
	}
}

void weapon::on_reload( )
{
	set_ui_ammo( true );
}

// claude@NOTE: structure correct. Target records 0 line records (ICF-folded COMDAT) and tail-jmps
// set_ammo_in_magazine with a 16-bit `add ax,[47A]` register-arg; our base does the 32-bit add +
// push/call/ret (no tail-call). LTCG call-convention cap, not source-steerable.
void weapon::on_chamber_a_round( )
{
	if ( m_game_ui && m_inventory )
		m_game_ui->set_ammo_in_magazine( ( m_is_round_chambered != 0 ) + m_ammo_in_magazine );
}

void weapon::on_unload_chambered_round( )
{
	if ( m_game_ui && m_inventory )
		m_game_ui->set_ammo_in_magazine( ( m_is_round_chambered != 0 ) + m_ammo_in_magazine );
}

void weapon::show_crosshair( )
{
	if ( m_game_ui )
		m_game_ui->show_crosshair( true );
}

void weapon::hide_crosshair( )
{
	if ( m_game_ui )
		m_game_ui->show_crosshair( false );
}

void weapon::on_before_fire( )
{
}

void weapon::on_after_fire( )
{
	if ( m_game_ui )
		m_game_ui->set_ammo_in_magazine( m_ammo_in_magazine );

	play_weapon_fire_pfx( );

	if ( m_game_ui && m_inventory )
		m_game_ui->set_ammo_in_magazine( ( m_is_round_chambered != 0 ) + m_ammo_in_magazine );
}

void weapon::set_target( const weapon_targets new_target )
{
	weapon_targets old_target = m_target;

	weapon_core::set_target( new_target );

	if ( m_target == weapon_target_fire )
	{
		if ( old_target != m_target )
		{
			m_weapon_fire_light_props.transform = m_barrel_transform;
			get_game_scene( )->renderer( ).scene( ).add_light( get_game_scene( )->render_scene( ), m_weapon_fire_light_id, m_weapon_fire_light_props );
			m_firing_light_added = true;
		}
	}
	else if ( old_target == weapon_target_fire )
	{
		if ( m_firing_light_added )
		{
			get_game_scene( )->renderer( ).scene( ).remove_light( get_game_scene( )->render_scene( ), m_weapon_fire_light_id );
			m_firing_light_added = false;
		}
	}
}

// claude@NOTE: play_weapon_fire_pfx / play_weapon_shell_pfx are STRUCTURE MATCH; the
// LTCG this-in-esi convention is now reproduced (anchor_game_weapons direct-call) and
// access mangles AAE/QAE correctly. Residual is the play_particle_system call-boundary
// register cascade: target keeps get_game_scene() in eax and computes render_scene()
// (`lea ecx,[eax+4]`) early; the base lands it in ecx with a late `add ecx,4`. Same
// non-steerable /Od arg-eval scheduling as object_particle_visual::insert (96%).
void weapon::play_weapon_fire_pfx( )
{
	if ( m_fire_pfx_list )
	{
		get_game_scene( )->renderer( ).scene( ).play_particle_system( get_game_scene( )->render_scene( ), m_fire_pfx_list[ m_current_fire_pfx_id ], m_barrel_transform );
		++m_current_fire_pfx_id;
		if ( m_current_fire_pfx_id == m_fire_pfx_count )
			m_current_fire_pfx_id = 0;
	}
}

void weapon::play_weapon_shell_pfx( )
{
	if ( m_shells_pfx_list )
	{
		get_game_scene( )->renderer( ).scene( ).play_particle_system( get_game_scene( )->render_scene( ), m_shells_pfx_list[ m_current_shell_pfx_id ], m_scope_transform );
		++m_current_shell_pfx_id;
		if ( m_current_shell_pfx_id == m_shells_pfx_count )
			m_current_shell_pfx_id = 0;
	}
}

// claude@NOTE: target structure records 6 statements that all compile away in
// MASTER_GOLD (asm is a bare `ret`) - likely debug-render / LOG calls that are
// no-ops in gold; their exact form is unrecoverable from the carcass. Empty body
// byte-matches but is structurally short by 6 statements.
void weapon::show_laser_pointer( )
{
}

void weapon::update_pfx_transform( )
{
	if ( m_firing_light_added )
	{
		m_weapon_fire_light_props.transform = m_barrel_transform;
		get_game_scene( )->renderer( ).scene( ).update_light( get_game_scene( )->render_scene( ), m_weapon_fire_light_id, m_weapon_fire_light_props );
	}

	if ( m_fire_pfx_list )
		for ( u8 i = 0; i < m_fire_pfx_count; ++i )
			get_game_scene( )->renderer( ).scene( ).update_particle_system_instance( get_game_scene( )->render_scene( ), m_fire_pfx_list[ i ], m_barrel_transform );
}

void weapon::set_next_fire_queue_type( )
{
	weapon_core::set_next_fire_queue_type( );

	if ( m_game_ui )
		m_game_ui->set_fire_queue_size( m_weapon_fire_queue_types[ m_fire_queue_type ] );
}

void weapon::set_next_ammo_type( )
{
	weapon_core::set_next_ammo_type( );

	if ( m_game_ui )
		m_game_ui->set_ammo_type( (u8)( m_ammo_slot != get_ammo_slot( first_ammo ) ) + 1 );
}

void weapon::on_reload_started( )
{
}

void weapon::on_ammo_empty( )
{
	if ( m_game_ui )
		m_game_ui->show_screen_message( "st_empty_ammo_message" );
}

// claude@NOTE: STUB - ~13 stmts. Guards on user.<player flag +0x10F80> (copies recoil/breath calc
// +0x284 -> +0x2C0), sets m_game_scene from engine (engine-0xC), calls weapon_core::activate, then
// finds the Left/RightFoot bone indices via user.skeleton() bone search (m_left/right_toe_bone_index
// +0xFD4/+0xFD8), and subscribes 4 animation callbacks ("sound_events"/"shell_extraction"/
// "left_hand_corrector"/"right_hand_corrector" -> on_foot_step / on_shell_extraction_event /
// on_hand_correction_event(left/right) via boost::bind). Walled: user is base_player but the +0x10F80
// flag + skeleton() are player-level (incomplete here); needs player.h. Structure recoverable after.
// STATE[STUB]
void weapon::activate( base_player& user, engine& engine )
{
}

// claude@NOTE: structure matched (4 remove_animation_callback + base deactivate). Capped by
// inline-vs-call: the target INLINES remove_animation_callback (the m_user->unsubscribe_animation_player
// virtual) and get_user(), while our LTCG keeps both as `call`s. Not source-steerable from here.
void weapon::deactivate( )
{
	remove_animation_callback( "sound_events", get_user( ) );
	remove_animation_callback( "shell_extraction", get_user( ) );
	remove_animation_callback( "left_hand_corrector", get_user( ) );
	remove_animation_callback( "right_hand_corrector", get_user( ) );

	weapon_core::deactivate( );
}

// claude@NOTE: STUB - structure recovered (5 stmts): if ( params.animated_object == m_user )
// { if ( params.domain_data == 5 || params.domain_data == 6 ) { float4x4 const& toe =
// params.domain_data == 5 ? m_left_toe_transform : m_right_toe_transform; if ( !<player flag
// at +0x10F80> ) get_game_scene()->get_step_manager().on_step( user(), toe.<pos +0x20>,
// toe.<dir +0x30>, <game_world> ); } }. Walled: the +0x10F80 guard and the on_step `player
// const&`/`game_world&` args read through user() (player), which is incomplete in this TU
// (forward-declared); spelling them needs player.h + the step_manager game_world plumbing.
// STATE[STUB]
animation::callback_return_type_enum weapon::on_foot_step( animation::animation_callback_params& params )
{
	return animation::callback_return_type_call_me_again;
}

// claude@NOTE: STUB - the big one (82 stmts). Per-tick skeleton processing: computes
// weapon_matrices_count from the begin/end range, runs the hand/legs IK + dispersion/recoil/breath
// updates and the toe-transform extraction (m_left/right_toe_transform from user_matrices at the toe
// bone indices), updates the pfx/light transforms, then the fingers corrector + UI. Locals scene
// (base_scene_ptr), weapon_matrices_count (u32), renderer (render::game::renderer&). Parked: too
// large to land in one pass and it threads player/IK-processor/render internals; recover statement
// group by group later (structure list preserved in the target PDB).
// STATE[STUB]
void weapon::on_skeleton_matrices_changed(
	const u32					current_time_in_ms,
	float4x4 const&				weapon_transform,
	float4x4 const* const		weapon_matrices_begin,
	float4x4 const* const		weapon_matrices_end,
	float4x4 const&				user_transform,
	float4x4* const				user_matrices_begin,
	float4x4* const				user_matrices_end,
	float4x4 const&				__formal
)
{
	// reconstruction parked - see note above
}

// claude@NOTE: faithful body (finger_corrector_enable guard + tail-call to
// fingers_to_weapon_corrector::process). Walled: process() is an empty STUB in
// fingers_to_weapon_corrector.cpp (a separate TU), so LTCG inlines its empty body and DCE
// drops the whole guarded statement, leaving our base a bare `ret 8`. Recovers once that
// process() lands a real body; do NOT collapse the guard to hold a % - structure is correct.
void weapon::process_finger_correction( const u32 current_time_in_ms, float4x4* const user_matrices )
{
	if ( s_enable_finger_corrector_value )
		m_fingers_corrector.process( current_time_in_ms, user_matrices );
}

animation::callback_return_type_enum weapon::on_hand_correction_event(
	animation::animation_callback_params&	params,
	const fingers_to_weapon_corrector::hands_enum	hand
)
{
	m_fingers_corrector.activate_hand( hand, params.domain_data == 9, params.callback_time_in_ms );
	return animation::callback_return_type_call_me_again;
}

// claude@NOTE: faithful body (play_weapon_shell_pfx + return call_me_again). Residual is
// an LTCG prologue-frame choice: the target reserves a 4-byte slot (push ecx) and preserves
// `this` in esi across the call; our LTCG build, seeing the full play_weapon_shell_pfx def,
// emits the leaf-minimal form (no esi save). Structure is correct (2 statements).
animation::callback_return_type_enum weapon::on_shell_extraction_event( animation::animation_callback_params& params )
{
	play_weapon_shell_pfx( );
	return animation::callback_return_type_call_me_again;
}

// claude@NOTE: STUB - structure recovered but parked on unrecoverable constants.
// Body (7 stmts): function-local magic-static cc_bool s_hide_crosshair_on_aim_cc over
// s_hide_crosshair_on_aim_value (line 685), then
//   if ( m_game_ui ) {
//     m_game_ui->show_crosshair( !( s_hide_crosshair_on_aim_value && m_aimed ) );  // m_aimed @0x488
//     const float crosshair_size = s_dispersion_gui_scale_coef_value / default_vertical_fov * get_dispersion( );
//     m_game_ui->set_crosshair_size( crosshair_size );
//   }
// Walls: survarium::default_vertical_fov (?default_vertical_fov@survarium@@3MA) has no home TU in
// the corpus and an unknown seed; s_dispersion_gui_scale_coef_value + the cc_value<bool> ctor args +
// the s_hide_crosshair_on_aim_value seed are also unrecoverable. Defining default_vertical_fov here
// with a guessed value would fabricate a cross-cutting global - left parked.
// STATE[STUB]
void weapon::update_dispersion_visual_representation( )
{
}

// claude@NOTE: structure correct (base call + 2 inlined activate_hand). The target records a
// separate statement (line 705) that hoists `!user_is_sprinting` (both hands' is_active computed
// up front, reused as al/cl); our base computes each is_active just-in-time. CSE/hoisting
// scheduling difference, no named local on either side - not source-steerable.
void weapon::on_user_sprint( const bool user_is_sprinting )
{
	weapon_core::on_user_sprint( user_is_sprinting );

	m_fingers_corrector.activate_hand( fingers_to_weapon_corrector::left,  m_is_double_handed || !user_is_sprinting, m_last_tick_time_in_ms );
	m_fingers_corrector.activate_hand( fingers_to_weapon_corrector::right, !user_is_sprinting,                        m_last_tick_time_in_ms );
}

} // namespace survarium
