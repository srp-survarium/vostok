// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include "object_sound.h"
#include "base_game_scene.h"
#include "game.h"
#include <vostok/configs_binary_config_value.h>
#include <vostok/resources.h>
#include <vostok/resources_queries_result.h>
#include <vostok/resources_query_result.h>
#include <vostok/math_float4x4.h>
#include <vostok/sound/world.h>
#include <vostok/sound/sound_instance_proxy.h>

namespace survarium {

void load_transform( configs::binary_config_value const& t, float4x4& dest );

 object_sound::object_sound( base_game_scene& w ) :
	game_object_static( w )
{
}

object_sound::~object_sound( )
{
}

// claude@NOTE: Source structure and locals match after passing a create_request temporary.
// The residual is the shared binary_config_value::operator int inline cut; reopen after
// that compiler context or function-scoped MAX attribution changes.
void object_sound::load(
	configs::binary_config_value const&		t,
	pcstr									__formal,
	boost::function< void( game_object_& ) >&	cb
)
{
	load_transform		( t, m_transform );
	m_sound_name		= t["sound_name"];
	m_sound_emitter_type	= t["sound_type"];
	resources::class_id_enum resource_id = resources::class_id_enum( s32( t["resource_id"] ) );

	resources::query_resources(
		&resources::create_request( m_sound_name, resource_id ),
		1,
		boost::bind( &object_sound::on_sound_resources_ready, this, _1, cb ),
		g_allocator
	);
}

void object_sound::on_sound_resources_ready(
	resources::queries_result&		data,
	boost::function< void( game_object_& ) >&	callback
)
{
	m_sound_emitter = static_cast_resource_ptr< sound::sound_emitter_ptr >( data[0].get_unmanaged_resource() );

	callback( *this );
}

// claude@NOTE: insert is a STRUCTURE MATCH (7 stmts); the byte-residual is a CSE/
// scheduling difference - the target hoists the get_sound_scene() + get_sound_world()
// .get_logic_world_user() chain (shared by both emit branches) out of the if/else and
// computes it once, our base recomputes it per branch. No named locals on either side,
// so this is a compiler optimization scheduling delta, not source-steerable here.
void object_sound::insert( )
{
	if ( m_sound_emitter_type == 1 )
	{
		m_sound_instance = m_sound_emitter->emit_spot_sound( get_game_scene().get_sound_scene(), get_game_scene().get_game().get_sound_world().get_logic_world_user(), sound::human );
		m_sound_instance->set_position_and_direction( m_transform.c.xyz(), m_transform.k.xyz() );
	}
	else
	{
		m_sound_instance = m_sound_emitter->emit_point_sound( get_game_scene().get_sound_scene(), get_game_scene().get_game().get_sound_world().get_logic_world_user() );
		m_sound_instance->set_position( m_transform.c.xyz() );
	}

	m_sound_instance->play( sound::looped );
}

void object_sound::remove( )
{
	m_sound_instance = NULL;
}

} // namespace survarium
