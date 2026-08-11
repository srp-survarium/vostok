

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
	// Module entry points keep reconstructed code reachable under /OPT:REF.
	void anchor_game_core( );
	void anchor_physics( );
	void anchor_collision( );
	void anchor_ai_navigation( );

	void anchor_game( );

	void anchor_scaleform( );

	namespace render {
		void anchor_render_facade( );
	}

	// Escapes addresses so LTCG observes anchored member stores.
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
