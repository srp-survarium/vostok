////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK\GAME\SOURCES\WEAPON_SOUND_EVENTS_HANDLER_STATE_COOK_INLINE_H_INCLUDED
#define VOSTOK\GAME\SOURCES\WEAPON_SOUND_EVENTS_HANDLER_STATE_COOK_INLINE_H_INCLUDED

namespace survarium {

// STATE[STUB]
inline  weapon_sound_events_handler_state_cook<survarium::weapon_sound_events_handler_state<survarium::weapon_core_show_state> >::weapon_sound_events_handler_state_cook<survarium::weapon_sound_events_handler_state<survarium::weapon_core_show_state> >( )
{
	// FUNCTION BODY[0x98a20]: 1
	// <0x98a58>|0x038|+0x026:'17'
	// ******
}

// STATE[STUB]
inline mutable_buffer weapon_sound_events_handler_state_cook<survarium::weapon_sound_events_handler_state<survarium::weapon_core_show_state> >::allocate_resource(
	resources::query_result_for_cook&		in_query,
	const_buffer							raw_file_data,
	const bool								file_exist
)
{
	// FUNCTION BODY[0x98a90]: 2
	// <0>
	// <0x98a97>|0x007|+0x023:'28'
	// ******
}

// STATE[STUB]
inline void weapon_sound_events_handler_state_cook<survarium::weapon_sound_events_handler_state<survarium::weapon_core_show_state> >::deallocate_resource( void* buffer )
{
	// FUNCTION BODY[0x9dfb0]: 1
	// <0x9dfb0>|0x000|+0x01c:'34'
	// ******
}

// STATE[STUB]
inline void weapon_sound_events_handler_state_cook<survarium::weapon_sound_events_handler_state<survarium::weapon_core_show_state> >::create_resource(
	resources::query_result_for_cook&		parent,
	const_buffer							raw_file_data,
	mutable_buffer							in_out_unmanaged_resource_buffer
)
{
	// LOCALS
	// configs::binary_config_value 	cfg
	// weapon_state_creation_params const* const params
	// weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< weapon_core_show_state > >::config_params config_parameters
	// configs::binary_config_value const& sounds_cfg
	// ******

	// TYPEDEFS
	// typedef
	// 	buffer_vector< resources::request >
	// 	requests_buffer_type;

	// ******

	// FUNCTION BODY[0x98ad0]: 90
	// <0x98ad0>|0x000|+0x013:'43'	{
	// <0x98ae3>|0x013|+0x00b:'44'
	// <0x98aee>|0x01e|+0x015:'45'
	// <0x98b03>|0x033|+0x024:'46'
	// <0x98b27>|0x057|+0x01d:'47'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x98b44>|0x074|+0x072:'52'
	// <0>
	// <1>
	// <2>
	// <0x98bb6>|0x0e6|+0x00e:'56'
	// <0>
	// <1>
	// <0x98bc4>|0x0f4|+0x012:'59'
	// <0>
	// <1>
	// <0x98bd6>|0x106|+0x023:'62'
	// <0>
	// <1>
	// <0x98bf9>|0x129|+0x011:'65'
	// <0x98c0a>|0x13a|+0x016:'66'
	// <0>
	// <1>
	// <2>
	// <0x98c20>|0x150|+0x019:'70'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x98c39>|0x169|+0x015:'76'
	// <0x98c4e>|0x17e|+0x00f:'77'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x98c5d>|0x18d|+0x011:'84'
	// <0x98c6e>|0x19e|+0x01e:'85'
	// <0x98c8c>|0x1bc|+0x007:'86'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x98c93>|0x1c3|+0x005:'91'
	// <0>
	// <0x98c98>|0x1c8|+0x01a:'93'
	// <0>
	// <0x98cb2>|0x1e2|+0x00e:'95'
	// <0x98cc0>|0x1f0|+0x01e:'96'
	// <0x98cde>|0x20e|+0x024:'97'
	// <0x98d02>|0x232|-0x01e:'97'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x98ce4>|0x214|+0x005:'102'
	// <0>
	// <0x98ce9>|0x219|+0x01c:'104'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x98d05>|0x235|+0x00f:'110'
	// <0x98d14>|0x244|+0x01a:'111'
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
	// <0x98d2e>|0x25e|-0x1fe:'132'
	// <0x98b30>|0x060|+0x322:'133'
	// <0x98e52>|0x382|-0x31d:'133'
	// <0x98b35>|0x065|+0x328:'134'
	// <0x98e5d>|0x38d|      :'134'	}
	// ******
}

// STATE[STUB]
inline void weapon_sound_events_handler_state_cook<survarium::weapon_sound_events_handler_state<survarium::weapon_core_show_state> >::on_subresources_ready(
	resources::queries_result&		data,
	mutable_buffer					buffer,
	weapon_state_creation_params const* const	params,
	weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< weapon_core_show_state > >::config_params	config_parameters
)
{
	// LOCALS
	// buffer_vector< resources::managed_resource_ptr > animations
	// const u32 						total_sounds_count
	// sound::sound_emitter_ptr* const 	buffer_for_sounds
	// ******

	// TYPEDEFS
	// typedef
	// 	buffer_vector< resources::managed_resource_ptr >
	// 	skeleton_animations_buffer_type;

	// ******

	// FUNCTION BODY[0x9f7f0]: 38
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x9f7f9>|0x009|+0x06e:'148'
	// <0>
	// <0x9f867>|0x077|+0x038:'150'
	// <0>
	// <1>
	// <2>
	// <0x9f89f>|0x0af|-0x095:'154'
	// <0x9f80a>|0x01a|+0x0a4:'155'
	// <0x9f8ae>|0x0be|+0x008:'155'
	// <0>
	// <0x9f8b6>|0x0c6|+0x02a:'157'
	// <0>
	// <0x9f8e0>|0x0f0|+0x076:'159'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x9f956>|0x166|+0x028:'167'
	// <0x9f97e>|0x18e|+0x002:'168'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x9f980>|0x190|+0x023:'177'
	// <0>
	// <1>
	// <0x9f9a3>|0x1b3|+0x06f:'180'
	// <0x9fa12>|0x222|+0x00e:'181'
	// ******
}

// STATE[STUB]
inline void weapon_sound_events_handler_state_cook<survarium::weapon_sound_events_handler_state<survarium::weapon_core_show_state> >::destroy_resource( resources::unmanaged_resource* const resource )
{
	// CALL SITE INFO
	// <0x9d21c> -> void* < unknown >( u32 )
	// ******

	// FUNCTION BODY[0x9d200]: 4
	// <0x9d200>|0x000|+0x00f:'187'
	// <0>
	// <0x9d20f>|0x00f|+0x00f:'189'
	// <0x9d21e>|0x01e|+0x019:'190'
	// ******
}

	// TYPEDEFS
	// typedef
	// 	survarium::base_project::resolve_link_object*
	// 	iterator_type;

	// typedef
	// 	survarium::scheduler::record*
	// 	iterator_type;

	// typedef
	// 	vostok::collision::bone_collision_data const*
	// 	iterator_type;

	// typedef
	// 	vostok::collision::bone_collision_data*
	// 	iterator_type;

	// ******

} // namespace survarium

#endif // #ifndef VOSTOK\GAME\SOURCES\WEAPON_SOUND_EVENTS_HANDLER_STATE_COOK_INLINE_H_INCLUDED
