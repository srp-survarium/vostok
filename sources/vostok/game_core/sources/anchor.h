

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
	// (instantiated in game/sources/game_entry_point.cpp::create_world - the real
	// engine entry point) dispatches to all of them.
	void anchor_game_core( );
	void anchor_network_core( );
	void anchor_network( );
	void anchor_physics( );
	void anchor_collision( );
	void anchor_logging( );
	void anchor_ai_navigation( );

	// game module's own per-module anchor (the engine_user world cone, the game/menu/
	// camera/stats skeleton, and the network-client carcass) - defined in
	// game/sources/anchor_game.cpp, not game_core.
	void anchor_game( );

	// animation module's n_ary_tree_comparer carcass anchor - defined in
	// animation/sources/anchor_animation_comparer.cpp (the comparer visitor has no
	// reachable constructor; mixer.cpp is not compiled in this build).
	void anchor_animation_comparer( );

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


class Callback1
{
public:
	Callback1();

	void complete ( char const * name );
	void partial  ( char const * name, int value );

public:
	typedef boost::function< void ( char const * ) > fn;

	fn m_complete;
	fn m_partial;
};

class Callback2
{
public:
	Callback2();

	void complete ( char const * name, int value );
	void partial  ( char const * name, int value, float precision );

public:
	typedef boost::function< void ( char const *, int ) > fn;

	fn m_complete;
	fn m_partial;
};

}
