////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "player.h"
#include "player_creation_params.h"	// params.game_scene field
#include "base_game_scene.h"			// game_scene supplies game / scheduler
#include "game.h"						// m_game.network_client()
#include "base_network_client.h"		// network_client().is_player_current
#include "flash_text_manager.h"			// text_manager().destroy_text( m_text ) in remove
#include <vostok/game_core/game_net_defines.h>		// match_options::player_profiles[] in remove
#include "game_world_ui.h"				// m_game_ui->fill_quick_slots
#include "player_input_handler.h"		// m_local_input_controller->set_near_plane
#include "game_memory.h"				// g_allocator for circular_buffer member
#include <vostok/game_core/weapon_core.h>			// cast_weapon_core()->could_be_used (quick slots)
#include <vostok/animation/linear_interpolator.h>	// fov_factor interpolation
#include <vostok/render/facade/scene_renderer.h>	// add/remove_model
#include <vostok/physics/character_controller.h>	// physics_controller->set_crouch / jump
#include <vostok/game_core/hit_info.h>				// hit() builds a hit_info
#include <vostok/game_core/hit_initiator.h>			// initiator->id
#include <vostok/collision/animated_object.h>		// m_damage_collision->body_part_name + bone_collision_data
#include <vostok/physics/world.h>					// get_physics_world()->remove rigid body

namespace survarium {

// TU statics (compiler-generated dynamic initializers / atexit
// destructors); a matcher recovers their types/initializers from the asm.
/*
// STATE[STUB]
void `dynamic initializer for 's_player_show_animations_command''( )
{
	// FUNCTION BODY[0x7d7df0]
	// <0x7d7df0>|0x000|      :'74'	{
	// ******
}

// STATE[STUB]
void `dynamic initializer for 's_ik_translation_time_command''( )
{
	// FUNCTION BODY[0x7d7e40]
	// <0x7d7e40>|0x000|      :'75'	{
	// ******
}

// STATE[STUB]
void `dynamic initializer for 's_crosshair_size_command''( )
{
	// FUNCTION BODY[0x7d7ea0]
	// <0x7d7ea0>|0x000|      :'76'	{
	// ******
}

// STATE[STUB]
void `dynamic initializer for 's_crosshair_line_length_command''( )
{
	// FUNCTION BODY[0x7d7f00]
	// <0x7d7f00>|0x000|      :'77'	{
	// ******
}

// STATE[STUB]
void `dynamic initializer for 's_first_person_animations_command''( )
{
	// FUNCTION BODY[0x7d7f60]
	// <0x7d7f60>|0x000|      :'78'	{
	// ******
}

// STATE[STUB]
void `dynamic initializer for 's_usable_objects_detection_distance_command''( )
{
	// FUNCTION BODY[0x7d7fb0]
	// <0x7d7fb0>|0x000|      :'80'	{
	// ******
}

// STATE[STUB]
void `dynamic initializer for 's_draw_linear_speed_graph_cc''( )
{
	// FUNCTION BODY[0x7d8020]
	// <0x7d8020>|0x000|      :'81'	{
	// ******
}

// STATE[STUB]
void `dynamic initializer for 's_show_server_player_command''( )
{
	// FUNCTION BODY[0x7d8070]
	// <0x7d8070>|0x000|      :'82'	{
	// ******
}

// STATE[STUB]
void `dynamic initializer for 's_show_client_player_command''( )
{
	// FUNCTION BODY[0x7d80c0]
	// <0x7d80c0>|0x000|      :'83'	{
	// ******
}
*/

// STATE[STUB]
// init-list sources off params.game_scene (it supplies game + scheduler);
// buildability shapes - a matcher confirms the real sources.
 player::player( player_creation_params const& params ) :
	base_player( params, params.game_scene->scheduler( ) ),
	m_history( *g_allocator, 0 ),				// buildability: allocator + capacity
	m_game_scene( *params.game_scene ),
	m_game( params.game_scene->get_game( ) )
{
	// FUNCTION BODY[0x5e44b0]: 20
	// <0x5e468c>|0x1dc|+0x050:'125'
	// <0>
	// <0x5e46dc>|0x22c|+0x01f:'127'
	// <0x5e46fb>|0x24b|+0x00e:'128'
	// <0x5e4709>|0x259|+0x047:'129'
	// <0x5e4750>|0x2a0|+0x044:'130'
	// <0>
	// <0x5e4794>|0x2e4|+0x009:'132'
	// <0>
	// <1>
	// <0x5e479d>|0x2ed|+0x032:'135'
	// <0x5e47cf>|0x31f|+0x00c:'136'
	// <0>
	// <0x5e47db>|0x32b|+0x032:'138'
	// <0x5e480d>|0x35d|+0x00c:'139'
	// <0>
	// <1>
	// <0x5e4819>|0x369|+0x00f:'142'
	// <0>
	// <0x5e4828>|0x378|+0x00c:'144'
	// ******
}

// STATE[STUB]
 player::~player( )
{
	// FUNCTION BODY[0x5e4310]: 14
	// <0>
	// <0x5e4310>|0x000|+0x032:'150'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x5e4342>|0x032|+0x032:'158'
	// <0>
	// <0x5e4374>|0x064|+0x013:'160'
	// <0x5e4387>|0x077|+0x012:'161'
	// <0>
	// ******
}

// claude@NOTE: scene_renderer() cap - base_game_scene::scene_renderer() is a
// stub returning NULL (sibling unit), so our base emits a call where the target
// inlines m_game.renderer().scene(). Re-score once scene_renderer() has its body.
void player::add_models_to_scene( )
{
	render::scene_ptr scene = m_game_scene.render_scene( );

	if ( m_show_client_player )
		m_game_scene.scene_renderer( ).add_model( scene, m_current.model->m_render_model, m_current.transform );
	if ( m_show_server_player )
		m_game_scene.scene_renderer( ).add_model( scene, m_target.model->m_render_model, m_target.transform );
}

// claude@NOTE: scene_renderer() cap - base_game_scene::scene_renderer() is a
// stub returning NULL (sibling unit); see add_models_to_scene.
void player::remove_models_from_scene( )
{
	render::scene_ptr scene = m_game_scene.render_scene( );

	if ( m_show_client_player )
		m_game_scene.scene_renderer( ).remove_model( scene, m_current.model->m_render_model );
	if ( m_show_server_player )
		m_game_scene.scene_renderer( ).remove_model( scene, m_target.model->m_render_model );
}

// STATE[STUB]
void player::on_before_active_object_changed(
	interactive_object_ptr const&		current_active_object,
	interactive_object_ptr const&		target_active_object
) const
{
	// CALL SITE INFO
	// <0x5e247d> -> void < unknown >( game_world_ui* )
	// <0x5e249e> -> void < unknown >( game_world_ui* )
	// ******

	// FUNCTION BODY[0x5e2460]: 33
	// <0x5e2460>|0x000|+0x016:'201'
	// <0x5e2476>|0x016|+0x009:'202'
	// <0>
	// <0x5e247f>|0x01f|+0x013:'204'
	// <0x5e2492>|0x032|+0x00f:'205'
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
	// <19>
	// <20>
	// <21>
	// <22>
	// <23>
	// <24>
	// <25>
	// <26>
	// <27>
	// ******
}

// STATE[STUB]
void player::insert_alive( )
{
	// CALL SITE INFO
	// <0x5e30a4> -> void < unknown >( float4x4 const& )
	// <0x5e30c5> -> void < unknown >( physics::bt_rigid_body_base*, u16, u16 )
	// <0x5e3133> -> damage_model_ptr const& < unknown >() const
	// ******

	// FUNCTION BODY[0x5e3000]: 32
	// <0>
	// <0x5e300f>|0x00f|+0x010:'239'
	// <0>
	// <0x5e301f>|0x01f|+0x02b:'241'
	// <0x5e304a>|0x04a|+0x03c:'242'
	// <0>
	// <1>
	// <0x5e3086>|0x086|+0x009:'245'
	// <0>
	// <0x5e308f>|0x08f|+0x017:'247'
	// <0>
	// <1>
	// <2>
	// <0x5e30a6>|0x0a6|+0x021:'251'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5e30c7>|0x0c7|+0x031:'256'
	// <0x5e30f8>|0x0f8|+0x009:'257'
	// <0x5e3101>|0x101|+0x02b:'258'
	// <0>
	// <1>
	// <0x5e312c>|0x12c|+0x010:'261'
	// <0x5e313c>|0x13c|+0x00b:'262'
	// <0x5e3147>|0x147|+0x020:'263'
	// <0>
	// <0x5e3167>|0x167|+0x00e:'265'
	// <0x5e3175>|0x175|+0x023:'266'
	// <0>
	// <1>
	// <2>
	// ******
}

// STATE[STUB]
void player::insert( const bool is_alive )
{
	// CALL SITE INFO
	// <0x5e4cc0> -> match_options& < unknown >()
	// <0x5e4e1a> -> void < unknown >( interactive_object_ptr const&, resources::resource_ptr< interactive_object, resources::unmanaged_intrusive_base > const& ) const
	// <0x5e4e6e> -> void < unknown >( base_player&, engine& )
	// ******

	// FUNCTION BODY[0x5e4c90]: 85
	// <0>
	// <1>
	// <2>
	// <0x5e4c9d>|0x00d|+0x012:'277'
	// <0>
	// <0x5e4caf>|0x01f|+0x013:'279'
	// <0x5e4cc2>|0x032|+0x024:'280'
	// <0x5e4ce6>|0x056|+0x002:'281'
	// <0>
	// <0x5e4ce8>|0x058|+0x008:'283'
	// <0>
	// <1>
	// <0x5e4cf0>|0x060|+0x009:'286'
	// <0x5e4cf9>|0x069|+0x007:'287'
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
	// <19>
	// <20>
	// <21>
	// <22>
	// <23>
	// <24>
	// <25>
	// <26>
	// <27>
	// <28>
	// <29>
	// <0x5e4d00>|0x070|+0x00d:'318'
	// <0>
	// <0x5e4d0d>|0x07d|+0x031:'320'
	// <0x5e4d3e>|0x0ae|+0x027:'321'
	// <0x5e4d65>|0x0d5|+0x02d:'322'
	// <0>
	// <0x5e4d92>|0x102|+0x00c:'324'
	// <0>
	// <1>
	// <0x5e4d9e>|0x10e|+0x014:'327'
	// <0x5e4db2>|0x122|+0x00e:'328'
	// <0>
	// <0x5e4dc0>|0x130|+0x015:'330'
	// <0x5e4dd5>|0x145|+0x00b:'331'
	// <0x5e4de0>|0x150|+0x012:'332'
	// <0x5e4df2>|0x162|+0x009:'333'
	// <0x5e4dfb>|0x16b|+0x002:'334'
	// <0>
	// <1>
	// <0x5e4dfd>|0x16d|+0x00e:'337'
	// <0>
	// <1>
	// <0x5e4e0b>|0x17b|+0x011:'340'
	// <0x5e4e1c>|0x18c|+0x038:'341'
	// <0x5e4e54>|0x1c4|+0x01c:'342'
	// <0>
	// <0x5e4e70>|0x1e0|+0x006:'344'
	// <0x5e4e76>|0x1e6|+0x006:'345'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x5e4e7c>|0x1ec|+0x00e:'354'
	// <0>
	// <1>
	// <2>
	// <3>
	// ******
}

// claude@NOTE: structure faithful; the lone diff is a line-table fold artifact -
// the target lists `m_is_alive=false` + the first deactivate() as ONE entry-line
// statement (367, folded into the prologue), our base lists the deactivate as a
// separate body statement. Content is identical; physics deactivate() bytes are
// the bullet from_*/remove inline residual.
void player::remove_alive( )
{
	m_is_alive = false;
	m_target.physics_controller->deactivate( );

	if ( m_use_physics_controller_for_current )
		m_current.physics_controller->deactivate( );

	if ( !m_is_demo_player )
		m_game_scene.get_physics_world( )->remove( m_damage_collision->get_rigid_body( ) );
}

// claude@NOTE: full body reconstructed (unblocks lobby_menu_scene::clear_resources).
// Residual is cross-module: flash_text_manager::destroy_text is a STUB in scaleform
// (target inlines the GFx Release + flash_text zero + need_capture=1), so our line
// 403 emits `call destroy_text` vs the target's inline; plus the intrusive_ptr/
// item_in_slot accessor-inlining cap shared with the quick-slot fns.
void player::remove( )
{
	m_has_been_inserted = false;

	if ( !m_is_demo_player )
		m_game_scene.text_manager( ).destroy_text( m_text );

	if ( m_is_visible )
		hide( );

	m_current_active_object->deactivate( );
	on_before_active_object_changed( interactive_object_ptr( ), m_current_active_object );
	m_current_active_object = interactive_object_ptr( );
	m_target_active_object = interactive_object_ptr( );

	if ( m_is_alive )
		remove_alive( );

	if ( !m_is_demo_player )
		inventory( ).unload_to_profile(
			m_game.network_client( ).match_options( ).player_profiles[ id ], m_game.items_dictionary( ) );

	inventory( ).remove( );

	if ( m_game.network_client( ).is_player_current( id ) )
		m_game.network_client( ).detach_from_player( );
}

float4x4 player::get_transform_for_animation_player( pcvoid const animated_object, float4x4 const& character_transform ) const
{
	if ( animated_object == this )
		return character_transform;

	return m_current_active_object->transform( );
}

// STATE[STUB]
void player::apply_input( client_player_state& player_state, float2 const& rotation_to_apply )
{
	// FUNCTION BODY[0x5e2ce0]: 8
	// <0x5e2ce0>|0x000|+0x006:'470'	{
	// <0>
	// <1>
	// <2>
	// <0x5e2ce6>|0x006|+0x038:'474'
	// <0x5e2d1e>|0x03e|+0x056:'475'
	// <0x5e2d74>|0x094|+0x046:'476'
	// <0x5e2dba>|0x0da|-0x00d:'476'
	// <0>
	// <1>
	// <0x5e2dad>|0x0cd|+0x01d:'479'
	// <0x5e2dca>|0x0ea|      :'479'	}
	// ******
}

// STATE[STUB]
void player::apply_input(
	client_player_state&	player_state,
	float2 const&			previous_velocity,
	float2 const&			current_acceleration,
	const float				time_delta
)
{
	// LOCALS
	// float2 							rotation_to_apply
	// ******

	// FUNCTION BODY[0x5e2de0]: 21
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
	// <0x5e2de3>|0x003|+0x033:'495'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x5e2e16>|0x036|+0x018:'503'
	// ******
}

// claude@NOTE: body fully reversed (6 stmts, 1 reference local `item`):
//   client_player_history_item& item = m_history.new_item( );
//   item.time_in_ms                  = current_time_in_ms;             // +0x5C
//   item.action.input                = m_input;                       // +0x00
//   item.action.state.transform      = m_target.transform;            // +0x14
//   item.action.state.look_pitch     = m_target.look_pitch;           // +0x54
//   item.action.weapon_state.slot_id = inventory( ).get_active_slot( );// +0x58
// WALLED cross-unit: circular_buffer<T>::new_item (game_core template, in
// circular_buffer_inline.h) is an empty STUB - instantiating it for
// client_player_history_item gives C4716 (must return a value). new_item itself
// placement-news the slot's player_input + weapon_state, which needs
// client_player_history_item's ctor (also a STUB). Body new_item +
// client_player_history_item::ctor first, then this compiles & matches.
// STATE[STUB]
void player::serialize_current_state( const u32 current_time_in_ms )
{
	// FUNCTION BODY[0x5e23b0]: 6 stmts (lines 519-524) - see note above
}

// STATE[STUB]
void player::compute_bones( const u32 current_time_in_ms )
{
	// LOCALS
	// render::game::renderer& 			r
	// ******

	// CALL SITE INFO
	// <0x5e427d> -> void < unknown >( animation::skeleton_ptr const&, float4x4* const, const u32, const u32, float4x4&, float4x4&, animation::animation_player const& )
	// ******

	// FUNCTION BODY[0x5e4210]: 27
	// <0x5e4214>|0x004|+0x039:'531'
	// <0x5e424d>|0x03d|+0x00a:'532'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x5e4257>|0x047|+0x028:'541'
	// <0>
	// <0x5e427f>|0x06f|+0x00d:'543'
	// <0>
	// <1>
	// <0x5e428c>|0x07c|+0x01e:'546'
	// <0>
	// <0x5e42aa>|0x09a|+0x017:'548'
	// <0>
	// <1>
	// <2>
	// <0x5e42c1>|0x0b1|+0x009:'552'
	// <0>
	// <0x5e42ca>|0x0ba|+0x01c:'554'
	// <0x5e42e6>|0x0d6|+0x01a:'555'
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
void player::render( const u32 __formal, const u32 current_time_in_ms )
{
	// LOCALS
	// render::base_scene_ptr 			scene
	// render::trample_desc 			trample
	// ******

	// CALL SITE INFO
	// <0x5e4b25> -> void < unknown >()
	// <0x5e4c1c> -> void < unknown >( physics::bt_rigid_body_base*, float4x4 const& )
	// ******

	// FUNCTION BODY[0x5e49f0]: 68
	// <0x5e49f8>|0x008|+0x02b:'562'
	// <0>
	// <1>
	// <0x5e4a23>|0x033|+0x01b:'565'
	// <0x5e4a3e>|0x04e|+0x023:'566'
	// <0>
	// <0x5e4a61>|0x071|+0x00d:'568'
	// <0>
	// <0x5e4a6e>|0x07e|+0x006:'570'
	// <0x5e4a74>|0x084|+0x009:'571'
	// <0x5e4a7d>|0x08d|+0x01b:'572'
	// <0x5e4a98>|0x0a8|+0x002:'573'
	// <0x5e4a9a>|0x0aa|+0x021:'574'
	// <0>
	// <1>
	// <0x5e4abb>|0x0cb|+0x00d:'577'
	// <0>
	// <0x5e4ac8>|0x0d8|+0x006:'579'
	// <0x5e4ace>|0x0de|+0x009:'580'
	// <0>
	// <0x5e4ad7>|0x0e7|+0x01b:'582'
	// <0x5e4af2>|0x102|+0x008:'583'
	// <0>
	// <0x5e4afa>|0x10a|+0x002:'585'
	// <0>
	// <0x5e4afc>|0x10c|+0x021:'587'
	// <0x5e4b1d>|0x12d|+0x00a:'588'
	// <0>
	// <1>
	// <2>
	// <0x5e4b27>|0x137|+0x00d:'592'
	// <0>
	// <0x5e4b34>|0x144|+0x00d:'594'
	// <0>
	// <0x5e4b41>|0x151|+0x022:'596'
	// <0>
	// <0x5e4b63>|0x173|+0x022:'598'
	// <0>
	// <1>
	// <0x5e4b85>|0x195|+0x007:'601'
	// <0>
	// <1>
	// <2>
	// <0x5e4b8c>|0x19c|+0x039:'605'
	// <0>
	// <1>
	// <2>
	// <0x5e4bc5>|0x1d5|+0x009:'609'
	// <0x5e4bce>|0x1de|+0x022:'610'
	// <0>
	// <1>
	// <0x5e4bf0>|0x200|+0x00a:'613'
	// <0>
	// <0x5e4bfa>|0x20a|+0x024:'615'
	// <0>
	// <1>
	// <0x5e4c1e>|0x22e|+0x03c:'618'
	// <0>
	// <0x5e4c5a>|0x26a|+0x005:'620'
	// <0x5e4c5f>|0x26f|+0x002:'621'
	// <0>
	// <0x5e4c61>|0x271|+0x002:'623'
	// <0x5e4c63>|0x273|+0x007:'624'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// ******
}

// STATE[STUB]
void player::render_crosshair_info( )
{
	// LOCALS
	// physics::closest_ray_result 		result
	// float3 							ray_dir
	// float3 							ray_from
	// ******

	// CALL SITE INFO
	// <0x5e3b6b> -> physics::closest_ray_result < unknown >( float3 const&, float3 const&, const float, u16, u16 )
	// ******

	// FUNCTION BODY[0x5e3af0]: 50
	// <0x5e3af3>|0x003|+0x00b:'634'
	// <0x5e3afe>|0x00e|+0x007:'635'
	// <0>
	// <1>
	// <0x5e3b05>|0x015|+0x038:'638'
	// <0>
	// <0x5e3b3d>|0x04d|+0x030:'640'
	// <0>
	// <0x5e3b6d>|0x07d|+0x007:'642'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5e3b74>|0x084|+0x03e:'647'
	// <0x5e3bb2>|0x0c2|+0x01d:'648'
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
	// <19>
	// <20>
	// <21>
	// <22>
	// <23>
	// <24>
	// <25>
	// <26>
	// <27>
	// <28>
	// <29>
	// <30>
	// <31>
	// <32>
	// <33>
	// <34>
	// ******
}

// STATE[STUB]
void player::update_speed_info( )
{
	// LOCALS
	// const float 						last_frame_angular_displacement
	// ******

	// FUNCTION BODY[0x5e3380]: 18
	// <0>
	// <0x5e3388>|0x008|+0x030:'689'
	// <0x5e33b8>|0x038|+0x009:'690'
	// <0x5e33c1>|0x041|+0x03d:'691'
	// <0>
	// <0x5e33fe>|0x07e|+0x026:'693'
	// <0x5e3424>|0x0a4|+0x07d:'694'
	// <0>
	// <1>
	// <0x5e34a1>|0x121|+0x014:'697'
	// <0>
	// <1>
	// <2>
	// <0x5e34b5>|0x135|+0x034:'701'
	// <0>
	// <0x5e34e9>|0x169|+0x01f:'703'
	// <0x5e3508>|0x188|+0x08d:'704'
	// <0>
	// ******
}

// STATE[STUB]
void player::set_physics_controller_walk_vector( client_player_state& state )
{
	// LOCALS
	// float4x4 						physics_transform
	// ******

	// FUNCTION BODY[0x5e2f10]: 15
	// <0>
	// <0x5e2f20>|0x010|+0x01c:'723'
	// <0>
	// <1>
	// <0x5e2f3c>|0x02c|+0x01b:'726'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5e2f57>|0x047|+0x035:'731'
	// <0>
	// <1>
	// <2>
	// <0x5e2f8c>|0x07c|+0x05e:'735'
	// <0>
	// ******
}

// STATE[STUB]
void player::detect_usable_objects( const u32 current_time_in_ms )
{
	// LOCALS
	// physics::closest_ray_result 		ray_result
	// vectora< usable_object* > 		results
	// ******

	// CALL SITE INFO
	// <0x5e35ef> -> physics::closest_ray_result < unknown >( float3 const&, float3 const&, const float, u16, u16 )
	// <0x5e3605> -> collision_geometry* < unknown >()
	// <0x5e3649> -> player_input const& < unknown >() const
	// <0x5e3667> -> bool < unknown >( usable_object_user_data* )
	// <0x5e3677> -> bool < unknown >( usable_object_user_data* )
	// <0x5e3686> -> bool < unknown >( usable_object_user_data* )
	// <0x5e3690> -> bool < unknown >( usable_object_user_data* )
	// <0x5e36a0> -> bool < unknown >( usable_object_user_data* )
	// <0x5e36b4> -> bool < unknown >( usable_object_user_data* )
	// <0x5e36eb> -> pcstr < unknown >( usable_object_user_data* )
	// <0x5e3709> -> bool < unknown >( usable_object_user_data* )
	// <0x5e3720> -> bool < unknown >( usable_object_user_data* )
	// ******

	// FUNCTION BODY[0x5e35a0]: 50
	// <0x5e35a0>|0x000|+0x005:'741'	{
	// <0x5e35a5>|0x005|+0x00b:'742'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x5e35b0>|0x010|+0x041:'751'
	// <0>
	// <0x5e35f1>|0x051|+0x00c:'753'
	// <0>
	// <0x5e35fd>|0x05d|+0x00c:'755'
	// <0>
	// <0x5e3609>|0x069|+0x009:'757'
	// <0x5e3612>|0x072|+0x02a:'758'
	// <0>
	// <1>
	// <0x5e363c>|0x09c|+0x004:'761'
	// <0>
	// <0x5e3640>|0x0a0|+0x012:'763'
	// <0>
	// <0x5e3652>|0x0b2|+0x009:'765'
	// <0>
	// <1>
	// <2>
	// <0x5e365b>|0x0bb|+0x026:'769'
	// <0>
	// <1>
	// <0x5e3681>|0x0e1|+0x007:'772'
	// <0x5e3688>|0x0e8|+0x022:'773'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5e36aa>|0x10a|+0x004:'779'
	// <0x5e36ae>|0x10e|+0x008:'780'
	// <0x5e36b6>|0x116|+0x022:'781'
	// <0x5e36d8>|0x138|+0x00b:'782'
	// <0x5e36e3>|0x143|+0x00a:'783'
	// <0x5e36ed>|0x14d|-0x084:'784'
	// <0>
	// <1>
	// <0x5e3669>|0x0c9|+0x029:'787'
	// <0x5e3692>|0x0f2|+0x069:'787'
	// <0x5e36fb>|0x15b|+0x018:'787'
	// <0>
	// <0x5e3713>|0x173|-0x09c:'789'
	// <0x5e3677>|0x0d7|+0x029:'790'
	// <0x5e36a0>|0x100|+0x069:'790'
	// <0x5e3709>|0x169|+0x011:'790'
	// <0x5e371a>|0x17a|-0x0a1:'790'
	// <0>
	// <0x5e3679>|0x0d9|+0x029:'792'
	// <0x5e36a2>|0x102|+0x069:'792'
	// <0x5e370b>|0x16b|+0x017:'792'
	// <0x5e3722>|0x182|      :'792'	}
	// ******
}

void player::use_ladder( ladder* __formal )
{
	NOT_IMPLEMENTED( __formal );
}

// claude@NOTE: STRUCTURE MATCH (3 stmts). The line-869 guard `m_game.network_client().
// is_player_current(id)` inlines in the target (reads m_current_player.c_ptr() then
// compares its id at +0x34 with this->id) but our base emits the standalone
// is_player_current call; damage_model()->apply_affect carries the virtual-dispatch
// inline residual. Both are inline-vs-call caps, not source-steerable here.
void player::apply_damage_model_affect(
	pcstr							part_name,
	const hit_affects_type_enum		affect,
	const affect_event_type_enum	event_type
)
{
	damage_model( )->apply_affect( part_name, affect, event_type );

	if ( m_game_ui && m_game.network_client( ).is_player_current( id ) )
		m_game_ui->on_damage_affect_applying( part_name, affect, event_type );
}

// claude@NOTE: STRUCTURE MATCH (2 stmts). Byte residual is the create_rotation_y /
// create_translation / mul4x3 inline schedule (the chained matrix assignment's temp
// re-use) - not source-steerable from here.
void player::set_character_transform( float3 const& position, const float orientation, const float look_pitch )
{
	m_current.transform = m_current.previous_transform = m_target.transform = m_target.previous_transform =
		math::mul4x3( math::create_rotation_y( orientation ), math::create_translation( position ) );
	m_current.look_pitch = m_target.look_pitch = look_pitch;
}

void player::hit(
	hit_initiator const* const		initiator,
	const u32						bone_index,
	pcstr const						damage_type,
	const float						amount,
	const float						armor_piercing,
	bullet* const					bullet
)
{
	hit_info info(
		initiator->id, id, m_damage_collision->body_part_name( bone_index ),
		damage_type, amount, armor_piercing, bullet );
	m_game.network_client( ).on_player_hit_received( info );
}

void player::hit(
	hit_initiator const* const		initiator,
	collision::bone_collision_data const&	bone_data,
	pcstr							damage_type,
	const float						amount,
	const float						armor_piercing,
	bullet* const					bullet
)
{
	hit_info info(
		initiator->id, id, bone_data.body_part_name.c_str( ),
		damage_type, amount, armor_piercing, bullet );
	m_game.network_client( ).on_player_hit_received( info );
}

// STATE[STUB]
void player::apply_hit_directly( hit_info const& info, const u32 current_time_in_ms )
{
	// LOCALS
	// player_ptr 						initiator_ptr
	// ******

	// CALL SITE INFO
	// <0x5e3944> -> damage_model_ptr const& < unknown >() const
	// <0x5e3a3d> -> player_ptr < unknown >( const u8 ) const
	// <0x5e3a7c> -> float4x4 const& < unknown >() const
	// <0x5e3ad0> -> void < unknown >( hit_receiver const*, player_actions_subscriber::action, float )
	// ******

	// FUNCTION BODY[0x5e3920]: 19
	// <0x5e3928>|0x008|+0x04f:'930'
	// <0>
	// <1>
	// <0x5e3977>|0x057|+0x024:'933'
	// <0x5e399b>|0x07b|+0x01f:'934'
	// <0>
	// <0x5e39ba>|0x09a|+0x03d:'936'
	// <0x5e39f7>|0x0d7|+0x014:'937'
	// <0>
	// <0x5e3a0b>|0x0eb|+0x064:'939'
	// <0x5e3a6f>|0x14f|+0x032:'940'
	// <0x5e3aa1>|0x181|+0x009:'941'
	// <0>
	// <1>
	// <0x5e3aaa>|0x18a|+0x006:'944'
	// <0x5e3ab0>|0x190|+0x006:'945'
	// <0>
	// <0x5e3ab6>|0x196|+0x00a:'947'
	// <0x5e3ac0>|0x1a0|+0x019:'948'
	// ******
}

void player::reset_fov_factor( )
{
	m_start_fov_factor		= 1.0f;
	m_target_fov_factor		= 1.0f;
	m_current_fov_factor	= 1.0f;
}

// claude@NOTE: transition_time is dropped (ret 4 = 1 arg): the sole caller passes
// a constant 0.3f, LTCG const-propagates it into m_fov_factor_transition_time
// (the [s_aim_transition_time]=0.3f memload). Allowed LTCG arg-drop.
void player::set_target_fov_factor( const float target_fov_factor, const float transition_time )
{
	m_start_fov_factor						= m_current_fov_factor;
	m_fov_factor_transition_time			= transition_time;
	m_start_fov_factor_change_time_in_ms	= m_current_time_in_ms;
	m_target_fov_factor						= target_fov_factor;
}

float player::fov_factor( const u32 current_time_in_ms ) const
{
	const float time = ( current_time_in_ms - m_start_fov_factor_change_time_in_ms ) * math::epsilon_3;
	if ( time >= m_fov_factor_transition_time )
		return m_target_fov_factor;

	return m_start_fov_factor + ( m_target_fov_factor - m_start_fov_factor )
		* animation::linear_interpolator( m_fov_factor_transition_time ).interpolated_value( time );
}

// claude@NOTE: inline-vs-store cap - the [controller+0x4C] store is game_camera::
// set_near_plane( near_plane_factor * 0.05f ), inlined; game_camera::set_near_plane
// is an empty stub in game_camera.h (render cone, another unit), so our base omits
// the store. Re-score once game_camera::set_near_plane has a body.
void player::set_near_plane_factor( const float near_plane_factor )
{
	if ( m_local_input_controller )
		m_local_input_controller->set_near_plane( near_plane_factor * 0.05f );
}

// STATE[STUB]
// claude@NOTE: 18 stmts, locals interpolation_time(const float) + transform(float4x4).
// Mostly reversed:
//   if ( !m_local_input_controller ) return;                                            // 1027
//   if ( m_start_fov_factor != m_target_fov_factor ) {                                  // 1030 (ucomiss start,target)
//     const float interpolation_time = (m_current_time_in_ms - m_start_fov_factor_change_time_in_ms) * math::epsilon_3; // 1032
//     <new_fov = interpolation_time < m_fov_factor_transition_time
//          ? m_start_fov_factor + (m_target_fov_factor-m_start_fov_factor)*linear_interpolator(m_fov_factor_transition_time).interpolated_value(interpolation_time)
//          : m_target_fov_factor>;                                                       // 1033-1036 (fov_factor() inlined)
//     m_current_fov_factor = new_fov; m_local_input_controller->set_fov_factor(new_fov); // 1037 ([ctrl+0x54]=game_camera m_fov_factor)
//     if ( interpolation_time >= m_fov_factor_transition_time ) {                        // 1041
//       m_start_fov_factor = m_target_fov_factor;                                        // 1041 ([ebx+10F1Ch])
//       if ( m_local_input_controller ) m_local_input_controller->set_near_plane(<satisfaction_equality_tolerance>); // 1042 ([ctrl+0x4C]=m_near_plane)
//     }
//   } else m_local_input_controller->set_fov_factor( m_target_fov_factor );              // 1046
//   if ( m_local_input_controller->input_mode() == first_person_mode )                   // 1048 ([ctrl+0x198]==0)
//     m_local_input_controller->update_inverted_view( m_character_head_transform );      // 1049 (base_player+0x48)
//   else {                                                                               // 1054/1060
//     transform = m_root_transform; <transform.c += m_root_transform.j * 1.4f>;          // (0x3fb33333, j = m_root_transform+0x10)
//     m_local_input_controller->update_inverted_view( transform );                       // 1062
//   }
// WALL: the fov_factor() inline (linear_interpolator vtable-inlined) + the exact 18-stmt
// boundary / the near_plane const (satisfaction_equality_tolerance) need a build pass to
// pin the statement shape; deferred to keep this batch's matches clean.
// STATE[STUB]
void player::update_camera( )
{
	// FUNCTION BODY[0x5e2b20]: 18 stmts (lines 1027-1062) - see note above
}

player_input player::local_input( ) const
{
	return m_local_input_controller ? m_local_input_controller->get_input( ) : player_input( );
}

// claude@NOTE: structure matches; residual is the LTCG custom calling convention
// (this in eax, struct-return buffer in a register vs base's stack slot + ret 4) -
// an argument-passing artifact, not a source-steerable diff. Same cap on local_input.
player_input player::remote_input( ) const
{
	return m_history.empty( ) ? player_input( ) : m_history.newest( ).action.input;
}

// claude@NOTE: 3 stmts, 0 locals. Virtual dispatched via a +0x30 base subobject so
// `this`=player+0x30; `[ecx+10EC0h]` resolves to m_damage_collision (0x10EF0). Reads
// m_damage_collision's geometry data (offset 0/0x18) and two bone transforms' x/z
// positions to compute abs(dx); if abs(dx) <= math::epsilon_5 returns 0, else
// (z1-z0)/dx. WALLED: the exact accessor over collision::animated_object's
// m_geometries_data (buffer_vector<bone_collision_data>) internals is not yet a named
// expression - needs the animated_object speed/displacement accessor identified.
// STATE[STUB]
float player::get_speed( ) const
{
	return 0.0f;

	// FUNCTION BODY[0x5e26b0]: 3 stmts (lines 1076-1077) - see note above
}

void player::hide( )
{
	m_is_visible = false;
	remove_models_from_scene( );
}

void player::show( )
{
	m_is_visible = true;
	add_models_to_scene( );
}

void player::subscribe_on_actions( player_actions_subscriber* subscriber )
{
	m_player_actions_subscribers.push_back( subscriber );
}

void player::unsubscribe_from_actions( player_actions_subscriber* subscriber )
{
	m_player_actions_subscribers.erase(
		std::remove( m_player_actions_subscribers.begin( ), m_player_actions_subscribers.end( ), subscriber ),
		m_player_actions_subscribers.end( )
	);
}

// claude@NOTE: fully reversed, parked on the 0-iterator-local loop shape -
//   const float movement = math::length( m_current.transform.c.xyz() - m_last_frame_position );  // 1108
//   <loop over m_player_actions_subscribers>                                                       // 1110/1111/1113 (x3)
//     if ( math::abs( movement ) >= math::epsilon_3 )                                               // 1115 (braced)
//       if ( m_input.is_trying_to_sprint() )                                                        // 1117 (m_input.actions_mask & 0x200)
//         subscriber->on_player_action( this, player_actions_subscriber::run, movement );           // 1118 (this upcasts to hit_receiver*, lea [+0x38])
//       else                                                                                        // 1119
//         subscriber->on_player_action( this, player_actions_subscriber::walk, movement );          // 1120
// run/walk match the header enum values (1/0). The loop records NO named iterator
// local (only `movement`) -> it is std::for_each with an inlined functor, not a
// raw `for` with a named iterator. Needs the functor-struct form to match the
// 0-local structure. (NB: on_fire/jump pass action 3/2 via the SAME enum, which
// under the current header = jump/sprint - the real enum is walk0/run1/jump2/
// shoot3/character_hit4; the header's sprint=2 is mis-positioned, a game_core fix.)
// STATE[STUB]
void player::notify_actions_subscribers( )
{
	// LOCALS
	// const float 						movement
	// ******

	// CALL SITE INFO
	// <0x5e2aef> -> void < unknown >( hit_receiver const*, player_actions_subscriber::action, float )
	// <0x5e2af8> -> void < unknown >( hit_receiver const*, player_actions_subscriber::action, float )
	// ******

	// FUNCTION BODY[0x5e2a30]: 15
	// <0x5e2a30>|0x000|+0x003:'1107'	{
	// <0x5e2a33>|0x003|+0x05e:'1108'
	// <0>
	// <0x5e2a91>|0x061|+0x006:'1110'
	// <0x5e2a97>|0x067|+0x009:'1111'
	// <0>
	// <0x5e2aa0>|0x070|+0x05e:'1113'
	// <0x5e2afe>|0x0ce|+0x00e:'1113'
	// <0x5e2b0c>|0x0dc|-0x068:'1113'
	// <0>
	// <0x5e2aa4>|0x074|+0x02f:'1115'
	// <0>
	// <0x5e2ad3>|0x0a3|+0x00a:'1117'
	// <0x5e2add>|0x0ad|+0x014:'1118'
	// <0x5e2af1>|0x0c1|+0x002:'1119'
	// <0x5e2af3>|0x0c3|+0x014:'1120'
	// <0>
	// <1>
	// <0x5e2b07>|0x0d7|+0x008:'1123'
	// <0x5e2b0f>|0x0df|      :'1123'	}
	// ******
}

// STATE[STUB]
void player::on_fire( )
{
	// CALL SITE INFO
	// <0x5e235a> -> void < unknown >( hit_receiver const*, player_actions_subscriber::action, float )
	// ******

	// FUNCTION BODY[0x5e2320]: 5
	// <0x5e2324>|0x004|+0x007:'1127'
	// <0x5e232b>|0x00b|+0x006:'1128'
	// <0>
	// <0x5e2331>|0x011|+0x00f:'1130'
	// <0x5e2340>|0x020|+0x027:'1131'
	// ******
}

// STATE[STUB]
void player::jump( )
{
	// CALL SITE INFO
	// <0x5e264b> -> void < unknown >()
	// <0x5e2670> -> void < unknown >( hit_receiver const*, player_actions_subscriber::action, float )
	// ******

	// FUNCTION BODY[0x5e2640]: 11
	// <0x5e2643>|0x003|+0x00a:'1136'
	// <0x5e264d>|0x00d|+0x006:'1137'
	// <0x5e2653>|0x013|+0x00e:'1138'
	// <0x5e2661>|0x021|+0x019:'1139'
	// <0>
	// <1>
	// <0x5e267a>|0x03a|+0x00d:'1142'
	// <0x5e2687>|0x047|+0x00b:'1143'
	// <0x5e2692>|0x052|+0x00f:'1144'
	// <0>
	// <1>
	// ******
}

void player::end_jump( )
{
	m_target.physics_controller->end_jump( );
	if ( m_use_physics_controller_for_current )
		m_current.physics_controller->end_jump( );
}

void player::crouch( )
{
	m_target.physics_controller->set_crouch( true );
	if ( m_use_physics_controller_for_current )
		m_current.physics_controller->set_crouch( true );
}

void player::stand_up( )
{
	m_target.physics_controller->set_crouch( false );
	if ( m_use_physics_controller_for_current )
		m_current.physics_controller->set_crouch( false );
}

// claude@NOTE: structure matches (4 stmts, lines 1190/1191/1194/1195) but capped
// on intrusive_ptr<interactive_object> inline-vs-call: the target inlines c_ptr()
// and operator=(object_type*) (addref/release as inline lock-xadd), our base
// out-lines them (calls). That inlining is a core-module template-instantiation
// decision not steerable from player.cpp. Byte residual only.
bool player::set_new_active_item( inventory_item_ptr const& item )
{
	if ( m_target_active_object != item.c_ptr( ) )
		force_animation_selection( );

	m_target_active_object = item.c_ptr( );
	return true;
}

static const profile_slot_enum accept_slots[] = {
	quick_slot1, quick_slot2, quick_slot3, quick_slot4, quick_slot5, quick_slot6
};

// claude@NOTE: structure mostly matches (for/if-occupied/set_item aligned to
// target lines 1227/1229/1231). The local-player UI guard (target line 1231) is
// m_game.network_client().is_player_current( id ) [== the standalone
// base_network_client::is_player_current, 99.6%: reads m_current_player, c_ptr,
// cmp id at +0x34] and the call (1232) is m_game_ui->fill_quick_slots(). BUT the
// guard + UI call + trailing return are NOT being emitted by our compile (base
// ends at set_item; ret) - the if-block vanishes with no /Od reason found yet.
// Loop check item_in_slot is also intrusive_ptr inline-vs-call capped. Park: find
// why the post-set_item guard block is dropped before banking. STATE[STUB]
void player::take_inventory_item( inventory_item_ptr const& item )
{
	for ( u32 i = 0; i < array_size( accept_slots ); ++i )
	{
		if ( inventory( ).item_in_slot( accept_slots[ i ] ) )
			continue;

		inventory( ).set_item( accept_slots[ i ], item );

		if ( m_game_ui && m_game.network_client( ).is_player_current( id ) )
			m_game_ui->fill_quick_slots( );

		return;
	}
}

// claude@NOTE: structure correct; residual is the intrusive_ptr c_ptr() out-of-line
// cap - target inlines to `mov eax,[eax+10Ch]`, our base emits `add ecx,10Ch; jmp
// c_ptr`. Same accessor-inlining wall as the quick-slot fns; not TU-steerable.
animation::skeleton const& player::skeleton( ) const
{
	return *m_current.model->m_skeleton.c_ptr( );
}

// claude@NOTE: STRUCTURE MATCH (6 stmts). Byte residual is the bt_character_controller
// activate()/get_transform() inline (the bullet from_vostok/from_bullet roundtrip) -
// the same physics-conversion wall as player_tick's set_transform tails.
void player::set_use_physics_controller_for_current( const bool value )
{
	if ( m_use_physics_controller_for_current == value )
		return;

	m_use_physics_controller_for_current = value;
	if ( value )
		m_current.physics_controller->activate( m_target.physics_controller->get_transform( ) );
	else
		m_current.physics_controller->deactivate( );
}

engine& player::get_engine( )
{
	return m_game_scene;
}

// claude@NOTE: weapon-swap blocks (0x1000/0x2000) + 6 quick-slot blocks + back_slot,
// driven by remote_input() flag bits (raw masks - no named bit enum in this engine).
// Residual: inventory().item_in_slot()/intrusive_ptr c_ptr stay out-of-line in our
// base (target inlines the slot load to `mov eax,[esi+8]; cmp [eax+124h],0`); same
// intrusive_ptr accessor-inlining cap that walls player::skeleton. cast_weapon_core
// slot now matches after the interactive_object vtable reorder.
void player::process_quick_slots_for_proxy_player( )
{
	if ( remote_input( ).actions_mask & 0x1000 )
	{
		weapon_core* const active_weapon = m_current_active_object->cast_weapon_core( );
		weapon_core* const slot_weapon = inventory( ).item_in_slot( weapon1_slot )
			? inventory( ).item_in_slot( weapon1_slot )->cast_weapon_core( ) : NULL;
		if ( slot_weapon && active_weapon != slot_weapon && slot_weapon->could_be_used( *this ) )
			inventory( ).action( weapon1_slot, true );
	}

	if ( remote_input( ).actions_mask & 0x2000 )
	{
		weapon_core* const active_weapon = m_current_active_object->cast_weapon_core( );
		weapon_core* const slot_weapon = inventory( ).item_in_slot( weapon2_slot )
			? inventory( ).item_in_slot( weapon2_slot )->cast_weapon_core( ) : NULL;
		if ( slot_weapon && active_weapon != slot_weapon && slot_weapon->could_be_used( *this ) )
			inventory( ).action( weapon2_slot, true );
	}

	if ( remote_input( ).actions_mask & 0xC000 )
	{
		const bool key_down = ( remote_input( ).actions_mask >> 14 ) & 1;
		inventory( ).action( quick_slot1, key_down );
	}

	if ( remote_input( ).actions_mask & 0x30000 )
	{
		const bool key_down = ( remote_input( ).actions_mask >> 16 ) & 1;
		inventory( ).action( quick_slot2, key_down );
	}

	if ( remote_input( ).actions_mask & 0xC0000 )
	{
		const bool key_down = ( remote_input( ).actions_mask >> 18 ) & 1;
		inventory( ).action( quick_slot3, key_down );
	}

	if ( remote_input( ).actions_mask & 0x300000 )
	{
		const bool key_down = ( remote_input( ).actions_mask >> 20 ) & 1;
		inventory( ).action( quick_slot4, key_down );
	}

	if ( remote_input( ).actions_mask & 0xC00000 )
	{
		const bool key_down = ( remote_input( ).actions_mask >> 22 ) & 1;
		inventory( ).action( quick_slot5, key_down );
	}

	if ( remote_input( ).actions_mask & 0x3000000 )
	{
		const bool key_down = ( remote_input( ).actions_mask >> 24 ) & 1;
		inventory( ).action( quick_slot6, key_down );
	}

	if ( remote_input( ).actions_mask & 0x4000000 )
		inventory( ).action( back_slot, true );
}

// claude@NOTE: current-player variant (unblocks player_tick::time_warp): like the
// proxy but driven by input() (virtual) and richer - weapon blocks show a "could not
// be used" screen message on failure, quick-slot blocks feed add_quick_slot_to_update
// when action() succeeds. Same item_in_slot/intrusive_ptr accessor-inlining residual.
void player::process_quick_slots_for_current_player( )
{
	if ( input( ).actions_mask & 0x1000 )
	{
		weapon_core* const active_weapon = m_current_active_object->cast_weapon_core( );
		weapon_core* const slot_weapon = inventory( ).item_in_slot( weapon1_slot )
			? inventory( ).item_in_slot( weapon1_slot )->cast_weapon_core( ) : NULL;
		if ( slot_weapon && active_weapon != slot_weapon )
		{
			if ( slot_weapon->could_be_used( *this ) )
				inventory( ).action( weapon1_slot, true );
			else if ( m_game_ui )
				m_game_ui->show_screen_message( "st_weapon_could_not_be_used" );
		}
	}

	if ( input( ).actions_mask & 0x2000 )
	{
		weapon_core* const active_weapon = m_current_active_object->cast_weapon_core( );
		weapon_core* const slot_weapon = inventory( ).item_in_slot( weapon2_slot )
			? inventory( ).item_in_slot( weapon2_slot )->cast_weapon_core( ) : NULL;
		if ( slot_weapon && active_weapon != slot_weapon )
		{
			if ( slot_weapon->could_be_used( *this ) )
				inventory( ).action( weapon2_slot, true );
			else if ( m_game_ui )
				m_game_ui->show_screen_message( "st_weapon_could_not_be_used" );
		}
	}

	if ( input( ).actions_mask & 0xC000 )
	{
		const bool key_down = ( input( ).actions_mask >> 14 ) & 1;
		if ( inventory( ).action( quick_slot1, key_down ) && m_game_ui )
			m_game_ui->add_quick_slot_to_update( quick_slot1 );
	}

	if ( input( ).actions_mask & 0x30000 )
	{
		const bool key_down = ( input( ).actions_mask >> 16 ) & 1;
		if ( inventory( ).action( quick_slot2, key_down ) && m_game_ui )
			m_game_ui->add_quick_slot_to_update( quick_slot2 );
	}

	if ( input( ).actions_mask & 0xC0000 )
	{
		const bool key_down = ( input( ).actions_mask >> 18 ) & 1;
		if ( inventory( ).action( quick_slot3, key_down ) && m_game_ui )
			m_game_ui->add_quick_slot_to_update( quick_slot3 );
	}

	if ( input( ).actions_mask & 0x300000 )
	{
		const bool key_down = ( input( ).actions_mask >> 20 ) & 1;
		if ( inventory( ).action( quick_slot4, key_down ) && m_game_ui )
			m_game_ui->add_quick_slot_to_update( quick_slot4 );
	}

	if ( input( ).actions_mask & 0xC00000 )
	{
		const bool key_down = ( input( ).actions_mask >> 22 ) & 1;
		if ( inventory( ).action( quick_slot5, key_down ) && m_game_ui )
			m_game_ui->add_quick_slot_to_update( quick_slot5 );
	}

	if ( input( ).actions_mask & 0x3000000 )
	{
		const bool key_down = ( input( ).actions_mask >> 24 ) & 1;
		if ( inventory( ).action( quick_slot6, key_down ) && m_game_ui )
			m_game_ui->add_quick_slot_to_update( quick_slot6 );
	}

	if ( input( ).actions_mask & 0x4000000 )
		if ( inventory( ).action( back_slot, true ) && m_game_ui )
			m_game_ui->add_quick_slot_to_update( back_slot );
}

physics::world* player::get_physics_world( )
{
	return m_game_scene.get_physics_world( );
}

// STATE[STUB]
void player::select_animations( const u32 current_time_in_ms )
{
	// LOCALS
	// mutable_buffer 					buffer
	// ******

	// CALL SITE INFO
	// <0x5e4ff7> -> animation::mixing::expression < unknown >( mutable_buffer&, const bool ) const
	// <0x5e511f> -> void* < unknown >( u32 )
	// ******

	// FUNCTION BODY[0x5e4f60]: 17
	// <0>
	// <1>
	// <0x5e4f6e>|0x00e|+0x007:'1439'
	// <0>
	// <1>
	// <2>
	// <0x5e4f75>|0x015|+0x02a:'1443'
	// <0x5e4f9f>|0x03f|+0x024:'1444'
	// <0>
	// <0x5e4fc3>|0x063|+0x01a:'1446'
	// <0x5e4fdd>|0x07d|+0x01c:'1447'
	// <0>
	// <1>
	// <2>
	// <0x5e4ff9>|0x099|+0x088:'1451'
	// <0>
	// <0x5e5081>|0x121|+0x088:'1453'
	// ******
}

// TU statics (compiler-generated dynamic initializers / atexit
// destructors); a matcher recovers their types/initializers from the asm.
/*
// STATE[STUB]
void `dynamic initializer for 'cc_death_camera_yaw''( )
{
	// FUNCTION BODY[0x7d8110]
	// <0x7d8110>|0x000|      :'1460'	{
	// ******
}

// STATE[STUB]
void `dynamic initializer for 'cc_death_camera_pitch''( )
{
	// FUNCTION BODY[0x7d8180]
	// <0x7d8180>|0x000|      :'1461'	{
	// ******
}

// STATE[STUB]
void `dynamic atexit destructor for 's_player_show_animations_command''( )
{
	// FUNCTION BODY[0x7efd00]
	// <0x7d81f0>|0x000|      :'1462'	{
	// ******
}
*/

// STATE[STUB]
void player::kill( const u32 current_time_in_ms )
{
	// FUNCTION BODY[0x5e54d0]: 18
	// <0x5e54d6>|0x006|+0x02c:'1466'
	// <0>
	// <0x5e5502>|0x032|+0x007:'1468'
	// <0x5e5509>|0x039|+0x007:'1469'
	// <0>
	// <0x5e5510>|0x040|+0x008:'1471'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5e5518>|0x048|+0x031:'1476'
	// <0x5e5549>|0x079|+0x036:'1477'
	// <0x5e557f>|0x0af|+0x010:'1478'
	// <0x5e558f>|0x0bf|+0x017:'1479'
	// <0x5e55a6>|0x0d6|+0x016:'1480'
	// <0>
	// <1>
	// <0x5e55bc>|0x0ec|+0x00b:'1483'
	// ******
}

// STATE[STUB]
void player::attach_controller(
	player_input_handler*		handler,
	stats_graph*				linear_speed,
	stats_graph*				angular_speed,
	game_world_ui*				ui
)
{
	// CALL SITE INFO
	// <0x5e32a0> -> void < unknown >( game_world_ui* )
	// ******

	// FUNCTION BODY[0x5e3290]: 42
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5e3291>|0x001|+0x011:'1493'
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
	// <19>
	// <20>
	// <21>
	// <22>
	// <23>
	// <24>
	// <0x5e32a2>|0x012|+0x023:'1519'
	// <0x5e32c5>|0x035|+0x02b:'1520'
	// <0x5e32f0>|0x060|+0x010:'1521'
	// <0x5e3300>|0x070|+0x002:'1522'
	// <0x5e3302>|0x072|+0x02b:'1523'
	// <0x5e332d>|0x09d|+0x010:'1524'
	// <0>
	// <0x5e333d>|0x0ad|+0x012:'1526'
	// <0x5e334f>|0x0bf|+0x017:'1527'
	// <0>
	// <0x5e3366>|0x0d6|+0x008:'1529'
	// ******
}

// STATE[STUB]
void player::detach_controller( )
{
	// CALL SITE INFO
	// <0x5e3239> -> void < unknown >( game_world_ui* )
	// ******

	// FUNCTION BODY[0x5e3220]: 14
	// <0>
	// <1>
	// <0x5e3220>|0x000|+0x013:'1536'
	// <0x5e3233>|0x013|+0x008:'1537'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5e323b>|0x01b|+0x024:'1544'
	// <0x5e325f>|0x03f|+0x027:'1545'
	// <0>
	// <0x5e3286>|0x066|+0x007:'1547'
	// ******
}

// STATE[STUB]
void player::set_head_visibility( bool is_visible )
{
	// FUNCTION BODY[0x5e4840]: 14
	// <0x5e4841>|0x001|+0x00c:'1552'
	// <0>
	// <1>
	// <0x5e484d>|0x00d|+0x006:'1555'
	// <0>
	// <0x5e4853>|0x013|+0x005:'1557'
	// <0>
	// <0x5e4858>|0x018|+0x004:'1559'
	// <0x5e485c>|0x01c|+0x005:'1560'
	// <0>
	// <0x5e4861>|0x021|+0x02a:'1562'
	// <0x5e488b>|0x04b|+0x02b:'1563'
	// <0x5e48b6>|0x076|+0x02b:'1564'
	// <0x5e48e1>|0x0a1|+0x02a:'1565'
	// ******
}

// STATE[STUB]
animation::callback_return_type_enum empty_callback( animation::animation_callback_params& params )
{
	return animation::callback_return_type_call_me_again;	// buildability return

	// FUNCTION BODY[0x5e2220]: 2
	// <0>
	// <0x5e2220>|0x000|+0x002:'1576'
	// ******
}

// STATE[STUB]
void player::subscribe_animation_player(
	pcstr			channel_id,
	boost::function< enum animation::callback_return_type_enum( animation::animation_callback_params& ) > const&	callback,
	pcvoid			callback_uid,
	resources::managed_resource_ptr const&	animation,
	const u8		event_type,
	pcvoid const	animated_object
)
{
	// CALL SITE INFO
	// <0x5e377c> -> < unknown >
	// <0x5e37e5> -> < unknown >
	// ******

	// FUNCTION BODY[0x5e3730]: 4
	// <0x5e3739>|0x009|+0x028:'1589'
	// <0>
	// <0x5e3761>|0x031|+0x089:'1591'
	// <0>
	// ******
}

// STATE[STUB]
void player::subscribe_animation_player(
	animation::reserved_channel_ids_enum	channel_id,
	boost::function< enum animation::callback_return_type_enum( animation::animation_callback_params& ) > const&	callback,
	pcvoid const	callback_uid,
	resources::managed_resource_ptr const&	animation,
	pcvoid const	animated_object
)
{
	// CALL SITE INFO
	// <0x5e389a> -> < unknown >
	// <0x5e3911> -> < unknown >
	// ******

	// FUNCTION BODY[0x5e3840]: 4
	// <0x5e3849>|0x009|+0x036:'1603'
	// <0>
	// <0x5e387f>|0x03f|+0x097:'1605'
	// <0>
	// ******
}

// claude@NOTE: capped on animation::animation_player::unsubscribe being a stub
// (carcass, no body) in the animation module - both calls inline to nothing in
// our base (0 stmts). Re-score once animation_player::unsubscribe has its body.
void player::unsubscribe_animation_player( pcstr channel_id, pcvoid callback_uid )
{
	m_current.animation_player.unsubscribe( channel_id, callback_uid );
	m_target.animation_player.unsubscribe( channel_id, callback_uid );
}

void player::unsubscribe_animation_player( animation::reserved_channel_ids_enum channel_id, pcvoid callback_uid )
{
	m_current.animation_player.unsubscribe( channel_id, callback_uid );
	m_target.animation_player.unsubscribe( channel_id, callback_uid );
}

// STATE[STUB]
void player::deserialize( network_core::packet_reader& reader )
{
	// LOCALS
	// profile_slot_enum 				server_target_active_slot
	// float3 							position
	// interactive_object_ptr 			server_current_active_object
	// ******

	// CALL SITE INFO
	// <0x5e53a0> -> void < unknown >()
	// <0x5e53af> -> void < unknown >( interactive_object_ptr const&, resources::resource_ptr< interactive_object, resources::unmanaged_intrusive_base > const& ) const
	// <0x5e5407> -> void < unknown >( base_player&, engine& )
	// <0x5e548a> -> player_stamina& < unknown >()
	// ******

	// FUNCTION BODY[0x5e52c0]: 42
	// <0x5e52c7>|0x007|+0x016:'1627'
	// <0x5e52dd>|0x01d|+0x008:'1628'
	// <0x5e52e5>|0x025|+0x011:'1629'
	// <0x5e52f6>|0x036|+0x006:'1630'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5e52fc>|0x03c|+0x017:'1635'
	// <0x5e5313>|0x053|+0x007:'1636'
	// <0>
	// <0x5e531a>|0x05a|+0x01c:'1638'
	// <0x5e5336>|0x076|+0x00b:'1639'
	// <0>
	// <0x5e5341>|0x081|+0x009:'1641'
	// <0x5e534a>|0x08a|+0x009:'1642'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5e5353>|0x093|+0x02f:'1648'
	// <0>
	// <0x5e5382>|0x0c2|+0x00c:'1650'
	// <0>
	// <1>
	// <0x5e538e>|0x0ce|+0x00b:'1653'
	// <0x5e5399>|0x0d9|+0x009:'1654'
	// <0x5e53a2>|0x0e2|+0x00f:'1655'
	// <0x5e53b1>|0x0f1|+0x03a:'1656'
	// <0x5e53eb>|0x12b|+0x01e:'1657'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5e5409>|0x149|+0x079:'1663'
	// <0>
	// <0x5e5482>|0x1c2|+0x011:'1665'
	// <0x5e5493>|0x1d3|+0x009:'1666'
	// <0>
	// <1>
	// ******
}

} // namespace survarium
