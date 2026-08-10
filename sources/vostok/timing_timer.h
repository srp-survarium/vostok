////////////////////////////////////////////////////////////////////////////
//	Created 	: 13.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_TIMING_TIMER_H_INCLUDED
#define VOSTOK_TIMING_TIMER_H_INCLUDED

namespace vostok {
namespace timing {

class VOSTOK_CORE_API timer
{
public:
	inline				timer					();

	inline	void		start					();

	inline	u64			get_elapsed_ticks		() const;
	inline	u32			get_elapsed_msec		() const;
	inline	float		get_elapsed_sec			() const;

	inline	void		set_time_factor			( float const time_factor );
	inline	float		get_time_factor			() const;

	inline	bool		is_paused				() const;
	inline	void		pause					();
	inline	void		resume					();
private:
	u64			m_current_time;
	u64			m_start_time;
	float		m_time_factor;
	float		m_backup_time_factor;
}; // class timer

STATIC_SIZE_ASSERT(timer, 0x18);

} // namespace timing
} // namespace vostok

#include <vostok/timing_timer_inline.h>

#endif // #ifndef VOSTOK_TIMING_TIMER_H_INCLUDED
