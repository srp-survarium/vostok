////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/ladder.h>

#include <vostok/game_core/usable_object.h>
#include <vostok/game_core/usable_object_user_data.h>
#include <vostok/game_core/collision_user.h>

namespace survarium {

// STATE[100%|DONE]
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
}

// STATE[100%|DONE]
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

// STATE[100%|DONE]
void ladder::resolve_links( base_project* p, configs::binary_config_value cfg )
{
	usable_object::resolve_links( p, cfg );
	if ( m_occluder )
		m_occluder->resolve_links( p, cfg );

	// FUNCTION BODY
	// <0x5964e7>|0x007|+0x034:'39'
	// <0x59651b>|0x03b|+0x009:'40'
	// <0x596524>|0x044|+0x044:'41'
	// ******
}

// STATE[99.77%|DONE]: target reserves a larger stack frame (LTCG callee temps), non-steerable
void ladder::add_landing_point( landing_point* const new_point )
{
	m_landing_points.push_back( new_point );
	// STRUCTURE DIFF[target 0x586470 | base 0x45b8e0]: target 1 / base 1 stmts
	// .. same ..
	// ; aligned 1, size-diffs 0, quantity-diffs 0
	// VERDICT: STRUCTURE MATCH (shape ok) - sole diff is target sub esp,38h vs base sub esp,0Ch (push_back inlined-temp frame reservation under LTCG), non-steerable. trail: add_landing_point.md
}

// STATE[99.70%|DONE]: target reserves a larger stack frame (LTCG callee temps), non-steerable
landing_point* ladder::pop_landing_point( )
{
	return m_landing_points.pop_front( );
	// STRUCTURE DIFF[target 0x586450 | base 0x45b8c0]: target 1 / base 1 stmts
	// .. same ..
	// ; aligned 1, size-diffs 0, quantity-diffs 0
	// VERDICT: STRUCTURE MATCH (shape ok) - sole diff is target sub esp,44h vs base sub esp,1Ch (pop_front inlined-temp frame reservation under LTCG), non-steerable. trail: pop_landing_point.md
}

// STATE[100%|DONE]
bool ladder::use_initialize( usable_object_user_data* user )
{
	user->owner->use_ladder( this );
	return true;

	// FUNCTION BODY
	// <0x596427>|0x007|+0x01a:'112'
	// <0x596441>|0x021|+0x002:'113'
	// ******
}

// STATE[100%|DONE]
bool ladder::use_execute( usable_object_user_data* user )
{
	VOSTOK_UNREFERENCED_PARAMETER( user );
	return true;

	// FUNCTION BODY
	// <0>
	// <0x596417>|0x007|+0x002:'119'
	// ******
}

// STATE[100%|DONE]
bool ladder::use_finalize( usable_object_user_data* user )
{
	VOSTOK_UNREFERENCED_PARAMETER( user );
	return true;

	// FUNCTION BODY
	// <0>
	// <0x596407>|0x007|+0x002:'125'
	// ******
}

// STATE[100%|DONE]
void ladder::activate( physics::world* world )
{
	usable_object::insert( world );

	// FUNCTION BODY
	// <0x5964c7>|0x007|+0x012:'130'
	// ******
}

// STATE[100%|DONE]
void ladder::deactivate( )
{
	usable_object::remove( );

	// FUNCTION BODY
	// <0x5964a7>|0x007|+0x00e:'135'
	// ******
}

} // namespace survarium
