////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\game\sources\game_world_bullet_manager_engine.h"

namespace survarium {

// STATE[STUB]
bool game_world::attach_tracer( bullet* bullet )
{
	// STATICS
	// static float4x4 					initial_tracer_matrix = <0x4c2b5c8>;
	// static u16 						s_tracer_idx = <0x4c2662c>;
	// ******

	return false;

	// FUNCTION BODY[0x79a390]: 12
	// <0x79a399>|0x009|+0x02c:'21'
	// <0>
	// <0x79a3c5>|0x035|+0x01a:'23'
	// <0>
	// <0x79a3df>|0x04f|+0x00f:'25'
	// <0>
	// <0x79a3ee>|0x05e|+0x007:'27'
	// <0x79a3f5>|0x065|+0x002:'28'
	// <0x79a3f7>|0x067|+0x01d:'29'
	// <0x79a414>|0x084|+0x007:'30'
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
bool game_world::detach_tracer( bullet* bullet )
{
	return false;

	// FUNCTION BODY[0x79a460]: 13
	// <0x79a460>|0x000|+0x009:'36'	{
	// <0>
	// <0x79a469>|0x009|+0x019:'38'
	// <0x79a482>|0x022|+0x004:'39'
	// <0>
	// <0x79a486>|0x026|+0x033:'41'
	// <0>
	// <1>
	// <2>
	// <0x79a4b9>|0x059|-0x016:'45'
	// <0>
	// <0x79a4a3>|0x043|+0x0ce:'47'
	// <0x79a571>|0x111|-0x0c3:'47'
	// <0>
	// <0x79a4ae>|0x04e|+0x002:'49'
	// <0x79a4b0>|0x050|+0x0ca:'50'
	// <0x79a57a>|0x11a|      :'50'	}
	// ******
}

// STATE[STUB]
void game_world::update_tracer(
	const u16			tracer_idx,
	float3 const&		position,
	float3 const&		direction,
	const float			length
)
{
	// LOCALS
	// float4x4 						m
	// ******

	// FUNCTION BODY[0x79a240]: 8
	// <0>
	// <0x79a249>|0x009|+0x013:'55'
	// <0x79a25c>|0x01c|+0x00a:'56'
	// <0>
	// <0x79a266>|0x026|+0x051:'58'
	// <0x79a2b7>|0x077|+0x060:'59'
	// <0x79a317>|0x0d7|+0x037:'60'
	// <0x79a34e>|0x10e|+0x02b:'61'
	// ******
}

// STATE[STUB]
void game_world::play_sound( resources::unmanaged_resource_ptr const& resource, float3 const& position )
{
	// LOCALS
	// sound::sound_emitter_ptr 		sound
	// ******

	// CALL SITE INFO
	// <0x799e31> -> sound::world_user& < unknown >() const
	// ******

	// FUNCTION BODY[0x799df0]: 5
	// <0x799df0>|0x000|+0x016:'66'
	// <0>
	// <0x799e06>|0x016|+0x012:'68'
	// <0x799e18>|0x028|+0x02b:'69'
	// <0x799e43>|0x053|+0x01c:'70'
	// ******
}

// STATE[STUB]
void game_world::play_particle(
	resources::unmanaged_resource_ptr const&	particle,
	float3 const&		position,
	float3 const&		direction,
	float3 const&		normal
)
{
	// LOCALS
	// float4x4 						m
	// ******

	// FUNCTION BODY[0x79a1d0]: 3
	// <0x79a1d9>|0x009|+0x014:'75'
	// <0x79a1ed>|0x01d|+0x00a:'76'
	// <0x79a1f7>|0x027|+0x037:'77'
	// ******
}

// STATE[STUB]
void game_world::add_decal(
	resources::unmanaged_resource_ptr const&	decal,
	const u32			id,
	float				size,
	const float			depth,
	float3 const&		position,
	float3 const&		direction,
	float3 const&		normal,
	const bool			is_front_face
)
{
	// LOCALS
	// float4x4 						transform
	// render::decal_properties 		properties
	// ******

	// FUNCTION BODY[0x799e70]: 17
	// <0x799e7f>|0x00f|+0x014:'89'
	// <0x799e93>|0x023|+0x124:'90'
	// <0x799fb7>|0x147|+0x080:'91'
	// <0>
	// <0x79a037>|0x1c7|+0x076:'93'
	// <0>
	// <0x79a0ad>|0x23d|+0x039:'95'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x79a0e6>|0x276|+0x015:'101'
	// <0>
	// <1>
	// <2>
	// <0x79a0fb>|0x28b|+0x09f:'105'
	// ******
}

// STATE[STUB]
void game_world::remove_decal( u32 id )
{
	// FUNCTION BODY[0x79a430]: 1
	// <0x79a430>|0x000|+0x01e:'110'
	// ******
}

// STATE[STUB]
void game_world::tick_bullet_manager_engine( bool is_game_paused )
{
	// FUNCTION BODY[0x799dc0]: 5
	// <0x799dc0>|0x000|+0x00a:'115'
	// <0>
	// <0x799dca>|0x00a|+0x007:'117'
	// <0x799dd1>|0x011|+0x015:'118'
	// <0>
	// ******
}

	// TYPEDEFS
	// typedef
	// 	long
	// 	counter_type;

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

	// typedef
	// 	vostok::memory::multi_threading_single_size_allocator_policy< vostok::memory::single_size_buffer_allocator< 128, vostok::threading::simple_lock >::node >::free_list_type
	// 	free_list_type;

	// ******

} // namespace survarium
