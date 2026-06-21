////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "booby_trap_cook.h"
#include "booby_trap.h"

namespace survarium {

booby_trap_cook::booby_trap_cook( game_world& gw ) :
	m_game_world( gw )
{
}

void booby_trap_cook::query_for_derived_resources(
	resources::query_result_for_cook*		parent,
	booby_trap_core*						resource,
	configs::binary_config_ptr				config
)
{
	configs::binary_config_value const& data = config->get_root( )["data"];

	resources::request requests[5] = {
		{ (pcstr)data["model_armed"],		resources::static_model_instance_class },
		{ (pcstr)data["model_fired"],		resources::static_model_instance_class },
		{ (pcstr)data["model_disarmed"],	resources::static_model_instance_class },

		{ (pcstr)data["particle_fired"],	resources::particle_system_instance_class },

		{ (pcstr)data["sound_fired"],		resources::single_sound_class },
	};

	resources::query_resources(
		requests,
		boost::bind( &booby_trap_cook::on_models_ready, this, _1, static_cast< booby_trap* >( resource ) ),
		g_allocator,
		NULL,
		parent,
		assert_on_fail_true
	);
}

// STATE[STUB]
void booby_trap_cook::on_models_ready( resources::queries_result& data, booby_trap* game_resource )
{
	// FUNCTION BODY[0x7672a0]: 21
	// <0>
	// <1>
	// <2>
	// <0x7672a4>|0x004|+0x004:'57'
	// <0x7672a8>|0x008|+0x030:'58'
	// <0x7672d8>|0x038|+0x0a8:'59'
	// <0x767380>|0x0e0|+0x0ac:'60'
	// <0x76742c>|0x18c|+0x0ae:'61'
	// <0>
	// <0x7674da>|0x23a|+0x0a5:'63'
	// <0>
	// <0x76757f>|0x2df|+0x0a3:'65'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x767622>|0x382|+0x012:'74'
	// ******
}


} // namespace survarium
