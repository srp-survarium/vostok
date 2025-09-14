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

#define LOG_ERROR( ... )				::vostok::logging::check_verbosity(VOSTOK_LOG_MODULE_INITIATOR ":" , ::vostok::logging::error) ? \
										::vostok::logging::helper	( __FILE__, __FUNCSIG__, __LINE__, VOSTOK_LOG_MODULE_INITIATOR ":", ::vostok::logging::error	) ( ##__VA_ARGS__ ) : 0

#define LOG_WARNING( ... )				::vostok::logging::check_verbosity(VOSTOK_LOG_MODULE_INITIATOR ":" , ::vostok::logging::warning) ? \
										::vostok::logging::helper	( __FILE__, __FUNCSIG__, __LINE__, VOSTOK_LOG_MODULE_INITIATOR ":", ::vostok::logging::warning	) ( ##__VA_ARGS__ ) : 0

#define LOG_INFO( ... )					::vostok::logging::check_verbosity(VOSTOK_LOG_MODULE_INITIATOR ":" , ::vostok::logging::info) ? \
										::vostok::logging::helper	( __FILE__, __FUNCSIG__, __LINE__, VOSTOK_LOG_MODULE_INITIATOR ":", ::vostok::logging::info		) ( ##__VA_ARGS__ ) : 0

#ifdef DEBUG
#	define LOG_TRACE( ... )				::vostok::logging::check_verbosity(VOSTOK_LOG_MODULE_INITIATOR ":" , ::vostok::logging::trace) ? \
										::vostok::logging::helper	( __FILE__, __FUNCSIG__, __LINE__, VOSTOK_LOG_MODULE_INITIATOR ":", ::vostok::logging::trace	) ( ##__VA_ARGS__ ) : 0
#	define LOGI_TRACE( initiator, ... )	::vostok::logging::check_verbosity(VOSTOK_LOG_MODULE_INITIATOR ":" initiator ":", ::vostok::logging::trace) ? \
										::vostok::logging::helper	( __FILE__, __FUNCSIG__, __LINE__, VOSTOK_LOG_MODULE_INITIATOR ":" initiator ":", ::vostok::logging::trace) ( ##__VA_ARGS__ ) : 0
#else // #ifdef DEBUG
#	define LOG_TRACE( ... )
#	define LOGI_TRACE( ... )
#endif // #ifdef DEBUG

#ifndef VOSTOK_MASTER_GOLD
#	define LOG_DEBUG( ... )				::vostok::logging::check_verbosity(VOSTOK_LOG_MODULE_INITIATOR ":" , ::vostok::logging::debug) ? \
										::vostok::logging::helper	( __FILE__, __FUNCSIG__, __LINE__, VOSTOK_LOG_MODULE_INITIATOR ":", ::vostok::logging::debug	) ( ##__VA_ARGS__ ) : 0

#	define LOGI_DEBUG( initiator, ... )	::vostok::logging::check_verbosity(VOSTOK_LOG_MODULE_INITIATOR ":" initiator ":", ::vostok::logging::debug) ? \
										::vostok::logging::helper	( __FILE__, __FUNCSIG__, __LINE__, VOSTOK_LOG_MODULE_INITIATOR ":" initiator ":", ::vostok::logging::debug) ( ##__VA_ARGS__ ) : 0
#else // #ifndef VOSTOK_MASTER_GOLD
#	define LOG_DEBUG( ... )
#	define LOGI_DEBUG( ... )
#endif // #ifndef VOSTOK_MASTER_GOLD

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


#endif // #ifndef VOSTOK_LOGGING_LOGGING_MACROS_H_INCLUDED