#include "pch.h"
#include "anchor.h"

namespace survarium
{

// the zlib inflate pins live in survarium_pc_library_linkage.cpp - they must
// sit in an EXPLICIT link object so zlibN.lib (1.2.3) resolves them before the
// Scaleform prebuilt's bundled 1.2.7 copy does
IncludeAll::IncludeAll()
{
	vostok::anchor_game_core( );
	vostok::anchor_physics( );
	vostok::anchor_collision( );
	vostok::anchor_ai_navigation( );
	vostok::anchor_game( );
	vostok::anchor_scaleform( );
	vostok::render::anchor_render_facade( );
}


}
