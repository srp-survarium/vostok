////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "booby_trap_set_cook.h"
#include "booby_trap_set.h"

namespace survarium {

void booby_trap_set_cook::query_for_derived_resources(
	resources::query_result_for_cook*		parent,
	booby_trap_set_core*					resource,
	booby_trap_set_cook_data const&			cook_data,
	configs::binary_config_ptr				config
)
{
	if ( !cook_data.is_local_player )
	{
		finish_query( parent, resource );
		return;
	}

	configs::binary_config_value const& data = config->get_root( )["data"];

	resources::request requests[2] = {
		{ (pcstr)data["model_ghost_allowed"],	resources::static_model_instance_class },
		{ (pcstr)data["model_ghost_denied"],	resources::static_model_instance_class },
	};

	resources::query_resources(
		requests,
		boost::bind( &booby_trap_set_cook::on_models_ready, this, _1, static_cast< booby_trap_set* >( resource ) ),
		g_allocator,
		NULL,
		parent,
		assert_on_fail_true
	);
}

// STATE[STUB]
void booby_trap_set_cook::on_models_ready( resources::queries_result& data, booby_trap_set* game_resource )
{
	// FUNCTION BODY[0x7b1f20]: 10
	// <0>
	// <1>
	// <2>
	// <0x7b1f23>|0x003|+0x0b8:'52'
	// <0x7b1fdb>|0x0bb|+0x0ac:'53'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x7b2087>|0x167|+0x012:'58'
	// ******
}


} // namespace survarium
