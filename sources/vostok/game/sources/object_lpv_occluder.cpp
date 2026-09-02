// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/game_net_defines.h>
#include "object_lpv_occluder.h"
#include "game_object_static.h"
#include "base_game_scene.h"
#include "game.h"
#include <vostok/math_float4x4.h>
#include <vostok/configs_binary_config_value.h>
#include <vostok/render/facade/game_renderer.h>
#include <vostok/render/facade/scene_renderer.h>

namespace survarium {

void load_transform( configs::binary_config_value const& t, float4x4& dest );

static u32 occluder_ids = 0;

 object_lpv_occluder::object_lpv_occluder( base_game_scene& w ) :
	game_object_static( w )
{
	m_occluder_id = occluder_ids++;
}

 object_lpv_occluder::~object_lpv_occluder( )
{
}

void object_lpv_occluder::load(
	configs::binary_config_value const&		t,
	pcstr									__formal,
	boost::function< void( game_object_& ) >&	cb
)
{
	load_transform( t, m_transform );
	cb( *this );
}

void object_lpv_occluder::insert( )
{
	get_game_scene().renderer().scene().update_lpv_occluder(
		get_game_scene().render_scene(),
		m_occluder_id,
		m_transform
	);
}

void object_lpv_occluder::remove( )
{
	get_game_scene().renderer().scene().remove_lpv_occluder( get_game_scene().render_scene(), m_occluder_id );
}

} // namespace survarium
