////////////////////////////////////////////////////////////////////////////
//	Created		: 15.08.2011
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_LOGGING_API_H_INCLUDED
#define VOSTOK_LOGGING_API_H_INCLUDED

#ifndef VOSTOK_LOGGING_API
#	ifdef VOSTOK_STATIC_LIBRARIES
#		define VOSTOK_LOGGING_API
#	else // #ifdef VOSTOK_STATIC_LIBRARIES
#		ifdef VOSTOK_LOGGING_BUILDING
#			define VOSTOK_LOGGING_API		VOSTOK_DLL_EXPORT
#		else // #ifdef VOSTOK_LOGGING_BUILDING
#			ifndef VOSTOK_ENGINE_BUILDING
#				define VOSTOK_LOGGING_API	VOSTOK_DLL_IMPORT
#			else // #ifndef VOSTOK_ENGINE_BUILDING
#				define VOSTOK_LOGGING_API	VOSTOK_DLL_EXPORT
#			endif // #ifndef VOSTOK_ENGINE_BUILDING
#		endif // #ifdef VOSTOK_LOGGING_BUILDING
#	endif // #ifdef VOSTOK_STATIC_LIBRARIES
#endif // #ifndef VOSTOK_LOGGING_API

#include <vostok/fs/device_file_system_proxy.h>

namespace vostok {
namespace logging {

enum log_file_usage;

enum verbosity {
	invalid		=	 0,	//	do not use it !!
	silent		=	 1,	//	1 << 0,
	error		=	 2,	//	1 << 1,
	warning		=	 3,	//	1 << 2,
	info		=	 4,	//	1 << 3,
	debug		=	 5,	//	1 << 4,
	trace		=	 6,	//	1 << 5,
	unset		=	 1 << 31,
}; // enum verbosity

VOSTOK_LOGGING_API	void				preinitialize			( );
VOSTOK_LOGGING_API	void				initialize				( fs_new::device_file_system_proxy	device, 
																  log_file_usage					log_file_usage );
VOSTOK_LOGGING_API	void				finalize				( );
VOSTOK_LOGGING_API	pcstr				verbosity_to_string		( int verbosity );
VOSTOK_LOGGING_API	verbosity			string_to_verbosity 	( pcstr in_verbosity );
VOSTOK_LOGGING_API	bool				use_console_for_logging ( );

enum stdstream_enum { stdstream_out, stdstream_error };
VOSTOK_LOGGING_API	void				write_to_stdstream		( stdstream_enum stream, pcstr format, ... );

} // namespace logging
} // namespace vostok

#endif // #ifndef VOSTOK_LOGGING_API_H_INCLUDED