////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.08.2008
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef LOGGING_H_INCLUDED
#define LOGGING_H_INCLUDED

#include <fastdelegate/fastdelegate.h>
#include <vostok/core/core.h>

namespace vostok {
namespace logging {

	enum verbosity_recursion {
		recurse_all	=	0,
		recurse_0	=	256,
	}; // enum recurse_level

	void			preinitialize			( );
	void			initialize				( core::log_file_usage log_file_usage, int root_rule );
	void			finalize				( );

	pcstr			verbosity_to_string		( int verbosity );
	verbosity		string_to_verbosity 	( pcstr in_verbosity );

	bool			ready_for_use			( );
	void			ready_for_use			( bool value );

	pcstr			log_file_name			( );

	bool			initialize_console		( );
	void			initialize_stdout_if_needed ( );
	void			finalize_console		( );
	bool			use_console_for_logging ( );
	void			write_to_stdout			( pcstr format, ... );

	void			write_exit_code_file	( int error_code );

	threading::mutex&	get_log_mutex		( );
	void			try_lock_log_mutex		( );
	void			try_unlock_log_mutex	( );

	typedef memory::doug_lea_allocator_type	allocator_type;
	extern allocator_type					g_allocator;

#	define USER_ALLOCATOR					::vostok::logging::g_allocator
#	include <vostok/std_containers.h>
#	include <vostok/unique_ptr.h>
#	undef USER_ALLOCATOR

} // namespace logging
} // namespace vostok

#define LOG_NEW( type )								VOSTOK_NEW_IMPL(		::vostok::logging::g_allocator, type )
#define LOG_DELETE( pointer )						VOSTOK_DELETE_IMPL(	::vostok::logging::g_allocator, pointer )
#define LOG_MALLOC( size, description )				VOSTOK_MALLOC_IMPL(	::vostok::logging::g_allocator, size, description )
#define LOG_REALLOC( pointer, size, description )	VOSTOK_REALLOC_IMPL(	::vostok::logging::g_allocator, pointer, size, description )
#define LOG_FREE( pointer )							VOSTOK_FREE_IMPL(		::vostok::logging::g_allocator, pointer )
#define LOG_ALLOC( type, count )					VOSTOK_ALLOC_IMPL(	::vostok::logging::g_allocator, type, count )

#endif // #ifndef LOGGING_H_INCLUDED