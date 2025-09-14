////////////////////////////////////////////////////////////////////////////
//	Created		: 04.08.2010
//	Author		: Konstantin Slipchenko
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_TESTING_MACRO_H_INCLUDED
#define VOSTOK_TESTING_MACRO_H_INCLUDED

#if VOSTOK_IN_PCH
#	error	sorry, you cant include testing.h to precompiled header, because it's OS dependent and could bring \
			to pch a lot of unneccessary stuff
#endif // #if VOSTOK_IN_PCH

#include <vostok/os_preinclude.h>
#include <vostok/os_include.h>

#pragma warning(disable:4509)

namespace vostok {
namespace testing {

#ifdef _MSC_VER
#	define	TEST_THROWS(code, awaited_exception)											\
		ASSERT	(awaited_exception != assert_untyped);										\
		vostok::assert_enum previous_awaited_exception	=									\
			vostok::testing::detail::set_awaited_exception (awaited_exception);				\
		__try { code; }																		\
		__except(VOSTOK_EXCEPTION_EXECUTE_HANDLER) { (void)0; }								\
		vostok::testing::detail::check_awaited_exception	(previous_awaited_exception);

#	define TEST_ASSERT_T( assert_type, expression, ... ) \
		__try { R_ASSERT_T_U(assert_type, expression, ##__VA_ARGS__); }						\
		__except (vostok::debug::unhandled_exception_filter(GetExceptionCode(), GetExceptionInformation())) { (void)0; }

#	define	TEST_ASSERT_CMP_T( value1, operation, value2, assert_type, ... )				\
		__try { R_ASSERT_CMP_T_U(value1, operation, value2, assert_type, ##__VA_ARGS__); }	\
		__except (vostok::debug::unhandled_exception_filter(GetExceptionCode(), GetExceptionInformation())) { (void)0; }

#	define TEST_CURE_ASSERT(expression, action, ...)	\
		__try {											\
			if ( !(expression) ) {						\
				static bool ignore_always = false;		\
				VOSTOK_ASSERT_HELPER		(				\
					ignore_always,						\
					vostok::process_error_true,			\
					VOSTOK_MAKE_STRING(expression),		\
					vostok::assert_untyped,				\
					##__VA_ARGS__						\
				);										\
				action;									\
			}											\
		}												\
		__except (vostok::debug::unhandled_exception_filter(GetExceptionCode(), GetExceptionInformation())) { (void)0; }

#	define TEST_CURE_ASSERT_CMP(value1, operation, value2, action, ...)					\
		__try {																				\
			if ( !((value1) operation (value2)) ) {											\
				static bool ignore_always = false;											\
				fixed_string4096 fail_message = vostok::debug::detail::make_fail_message		\
												(value1, value2, ##__VA_ARGS__).c_str();	\
				VOSTOK_ASSERT_HELPER		(													\
					ignore_always,															\
					vostok::process_error_true,												\
					VOSTOK_MAKE_STRING((value1) operation (value2)),							\
					vostok::assert_untyped,													\
					"%s",																	\
					fail_message.c_str()													\
				);																			\
				action;																		\
			}																				\
		}																					\
		__except (vostok::debug::unhandled_exception_filter(GetExceptionCode(), GetExceptionInformation())) { (void)0; }
#else // #ifdef _MSC_VER
#	define TEST_THROWS(...)
#	define TEST_ASSERT_T		ASSERT_T
#	define TEST_ASSERT_CMP_T	ASSERT_CMP_T
#	define TEST_CURE_ASSERT		CURE_ASSERT
#	define TEST_CURE_ASSERT_CMP	CURE_ASSERT_CMP
#endif // #ifdef _MSC_VER

#define TEST_ASSERT(expression, ...) \
	TEST_ASSERT_T( vostok::assert_untyped, expression, ##__VA_ARGS__ )

#define TEST_ASSERT_CMP(value1, operation, value2, ...)	\
	TEST_ASSERT_CMP_T(value1, operation, value2, vostok::assert_untyped, ##__VA_ARGS__)

} // namespace testing
} // namespace vostok

#endif // #ifndef VOSTOK_TESTING_MACRO_H_INCLUDED