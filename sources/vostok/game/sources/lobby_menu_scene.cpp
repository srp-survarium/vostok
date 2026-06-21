////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "lobby_menu.h"
// this compiland also holds the out-of-line bodies of these two (batch 6) types
#include "profile_player_character.h"
#include "profile_character.h"
#include "player.h" // complete type for player_ptr (intrusive_ptr<player>) dtor

namespace survarium {

// STATE[STUB]
// claude@NOTE: PARKED. Body iterates m_lobby_game_project's scenes / static_collisions
// (simple_game_project members not declared in our tree) calling scene_renderer::remove_model
// + static_collision::remove, then releases m_character (profile_player_character) and calls
// a base_game_scene virtual. Walled by simple_game_project's scene/collision container layout.
// NEXT: declare simple_game_project's scene_view + static_collision arrays, then reconstruct.
void lobby_menu::clear_resources( )
{
	// CALL SITE INFO
	// <0x73fb27> -> void < unknown >()
	// <0x73fc28> -> void < unknown >( bool )
	// ******

	// FUNCTION BODY[0x73faf0]: 24
	// <0>
	// <0x73faf3>|0x003|+0x016:'25'
	// <0>
	// <1>
	// <2>
	// <0x73fb09>|0x019|+0x00d:'29'
	// <0>
	// <0x73fb16>|0x026|+0x00a:'31'
	// <0>
	// <0x73fb20>|0x030|+0x002:'33'
	// <0x73fb22>|0x032|+0x00e:'34'
	// <0>
	// <1>
	// <0x73fb30>|0x040|+0x01e:'37'
	// <0x73fb4e>|0x05e|+0x034:'38'
	// <0>
	// <0x73fb82>|0x092|+0x01c:'40'
	// <0x73fb9e>|0x0ae|+0x01a:'41'
	// <0>
	// <1>
	// <0x73fbb8>|0x0c8|+0x025:'44'
	// <0x73fbdd>|0x0ed|+0x042:'45'
	// <0>
	// <0x73fc1f>|0x12f|+0x00e:'47'
	// ******
}

// STATE[STUB]
// claude@NOTE: PARKED. Large body building sound::sound_scene_creation_params +
// render::scene_configuration, a resources::request[9] array and variant<32> data[10],
// then resources::query_resources with an on_render_scenes_ready boost::bind callback.
// Walled by the request-array assembly + scene_configuration/sound_scene_creation_params
// field layout. NEXT: recover the request array fill order from --view target then reconstruct.
void lobby_menu::query_scene_resources( )
{
	// LOCALS
	// variant< 32 > 					sound_scene_data
	// variant< 32 > const*[10] 		data
	// variant< 32 > 					temp_data
	// sound::sound_scene_creation_params sound_configuration
	// resources::request[9] 			requests
	// render::scene_configuration 		render_configuration
	// variant< 32 > 					lobby_scene_data
	// ******

	// FUNCTION BODY[0x73ff20]: 51
	// <0>
	// <0x73ff2a>|0x00a|+0x004:'53'
	// <0>
	// <1>
	// <2>
	// <0x73ff2e>|0x00e|+0x017:'57'
	// <0x73ff45>|0x025|+0x027:'58'
	// <0>
	// <0x73ff6c>|0x04c|+0x00c:'60'
	// <0x73ff78>|0x058|+0x00b:'61'
	// <0>
	// <1>
	// <0x73ff83>|0x063|+0x024:'64'
	// <0x73ffa7>|0x087|+0x003:'65'
	// <0x73ffaa>|0x08a|+0x003:'66'
	// <0x73ffad>|0x08d|+0x00c:'67'
	// <0x73ffb9>|0x099|+0x010:'68'
	// <0>
	// <0x73ffc9>|0x0a9|+0x009:'70'
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
	// <0x73ffd2>|0x0b2|+0x008:'89'
	// <0x73ffda>|0x0ba|+0x003:'90'
	// <0x73ffdd>|0x0bd|+0x003:'91'
	// <0x73ffe0>|0x0c0|+0x003:'92'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x73ffe3>|0x0c3|+0x12c:'101'
	// <0>
	// ******
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

// claude@NOTE: faithful body; pairs at 62% only because player::remove (player.cpp) is still
// an empty stub - LTCG inlines it away so the if(m_player) m_player->remove() half collapses,
// leaving the m_player=NULL release. Reaches 100% once player::remove gets a real body.
void profile_player_character::clear_resources( )
{
	if ( m_player )
		m_player->remove( );

	m_player = NULL;
}

// claude@NOTE: faithful one-line forward; /OPT:REF-collapses to `ret` (unpaired) because the
// callee query_profile_contents is itself a /OPT:REF-stripped stub (parked below). Pairs once
// query_profile_contents gets a real body.
void profile_player_character::profile_changed( player_profile const* profile )
{
	query_profile_contents( profile );
}

// STATE[STUB]
// claude@NOTE: PARKED. Body deep-copies the player_profile (doug_lea malloc 0x1B8 +
// player_profile ctor + memcpy) into a player_initial_info, then resources::query_resources
// with an on_player_ready boost::bind callback (the bind_t/mf2/list3 vtable assign_to is
// the heavy part). Walled by the boost::bind callback-object construction sequence.
// NEXT: model the query_resources<player_initial_info> + bind on_player_ready idiom.
void profile_player_character::query_profile_contents( player_profile const* profile )
{
	// LOCALS
	// player_initial_info 				info
	// variant< 32 > 					ud
	// ******

	// FUNCTION BODY[0x73fdc0]: 19
	// <0x73fdc6>|0x006|+0x028:'134'
	// <0>
	// <0x73fdee>|0x02e|+0x00f:'136'
	// <0>
	// <1>
	// <2>
	// <0x73fdfd>|0x03d|+0x013:'140'
	// <0x73fe10>|0x050|+0x004:'141'
	// <0>
	// <0x73fe14>|0x054|+0x008:'143'
	// <0x73fe1c>|0x05c|+0x017:'144'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x73fe33>|0x073|+0x0b9:'152'
	// ******
}

// STATE[STUB]
// claude@NOTE: PARKED. Callback fired by query_profile_contents' query_resources; cooks the
// player from the queries_result + profile_to_cook. Depends on the player_cook / player ctor
// path and queries_result accessors. NEXT: pair with query_profile_contents (shared bind type).
void profile_player_character::on_player_ready( resources::queries_result& data, player_profile* profile_to_cook )
{
	// FUNCTION BODY[0x73fc30]: 7
	// <0>
	// <0x73fc30>|0x000|+0x024:'158'
	// <0x73fc54>|0x024|+0x038:'159'
	// <0>
	// <0x73fc8c>|0x05c|+0x0b8:'161'
	// <0x73fd44>|0x114|+0x03b:'162'
	// <0x73fd7f>|0x14f|+0x00a:'163'
	// ******
}

// STATE[STUB]
// claude@NOTE: PARKED. Pulls the skeleton_model_instance from the queries_result and stores
// it into m_character_model + m_skeleton, adds it to the scene via m_scene_renderer. Walled
// by the render::facade model/scene_renderer accessors + queries_result result extraction.
// NEXT: recover the queries_result -> skeleton_model_ptr extraction + scene_renderer::add_model.
void profile_character::character_model_ready( resources::queries_result& data )
{
	// FUNCTION BODY[0x73f8f0]: 16
	// <0x73f8f5>|0x005|+0x015:'244'
	// <0>
	// <0x73f90a>|0x01a|+0x019:'246'
	// <0x73f923>|0x033|+0x027:'247'
	// <0>
	// <1>
	// <0x73f94a>|0x05a|+0x011:'250'
	// <0>
	// <0x73f95b>|0x06b|+0x0a4:'252'
	// <0x73f9ff>|0x10f|+0x017:'253'
	// <0x73fa16>|0x126|+0x04e:'254'
	// <0>
	// <1>
	// <2>
	// <0x73fa64>|0x174|+0x039:'258'
	// <0>
	// ******
}

// STATE[STUB]
// claude@NOTE: PARKED. Builds the two preview_weapon entries (model + addon + animation +
// model_locator_item) from the queries_result, resolving an addon bone via the unknown
// 'bool(pcstr, render::model_locator_item&) const' locator query. Walled by the render facade
// model_locator + queries_result result extraction. NEXT: identify the locator-lookup accessor.
void profile_character::weapon_resources_ready( resources::queries_result& data )
{
	// CALL SITE INFO
	// <0x73f8c0> -> bool < unknown >( pcstr, render::model_locator_item& ) const
	// ******

	// FUNCTION BODY[0x73f5d0]: 31
	// <0x73f5db>|0x00b|+0x02c:'264'
	// <0>
	// <1>
	// <2>
	// <0x73f607>|0x037|+0x006:'268'
	// <0>
	// <0x73f60d>|0x03d|+0x010:'270'
	// <0x73f61d>|0x04d|+0x019:'271'
	// <0>
	// <0x73f636>|0x066|+0x00f:'273'
	// <0x73f645>|0x075|+0x019:'274'
	// <0>
	// <1>
	// <0x73f65e>|0x08e|+0x025:'277'
	// <0x73f683>|0x0b3|+0x023:'278'
	// <0x73f6a6>|0x0d6|+0x017:'279'
	// <0x73f6bd>|0x0ed|+0x004:'280'
	// <0>
	// <0x73f6c1>|0x0f1|+0x00a:'282'
	// <0>
	// <0x73f6cb>|0x0fb|+0x0a4:'284'
	// <0x73f76f>|0x19f|+0x087:'285'
	// <0>
	// <1>
	// <2>
	// <0x73f7f6>|0x226|+0x00d:'289'
	// <0>
	// <0x73f803>|0x233|+0x0a6:'291'
	// <0x73f8a9>|0x2d9|+0x039:'292'
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
// claude@NOTE: PARKED. Iterates the queries_result animations into m_character_animation[2]
// behind a logging::has_passed_filters("game") guarded LOG and feeds the animation_player.
// Walled by the LOG idiom + queries_result animation iteration + animation_player accessors.
// NEXT: recover the LOG("game", ...) statement + the result-loop into the animation player.
void profile_character::character_animation_ready( resources::queries_result& data )
{
	// LOCALS
	// u32 								i
	// ******

	// FUNCTION BODY[0x73f370]: 9
	// <0x73f37d>|0x00d|+0x11e:'299'
	// <0x73f49b>|0x12b|-0x0fd:'299'
	// <0>
	// <0x73f39e>|0x02e|+0x010:'301'
	// <0x73f3ae>|0x03e|+0x0f1:'302'
	// <0>
	// <1>
	// <2>
	// <0x73f49f>|0x12f|+0x085:'306'
	// <0x73f524>|0x1b4|+0x089:'307'
	// ******
}

// STATE[STUB]
float4x4 identity_transform_functor( pcvoid __formal )
{
	return vostok::math::float4x4();

	// FUNCTION BODY[0x73f340]: 1
	// <0x73f34b>|0x00b|+0x017:'339'
	// ******
}

} // namespace survarium
