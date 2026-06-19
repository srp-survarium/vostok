////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "player.h"
#include "player_creation_params.h"	// params.game_scene field
#include "base_game_scene.h"			// game_scene supplies game / scheduler
#include "player_input_handler.h"		// m_local_input_controller->set_near_plane
#include "game_memory.h"				// g_allocator for circular_buffer member
#include <vostok/render/facade/scene_renderer.h>	// add/remove_model
#include <vostok/physics/character_controller.h>	// physics_controller->set_crouch / jump

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

// STATE[STUB]
void player::remove_alive( )
{
	// CALL SITE INFO
	// <0x5e28e0> -> void < unknown >( physics::bt_rigid_body_base* )
	// ******

	// FUNCTION BODY[0x5e2890]: 18
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5e2890>|0x000|+0x015:'367'
	// <0x5e28a5>|0x015|+0x009:'368'
	// <0x5e28ae>|0x01e|+0x00e:'369'
	// <0>
	// <1>
	// <0x5e28bc>|0x02c|+0x009:'372'
	// <0x5e28c5>|0x035|+0x01d:'373'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// ******
}

// STATE[STUB]
void player::remove( )
{
	// CALL SITE INFO
	// <0x5e51b3> -> void < unknown >()
	// <0x5e51c6> -> void < unknown >( interactive_object_ptr const&, resources::resource_ptr< interactive_object, resources::unmanaged_intrusive_base > const& ) const
	// <0x5e5254> -> match_options& < unknown >()
	// ******

	// FUNCTION BODY[0x5e5150]: 59
	// <0x5e5150>|0x000|+0x007:'398'	{
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
	// <0x5e5157>|0x007|+0x009:'420'
	// <0>
	// <0x5e5160>|0x010|+0x008:'422'
	// <0x5e5168>|0x018|+0x02d:'423'
	// <0>
	// <0x5e5195>|0x045|+0x008:'425'
	// <0x5e519d>|0x04d|+0x00b:'426'
	// <0>
	// <1>
	// <2>
	// <0x5e51a8>|0x058|+0x00d:'430'
	// <0x5e51b5>|0x065|+0x039:'431'
	// <0x5e51ee>|0x09e|+0x01f:'432'
	// <0x5e520d>|0x0bd|+0x021:'433'
	// <0>
	// <0x5e522e>|0x0de|+0x008:'435'
	// <0>
	// <0x5e5236>|0x0e6|+0x005:'437'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x5e523b>|0x0eb|+0x008:'447'
	// <0>
	// <0x5e5243>|0x0f3|+0x013:'449'
	// <0x5e5256>|0x106|+0x022:'450'
	// <0>
	// <1>
	// <0x5e5278>|0x128|+0x008:'453'
	// <0>
	// <1>
	// <0x5e5280>|0x130|+0x02a:'456'
	// <0x5e52aa>|0x15a|-0x006:'457'
	// <0x5e52a4>|0x154|+0x00b:'458'
	// <0x5e52af>|0x15f|      :'458'	}
	// ******
}

// STATE[STUB]
float4x4 player::get_transform_for_animation_player( pcvoid const animated_object, float4x4 const& character_transform ) const
{
	// CALL SITE INFO
	// <0x5e244b> -> float4x4 < unknown >() const
	// ******

	return vostok::math::float4x4();

	// FUNCTION BODY[0x5e2420]: 5
	// <0x5e2420>|0x000|+0x001:'461'	{
	// <0x5e2421>|0x001|+0x006:'462'
	// <0x5e2427>|0x007|+0x017:'463'
	// <0>
	// <1>
	// <0x5e243e>|0x01e|-0x003:'466'
	// <0x5e243b>|0x01b|+0x015:'467'
	// <0x5e2450>|0x030|      :'467'	}
	// ******
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

// STATE[STUB]
void player::serialize_current_state( const u32 current_time_in_ms )
{
	// FUNCTION BODY[0x5e23b0]: 19
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
	// <0x5e23b7>|0x007|+0x00b:'519'
	// <0x5e23c2>|0x012|+0x007:'520'
	// <0x5e23c9>|0x019|+0x022:'521'
	// <0x5e23eb>|0x03b|+0x010:'522'
	// <0x5e23fb>|0x04b|+0x009:'523'
	// <0x5e2404>|0x054|+0x00f:'524'
	// <0>
	// <1>
	// ******
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

// STATE[STUB]
void player::use_ladder( ladder* __formal )
{
	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5d12f0 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN3") })
	// ******

	// FUNCTION BODY[0x5e22f0]
	// <0x5e22f0>|0x000|      :'795'	{
	// ******
}

// STATE[STUB]
void player::apply_damage_model_affect(
	pcstr							part_name,
	const hit_affects_type_enum		arg_1 /* hit_affects_type_enum affect */,
	const affect_event_type_enum	arg_2 /* affect_event_type_enum event_type */
)
{
	// CALL SITE INFO
	// <0x5e31c5> -> damage_model_ptr const& < unknown >() const
	// ******

	// FUNCTION BODY[0x5e31b0]: 3
	// <0x5e31be>|0x00e|+0x017:'868'
	// <0x5e31d5>|0x025|+0x02e:'869'
	// <0x5e3203>|0x053|+0x00b:'870'
	// ******
}

// STATE[STUB]
void player::set_character_transform( float3 const& position, const float orientation, const float look_pitch )
{
	// FUNCTION BODY[0x5e27e0]: 6
	// <0x5e27f2>|0x012|+0x02e:'875'
	// <0>
	// <1>
	// <2>
	// <0x5e2820>|0x040|+0x04c:'879'
	// <0>
	// ******
}

// STATE[STUB]
void player::hit(
	hit_initiator const* const		initiator,
	const u32						bone_index,
	pcstr const						damage_type,
	const float						amount,
	const float						armor_piercing,
	bullet* const					bullet
)
{
	// LOCALS
	// hit_info 						info
	// ******

	// CALL SITE INFO
	// <0x5e27d3> -> void < unknown >( hit_info const& )
	// ******

	// FUNCTION BODY[0x5e2770]: 6
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5e2776>|0x006|+0x047:'908'
	// <0x5e27bd>|0x04d|+0x018:'909'
	// ******
}

// STATE[STUB]
void player::hit(
	hit_initiator const* const		initiator,
	collision::bone_collision_data const&	bone_data,
	pcstr							damage_type,
	const float						amount,
	const float						armor_piercing,
	bullet* const					bullet
)
{
	// LOCALS
	// hit_info 						info
	// ******

	// CALL SITE INFO
	// <0x5e2767> -> void < unknown >( hit_info const& )
	// ******

	// FUNCTION BODY[0x5e2710]: 5
	// <0>
	// <0x5e2716>|0x006|+0x03b:'922'
	// <0x5e2751>|0x041|+0x018:'923'
	// <0>
	// <1>
	// ******
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
	m_start_fov_factor		= 0.0f;
	m_target_fov_factor		= 0.0f;
	m_current_fov_factor	= 0.0f;
}

// STATE[STUB]
void player::set_target_fov_factor( const float target_fov_factor, const float transition_time )
{
	// FUNCTION BODY[0x5e2280]: 4
	// <0x5e2280>|0x000|+0x008:'1006'
	// <0>
	// <0x5e2288>|0x008|+0x01a:'1008'
	// <0x5e22a2>|0x022|+0x014:'1009'
	// ******
}

// STATE[STUB]
float player::fov_factor( const u32 current_time_in_ms ) const
{
	// LOCALS
	// const float 						time
	// ******

	// CALL SITE INFO
	// <0x5e260f> -> float < unknown >( float ) const
	// ******

	return 0.0f;

	// FUNCTION BODY[0x5e25b0]: 3
	// <0x5e25b0>|0x000|+0x00a:'1013'	{
	// <0x5e25ba>|0x00a|+0x01c:'1014'
	// <0x5e25d6>|0x026|+0x00b:'1015'
	// <0x5e25e1>|0x031|+0x018:'1016'
	// <0x5e25f9>|0x049|-0x004:'1016'
	// <0x5e25f5>|0x045|+0x040:'1017'
	// <0x5e2635>|0x085|      :'1017'	}
	// ******
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
void player::update_camera( )
{
	// LOCALS
	// const float 						interpolation_time
	// float4x4 						transform
	// const float 						time
	// ******

	// CALL SITE INFO
	// <0x5e2bb8> -> float < unknown >( float ) const
	// ******

	// FUNCTION BODY[0x5e2b20]: 35
	// <0x5e2b20>|0x000|+0x008:'1026'	{
	// <0x5e2b28>|0x008|+0x018:'1027'
	// <0>
	// <1>
	// <0x5e2b40>|0x020|+0x01d:'1030'
	// <0>
	// <0x5e2b5d>|0x03d|+0x024:'1032'
	// <0x5e2b81>|0x061|+0x012:'1033'
	// <0x5e2b93>|0x073|+0x053:'1034'
	// <0x5e2be6>|0x0c6|+0x013:'1035'
	// <0x5e2bf9>|0x0d9|+0x00b:'1036'
	// <0x5e2c04>|0x0e4|+0x008:'1037'
	// <0>
	// <1>
	// <2>
	// <0x5e2c0c>|0x0ec|+0x015:'1041'
	// <0x5e2c21>|0x101|+0x017:'1042'
	// <0>
	// <1>
	// <0x5e2c38>|0x118|+0x002:'1045'
	// <0x5e2c3a>|0x11a|+0x005:'1046'
	// <0>
	// <0x5e2c3f>|0x11f|+0x00f:'1048'
	// <0x5e2c4e>|0x12e|+0x013:'1049'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5e2c61>|0x141|-0x00f:'1054'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5e2c52>|0x132|+0x05e:'1060'
	// <0x5e2cb0>|0x190|-0x058:'1060'
	// <0>
	// <0x5e2c58>|0x138|+0x07d:'1062'
	// <0x5e2cd5>|0x1b5|      :'1062'	}
	// ******
}

player_input player::local_input( ) const
{
	return m_local_input_controller ? m_local_input_controller->get_input( ) : player_input( );
}

// STATE[STUB]
player_input player::remote_input( ) const
{
	return m_input;	// buildability return

	// FUNCTION BODY[0x5e2530]: 1
	// <0x5e2530>|0x000|+0x057:'1071'
	// ******
}

// STATE[STUB]
float player::get_speed( ) const
{
	return 0.0f;

	// FUNCTION BODY[0x5e26b0]: 1
	// <0x5e26b0>|0x000|+0x001:'1075'	{
	// <0x5e26b1>|0x001|+0x03f:'1076'
	// <0x5e26f0>|0x040|-0x002:'1076'
	// <0x5e26ee>|0x03e|+0x018:'1077'
	// <0x5e2706>|0x056|      :'1077'	}
	// ******
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

// STATE[STUB]
bool player::set_new_active_item( inventory_item_ptr const& item )
{
	return false;

	// FUNCTION BODY[0x5e4180]: 7
	// <0x5e4180>|0x000|+0x01d:'1189'
	// <0x5e419d>|0x01d|+0x005:'1190'
	// <0x5e41a2>|0x022|+0x007:'1191'
	// <0>
	// <1>
	// <0x5e41a9>|0x029|+0x03c:'1194'
	// <0x5e41e5>|0x065|+0x01e:'1195'
	// ******
}

// STATE[STUB]
void player::take_inventory_item( inventory_item_ptr const& item )
{
	// FUNCTION BODY[0x5e4430]: 13
	// <0x5e4436>|0x006|+0x00a:'1227'
	// <0>
	// <0x5e4440>|0x010|+0x01b:'1229'
	// <0>
	// <0x5e445b>|0x02b|+0x012:'1231'
	// <0x5e446d>|0x03d|+0x02f:'1232'
	// <0>
	// <0x5e449c>|0x06c|-0x044:'1234'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// ******
}

animation::skeleton const& player::skeleton( ) const
{
	return *m_current.model->m_skeleton.c_ptr( );
}

// STATE[STUB]
void player::set_use_physics_controller_for_current( const bool value )
{
	// FUNCTION BODY[0x5e2e80]: 10
	// <0x5e2e80>|0x000|+0x011:'1294'	{
	// <0>
	// <0x5e2e91>|0x011|+0x008:'1296'
	// <0>
	// <1>
	// <0x5e2e99>|0x019|+0x006:'1299'
	// <0x5e2e9f>|0x01f|+0x004:'1300'
	// <0x5e2ea3>|0x023|+0x049:'1301'
	// <0>
	// <0x5e2eec>|0x06c|-0x007:'1303'
	// <0>
	// <0x5e2ee5>|0x065|+0x015:'1305'
	// <0x5e2efa>|0x07a|      :'1305'	}
	// ******
}

engine& player::get_engine( )
{
	return m_game_scene;
}

// STATE[STUB]
void player::process_quick_slots_for_proxy_player( )
{
	// LOCALS
	// bool 							key_down
	// bool 							key_down
	// bool 							key_down
	// bool 							key_down
	// bool 							key_down
	// bool 							key_down
	// ******

	// CALL SITE INFO
	// <0x5e3f50> -> weapon_core* < unknown >()
	// <0x5e3f76> -> weapon_core* < unknown >()
	// <0x5e3fb4> -> weapon_core* < unknown >()
	// <0x5e3fda> -> weapon_core* < unknown >()
	// ******

	// FUNCTION BODY[0x5e3f30]: 45
	// <0x5e3f34>|0x004|+0x014:'1314'
	// <0x5e3f48>|0x018|+0x00c:'1315'
	// <0x5e3f54>|0x024|+0x024:'1316'
	// <0x5e3f78>|0x048|+0x008:'1317'
	// <0x5e3f80>|0x050|+0x00c:'1318'
	// <0x5e3f8c>|0x05c|+0x00c:'1319'
	// <0>
	// <1>
	// <2>
	// <0x5e3f98>|0x068|+0x014:'1323'
	// <0x5e3fac>|0x07c|+0x00c:'1324'
	// <0x5e3fb8>|0x088|+0x024:'1325'
	// <0x5e3fdc>|0x0ac|+0x008:'1326'
	// <0x5e3fe4>|0x0b4|+0x00c:'1327'
	// <0x5e3ff0>|0x0c0|+0x00c:'1328'
	// <0>
	// <1>
	// <2>
	// <0x5e3ffc>|0x0cc|+0x014:'1332'
	// <0x5e4010>|0x0e0|+0x018:'1333'
	// <0x5e4028>|0x0f8|+0x00f:'1334'
	// <0>
	// <0x5e4037>|0x107|+0x014:'1336'
	// <0x5e404b>|0x11b|+0x014:'1337'
	// <0x5e405f>|0x12f|+0x00f:'1338'
	// <0>
	// <0x5e406e>|0x13e|+0x014:'1340'
	// <0x5e4082>|0x152|+0x00e:'1341'
	// <0x5e4090>|0x160|+0x019:'1342'
	// <0>
	// <0x5e40a9>|0x179|+0x014:'1344'
	// <0x5e40bd>|0x18d|+0x018:'1345'
	// <0x5e40d5>|0x1a5|+0x00f:'1346'
	// <0>
	// <0x5e40e4>|0x1b4|+0x014:'1348'
	// <0x5e40f8>|0x1c8|+0x017:'1349'
	// <0x5e410f>|0x1df|+0x00f:'1350'
	// <0>
	// <0x5e411e>|0x1ee|+0x014:'1352'
	// <0x5e4132>|0x202|+0x00e:'1353'
	// <0x5e4140>|0x210|+0x016:'1354'
	// <0>
	// <0x5e4156>|0x226|+0x015:'1356'
	// <0x5e416b>|0x23b|+0x00c:'1357'
	// <0>
	// ******
}

// STATE[STUB]
void player::process_quick_slots_for_current_player( )
{
	// LOCALS
	// bool 							key_down
	// bool 							key_down
	// bool 							key_down
	// bool 							key_down
	// bool 							key_down
	// bool 							key_down
	// ******

	// CALL SITE INFO
	// <0x5e3c49> -> player_input const& < unknown >() const
	// <0x5e3c5c> -> weapon_core* < unknown >()
	// <0x5e3c82> -> weapon_core* < unknown >()
	// <0x5e3cc2> -> player_input const& < unknown >() const
	// <0x5e3cd5> -> weapon_core* < unknown >()
	// <0x5e3cfb> -> weapon_core* < unknown >()
	// <0x5e3d3b> -> player_input const& < unknown >() const
	// <0x5e3d4d> -> player_input const& < unknown >() const
	// <0x5e3d87> -> player_input const& < unknown >() const
	// <0x5e3d99> -> player_input const& < unknown >() const
	// <0x5e3dcf> -> player_input const& < unknown >() const
	// <0x5e3de1> -> player_input const& < unknown >() const
	// <0x5e3e1a> -> player_input const& < unknown >() const
	// <0x5e3e2c> -> player_input const& < unknown >() const
	// <0x5e3e65> -> player_input const& < unknown >() const
	// <0x5e3e77> -> player_input const& < unknown >() const
	// <0x5e3eb0> -> player_input const& < unknown >() const
	// <0x5e3ec2> -> player_input const& < unknown >() const
	// <0x5e3ef8> -> player_input const& < unknown >() const
	// ******

	// FUNCTION BODY[0x5e3c40]: 65
	// <0x5e3c41>|0x001|+0x013:'1363'
	// <0x5e3c54>|0x014|+0x00c:'1364'
	// <0x5e3c60>|0x020|+0x024:'1365'
	// <0x5e3c84>|0x044|+0x008:'1366'
	// <0x5e3c8c>|0x04c|+0x00c:'1367'
	// <0x5e3c98>|0x058|+0x00c:'1368'
	// <0x5e3ca4>|0x064|+0x002:'1369'
	// <0x5e3ca6>|0x066|+0x00a:'1370'
	// <0x5e3cb0>|0x070|+0x00b:'1371'
	// <0>
	// <1>
	// <2>
	// <0x5e3cbb>|0x07b|+0x012:'1375'
	// <0x5e3ccd>|0x08d|+0x00c:'1376'
	// <0x5e3cd9>|0x099|+0x024:'1377'
	// <0x5e3cfd>|0x0bd|+0x008:'1378'
	// <0x5e3d05>|0x0c5|+0x00c:'1379'
	// <0x5e3d11>|0x0d1|+0x00c:'1380'
	// <0x5e3d1d>|0x0dd|+0x002:'1381'
	// <0x5e3d1f>|0x0df|+0x00a:'1382'
	// <0x5e3d29>|0x0e9|+0x00b:'1383'
	// <0>
	// <1>
	// <2>
	// <0x5e3d34>|0x0f4|+0x012:'1387'
	// <0x5e3d46>|0x106|+0x016:'1388'
	// <0x5e3d5c>|0x11c|+0x013:'1389'
	// <0x5e3d6f>|0x12f|+0x00a:'1390'
	// <0x5e3d79>|0x139|+0x007:'1391'
	// <0>
	// <0x5e3d80>|0x140|+0x012:'1393'
	// <0x5e3d92>|0x152|+0x00c:'1394'
	// <0x5e3d9e>|0x15e|+0x019:'1395'
	// <0x5e3db7>|0x177|+0x00a:'1396'
	// <0x5e3dc1>|0x181|+0x007:'1397'
	// <0>
	// <0x5e3dc8>|0x188|+0x012:'1399'
	// <0x5e3dda>|0x19a|+0x00c:'1400'
	// <0x5e3de6>|0x1a6|+0x01c:'1401'
	// <0x5e3e02>|0x1c2|+0x00a:'1402'
	// <0x5e3e0c>|0x1cc|+0x007:'1403'
	// <0>
	// <0x5e3e13>|0x1d3|+0x012:'1405'
	// <0x5e3e25>|0x1e5|+0x00c:'1406'
	// <0x5e3e31>|0x1f1|+0x01c:'1407'
	// <0x5e3e4d>|0x20d|+0x00a:'1408'
	// <0x5e3e57>|0x217|+0x007:'1409'
	// <0>
	// <0x5e3e5e>|0x21e|+0x012:'1411'
	// <0x5e3e70>|0x230|+0x00c:'1412'
	// <0x5e3e7c>|0x23c|+0x01c:'1413'
	// <0x5e3e98>|0x258|+0x00a:'1414'
	// <0x5e3ea2>|0x262|+0x007:'1415'
	// <0>
	// <0x5e3ea9>|0x269|+0x012:'1417'
	// <0x5e3ebb>|0x27b|+0x00c:'1418'
	// <0x5e3ec7>|0x287|+0x019:'1419'
	// <0x5e3ee0>|0x2a0|+0x00a:'1420'
	// <0x5e3eea>|0x2aa|+0x007:'1421'
	// <0>
	// <0x5e3ef1>|0x2b1|+0x012:'1423'
	// <0x5e3f03>|0x2c3|+0x010:'1424'
	// <0x5e3f13>|0x2d3|+0x00a:'1425'
	// <0x5e3f1d>|0x2dd|+0x007:'1426'
	// <0>
	// ******
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

// STATE[STUB]
void player::unsubscribe_animation_player( pcstr channel_id, pcvoid callback_uid )
{
	// FUNCTION BODY[0x5e3800]: 4
	// <0x5e3808>|0x008|+0x019:'1611'
	// <0>
	// <0x5e3821>|0x021|+0x00f:'1613'
	// <0>
	// ******
}

// STATE[STUB]
void player::unsubscribe_animation_player( animation::reserved_channel_ids_enum channel_id, pcvoid callback_uid )
{
	// FUNCTION BODY[0x5e3bf0]: 4
	// <0x5e3bf1>|0x001|+0x026:'1619'
	// <0>
	// <0x5e3c17>|0x027|+0x01d:'1621'
	// <0>
	// ******
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
