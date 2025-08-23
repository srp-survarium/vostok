////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_DEBUG_MACROS_H_INCLUDED
#define VOSTOK_DEBUG_MACROS_H_INCLUDED

#if defined (R_ASSERT) || defined (R_ASSERT_T) || defined (R_ASSERT_U) || defined (R_ASSERT_T_U) || defined (R_ASSERT_CMP) || defined (R_ASSERT_CMP_T) || defined (R_ASSERT_CMP_U) || defined (R_ASSERT_CMP_T_U)
#	error please do not define R_ASSERT, R_ASSERT_T, R_ASSERT_U, R_ASSERT_T_U, R_ASSERT_CMP, R_ASSERT_CMP_T, R_ASSERT_CMP_U and R_ASSERT_CMP_T_U macros
#endif // #if defined (R_ASSERT) || defined (R_ASSERT_T) || defined (R_ASSERT_U) || defined (R_ASSERT_T_U) || defined (R_ASSERT_CMP) || defined (R_ASSERT_CMP_T) || defined (R_ASSERT_CMP_U) || defined (R_ASSERT_CMP_T_U)

#if defined (ASSERT) || defined (ASSERT_T) || defined (ASSERT_U) || defined (ASSERT_T_U) || defined (ASSERT_CMP) || defined (ASSERT_CMP_T) || defined (ASSERT_CMP_U) || defined (ASSERT_CMP_T_U)
#	error please do not define ASSERT, ASSERT_T, ASSERT_U, ASSERT_T_U, ASSERT_CMP, ASSERT_CMP_T, ASSERT_CMP_U and ASSERT_CMP_T_U macros
#endif // #if defined (ASSERT) || defined (ASSERT_T) || defined (ASSERT_U) || defined (ASSERT_T_U) || defined (ASSERT_CMP) || defined (ASSERT_CMP_T) || defined (ASSERT_CMP_U) || defined (ASSERT_CMP_T_U)

#if defined (CURE_ASSERT) || defined (CURE_ASSERT_CMP)
#	error please do not define CURE_ASSERT and CURE_ASSERT_CMP macros
#endif // #if defined (CURE_ASSERT) defined (CURE_ASSERT_CMP)

#if defined (FATAL) || defined (UNREACHABLE_CODE) || defined (NODEFAULT) || defined (NOT_IMPLEMENTED) || defined (CHECK_AND_CORRECT) || defined (CHECK_OR_EXIT)
#	error please do not define FATAL, UNREACHABLE_CODE, NODEFAULT, NOT_IMPLEMENTED, CHECK_AND_CONTINUE and CHECK_OR_EXIT macros
#endif // #if defined (FATAL) || defined (UNREACHABLE_CODE) || defined (NODEFAULT) || defined (NOT_IMPLEMENTED) || defined (CHECK_AND_CORRECT) || defined (CHECK_OR_EXIT)

#if VOSTOK_PLATFORM_WINDOWS
#	define VOSTOK_ANALYSIS_ASSUME				__analysis_assume
#	define DEBUG_BREAK						__debugbreak
#	define VOSTOK_INIT_PRIORITY( priority )
#elif VOSTOK_PLATFORM_XBOX_360	// #if VOSTOK_PLATFORM_WINDOWS
#	define VOSTOK_ANALYSIS_ASSUME				__analysis_assume
#	define DEBUG_BREAK						__debugbreak
#	define VOSTOK_INIT_PRIORITY( priority )
#elif VOSTOK_PLATFORM_PS3			// #elif VOSTOK_PLATFORM_XBOX_360
#	define VOSTOK_ANALYSIS_ASSUME( ... )		(void)0
#	define DEBUG_BREAK						__builtin_trap
#	define VOSTOK_INIT_PRIORITY( priority )	__attribute__( (init_priority(101 + priority)) )
#endif // #if VOSTOK_PLATFORM_WINDOWS

////////////////////////////////////////////////////////////////////////////
// macros helpers
////////////////////////////////////////////////////////////////////////////

namespace vostok {

enum assert_enum
{
	assert_untyped,
}; // enum assert_enum

enum process_error_enum
{
	process_error_false,
	process_error_true,
	process_error_to_message_box
}; // enum process_error_enum

inline u32	assert_log_times_in_gold () { return 10; }

#ifndef _MANAGED
	template <typename T>
	inline T const & identity( T const & value ) { return value; }
	template <typename T>
	inline T const * identity( T const * value ) { return value; }
#endif // #ifndef _MANAGED

} // namespace vostok

#define VOSTOK_ASSERT_HELPER( ignore_always, process_error, assertion_string, assert_type, ... )	\
	if ( !ignore_always ) {													\
		bool do_debug_break = false;										\
		::vostok::debug::on_error	(											\
			& do_debug_break,												\
			process_error,													\
			& ignore_always,												\
			assert_type,													\
			"assertion_failed",												\
			assertion_string,												\
			__FILE__,														\
			__FUNCTION__,													\
			__LINE__, 														\
			##__VA_ARGS__													\
		);																	\
		if ( ::vostok::debug::is_debugger_present( ) || do_debug_break )		\
			DEBUG_BREAK	( );												\
	}

#define VOSTOK_EMPTY_EXPRESSION												if ( ::vostok::identity(false) ) { } else (void)0
#define VOSTOK_EMPTY_EXPRESSION_VA_ARGS( ... )								VOSTOK_EMPTY_EXPRESSION
#define VOSTOK_EMPTY_EXPRESSION_U_VA_ARGS( assertion_type, expression, ... )	if ( ::vostok::identity(false) ) { ::vostok::debug::detail::expression_eater(assertion_type, expression, ##__VA_ARGS__); } else (void)0

////////////////////////////////////////////////////////////////////////////
// R_ASSERT* macros
////////////////////////////////////////////////////////////////////////////
#ifdef MASTER_GOLD
#	define FATAL( format, ... )												VOSTOK_EMPTY_EXPRESSION

#	define R_ASSERT_T														VOSTOK_EMPTY_EXPRESSION_VA_ARGS
#	define R_ASSERT_CMP_T( value1, operation, value2, assert_type, ... )	VOSTOK_EMPTY_EXPRESSION

#	define R_ASSERT_T_U														VOSTOK_EMPTY_EXPRESSION_U_VA_ARGS
#	define R_ASSERT_CMP_T_U( value1, operation, value2, assert_type, ... )	if ( ::vostok::identity(false) ) { ::vostok::debug::detail::expression_eater(value1, value2, assert_type, ##__VA_ARGS__); } else (void)0

#	define R_ASSERT_BOX														VOSTOK_EMPTY_EXPRESSION_VA_ARGS

#else // #ifdef MASTER_GOLD
#	define FATAL( format, ... )												\
		if ( ::vostok::identity(true) ) {										\
			static bool debug_macro_helper_ignore_always = false;			\
			if ( !debug_macro_helper_ignore_always ) {						\
				VOSTOK_ASSERT_HELPER(											\
					debug_macro_helper_ignore_always,						\
					::vostok::process_error_true,								\
					"fatal error",											\
					::vostok::assert_untyped,									\
					format,													\
					##__VA_ARGS__											\
				)															\
			}																\
		}																	\
		else (void)0

#	define R_ASSERT_T( assert_type, expression, ... )						\
		if ( ::vostok::identity(true) ) {										\
			static bool debug_macro_helper_ignore_always = false;			\
			if ( !debug_macro_helper_ignore_always && !::vostok::identity(expression) ) {	\
				VOSTOK_ASSERT_HELPER(											\
					debug_macro_helper_ignore_always,						\
					::vostok::process_error_true,								\
					VOSTOK_MAKE_STRING(expression),							\
					assert_type,											\
					##__VA_ARGS__											\
				)															\
			}																\
			VOSTOK_ANALYSIS_ASSUME( !!(expression) );							\
		} else VOSTOK_ANALYSIS_ASSUME( !!(expression) )

#	define R_ASSERT_CMP_T( value1, operation, value2, assert_type, ... )	\
		if ( ::vostok::identity(true) ) {										\
			static bool debug_macro_helper_ignore_always = false;			\
			if ( !debug_macro_helper_ignore_always ) {						\
				bool debug_macro_helper_comparison_result;					\
				::vostok::debug::detail::string_helper fail_message;			\
				::vostok::debug::detail::make_fail_message(					\
					& fail_message,											\
					debug_macro_helper_comparison_result,					\
					::vostok::debug::detail::make_tuple( value1, value2 )		\
					operation												\
					::vostok::debug::detail::compare_helper()					\
				);															\
				if ( !debug_macro_helper_comparison_result )	{			\
					fail_message.appendf	(__VA_ARGS__);					\
					VOSTOK_ASSERT_HELPER(										\
						debug_macro_helper_ignore_always,					\
						::vostok::process_error_true,							\
						VOSTOK_MAKE_STRING(value1)							\
						" "													\
						VOSTOK_MAKE_STRING(operation)							\
						" "													\
						VOSTOK_MAKE_STRING(value2),							\
						assert_type,										\
						"%s",												\
						fail_message.c_str( )								\
					);														\
				}															\
			}																\
			VOSTOK_ANALYSIS_ASSUME( ((value1) operation (value2)) );			\
		}																	\
		else																\
			VOSTOK_ANALYSIS_ASSUME( ((value1) operation (value2)) )

#	define R_ASSERT_T_U			R_ASSERT_T
#	define R_ASSERT_CMP_T_U		R_ASSERT_CMP_T

#	define R_ASSERT_BOX( expression, ... )									\
		if ( ::vostok::identity(true) ) {										\
			static bool debug_macro_helper_ignore_always = false;			\
			if ( !debug_macro_helper_ignore_always && !::vostok::identity(expression) ) {		\
				VOSTOK_ASSERT_HELPER(											\
					debug_macro_helper_ignore_always,						\
					::vostok::process_error_to_message_box,					\
					VOSTOK_MAKE_STRING(expression),							\
					::vostok::assert_untyped,									\
					##__VA_ARGS__											\
				)															\
			}																\
			VOSTOK_ANALYSIS_ASSUME( !!(expression) );							\
		} else VOSTOK_ANALYSIS_ASSUME( !!(expression) )

#endif // #ifdef MASTER_GOLD

#define R_ASSERT( expression, ... )											R_ASSERT_T( ::vostok::assert_untyped, expression, ##__VA_ARGS__ )
#define R_ASSERT_CMP( value1, operation, value2, ... )						R_ASSERT_CMP_T( value1, operation, value2, ::vostok::assert_untyped, ##__VA_ARGS__ )

#define R_ASSERT_U( expression, ... )										R_ASSERT_T_U( ::vostok::assert_untyped, expression, ##__VA_ARGS__ )
#define R_ASSERT_CMP_U( value1, operation, value2, ... )					R_ASSERT_CMP_T_U( value1, operation, value2, ::vostok::assert_untyped, ##__VA_ARGS__ )

////////////////////////////////////////////////////////////////////////////
// ASSERT* macros
////////////////////////////////////////////////////////////////////////////
#ifdef DEBUG
#	define ASSERT_T															R_ASSERT_T
#	define ASSERT_CMP_T														R_ASSERT_CMP_T

#	define ASSERT_T_U														ASSERT_T
#	define ASSERT_CMP_T_U													ASSERT_CMP_T

#	define VOSTOK_UNREACHABLE_CODE( description, ... )						\
		if ( ::vostok::identity(true) ) {										\
			FATAL( description );											\
			__VA_ARGS__;													\
		}																	\
		else																\
			__VA_ARGS__

#else // #ifdef DEBUG
#	define ASSERT_T															VOSTOK_EMPTY_EXPRESSION_VA_ARGS
#	define ASSERT_CMP_T( value1, operation, value2, assert_type, ... )		VOSTOK_EMPTY_EXPRESSION

#	define ASSERT_T_U														VOSTOK_EMPTY_EXPRESSION_U_VA_ARGS
#	define ASSERT_CMP_T_U( value1, operation, value2, assert_type, ... )	if ( ::vostok::identity(false) ) { ::vostok::debug::detail::expression_eater(value1, value2, assert_type, ##__VA_ARGS__); } else (void)0
#	ifndef __SNC__
#		define VOSTOK_UNREACHABLE_CODE( ... )									__assume(0)
#	else // #ifndef __SNC__
#		define VOSTOK_UNREACHABLE_CODE( description, ... )					__VA_ARGS__
#	endif // #ifndef __SNC__
#endif // #ifdef DEBUG

#define UNREACHABLE_CODE( ... )												VOSTOK_UNREACHABLE_CODE( "unreachable code accessed!", ##__VA_ARGS__ )
#define NODEFAULT( ... )													VOSTOK_UNREACHABLE_CODE( "unintended default case accessed!", ##__VA_ARGS__ )
#define NOT_IMPLEMENTED( ... )												VOSTOK_UNREACHABLE_CODE( "functionality has not been implemented yet!", ##__VA_ARGS__ )

#define ASSERT( expression, ... )											ASSERT_T( ::vostok::assert_untyped, expression, ##__VA_ARGS__ )
#define ASSERT_CMP( value1, operation, value2, ... )						ASSERT_CMP_T( value1, operation, value2, ::vostok::assert_untyped, ##__VA_ARGS__ )

#define ASSERT_U( expression, ... )											ASSERT_T_U( ::vostok::assert_untyped, expression, ##__VA_ARGS__ )
#define ASSERT_CMP_U( value1, operation, value2, ... )						ASSERT_CMP_T_U( value1, operation, value2, ::vostok::assert_untyped, ##__VA_ARGS__ )

#define CHECK_AND_CORRECT( expression, action, ... )						\
	if ( ::vostok::identity(true) ) {											\
		if ( !::vostok::identity( expression ) ) {							\
			LOG_WARNING					( __VA_ARGS__ );					\
			action;															\
		}																	\
	} else (void)0
	
#define CHECK_OR_EXIT( expression, format, ... )							\
	if ( ::vostok::identity(true) ) {											\
		if ( !::vostok::identity( expression ) )								\
		::vostok::debug::terminate		( format, ##__VA_ARGS__ );			\
	} else (void)0

////////////////////////////////////////////////////////////////////////////
// CURE_ASSERT* macros
////////////////////////////////////////////////////////////////////////////
#ifdef MASTER_GOLD
#	define CURE_ASSERT( expression, action, ... )							\
		if ( ::vostok::identity(true) ) {										\
			static bool debug_macro_helper_ignore_always = false;			\
			if ( !debug_macro_helper_ignore_always && !::vostok::identity(expression) ) {	\
				static u32	occurances_left	=	u32(-1);					\
				if ( occurances_left == u32(-1) )							\
					occurances_left	=	assert_log_times_in_gold ();		\
				if ( occurances_left-- ) {									\
					VOSTOK_ASSERT_HELPER	(									\
						debug_macro_helper_ignore_always,					\
						::vostok::process_error_false,						\
						VOSTOK_MAKE_STRING(expression),						\
						::vostok::assert_untyped,								\
						##__VA_ARGS__										\
					);														\
				}															\
				action;														\
			}																\
		} else (void)0

#	define CURE_ASSERT_CMP( value1, operation, value2, action, ... )		\
		if ( ::vostok::identity(true) ) {										\
			static bool debug_macro_helper_ignore_always = false;			\
			if ( !debug_macro_helper_ignore_always ) {						\
				bool debug_macro_helper_comparison_result;					\
				::vostok::debug::detail::string_helper fail_message;			\
				::vostok::debug::detail::make_fail_message(					\
					& fail_message,											\
					debug_macro_helper_comparison_result,					\
					::vostok::debug::detail::make_tuple( value1, value2 )		\
					operation												\
					::vostok::debug::detail::compare_helper()					\
				);															\
				if ( !debug_macro_helper_comparison_result ) {				\
					fail_message.appendf(__VA_ARGS__);						\
					static u32 occurances_left	= u32(-1);					\
					if ( occurances_left == u32(-1) )						\
						occurances_left	=	assert_log_times_in_gold( );	\
					if ( occurances_left-- )								\
						VOSTOK_ASSERT_HELPER	(								\
							debug_macro_helper_ignore_always,				\
							::vostok::process_error_false,					\
							VOSTOK_MAKE_STRING(value1)						\
							" "												\
							VOSTOK_MAKE_STRING(operation)						\
							" "												\
							VOSTOK_MAKE_STRING(value2),						\
							::vostok::assert_untyped,							\
							"%s",											\
							fail_message.c_str( )							\
						);													\
				}															\
				action;														\
			}																\
		} else (void)0

#else // #ifdef MASTER_GOLD
#	define CURE_ASSERT( expression, action, ... )							\
		if ( ::vostok::identity(true) ) {										\
			static bool debug_macro_helper_ignore_always = false;			\
			if ( !debug_macro_helper_ignore_always && !::vostok::identity(expression) ) {	\
				VOSTOK_ASSERT_HELPER	(										\
					debug_macro_helper_ignore_always,						\
					::vostok::process_error_true,								\
					VOSTOK_MAKE_STRING(expression),							\
					::vostok::assert_untyped,									\
					##__VA_ARGS__											\
				);															\
				action;														\
			}																\
		} else (void)0

#	define CURE_ASSERT_CMP( value1, operation, value2, action, ... )		\
		if ( ::vostok::identity(true) ) {										\
			static bool debug_macro_helper_ignore_always = false;			\
			if ( !debug_macro_helper_ignore_always ) {						\
				bool debug_macro_helper_comparison_result;					\
				::vostok::debug::detail::string_helper fail_message;			\
				::vostok::debug::detail::make_fail_message(					\
					& fail_message,											\
					debug_macro_helper_comparison_result,					\
					::vostok::debug::detail::make_tuple( value1, value2 )		\
					operation												\
					::vostok::debug::detail::compare_helper()					\
				);															\
				if ( !debug_macro_helper_comparison_result ) {				\
					fail_message.appendf(__VA_ARGS__);						\
					VOSTOK_ASSERT_HELPER	(									\
						debug_macro_helper_ignore_always,					\
						::vostok::process_error_true,							\
						VOSTOK_MAKE_STRING((value1) operation (value2)),		\
						::vostok::assert_untyped,								\
						"%s",												\
						fail_message.c_str( )								\
					);														\
					action;													\
				}															\
			}																\
		} else (void)0

#endif // #ifdef MASTER_GOLD

#include <vostok/debug/macros_inline.h>

#endif // #ifndef VOSTOK_DEBUG_MACROS_H_INCLUDED