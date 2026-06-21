////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "weapon_cook.h"
#include "weapon.h"
#include "game.h"

namespace survarium {

// claude@NOTE: STRUCTURE MATCH (0 stmts, base-ctor call + vtable store + m_game init).
// Capped ~89%: reached only via the carcass anchor's singleton s_weapon_cook, so the
// target loads `this` as the absolute &s_weapon_cook instead of the register the real
// caller (game_world) would pass. Anchor-convention residual, not source-steerable.
weapon_cook::weapon_cook( game& g ) :
	m_game( g )
{
}

// STATE[STUB]
// claude@NOTE: structure recovered (28 function-local `static <CookType> s_*;` in the
// $S6-guard order: inactive, idle, aimed, show, hide, reload, chamber_a_round,
// chamber_a_round_aimed, fire, aimed_fire, shotgun_reload, shotgun_{start,one_round,
// finish}, double_barreled {idle, aimed_idle, show, hide, reload, fire, aimed_fire},
// pistol {idle, aimed_idle, show, hide, reload, fire, aimed_fire}). Each declaration is
// a magic-statics guard + cook ctor + atexit(dtor). BLOCKED: emitting the bodies forces
// instantiation of the cook-template ctors (weapon_core_state_cook_template<T>::ctor in
// weapon_core_state_cook_template_inline.h and weapon_sound_events_handler_state_cook<T>::
// ctor in weapon_sound_events_handler_state_cook_inline.h), which are themselves STUBs:
// their empty bodies implicitly default-construct the base resources::unmanaged_cook,
// which has NO default ctor (C2512). NEXT: recover those two template cook ctors first
// (each needs `unmanaged_cook( <per-T class_id>, reuse_false, use_current_thread_id,
// use_current_thread_id )` + register_cook(this), per weapon_core_inactive_state_cook.cpp),
// then the 28-static body here builds and pairs.
void weapon_cook::register_cooks_for_logic_states( )
{
}

// STATE[STUB]
// claude@NOTE: parked - large (71-stmt) weapon-config parser. Now PAIRS (access fixed to
// private EAE). Recovers as: read config_ptr = data[0] (sound_emitter::set + destroy on the
// intrusive temp); cfg = config_ptr root; fire_pfx_count = particles/bullet_shells_count
// (default 0Ah), shells/shoot_pfx_count = particles/shoot_pfx_count (default 3); count the
// request slots ((fire+shells+4) plus a rifle_scope_dict_id addon via items_dictionary::
// item_by_id when addons/rifle_scope_dict_id exists); build the request array on alloca
// (leupold scope model 0x5A, object/model 0x14, then user_animations_in_place/death_hud,
// /death, /preview each iterated by binary_config_value array stride 0x18, class 0x3D);
// malloc the weapon block (sizeof(weapon)+sum*4) + placement weapon(counts); query_resources
// bound to on_weapon_subresources_ready. NEXT: reconstruct statement-by-statement against
// the carcass line records below (lines 96-166).
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
