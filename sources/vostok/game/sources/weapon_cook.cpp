////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "weapon_cook.h"
#include "weapon.h"
#include "game.h"

namespace survarium {

// STATE[STUB]
weapon_cook::weapon_cook( game& g ) :
	// ref member; the same-named param is the obvious source - a matcher confirms
	m_game( g )
{
	// FUNCTION BODY[0x5cd730]: 0
	// <0x5cd730>|0x000|+0x01d:'55'	{
	// <0x5cd74d>|0x01d|      :'56'	}
	// ******
}

// STATE[STUB]
void weapon_cook::register_cooks_for_logic_states( )
{
	// STATICS
	// static weapon_core_state_cook_template< weapon_core_idle_state > s_weapon_core_idle_state_cook = <0x4c275b0>;
	// static weapon_core_state_cook_template< pistol_weapon_core_idle_state > s_pistol_weapon_core_idle_state_cook = <0x4c273d0>;
	// static weapon_core_state_cook_template< double_barreled_weapon_core_idle_state > s_double_barreled_weapon_core_idle_state_cook = <0x4c27470>;
	// static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< pistol_weapon_core_show_state > > s_pistol_show_state_cook = <0x4c27630>;
	// static shotgun_weapon_reload_state_cook s_shotgun_weapon_reload_state_cook = <0x4c27284>;
	// static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< double_barreled_weapon_core_fire_state > > s_double_barreled_fire_cook = <0x4c27410>;
	// static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< pistol_weapon_core_hide_state > > s_pistol_hide_state_cook = <0x4c27610>;
	// static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< double_barreled_weapon_core_reload_state > > s_double_barreled_reload_cook = <0x4c27430>;
	// static weapon_core_state_cook_template< weapon_core_aimed_state > s_weapon_core_aimed_state_cook = <0x4c27590>;
	// static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< weapon_core_hide_state > > s_hide_state_cook = <0x4c27650>;
	// static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< weapon_core_fire_state > > s_fire_cook = <0x4c27510>;
	// static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< weapon_core_aimed_fire_state > > s_aimed_fire_cook = <0x4c274f0>;
	// static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< pistol_weapon_core_fire_state > > s_pistol_fire_cook = <0x4c27370>;
	// static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< weapon_core_show_state > > s_show_state_cook = <0x4c27670>;
	// static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< pistol_weapon_core_reload_state > > s_pistol_reload_cook = <0x4c27390>;
	// static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< weapon_core_chamber_a_round_aimed_state > > s_chamber_a_round_aimed_cook = <0x4c27530>;
	// static weapon_core_state_cook_template< double_barreled_weapon_core_aimed_idle_state > s_double_barreled_weapon_core_aimed_idle_state_cook = <0x4c27450>;
	// static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< weapon_core_reload_state > > s_reload_cook = <0x4c27570>;
	// static weapon_core_inactive_state_cook s_weapon_core_inactive_state_cook = <0x4c27690>;
	// static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< double_barreled_weapon_core_aimed_fire_state > > s_double_barreled_aimed_fire_cook = <0x4c273f0>;
	// static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< weapon_core_chamber_a_round_state > > s_chamber_a_round_cook = <0x4c27550>;
	// static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< weapon_core_shotgun_reload_one_round_substate > > s_shotgun_reload_one_round_substate_cook = <0x4c274b0>;
	// static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< weapon_core_shotgun_reload_finish_substate > > s_shotgun_reload_finish_substate_cook = <0x4c27490>;
	// static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< double_barreled_weapon_core_hide_state > > s_double_barreled_hide_state_cook = <0x4c275d0>;
	// static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< weapon_core_shotgun_reload_start_substate > > s_shotgun_reload_start_substate_cook = <0x4c274d0>;
	// static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< double_barreled_weapon_core_show_state > > s_double_barreled_show_state_cook = <0x4c275f0>;
	// static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< pistol_weapon_core_aimed_fire_state > > s_pistol_aimed_fire_cook = <0x4c27350>;
	// static weapon_core_state_cook_template< pistol_weapon_core_aimed_idle_state > s_pistol_weapon_core_aimed_idle_state_cook = <0x4c273b0>;
	// ******

	// FUNCTION BODY[0x5cd320]: 32
	// <0x5cd320>|0x000|+0x02a:'60'
	// <0>
	// <0x5cd34a>|0x02a|+0x025:'62'
	// <0x5cd36f>|0x04f|+0x025:'63'
	// <0x5cd394>|0x074|+0x025:'64'
	// <0x5cd3b9>|0x099|+0x025:'65'
	// <0x5cd3de>|0x0be|+0x025:'66'
	// <0x5cd403>|0x0e3|+0x025:'67'
	// <0x5cd428>|0x108|+0x025:'68'
	// <0x5cd44d>|0x12d|+0x025:'69'
	// <0x5cd472>|0x152|+0x025:'70'
	// <0>
	// <0x5cd497>|0x177|+0x025:'72'
	// <0x5cd4bc>|0x19c|+0x025:'73'
	// <0x5cd4e1>|0x1c1|+0x025:'74'
	// <0x5cd506>|0x1e6|+0x025:'75'
	// <0>
	// <0x5cd52b>|0x20b|+0x025:'77'
	// <0x5cd550>|0x230|+0x025:'78'
	// <0x5cd575>|0x255|+0x025:'79'
	// <0x5cd59a>|0x27a|+0x025:'80'
	// <0x5cd5bf>|0x29f|+0x025:'81'
	// <0x5cd5e4>|0x2c4|+0x025:'82'
	// <0x5cd609>|0x2e9|+0x025:'83'
	// <0>
	// <0x5cd62e>|0x30e|+0x025:'85'
	// <0x5cd653>|0x333|+0x025:'86'
	// <0x5cd678>|0x358|+0x025:'87'
	// <0x5cd69d>|0x37d|+0x025:'88'
	// <0x5cd6c2>|0x3a2|+0x025:'89'
	// <0x5cd6e7>|0x3c7|+0x025:'90'
	// <0x5cd70c>|0x3ec|+0x023:'91'
	// ******
}

// STATE[STUB]
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

// STATE[STUB]
void weapon_cook::delete_resource( resources::resource_base* const resource )
{
	// carcass signature is const-ptr; delete_helper takes a non-const ref - matcher reconciles
	resources::resource_base* to_delete	= resource;
	VOSTOK_DELETE_IMPL					( g_allocator, to_delete );

	// CALL SITE INFO
	// <0x5cd781> -> void* < unknown >( u32 )
	// ******

	// FUNCTION BODY[0x5cd760]: 16
	// <0>
	// <1>
	// <2>
	// <0x5cd762>|0x002|+0x004:'238'
	// <0>
	// <0x5cd766>|0x006|+0x01d:'240'
	// <0>
	// <0x5cd783>|0x023|+0x019:'242'
	// <0x5cd79c>|0x03c|+0x01d:'243'
	// <0>
	// <0x5cd7b9>|0x059|+0x006:'245'
	// <0x5cd7bf>|0x05f|+0x004:'246'
	// <0x5cd7c3>|0x063|+0x00f:'247'
	// <0>
	// <0x5cd7d2>|0x072|+0x009:'249'
	// <0x5cd7db>|0x07b|+0x019:'250'
	// ******
}

u32 weapon_cook::cooked_object_size( weapon_core& object_to_cook ) const
{
	return sizeof( weapon ) + ( static_cast< weapon& >( object_to_cook ).first_view_death_animations_count( ) + static_cast< weapon& >( object_to_cook ).third_view_death_animations_count( ) + static_cast< weapon& >( object_to_cook ).preview_animations_count( ) ) * sizeof( float4x4* );
}

} // namespace survarium
