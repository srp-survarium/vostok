// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "object_ambient_volume.h"
#include "game_object_static.h"
#include "base_game_scene.h"
#include "game.h"
#include <vostok/math_float4x4.h>
#include <vostok/configs_binary_config_value.h>
#include <vostok/render/facade/game_renderer.h>
#include <vostok/render/facade/scene_renderer.h>
#include <vostok/render/facade/ambient_volume_properties.h>

namespace survarium {

void load_transform( configs::binary_config_value const& t, float4x4& dest );

static u32	ambient_volume_ids	= 0;

// claude@NOTE: clear_value file-static float; init value is not encoded in the
// load asm (referenced only by address) and cannot be recovered from the
// disassembly. Guessed 0.0f - does not affect load's codegen, only the unmatched
// data initializer.
static float clear_value = 0.0f;

 object_ambient_volume::object_ambient_volume( base_game_scene& w ) :
	game_object_static( w )
{
	m_id	= ambient_volume_ids++;
	m_valid	= true;
}

 object_ambient_volume::~object_ambient_volume( )
{
}

void object_ambient_volume::load(
	configs::binary_config_value const&		t,
	pcstr									__formal,
	boost::function< void( game_object_& ) >&	cb
)
{
	load_transform( t, m_transform );

	m_enabled				= t["enabled"];
	m_ambient_multiplier	= t["ambient_multiplier"];

	if ( !m_enabled || m_ambient_multiplier == clear_value )
		m_valid = false;

	cb( *this );
}

void object_ambient_volume::insert( )
{
	if ( !m_valid )
		return;

	render::ambient_volume_properties	properties;

	properties.transform			= m_transform;
	properties.ambient_multiplier	= m_ambient_multiplier;
	properties.enabled				= m_enabled;

	get_game_scene().renderer().scene().update_ambient_volume(
		get_game_scene().render_scene(),
		m_id,
		properties
	);
}

void object_ambient_volume::remove( )
{
	if ( !m_valid )
		return;

	get_game_scene().renderer().scene().remove_ambient_volume(
		get_game_scene().render_scene(),
		m_id
	);
}

} // namespace survarium
