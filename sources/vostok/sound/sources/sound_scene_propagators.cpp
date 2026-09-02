// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created		: 29.07.2026
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include "sound_scene.h"
#include "sound_instance_proxy_order.h"
#include "create_sound_propagator_params.h"
#include <vostok/sound/sound_propagator_emitter.h>
#include <vostok/sound/world_user.h>

namespace vostok {
namespace sound {

void sound_scene::emit_sound_propagators	(	sound_instance_proxy_internal& proxy,
												playback_mode mode,
												u32 playback_id,
												sound_producer const* const producer,
												sound_receiver const* const ignorable_receiver	)
{
	LOG_DEBUG							( "sound_scene::emit_sound_propagators" );
	create_sound_propagator_params params
		(
			mode,
			proxy,
			playback_id,
			producer,
			ignorable_receiver
		);

	typedef sound_instance_proxy_order_with_data< create_sound_propagator_params > order_type;

	order_type::functor_type functor	= boost::bind
		(
			&sound_scene::emit_sound_propagators_impl,
			this,
			_1
		);

	order_type* order	=
		VOSTOK_NEW_IMPL	( proxy.get_world_user( ).get_allocator( ), order_type )
		( proxy.get_world_user( ), proxy, functor, params );

	proxy.get_world_user( ).add_order	( order );
}

void sound_scene::emit_sound_propagators	(	sound_instance_proxy_internal& proxy,
												source_params const& params,
												playback_mode mode,
												u32 playback_id,
												sound_producer const* const producer,
												sound_receiver const* const ignorable_receiver
											)
{
	LOG_DEBUG							( "sound_scene::emit_sound_propagators" );
	create_sound_propagator_params prop_params
		(
			params,
			mode,
			proxy,
			playback_id,
			producer,
			ignorable_receiver
		);

	typedef sound_instance_proxy_order_with_data< create_sound_propagator_params > order_type;

	order_type::functor_type functor	= boost::bind
		(
			&sound_scene::emit_sound_propagators_impl,
			this,
			_1
		);

	order_type* order	=
		VOSTOK_NEW_IMPL	( proxy.get_world_user( ).get_allocator( ), order_type )
		( proxy.get_world_user( ), proxy, functor, prop_params );

	proxy.get_world_user( ).add_order	( order );
}

#line 85
void sound_scene::emit_sound_propagators_impl	( create_sound_propagator_params const& params )
{
	LOG_DEBUG							( "sound_scene::emit_sound_propagators_impl" );
	sound_instance_proxy_internal& proxy		= params.m_proxy;
	sound_propagator_emitter const& emitter		= proxy.get_sound_propagator_emitter( );

	u32 old_props_count							= proxy.get_propagators( ).size( );


	emitter.emit_sound_propagators
	(
		proxy,
		params.m_mode,
		params.m_playback_id,
		0,
		0,
		params.m_producer,
		params.m_ignorable_receiver
	);


	new_sound_propagator* prop					= proxy.get_propagators( ).front( );
	for ( u32 i = 0; i < old_props_count; ++i )
	{
		prop										= proxy.get_propagators( ).get_next_of_object( prop );
		prop->set_as_callback_executer			( false );
	}

	new_sound_propagator* last					= prop;
	while ( prop )
	{
		last										= prop;
		prop										= proxy.get_propagators( ).get_next_of_object( prop );
	}

	R_ASSERT									( last );
	last->set_as_callback_executer				( true );

	LOG_INFO									( "sound propagators with id %d created", params.m_proxy.get_id() );

	if ( old_props_count == 0 )
	{
		R_ASSERT								( !m_active_proxies.contains_object( &proxy ) );
		m_active_proxies.push_back				( &proxy );
	}
}

#line 132
new_sound_propagator* sound_scene::create_sound_propagator (
	sound_propagator_emitter const& owner,
	sound_instance_proxy_internal& proxy,
	playback_mode mode,
	u32 playback_id,
	u32 playing_offset, u32 before_playing_offset,
	u32 after_playing_offset,
	sound_producer const* const, sound_receiver const* const
)
{
	LOG_DEBUG							( "sound_scene::create_sound_propagator" );
	if ( m_propagators_allocator->total_size() == m_propagators_allocator->allocated_size() )
	{
		LOG_ERROR						( "can't allocate sound_propagator, memory pool is empty :(" );
		return							0;
	}

	new_sound_propagator* new_propagator	= VOSTOK_NEW_IMPL (
		m_propagators_allocator.c_ptr( ),
		new_sound_propagator
	) (
		proxy.get_sound_type( ) == volumetric ? proxy.get_volumetric_position( m_list_orient_front.get( ) ) : proxy.get_position( ),
		m_list_orient_front.get( ),
		mode,
		playback_id, playing_offset,
		before_playing_offset,
		after_playing_offset,
		proxy,
		owner
	);

	new_propagator->set_as_callback_executer	( true );
	return new_propagator;
}

#line 167
void sound_scene::delete_sound_propagator	( sound_instance_proxy_internal& proxy, new_sound_propagator* propagator )
{
	LOG_DEBUG							( "sound_scene::delete_sound_propagator" );
	if ( propagator == 0 )
	{
		LOG_ERROR						( "can't delete sound_propagator, pointer == 0" );
		return;
	}

	R_ASSERT							( propagator->can_be_deleted( ) );
	proxy.get_propagators().erase		( propagator );
	VOSTOK_DELETE_IMPL					( m_propagators_allocator.c_ptr(), propagator );

	R_ASSERT							( propagator == 0 );
	if ( proxy.get_propagators().empty() )
		m_active_proxies.erase			( &proxy );
}

} // namespace sound
} // namespace vostok
