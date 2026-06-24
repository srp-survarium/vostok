////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "object_environment_probe.h"
#include "game_object_static.h"
#include "base_game_scene.h"
#include "game.h"
#include <vostok/math_float4x4.h>
#include <vostok/configs_binary_config_value.h>

namespace survarium {

void load_transform( configs::binary_config_value const& t, float4x4& dest );

static u32	probe_ids	= 0;

 object_environment_probe::object_environment_probe( base_game_scene& w ) :
	game_object_static( w )
{
	m_probe_id	= probe_ids++;
	m_geometry	= 0;
}

 object_environment_probe::~object_environment_probe( )
{
}

void object_environment_probe::load(
	configs::binary_config_value const&		t,
	pcstr									__formal,
	boost::function< void( game_object_& ) >&	cb
)
{
	load_transform( t, m_transform );

	m_cubemap_resolution	= t["cubemap_resolution"];
	m_radius				= t["radius"];
	m_diffuse_multiplier	= t["diffuse_multiplier"];
	m_specular_multiplier	= t["specular_multiplier"];
	m_enabled				= t["enabled"];
	m_texture_name			= pcstr( t["texture"] );
	m_clip_by_normal		= t["clip_by_normal"];
	m_with_shadows			= t["with_shadows"];

	if ( t.value_exists( "geometry" ) )
		m_geometry = t["geometry"];
	else
		m_geometry = 0;

	cb( *this );
}

// claude@NOTE: insert/remove need render::environment_probe_properties and call
// render::scene_renderer::update_environment_probe / remove_environment_probe,
// none of which are declared in our render-facade headers (they live in the
// vostok/render/facade unit - scene_renderer.cpp). Blocked until that facade cook
// lands in its own PR; left STUB.

// STATE[STUB]
void object_environment_probe::insert( )
{
}

// STATE[STUB]
void object_environment_probe::remove( )
{
}

} // namespace survarium
