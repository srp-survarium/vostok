// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "game_object_.h"
#include "game_object_static.h"
#include <vostok/math_float4x4.h>
#include <vostok/configs_binary_config_value.h>

namespace survarium {

void load_transform( configs::binary_config_value const& t, float4x4& dest );

game_object_::game_object_( base_game_scene& s ) :
	m_game_scene( s )
{
}

game_object_static::game_object_static( base_game_scene& s ) :
	game_object_( s )
{
}

void game_object_static::load(
		configs::binary_config_value const&			t,
		pcstr										project_resources_path,
		boost::function< void( game_object_& ) >&	cb
	)
{
	load_transform( t, m_transform );
}

void load_transform( configs::binary_config_value const& t, float4x4& dest )
{
	float3 const&	scale		= t["scale"];
	float3 const&	rotation	= t["rotation"];
	float3 const&	position	= t["position"];
	dest = create_scale( scale ) * create_rotation( rotation ) * create_translation( position );
}

} // namespace survarium
