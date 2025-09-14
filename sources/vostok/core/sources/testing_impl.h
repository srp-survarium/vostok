////////////////////////////////////////////////////////////////////////////
//	Created		: 15.06.2009
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef TESTING_IMPL_H_INCLUDED
#define TESTING_IMPL_H_INCLUDED

namespace vostok {
namespace testing {

extern command_line::key	g_run_tests;
extern command_line::key	g_run_tests_and_exit;

VOSTOK_CORE_API bool	run_tests_command_line	();
VOSTOK_CORE_API bool	is_testing				();
VOSTOK_CORE_API void	on_exception			(assert_enum			assert_type, 
											 pcstr					description, 
											 _EXCEPTION_POINTERS*	exception_information,
											 bool					is_assertion);

} // namespace testing
} // namespace vostok

#endif // #ifndef TESTING_IMPL_H_INCLUDED