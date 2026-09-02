// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "victory_item.h"
#include "game_world.h"
#include <vostok/game_core/inventory_holder.h>
#include <vostok/game_core/collision_user.h>
#include <vostok/render/facade/scene_renderer.h>

namespace survarium {

victory_item::victory_item( game_world& w ) :
	m_game_world( w )
{
	m_transform = create_scale( float3( 1.f, 1.f, 1.f ) ) * create_rotation( float3( 0.f, 0.f, 0.f ) ) * create_translation( float3( 0.f, 0.f, 0.f ) );
}

void victory_item::unload( )
{
	victory_item_core::unload( );

	m_scheduler->unregister( &m_scheduler_identifier );
	m_scheduler = NULL;

	if ( m_model )
		m_game_world.scene_renderer( ).remove_model( m_game_world.render_scene( ), m_model->m_render_model );
}

void victory_item::tick( const u32, const u32 )
{
}

void victory_item::put( physics::world* world, float4x4 const& transform, scheduler& scheduler )
{
	victory_item_core::put( world, transform );

	m_scheduler = &scheduler;
	m_scheduler->register_on_frame(
		&m_scheduler_identifier,
		boost::bind( &victory_item::tick, this, _1, _2 ),
		true
	);

	if ( m_model.c_ptr( ) )
		m_game_world.scene_renderer( ).add_model( m_game_world.render_scene( ), m_model->m_render_model, m_transform );
}

void victory_item::take( )
{
	victory_item_core::take( );

	m_scheduler->unregister( &m_scheduler_identifier );
	m_scheduler = NULL;

	if ( m_model )
		m_game_world.scene_renderer( ).remove_model( m_game_world.render_scene( ), m_model->m_render_model );
}

pcstr victory_item::use_info( usable_object_user_data* user )
{
	if ( inventory_holder* holder = user->owner->cast_to_inventory_holder( ) )
	{
		if ( holder->inventory( ).get_victory_item( ) == NULL )
			return "st_pickup_item";
		else
			return "st_cannot_pickup_item";
	}

	return "";
}


} // namespace survarium
