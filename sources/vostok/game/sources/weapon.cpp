////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "weapon.h"
#include "game_world_ui.h"

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

// STATE[STUB]
 weapon::weapon(
	const u32		first_view_death_animations_count,
	const u32		third_view_death_animations_count,
	u32				preview_animations_count
) :
	// const members must be initialized; counts map from the params,
	// m_fire_light_anim_length is a buildability placeholder (matcher supplies real value)
	m_fire_light_anim_length				( 0 ),
	m_first_view_death_animations_count		( first_view_death_animations_count ),
	m_third_view_death_animations_count		( third_view_death_animations_count ),
	m_preview_animations_count				( preview_animations_count )
{
	// FUNCTION BODY[0x5c2100]: 30
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
	// <0x5c2169>|0x069|+0x12d:'86'
	// <0>
	// <0x5c2296>|0x196|+0x008:'88'
	// <0x5c229e>|0x19e|+0x010:'89'
	// <0x5c22ae>|0x1ae|+0x003:'90'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x5c22b1>|0x1b1|+0x060:'98'
	// <0x5c2311>|0x211|+0x00f:'99'
	// ******
}

// STATE[STUB]
 weapon::~weapon( )
{
	// LOCALS
	// render::scene_renderer& 			scene
	// ******

	// FUNCTION BODY[0x5c1f60]: 19
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5c1f66>|0x006|+0x037:'109'
	// <0>
	// <1>
	// <0x5c1f9d>|0x03d|+0x00f:'112'
	// <0>
	// <1>
	// <0x5c1fac>|0x04c|+0x014:'115'
	// <0x5c1fc0>|0x060|+0x01c:'116'
	// <0x5c1fdc>|0x07c|+0x022:'117'
	// <0>
	// <0x5c1ffe>|0x09e|+0x012:'119'
	// <0x5c2010>|0x0b0|+0x01c:'120'
	// <0x5c202c>|0x0cc|+0x022:'121'
	// <0>
	// ******
}

// claude@NOTE: selects m_barrel_transform when third-view else the param, then
// forwards to weapon_core::set_fire_bullet_transform. Blocked: the third-view
// test reads m_is_third_view via weapon_core::is_third_view(), whose inline body
// in weapon_core.h (owned by game_core) is an empty `/* no source */` stub -
// calling it force-codegens the stub (C4716). Parked until that accessor lands.
// STATE[STUB]
void weapon::set_fire_bullet_transform( float4x4 const& transform )
{
	// FUNCTION BODY[0x5c1650]: 5
}

// STATE[STUB]
void weapon::instant_aim_start( )
{
	// FUNCTION BODY[0x5c1840]: 9
	// <0x5c1843>|0x003|+0x005:'144'
	// <0>
	// <0x5c1848>|0x008|+0x00d:'146'
	// <0>
	// <1>
	// <0x5c1855>|0x015|+0x05b:'149'
	// <0x5c18b0>|0x070|+0x042:'150'
	// <0>
	// <0x5c18f2>|0x0b2|+0x017:'152'
	// ******
}

// STATE[STUB]
void weapon::instant_aim_end( )
{
	// FUNCTION BODY[0x5c1670]: 9
	// <0x5c1673>|0x003|+0x005:'157'
	// <0>
	// <0x5c1678>|0x008|+0x009:'159'
	// <0>
	// <1>
	// <0x5c1681>|0x011|+0x03e:'162'
	// <0>
	// <1>
	// <0x5c16bf>|0x04f|+0x017:'165'
	// ******
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
	// claude@NOTE: parked - real body builds a death_lexeme + expression pair from
	// the weapon animation parameters. VOSTOK_UNREACHABLE_CODE is the buildability
	// device since the pair element types have no default ctor (empty_hands precedent).
	VOSTOK_UNREACHABLE_CODE( );

	// LOCALS
	// animation::mixing::animation_lexeme death_lexeme
	// ******

	// FUNCTION BODY[0xab400]: 23
	// <0>
	// <1>
	// <2>
	// <0xab40e>|0x00e|+0x00d:'229'
	// <0xab41b>|0x01b|+0x01f:'230'
	// <0xab43a>|0x03a|+0x002:'231'
	// <0xab43c>|0x03c|+0x015:'232'
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
	// <0xab451>|0x051|+0x12f:'247'
	// <0xab580>|0x180|+0x03e:'248'
	// ******
}

// STATE[STUB]
bool weapon_user_dead_state::is_ready_for_transition( ) const
{
	return false;

	// FUNCTION BODY[0x22700]
	// <0x22700>|0x000|      :'254'	{
	// ******
}

// STATE[STUB]
void weapon::load_weapon(
	render::skeleton_model_ptr const&	base_model,
	rifle_scope_ptr const&		rifle_scope
)
{
	// LOCALS
	// player_logic_base_state* const 	dead
	// ******

	// CALL SITE INFO
	// <0x5c1c7d> -> bool < unknown >( pcstr, render::model_locator_item& ) const
	// <0x5c1c9c> -> bool < unknown >( pcstr, render::model_locator_item& ) const
	// <0x5c1d7b> -> < unknown >
	// <0x5c1dfb> -> < unknown >
	// ******

	// FUNCTION BODY[0x5c1bd0]: 20
	// <0x5c1bd0>|0x000|+0x006:'265'	{
	// <0x5c1bd6>|0x006|+0x047:'266'
	// <0x5c1c1d>|0x04d|+0x043:'267'
	// <0>
	// <0x5c1c60>|0x090|+0x01f:'269'
	// <0x5c1c7f>|0x0af|+0x01f:'270'
	// <0>
	// <0x5c1c9e>|0x0ce|+0x038:'272'
	// <0>
	// <1>
	// <2>
	// <0x5c1cd6>|0x106|+0x016:'276'
	// <0>
	// <0x5c1cec>|0x11c|+0x11c:'278'
	// <0x5c1e08>|0x238|-0x108:'278'
	// <0x5c1d00>|0x130|+0x080:'279'
	// <0x5c1d80>|0x1b0|+0x093:'280'
	// <0>
	// <1>
	// <0x5c1e13>|0x243|+0x00c:'283'
	// <0x5c1e1f>|0x24f|+0x067:'284'
	// <0x5c1e86>|0x2b6|-0x017:'284'
	// <0x5c1e6f>|0x29f|+0x019:'285'
	// <0x5c1e88>|0x2b8|-0x00a:'285'
	// <0x5c1e7e>|0x2ae|+0x012:'286'
	// <0x5c1e90>|0x2c0|      :'286'	}
	// ******
}

// STATE[STUB]
float4x4 weapon::calculate_locator(
	render::model_locator_item const&		locator,
	float4x4 const*							matrices,
	const u32								matrices_count
)
{
	// STATICS
	// static float4x4 					add = <0x4c272a8>;
	// ******

	return vostok::math::float4x4();

	// FUNCTION BODY[0x5c1750]: 8
	// <0x5c1750>|0x000|+0x01a:'289'	{
	// <0x5c176a>|0x01a|+0x02a:'290'
	// <0>
	// <0x5c1794>|0x044|+0x004:'292'
	// <0x5c1798>|0x048|+0x045:'293'
	// <0>
	// <1>
	// <0x5c17dd>|0x08d|+0x00f:'296'
	// <0x5c17ec>|0x09c|-0x00d:'296'
	// <0>
	// <0x5c17df>|0x08f|+0x04a:'298'
	// <0x5c1829>|0x0d9|      :'298'	}
	// ******
}

// STATE[STUB]
void weapon::on_show( )
{
	// LOCALS
	// render::base_scene_ptr 			scene
	// ******

	// FUNCTION BODY[0x5c2460]: 18
	// <0>
	// <1>
	// <2>
	// <0x5c246f>|0x00f|+0x030:'305'
	// <0x5c249f>|0x03f|+0x043:'306'
	// <0x5c24e2>|0x082|+0x013:'307'
	// <0x5c24f5>|0x095|+0x03e:'308'
	// <0>
	// <0x5c2533>|0x0d3|+0x009:'310'
	// <0>
	// <0x5c253c>|0x0dc|+0x021:'312'
	// <0x5c255d>|0x0fd|+0x01c:'313'
	// <0x5c2579>|0x119|+0x00d:'314'
	// <0x5c2586>|0x126|+0x011:'315'
	// <0x5c2597>|0x137|+0x009:'316'
	// <0>
	// <0x5c25a0>|0x140|+0x016:'318'
	// <0>
	// ******
}

// STATE[STUB]
void weapon::on_hide( )
{
	// LOCALS
	// render::base_scene_ptr 			scene
	// ******

	// FUNCTION BODY[0x5c2a40]: 19
	// <0>
	// <1>
	// <0x5c2a4c>|0x00c|+0x032:'326'
	// <0>
	// <0x5c2a7e>|0x03e|+0x01c:'328'
	// <0x5c2a9a>|0x05a|+0x02d:'329'
	// <0>
	// <0x5c2ac7>|0x087|+0x013:'331'
	// <0x5c2ada>|0x09a|+0x007:'332'
	// <0x5c2ae1>|0x0a1|+0x012:'333'
	// <0x5c2af3>|0x0b3|+0x002:'334'
	// <0x5c2af5>|0x0b5|+0x022:'335'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5c2b17>|0x0d7|+0x018:'340'
	// <0x5c2b2f>|0x0ef|+0x00a:'341'
	// <0x5c2b39>|0x0f9|+0x007:'342'
	// ******
}

// STATE[STUB]
void weapon::set_ui_ammo( bool update_total_count )
{
	// FUNCTION BODY[0x5c1ac0]: 14
	// <0x5c1ac5>|0x005|+0x01c:'347'
	// <0>
	// <1>
	// <0x5c1ae1>|0x021|+0x012:'350'
	// <0x5c1af3>|0x033|+0x008:'351'
	// <0>
	// <0x5c1afb>|0x03b|+0x00b:'353'
	// <0>
	// <1>
	// <0x5c1b06>|0x046|+0x006:'356'
	// <0x5c1b0c>|0x04c|+0x029:'357'
	// <0x5c1b35>|0x075|+0x029:'358'
	// <0>
	// <0x5c1b5e>|0x09e|+0x026:'360'
	// ******
}

void weapon::on_reload( )
{
	set_ui_ammo( true );
}

// STATE[STUB]
void weapon::on_chamber_a_round( )
{
	// FUNCTION BODY[0x5c1f10]: 1
	// <0x5c1f10>|0x000|+0x02d:'370'
	// ******
}

// STATE[STUB]
void weapon::on_unload_chambered_round( )
{
	// FUNCTION BODY[0x5c1ee0]: 1
	// <0x5c1ee0>|0x000|+0x02d:'375'
	// ******
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

// claude@NOTE: the second UI update needs weapon_core inline reads of
// m_ammo_in_magazine (0x47A) + m_is_round_chambered (0x48E); both are private to
// weapon_core (no inline getter in weapon_core.h, owned by game_core), so the
// (is_round_chambered() + ammo_in_magazine()) chambered-count and the [esi+10C]
// guard cannot be spelled here. Parked - structure recoverable once those inline
// accessors land in weapon_core.h.
void weapon::on_after_fire( )
{
	// FUNCTION BODY[0x5c2410]: 6
	// <0x5c2413>|0x003|+0x00a:'396'
	// <0x5c241d>|0x00d|+0x00c:'397'
	// <0>
	// <0x5c2429>|0x019|+0x005:'399'
	// <0>
	// <0x5c242e>|0x01e|+0x02f:'401'
	// ******
}

// STATE[STUB]
void weapon::set_target(
	const weapon_targets	arg_0 /* weapon_targets new_target */
)
{
	// FUNCTION BODY[0x5c2c20]: 15
	// <0x5c2c20>|0x000|+0x007:'405'	{
	// <0>
	// <0x5c2c27>|0x007|+0x014:'407'
	// <0x5c2c3b>|0x01b|+0x006:'408'
	// <0>
	// <0x5c2c41>|0x021|+0x00d:'410'
	// <0>
	// <1>
	// <0x5c2c4e>|0x02e|+0x03c:'413'
	// <0x5c2c8a>|0x06a|+0x010:'414'
	// <0>
	// <0x5c2c9a>|0x07a|+0x00e:'416'
	// <0>
	// <0x5c2ca8>|0x088|+0x026:'418'
	// <0x5c2cce>|0x0ae|-0x03d:'419'
	// <0>
	// <0x5c2c91>|0x071|+0x044:'421'
	// <0x5c2cd5>|0x0b5|      :'421'	}
	// ******
}

// claude@NOTE: the play_particle_system call boundary carries the same render-facade
// signature residual as object_particle_visual::insert - the PDB / render::engine::world_pc
// take in_instance BY VALUE so the target builds a temp particle_system_instance_ptr
// (intrusive_ptr::set), but scene_renderer.h's facade spells it const& so the base elides
// that temp. Statement shape (guard, play, ++id, wrap-to-zero) is faithful; recovers when
// the facade play_particle_system signature is corrected (its own match phase).
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

// STATE[STUB]
void weapon::update_pfx_transform( )
{
	// FUNCTION BODY[0x5c2b70]: 19
	// <0x5c2b76>|0x006|+0x00b:'461'
	// <0>
	// <0x5c2b81>|0x011|+0x006:'463'
	// <0x5c2b87>|0x017|+0x036:'464'
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
	// <0x5c2bbd>|0x04d|+0x009:'476'
	// <0x5c2bc6>|0x056|+0x01a:'477'
	// <0x5c2be0>|0x070|+0x036:'478'
	// <0>
	// ******
}

// STATE[STUB]
void weapon::set_next_fire_queue_type( )
{
	// FUNCTION BODY[0x5c16e0]: 4
	// <0x5c16e3>|0x003|+0x005:'484'
	// <0>
	// <0x5c16e8>|0x008|+0x00a:'486'
	// <0x5c16f2>|0x012|+0x018:'487'
	// ******
}

// STATE[STUB]
void weapon::set_next_ammo_type( )
{
	// FUNCTION BODY[0x5c1ea0]: 4
	// <0x5c1ea3>|0x003|+0x005:'492'
	// <0>
	// <0x5c1ea8>|0x008|+0x00a:'494'
	// <0x5c1eb2>|0x012|+0x024:'495'
	// ******
}

void weapon::on_reload_started( )
{
}

void weapon::on_ammo_empty( )
{
	if ( m_game_ui )
		m_game_ui->show_screen_message( "st_empty_ammo_message" );
}

// STATE[STUB]
void weapon::activate( base_player& user, engine& engine )
{
	// CALL SITE INFO
	// <0x5c263a> -> animation::skeleton const& < unknown >() const
	// <0x5c266f> -> animation::skeleton const& < unknown >() const
	// <0x5c26b3> -> animation::skeleton const& < unknown >() const
	// <0x5c26e8> -> animation::skeleton const& < unknown >() const
	// <0x5c2782> -> void < unknown >( pcstr, boost::function< enum animation::callback_return_type_enum( animation::animation_callback_params& ) > const&, pcvoid, resources::managed_resource_ptr const&, const u8, pcvoid const )
	// <0x5c2812> -> void < unknown >( pcstr, boost::function< enum animation::callback_return_type_enum( animation::animation_callback_params& ) > const&, pcvoid, resources::managed_resource_ptr const&, const u8, pcvoid const )
	// <0x5c28a8> -> void < unknown >( pcstr, boost::function< enum animation::callback_return_type_enum( animation::animation_callback_params& ) > const&, pcvoid, resources::managed_resource_ptr const&, const u8, pcvoid const )
	// <0x5c293e> -> void < unknown >( pcstr, boost::function< enum animation::callback_return_type_enum( animation::animation_callback_params& ) > const&, pcvoid, resources::managed_resource_ptr const&, const u8, pcvoid const )
	// <0x5c2a02> -> animation::skeleton const& < unknown >() const
	// ******

	// FUNCTION BODY[0x5c25f0]: 21
	// <0x5c25fa>|0x00a|+0x010:'510'
	// <0>
	// <1>
	// <0x5c260a>|0x01a|+0x00c:'513'
	// <0>
	// <1>
	// <0x5c2616>|0x026|+0x00e:'516'
	// <0>
	// <0x5c2624>|0x034|+0x00f:'518'
	// <0>
	// <0x5c2633>|0x043|+0x079:'520'
	// <0x5c26ac>|0x0bc|+0x073:'521'
	// <0>
	// <0x5c271f>|0x12f|+0x096:'523'
	// <0x5c27b5>|0x1c5|+0x090:'524'
	// <0x5c2845>|0x255|+0x096:'525'
	// <0x5c28db>|0x2eb|+0x09a:'526'
	// <0>
	// <0x5c2975>|0x385|+0x02e:'528'
	// <0x5c29a3>|0x3b3|+0x028:'529'
	// <0x5c29cb>|0x3db|+0x068:'530'
	// ******
}

// STATE[STUB]
void weapon::deactivate( )
{
	// CALL SITE INFO
	// <0x5c1944> -> void < unknown >( pcstr, pcvoid )
	// <0x5c1957> -> void < unknown >( pcstr, pcvoid )
	// <0x5c196a> -> void < unknown >( pcstr, pcvoid )
	// <0x5c197d> -> void < unknown >( pcstr, pcvoid )
	// ******

	// FUNCTION BODY[0x5c1930]: 5
	// <0>
	// <0x5c1933>|0x003|+0x013:'536'
	// <0x5c1946>|0x016|+0x013:'537'
	// <0x5c1959>|0x029|+0x013:'538'
	// <0x5c196c>|0x03c|+0x013:'539'
	// ******
}

// STATE[STUB]
animation::callback_return_type_enum weapon::on_foot_step( animation::animation_callback_params& params )
{
	return animation::callback_return_type_call_me_again;

	// FUNCTION BODY[0x5c1a60]: 13
	// <0x5c1a60>|0x000|+0x01a:'545'
	// <0>
	// <0x5c1a7a>|0x01a|+0x012:'547'
	// <0>
	// <1>
	// <0x5c1a8c>|0x02c|+0x009:'550'
	// <0>
	// <0x5c1a95>|0x035|+0x006:'552'
	// <0x5c1a9b>|0x03b|+0x014:'553'
	// <0>
	// <1>
	// <2>
	// <0x5c1aaf>|0x04f|+0x002:'557'
	// ******
}

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
	// LOCALS
	// render::base_scene_ptr 			scene
	// const u32 						weapon_matrices_count
	// render::game::renderer& 			renderer
	// ******

	// FUNCTION BODY[0x5c2ce0]: 82
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5c2cec>|0x00c|+0x00b:'576'
	// <0>
	// <1>
	// <0x5c2cf7>|0x017|+0x015:'579'
	// <0>
	// <0x5c2d0c>|0x02c|+0x00c:'581'
	// <0x5c2d18>|0x038|+0x00a:'582'
	// <0>
	// <0x5c2d22>|0x042|+0x00b:'584'
	// <0x5c2d2d>|0x04d|+0x00a:'585'
	// <0>
	// <0x5c2d37>|0x057|+0x002:'587'
	// <0x5c2d39>|0x059|+0x036:'588'
	// <0>
	// <0x5c2d6f>|0x08f|+0x02d:'590'
	// <0>
	// <1>
	// <0x5c2d9c>|0x0bc|+0x006:'593'
	// <0>
	// <0x5c2da2>|0x0c2|+0x022:'595'
	// <0x5c2dc4>|0x0e4|+0x02f:'596'
	// <0>
	// <0x5c2df3>|0x113|+0x02b:'598'
	// <0>
	// <0x5c2e1e>|0x13e|+0x037:'600'
	// <0>
	// <0x5c2e55>|0x175|+0x008:'602'
	// <0>
	// <0x5c2e5d>|0x17d|+0x029:'604'
	// <0>
	// <1>
	// <0x5c2e86>|0x1a6|+0x01f:'607'
	// <0x5c2ea5>|0x1c5|+0x01d:'608'
	// <0>
	// <0x5c2ec2>|0x1e2|+0x01a:'610'
	// <0x5c2edc>|0x1fc|+0x027:'611'
	// <0x5c2f03>|0x223|+0x008:'612'
	// <0x5c2f0b>|0x22b|+0x01c:'613'
	// <0x5c2f27>|0x247|+0x027:'614'
	// <0>
	// <0x5c2f4e>|0x26e|+0x034:'616'
	// <0x5c2f82>|0x2a2|+0x037:'617'
	// <0>
	// <0x5c2fb9>|0x2d9|+0x013:'619'
	// <0x5c2fcc>|0x2ec|+0x02d:'620'
	// <0x5c2ff9>|0x319|+0x032:'621'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5c302b>|0x34b|+0x005:'626'
	// <0x5c3030>|0x350|+0x03f:'627'
	// <0x5c306f>|0x38f|+0x008:'628'
	// <0x5c3077>|0x397|+0x01c:'629'
	// <0x5c3093>|0x3b3|+0x03a:'630'
	// <0>
	// <0x5c30cd>|0x3ed|+0x037:'632'
	// <0x5c3104>|0x424|+0x032:'633'
	// <0x5c3136>|0x456|+0x00f:'634'
	// <0x5c3145>|0x465|+0x042:'635'
	// <0x5c3187>|0x4a7|+0x036:'636'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5c31bd>|0x4dd|+0x01c:'642'
	// <0x5c31d9>|0x4f9|+0x01f:'643'
	// <0x5c31f8>|0x518|+0x01e:'644'
	// <0>
	// <1>
	// <0x5c3216>|0x536|+0x013:'647'
	// <0x5c3229>|0x549|+0x009:'648'
	// <0x5c3232>|0x552|+0x01b:'649'
	// <0x5c324d>|0x56d|+0x002:'650'
	// <0x5c324f>|0x56f|+0x01f:'651'
	// <0>
	// ******
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

// STATE[STUB]
animation::callback_return_type_enum weapon::on_hand_correction_event(
	animation::animation_callback_params&	params,
	const fingers_to_weapon_corrector::hands_enum	hand
)
{
	// claude@NOTE: parked - updates the per-hand block of m_fingers_corrector
	// (stride 0x404 from base 0x6A0) when the event-type flag changes; needs the
	// fingers_to_weapon_corrector per-hand layout/accessor (0x6A0/0xAA0 offsets).
	return animation::callback_return_type_call_me_again;

	// FUNCTION BODY[0x5c15c0]: 5
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

// STATE[STUB]
void weapon::update_dispersion_visual_representation( )
{
	// LOCALS
	// const float 						crosshair_size
	// ******

	// STATICS
	// static console_commands::cc_bool s_hide_crosshair_on_aim_cc = <0x4c2b398>;
	// static bool 						s_hide_crosshair_on_aim_value = <0xa88623>;
	// ******

	// FUNCTION BODY[0x5c19b0]: 16
	// <0x5c19b0>|0x000|+0x001:'683'	{
	// <0>
	// <0x5c19b1>|0x001|+0x048:'685'
	// <0>
	// <0x5c19f9>|0x049|+0x00a:'687'
	// <0>
	// <0x5c1a03>|0x053|+0x01d:'689'
	// <0x5c1a20>|0x070|+0x019:'690'
	// <0x5c1a39>|0x089|+0x013:'691'
	// <0x5c1a4c>|0x09c|-0x003:'691'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x5c1a49>|0x099|+0x008:'700'
	// <0x5c1a51>|0x0a1|      :'700'	}
	// ******
}

// STATE[STUB]
void weapon::on_user_sprint( const bool user_is_sprinting )
{
	// FUNCTION BODY[0x5c1560]: 5
	// <0x5c1561>|0x001|+0x00d:'704'
	// <0x5c156e>|0x00e|+0x013:'705'
	// <0>
	// <0x5c1581>|0x021|+0x01f:'707'
	// <0x5c15a0>|0x040|+0x01c:'708'
	// ******
}

} // namespace survarium
