////////////////////////////////////////////////////////////////////////////
//	Created		: 24.03.2011
//	Author		: Tetyana Meleshchenko
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "sound_world.h"
#include <vostok/sound/world_user.h>
#include <vostok/sound/sound_instance_emitter.h>
#include <vostok/sound/sound_propagator_emitter.h>
#include <vostok/sound/single_sound.h>
#include <vostok/collision/api.h>
#include <vostok/collision/space_partitioning_tree.h>
#include <vostok/collision/object.h>
#include <vostok/sound/sound_receiver.h>
#include "voice_bridge.h"
#include "sound_instance_proxy_order.h"

namespace vostok {
namespace sound {

void sound_world::try_process_order	( sound_instance_proxy_order* order	)
{
	R_ASSERT						( order );
	return order->execute_functor	( );
}

void	sound_world::process_destroy_sound_instance_proxy (destroy_sound_instance_proxy_order* order )
{
	R_ASSERT											( order );

	sound_instance_proxy_internal* proxy				= &order->get_proxy();
	proxy->get_sound_scene().stop_propagate_sound		( *proxy );
	R_ASSERT											( proxy->get_propagators().empty() );
	proxy->get_sound_scene().free_sound_instance_proxy	( proxy );
}


void	sound_world::serialize_proxy	( sound_instance_proxy_internal* proxy, boost::function < void ( memory::writer*, memory::writer* ) >& fn, memory::writer* w )  const
{
	R_ASSERT									( proxy );
	memory::writer* sound_thread_writer			= VOSTOK_NEW_IMPL( g_allocator, memory::writer )( g_allocator );
	sound_thread_writer->write_u32				( proxy->get_propagators( ).size( ) );

	new_sound_propagator* prop		= proxy->get_propagators( ).front( );
	while ( prop )
	{
		prop->serialize				( *sound_thread_writer );
		prop						= proxy->get_propagators( ).get_next_of_object( prop );
	}

	functor_response* response		= VOSTOK_NEW_IMPL ( proxy->get_world_user().get_channel().responses.owner_allocator(), functor_response )
		( boost::bind( &sound_instance_proxy_internal::on_propagators_serialized, proxy, fn, sound_thread_writer, w ) );
	proxy->get_world_user().add_response	( response );
}

void	sound_world::on_proxy_serialized	( memory::writer* writer )
{
	R_ASSERT					( writer );
	VOSTOK_DELETE_IMPL			( g_allocator, writer );
}

void	sound_world::deserialize_proxy	( sound_instance_proxy_internal* proxy, memory::reader* r )
{
	R_ASSERT							( proxy );
	R_ASSERT							( r );
	VOSTOK_UNREFERENCED_PARAMETERS		( proxy, r );
}

} // namespace sound
} // namespace vostok
