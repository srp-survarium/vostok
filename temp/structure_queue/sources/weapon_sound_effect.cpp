////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\game\sources\weapon_sound_effect.h"

namespace survarium {

// STATE[STUB]
 weapon_sound_effect::weapon_sound_effect(
	weapon&								weapon,
	sound::sound_emitter_ptr* const		first_view_sounds_emitters,
	const u8							first_view_sounds_emitters_count,
	sound::sound_instance_proxy_ptr* const	first_view_sounds_instances,
	const u8							max_first_view_sounds_instances_count,
	sound::sound_emitter_ptr* const		third_view_sounds_emitters,
	const u8							third_view_sounds_emitters_count,
	sound::sound_instance_proxy_ptr* const	third_view_sounds_instances,
	const u8							max_third_view_sounds_instances_count,
	bool								stop_sounds_on_state_finalize,
	u8									simultaneous_sounds_queue_size
)
{
	// FUNCTION BODY[0x5bd660]: 2
	// <0x5bd660>|0x000|+0x052:'56'	{
	// <0>
	// <1>
	// <0x5bd6b2>|0x052|      :'59'	}
	// ******
}

// STATE[STUB]
void weapon_sound_effect::initialize( )
{
	// FUNCTION BODY[0x5bd2f0]: 2
	// <0x5bd2f0>|0x000|+0x004:'63'
	// <0>
	// ******
}

// STATE[STUB]
void weapon_sound_effect::finalize( )
{
	// FUNCTION BODY[0x5bd630]: 7
	// <0>
	// <1>
	// <0x5bd630>|0x000|+0x006:'71'
	// <0>
	// <0x5bd636>|0x006|+0x011:'73'
	// <0x5bd647>|0x017|+0x011:'74'
	// <0>
	// ******
}

// STATE[STUB]
void on_sound_finished(
	buffer_vector< sound::sound_instance_proxy_ptr >&	instances,
	sound::sound_instance_proxy const&		instance
)
{
	// LOCALS
	// sound::sound_instance_proxy_ptr* const found
	// ******

	// FUNCTION BODY[0x5bd300]: 6
	// <0>
	// <0x5bd300>|0x000|+0x01d:'81'
	// <0>
	// <1>
	// <0x5bd31d>|0x01d|+0x01e:'84'
	// <0>
	// ******
}

// STATE[STUB]
animation::callback_return_type_enum weapon_sound_effect::on_sound_event( animation::animation_callback_params& params )
{
	// LOCALS
	// sound::sound_instance_proxy_ptr 	instance
	// ******

	// CALL SITE INFO
	// <0x5bd394> -> sound::world_user& < unknown >() const
	// <0x5bd4e8> -> void < unknown >( sound::playback_mode, sound::sound_producer const* const, sound::sound_receiver const* const )
	// <0x5bd546> -> void < unknown >( float3 const& )
	// <0x5bd605> -> void < unknown >( sound::playback_mode, sound::sound_producer const* const, sound::sound_receiver const* const )
	// ******

	// FUNCTION BODY[0x5bd340]: 62
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5bd346>|0x006|+0x012:'95'
	// <0x5bd358>|0x018|+0x017:'96'
	// <0>
	// <0x5bd36f>|0x02f|+0x002:'98'
	// <0>
	// <0x5bd371>|0x031|+0x003:'100'
	// <0>
	// <1>
	// <0x5bd374>|0x034|+0x00f:'103'
	// <0x5bd383>|0x043|+0x013:'104'
	// <0x5bd396>|0x056|+0x039:'105'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5bd3cf>|0x08f|+0x050:'110'
	// <0x5bd41f>|0x0df|+0x006:'111'
	// <0x5bd425>|0x0e5|+0x006:'112'
	// <0>
	// <0x5bd42b>|0x0eb|+0x064:'114'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5bd48f>|0x14f|+0x016:'120'
	// <0x5bd4a5>|0x165|+0x01b:'121'
	// <0>
	// <0x5bd4c0>|0x180|+0x01b:'123'
	// <0x5bd4db>|0x19b|+0x00f:'124'
	// <0>
	// <1>
	// <2>
	// <0x5bd4ea>|0x1aa|+0x005:'128'
	// <0>
	// <0x5bd4ef>|0x1af|+0x03b:'130'
	// <0x5bd52a>|0x1ea|+0x006:'131'
	// <0x5bd530>|0x1f0|+0x006:'132'
	// <0>
	// <0x5bd536>|0x1f6|+0x012:'134'
	// <0x5bd548>|0x208|+0x064:'135'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5bd5ac>|0x26c|+0x016:'141'
	// <0x5bd5c2>|0x282|+0x01b:'142'
	// <0>
	// <0x5bd5dd>|0x29d|+0x01b:'144'
	// <0x5bd5f8>|0x2b8|+0x00f:'145'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5bd607>|0x2c7|+0x01c:'150'
	// <0>
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
