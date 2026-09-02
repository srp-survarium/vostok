// SPDX-License-Identifier: GPL-3.0-or-later
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
	m_props.load_light_props( t );
	cb( *this );
}

void object_light::insert( )
{
	get_game_scene().renderer().scene().add_light( get_game_scene().render_scene(), m_light_id, &m_props );
}

void object_light::remove( )
{
	get_game_scene().renderer().scene().remove_light( get_game_scene().render_scene(), m_light_id );
}

} // namespace survarium
