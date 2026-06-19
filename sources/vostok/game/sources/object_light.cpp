////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "object_light.h"
#include "game_object_static.h"
#include "base_game_scene.h"
#include "game.h"
#include <vostok/math_float4x4.h>
#include <vostok/configs_binary_config_value.h>
#include <vostok/render/facade/game_renderer.h>
#include <vostok/render/facade/scene_renderer.h>

namespace survarium {

void load_transform( configs::binary_config_value const& t, float4x4& dest );

static u32 light_ids = 1000000;

// claude@NOTE: ctor/dtor/load byte-residual is a render-facade gap, not a source
// shape issue - light_props has no out-of-line ctor/dtor in our header (target
// calls render::light_props::light_props/~light_props out-of-line, ours inlines a
// trivial member-init), and load_props_impl is a noinline stub in scene_renderer.cpp
// (real body ~1664 bytes, unmatched). Recovers once the render-facade light_props
// batch lands.
object_light::object_light( base_game_scene& w ) :
	game_object_static( w )
{
	m_light_id = ++light_ids;
}

object_light::~object_light( )
{
}

void object_light::load(
	configs::binary_config_value const&		t,
	pcstr									__formal,
	boost::function< void( game_object_& ) >&	cb
)
{
	load_transform( t, m_transform );
	render::load_props_impl( m_props, t );
	cb( *this );
}

void object_light::insert( )
{
	get_game_scene().renderer().scene().add_light( get_game_scene().render_scene(), m_light_id, m_props );
}

void object_light::remove( )
{
	get_game_scene().renderer().scene().remove_light( get_game_scene().render_scene(), m_light_id );
}

} // namespace survarium
