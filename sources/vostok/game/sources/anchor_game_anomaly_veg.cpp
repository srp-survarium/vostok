#include "pch.h"

// Game-module /OPT:REF reachability anchor for the world-object carcasses:
//   generic_anomaly, object_vegetation.
//
// These carcass TUs compile into game.lib but no reachable engine call graph
// touches them yet (the project cooker creates them, still a stub), so /OPT:REF
// strips their symbols and objdiff reports them "unpaired". use_game_anomaly_veg()
// constructs each (keeping ctor/dtor/vtable) and address-takes the virtual
// overrides through a volatile sink so the linker keeps them past /OPT:REF.
// Self-guards like the other carcass anchors: the heavy ctors never run (the
// scene ref is a placeholder), the compiler still emits every reference.
// Retire once the real game call graph (project cooker) reaches these.

#include "generic_anomaly.h"
#include "object_vegetation.h"
#include "base_game_scene.h"
#include <vostok/game_core/scheduler.h>
#include <vostok/physics/api.h>
#include <vostok/configs_binary_config_value.h>

namespace survarium {

	static pcvoid volatile s_anomaly_veg_sink = 0;

	template < typename T >
	static void keep( T m ) { s_anomaly_veg_sink = *( pcvoid const* )&m; }

	void use_game_anomaly_veg( )
	{
		static volatile bool s_run = false;
		if ( s_run )
		{
			// deref the volatile scene ptr ONCE into a reference (mirrors
			// use_object_skeleton): passing the same opaque ref to both ctors stops
			// LTCG proving a constant null and specializing the ctor's convention.
			static survarium::base_game_scene* volatile	s_scene	= 0;
			survarium::base_game_scene&					scene	= *s_scene;

			survarium::generic_anomaly	anomaly( scene );
			survarium::object_vegetation	veg( scene );
		}

		// ---- generic_anomaly: virtual overrides forwarding to the core base ----
		// (load shadows the non-virtual core::load, so disambiguate the address-take)
		typedef survarium::generic_anomaly ga;
		keep( static_cast< void ( ga::* )( configs::binary_config_value const& ) >( &ga::load ) );
		keep( &ga::activate );
		keep( &ga::deactivate );
		keep( &ga::tick );

		// ---- object_vegetation -----------------------------------------------
		// load / insert / remove are render-facade-walled stubs (see the TU note);
		// constructing veg above keeps their vtable slots + ctor/dtor past /OPT:REF.
		// on_grass_loaded is non-virtual (no vtable slot) and has no reachable caller
		// while load is a stub, so address-take it to keep it for pairing.
		typedef survarium::object_vegetation ov;
		keep( &ov::on_grass_loaded );
	}

} // namespace survarium
