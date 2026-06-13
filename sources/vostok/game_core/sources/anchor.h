

#include <vostok/game_core/affect_event_type_enum.h>
#include <vostok/game_core/affects_applying_type_enum.h>
#include <vostok/game_core/hit_affects_type_enum.h>

#include <vostok/game_core/affects_threshold.h>
#include <vostok/game_core/body_part_parameters.h>
#include <vostok/game_core/damage_model.h>
#include <vostok/game_core/damage_protector.h>
#include <vostok/game_core/hit_type_parameters.h>

namespace vostok
{
	// /OPT:REF reachability anchor entry points, one per engine module. Each
	// instantiates its module's classes and calls their methods so the linker
	// keeps them in the base EXE for the delinker to score. IncludeAll::IncludeAll()
	// (instantiated in game/sources/game_world.cpp) dispatches to all of them.
	void anchor_game_core( );
	void anchor_network_core( );
	void anchor_physics( );
	void anchor_collision( );
	void anchor_logging( );

	// shared opaque sink: escapes addresses so LTCG observes member stores.
	// Defined once in anchor_network_core.cpp; used by every anchor TU.
	void example_callback( const char* name );
}

namespace survarium
{

class IncludeAll
{
public:
	IncludeAll();
};

}
