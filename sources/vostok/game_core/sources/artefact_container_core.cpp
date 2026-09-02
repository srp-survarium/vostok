// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/game_net_defines.h>
#include <vostok/game_core/artefact_container_core.h>

#include <vostok/game_core/collision_user.h>
#include <vostok/game_core/generic_anomaly_core.h>
#include <vostok/game_core/inventory_holder.h>

namespace survarium {

artefact_container_core::artefact_container_core( ) : m_owner( NULL )
{
}

void artefact_container_core::load( configs::binary_config_value const& cfg )
{
	usable_object::load( cfg );
	m_artefact_search_time_ms = math::floor( (float)cfg["artefacts_search_time_sec"] * 1000.0f );
}

void artefact_container_core::activate( generic_anomaly_core* owner, physics::world* world, scheduler& __formal )
{
	m_owner = owner;
	usable_object::insert( world );
}

void artefact_container_core::deactivate( )
{
	usable_object::remove( );
	m_owner = NULL;
}

bool artefact_container_core::use_initialize( usable_object_user_data* user )
{
	if ( !m_usable_object_users.empty( ) )
		return false;

	ASSERT( UNKNOWN_EXPRESSION_T( user ) );
	m_usable_object_users.push_front( user );
	user->current_object = this;
	user->start_using_time_ms = user->current_time_ms;

	return true;
}

bool artefact_container_core::use_execute( usable_object_user_data* user )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_usable_object_users[0] == user ) );
	ASSERT( UNKNOWN_EXPRESSION_T( user ) );															// sushi@NOTE: Stupid assert. Instead we should verify that the state at the point of "execution" is correct. Something like m_owner and m_artefact exist, since they should! Though ifs down there don't make sense any longer.
	u32 left_ms					= user->current_time_ms - user->start_using_time_ms;				// sushi@NOTE: Misnomer! elapsed_ms would be a better name.

	float const artsearch_time	= m_artefact_search_time_ms * user->booster_artcont_time_factor;

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
}

bool artefact_container_core::use_finalize( usable_object_user_data* user )
{
	ASSERT( UNKNOWN_EXPRESSION_T( user ) );
	ASSERT( UNKNOWN_EXPRESSION_T( m_usable_object_users.contains_object( user ) ) );
	ASSERT( UNKNOWN_EXPRESSION_T( user->current_object == this ) );

	user->current_object = NULL;
	user->current_progress = u32(-1);
	m_usable_object_users.erase( user );
	return true;
}

void artefact_container_core::artefact_spawned( resources::queries_result& data )
{
	ASSERT( UNKNOWN_EXPRESSION );
	m_artefact = static_cast_resource_ptr<artefact_base_ptr>( data[0].get_unmanaged_resource( ) );
	m_artefact->set_amount( 1 );
}

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
}

void artefact_container_core::transfer_artefact( inventory_holder* holder )
{
	holder->take_inventory_item( static_cast_resource_ptr< inventory_item_ptr >( m_artefact ) );
	m_artefact = NULL;
}

} // namespace survarium
