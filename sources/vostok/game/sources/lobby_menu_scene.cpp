// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "lobby_menu.h"
#include "profile_player_character.h"
#include "profile_character.h"
#include "player.h" // complete type for player_ptr (intrusive_ptr<player>) dtor
#include <vostok/animation/mixing_addition_lexeme.h>
#include <vostok/animation/mixing_animation_lexeme.h>
#include <vostok/animation/mixing_animation_lexeme_parameters.h>
#include <vostok/game_core/player_initial_info.h>
#include <vostok/game_core/player_profile.h>
#include <vostok/resources.h>
#include <vostok/resources_queries_result.h>	// queries_result [] / size / is_successful
#include <vostok/resources_query_result.h>		// query_result_for_user accessors
#include <vostok/render/facade/common_types.h>
#include <vostok/render/facade/scene_renderer.h>	// *_ready: add_model / remove_model
#include <vostok/sound/sound_scene_creation_params.h>
#include <boost/bind.hpp>

namespace survarium {

// STATE[STUB]
// claude@NOTE: PARKED. Body iterates m_lobby_game_project's scenes / static_collisions
// (simple_game_project members not declared in our tree) calling scene_renderer::remove_model
// + static_collision::remove, then releases m_character (profile_player_character) and calls
// a base_game_scene virtual. Walled by simple_game_project's scene/collision container layout.
// NEXT: declare simple_game_project's scene_view + static_collision arrays, then reconstruct.
void lobby_menu::clear_resources( )
{
}

void lobby_menu::query_scene_resources( )
{
	render::scene_configuration render_configuration;
	render_configuration.m_create_terrain			= false;
	render_configuration.m_create_particle_world	= true;
	render_configuration.m_has_clouds				= false;
	render_configuration.m_create_speedtree_world	= false;
	render_configuration.m_create_grass_world		= false;
	render_configuration.m_sky_enabled				= true;
	render_configuration.m_use_occlusion_culling	= false;

	resources::user_data_variant temp_data;
	temp_data.set( render_configuration );

	resources::user_data_variant lobby_scene_data;
	lobby_scene_data.set( static_cast< base_game_scene* >( this ) );

	sound::sound_scene_creation_params sound_configuration;
	sound_configuration.proxies_count		= 5;
	sound_configuration.propagators_count	= 5;
	sound_configuration.receivers_count		= 2;

	resources::user_data_variant sound_scene_data;
	sound_scene_data.set( sound_configuration );

	resources::user_data_variant const* data[] =
	{
		&temp_data,
		NULL,
		&sound_scene_data,
		&lobby_scene_data,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL
	};

	resources::request requests[] =
	{
		{ "game_scene", resources::scene_class },
		{ "game_scene_view", resources::scene_view_class },
		{ "sound_scene", resources::sound_scene_class },
		{ "lobby_scene", resources::client_game_project_class },
		{ "resources/flash_movies/cursor.swf", resources::flash_movie_class },
		{ "resources/flash_movies/inventory.swf", resources::flash_movie_class },
		{ "resources/flash_movies/message.swf", resources::flash_movie_class },
		{ "resources/flash_movies/match_making.swf", resources::flash_movie_class },
		{ "resources/gameplay/players/default.player", resources::binary_config_class }
	};

	resources::query_resources(
		requests,
		boost::bind( &lobby_menu::on_render_scenes_ready, this, _1 ),
		g_allocator,
		data
	);
}

profile_player_character::profile_player_character( lobby_menu& lobby_menu )
	: m_lobby_menu( lobby_menu )
{
}

void profile_player_character::update( const u32 current_time_in_ms )
{
	if ( m_player )
		m_player->tick( current_time_in_ms );
}

void profile_player_character::clear_resources( )
{
	if ( m_player )
		m_player->remove( );

	m_player = NULL;
}

void profile_player_character::profile_changed( player_profile const* profile )
{
	query_profile_contents( profile );
}

void profile_player_character::query_profile_contents( player_profile const* profile )
{
	player_profile* profile_to_cook = VOSTOK_NEW_IMPL( ::survarium::g_allocator, player_profile );
	memory::copy( profile_to_cook, sizeof( player_profile ), profile, sizeof( player_profile ) );

	player_initial_info info;
	info.id = 0;
	info.is_demo_player = true;
	info.game_scene = &m_lobby_menu;
	info.profile = profile_to_cook;

	variant< 32 > ud;
	ud.set( info );

	resources::query_resource(
		"gameplay/players/default.player",
		resources::player_class,
		boost::bind( &profile_player_character::on_player_ready, this, _1, profile_to_cook ),
		g_allocator,
		&ud
	);
}

void profile_player_character::on_player_ready( resources::queries_result& data, player_profile* profile_to_cook )
{
	VOSTOK_DELETE_IMPL( ::survarium::g_allocator, profile_to_cook );

	clear_resources( );
	m_player = static_cast_resource_ptr< player_ptr >( data[0].get_unmanaged_resource( ) );
	m_player->set_character_transform( float3( 0.f, -0.2f, 0.f ), math::pi, 0.f );
	m_player->insert( true );
}

// sushi@TODO: recover update's guards, tick, matrix publication and weapon visibility beyond this expression model.
void profile_character::update( const u32 current_time_in_ms )
{
	mutable_buffer					buffer(
		ALLOCA( animation::animation_player::stack_buffer_size ),
		animation::animation_player::stack_buffer_size
	);

	animation::mixing::animation_lexeme	character_lexeme(
		animation::mixing::animation_lexeme_parameters(
			buffer,
			NULL,
			m_character_animation[ 0 ],
			NULL,
			NULL
		).animated_object( this )
	);

	animation::mixing::animation_lexeme	weapon_lexeme(
		animation::mixing::animation_lexeme_parameters(
			buffer,
			NULL,
			m_character_animation[ 1 ],
			NULL,
			NULL
		).animated_object( this )
	);

	animation::mixing::expression		animation_expression( character_lexeme );
	animation_expression				= animation_expression + weapon_lexeme;

	VOSTOK_UNREFERENCED_PARAMETER		( current_time_in_ms );
}

void profile_character::character_model_ready( resources::queries_result& data )
{
	if ( m_character_model )
	{
		m_scene_renderer.remove_model( m_scene, m_character_model->m_render_model );
		m_character_model	= 0;
	}

	if ( data.is_successful( ) )
	{
		m_character_model	= static_cast_resource_ptr< render::skeleton_model_ptr >( data[ 0 ].get_unmanaged_resource( ) );
		m_skeleton			= m_character_model->m_skeleton;
		m_weapon_bone_index	= m_skeleton->get_bone_index( "Weapon" ) - m_skeleton->get_root_bones_count( );
		m_scene_renderer.add_model( m_scene, m_character_model->m_render_model, m_initial_matrix );
	}
}

// 3 queries per weapon slot: [3i] model, [3i+1] animation, [3i+2] addon; the
// addon attach point resolves off the MODEL's render model into m_addon_locator
void profile_character::weapon_resources_ready( resources::queries_result& data )
{
	for ( u32 i = 0; i < 2; ++i )
	{
		if ( m_preview_weapon[ i ].m_visible )
		{
			if ( m_preview_weapon[ i ].m_model )
				m_scene_renderer.remove_model( m_scene, m_preview_weapon[ i ].m_model->m_render_model );
			if ( m_preview_weapon[ i ].m_addon )
				m_scene_renderer.remove_model( m_scene, m_preview_weapon[ i ].m_addon->m_render_model );
		}

		m_preview_weapon[ i ].m_model		= 0;
		m_preview_weapon[ i ].m_addon		= 0;
		m_preview_weapon[ i ].m_animation	= 0;
		m_preview_weapon[ i ].m_visible		= false;

		if ( data[ 3 * i ].is_successful( ) )
		{
			m_preview_weapon[ i ].m_model		= static_cast_resource_ptr< render::skeleton_model_ptr >( data[ 3 * i ].get_unmanaged_resource( ) );
			m_preview_weapon[ i ].m_animation	= data[ 3 * i + 1 ].get_managed_resource( );
		}

		if ( data[ 3 * i + 2 ].is_successful( ) )
		{
			m_preview_weapon[ i ].m_addon		= static_cast_resource_ptr< render::static_model_ptr >( data[ 3 * i + 2 ].get_unmanaged_resource( ) );
			m_preview_weapon[ i ].m_model->m_render_model->get_locator( "scope_point", m_preview_weapon[ i ].m_addon_locator );
		}
	}
}

#line 298
void profile_character::character_animation_ready( resources::queries_result& data )
{
	for ( u32 i = 0; i < data.size( ); ++i )
		if ( !data[ i ].is_successful( ) )
			LOG_ERROR( "Wrong data while querying [%s]", data[ i ].get_requested_path( ) );

	m_character_animation[ 0 ] = data[ 0 ].get_managed_resource( );
	m_character_animation[ 1 ] = data[ 1 ].get_managed_resource( );
}

// internal-linkage scene-load functor (the original is a file-local static: the PDB
// records only an S_LPROC32 for it, no mangled COFF symbol). Originally referenced by
// query_scene_resources (still a STUB), so pin its address from this TU to keep
static float4x4 identity_transform_functor( pcvoid )
{
	return float4x4( ).identity( );
}

void pin_lobby_menu_scene_statics( )
{
	static pcvoid volatile s_sink = 0;
	static volatile bool s_run = false;
	if ( s_run )
		s_sink = ( pcvoid )&identity_transform_functor;
}

} // namespace survarium
