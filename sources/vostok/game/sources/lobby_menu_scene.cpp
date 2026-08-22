////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "lobby_menu.h"
// this compiland also holds the out-of-line bodies of these two (batch 6) types
#include "profile_player_character.h"
#include "profile_character.h"
#include "player.h" // complete type for player_ptr (intrusive_ptr<player>) dtor
#include <vostok/animation/mixing_addition_lexeme.h>
#include <vostok/animation/mixing_animation_lexeme.h>
#include <vostok/animation/mixing_animation_lexeme_parameters.h>
#include <vostok/resources_queries_result.h>	// queries_result [] / size / is_successful
#include <vostok/resources_query_result.h>		// query_result_for_user accessors
#include <vostok/render/facade/scene_renderer.h>	// *_ready: add_model / remove_model

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

// STATE[STUB]
// claude@NOTE: PARKED. 51-stmt body building sound::sound_scene_creation_params +
// render::scene_configuration, a resources::request[9] array and variant<32> data[10],
// then resources::query_resources with an on_render_scenes_ready boost::bind callback.
// Named LOCALS (structure): variant<32> sound_scene_data; variant<32> const* data[10];
// variant<32> temp_data; sound::sound_scene_creation_params sound_configuration;
// resources::request[9] requests; render::scene_configuration render_configuration;
// variant<32> lobby_scene_data. Walled by the request-array assembly + scene_configuration /
// sound_scene_creation_params field layout (not declared in our tree). NEXT: recover the
// request fill order from --view target then reconstruct.
void lobby_menu::query_scene_resources( )
{
}

profile_player_character::profile_player_character( lobby_menu& lobby_menu )
	: m_lobby_menu( lobby_menu )
{
}

// claude@NOTE: faithful body but currently /OPT:REF-collapses to `ret` (unpaired) because
// player::tick is still an empty stub (player_tick.cpp) - LTCG inlines the empty callee and
// drops the if(m_player) guard. Pairs once player::tick gets a real body.
void profile_player_character::update( const u32 current_time_in_ms )
{
	if ( m_player )
		m_player->tick( current_time_in_ms );
}

// claude@NOTE: faithful body, structure + byte-size match. Residual is a pure eax/ecx swap in
// the inlined if(m_player) guard: the TARGET's player::remove uses the LTCG this-in-eax
// convention (mov esi,eax) so m_player loads into eax (the call's this), while our base
// player::remove is __thiscall (this in ecx) because anchor_game_player.cpp pins it via an
// address-take (ANCHOR_PIN(&player::remove)). Flipping that to a guarded direct call (the
// anchor-direct-call-this-convention pattern) would fix this but is a DIFFERENT unit's anchor
// and would re-converge every other player method on its own convention - out of scope here.
void profile_player_character::clear_resources( )
{
	if ( m_player )
		m_player->remove( );

	m_player = NULL;
}

// Faithful one-line forward. The empty query_profile_contents reconstruction is stripped by
// LTCG, leaving this body as a return. Reopen with the query_profile_contents caller cone.
void profile_player_character::profile_changed( player_profile const* profile )
{
	query_profile_contents( profile );
}

// STATE[STUB]
// PARKED: body deep-copies the player_profile (doug_lea malloc 0x1B8 +
// player_profile ctor + memcpy) into a player_initial_info, then resources::query_resources
// with an on_player_ready boost::bind callback (the bind_t/mf2/list3 vtable assign_to is
// the heavy part). Walled by the boost::bind callback-object construction sequence.
// NEXT: model the query_resources<player_initial_info> + bind on_player_ready idiom.
// Named locals: player_initial_info info; variant<32> ud.
void profile_player_character::query_profile_contents( player_profile const* profile )
{
}

// STATE[STUB]
// claude@NOTE: PARKED. Callback fired by query_profile_contents' query_resources; cooks the
// player from the queries_result + profile_to_cook. Depends on the player_cook / player ctor
// path and queries_result accessors. NEXT: pair with query_profile_contents (shared bind type).
void profile_player_character::on_player_ready( resources::queries_result& data, player_profile* profile_to_cook )
{
}

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

// claude@NOTE: structure matches (for / if(!is_successful) LOG_ERROR / two array
// assigns). Byte residual is a cross-module layout shift: our resources::queries_result
// header is 8 bytes larger than the target's, so m_size reads [esi+40h] vs target
// [esi+38h] and m_queries[i] is at +0x154 vs +0x14C; that 8-byte offset cascades into
// the loop base-register choice (ebx vs edi) and the get_managed_resource ref-count
// temp count. Fixing it would mean editing core resources_query_result.h - out of scope.
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
// /OPT:REF from stripping the standalone body until that call site is recovered.
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
