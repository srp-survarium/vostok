////////////////////////////////////////////////////////////////////////////
//	Created		: 17.06.2009
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "rpc.h"
#include <vostok/resources.h>

static vostok::fixed_vector<vostok::rpc::callback, vostok::rpc::count>	g_threads;

void vostok::rpc::initialize				( )
{
	g_threads.resize		( count );
}

void vostok::rpc::assign_thread_id		( threads_enum thread, u32 thread_id )
{
	g_threads[ thread ].m_thread_id	=	thread_id;
}

void vostok::rpc::wait					( threads_enum const thread_id )
{
	rpc::callback& thread	= g_threads[thread_id];
	while ( thread.m_pending )
		threading::yield	( 1 );
}

bool vostok::rpc::is_same_thread			( threads_enum const thread_id )
{
	return					( g_threads[thread_id].m_thread_id == threading::current_thread_id() );
}

static inline void run					(
		vostok::rpc::threads_enum const thread_id,
		vostok::rpc::callback_type const& callback,
		vostok::rpc::break_parameters const break_parameters,
		vostok::rpc::wait_parameters const wait_parameters,
		bool remote_only
	)
{
	if ( is_same_thread(thread_id) ) {
		if ( remote_only )
			return;

		callback			( );
		return;
	}

	vostok::rpc::wait			( thread_id );

	vostok::rpc::callback& thread	= g_threads[thread_id];
	R_ASSERT				( !thread.m_pending );

	thread.m_callback		= callback;
	thread.m_break_parameters = break_parameters;
	vostok::threading::interlocked_exchange	( thread.m_pending, 1 );

	if ( wait_parameters == vostok::rpc::wait_for_completion )
		wait				( thread_id );
	else
		R_ASSERT			( wait_parameters == vostok::rpc::dont_wait_for_completion );
}

void vostok::rpc::run						(
		threads_enum const thread_id,
		callback_type const& callback,
		break_parameters const break_parameters,
		wait_parameters const wait_parameters
	)
{
	::run					( thread_id, callback, break_parameters, wait_parameters, false );
}

void vostok::rpc::run_remote_only			(
		threads_enum const thread_id,
		callback_type const& callback,
		break_parameters const break_parameters,
		wait_parameters const wait_parameters
	)
{
	::run					( thread_id, callback, break_parameters, wait_parameters, true );
}

void vostok::rpc::process					( threads_enum const thread_id )
{
	R_ASSERT				( rpc::is_same_thread(thread_id) );

	callback& thread		= g_threads[thread_id];

	for (;;)  {
		while ( !thread.m_pending )
			threading::yield( 1 );

		break_parameters const break_parameters = thread.m_break_parameters;
		thread.m_callback	( );
		threading::interlocked_exchange	( thread.m_pending, 0 );
		if ( break_parameters == break_process_loop )
			break;
	}
}

bool vostok::rpc::try_process_single_call	( threads_enum const thread_id )
{
	callback& thread		= g_threads[thread_id];
	if ( !thread.m_pending )
		return				false;

	thread.m_callback		( );
	threading::interlocked_exchange	( thread.m_pending, false );
	return					true;
}