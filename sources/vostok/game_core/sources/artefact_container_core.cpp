////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/artefact_container_core.h>

#include <vostok/game_core/collision_user.h>
#include <vostok/game_core/generic_anomaly_core.h>
#include <vostok/game_core/inventory_holder.h>

namespace survarium {

// STATE[100%|DONE]
artefact_container_core::artefact_container_core( ) : m_owner( NULL )
{
	// FUNCTION BODY
	// ******
}

// STATE[99.77%|DONE]
void artefact_container_core::load( configs::binary_config_value const& cfg )
{
	usable_object::load( cfg );
	m_artefact_search_time_ms = math::floor( (float)cfg["artefacts_search_time_sec"] * 1000.0f );

	// FUNCTION BODY
	// <0x73dee0>|0x000|+0x009:'20'	{
	// <0x73dee9>|0x009|+0x00c:'21'
	// <0x73def5>|0x015|+0x02e:'22'
	// <0x73df23>|0x043|      :'23'	}
	// ******
}

// STATE[100%|DONE]
void artefact_container_core::activate( generic_anomaly_core* owner, physics::world* world, scheduler& __formal )
{
	m_owner = owner;
	usable_object::insert( world );

	// FUNCTION BODY
	// <0x73deb0>|0x000|+0x007:'26'	{
	// <0x73deb7>|0x007|+0x009:'27'
	// <0x73dec0>|0x010|+0x00c:'28'
	// <0x73decc>|0x01c|      :'29'	}
	// ******
}

// STATE[100%|DONE]
void artefact_container_core::deactivate( )
{
	usable_object::remove( );
	m_owner = NULL;

	// FUNCTION BODY
	// <0x73de90>|0x000|+0x007:'32'	{
	// <0x73de97>|0x007|+0x008:'33'
	// <0x73de9f>|0x00f|+0x00a:'34'
	// <0x73dea9>|0x019|      :'35'	}
	// ******
}

// STATE[100%|DONE]
bool artefact_container_core::use_initialize( usable_object_user_data* user )
{
	if ( !m_usable_object_users.empty( ) )
		return false;

	ASSERT( UNKNOWN_EXPRESSION_T( user ) );
	m_usable_object_users.push_front( user );
	user->current_object = this;
	user->start_using_time_ms = user->current_time_ms;

	return true;

	// FUNCTION BODY
	// <0x73de30>|0x000|+0x009:'38'	{
	// <0x73de39>|0x009|+0x013:'39'
	// <0x73de4c>|0x01c|+0x004:'40'
	// <0>
	// <0x73de50>|0x020|+0x00c:'42'
	// <0x73de5c>|0x02c|+0x011:'43'
	// <0x73de6d>|0x03d|+0x009:'44'
	// <0x73de76>|0x046|+0x00c:'45'
	// <0x73de82>|0x052|+0x002:'46'
	// <0x73de84>|0x054|      :'47'	}
	// ******
}

// STATE[92.86%|PARTIAL]
bool artefact_container_core::use_execute( usable_object_user_data* user )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_usable_object_users[0] == user ) );
	ASSERT( UNKNOWN_EXPRESSION_T( user ) );															// sushi@NOTE: Stupid assert. Instead we should verify that the state at the point of "execution" is correct. Something like m_owner and m_artefact exist, since they should! Though ifs down there don't make sense any longer.
	u32 left_ms					= user->current_time_ms - user->start_using_time_ms;				// sushi@NOTE: Misnomer! elapsed_ms would be a better name.

	float artsearch_time		= m_artefact_search_time_ms * user->booster_artcont_time_factor;
	u32 artefact_search_time_ms = artsearch_time > 0.0f ? (u32)artsearch_time : 0;					// sushi@NOTE: Just use min. Also how can this ever be negative. And this can result in div by zero, which is not good!

	user->current_progress		= math::floor( left_ms / float(artefact_search_time_ms) * 100.0f ); // sushi@NOTE: This can get higher than 100%!

	if ( left_ms >= artefact_search_time_ms ) // if ( search_finished )
	{
		if ( m_owner )
			m_owner->on_artefact_container_use( this );

		user->start_using_time_ms = user->current_time_ms;
		if ( m_artefact	)	// sushi@NOTE: In target this calls `unspecified_bool_type` conversion. For some reason it doesn't in base.
			transfer_artefact( user->owner->cast_to_inventory_holder( ) );
	}

	return true;

	// FUNCTION BODY
	// <0x73e090>|0x000|+0x009:'50'	{
	// <0x73e099>|0x009|+0x00c:'51'	ASSERT( UNKNOWN_EXPRESSION_T( m_usable_object_users[0] == user ) );
	// <0x73e0a5>|0x015|+0x00c:'52'	ASSERT( UNKNOWN_EXPRESSION_T( user ) );
	// <0x73e0b1>|0x021|+0x00f:'53'	u32 left_ms					= user->current_time_ms - user->start_using_time_ms
	// <0>
	// <0x73e0c0>|0x030|+0x01c:'55'	float artsearch_time		= m_artefact_search_time_ms * user->booster_artcont_time_factor;
	// <0>
	// <0x73e0dc>|0x04c|+0x03e:'57' u32 artefact_search_time_ms = artsearch_time > 0.0f ? artsearch_time : 0.0f;
	// <0>
	// <0x73e11a>|0x08a|+0x03a:'59' user->current_progress		= math::floor( left_ms / artefact_search_time_ms * 100.0f );
	// <0>
	// <0x73e154>|0x0c4|+0x008:'61'	if ( left_ms >= artefact_search_time_ms )
	// <0>
	// <0x73e15c>|0x0cc|+0x009:'63'		if ( m_owner )
	// <0x73e165>|0x0d5|+0x00f:'64'			m_owner->on_artefact_container_use( this );
	// <0>
	// <0x73e174>|0x0e4|+0x00c:'66'		user->start_using_time_ms = user->current_time_ms;
	// <0x73e180>|0x0f0|+0x029:'67'		if ( m_artefact )
	// <0x73e1a9>|0x119|+0x01c:'68'			transfer_artefact( user->owner->cast_to_inventory_holder( ) );
	// <0>
	// <0x73e1c5>|0x135|+0x002:'70'	return true;
	// <0x73e1c7>|0x137|      :'71'	}
	// ******
}

// STATE[100%|DONE]
bool artefact_container_core::use_finalize( usable_object_user_data* user )
{
	ASSERT( UNKNOWN_EXPRESSION_T( user ) );
	ASSERT( UNKNOWN_EXPRESSION_T( m_usable_object_users.contains_object( user ) ) );
	ASSERT( UNKNOWN_EXPRESSION_T( user->current_object == this ) );

	user->current_object = NULL;
	user->current_progress = u32(-1);
	m_usable_object_users.erase( user );
	return true;

	// FUNCTION BODY
	// <0x73ddd0>|0x000|+0x009:'74'	{
	// <0x73ddd9>|0x009|+0x00c:'75'
	// <0x73dde5>|0x015|+0x00c:'76'
	// <0>
	// <0x73ddf1>|0x021|+0x00c:'78'
	// <0x73ddfd>|0x02d|+0x00a:'79'
	// <0x73de07>|0x037|+0x00a:'80'
	// <0x73de11>|0x041|+0x00f:'81'
	// <0x73de20>|0x050|+0x002:'82'
	// <0x73de22>|0x052|      :'83'	}
	// ******
}

// STATE[64.31%|PARTIAL]: Everything inlined differently
void artefact_container_core::artefact_spawned( resources::queries_result& data )
{
	ASSERT( UNKNOWN_EXPRESSION );
	m_artefact = static_cast_resource_ptr<artefact_base_ptr>( data[0].get_unmanaged_resource( ) );
	m_artefact->set_amount( 1 );

	// FUNCTION BODY
	// <0x73df30>|0x000|+0x00a:'86'	{
	// <0x73df3a>|0x00a|+0x00c:'87'
	// <0x73df46>|0x016|+0x06c:'88'
	// <0x73dfb2>|0x082|+0x022:'89'
	// <0x73dfd4>|0x0a4|      :'90'	}
	// ******
}

// STATE[96.11%|DONE]: LTCG for query_resource
void artefact_container_core::spawn_artefact( )
{
	variant<32> ud;
	ud.set( u16(0x39) ); // sushi@TODO: What does that mean

	resources::query_resource(
		"gameplay/items/artefacts/lifebone",
		resources::item_class,
		boost::bind( &artefact_container_core::artefact_spawned, this, _1 ),
		g_allocator,
		&ud,
		NULL
	);

	// FUNCTION BODY
	// <0x73e1e3>|0x013|+0x009:'94'
	// <0x73e1ec>|0x01c|+0x018:'95'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x73e204>|0x034|+0x0eb:'103'
	// ******
}

// STATE[0.00%|PARTIAL]: static_cast_resource_ptr didn't inline in target.
void artefact_container_core::transfer_artefact( inventory_holder* holder )
{
	holder->take_inventory_item( static_cast_resource_ptr< inventory_item_ptr >( m_artefact ) );
	m_artefact = NULL;

	// FUNCTION BODY
	// <0x73e030>|0x000|+0x009:'107'	{
	// <0x73e039>|0x009|+0x035:'108'
	// <0x73e06e>|0x03e|+0x013:'109'
	// <0x73e081>|0x051|      :'110'	}
	// ******
}

} // namespace survarium
