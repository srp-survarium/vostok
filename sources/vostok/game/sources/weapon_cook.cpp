////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "weapon_cook.h"
#include "weapon.h"
#include "game.h"
#include "shotgun_weapon_reload_state_cook.h"
#include "weapon_sound_events_handler_state_cook.h"

#include <vostok/game_core/double_barreled_weapon_core_aimed_idle_state.h>
#include <vostok/game_core/double_barreled_weapon_core_idle_state.h>
#include <vostok/game_core/pistol_weapon_core_aimed_idle_state.h>
#include <vostok/game_core/pistol_weapon_core_idle_state.h>
#include <vostok/game_core/weapon_core_aimed_state.h>
#include <vostok/game_core/weapon_core_idle_state.h>
#include <vostok/game_core/weapon_core_inactive_state_cook.h>
#include <vostok/game_core/weapon_core_state_cook_template.h>

namespace survarium {

weapon_cook::weapon_cook( game& g ) :
	m_game( g )
{
}

void weapon_cook::register_cooks_for_logic_states( )
{
	static weapon_core_inactive_state_cook s_weapon_core_inactive_state_cook;
	static weapon_core_state_cook_template< weapon_core_idle_state > s_weapon_core_idle_state_cook;
	static weapon_core_state_cook_template< weapon_core_aimed_state > s_weapon_core_aimed_state_cook;
	static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< weapon_core_show_state > > s_show_state_cook;
	static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< weapon_core_hide_state > > s_hide_state_cook;
	static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< weapon_core_reload_state > > s_reload_cook;
	static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< weapon_core_chamber_a_round_state > > s_chamber_a_round_cook;
	static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< weapon_core_chamber_a_round_aimed_state > > s_chamber_a_round_aimed_cook;
	static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< weapon_core_fire_state > > s_fire_cook;
	static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< weapon_core_aimed_fire_state > > s_aimed_fire_cook;
	static shotgun_weapon_reload_state_cook s_shotgun_weapon_reload_state_cook;
	static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< weapon_core_shotgun_reload_start_substate > > s_shotgun_reload_start_substate_cook;
	static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< weapon_core_shotgun_reload_one_round_substate > > s_shotgun_reload_one_round_substate_cook;
	static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< weapon_core_shotgun_reload_finish_substate > > s_shotgun_reload_finish_substate_cook;
	static weapon_core_state_cook_template< double_barreled_weapon_core_idle_state > s_double_barreled_weapon_core_idle_state_cook;
	static weapon_core_state_cook_template< double_barreled_weapon_core_aimed_idle_state > s_double_barreled_weapon_core_aimed_idle_state_cook;
	static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< double_barreled_weapon_core_show_state > > s_double_barreled_show_state_cook;
	static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< double_barreled_weapon_core_hide_state > > s_double_barreled_hide_state_cook;
	static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< double_barreled_weapon_core_reload_state > > s_double_barreled_reload_cook;
	static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< double_barreled_weapon_core_fire_state > > s_double_barreled_fire_cook;
	static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< double_barreled_weapon_core_aimed_fire_state > > s_double_barreled_aimed_fire_cook;
	static weapon_core_state_cook_template< pistol_weapon_core_idle_state > s_pistol_weapon_core_idle_state_cook;
	static weapon_core_state_cook_template< pistol_weapon_core_aimed_idle_state > s_pistol_weapon_core_aimed_idle_state_cook;
	static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< pistol_weapon_core_show_state > > s_pistol_show_state_cook;
	static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< pistol_weapon_core_hide_state > > s_pistol_hide_state_cook;
	static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< pistol_weapon_core_reload_state > > s_pistol_reload_cook;
	static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< pistol_weapon_core_fire_state > > s_pistol_fire_cook;
	static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< pistol_weapon_core_aimed_fire_state > > s_pistol_aimed_fire_cook;
}

// STATE[STUB]
// claude@NOTE: parked - large weapon-config parser. PAIRS (private EAE access). DECODED
// 2026-06-24 (target 0x5cde90, 34 stmts, all callees buildable - configs operator[]/
// value_exists, items_dictionary::item_by_id, weapon::weapon(u32,u32,u32), query_resources,
// boost::bind to on_weapon_subresources_ready). Statement->source-line map (PDB line table):
//   L96  parent = data.get_parent_query()
//   L99  config_ptr = static_cast_resource_ptr<binary_config_ptr>( data[0].get_unmanaged_resource() )
//   L100 binary_config_value const& config = config_ptr->get_root()
//   L108 u8 fire_pfx_count   = config["particles"].value_exists("bullet_shells_count")
//                              ? (u8)config["particles"]["bullet_shells_count"] : 0x0A
//   L109 u8 shells_pfx_count = config["particles"].value_exists("shoot_pfx_count")
//                              ? (u8)config["particles"]["shoot_pfx_count"]     : 3
//   L111 u32 requests_count  = fire_pfx_count + shells_pfx_count + 4   (lea edx,[ecx+eax+4])
//   L114/L115 scope detection: if config["addons"].value_exists() &&
//             config["addons"]["rifle_scope_dict_id"].value_exists() then
//             scope = m_game.items_dictionary().item_by_id( config["addons"]["rifle_scope_dict_id"] ).item_cfg_name  (loads [item+8])
//   L117 requests on raw alloca, size (requests_count + (scope?1:0)) * sizeof(request);
//        the request build is INLINED here (no `requests` named local, no separate helper
//        symbol - create_requests_for_animations 0x762950 is the *container* cook's, not this);
//        each slot is a guarded `if(cursor){[cursor]=path;[cursor+4]=id} cursor+=8`, count later
//        computed (cursor-start)/8 at the query_resources call.
//   L118/L119 if(scope) requests[i++].set("gameplay/items/scopes/leupold/...", 0x5A)
//   L121 requests[i++].set( (pcstr)config["object"]["model"], 0x14 )
//   L123 binary_config_value const& first_view_animations = config["user_animations_in_place"]["death_hud"]
//   L124/L125 loop config["user_animations_in_place"]["death_hud"][k] -> requests[i++].set(.,0x3D)  (array stride 0x18, count = [node+0x16])
//   L127/L128/L129 same loop over ["death"]
//   L131/L132/L133 same loop over ["preview"]
//   L135 malloc weapon block sizeof(weapon)+ (death_hud+death+preview)*4 (= *4+0xFF0); placement new
//   L137 weapon( death_hud_count, death_count, preview_count )   (note: scope-counts are crossed in
//        the two pfx-list mallocs at L142/L145 - the FIRST malloc uses [ebp-14h] count and stores to
//        m_shells_pfx_list/0xFAC + m_shells_pfx_count/0xFB1, the SECOND uses [ebp-18h] -> m_fire_pfx_list/
//        0xFA8 + m_fire_pfx_count/0xFB0; verify the exact source spelling before trusting which member
//        gets which count - reproduce the crossing, do NOT "fix" it)
//   L140 m_shells_pfx_list = (unmanaged_resource_ptr*)malloc(count*4+8) zero-inited (array cookie)
//   L142/L143 m_shells_pfx_count = ...; same malloc dance for m_fire_pfx_list (L145)
//   L148/L149 if fire_pfx_count: loop requests[i++].set((pcstr)config["particles"]["bullet_shells"], 0x48)
//   L151/L152 if shells_pfx_count: loop requests[i++].set((pcstr)config["particles"]["shoot"], 0x48)
//   L166 query_resources( requests, count, boost::bind(&on_weapon_subresources_ready, this, _1, config_ptr, weapon), g_allocator, NULL, parent )
// NEXT: write the body laying statements on these exact lines (the recorded line table must
// agree to byte-match); converge with `--view structure-diff` over several rebuilds. The
// inlined request walk is the only non-obvious shape - try a raw `request*` cursor with
// create_request() and the guarded push, NOT a buffer_vector (no named local).
void weapon_cook::on_weapon_config_loaded( resources::queries_result& data )
{
	// LOCALS
	// configs::binary_config_ptr 		config_ptr
	// u32 								requests_count
	// resources::query_result_for_cook* const parent
	// u8 								fire_pfx_count
	// configs::binary_config_value const& first_view_animations
	// configs::binary_config_value const& config
	// u8 								shells_pfx_count
	// ******

	// FUNCTION BODY[0x5cde90]: 71
	// <0x5cde96>|0x006|+0x00c:'96'
	// <0>
	// <1>
	// <0x5cdea2>|0x012|+0x040:'99'
	// <0x5cdee2>|0x052|+0x009:'100'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x5cdeeb>|0x05b|+0x045:'108'
	// <0x5cdf30>|0x0a0|+0x042:'109'
	// <0>
	// <0x5cdf72>|0x0e2|+0x00f:'111'
	// <0>
	// <1>
	// <0x5cdf81>|0x0f1|+0x034:'114'
	// <0x5cdfb5>|0x125|+0x035:'115'
	// <0>
	// <0x5cdfea>|0x15a|+0x01a:'117'
	// <0x5ce004>|0x174|+0x004:'118'
	// <0x5ce008>|0x178|+0x016:'119'
	// <0>
	// <0x5ce01e>|0x18e|+0x068:'121'
	// <0x5ce086>|0x1f6|-0x040:'121'
	// <0>
	// <0x5ce046>|0x1b6|+0x01d:'123'
	// <0x5ce063>|0x1d3|+0x025:'124'
	// <0x5ce088>|0x1f8|+0x01f:'125'
	// <0>
	// <0x5ce0a7>|0x217|+0x01a:'127'
	// <0x5ce0c1>|0x231|+0x020:'128'
	// <0x5ce0e1>|0x251|+0x029:'129'
	// <0>
	// <0x5ce10a>|0x27a|+0x01b:'131'
	// <0x5ce125>|0x295|+0x02b:'132'
	// <0x5ce150>|0x2c0|+0x01f:'133'
	// <0>
	// <0x5ce16f>|0x2df|+0x06d:'135'
	// <0>
	// <0x5ce1dc>|0x34c|+0x013:'137'
	// <0>
	// <1>
	// <0x5ce1ef>|0x35f|+0x076:'140'
	// <0>
	// <0x5ce265>|0x3d5|+0x04c:'142'
	// <0x5ce2b1>|0x421|+0x009:'143'
	// <0>
	// <0x5ce2ba>|0x42a|+0x04f:'145'
	// <0>
	// <1>
	// <0x5ce309>|0x479|+0x05b:'148'
	// <0x5ce364>|0x4d4|-0x034:'148'
	// <0x5ce330>|0x4a0|+0x039:'149'
	// <0>
	// <0x5ce369>|0x4d9|+0x009:'151'
	// <0x5ce372>|0x4e2|+0x03d:'152'
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
	// <0x5ce3af>|0x51f|+0x0ec:'166'
	// ******
}

// STATE[STUB]
// claude@NOTE: parked - large (60-stmt) subresource installer. Now PAIRS (access fixed to
// private AAE). Recovers as: extract scope = rifle_scope from data when addons/
// rifle_scope_dict_id exists (resource_ptr::operator= over the queried item, with the
// intrusive_base destroy dance); then iterate the user_animations_in_place death_hud/death/
// preview config arrays (binary_config_value stride 0x18), storing each queried managed
// animation into the weapon's appended intrusive_ptr array at object+0xFF0 and into the
// pfx/shells slots (object+0xFAC iterated by [object+0xFB1]=m_shells_pfx_count); finally
// load_weapon( base_model, scope ) + weapon_core_cook::process_loading_weapon_core. NEXT:
// reconstruct statement-by-statement against the carcass line records below (lines 171-230).
void weapon_cook::on_weapon_subresources_ready(
	resources::queries_result&		data,
	configs::binary_config_ptr		config_ptr,
	weapon_core* const				object_to_cook
)
{
	// LOCALS
	// u32 								resource_index
	// render::skeleton_model_instance_ptr base_model
	// resources::query_result_for_cook* const parent
	// rifle_scope_ptr 					scope
	// resources::managed_resource_ptr* animations
	// u32 								i
	// ******

	// FUNCTION BODY[0x5cd800]: 60
	// <0x5cd803>|0x003|+0x004:'171'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5cd807>|0x007|+0x061:'177'
	// <0x5cd868>|0x068|+0x086:'178'
	// <0>
	// <0x5cd8ee>|0x0ee|+0x05e:'180'
	// <0>
	// <1>
	// <0x5cd94c>|0x14c|+0x02a:'183'
	// <0x5cd976>|0x176|+0x0a9:'184'
	// <0x5cda1f>|0x21f|-0x06d:'184'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5cd9b2>|0x1b2|+0x074:'191'
	// <0>
	// <0x5cda26>|0x226|+0x020:'193'
	// <0x5cda46>|0x246|+0x0a7:'194'
	// <0x5cdaed>|0x2ed|-0x06d:'194'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5cda80>|0x280|+0x074:'201'
	// <0>
	// <0x5cdaf4>|0x2f4|+0x020:'203'
	// <0x5cdb14>|0x314|+0x0a9:'204'
	// <0x5cdbbd>|0x3bd|-0x06d:'204'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5cdb50>|0x350|+0x074:'211'
	// <0>
	// <1>
	// <2>
	// <0x5cdbc4>|0x3c4|+0x02c:'215'
	// <0>
	// <0x5cdbf0>|0x3f0|+0x0e4:'217'
	// <0>
	// <1>
	// <0x5cdcd4>|0x4d4|+0x030:'220'
	// <0>
	// <0x5cdd04>|0x504|+0x0cf:'222'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5cddd3>|0x5d3|+0x012:'229'
	// <0x5cdde5>|0x5e5|+0x038:'230'
	// ******
}

// claude@NOTE: structure matches (cast, count, ~weapon, the two pfx-array deletes, the
// appended-animations dtor loop). Capped ~77%: the final free is the strip_pointer/
// delete_helper inline-vs-call wall. Target emits TWO statements - __RTCastToVoid
// (dynamic_cast<pvoid> via get_top_pointer, line 249) then mspace_free under an if(cast)
// guard (line 250) - whereas VOSTOK_FREE_IMPL's free_helper passes (remove_cv*)pointer
// (a static cast, no dynamic_cast) and folds to ONE statement. The target free_helper
// variant routes the pointer through get_top_pointer; not reproducible from this header.
void weapon_cook::delete_resource( resources::resource_base* const resource )
{
	weapon* to_delete = static_cast< weapon* >( resource );

	u32 const animations_count	= to_delete->m_first_view_death_animations_count + to_delete->m_third_view_death_animations_count + to_delete->m_preview_animations_count;
	to_delete->~weapon( );

	VOSTOK_DELETE_ARRAY_IMPL( g_allocator, to_delete->m_shells_pfx_list );
	VOSTOK_DELETE_ARRAY_IMPL( g_allocator, to_delete->m_fire_pfx_list );

	resources::managed_resource_ptr* const animations = ( resources::managed_resource_ptr* )( to_delete + 1 );
	for ( u32 i = 0; i != animations_count; ++i )
		animations[ i ].~resource_ptr( );

	VOSTOK_FREE_IMPL( g_allocator, to_delete );
}

u32 weapon_cook::cooked_object_size( weapon_core& object_to_cook ) const
{
	return sizeof( weapon ) + ( static_cast< weapon& >( object_to_cook ).first_view_death_animations_count( ) + static_cast< weapon& >( object_to_cook ).third_view_death_animations_count( ) + static_cast< weapon& >( object_to_cook ).preview_animations_count( ) ) * sizeof( float4x4* );
}

} // namespace survarium
