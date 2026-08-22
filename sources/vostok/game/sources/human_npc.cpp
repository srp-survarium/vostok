////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "human_npc.h"
#include "game_world.h"				// base game_object_ + ref members source off game_world
#include "game.h"					// ctor: game_world.get_game().get_sound_world()/renderer()
#include "animated_model_instance.h"	// resource_ptr member dtor needs complete type
#include "animation_space_graph.h"		// resource_ptr member dtor needs complete type
#include "animations_selector.h"		// dtor DELETEs m_animations_selector
#include <vostok/ai/world.h>
#include <vostok/ai/weapon.h>
#include <vostok/ai/movement_target.h>		// move_to_position: target->target_position
#include <vostok/ai/animation_item.h>		// play_animation: target->animation
#include <vostok/ai/collision_object.h>	// is_at_node: get_collision_object()->get_origin()
#include <vostok/ai/npc_statistics.h>
#include <vostok/ai/sensed_sound_object.h>
#include <vostok/ai/sensed_hit_object.h>
#include <vostok/collision/animated_object.h>
#include <vostok/render/facade/game_renderer.h>
#include <vostok/render/facade/debug_renderer.h>	// draw: debug().draw_origin/draw_arrow/draw_line_ellipsoid
#include <vostok/render/facade/scene_renderer.h>	// set_transform: scene().update_model
#include <vostok/animation/animation_player.h>		// set_transform: animation_player::set_object_transform
#include <vostok/sound/world.h>				// enable: get_logic_world_user
#include <vostok/sound/world_user.h>			// enable: register_receiver
#include <vostok/physics/world.h>				// enable: m_physics_world.add
#include "game_memory.h"						// enable: NEW( animations_selector )
#include <vostok/console_command.h>

static bool s_npc_debug_draw		= false;
#line 35
static vostok::console_commands::cc_bool s_npc_debug_draw_command( "npc_debug_draw", s_npc_debug_draw, true, vostok::console_commands::command_type_engine_internal );

namespace survarium {

// TU-local (canonical headers/hit_object.h; owner mapping in
// temp/triage_log.md) - the on_hit_event parameter type
struct hit_object {
	inline		hit_object	( ) { /* no source */ }

public:
	/* 0x0000 */	ai::game_object*	m_source;
	/* 0x0004 */	float3				m_position;
	/* 0x0010 */	u16					m_target_bone;
	/* 0x0014 */	float				m_power;
}; // struct hit_object

STATIC_SIZE_ASSERT(hit_object, 0x18);

// TU statics (compiler-generated dynamic initializers / atexit
// destructors); a matcher recovers their types/initializers from the asm.
/*
void `dynamic atexit destructor for 's_npc_debug_draw_command''( )
{
}
*/

 human_npc::npc_game_attributes::npc_game_attributes( ) :
	initial_position		( float3( 0.f, 0.f, 0.f ) ),
	initial_scale			( float3( 1.f, 1.f, 1.f ) ),
	initial_rotation		( float3( 0.f, 0.f, 0.f ) ),
	debug_draw_color		( math::color( 0, 0, 0 ) ),
	name					( "noname" ),
	description				( "human" ),
	initial_velocity		( 0.f ),
	initial_luminosity		( 0.002f ),
	id						( u32(-1) ),
	group_id				( u32(-1) ),
	class_id				( 0 ),
	outfit_id				( u32(-1) )
#line 52
{
}

human_npc::npc_game_attributes& human_npc::npc_game_attributes::operator=( human_npc::npc_game_attributes& other )
#line 56
{
	if ( this != &other )
	{
		initial_position	= other.initial_position;
		initial_scale		= other.initial_scale;
		initial_rotation	= other.initial_rotation;
		debug_draw_color	= other.debug_draw_color;
		name				= other.name;
		description			= other.description;
		initial_velocity	= other.initial_velocity;
		initial_luminosity	= other.initial_luminosity;
		id					= other.id;
		group_id			= other.group_id;
		class_id			= other.class_id;
		outfit_id			= other.outfit_id;
		weapons.swap		( other.weapons );
	}

	return *this;
}

 human_npc::human_npc( game_world& game_world ) :
	game_object_( game_world ),
	m_ai_world( game_world.get_ai_world( ) ),
	m_sound_world( game_world.get_game( ).get_sound_world( ) ),
	m_physics_world( *game_world.get_physics_world( ) ),
	m_game_world( game_world ),
	m_renderer( game_world.get_game( ).renderer( ) ),
	m_visibility_parameters( 0.0f ),
	m_scene( game_world.render_scene( ) ),
	m_sound_scene( game_world.get_sound_scene( ) ),
	m_affects_subscription( boost::bind( &human_npc::on_affect_event, this, _1, _2, _3 ) ),
	m_feet_adjustment_speed( 1.f )
#line 101
{
}

 human_npc::~human_npc( )
#line 105
{
	DELETE						( m_animations_selector );
}

void human_npc::clear_resources( )
#line 110
{
	m_model_instance->m_damage_model->unsubscribe_from_affect	( affects_type_death, &m_affects_subscription );

	m_sound_world.get_logic_world_user( ).unregister_receiver	( m_sound_scene, *this );

	m_renderer.scene( ).remove_model	( m_scene, m_model_instance->m_render_model->m_model );
	m_physics_world.remove				( m_model_instance->m_damage_collision->get_rigid_body() );

	m_ai_world.on_destruction_event		( *this );
	m_ai_world.remove_brain_unit		( m_brain_unit );
}

void human_npc::set_brain_unit( resources::unmanaged_resource_ptr const& brain_unit )
#line 123
{
	ASSERT						( !m_brain_unit );
	m_brain_unit				= brain_unit;
}

void human_npc::set_animation_space_graph( animation_space_graph_ptr const& space_graph )
#line 129
{
	m_animation_space_graph		= space_graph;

}

void human_npc::set_model( animated_model_instance_ptr const& model )
#line 135
{
	m_model_instance			= model;

	m_model_instance->m_damage_collision->set_owner		( this );
	m_model_instance->m_damage_model->subscribe_on_affect( affects_type_death, &m_affects_subscription );
}

void human_npc::set_default_animation( resources::managed_resource_ptr const& default_animation )
#line 143
{
	m_default_animation			= default_animation;
}

void human_npc::enable( )
#line 148
{

	m_ai_world.add_brain_unit			( m_brain_unit );

	m_sound_world.get_logic_world_user( ).register_receiver	( m_sound_scene, *this );
	set_position						( get_position() );



	m_physics_world.add					( m_model_instance->m_damage_collision->get_rigid_body(), 0x40, 0xffff );
	m_renderer.scene( ).add_model		( m_scene, m_model_instance->m_render_model->m_model, m_transform );
	m_model_instance->m_animation_player->set_object_transform	( m_transform, 0 );

	m_feet_target						= get_position();

	m_animations_selector				= NEW( animations_selector )(
		*m_model_instance->m_animation_player,
		m_animation_space_graph,
		m_default_animation,
		m_game_world.get_ai_navigation_world( ),
		m_game_world,
		*this
	);
	m_ai_world.select_new_goal			( m_brain_unit );
}

void human_npc::on_sound_event( sound::sound_producer const& sound_source )
#line 175
{
	m_sound_perceived			= true;

	ai::sensed_sound_object		perceived_sound;
	human_npc const* source		= static_cast_checked< human_npc const* >( &sound_source );
	perceived_sound.object		= source->cast_game_object();
	perceived_sound.position	= sound_source.get_source_position( float3( 0, 0, 0 ) );
	perceived_sound.type		= (ai::sound_collection_types)sound_source.m_sound_type;
	perceived_sound.power		= sound_source.m_sound_power;

	m_ai_world.on_sound_event	( *this, perceived_sound );
}

void human_npc::on_hit_event( hit_object const& hit_source )
#line 189
{
	ai::sensed_hit_object			perceived_hit;
	perceived_hit.own_position		= get_position( hit_source.m_position );
	perceived_hit.object			= hit_source.m_source;
	perceived_hit.direction			= hit_source.m_position;
	perceived_hit.bone_index		= hit_source.m_target_bone;
	perceived_hit.extent_of_damage	= hit_source.m_power;

	m_ai_world.on_hit_event			( *this, perceived_hit );
	m_sound_produced				= true;
}

math::aabb human_npc::get_aabb( ) const
#line 202
{
	return m_model_instance->m_damage_collision->get_aabb();
}

float3 human_npc::get_random_surface_point( const u32 current_time ) const
#line 207
{
	return m_model_instance->m_damage_collision->get_random_surface_point( current_time );
}

float3 human_npc::get_position( float3 const& requester ) const
#line 212
{
	return local_to_cell( requester ).c.xyz();
}

float3 human_npc::get_position( ) const
#line 217
{
	return m_transform.c.xyz( );
}

float4x4 human_npc::get_eyes_matrix( ) const
#line 222
{
	return math::create_camera_direction	(
		get_eyes_position(),
		get_eyes_direction(),
		float3( 0.f, 1.f, 0.f )
	);
}

float3 human_npc::get_eyes_direction( ) const
#line 231
{
	return normalize( m_transform.transform_direction( m_model_instance->m_damage_collision->get_eyes_direction() ) );
}

float3 human_npc::get_eyes_position( ) const
#line 236
{
	return m_transform.transform_position( m_model_instance->m_damage_collision->get_head_bone_center() );
}

float4x4 human_npc::local_to_cell( float3 const& requester ) const
#line 241
{
	VOSTOK_UNREFERENCED_PARAMETER	( requester );
	return m_transform;
}

void human_npc::draw_damage_model( render::game::renderer& render, render::scene_ptr const& scene ) const
#line 247
{
	VOSTOK_UNREFERENCED_PARAMETERS( render, scene );

	u32 const bones_count = m_model_instance->m_damage_collision->get_bones_count( );
	for ( u32 i = 0; i < bones_count; ++i )
	{
		m_model_instance->m_damage_model->get_body_part(
			m_model_instance->m_damage_collision->body_part_name( i ) );
	}




}

void human_npc::draw( render::game::renderer& render, render::scene_ptr const& scene ) const
#line 263
{
	draw_damage_model				( render, scene );








	m_renderer.debug( ).draw_origin	( m_scene, m_transform, 3.f );

	float3 const eyes_position		= get_eyes_position( );
	float3 const eyes_direction		= get_eyes_direction( ) * 3.f;
	m_renderer.debug( ).draw_arrow	( m_scene, eyes_position, eyes_position + eyes_direction, math::color( 255, 0, 0 ) );

	if ( m_sound_perceived )
	{
		m_renderer.debug( ).draw_line_ellipsoid	( m_scene, create_translation( get_eyes_position( ) ), math::color( 255, 0, 0 ) );
		m_sound_perceived			= false;
	}
	if ( m_sound_produced )
	{
		m_renderer.debug( ).draw_line_ellipsoid	( m_scene, m_transform, math::color( 0, 255, 0 ) );
		m_sound_produced			= false;
	}
	// target passes the members, not the (dead, LTCG-stripped) params
	m_animations_selector->debug_draw	( m_renderer, m_scene );
}

void human_npc::set_transform( float4x4 const& transform )
#line 304
{
	m_transform					= transform;
	m_feet_target				= transform.c.xyz();

	m_renderer.scene( ).update_model	( m_scene, m_model_instance->m_render_model->m_model, m_transform );
	m_model_instance->m_animation_player->set_object_transform	( m_transform, 0 );
}

// Matching wall: target LTCG eliminates the position temporary and specializes draw's
// two member arguments away; target and base otherwise have the same statements and flow.
void human_npc::tick( const u32 current_time_in_ms, const bool is_game_paused )
#line 313
{
	const u32 time_delta		= current_time_in_ms > m_last_tick_time_in_ms ? current_time_in_ms - m_last_tick_time_in_ms : 0;

	if ( !is_game_paused )
	{
		m_physics_world.move				( m_model_instance->m_damage_collision->get_rigid_body(), m_transform );
		set_position						( get_position() );
		m_model_instance->m_damage_model->tick	( time_delta, current_time_in_ms );
		tick_animation_player				( current_time_in_ms );
	}

	if ( debug_draw_allowed() )
		draw							( m_renderer, m_scene );

	m_last_tick_time_in_ms		= current_time_in_ms;
}

// Matching wall: target update_skeleton uses a render-LTCG register convention.
void human_npc::render_model( )
#line 331
{
	animation::animation_player* animation_player	= m_model_instance->m_animation_player;
	animation::skeleton_ptr skeleton					= m_model_instance->m_physics_model->m_skeleton;

	u32 const bone_matrices_count	= skeleton->get_non_root_bones_count();
	float4x4* const bone_matrices	= static_cast< float4x4* >( ALLOCA( bone_matrices_count * sizeof( float4x4 ) ) );
	animation_player->compute_bones_matrices	( *skeleton, bone_matrices, bone_matrices + bone_matrices_count, 0, NULL );

	m_renderer.scene( ).update_model	( m_scene, m_model_instance->m_render_model->m_model, m_transform );




	m_renderer.scene( ).update_skeleton	( m_model_instance->m_render_model->m_model, bone_matrices, bone_matrices_count );



	m_model_instance->m_damage_collision->update	( bone_matrices, bone_matrices + bone_matrices_count );
}

object_weapon* human_npc::pop_weapon( )
#line 363
{
	return m_game_attributes.weapons.pop_front( );
}

bool human_npc::is_safe( ) const
#line 368
{
	return m_ai_world.is_npc_safe( m_brain_unit );
}

bool human_npc::is_target_in_melee_range( ai::npc const* const target ) const
#line 373
{
	ASSERT						( target );
	return math::length			( target->get_position( get_position() ) - get_position() ) <= 10;
}

bool human_npc::is_at_node( ai::game_object const* const node ) const
#line 379
{
	VOSTOK_UNREFERENCED_PARAMETER	( node );
	return true;
}

void human_npc::prepare_to_attack( ai::npc const* const target, ai::weapon const* const gun )
#line 387
{
	LOG_INFO					( "%s: prepare to attack %s with %s", get_name(), target->cast_game_object()->get_name(), gun->cast_game_object()->get_name() );
	m_current_target			= target;
	m_current_weapon			= gun;
}

void human_npc::attack( ai::npc const* const target, ai::weapon const* const gun )
#line 394
{
	VOSTOK_UNREFERENCED_PARAMETERS( target, gun );
	LOG_INFO					( "%s: attacking %s with %s", get_name(), m_current_target->cast_game_object()->get_name(), m_current_weapon->cast_game_object()->get_name() );
}

void human_npc::attack_melee( ai::npc const* const target, ai::weapon const* const gun )
#line 400
{
	VOSTOK_UNREFERENCED_PARAMETERS( target, gun );
	LOG_INFO					( "%s: melee attacking %s with %s", get_name(), m_current_target->cast_game_object()->get_name(), m_current_weapon->cast_game_object()->get_name() );
}

void human_npc::attack_from_cover( ai::npc const* const target, ai::weapon const* const gun )
#line 406
{
	VOSTOK_UNREFERENCED_PARAMETERS( target, gun );
	LOG_INFO					( "%s: attacking from cover %s with %s", get_name(), m_current_target->cast_game_object()->get_name(), m_current_weapon->cast_game_object()->get_name() );
}

void human_npc::stop_attack( ai::npc const* const target, ai::weapon const* const gun )
#line 412
{
	VOSTOK_UNREFERENCED_PARAMETERS( target, gun );
	LOG_INFO					( "%s: stopping attack", get_name() );
	m_current_target			= 0;
	m_current_weapon			= 0;
}

void human_npc::survey_area( )
#line 420
{
	LOG_INFO					( "%s: patrolling", get_name() );
	m_is_patrolling				= true;
}

void human_npc::stop_patrolling( )
#line 426
{
	LOG_INFO					( "%s: quit patrolling", get_name() );
	m_is_patrolling				= false;
}

void human_npc::reload( ai::weapon const* const gun )
#line 432
{
	LOG_INFO					( "%s: reloading %s", get_name(), gun->cast_game_object()->get_name() );
}

void human_npc::fill_stats( ai::npc_statistics& stats ) const
#line 437
{
	stats.general_state.caption	= "general properties:";

	typedef ai::npc_statistics::general_info_type::content_type content_type;
	content_type				new_item_content( "name: " );
	new_item_content.append		( get_name() );
	stats.general_state.content.push_back( new_item_content );

	new_item_content.clear		( );
	new_item_content.appendf	( "position: %f  %f  %f", get_position().x, get_position().y, get_position().z );
	stats.general_state.content.push_back( new_item_content );

	new_item_content.clear		( );
	new_item_content.appendf	( "eyes direction: %f  %f  %f", get_eyes_direction().x, get_eyes_direction().y, get_eyes_direction().z );
	stats.general_state.content.push_back( new_item_content );

	m_ai_world.fill_npc_stats	( stats, m_brain_unit );


}

void human_npc::set_attributes( human_npc::npc_game_attributes& attributes )
#line 458
{
	m_game_attributes			= attributes;
	m_transform					= create_scale( m_game_attributes.initial_scale ) *
								  math::create_rotation_y( m_game_attributes.initial_rotation.y ) *
								  create_translation( m_game_attributes.initial_position );
}

void human_npc::get_available_weapons( vectora< ai::weapon* >& list_to_be_filled ) const
#line 466
{
	for ( object_weapon* weapon = m_game_attributes.weapons.front(); weapon; weapon = npc_game_attributes::object_weapon_list::get_next_of_object( weapon ) )
		list_to_be_filled.push_back		( weapon );
}

void human_npc::set_translation( float4x4 const& new_translation )
#line 481
{
	float4x4 new_transform		= create_scale( m_transform.get_scale() ) *
								  create_rotation( m_transform.get_angles_xyz() ) *
								  new_translation;
	set_transform				( new_transform );
}

void human_npc::set_behaviour( resources::unmanaged_resource_ptr new_behaviour )
#line 490
{
	m_ai_world.set_behaviour			( new_behaviour, m_brain_unit );
}

bool human_npc::debug_draw_allowed( ) const
#line 495
{
	return s_npc_debug_draw;
}

void human_npc::move_to_position( ai::movement_target const* const target )
#line 500
{
	m_current_movement_target			= target;
	LOG_INFO							(
		"%s: trying to move to point %.2f %.2f %.2f",
		get_name(),
		m_current_movement_target->target_position.x,
		m_current_movement_target->target_position.y,
		m_current_movement_target->target_position.z
	);
	m_animations_selector->set_target	( *m_current_movement_target );
}

// Retail passes the path object itself through LOG_INFO; only the embedded source line differs.
void human_npc::on_animation_end( )
#line 514
{
	if ( m_current_animation )
	{
#line 520
		LOG_INFO						( "%s: stop playing animation %s", get_name(), m_current_animation->name );
		m_ai_world.on_animation_finish	( m_current_animation, m_brain_unit );
		m_current_animation				= 0;	m_ai_world.select_new_goal( m_brain_unit );
	}
}

void human_npc::hit(
	hit_initiator const* const		initiator,
	const u32						bone_index,
	pcstr							damage_type,
	const float						amount,
	const float						armor_piercing,
	bullet* const					bullet
)
#line 538
{
	m_model_instance->m_damage_model->hit_body_part	(
		initiator->id,
		m_model_instance->m_damage_collision->body_part_name( bone_index ),
		damage_type,
		amount,
		armor_piercing,
		m_last_tick_time_in_ms,
		bullet
	);
}

void human_npc::hit(
	hit_initiator const* const		initiator,
	collision::bone_collision_data const&	bone_data,
	pcstr							damage_type,
	const float						amount,
	const float						armor_piercing,
	bullet* const					bullet
)
#line 551
{
	m_model_instance->m_damage_model->hit_body_part	(
		initiator->id,
		bone_data.body_part_name.c_str( ),
		damage_type,
		amount,
		armor_piercing,
		m_last_tick_time_in_ms,
		bullet
	);
}

// Matching wall: only LOG_INFO's embedded original source line differs.
#line 565
void human_npc::on_movement_end( )
#line 556
{
	if ( m_current_movement_target )
	{
		LOG_INFO						(
			"target reached: [%.2f][%.2f][%.2f]",
			m_current_movement_target->target_position.x,
			m_current_movement_target->target_position.y,
			m_current_movement_target->target_position.z
		);
		m_current_movement_target		= 0;	m_ai_world.select_new_goal( m_brain_unit );
	}
}

// Retail passes the path object by value; set_target retains an animation-LTCG register-argument wall.
void human_npc::play_animation( ai::animation_item const* const target )
#line 575
{
	m_current_animation					= target;
	animation::animation_expression_emitter_ptr animation_emitter	= static_cast_resource_ptr< animation::animation_expression_emitter_ptr >( target->animation );
	m_animations_selector->set_target	( *m_current_animation );
	LOG_INFO							( "%s: playing animation %s", get_name(), m_current_animation->name );
}

void human_npc::tick_animation_player( const u32 current_time_in_ms )
#line 583
{
	m_model_instance->m_animation_player->tick					( current_time_in_ms );
	m_transform					= m_model_instance->m_animation_player->get_object_transform( 0 );
	up_to_terrain				( );
	render_model				( );
}

void human_npc::up_to_terrain( )
#line 591
{
	// Matching wall: target scalar-replaces position and result; faithful value/reference
	// forms either retain aggregate stack slots or lose the target's position statement.
	float3 const position			= m_transform.c.xyz( );

	physics::closest_ray_result result	= m_game_world.get_physics_world( )->ray_test( float3( position.x, position.y + 1.f, position.z ), float3( 0.f, -1.f, 0.f ), 2.f, 0x20, 2 );

	if ( result.object )
		m_feet_target			= result.hit_point_world;

	if ( m_feet_target != position )
	{
		float3 const offset		= m_feet_target - position;
		float const factor		= math::min( m_game_world.get_game().last_frame_time() / ( length( offset ) / m_feet_adjustment_speed ), 1.f );
		set_translation			( create_translation( position + offset * factor ) );
	}
}

// claude@NOTE: body byte-identical (11/14 instrs); the only diff is the prologue/epilogue -
// the shipped build out-lined this with `this` already in esi (no `mov esi, ecx`), an LTCG
// codegen artifact no normal source emits. Body structure (one select_new_goal forward) is right.
void human_npc::select_new_goal( )
#line 608
{
	m_ai_world.select_new_goal	( m_brain_unit );
}

void human_npc::on_affect_event(
	pcstr							body_part_name,
	const hit_affects_type_enum		affect_type,
#line 616
	const affect_event_type_enum	event_type
) const
#line 613
{
	pcstr event					= event_type == affect_applying ? "applied" : "recalled";
	LOG_INFO					( "[%s] - death affect %s on body part %s", get_name(), event, body_part_name );
}

} // namespace survarium
#line 0
