////////////////////////////////////////////////////////////////////////////
//	Created		: 17.06.2009
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "apc.h"
#include <vostok/resources.h>

static vostok::fixed_vector<vostok::apc::callback, vostok::apc::count>	g_threads;

void vostok::apc::initialize				( )
{
	g_threads.resize		( count );
}

void vostok::apc::assign_thread_id		( threads_enum thread, u32 thread_id )
{
	g_threads[ thread ].m_thread_id	=	thread_id;
}

void vostok::apc::wait					( threads_enum const thread_id )
{
	apc::callback& thread	= g_threads[thread_id];
	while ( thread.m_pending ) {
		resources::dispatch_callbacks	( );
		threading::yield				( 1 );
	}
}

bool vostok::apc::is_same_thread			( threads_enum const thread_id )
{
	return					( g_threads[thread_id].m_thread_id == threading::current_thread_id() );
}

static inline void run					(
		vostok::apc::threads_enum const thread_id,
		vostok::apc::callback_type const& callback,
		vostok::apc::break_parameters const break_parameters,
		vostok::apc::wait_parameters const wait_parameters,
		bool remote_only
	)
{
	if ( is_same_thread(thread_id) ) {
		if ( remote_only )
			return;

		callback			( );
		return;
	}

	vostok::apc::wait			( thread_id );

	vostok::apc::callback& thread	= g_threads[thread_id];
	R_ASSERT				( !thread.m_pending );

	thread.m_callback		= callback;
	thread.m_break_parameters = break_parameters;
	vostok::threading::interlocked_exchange	( thread.m_pending, 1 );

	if ( wait_parameters == vostok::apc::wait_for_completion )
		wait				( thread_id );
	else
		R_ASSERT			( wait_parameters == vostok::apc::dont_wait_for_completion );
}

void vostok::apc::run						(
		threads_enum const thread_id,
		callback_type const& callback,
		break_parameters const break_parameters,
		wait_parameters const wait_parameters
	)
{
	::run					( thread_id, callback, break_parameters, wait_parameters, false );
}

void vostok::apc::run_remote_only			(
		threads_enum const thread_id,
		callback_type const& callback,
		break_parameters const break_parameters,
		wait_parameters const wait_parameters
	)
{
	::run					( thread_id, callback, break_parameters, wait_parameters, true );
}

void vostok::apc::process					( threads_enum const thread_id )
{
	R_ASSERT				( apc::is_same_thread(thread_id) );

	callback& thread		= g_threads[thread_id];

	for (;;)  {
		while ( !thread.m_pending )
		{
			resources::dispatch_callbacks( );
			threading::yield( 1 );
		}

		break_parameters const break_parameters = thread.m_break_parameters;
		thread.m_callback	( );
		threading::interlocked_exchange	( thread.m_pending, 0 );
		if ( break_parameters == break_process_loop )
			break;
	}
}

bool vostok::apc::try_process_single_call	( threads_enum const thread_id )
{
	callback& thread		= g_threads[thread_id];
	if ( !thread.m_pending )
		return				false;

	thread.m_callback		( );
	threading::interlocked_exchange	( thread.m_pending, false );
	return					true;
}