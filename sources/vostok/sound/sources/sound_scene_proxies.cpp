////////////////////////////////////////////////////////////////////////////
//	Created		: 02.08.2011
//	Author		: Dmitry Kulikov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "sound_scene.h"

namespace vostok {
namespace sound {

sound_instance_proxy_ptr sound_scene::new_point_sound_instance_proxy	(
													sound_emitter_ptr const& emitter,
													sound_propagator_emitter const& propagator_emitter,
													world_user& user
												)
{
	if ( m_proxies_allocator->total_size( ) == m_proxies_allocator->allocated_size( ) )
	{
		LOG_ERROR						( "can't allocate sound_instance_proxy, memory is full (%d proxies are allocated)", m_proxies_allocator->allocated_size( ) / sizeof( sound_instance_proxy_internal ) );
		return sound_instance_proxy_ptr	( 0 );
	}

	sound_instance_proxy* new_proxy		= VOSTOK_NEW_IMPL( m_proxies_allocator.c_ptr(), sound_instance_proxy_internal )( *this, emitter, propagator_emitter, user );
	LOG_INFO							( "new sound_instance_proxy allocated, id %d", new_proxy->get_id( ) );
	return sound_instance_proxy_ptr		( new_proxy );
}
sound_instance_proxy_ptr sound_scene::new_hud_sound_instance_proxy	(
												sound_emitter_ptr emitter,
													sound_propagator_emitter const& propagator_emitter,
													world_user& user
												)
{
	if ( m_proxies_allocator->total_size( ) == m_proxies_allocator->allocated_size( ) )
	{
		LOG_ERROR						( "can't allocate sound_instance_proxy, memory is full (%d proxies are allocated)", m_proxies_allocator->allocated_size( ) / sizeof( sound_instance_proxy_internal ) );
		return sound_instance_proxy_ptr	( 0 );
	}

	sound_instance_proxy* new_proxy		= VOSTOK_NEW_IMPL( m_proxies_allocator.c_ptr(), sound_instance_proxy_internal )( *this, emitter, propagator_emitter, user, hud );
	LOG_INFO							( "new sound_instance_proxy allocated, id %d", new_proxy->get_id( ) );
	return sound_instance_proxy_ptr		( new_proxy );
}
sound_instance_proxy_ptr sound_scene::new_spot_sound_instance_proxy	(
												sound_emitter_ptr const& emitter,
												sound_propagator_emitter const& propagator_emitter,
												world_user& user,
												const sound_cone_type cone_type
											)
{
	if ( m_proxies_allocator->total_size( ) == m_proxies_allocator->allocated_size( ) )
	{
		LOG_ERROR						( "can't allocate sound_instance_proxy, memory is full (%d proxies are allocated)", m_proxies_allocator->allocated_size( ) / sizeof( sound_instance_proxy_internal ) );
		return sound_instance_proxy_ptr	( 0 );
	}

	sound_instance_proxy* new_proxy		= VOSTOK_NEW_IMPL( m_proxies_allocator.c_ptr(), sound_instance_proxy_internal )( *this, emitter, propagator_emitter, user, cone_type );
	LOG_INFO							( "new sound_instance_proxy allocated, id %d", new_proxy->get_id( ) );
	return sound_instance_proxy_ptr		( new_proxy );
}


sound_instance_proxy_ptr sound_scene::new_volumetric_sound_instance_proxy	(
												sound_emitter_ptr const& emitter,
												sound_propagator_emitter const& propagator_emitter,
												world_user& user,
												collision::geometry_instance& geometry,
												float radius
											)
{
	if ( m_proxies_allocator->total_size( ) == m_proxies_allocator->allocated_size( ) )
	{
		LOG_ERROR						( "can't allocate sound_instance_proxy, memory is full (%d proxies are allocated)", m_proxies_allocator->allocated_size( ) / sizeof( sound_instance_proxy_internal ) );
		return sound_instance_proxy_ptr	( 0 );
	}

	sound_instance_proxy* new_proxy		= VOSTOK_NEW_IMPL( m_proxies_allocator.c_ptr(), sound_instance_proxy_internal )( *this, emitter, propagator_emitter, user, geometry, radius );
	LOG_INFO							( "new sound_instance_proxy allocated, id %d", new_proxy->get_id( ) );
	return sound_instance_proxy_ptr		( new_proxy );
}

void sound_scene::free_sound_instance_proxy	( sound_instance_proxy* proxy )
{
	LOG_INFO							( "sound instance proxy with id %d is deallocated", proxy->get_id( ) );
	VOSTOK_DELETE_IMPL					( m_proxies_allocator.c_ptr(), proxy );
}

} // namespace sound
} // namespace vostok
