////////////////////////////////////////////////////////////////////////////
//	Created		: 25.08.2011
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_LOGGING_LOGGING_MACROS_H_INCLUDED
#define VOSTOK_LOGGING_LOGGING_MACROS_H_INCLUDED

#ifdef LOG_ERROR
#	error do not define LOG_ERROR macro
#endif // #ifdef LOG_ERROR
#ifdef LOG_WARNING
#	error do not define LOG_WARNING macro
#endif // #ifdef LOG_WARNING
#ifdef LOG_INFO
#	error do not define LOG_INFO macro
#endif // #ifdef LOG_INFO


#define __CHECK_VERBOSITY( level ) 										\
	::vostok::core::g_log_filter_tree == NULL ||						\
		::vostok::logging::has_passed_filters(							\
			*::vostok::core::g_log_filter_tree,							\
			VOSTOK_LOG_MODULE_INITIATOR ":",							\
			level )

#define __LOG( level, format, data, initiator, ... )	__CHECK_VERBOSITY( level )			\
		? ::vostok::logging::append(														\
				::vostok::core::g_log_callback,										\
				(void*)data,																\
				format,																		\
				__FILE__,																	\
				__LINE__,																	\
				__FUNCSIG__,																\
				initiator,																	\
				level,																		\
				##__VA_ARGS__  )															\
		: NULL

#define __LOG_FORCED( level, format, data, initiator, ... )									\
		::vostok::logging::append(															\
				::vostok::core::g_log_callback,										\
				(void*)data,																\
				format,																		\
				__FILE__,																	\
				__LINE__,																	\
				__FUNCSIG__,																\
				initiator,																	\
				level,																		\
				##__VA_ARGS__  )


#define LOG_ERROR( ... )	__LOG( ::vostok::logging::error,	&::vostok::core::g_log_format, ::vostok::core::g_log_flags, VOSTOK_LOG_MODULE_INITIATOR ":", ##__VA_ARGS__ )
#define LOG_WARNING( ... )	__LOG( ::vostok::logging::warning,	&::vostok::core::g_log_format, ::vostok::core::g_log_flags, VOSTOK_LOG_MODULE_INITIATOR ":", ##__VA_ARGS__ )
#define LOG_INFO( ... )		__LOG( ::vostok::logging::info,		&::vostok::core::g_log_format, ::vostok::core::g_log_flags, VOSTOK_LOG_MODULE_INITIATOR ":", ##__VA_ARGS__ )

// sushi@NOTE: Those prefixes are definitely incorrect, though PDB files do not contain any symbols to deal with them.
// Possibly we can write some inline functions over `logging::append`. Though doing this easily is problematic, since it requires varargs at the end.
#define LOGF_ERROR( format, ... )	__LOG( ::vostok::logging::error,	format, ::vostok::core::g_log_flags, VOSTOK_LOG_MODULE_INITIATOR ":", ##__VA_ARGS__ )
#define LOGF_WARNING( format, ... )	__LOG( ::vostok::logging::warning,	format, ::vostok::core::g_log_flags, VOSTOK_LOG_MODULE_INITIATOR ":", ##__VA_ARGS__ )
#define LOGF_INFO( format, ... )	__LOG( ::vostok::logging::info,		format, ::vostok::core::g_log_flags, VOSTOK_LOG_MODULE_INITIATOR ":", ##__VA_ARGS__ )

#define LOGFD_ERROR( format, data, ... )	__LOG( ::vostok::logging::error,	format, data, VOSTOK_LOG_MODULE_INITIATOR ":", ##__VA_ARGS__ )
#define LOGFD_WARNING( format, data, ... )	__LOG( ::vostok::logging::warning,	format, data, VOSTOK_LOG_MODULE_INITIATOR ":", ##__VA_ARGS__ )
#define LOGFD_INFO( format, data, ... )		__LOG( ::vostok::logging::info,		format, data, VOSTOK_LOG_MODULE_INITIATOR ":", ##__VA_ARGS__ )

#define LOGI_ERROR( initiator, ... )	__LOG( ::vostok::logging::error,	&::vostok::core::g_log_format, ::vostok::core::g_log_flags, VOSTOK_LOG_MODULE_INITIATOR ":" initiator ":", ##__VA_ARGS__ )
#define LOGI_WARNING( initiator, ... )	__LOG( ::vostok::logging::warning,	&::vostok::core::g_log_format, ::vostok::core::g_log_flags, VOSTOK_LOG_MODULE_INITIATOR ":" initiator ":", ##__VA_ARGS__ )
#define LOGI_INFO( initiator, ... )		__LOG( ::vostok::logging::info,		&::vostok::core::g_log_format, ::vostok::core::g_log_flags, VOSTOK_LOG_MODULE_INITIATOR ":" initiator ":", ##__VA_ARGS__ )

#define LOGIFD_ERROR( initiator, format, data, ... )	__LOG( ::vostok::logging::error,	format, data, initiator ":", ##__VA_ARGS__ )
#define LOGIFD_WARNING( initiator, format, data, ... )	__LOG( ::vostok::logging::warning,	format, data, initiator ":", ##__VA_ARGS__ )
#define LOGIFD_INFO( initiator, format, data, ... )		__LOG( ::vostok::logging::info,		format, data, initiator ":", ##__VA_ARGS__ )

#define LOGIFD_FORCED( initiator, type, format, data, ... )	__LOG_FORCED( type, format, data, initiator ":", ##__VA_ARGS__ )


#ifdef DEBUG
#	define LOG_TRACE( ... )				__LOG( ::vostok::logging::trace, &::vostok::core::g_log_format, ::vostok::core::g_log_flags, VOSTOK_LOG_MODULE_INITIATOR ":",				##__VA_ARGS__ )
#	define LOGI_TRACE( initiator, ... )	__LOG( ::vostok::logging::trace, &::vostok::core::g_log_format, ::vostok::core::g_log_flags, VOSTOK_LOG_MODULE_INITIATOR ":" initiator ":",	##__VA_ARGS__ )
#else // #ifdef DEBUG
#	define LOG_TRACE( ... )
#	define LOGI_TRACE( ... )
#endif // #ifdef DEBUG

#ifndef VOSTOK_MASTER_GOLD
#	define LOG_DEBUG( ... )				__LOG( ::vostok::logging::debug, &::vostok::core::g_log_format, ::vostok::core::g_log_flags, VOSTOK_LOG_MODULE_INITIATOR ":",				##__VA_ARGS__ )
#	define LOGI_DEBUG( initiator, ... )	__LOG( ::vostok::logging::debug, &::vostok::core::g_log_format, ::vostok::core::g_log_flags, VOSTOK_LOG_MODULE_INITIATOR ":" initiator ":",	##__VA_ARGS__ )
#else // #ifndef VOSTOK_MASTER_GOLD
#	define LOG_DEBUG( ... )
#	define LOGI_DEBUG( ... )
#endif // #ifndef VOSTOK_MASTER_GOLD

#define LOGI( initiator, type, ... )		__LOG( type,	&::vostok::core::g_log_format, ::vostok::core::g_log_flags, VOSTOK_LOG_MODULE_INITIATOR ":" initiator ":", ##__VA_ARGS__ )

#define LOG_FORCED( type, ... )					__LOG_FORCED( type,	&::vostok::core::g_log_format,	::vostok::core::g_log_flags, VOSTOK_LOG_MODULE_INITIATOR ":",				##__VA_ARGS__ )
#define LOGFD_FORCED( type, format, data, ... )	__LOG_FORCED( type,	format,							data,						 VOSTOK_LOG_MODULE_INITIATOR ":",				##__VA_ARGS__ )
#define LOGI_FORCED( initiator, type, ... )		__LOG_FORCED( type,	&::vostok::core::g_log_format,	::vostok::core::g_log_flags, VOSTOK_LOG_MODULE_INITIATOR ":" initiator ":",	##__VA_ARGS__ )

#endif // #ifndef VOSTOK_LOGGING_LOGGING_MACROS_H_INCLUDED
