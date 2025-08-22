////////////////////////////////////////////////////////////////////////////
//	Created		: 27.09.2010
//	Author		: Konstantin Slipchenko
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/animation/event_channel.h>

namespace vostok {
namespace animation {



u32		event_channel::internal_memory_size	()const
{
	return time_channel_type::count_internal_memory_size( m_time_channel.knots_count() );
}



} // namespace animation
} // namespace vostok