////////////////////////////////////////////////////////////////////////////
//	Created 	: 09.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef MEMORY_MONITOR_H_INCLUDED
#define MEMORY_MONITOR_H_INCLUDED

#if defined(NDEBUG) || !defined(_MSC_VER)
#	define VOSTOK_USE_MEMORY_MONITOR	0
#else // #ifdef NDEBUG
#	define VOSTOK_USE_MEMORY_MONITOR	1
#endif // #ifdef NDEBUG

#if VOSTOK_USE_MEMORY_MONITOR

namespace vostok {
namespace memory {
namespace monitor {

		void	initialize				( );
		void	finalize				( );

		void	on_alloc				( pvoid& pointer, size_t& size, size_t previous_size, pcstr description );
		void	on_free					( pvoid& pointer, bool can_clear = true );
inline	pvoid	get_real_pointer		( pvoid pointer )	{ return pointer; }
inline	u32		get_house_keeping_size	( )					{ return 0; }

} // namespace monitor
} // namespace memory
} // namespace vostok

#endif // #if VOSTOK_USE_MEMORY_MONITOR

#endif // #ifndef MEMORY_MONITOR_H_INCLUDED