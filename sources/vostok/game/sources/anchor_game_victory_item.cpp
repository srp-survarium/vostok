#include "pch.h"

// Game-module /OPT:REF reachability anchor for the VICTORY-ITEM cluster:
//   victory_item, victory_item_cook, victory_items_container, artefact_container.
//
// These carcass TUs compile into game.lib but no reachable engine call graph
// touches them yet (game_world registers the core cook, not these runtime
// objects), so /OPT:REF strips them from the EXE and objdiff reports their
// target symbols as "unpaired". use_game_victory_item() address-takes each
// method through a volatile sink so the linker keeps the symbol; the ctors are
// pinned by a self-guarded construction (which also emits the derived vtable so
// the forwarding load/activate/deactivate/use_info overrides survive). Retire
// once the real game call graph reaches these for itself.

#include "victory_item.h"
#include "victory_item_cook.h"
#include "game_world.h"
#include "victory_items_container.h"
#include "artefact_container.h"
#include "base_game_scene.h"

namespace vostok
{
	static pcvoid volatile s_victory_item_sink = 0;

	template < typename T >
	static void keep( T m ) { s_victory_item_sink = *( pcvoid const* )&m; }

	void use_game_victory_item( )
	{
		static volatile bool s_run = false;
		if ( s_run )
		{
			static survarium::game_world* volatile		s_world = 0;
			survarium::victory_item			item( *s_world );
			survarium::victory_item_cook	cook( *s_world );

			// Source the scene through a volatile pointer AND keep the constructed
			// objects observably live (load + address-sink): with a single dead
			// construction site LTCG would constant-propagate the scene reference
			// into the tiny frameless ctor, reading it from [s_scene] instead of the
			// real [esp+4] thiscall param and dropping `ret 4` (anchor-sole-caller-
			// convention.md). Making them escape pins the standard convention so the
			// ctors pair the way create_game_objects' real construction does.
			static survarium::base_game_scene* volatile			s_scene	= 0;
			static configs::binary_config_value const* volatile		s_cfg	= 0;
			survarium::base_game_scene&					scene	= *s_scene;
			configs::binary_config_value const&			cfg		= *s_cfg;

			survarium::victory_items_container	vic_container( scene );
			vic_container.load( cfg );
			s_victory_item_sink = &vic_container;

			survarium::artefact_container		art_container( scene );
			art_container.load( cfg );
			s_victory_item_sink = &art_container;
		}

		// ---- victory_item ----------------------------------------------------
		typedef survarium::victory_item vi;
		keep( &vi::tick );
		keep( &vi::put );
		keep( &vi::use_info );
		keep( &vi::take );
		keep( &vi::unload );

		// ---- victory_item_cook -----------------------------------------------
		typedef survarium::victory_item_cook vic;
		keep( &vic::create_resource );
		keep( &vic::on_config_loaded );
		keep( &vic::on_subresources_loaded );

		// victory_items_container / artefact_container: the self-guarded ctors
		// above emit each derived vtable, which references the forwarding
		// load/activate/deactivate/use_info override bodies; the load + address-sink
		// keeps the instances live so the ctors keep the standard thiscall ABI.
	}
} // namespace vostok
