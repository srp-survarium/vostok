// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "object_environment_probe.h"
#include "game_object_static.h"
#include "base_game_scene.h"
#include "game.h"
#include <vostok/math_float4x4.h>
#include <vostok/configs_binary_config_value.h>
#include <vostok/render/facade/game_renderer.h>
#include <vostok/render/facade/scene_renderer.h>
#include <vostok/render/facade/environment_probe_properties.h>

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

void object_environment_probe::insert( )
{
	render::environment_probe_properties properties;
	properties.transform			= m_transform;
	properties.location				= m_transform.c.xyz( );
	properties.texture_name			= m_texture_name;
	properties.texture_invalidated	= true;
	properties.radius				= m_radius;
	properties.diffuse_multiplier	= m_diffuse_multiplier;
	properties.specular_multiplier	= m_specular_multiplier;
	properties.enabled				= m_enabled;
	properties.cubemap_resolution	= m_cubemap_resolution;
	properties.clip_by_normal		= m_clip_by_normal;
	properties.with_shadows			= m_with_shadows;
	properties.geometry				= m_geometry;
	properties.preview_mip			= 0;

	get_game_scene( ).renderer( ).scene( ).update_environment_probe( get_game_scene( ).render_scene( ), m_probe_id, properties );
}

void object_environment_probe::remove( )
{
	get_game_scene( ).renderer( ).scene( ).remove_environment_probe( get_game_scene( ).render_scene( ), m_probe_id );
}

} // namespace survarium
