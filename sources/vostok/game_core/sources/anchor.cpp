#include "pch.h"
#include "anchor.h"

namespace survarium
{

IncludeAll::IncludeAll()
{
	vostok::anchor_game_core( );
	vostok::anchor_network_core( );
	vostok::anchor_network( );
	vostok::anchor_physics( );
	vostok::anchor_collision( );
	vostok::anchor_logging( );
	vostok::anchor_ai_navigation( );
	vostok::anchor_game( );
	vostok::anchor_scaleform( );
	vostok::render::anchor_render_facade( );
}


}
