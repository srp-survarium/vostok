// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/ladder.h>

#include <vostok/game_core/usable_object.h>
#include <vostok/game_core/usable_object_user_data.h>
#include <vostok/game_core/collision_user.h>

namespace survarium {

ladder::ladder( resources::managed_resource_ptr const& main_animation, math::plane const& p ) :
	m_main_animation	( main_animation ),
	m_plane				( p ),
	m_occluder			( NULL )
{
}

ladder::~ladder( )
{
	VOSTOK_DELETE_IMPL( g_allocator, m_occluder );
}

void ladder::load( configs::binary_config_value const& cfg_val )
{
	ASSERT( UNKNOWN_EXPRESSION_T( !m_occluder ) ); // sushi@NOTE: guard that this object wasn't loaded before
	usable_object::load( cfg_val["collision_geometries"] );
	if ( cfg_val.value_exists( "occlusion_geometries" ) )
	{
		m_occluder = VOSTOK_NEW_IMPL( g_allocator, ladder::ladder_occluder );
		m_occluder->load( cfg_val["occlusion_geometries"] );
	}
}

void ladder::resolve_links( base_project* p, configs::binary_config_value cfg )
{
	usable_object::resolve_links( p, cfg );
	if ( m_occluder )
		m_occluder->resolve_links( p, cfg );
}

void ladder::add_landing_point( landing_point* const new_point )
{
	m_landing_points.push_back( new_point );
}

landing_point* ladder::pop_landing_point( )
{
	return m_landing_points.pop_front( );
}

bool ladder::use_initialize( usable_object_user_data* user )
{
	user->owner->use_ladder( this );
	return true;
}

bool ladder::use_execute( usable_object_user_data* user )
{
	VOSTOK_UNREFERENCED_PARAMETER( user );
	return true;
}

bool ladder::use_finalize( usable_object_user_data* user )
{
	VOSTOK_UNREFERENCED_PARAMETER( user );
	return true;
}

void ladder::activate( physics::world* world )
{
	usable_object::insert( world );
}

void ladder::deactivate( )
{
	usable_object::remove( );
}

} // namespace survarium
