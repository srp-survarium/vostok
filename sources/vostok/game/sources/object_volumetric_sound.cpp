// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "object_volumetric_sound.h"
#include "base_game_scene.h"
#include <vostok/configs_binary_config_value.h>

namespace survarium {

// claude@NOTE: ctor/dtor/load byte-residual is a cross-TU wall - the object_sound
// base ctor/dtor/load are STUBs on this branch (object_sound.cpp unmatched), so the
// inlined base ctor + the base load call + the dtor tail-call line up differently
// than the target. The link_resolver implicit ctor is also emitted out-of-line here
// (target inlines the [+160h] vtable store). Structure is correct (ctor/dtor/insert/
// resolve_links STRUCTURE MATCH); recovers once object_sound.cpp is matched.
object_volumetric_sound::object_volumetric_sound( base_game_scene& w ) :
	object_sound			( w ),
	m_collision_geometry	( 0 ),
	m_radius				( 10.0f )	// target ctor loads a non-zero float constant (movss, not xorps)
{
}

object_volumetric_sound::~object_volumetric_sound( )
{
}

void object_volumetric_sound::load(
	configs::binary_config_value const&		t,
	pcstr									project_resources_path,
	boost::function< void( game_object_& ) >&	cb
)
{
	object_sound::load( t, project_resources_path, cb );

	m_radius = t["radius"];
}

void object_volumetric_sound::insert( )
{
}

void object_volumetric_sound::resolve_links( base_project* p, configs::binary_config_value config )
{
}

} // namespace survarium
