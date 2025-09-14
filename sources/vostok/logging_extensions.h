////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.08.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_LOGGING_EXTENSIONS_H_INCLUDED
#define VOSTOK_LOGGING_EXTENSIONS_H_INCLUDED

#if VOSTOK_CORE_BUILDING
#	define VOSTOK_LOG_MODULE_INITIATOR			"core"
#endif // #if VOSTOK_CORE_BUILDING

#ifndef VOSTOK_LOG_MODULE_INITIATOR
#	error you should define VOSTOK_LOG_MODULE_INITIATOR macro before including this header
#endif // #ifndef VOSTOK_LOG_MODULE_INITIATOR

#include <fastdelegate/fastdelegate.h>

namespace vostok {
namespace logging {

enum verbosity {
	invalid	= 0, // do not use it !!
	silent	= 1,//1 << 0,
	error	= 2,//1 << 1,
	warning	= 3,//1 << 2,
	info	= 4,//1 << 3,
	debug	= 5,//1 << 4,
	trace	= 6,//1 << 5,
	unset	= 1 << 31,
}; // enum verbosity

} // namespace logging
} // namespace vostok

#include <vostok/logging_helper.h>
#include <vostok/logging_settings.h>

namespace vostok {
namespace logging {

enum callback_flag {
	first	= 1 << 0,
	last	= 1 << 1,
}; // enum callback_flag

typedef	fastdelegate::FastDelegate<
	void (
		pcstr,					// file
		pcstr,					// function signature
		int	 ,					// line
		pcstr,					// initiator
		int	 ,					// verbosity
		pcstr,					// log string
		callback_flag   		// first/last string
	)
>				log_callback;

VOSTOK_CORE_API	void			flush				( pcstr file_name = 0 );

VOSTOK_CORE_API	void			set_log_callback	( log_callback callback );
VOSTOK_CORE_API	log_callback	get_log_callback	( );

VOSTOK_CORE_API	void			push_rule			( pcstr initiator, int verbosity, u32 thread_id = u32(-1) );
VOSTOK_CORE_API	void			pop_rule			( );
VOSTOK_CORE_API	void			on_thread_spawn		( );

VOSTOK_CORE_API	pcstr			thread_id_to_string	( u32 thread_id );

} // namespace logging
} // namespace vostok

#ifdef LOG_ERROR
#	error do not define LOG_ERROR macro
#endif // #ifdef LOG_ERROR
#ifdef LOG_WARNING
#	error do not define LOG_WARNING macro
#endif // #ifdef LOG_WARNING
#ifdef LOG_INFO
#	error do not define LOG_INFO macro
#endif // #ifdef LOG_INFO

#define LOG_ERROR( ... )				::vostok::logging::helper	( __FILE__, __FUNCSIG__, __LINE__, VOSTOK_LOG_MODULE_INITIATOR ":", ::vostok::logging::error	) ( ##__VA_ARGS__ )
#define LOG_WARNING( ... )				::vostok::logging::helper	( __FILE__, __FUNCSIG__, __LINE__, VOSTOK_LOG_MODULE_INITIATOR ":", ::vostok::logging::warning	) ( ##__VA_ARGS__ )
#define LOG_INFO( ... )					::vostok::logging::helper	( __FILE__, __FUNCSIG__, __LINE__, VOSTOK_LOG_MODULE_INITIATOR ":", ::vostok::logging::info		) ( ##__VA_ARGS__ )

#ifdef DEBUG
#	define LOG_TRACE( ... )				::vostok::logging::helper	( __FILE__, __FUNCSIG__, __LINE__, VOSTOK_LOG_MODULE_INITIATOR ":", ::vostok::logging::trace	) ( ##__VA_ARGS__ )
#	define LOGI_TRACE( initiator, ... )	::vostok::logging::helper	( __FILE__, __FUNCSIG__, __LINE__, VOSTOK_LOG_MODULE_INITIATOR ":" initiator ":", ::vostok::logging::trace) ( ##__VA_ARGS__ )
#else // #ifdef DEBUG
#	define LOG_TRACE( ... )
#	define LOGI_TRACE( ... )
#endif // #ifdef DEBUG

#ifndef VOSTOK_MASTER_GOLD
#	define LOG_DEBUG( ... )				::vostok::logging::helper	( __FILE__, __FUNCSIG__, __LINE__, VOSTOK_LOG_MODULE_INITIATOR ":", ::vostok::logging::debug	) ( ##__VA_ARGS__ )
#	define LOGI_DEBUG( initiator, ... )	::vostok::logging::helper	( __FILE__, __FUNCSIG__, __LINE__, VOSTOK_LOG_MODULE_INITIATOR ":" initiator ":", ::vostok::logging::debug) ( ##__VA_ARGS__ )
#else // #ifndef VOSTOK_MASTER_GOLD
#	define LOG_DEBUG( ... )
#	define LOGI_DEBUG( ... )
#endif // #ifndef VOSTOK_MASTER_GOLD

#define LOGI_ERROR( initiator, ... )	::vostok::logging::helper	( __FILE__, __FUNCSIG__, __LINE__, VOSTOK_LOG_MODULE_INITIATOR ":" initiator ":", ::vostok::logging::error)	( ##__VA_ARGS__ )
#define LOGI_WARNING( initiator, ... )	::vostok::logging::helper	( __FILE__, __FUNCSIG__, __LINE__, VOSTOK_LOG_MODULE_INITIATOR ":" initiator ":", ::vostok::logging::warning) ( ##__VA_ARGS__ )
#define LOGI_INFO( initiator, ... )		::vostok::logging::helper	( __FILE__, __FUNCSIG__, __LINE__, VOSTOK_LOG_MODULE_INITIATOR ":" initiator ":", ::vostok::logging::info) ( ##__VA_ARGS__ )
#define LOGI( initiator, type, ... )	::vostok::logging::helper	( __FILE__, __FUNCSIG__, __LINE__, VOSTOK_LOG_MODULE_INITIATOR ":" initiator ":", type) ( ##__VA_ARGS__ )

#endif // #ifndef VOSTOK_LOGGING_EXTENSIONS_H_INCLUDED