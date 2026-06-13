#include "pch.h"
#include "anchor.h"

namespace survarium
{

IncludeAll::IncludeAll()
{
	// /OPT:REF reachability dispatcher: every use_* the old monolithic
	// temp_include_all.cpp called is still called here, one module entry point
	// per engine module (anchor_game_core.cpp, anchor_network_core.cpp, ...).
	vostok::anchor_game_core( );
	vostok::anchor_network_core( );
	vostok::anchor_physics( );
	vostok::anchor_collision( );
	vostok::anchor_logging( );
}


}
