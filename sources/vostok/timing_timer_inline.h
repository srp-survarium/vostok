////////////////////////////////////////////////////////////////////////////
//	Created 	: 13.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_TIMING_TIMER_INLINE_H_INCLUDED
#define VOSTOK_TIMING_TIMER_INLINE_H_INCLUDED

#include <vostok/timing_functions.h>

namespace vostok {
namespace timing {

inline timer::timer						() :
	m_current_time		(0),
	m_start_time		(get_QPC()),
	m_time_factor		(1.0f),
	m_backup_time_factor(1.0f)
{}

inline void timer::start				()
{
	m_start_time				= get_QPC();
	m_current_time				= 0;
}

inline u64 timer::get_elapsed_ticks		() const
{
	return m_current_time + u64(m_time_factor*float(get_QPC() - m_start_time));
}

inline u32 timer::get_elapsed_msec		() const
{
	return ( u32( get_elapsed_ticks( )*u64( 1000 ) / g_qpc_per_second ) );
}

inline float timer::get_elapsed_sec		() const
{
	return ( float( double(get_elapsed_ticks())/g_qpc_per_second) );
}

inline void timer::set_time_factor		( float const time_factor )
{
	R_ASSERT					( !(time_factor < 0.0f) );
	m_current_time				= get_elapsed_ticks();
	m_start_time				= get_QPC();
	m_time_factor				= time_factor;
}

inline float timer::get_time_factor		() const
{
	return m_time_factor;
}

inline bool timer::is_paused			() const
{
	return m_time_factor == 0.0f;
}

inline void timer::pause				()
{
	m_backup_time_factor		= m_time_factor;
	set_time_factor				( 0.0f );
}

inline void timer::resume				()
{
	set_time_factor				( m_backup_time_factor );
}

} // namespace timing
} // namespace vostok

#endif // #ifndef VOSTOK_TIMING_TIMER_INLINE_H_INCLUDED
