////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/ladder.h>

#include <vostok/game_core/usable_object.h>
#include <vostok/game_core/usable_object_user_data.h>
#include <vostok/game_core/collision_user.h>

namespace survarium {

// STATE[89.26%|DONE]: LTCG for `resources::managed_resource`.
ladder::ladder( resources::managed_resource_ptr const& main_animation, math::plane const& p ) :
	m_main_animation	( main_animation ),
	m_plane				( p ),
	m_occluder			( NULL )
{
}

// STATE[100%|DONE]
ladder::~ladder( )
{
	VOSTOK_DELETE_IMPL( g_allocator, m_occluder );

	// FUNCTION BODY[0x596640]: 1
	// <0x59666c>|0x02c|+0x01c:'23'
	// ******
}

// STATE[87.95%|DONE]: LTCG for `binary_config_value::value_exists` and `malloc`.
void ladder::load( configs::binary_config_value const& cfg_val )
{
	ASSERT( UNKNOWN_EXPRESSION ); // sushi@NOTE: Checks that this class wasn't loaded before? We can see how other `load` functions in engine are implemented
	usable_object::load( cfg_val["collision_geometries"] );
	if ( cfg_val.value_exists( "occlusion_geometries" ) )
	{
		m_occluder = VOSTOK_NEW_IMPL( g_allocator, ladder::ladder_occluder );
		m_occluder->load( cfg_val["occlusion_geometries"] );
	}

	// FUNCTION BODY[0x596570]: 7
	// <0x596579>|0x009|+0x00c:'28'
	// <0x596585>|0x015|+0x016:'29'
	// <0x59659b>|0x02b|+0x018:'30'
	// <0>
	// <0x5965b3>|0x043|+0x064:'32'
	// <0x596617>|0x0a7|+0x021:'33'
	// <0>
	// ******
}

// STATE[100%|DONE]
void ladder::resolve_links( base_project* p, configs::binary_config_value cfg )
{
	usable_object::resolve_links( p, cfg );
	if ( m_occluder )
		m_occluder->resolve_links( p, cfg );

	// FUNCTION BODY[0x5964e0]: 3
	// <0x5964e7>|0x007|+0x034:'39'
	// <0x59651b>|0x03b|+0x009:'40'
	// <0x596524>|0x044|+0x044:'41'
	// ******
}

// STATE[99.77%|DONE]: Target allocated less space on stack
void ladder::add_landing_point( landing_point* const new_point )
{
	m_landing_points.push_back( new_point );

	// FUNCTION BODY[0x596470]: 1
	// <0x596479>|0x009|+0x014:'102'
	// ******
}

// STATE[99.70%|DONE]: Target allocated less space on stack
landing_point* ladder::pop_landing_point( )
{
	return m_landing_points.pop_front( );

	// FUNCTION BODY[0x596450]: 1
	// <0x596459>|0x009|+0x00e:'107'
	// ******
}

// STATE[100%|DONE]
bool ladder::use_initialize( usable_object_user_data* user )
{
	user->owner->use_ladder( this );
	return true;

	// FUNCTION BODY[0x596420]: 2
	// <0x596427>|0x007|+0x01a:'112'
	// <0x596441>|0x021|+0x002:'113'
	// ******
}

// STATE[100%|DONE]
bool ladder::use_execute( usable_object_user_data* user )
{
	VOSTOK_UNREFERENCED_PARAMETER( user );
	return true;

	// FUNCTION BODY[0x596410]: 2
	// <0>
	// <0x596417>|0x007|+0x002:'119'
	// ******
}

// STATE[100%|DONE]
bool ladder::use_finalize( usable_object_user_data* user )
{
	VOSTOK_UNREFERENCED_PARAMETER( user );
	return true;

	// FUNCTION BODY[0x596400]: 2
	// <0>
	// <0x596407>|0x007|+0x002:'125'
	// ******
}

// STATE[100%|DONE]
void ladder::activate( physics::world* world )
{
	usable_object::insert( world );

	// FUNCTION BODY[0x5964c0]: 1
	// <0x5964c7>|0x007|+0x012:'130'
	// ******
}

// STATE[100%|DONE]
void ladder::deactivate( )
{
	usable_object::remove( );

	// FUNCTION BODY[0x5964a0]: 1
	// <0x5964a7>|0x007|+0x00e:'135'
	// ******
}

} // namespace survarium
