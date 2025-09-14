////////////////////////////////////////////////////////////////////////////
//	Created		: 05.09.2011
//	Author		: Dmitry Kulikov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_SOUND_TWO_WAY_THREAD_CHANNEL_H_INCLUDED
#define VOSTOK_SOUND_TWO_WAY_THREAD_CHANNEL_H_INCLUDED

#include <vostok/one_way_threads_channel.h>
#include <vostok/sound/sound_object_commands.h>
#include <vostok/sound/functor_command.h>

namespace vostok {
namespace sound {

typedef one_way_threads_channel<
	sound_responses_queue_type,
	sound_responses_queue_type
>	responses_channel_type;

typedef one_way_threads_channel<
	sound_orders_queue_type,
	sound_orders_queue_type
>	orders_channel_type;

struct VOSTOK_SOUND_API two_way_threads_channel : private sound::noncopyable {
	inline two_way_threads_channel	(
			memory::base_allocator& responses_allocator,
			memory::base_allocator& orders_allocator
		) :
		responses	( responses_allocator ),
		orders		( orders_allocator )
	{
	}

	responses_channel_type	responses;
	orders_channel_type		orders;
}; // struct two_way_threads_channel

} // namespace sound
} // namespace vostok

#endif // #ifndef VOSTOK_SOUND_TWO_WAY_THREAD_CHANNEL_H_INCLUDED