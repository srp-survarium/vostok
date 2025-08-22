////////////////////////////////////////////////////////////////////////////
//	Created		: 24.05.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_STATISTICS_INLINE_H_INCLUDED
#define VOSTOK_RENDER_STATISTICS_INLINE_H_INCLUDED

#include <vostok/render/core/quasi_singleton.h>
#include <vostok/render/core/gpu_timer.h>

namespace vostok {
namespace render {

template< class T > statistics_value<T>::statistics_value(statistics_group* group, pcstr name): 
	statistics_base(group, name), value((T)0), history_index(0)
{
	vostok::memory::zero(history);
}

template< class T > void statistics_value<T>::start()
{
	history[history_index++]	= value;
	
	if (history_index == num_history_values)
		history_index = 0;
	
	value						= (T)0;
}

template< class T > T statistics_value<T>::average()
{
	T result = (T)0;
	for (u32 index = 0; index < num_history_values; index++)
	{
		result += history[index];
	}
	return result / num_history_values;
}

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_STATISTICS_COUNTERS_INLINE_H_INCLUDED