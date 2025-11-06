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


#define LOG_ERROR( ... )
#define LOG_WARNING( ... )
#define LOG_INFO( ... )

#if 0 // sushi@TODO
#define __CHECK_VERBOSITY( level ) \
	::vostok::core::is_logging_initialized() &&	\
		::vostok::logging::has_passed_filters(	\
			::vostok::core::g_log_filter_tree,	\
			VOSTOK_LOG_MODULE_INITIATOR ":",	\
			level )

#define __LOG( level, ... )	__CHECK_VERBOSITY( level )	\
		? ::vostok::logging::append(					\
				NULL,									\
				::vostok::core::g_log_flags,			\
				&::vostok::core::g_log_format,			\
				__FILE__,								\
				__LINE__,								\
				__FUNCSIG__,							\
				VOSTOK_LOG_MODULE_INITIATOR ":",		\
				level,									\
				( ##__VA_ARGS__ ) )						\
		: NULL


#define LOG_ERROR( ... )	__LOG( ::vostok::logging::error )
#define LOG_WARNING( ... )	__LOG( ::vostok::logging::warning )
#define LOG_INFO( ... )		__LOG( ::vostok::logging::info )
#endif // #if 0


#if 0 // sushi@TODO
#define LOG_ERROR( ... )				::vostok::logging::check_verbosity(VOSTOK_LOG_MODULE_INITIATOR ":" , ::vostok::logging::error) ? \
										::vostok::logging::helper	( __FILE__, __FUNCSIG__, __LINE__, VOSTOK_LOG_MODULE_INITIATOR ":", ::vostok::logging::error	) ( ##__VA_ARGS__ ) : 0

#define LOG_WARNING( ... )				::vostok::logging::check_verbosity(VOSTOK_LOG_MODULE_INITIATOR ":" , ::vostok::logging::warning) ? \
										::vostok::logging::helper	( __FILE__, __FUNCSIG__, __LINE__, VOSTOK_LOG_MODULE_INITIATOR ":", ::vostok::logging::warning	) ( ##__VA_ARGS__ ) : 0

#define LOG_INFO( ... )					::vostok::logging::check_verbosity(VOSTOK_LOG_MODULE_INITIATOR ":" , ::vostok::logging::info) ? \
										::vostok::logging::helper	( __FILE__, __FUNCSIG__, __LINE__, VOSTOK_LOG_MODULE_INITIATOR ":", ::vostok::logging::info		) ( ##__VA_ARGS__ ) : 0
#endif // #if 0

#ifdef DEBUG
#	define LOG_TRACE( ... )				::vostok::logging::check_verbosity(VOSTOK_LOG_MODULE_INITIATOR ":" , ::vostok::logging::trace) ? \
										::vostok::logging::helper	( __FILE__, __FUNCSIG__, __LINE__, VOSTOK_LOG_MODULE_INITIATOR ":", ::vostok::logging::trace	) ( ##__VA_ARGS__ ) : 0
#	define LOGI_TRACE( initiator, ... )	::vostok::logging::check_verbosity(VOSTOK_LOG_MODULE_INITIATOR ":" initiator ":", ::vostok::logging::trace) ? \
										::vostok::logging::helper	( __FILE__, __FUNCSIG__, __LINE__, VOSTOK_LOG_MODULE_INITIATOR ":" initiator ":", ::vostok::logging::trace) ( ##__VA_ARGS__ ) : 0
#else // #ifdef DEBUG
#	define LOG_TRACE( ... )
#	define LOGI_TRACE( ... )
#endif // #ifdef DEBUG


#	define LOG_DEBUG( ... )
#	define LOGI_DEBUG( ... )
#if 0
#ifndef VOSTOK_MASTER_GOLD
#	define LOG_DEBUG( ... )				::vostok::logging::check_verbosity(VOSTOK_LOG_MODULE_INITIATOR ":" , ::vostok::logging::debug) ? \
										::vostok::logging::helper	( __FILE__, __FUNCSIG__, __LINE__, VOSTOK_LOG_MODULE_INITIATOR ":", ::vostok::logging::debug	) ( ##__VA_ARGS__ ) : 0

#	define LOGI_DEBUG( initiator, ... )	::vostok::logging::check_verbosity(VOSTOK_LOG_MODULE_INITIATOR ":" initiator ":", ::vostok::logging::debug) ? \
										::vostok::logging::helper	( __FILE__, __FUNCSIG__, __LINE__, VOSTOK_LOG_MODULE_INITIATOR ":" initiator ":", ::vostok::logging::debug) ( ##__VA_ARGS__ ) : 0
#else // #ifndef VOSTOK_MASTER_GOLD
#	define LOG_DEBUG( ... )
#	define LOGI_DEBUG( ... )
#endif // #ifndef VOSTOK_MASTER_GOLD
#endif // #if 0

#define LOGI_ERROR( initiator, ... )
#define LOGI_WARNING( initiator, ... )
#define LOGI_INFO( initiator, ... )
#define LOGI( initiator, type, ... )
#define LOGI_FORCED( initiator, type, ... )
#define LOG_FORCED( type, ... )
#if 0 // sushi@TODO
#define LOGI_ERROR( initiator, ... )	::vostok::logging::check_verbosity(VOSTOK_LOG_MODULE_INITIATOR ":" initiator ":", ::vostok::logging::error) ? \
										::vostok::logging::helper	( __FILE__, __FUNCSIG__, __LINE__, VOSTOK_LOG_MODULE_INITIATOR ":" initiator ":", ::vostok::logging::error)	( ##__VA_ARGS__ ) : 0

#define LOGI_WARNING( initiator, ... )	::vostok::logging::check_verbosity(VOSTOK_LOG_MODULE_INITIATOR ":" initiator ":", ::vostok::logging::warning) ? \
										::vostok::logging::helper	( __FILE__, __FUNCSIG__, __LINE__, VOSTOK_LOG_MODULE_INITIATOR ":" initiator ":", ::vostok::logging::warning) ( ##__VA_ARGS__ ) : 0

#define LOGI_INFO( initiator, ... )		::vostok::logging::check_verbosity(VOSTOK_LOG_MODULE_INITIATOR ":" initiator ":", ::vostok::logging::info) ? \
										::vostok::logging::helper	( __FILE__, __FUNCSIG__, __LINE__, VOSTOK_LOG_MODULE_INITIATOR ":" initiator ":", ::vostok::logging::info) ( ##__VA_ARGS__ ) : 0

#define LOGI( initiator, type, ... )	::vostok::logging::check_verbosity(VOSTOK_LOG_MODULE_INITIATOR ":" initiator ":", type) ? \
										::vostok::logging::helper	( __FILE__, __FUNCSIG__, __LINE__, VOSTOK_LOG_MODULE_INITIATOR ":" initiator ":", type) ( ##__VA_ARGS__ ) : 0

#define LOGI_FORCED( initiator, type, ... )	::vostok::logging::helper	( __FILE__, __FUNCSIG__, __LINE__, VOSTOK_LOG_MODULE_INITIATOR ":" initiator ":", type) ( ##__VA_ARGS__ )

#define LOG_FORCED( type, ... )				::vostok::logging::helper	( __FILE__, __FUNCSIG__, __LINE__, VOSTOK_LOG_MODULE_INITIATOR ":", type) ( ##__VA_ARGS__ )
#endif // #if 0

#endif // #ifndef VOSTOK_LOGGING_LOGGING_MACROS_H_INCLUDED