////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "weapon.h"
#include "game_world_ui.h"

#include <vostok/game_core/inventory.h>
#include <vostok/particle/world.h>

#include "weapon_user_dead_state.h"
#include "player_logic_preview_state.h"

#include <vostok/math_constants.h>
#include <vostok/console_command.h>

#include "base_game_scene.h"
#include "game.h"
#include "game_camera.h"
#include "player.h"
#include "player_input_handler.h"
#include "step_manager.h"
#include "game_world.h"
#include <vostok/render/facade/debug_renderer.h>
#include <vostok/render/facade/game_renderer.h>
#include <vostok/render/facade/scene_renderer.h>
#include <vostok/animation/mixing_animation_lexeme.h>			// dead-state selected_animations
#include <vostok/animation/mixing_animation_lexeme_parameters.h>

// TU-local console-command statics (file scope, no namespace prefix in the PDB).
// finger_corrector_enable gates weapon::process_finger_correction.
static bool s_enable_finger_corrector_value = true;
static vostok::console_commands::cc_bool s_attach_fingers_to_weapon_cc( "finger_corrector_enable", s_enable_finger_corrector_value, false, vostok::console_commands::command_type_user_specific );

static bool s_draw_fire_point = false;

// hide_crosshair_on_aim gates the crosshair in update_dispersion_visual_representation;
// s_dispersion_gui_scale_coef_value scales the crosshair size by the dispersion. The
// cc_float registration (dispersion_magic_coef_cc) takes the value's address, which keeps
// the coef load alive in update_dispersion (without it MSVC folds the 1.0 default away).
// sushi@TODO: the s_hide_crosshair_on_aim_value/s_dispersion_gui_scale_coef_value seeds and
// the dispersion_magic_coef_cc command-name string are unrecoverable from the asm (data
// section); cc_float min/max (0/10000) recovered from the initializer; function bytes are
// seed/name-independent.
static bool s_hide_crosshair_on_aim_value = true;
static float s_dispersion_gui_scale_coef_value = 1.0f;

// aim FOV/near-plane transition duration, passed to player::set_target_fov_factor in
// instant_aim_start/end (the [s_aim_transition_time] float-pool memload). sushi@TODO:
// seed unrecoverable from asm (data section); 0.3f matches the documented aim transition.
static float s_aim_transition_time = 0.3f;
static vostok::console_commands::cc_float dispersion_magic_coef_cc( "dispersion_magic_coef", s_dispersion_gui_scale_coef_value, 0.0f, 10000.0f, true, vostok::console_commands::command_type_user_specific );	// sushi@TODO: name string + min/max source unverified

namespace survarium {

// shared light-id counter (also in object_light.cpp); each light grabs ++light_ids.
static u32 light_ids = 1000000;

// claude@NOTE: STUB-grade structure (6 target stmts vs our many). The target emits the
// member-init/light_props setup as ONE batched /Od statement (line 86, 0x12d bytes,
// declaration-order writes) then 5 small ones (88-90 = attenuation_power; 98/99 =
// m_barrel/m_scope = identity). Walled structurally: m_weapon_fire_light_props is built
// field-by-field here, but render::light_props has NO out-of-line ctor in our headers
// (sushi@TODO in light_props.h) so we cannot reproduce the single batched statement - each
// field assignment becomes its own line record (16 stmts). light_ids is a shared global
// (file-static here + object_light.cpp, ?light_ids@survarium@@3IA via ICF). Field VALUES are
// recovered (range=5, color=color_rgba(1,1,1,1)=0xFFFFFFFF, attenuation=2, intensity=1,
// diffuse/specular=1, does_cast_shadows=true, type=point, anim_length const=200). Recovers
// once the render-facade light_props ctor lands and the init can collapse.
// STATE[STUB]
 weapon::weapon(
	const u32		first_view_death_animations_count,
	const u32		third_view_death_animations_count,
	u32				preview_animations_count
) :
	m_fire_light_anim_length				( 200 ),
	m_first_view_death_animations_count		( first_view_death_animations_count ),
	m_third_view_death_animations_count		( third_view_death_animations_count ),
	m_preview_animations_count				( preview_animations_count )
{
	m_fire_pfx_list			= NULL;
	m_shells_pfx_list		= NULL;
	m_fire_pfx_count		= 0;
	m_shells_pfx_count		= 0;
	m_current_shell_pfx_id	= 0;
	m_current_fire_pfx_id	= 0;
	model					= NULL;
	m_game_ui				= NULL;
	m_rifle_scope			= NULL;
	m_weapon_fire_light_id	= ++light_ids;
	m_game_scene			= NULL;
	m_firing_light_added	= false;
	m_is_in_scene			= false;
	m_is_scope_aimed		= false;

	m_weapon_fire_light_props.transform					= weapon_core::m_transform;
	m_weapon_fire_light_props.local_light_z_bias		= 0.0f;
	m_weapon_fire_light_props.shadow_transparency		= 0.0f;
	m_weapon_fire_light_props.range						= 5.0f;
	m_weapon_fire_light_props.sun_shadow_map_size		= 0;
	m_weapon_fire_light_props.shadow_map_size_index		= 0;
	m_weapon_fire_light_props.num_sun_cascades			= 0;
	m_weapon_fire_light_props.shadow_distribution_sides[ 0 ]	= false;
	m_weapon_fire_light_props.shadow_distribution_sides[ 1 ]	= false;
	m_weapon_fire_light_props.shadow_distribution_sides[ 2 ]	= false;
	m_weapon_fire_light_props.shadow_distribution_sides[ 3 ]	= false;
	m_weapon_fire_light_props.shadow_distribution_sides[ 4 ]	= false;
	m_weapon_fire_light_props.shadow_distribution_sides[ 5 ]	= false;
	m_weapon_fire_light_props.does_cast_shadows			= true;
	m_weapon_fire_light_props.lighting_model			= 0;
	m_weapon_fire_light_props.color						= math::color_rgba( 1.0f, 1.0f, 1.0f, 1.0f );

	m_weapon_fire_light_props.attenuation_power			= 2.0f;

	m_weapon_fire_light_props.intensity					= 1.0f;
	m_weapon_fire_light_props.type						= render::light_type_point;
	m_weapon_fire_light_props.spot_umbra_angle			= 0.0f;
	m_weapon_fire_light_props.spot_penumbra_angle		= 0.0f;
	m_weapon_fire_light_props.spot_falloff				= 0.0f;
	m_weapon_fire_light_props.diffuse_influence_factor	= 1.0f;
	m_weapon_fire_light_props.specular_influence_factor	= 1.0f;
	m_weapon_fire_light_props.shadower					= false;
	m_weapon_fire_light_props.use_with_lpv				= false;

	m_barrel_transform = float4x4( ).identity( );
	m_scope_transform = float4x4( ).identity( );
}

 weapon::~weapon( )
{
	if ( get_game_scene( ) && get_game_scene( )->render_scene( ) )
	{
		render::scene_ptr const& render_scene = get_game_scene( )->render_scene( );
		render::scene_renderer& scene = get_game_scene( )->renderer( ).scene( );

		for ( u8 i = 0; i < m_fire_pfx_count; ++i )
			if ( particle::is_playing( m_fire_pfx_list[ i ] ) )
				scene.remove_particle_system_instance( render_scene, m_fire_pfx_list[ i ] );

		for ( u8 i = 0; i < m_shells_pfx_count; ++i )
			if ( particle::is_playing( m_shells_pfx_list[ i ] ) )
				scene.remove_particle_system_instance( render_scene, m_shells_pfx_list[ i ] );
	}
}

void weapon::set_fire_bullet_transform( float4x4 const& transform )
{
	weapon_core::set_fire_bullet_transform( m_is_third_view ? m_barrel_transform : transform );
}

void weapon::instant_aim_start( )
{
	weapon_core::instant_aim_start( );

	if ( m_game_ui )
	{
		user( ).set_target_fov_factor( m_rifle_scope ? m_rifle_scope->fov_factor( ) : m_aim_fov_factor, s_aim_transition_time );
		user( ).set_near_plane_factor( m_rifle_scope ? m_rifle_scope->near_plane_factor( ) : m_aim_near_plane_factor );
		user( ).get_input_handler( ).set_key_binder_context( 4 );
	}
}

void weapon::instant_aim_end( )
{
	weapon_core::instant_aim_end( );

	if ( m_game_ui )
	{
		user( ).set_target_fov_factor( 1.0f, s_aim_transition_time );
		user( ).get_input_handler( ).set_key_binder_context( 1 );
	}
}

void weapon::tick( )
{
	weapon_core::tick( );
}

// claude@NOTE: structure correct (single statement = the base call at line 187). The target
// INLINES the trivial in-header weapon_core::set_transform (`m_transform = transform`) directly
// into a `rep movsd` of 0x10 dwords to [this+0x158], so its body is byte-identical to (and ICF-
// adjacent) weapon_core::set_transform; our per-TU build keeps the qualified call and tail-jmps
// it (weapon_core::set_transform is a VIRTUAL whose body is emitted as an addressable COMDAT for
// the vtable, which /O2 declines to inline at the call site - only the whole-program optimizer
// inlines it). m_transform is private to weapon_core so the assignment cannot be spelled here.
// Inline-vs-call LTCG wall, not source-steerable.
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

std::pair< animation::mixing::expression, animation::mixing::animation_lexeme > weapon_user_dead_state::selected_animations(
	mutable_buffer&							buffer,
	weapon_animation_parameters const&		weapon_parameters,
	const bool								is_third_view
) const
{
	VOSTOK_UNREFERENCED_PARAMETER( weapon_parameters );

	// first-view animations lead the trailing array, third-view ones follow
	u32 const death_animation_index =
		is_third_view
			? m_weapon.first_view_death_animations_count( ) + m_random.random( m_weapon.third_view_death_animations_count( ) )
			: m_random.random( m_weapon.first_view_death_animations_count( ) );

	animation::mixing::animation_lexeme	death_lexeme(
		animation::mixing::animation_lexeme_parameters(
			buffer,
			0,
			m_weapon.death_animations( )[ death_animation_index ],
			0,
			0
		)
		.time_calculator		( &freeze_at_end_time_calculator )
		.animated_object		( m_user )
	);

	return std::make_pair< animation::mixing::expression, animation::mixing::animation_lexeme >(
		death_lexeme,
		death_lexeme
	);
}

bool weapon_user_dead_state::is_ready_for_transition( ) const
{
	return false;
}

void weapon::load_weapon(
	render::skeleton_model_ptr const&	base_model,
	rifle_scope_ptr const&		rifle_scope
)
{
	m_rifle_scope = rifle_scope;
	model = base_model;

	model->m_render_model->get_locator( "barrel_point", m_barrel_locator );
	model->m_render_model->get_locator( "scope_point", m_scope_locator );

	player_logic_base_state* const dead = VOSTOK_NEW_IMPL( g_allocator, weapon_user_dead_state )( *this );

	m_user_animations_selector.logic( ).add_state( dead );

	for ( ai::fsm_state* i = m_user_animations_selector.logic( ).states( ).front( ); i; i = i->next )
	{
		m_user_animations_selector.logic( ).add_transition( i, dead, boost::bind( &is_dead, boost::ref( m_user ) ) );
		m_user_animations_selector.logic( ).add_transition( dead, i, boost::bind( &is_alive, boost::ref( m_user ) ) );
	}

	player_logic_base_state* const preview = VOSTOK_NEW_IMPL( g_allocator, player_logic_preview_state )(
		( resources::managed_resource_ptr* )( this + 1 ) + m_first_view_death_animations_count + m_third_view_death_animations_count,
		m_preview_animations_count,
		m_user_animations_selector
	);
	m_user_animations_selector.logic( ).add_state( preview );
}

// claude@NOTE: structure faithful (static add, if-guard, the two returns exactly as the target
// records lines 290/292/293/296+298+296). The target keeps BOTH return paths fully expanded with
// duplicated epilogues (frame sub esp,0xC8, a float4x4 stack temp per branch) so each return body
// + its `}` epilogue is a distinct statement (6 stmts); our build cross-jump/tail-merges the two
// returns through a shared final mul4x3 (`jmp short .3`) into a tighter frame (sub esp,0x80),
// collapsing to 4 statements. Tail-merge / epilogue-sharing codegen difference, not source-steerable.
float4x4 weapon::calculate_locator(
	render::model_locator_item const&		locator,
	float4x4 const*							matrices,
	const u32								matrices_count
)
{
	static float4x4 add = math::create_rotation_y( math::pi );

	if ( locator.m_bone == 0xffff )
		return math::mul4x3( math::mul4x3( locator.m_offset, add ), weapon_core::m_transform );

	return math::mul4x3( math::mul4x3( matrices[ locator.m_bone ], weapon_core::m_transform ), math::mul4x3( locator.m_offset, add ) );
}

void weapon::on_show( )
{
	m_is_in_scene = true;
	render::scene_ptr scene = get_game_scene( )->render_scene( );

	get_game_scene( )->renderer( ).scene( ).add_model( scene, model->m_render_model, get_transform( ) );

	if ( m_rifle_scope )
		get_game_scene( )->renderer( ).scene( ).add_model( scene, m_rifle_scope->idle_model( )->m_render_model, get_transform( ) );

	if ( m_game_ui )
	{
		m_game_ui->set_ammo_type( (u8)( m_ammo_slot != get_ammo_slot( first_ammo ) ) + 1 );
		m_game_ui->set_fire_queue_size( m_weapon_fire_queue_types[ m_fire_queue_type ] );
		m_game_ui->show_ammo_indicator( true );

		if ( m_game_ui )
			m_game_ui->show_crosshair( true );

		set_ui_ammo( true );
		user( ).get_input_handler( ).set_key_binder_context( 1 );
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

	m_is_scope_aimed = false;
	if ( m_game_ui )
		m_game_ui->show_crosshair( false );

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

// claude@NOTE: structure correct (single statement = set_ui_ammo( true )). The target emits
// `mov eax,ecx; push 1; call set_ui_ammo` because its set_ui_ammo takes `this` in eax (LTCG
// custom convention); our set_ui_ammo is standard __thiscall (this in ecx) so we skip the
// `mov eax,ecx` and the call lands in tail position (attributed to no line). Calling-convention
// LTCG wall on the set_ui_ammo callee, not source-steerable.
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

// claude@NOTE: show_crosshair/hide_crosshair structure correct (the if-guard + the inner call).
// The target shows 2 statements (if-line + call-line) because its game_world_ui::show_crosshair
// uses an LTCG custom register convention (`this` in edx, bool in al, plain `ret`) which makes the
// weapon wrapper reserve a frame (`push ecx`/`pop ecx`), and that frame splits the prologue `{`
// from the if-test (giving the if its own line record). Our build's game_world_ui::show_crosshair
// is standard __thiscall (this in ecx, bool pushed, `ret 4`), so our wrapper is leaf-minimal (no
// frame) and the `{`/if-test collapse to one statement. game_world_ui::show_crosshair is a
// non-trivial out-of-line Invoke wrapper in another TU; its convention is whole-program-chosen.
// Inline/calling-convention LTCG wall, not source-steerable.
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

// claude@NOTE: the 2nd guard is ONE statement target-side (if+body on one line); split
// across two lines it emitted a spurious BASE_ONLY body row (4/5). Residual is the same
// set_ammo_in_magazine LTCG register-arg / tail-jmp wall as on_chamber_a_round (16-bit
// `add ax,[47A]` + tail-call vs our 32-bit add + push/call); not source-steerable.
void weapon::on_after_fire( )
{
	if ( m_game_ui )
		m_game_ui->set_ammo_in_magazine( m_ammo_in_magazine );

	play_weapon_fire_pfx( );

	if ( m_game_ui && m_inventory ) m_game_ui->set_ammo_in_magazine( ( m_is_round_chambered != 0 ) + m_ammo_in_magazine );
}

// claude@NOTE: structure correct - both sides emit 9 statements (the match.db SPLIT flag is a
// line-number-shift artifact, not a count mismatch; --view structure-diff pairs every statement
// with only SIZE deltas). Residual is codegen: the target aligns the stack (`and esp,0FFFFFFF8h`
// + a reserved slot) and uses ebx as `this`, our build uses ebp with no alignment, plus minor
// register-allocation differences across the add_light/remove_light arg-eval. old_target is a real
// source local that the optimizer register-allocates into esi (its name drops in the optimized
// game-module PDB - do not delete it). Codegen/frame difference, not source-steerable.
void weapon::set_target( const weapon_targets new_target )
{
	weapon_targets old_target = m_target;

	weapon_core::set_target( new_target );

	if ( m_target == weapon_target_fire )
	{
		if ( old_target != m_target )
		{
			m_weapon_fire_light_props.transform = m_barrel_transform;
			get_game_scene( )->renderer( ).scene( ).add_light( get_game_scene( )->render_scene( ), m_weapon_fire_light_id, &m_weapon_fire_light_props );
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
		get_game_scene( )->renderer( ).scene( ).update_light( get_game_scene( )->render_scene( ), m_weapon_fire_light_id, &m_weapon_fire_light_props );
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

void weapon::activate( base_player& user, engine& engine )
{
	if ( static_cast< player& >( user ).is_demo_player( ) )
		m_user_animations_selector.set_player_logic_initial_state(
			static_cast_checked< player_logic_base_state* >( m_user_animations_selector.logic( ).states( ).back( ) )
		);

	m_game_scene = static_cast< base_game_scene* >( &engine );
	weapon_core::activate( user, engine );

	m_left_toe_bone_index = user.skeleton( ).get_bone_index( "LeftFoot" ) - user.skeleton( ).get_root_bones_count( );
	m_right_toe_bone_index = user.skeleton( ).get_bone_index( "RightFoot" ) - user.skeleton( ).get_root_bones_count( );

	set_animation_callback( "sound_events", get_user( ), boost::bind( &weapon::on_foot_step, this, _1 ) );
	set_animation_callback( "shell_extraction", get_user( ), boost::bind( &weapon::on_shell_extraction_event, this, _1 ) );
	set_animation_callback( "left_hand_corrector", get_user( ), boost::bind( &weapon::on_hand_correction_event, this, _1, fingers_to_weapon_corrector::left ) );
	set_animation_callback( "right_hand_corrector", get_user( ), boost::bind( &weapon::on_hand_correction_event, this, _1, fingers_to_weapon_corrector::right ) );

	m_fingers_corrector.activate_hand( fingers_to_weapon_corrector::left, true, m_game_scene->get_game( ).game_time_ms( ) );
	m_fingers_corrector.activate_hand( fingers_to_weapon_corrector::right, true, m_game_scene->get_game( ).game_time_ms( ) );
	m_fingers_corrector.activate( user.skeleton( ), model->m_render_model, m_game_ui != NULL );
}

// The target inlines remove_animation_callback at these call sites.
void weapon::deactivate( )
{
	remove_animation_callback( "sound_events", get_user( ) );
	remove_animation_callback( "shell_extraction", get_user( ) );
	remove_animation_callback( "left_hand_corrector", get_user( ) );
	remove_animation_callback( "right_hand_corrector", get_user( ) );

	weapon_core::deactivate( );
}

animation::callback_return_type_enum weapon::on_foot_step( animation::animation_callback_params& params )
{
	if ( params.animated_object == get_user( ) && ( params.domain_data == 5 || params.domain_data == 6 ) )
	{
		float4x4 const& toe = params.domain_data == 5 ? m_left_toe_transform : m_right_toe_transform;

		if ( !user( ).is_demo_player( ) )
		{
			game_world& world = static_cast< game_world& >( *get_game_scene( ) );
			world.get_step_manager( ).on_step( user( ), toe.c.xyz( ), toe.k.xyz( ), world );
		}
	}

	return animation::callback_return_type_call_me_again;
}

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
	u32 const time_delta = current_time_in_ms - m_last_tick_time_in_ms;
	m_last_tick_time_in_ms = current_time_in_ms;

	if ( m_firing_light_added )
	{
		m_current_fire_light_anim_time += time_delta;
		if ( m_current_fire_light_anim_time >= m_fire_light_anim_length )
		{
			m_weapon_fire_light_props.range = 0.f;
			m_current_fire_light_anim_time = 0;
		}
		else
			m_weapon_fire_light_props.range =
				( m_fire_light_anim_length - m_current_fire_light_anim_time ) * 5.f /
				m_fire_light_anim_length;

		get_game_scene( )->renderer( ).scene( ).update_light(
			get_game_scene( )->render_scene( ),
			m_weapon_fire_light_id,
			&m_weapon_fire_light_props
		);
	}

	u32 const weapon_matrices_count = weapon_matrices_end - weapon_matrices_begin;
	m_left_toe_transform = math::mul4x3( user_matrices_begin[ m_left_toe_bone_index ], user_transform );
	m_right_toe_transform = math::mul4x3( user_matrices_begin[ m_right_toe_bone_index ], user_transform );
	m_barrel_transform = calculate_locator( m_barrel_locator, weapon_matrices_begin, weapon_matrices_count );
	m_scope_transform = calculate_locator( m_scope_locator, weapon_matrices_begin, weapon_matrices_count );
	update_pfx_transform( );

	render::base_scene_ptr scene = get_game_scene( )->render_scene( );
	render::game::renderer& renderer = get_game_scene( )->renderer( );
	if ( s_draw_fire_point )
		renderer.debug( ).draw_origin( scene, m_barrel_transform, 0.5f, false );

	if ( !m_is_third_view && m_aimed )
	{
		if ( !m_is_scope_aimed && m_rifle_scope )
		{
			float const scope_fov_factor = m_rifle_scope->fov_factor( );
			float const fov_factor = user( ).fov_factor( current_time_in_ms );
			if ( ( 1.f - fov_factor ) / ( 1.f - scope_fov_factor ) >= m_rifle_scope->change_scope_factor( ) )
			{
				m_is_scope_aimed = true;
				renderer.scene( ).remove_model( scene, m_rifle_scope->idle_model( )->m_render_model );
				renderer.scene( ).add_model( scene, m_rifle_scope->aimed_model( )->m_render_model, m_scope_transform );
				if ( m_rifle_scope->hide_weapon_on_aim( ) )
				{
					renderer.scene( ).remove_model( scene, model->m_render_model );
					renderer.scene( ).set_model_visible( user( ).get_current( ).model->m_render_model, 1, 2 );
				}
			}
		}
	}
	else if ( ( !m_is_third_view || !user( ).is_alive( ) ) && m_is_scope_aimed && m_rifle_scope )
	{
		float const scope_fov_factor = m_rifle_scope->fov_factor( );
		float const fov_factor = user( ).fov_factor( current_time_in_ms );
		if ( m_rifle_scope->change_scope_factor( ) >
			( 1.f - fov_factor ) / ( 1.f - scope_fov_factor ) ||
			!user( ).is_alive( ) )
		{
			m_is_scope_aimed = false;
			renderer.scene( ).add_model( scene, m_rifle_scope->idle_model( )->m_render_model, m_scope_transform );
			renderer.scene( ).remove_model( scene, m_rifle_scope->aimed_model( )->m_render_model );
			if ( m_rifle_scope->hide_weapon_on_aim( ) )
			{
				renderer.scene( ).add_model( scene, model->m_render_model, weapon_core::m_transform );
				renderer.scene( ).set_model_visible( user( ).get_current( ).model->m_render_model, 1, 3 );
			}
		}
	}

	if ( !( m_is_scope_aimed && m_rifle_scope.c_ptr( ) && m_rifle_scope->hide_weapon_on_aim( ) ) )
	{
		renderer.scene( ).update_model( scene, model->m_render_model, weapon_transform );
		renderer.scene( ).update_skeleton( model->m_render_model, weapon_matrices_begin, weapon_matrices_count );
	}

	if ( m_rifle_scope )
	{
		if ( m_is_scope_aimed )
			renderer.scene( ).update_model( scene, m_rifle_scope->aimed_model( )->m_render_model, m_scope_transform );
		else
			renderer.scene( ).update_model( scene, m_rifle_scope->idle_model( )->m_render_model, m_scope_transform );
	}

	VOSTOK_UNREFERENCED_PARAMETER( user_matrices_end );
	VOSTOK_UNREFERENCED_PARAMETER( __formal );
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

void weapon::update_dispersion_visual_representation( )
{
	static console_commands::cc_bool s_hide_crosshair_on_aim_cc( "hide_crosshair_on_aim", s_hide_crosshair_on_aim_value, true, console_commands::command_type_user_specific, console_commands::execution_filter_general );

	if ( m_game_ui )
	{
		m_game_ui->show_crosshair( !( s_hide_crosshair_on_aim_value && m_aimed ) );

		const float crosshair_size = s_dispersion_gui_scale_coef_value / default_vertical_fov
			* get_dispersion( );
		m_game_ui->set_crosshair_size( crosshair_size );
	}
}

// claude@NOTE: structure correct (base call + 2 inlined activate_hand). The target records a
// separate statement (line 705) that hoists the left hand's is_active (`m_is_double_handed ||
// !user_is_sprinting`) into al up front, then reuses al/cl across both inlined activate_hand
// stores; our build computes each is_active just-in-time inside its own activate_hand body.
// Tried spreading the left call across source lines to coax the arg onto its own statement -
// did not split (the `||` short-circuit + inlined `if` schedule as one statement regardless).
// Pure CSE/code-motion scheduling difference, no named local on either side - not source-steerable.
void weapon::on_user_sprint( const bool user_is_sprinting )
{
	weapon_core::on_user_sprint( user_is_sprinting );

	bool const left_hand_active = m_is_double_handed || !user_is_sprinting;
	m_fingers_corrector.activate_hand( fingers_to_weapon_corrector::left,  left_hand_active,   m_last_tick_time_in_ms );
	m_fingers_corrector.activate_hand( fingers_to_weapon_corrector::right, !user_is_sprinting, m_last_tick_time_in_ms );
}

} // namespace survarium
