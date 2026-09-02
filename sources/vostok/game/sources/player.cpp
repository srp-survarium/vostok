// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include "player.h"
#include "player_creation_params.h"	// params.game_scene field
#include "base_game_scene.h"			// game_scene supplies game / scheduler
#include "game.h"
#include "base_network_client.h"
#include "flash_text_manager.h"			// text_manager().destroy_text( m_text ) in remove
#include <vostok/game_core/game_net_defines.h>		// match_options::player_profiles[] in remove
#include <vostok/game_core/player_profile.h>
#include "game_world.h"					// kill() casts m_game_scene to game_world for switch_camera_mode
#include "game_world_ui.h"				// m_game_ui->fill_quick_slots
#include "player_input_handler.h"		// m_local_input_controller->set_near_plane
#include "game_memory.h"				// g_allocator for circular_buffer member
#include <vostok/game_core/weapon_core.h>			// cast_weapon_core()->could_be_used (quick slots)
#include <vostok/animation/linear_interpolator.h>	// fov_factor interpolation
#include <vostok/render/facade/game_renderer.h>	// renderer().ui() in render
#include <vostok/render/facade/scene_renderer.h>	// add/remove_model
#include <vostok/render/engine/sources/trample_desc.h>
#include <vostok/physics/character_controller.h>	// physics_controller->set_crouch / jump
#include <vostok/game_core/hit_info.h>				// hit() builds a hit_info
#include <vostok/game_core/hit_initiator.h>			// initiator->id
#include <vostok/collision/animated_object.h>		// m_damage_collision->body_part_name + bone_collision_data
#include <vostok/physics/world.h>					// get_physics_world()->remove rigid body
#include <vostok/game_core/player_actions_subscriber.h>	// on_player_action notify loop (on_fire/jump/notify)
#include <vostok/game_core/hit_receiver.h>			// static_cast<hit_receiver const*>(this) in notify loop
#include <vostok/game_core/collision_geometry.h>
#include <vostok/game_core/usable_object.h>
#include <vostok/physics/base_physics_object.h>
#include <vostok/collision/game_object.h>
#include "stats.h"						// m_game.get_stats().set_player_*_speed (update_speed_info)
#include "stats_graph.h"				// m_*_speed_graph->add_value / average_value (update_speed_info)
#include <vostok/network_core/packet_reader.h>		// reader.r<T>() in deserialize
#include <vostok/console_command.h>					// the TU's cc_bool / cc_float registrations

namespace survarium {

static bool		s_show_animations					= true;
static float	s_ik_translation_time_value			= 0.1f;
static float	s_crosshair_size_value				= 0.1f;
static float	s_crosshair_line_length_value		= 0.025f;
static bool		s_first_person_animations_only;
static float	s_usable_objects_detection_distance	= 1.0f;
static bool		s_draw_linear_speed_graph_value;
static bool		s_show_server_player;
static bool		s_show_client_player				= true;

static console_commands::cc_bool	s_player_show_animations_command			( "show_animations",						s_show_animations,						true,	console_commands::command_type_user_specific );
static console_commands::cc_float	s_ik_translation_time_command				( "ik_translation_time",					s_ik_translation_time_value,			0.0f,	5.0f,	true,	console_commands::command_type_engine_internal );
static console_commands::cc_float	s_crosshair_size_command					( "crosshair_size",							s_crosshair_size_value,					0.0f,	1.0f,	true,	console_commands::command_type_engine_internal );
static console_commands::cc_float	s_crosshair_line_length_command				( "crosshair_line_length",					s_crosshair_line_length_value,			0.0f,	1.0f,	true,	console_commands::command_type_engine_internal );
static console_commands::cc_bool	s_first_person_animations_command			( "first_person_animations",				s_first_person_animations_only,			false,	console_commands::command_type_engine_internal );

static console_commands::cc_float	s_usable_objects_detection_distance_command	( "usable_objects_detection_distance",		s_usable_objects_detection_distance,	0.1f,	5.0f,	true,	console_commands::command_type_engine_internal );
static console_commands::cc_bool	s_draw_linear_speed_graph_cc				( "draw_linear_speed_graph",				s_draw_linear_speed_graph_value,		false,	console_commands::command_type_user_specific );
static console_commands::cc_bool	s_show_server_player_command				( "show_server_player",						s_show_server_player,					false,	console_commands::command_type_user_specific );
static console_commands::cc_bool	s_show_client_player_command				( "show_client_player",						s_show_client_player,					true,	console_commands::command_type_user_specific );

 player::player( player_creation_params const& params ) :
	base_player( params, params.game_scene->scheduler( ) ),
	m_history( *g_allocator, 64 ),
	m_stamina( params.initial_stamina ),
	m_stealth( params.initial_stealth ),
	m_last_frame_position( 0.0f, 0.0f, 0.0f ),
	m_damage_collision( params.damage_collision ),
	m_local_input_controller( NULL ),
	m_linear_speed_graph( NULL ),
	m_angular_speed_graph( NULL ),
	m_game_scene( *params.game_scene ),
	m_game( params.game_scene->get_game( ) ),
	m_empty_hands( params.empty_hands ),
	m_current_time_in_ms( 0 ),
	m_last_frame_rotation( 0.0f ),
	m_last_server_correction_time( 0 ),
	m_target_fov_factor( 1.0f ),
	m_start_fov_factor( 1.0f ),
	m_current_fov_factor( 1.0f ),
	m_start_fov_factor_change_time_in_ms( 0 ),
	foot_3rd_view_game_material_id( params.foot_3rd_view_game_material_id ),
	foot_1st_view_game_material_id( params.foot_1st_view_game_material_id ),
	m_show_server_player( false ),
	m_show_client_player( true ),
	m_is_visible( false ),
	m_is_first_born( true ),
	m_use_physics_controller_for_current( true ),
	m_player_head_visible( true ),
	m_game_ui( NULL ),
	m_is_demo_player( params.initial_info.is_demo_player ),
	m_force_bones_recompute( false )
{
	m_current.animation_player.enable_logging( is_local );

	size_t converted_chars_count = 0;
	mbstowcs_s( &converted_chars_count, profile_name, array_size( profile_name ), params.initial_info.profile->profile_name, _TRUNCATE );
	m_team_id = params.initial_info.profile->team;
	m_current.model = params.character_model;
	m_target.model = params.server_character_model;

	inventory( ).set_holder( this );

	m_current.physics_controller = physics::create_character_controller( *g_allocator, m_game_scene.get_physics_world( ) );
	m_current.physics_controller->initialize( );

	m_target.physics_controller = physics::create_character_controller( *g_allocator, m_game_scene.get_physics_world( ) );
	m_target.physics_controller->initialize( );

	m_damage_collision->set_owner( static_cast< hit_receiver* >( this ) );

	params.player_parameters->apply( this );
}

player::~player( )
{
	inventory( ).unset_holder( );

	DELETE( m_damage_collision );

	DELETE( m_target.physics_controller );
	DELETE( m_current.physics_controller );
}

// claude@NOTE: target and base share five statements and a ten-block CFG. The
// target hoists the inlined scene-renderer pointer before the visibility guards;
// base reloads it inside each guarded add_model call.
void player::add_models_to_scene( )
{
	render::scene_ptr scene = m_game_scene.render_scene( );

	if ( m_show_client_player )
		m_game_scene.scene_renderer( ).add_model( scene, m_current.model->m_render_model, m_current.transform );
	if ( m_show_server_player )
		m_game_scene.scene_renderer( ).add_model( scene, m_target.model->m_render_model, m_target.transform );
}

// claude@NOTE: target and base share five statements and a ten-block CFG. The
// target hoists the inlined scene-renderer pointer before the visibility guards;
// base reloads it inside each guarded remove_model call.
void player::remove_models_from_scene( )
{
	render::scene_ptr scene = m_game_scene.render_scene( );

	if ( m_show_client_player )
		m_game_scene.scene_renderer( ).remove_model( scene, m_current.model->m_render_model );
	if ( m_show_server_player )
		m_game_scene.scene_renderer( ).remove_model( scene, m_target.model->m_render_model );
}

// claude@NOTE: structure faithful (two if-guarded assign_game_ui virtual calls,
// NULL then m_game_ui). Capped by the intrusive_ptr<interactive_object> c_ptr()
// inline-vs-call: the target inlines c_ptr() (the guard + call fold into one stmt
// each), our base out-lines it, splitting the call into its own statement. Core
// template-instantiation inline decision, not steerable from player.cpp.
void player::on_before_active_object_changed(
	interactive_object_ptr const&		current_active_object,
	interactive_object_ptr const&		target_active_object
) const
{
	if ( current_active_object.c_ptr() )
		current_active_object.c_ptr()->assign_game_ui( NULL );

	if ( target_active_object.c_ptr() )
		target_active_object.c_ptr()->assign_game_ui( m_game_ui );
}

void player::insert_alive( )
{
	m_is_first_tick	= true;
	m_input			= player_input( );

	if ( m_local_input_controller )
		m_local_input_controller->set_position_direction(
			m_current.transform.c.xyz( ),
			math::create_rotation_y( m_current.transform.get_angles( math::rotation_zxy ).y ).k.xyz( ) );

	if ( !m_is_demo_player )
	{
		m_damage_collision->get_rigid_body( )->set_transform( m_current.transform );
		m_game_scene.get_physics_world( )->add( m_damage_collision->get_rigid_body( ), 0x68, 0x11 );
	}

	m_target.physics_controller->activate( m_target.transform );
	if ( m_use_physics_controller_for_current )
		m_current.physics_controller->activate( m_current.transform );

	damage_model( )->reset( );
	m_stamina.reset( );
	reset_fov_factor( );

	while ( !m_history.empty( ) )
		m_history.pop_tail( );
	m_is_alive = true;
}

void player::insert( const bool is_alive )
{
	m_has_been_inserted = true;

	if ( !m_is_demo_player )
		inventory( ).setup_from_profile(
			m_game.get_network_client( )->match_options( ).player_profiles[ id ], m_game.items_dictionary( ) );
	else
		inventory( ).setup_demo_profile( );

	if ( m_is_first_born )
		m_is_first_born = false;

	if ( !m_is_demo_player )
	{
		m_text = m_game_scene.text_manager( ).create_text_w( profile_name );
		m_text.set_visible( false );
		m_text.set_color( 0, 0xff, 0, 0xff );
	}

	m_current_time_in_ms = m_game.game_time_ms( );

	m_target.animation_player.reset( true );
	m_current.animation_player.reset( true );

	if ( inventory( ).item_in_slot( weapon1_slot ) )
		inventory( ).action( weapon1_slot, true );
	else if ( inventory( ).item_in_slot( weapon2_slot ) )
		inventory( ).action( weapon2_slot, true );
	else
		m_target_active_object = m_empty_hands;

	on_before_active_object_changed( m_current_active_object, m_target_active_object );
	m_current_active_object = m_target_active_object;
	m_current_active_object->activate( *this, m_game_scene );

	if ( is_alive )
		insert_alive( );

	m_is_visible = true;
	add_models_to_scene( );
	m_force_animation_selection = true;
	m_force_bones_recompute = true;
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
			m_game.get_network_client( )->match_options( ).player_profiles[ id ], m_game.items_dictionary( ) );

	inventory( ).remove( );

	if ( m_game.get_network_client( )->is_player_current( id ) )
		m_game.get_network_client( )->detach_from_player( );
}

float4x4 player::get_transform_for_animation_player( pcvoid const animated_object, float4x4 const& character_transform ) const
{
	if ( animated_object == this )
		return character_transform;

	return m_current_active_object->transform( );
}

void player::apply_input( client_player_state& player_state, float2 const& rotation_to_apply )
{
	player_state.transform =
		math::create_rotation( player_state.transform.get_angles_xyz() ) *
		math::create_rotation( float3( 0.f, rotation_to_apply.x, 0.f ) ) *
		math::create_translation( player_state.transform.c.xyz() );

	player_state.look_pitch = math::clamp_r( player_state.look_pitch + rotation_to_apply.y, -1.f, 1.f );
}

// claude@NOTE: STRUCTURE MATCH (2 stmts, 1 local rotation_to_apply). Builds the
// per-component rotation increment ( accel * dt * 0.5 + prev_velocity ) * dt and
// forwards to the 2-arg apply_input. Byte residual is inherited from that callee's
// create_rotation/mul4x3 inline schedule (see the 2-arg note) plus the LTCG custom
// calling convention (time_delta/accel/velocity arrive in xmm0/eax/ecx, not slots).
void player::apply_input(
	client_player_state&	player_state,
	float2 const&			previous_velocity,
	float2 const&			current_acceleration,
	const float				time_delta
)
{
	float2 rotation_to_apply(
		( current_acceleration.x * time_delta * 0.5f + previous_velocity.x ) * time_delta,
		( current_acceleration.y * time_delta * 0.5f + previous_velocity.y ) * time_delta
	);

	apply_input( player_state, rotation_to_apply );
}

void player::serialize_current_state( const u32 current_time_in_ms )
{
	client_player_history_item& item = m_history.new_item( );
	item.time_in_ms						= current_time_in_ms;
	item.action.input					= m_input;
	item.action.state.transform			= m_target.transform;
	item.action.state.look_pitch		= m_target.look_pitch;
	item.action.weapon_state.slot_id	= inventory( ).get_active_slot( );
}

void player::compute_bones( const u32 current_time_in_ms )
{
	u32 const bones_count = m_current.model->m_skeleton->get_non_root_bones_count( );
	float4x4* const bones_matrices = (float4x4*)alloca( bones_count * sizeof( float4x4 ) );

	m_current_active_object->update_bones_matrices(
		m_current.model->m_skeleton,
		bones_matrices,
		bones_count,
		current_time_in_ms,
		m_character_head_transform,
		m_root_transform,
		m_current.animation_player
	);
	m_damage_collision->update( bones_matrices, bones_matrices + bones_count );

	render::game::renderer& r = m_game_scene.renderer( );
	if ( m_show_client_player )
		r.scene( ).update_skeleton( m_current.model->m_render_model, bones_matrices, bones_count );

	if ( m_show_server_player )
	{
		m_target.animation_player.compute_bones_matrices(
			*m_target.model->m_skeleton,
			bones_matrices,
			bones_matrices + bones_count,
			this,
			NULL
		);
		r.scene( ).update_skeleton( m_target.model->m_render_model, bones_matrices, bones_count );
	}
}

void player::render( const u32 __formal, const u32 current_time_in_ms )
{
	render::base_scene_ptr scene = m_game_scene.render_scene( );

	if ( s_draw_linear_speed_graph_value )
		m_linear_speed_graph->render( m_game_scene.renderer( ).ui( ), m_game_scene.render_scene_view( ), 510, 256, __formal, 1004, 10 );

	if ( m_show_server_player != s_show_server_player )
	{
		m_show_server_player = s_show_server_player;
		if ( !s_show_server_player )
			m_game_scene.scene_renderer( ).remove_model( scene, m_target.model->m_render_model );
		else
			m_game_scene.scene_renderer( ).add_model( scene, m_target.model->m_render_model, m_target.transform );
	}

	if ( m_show_client_player != s_show_client_player )
	{
		m_show_client_player = s_show_client_player;
		if ( !s_show_client_player )
		{
			m_game_scene.scene_renderer( ).remove_model( scene, m_current.model->m_render_model );
			m_current_active_object->on_player_model_removed( );
		}
		else
		{
			m_game_scene.scene_renderer( ).add_model( scene, m_current.model->m_render_model, m_current.transform );
			m_current_active_object->on_player_model_added( );
		}
	}

	if ( m_is_visible )
	{
		if ( m_show_client_player )
		{
			m_game_scene.scene_renderer( ).update_model( scene, m_current.model->m_render_model, m_current.transform );
			if ( !m_current.transform.c.xyz( ).is_similar( m_current.previous_transform.c.xyz( ), 0.025f ) )
			{
				render::trample_desc trample;
				trample.position = m_current.transform.c.xyz( );
				trample.radius = 0.f;
				trample.multiplier = 0.5f;
				m_game_scene.scene_renderer( ).add_vegetation_trample( scene, trample );
			}
		}

		if ( m_show_server_player )
			m_game_scene.scene_renderer( ).update_model( scene, m_target.model->m_render_model, m_target.transform );
	}

	compute_bones( current_time_in_ms );
	m_game_scene.get_physics_world( )->move( m_damage_collision->get_rigid_body( ), m_current.transform );

	if ( m_game.get_network_client( )->is_player_current( id ) &&
		( s_first_person_animations_only || m_local_input_controller->input_mode( ) == first_person_mode ) )
	{
		render_crosshair_info( );
		set_head_visibility( false );
	}
	else
		set_head_visibility( true );
}

void player::render_crosshair_info( )
{
	float3 ray_from = m_character_head_transform.c.xyz( );
	float3 ray_dir = m_character_head_transform.k.xyz( );

	render::base_scene_ptr scene = m_game_scene.render_scene( );
	physics::closest_ray_result result = m_game_scene.get_physics_world( )->ray_test( ray_from, ray_dir, 1000.f, 16, 8 );
	if ( result.object )
	{
		float const distance = math::length( result.hit_point_world - ray_from );
		m_game.get_stats( ).set_crosshair_info( distance );
	}
}

void player::update_speed_info( )
{
	float3 const movement	= m_current.transform.c.xyz( ) - m_last_frame_position;
	m_last_frame_position	= m_current.transform.c.xyz( );
	if ( m_linear_speed_graph )
	{
		m_linear_speed_graph->add_value( m_game.last_frame_time( ), math::length( movement ) );
		m_game.get_stats( ).set_player_linear_speed( m_linear_speed_graph->average_value( ) );
	}

	const float rotation = m_current.transform.get_angles( math::rotation_zxy ).y;

	const float last_frame_angular_displacement	= math::abs( rotation - m_last_frame_rotation );
	m_last_frame_rotation						= rotation;
	if ( m_angular_speed_graph )
	{
		m_angular_speed_graph->add_value( m_game.last_frame_time( ), last_frame_angular_displacement );
		m_game.get_stats( ).set_player_angular_speed( math::rad2deg( m_angular_speed_graph->average_value( ) ) );
	}
}

// claude@NOTE: paired ~93%. Reads the bullet controller transform (bt_character_controller::
// get_transform = from_bullet(...) inlined), keeps its translation while taking state.transform's
// rotation, pushes it back (set_transform = from_vostok(...) inlined) and feeds the residual
// position delta as the walk direction (set_walk_direction = set_desired_walk_vector(from_vostok)
// inlined). Residual is a single statement-fusion boundary: the target keeps the rotation-copy
// temp build (L726) and set_transform (L731) as two statements; our /Od compile fuses the c
// override into the set_transform statement (1 fewer stmt). Non-steerable line-boundary artifact.
void player::set_physics_controller_walk_vector( client_player_state& state )
{
	float4x4 physics_transform = state.physics_controller->get_transform( );

	float4x4 transform = state.transform;
	transform.c.xyz( ) = physics_transform.c.xyz( );

	state.physics_controller->set_transform( transform );

	state.physics_controller->set_walk_direction( state.transform.c.xyz( ) - physics_transform.c.xyz( ) );
}

void player::detect_usable_objects( const u32 current_time_in_ms )
{
	usable_object_user_data( )->owner			= this;
	usable_object_user_data( )->current_time_ms	= current_time_in_ms;

	physics::closest_ray_result ray_result = m_game_scene.get_physics_world( )->ray_test(
		m_character_head_transform.c.xyz( ),
		m_character_head_transform.k.xyz( ),
		s_usable_objects_detection_distance,
		0x100,
		0x80
	);

	if ( ray_result.object )
	{
		collision_geometry* geometry = ray_result.object->user_data->cast_to_collision_geometry( );
		vectora< usable_object* > results( g_allocator );
		geometry->query_objects_by_type( results, &collision_geometry_subscriber::cast_to_usable );

		usable_object* object = results[0];
		if ( input( ).actions_mask & 0x10000000 )
		{
			if ( usable_object_user_data( )->current_object )
			{
				if ( usable_object_user_data( )->current_object == object )
					object->use_execute( usable_object_user_data( ) );
				else
				{
					usable_object_user_data( )->current_object->use_finalize( usable_object_user_data( ) );
					object->use_initialize( usable_object_user_data( ) );
				}
			}
			else
				object->use_initialize( usable_object_user_data( ) );
		}
		else
		{
			if ( usable_object_user_data( )->current_object )
				usable_object_user_data( )->current_object->use_finalize( usable_object_user_data( ) );

			if ( m_game.get_network_client( )->is_player_current( id ) )
				if ( m_game_ui )
				{
					pcstr const info = object->use_info( usable_object_user_data( ) );
					m_game_ui->set_using_info_message( info );
				}
		}
	}
	else if ( usable_object_user_data( )->current_object )
		usable_object_user_data( )->current_object->use_finalize( usable_object_user_data( ) );
}

void player::use_ladder( ladder* __formal )
{
	NOT_IMPLEMENTED( __formal );
}

void player::apply_damage_model_affect(
	pcstr							part_name,
	const hit_affects_type_enum		affect,
	const affect_event_type_enum	event_type
)
{
	damage_model( )->apply_affect( part_name, affect, event_type );

	if ( m_game_ui && m_game.get_network_client( )->is_player_current( id ) )
		m_game_ui->on_damage_affect_applying( part_name, affect, event_type );
}

void player::set_character_transform( float3 const& position, const float orientation, const float look_pitch )
{
	m_target.previous_transform = m_target.transform = m_current.previous_transform = m_current.transform =
		math::mul4x3( math::create_rotation_y( orientation ), math::create_translation( position ) );
	m_target.look_pitch = m_current.look_pitch = look_pitch;
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
	m_game.get_network_client( )->on_player_hit_received( info );
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
	m_game.get_network_client( )->on_player_hit_received( info );
}

void player::apply_hit_directly( hit_info const& info, const u32 current_time_in_ms )
{
	if ( !damage_model( )->hit_body_part(
		info.hit_initiator,
		info.body_part_name.c_str( ),
		info.damage_type.c_str( ),
		info.amount,
		info.armor_piercing,
		current_time_in_ms,
		info.bullet
	) )
		return;

	if ( m_game.get_network_client( )->is_player_current( info.hit_initiator ) )
		m_game.get_game_world( ).game_ui.on_enemy_hitted( );

	if ( m_game_ui && m_game.get_network_client( )->is_player_current( id ) )
	{
		if ( info.being_hit == id && info.hit_initiator != u8( -1 ) )
		{
			player_ptr initiator_ptr = info.hit_initiator == id
				? player_ptr( this )
				: m_game.get_network_client( )->get_player( info.hit_initiator );
			m_game_ui->on_hit_from_pos( initiator_ptr->get_transform( ).c.xyz( ) );
		}
	}

	player_actions_subscribers::iterator i = m_player_actions_subscribers.begin( );
	player_actions_subscribers::iterator e = m_player_actions_subscribers.end( );
	for ( ; i != e; ++i )
		( *i )->on_player_action( this, player_actions_subscriber::hit, info.amount );
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

void player::update_camera( )
{
	if ( !m_local_input_controller )
		return;

	if ( m_start_fov_factor != m_target_fov_factor )
	{
		const float interpolation_time = ( m_current_time_in_ms - m_start_fov_factor_change_time_in_ms ) * math::epsilon_3;
		m_current_fov_factor = interpolation_time >= m_fov_factor_transition_time
			? m_target_fov_factor
			: m_start_fov_factor + animation::linear_interpolator( m_fov_factor_transition_time ).interpolated_value( interpolation_time ) * ( m_target_fov_factor - m_start_fov_factor );

		m_local_input_controller->set_fov_factor( m_current_fov_factor );

		if ( interpolation_time >= m_fov_factor_transition_time )
		{
			m_start_fov_factor = m_target_fov_factor;
			if ( m_target_fov_factor == 1.0f )
				set_near_plane_factor( 1.0f );
		}
	}
	else
		m_local_input_controller->set_fov_factor( m_target_fov_factor );

	if ( m_local_input_controller->input_mode( ) == first_person_mode )
		m_local_input_controller->update_inverted_view( m_character_head_transform );
	else
	{
		float4x4 transform = m_root_transform;
		transform.c.xyz( ) += m_root_transform.j.xyz( ) * 1.4f;
		m_local_input_controller->update_inverted_view( transform );
	}
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

float player::get_speed( ) const
{
	return m_linear_speed_graph->average_value( );
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
		std::remove( m_player_actions_subscribers.begin( ), m_player_actions_subscribers.end( ), subscriber )
	);
}

namespace {

// the 3-arg notify variant: per subscriber, if the player moved (abs(movement)
// past epsilon_3) report run while sprinting else walk, with the movement amount.
struct player_movement_notifier {
	hit_receiver const*		m_receiver;
	bool					m_sprinting;
	float					m_movement;
	inline	player_movement_notifier( hit_receiver const* receiver, bool sprinting, float movement ) :
		m_receiver( receiver ), m_sprinting( sprinting ), m_movement( movement ) { }
	inline	void operator( )( player_actions_subscriber* subscriber ) const {
		if ( math::abs( m_movement ) >= math::epsilon_3 )
		{
			if ( m_sprinting )
				subscriber->on_player_action( m_receiver, player_actions_subscriber::sprint, m_movement );
			else
				subscriber->on_player_action( m_receiver, player_actions_subscriber::run, m_movement );
		}
	}
};

} // namespace

// claude@NOTE: std::for_each over m_player_actions_subscribers (0 iterator locals);
// the only named local `movement` survives. run/walk = header enum 1/0. Receiver
// is static_cast<hit_receiver const*>(this) (player+0x38). Same STLport for_each
// inline-vs-call wall as on_fire/jump - the functor body inlines in the target
// (its 1113/1115/1117-1120 statements) but our for_each stays out-of-line.
void player::notify_actions_subscribers( )
{
	const float movement = math::length( m_current.transform.c.xyz( ) - m_last_frame_position );
	std::for_each(
		m_player_actions_subscribers.begin( ),
		m_player_actions_subscribers.end( ),
		player_movement_notifier( this, m_input.is_trying_to_sprint( ), movement ) );
}

namespace {

struct player_action_notifier {
	hit_receiver const*						m_receiver;
	player_actions_subscriber::action		m_action;
	inline	player_action_notifier( hit_receiver const* receiver, player_actions_subscriber::action action ) :
		m_receiver( receiver ), m_action( action ) { }
	inline	void operator( )( player_actions_subscriber* subscriber ) const {
		subscriber->on_player_action( m_receiver, m_action, 0.f );
	}
};

} // namespace

// claude@NOTE: std::for_each over m_player_actions_subscribers, 0 named locals
// (the iterator lives in the inlined for_each, not on_fire). The target INLINES
// the loop (begin/end direct member reads + the loop body = its 4 separate
// statements 1127-1131); our STLport for_each / vector::begin()/end() stay
// OUT-OF-LINE (one `call stlp_std::for_each`), so we emit 1 statement vs 4. The
// for_each functor form is structurally correct; the residual is the STLport
// header-template inline boundary, not steerable from player.cpp. The retail
// action value is the named `shoot` enumerator.
void player::on_fire( )
{
	std::for_each(
		m_player_actions_subscribers.begin( ),
		m_player_actions_subscribers.end( ),
		player_action_notifier( this, player_actions_subscriber::shoot ) );
}

// claude@NOTE: stand_up() (virtual, this->vtable+0x44) then the same for_each
// subscriber notify with the named `jump` action, then
// bullet_character_controller::jump on both controllers.
// Same STLport for_each inline-vs-call wall as on_fire for the notify loop.
void player::jump( )
{
	stand_up( );
	std::for_each(
		m_player_actions_subscribers.begin( ),
		m_player_actions_subscribers.end( ),
		player_action_notifier( this, player_actions_subscriber::jump ) );

	m_target.physics_controller->jump( );
	if ( m_use_physics_controller_for_current )
		m_current.physics_controller->jump( );
}

void player::end_jump( )
{
	m_target.physics_controller->end_jump( );
	if ( m_use_physics_controller_for_current )
		m_current.physics_controller->end_jump( );
}

// claude@NOTE: both functions have the target three-statement source shape.
// The target inlines bt_character_controller::set_crouch through the Bullet
// crouch-state/list cleanup; base keeps the physics-owned wrapper call.
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

bool player::set_new_active_item( inventory_item_ptr const& item )
{
	interactive_object_ptr const new_active_item = item.c_ptr( );

	if ( m_target_active_object != new_active_item )
		force_animation_selection( );

	m_target_active_object = new_active_item;
	return true;
}

static const profile_slot_enum accept_slots[] = {
	quick_slot1, quick_slot2, quick_slot3, quick_slot4, quick_slot5, quick_slot6
};

void player::take_inventory_item( inventory_item_ptr const& item )
{
	for ( u32 i = 0; i < array_size( accept_slots ); ++i )
	{
		if ( !inventory( ).item_in_slot( accept_slots[ i ] ) )
		{
			inventory( ).set_item( accept_slots[ i ], item );

			if ( m_game_ui && m_game.get_network_client( )->is_player_current( id ) )
				m_game_ui->fill_quick_slots( );

			return;
		}
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

// gold keeps the buffer ctor out of line at the select_animations site (folds
// with the two-store group); a TU-local noinline forwarder reproduces that
static __declspec( noinline ) mutable_buffer make_stack_buffer( pvoid const data, u32 const size )
{
	return mutable_buffer( data, size );
}

void player::select_animations( const u32 current_time_in_ms )
{
	tick_active_object( );

	const bool is_current = m_game.get_network_client( )->is_player_current( id );
	const input_mode_type_enum input_mode = s_first_person_animations_only ? first_person_mode : ( is_current ? m_local_input_controller->input_mode( ) : third_person_mode );

	mutable_buffer buffer = make_stack_buffer( ALLOCA( 0x4000 ), 0x4000 );
	animation::mixing::expression expression = m_current_active_object->selected_animations( buffer, input_mode == third_person_mode );

	m_target.animation_player.set_target_and_tick( expression, current_time_in_ms, boost::bind( &player::get_transform_for_animation_player, this, _1, boost::cref( m_target.transform ) ) );

	m_current.animation_player.set_target_and_tick( expression, current_time_in_ms, boost::bind( &player::get_transform_for_animation_player, this, _1, boost::cref( m_current.transform ) ) );
}

static float s_death_camera_yaw			= 0.6f;
static float s_death_camera_pitch		= -1.2f;
static float s_death_camera_distance	= 1.0f;

static console_commands::cc_float cc_death_camera_yaw		( "death_camera_yaw",		s_death_camera_yaw,			-math::pi,	math::pi,	true,	console_commands::command_type_engine_internal );
static console_commands::cc_float cc_death_camera_pitch		( "death_camera_pitch",		s_death_camera_pitch,		-math::pi,	math::pi,	true,	console_commands::command_type_engine_internal );
static console_commands::cc_float cc_death_camera_distance	( "death_camera_distance",	s_death_camera_distance,	0.0f,		1000.0f,	true,	console_commands::command_type_engine_internal );

// claude@NOTE: target and base share the twelve-block camera/UI sequence. The
// target retains the inlined is_player_current entry statement while base folds
// it into the prologue; the final select_animations differs by LTCG tail-calling.
void player::kill( const u32 current_time_in_ms )
{
	const bool is_local_player = m_game.get_network_client( )->is_player_current( id );

	remove_alive( );
	on_player_death( );

	if ( is_local_player )
	{
		m_local_input_controller->set_input_mode( third_person_mode );
		m_local_input_controller->set_yaw_pitch_distance( s_death_camera_yaw, s_death_camera_pitch, s_death_camera_distance );
		m_local_input_controller->set_key_binder_context( 16 );
		static_cast< game_world& >( m_game_scene ).switch_camera_mode( m_local_input_controller->input_mode( ) );
		m_game_ui->show_ammo_indicator( false ); m_game_ui->show_quick_slots( false );
	}

	select_animations( current_time_in_ms );
}

// claude@NOTE: structure matches except the assign_game_ui (L1493) + member-store
// block (L1519) merge into our prologue (target keeps them as 2 statements separated
// by a source-line gap we cannot reproduce) and the intrusive_ptr/assign inline; the
// if-alive input-mode/key-binder gate and the camera wiring all pair.
void player::attach_controller(
	player_input_handler*		handler,
	stats_graph*				linear_speed,
	stats_graph*				angular_speed,
	game_world_ui*				ui
)
{
	m_current_active_object->assign_game_ui( m_game_ui = ui );

	m_local_input_controller	= handler;
	m_linear_speed_graph		= linear_speed;
	m_angular_speed_graph		= angular_speed;

	if ( m_is_alive )
	{
		m_local_input_controller->set_input_mode( first_person_mode );
		m_local_input_controller->set_key_binder_context( 1 );
	}
	else
	{
		m_local_input_controller->set_input_mode( third_person_mode );
		m_local_input_controller->set_key_binder_context( 16 );
	}

	static_cast< game_world& >( m_game_scene ).set_local_player_camera( m_local_input_controller );
	static_cast< game_world& >( m_game_scene ).switch_camera_mode( m_local_input_controller->input_mode( ) );
	m_force_animation_selection = true;
}

// claude@NOTE: 4-stmt structure matches. Byte residual is the intrusive_ptr operator-bool
// inline (target folds the m_current_active_object null-check into the assign_game_ui
// statement / prologue; our base emits a separate `if` line) - the same intrusive_ptr
// accessor inline-vs-call wall as skeleton()/the quick-slot fns; not TU-steerable.
void player::detach_controller( )
{
	if ( m_current_active_object )
		m_current_active_object->assign_game_ui( NULL );

	m_game_ui					= NULL;
	m_local_input_controller	= NULL;
	m_linear_speed_graph		= NULL;
	m_angular_speed_graph		= NULL;
	static_cast< game_world& >( m_game_scene ).set_local_player_camera( NULL );

	static_cast< game_world& >( m_game_scene ).switch_to_free_fly_camera( );

	m_force_animation_selection = true;
}

void player::set_head_visibility( bool is_visible )
{
	if ( m_player_head_visible == is_visible )
		return;

	m_player_head_visible = is_visible;

	u32 mode = 2;
	if ( is_visible )
		mode = 3;

	m_game_scene.scene_renderer( ).set_model_visible( m_current.model->m_render_model, 0, mode );
	m_game_scene.scene_renderer( ).set_model_visible( m_current.model->m_render_model, 6, mode );
	m_game_scene.scene_renderer( ).set_model_visible( m_current.model->m_render_model, 7, mode );
	m_game_scene.scene_renderer( ).set_model_visible( m_current.model->m_render_model, 5, mode );
}

// claude@NOTE: file-static default callback; target is `xor eax,eax; ret`
// (returns callback_return_type_call_me_again = 0). The two subscribe_animation_player
// overloads below reference it (as the m_target.animation_player.subscribe callback).
animation::callback_return_type_enum empty_callback( animation::animation_callback_params& params )
{
	VOSTOK_UNREFERENCED_PARAMETER( params );
	return animation::callback_return_type_call_me_again;
}

// claude@NOTE: both overloads have the target two statements and ten-block CFG.
// Their three-byte size residual is argument/register scheduling around the two
// subscribe calls and the temporary empty_callback boost::function.
void player::subscribe_animation_player(
	pcstr			channel_id,
	boost::function< enum animation::callback_return_type_enum( animation::animation_callback_params& ) > const&	callback,
	pcvoid			callback_uid,
	resources::managed_resource_ptr const&	animation,
	const u8		event_type,
	pcvoid const	animated_object
)
{
	m_current.animation_player.subscribe( channel_id, callback, callback_uid, animation, event_type, animated_object );
	m_target.animation_player.subscribe( channel_id, empty_callback, callback_uid, animation, event_type, animated_object );
}

void player::subscribe_animation_player(
	animation::reserved_channel_ids_enum	channel_id,
	boost::function< enum animation::callback_return_type_enum( animation::animation_callback_params& ) > const&	callback,
	pcvoid const	callback_uid,
	resources::managed_resource_ptr const&	animation,
	pcvoid const	animated_object
)
{
	m_current.animation_player.subscribe( channel_id, callback, callback_uid, animation, animated_object );
	m_target.animation_player.subscribe( channel_id, empty_callback, callback_uid, animation, animated_object );
}

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

void player::deserialize( network_core::packet_reader& reader )
{
	float3 const position		= reader.r< float3 >( );
	const float orientation		= reader.r< float >( );
	const float look_pitch		= reader.r< float >( );
	const bool is_alive			= reader.r< bool >( );

	if ( m_has_been_inserted )
		remove( );

	set_character_transform( position, orientation, look_pitch );
	insert( is_alive );

	const profile_slot_enum server_current_active_slot	= (profile_slot_enum)reader.r< u8 >( );
	const profile_slot_enum server_target_active_slot	= (profile_slot_enum)reader.r< u8 >( );

	interactive_object_ptr server_current_active_object = inventory( ).item_in_slot( server_current_active_slot ).c_ptr( );
	if ( m_current_active_object != server_current_active_object )
	{
		inventory( ).action( server_current_active_slot, true );
		m_current_active_object->deactivate( );
		on_before_active_object_changed( m_current_active_object, server_current_active_object );
		m_current_active_object = server_current_active_object;
		m_current_active_object->activate( *this, player::get_engine( ) );
	}

	m_target_active_object = interactive_object_ptr( inventory( ).item_in_slot( server_target_active_slot ).c_ptr( ) );

	stamina( ).deserialize( reader );
	inventory( ).deserialize( reader );
}

} // namespace survarium
