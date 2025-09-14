////////////////////////////////////////////////////////////////////////////
//	Created		: 22.06.2009
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_CORE_TEST_SUITE_H_INCLUDED
#define VOSTOK_CORE_TEST_SUITE_H_INCLUDED

#include <vostok/testing.h>
#include <vostok/linkage_helper.h>
#include <vostok/fs/native_path_string.h>

VOSTOK_INCLUDE_TO_LINKAGE(resources_test);
VOSTOK_INCLUDE_TO_LINKAGE(compressor_test);
VOSTOK_INCLUDE_TO_LINKAGE(threading_test);
VOSTOK_INCLUDE_TO_LINKAGE(math_tests);
VOSTOK_INCLUDE_TO_LINKAGE(game_resources_manager_test);
VOSTOK_INCLUDE_TO_LINKAGE(resources_fs_task_erase);

namespace vostok {

class VOSTOK_CORE_API core_test_suite : public testing::suite_base<core_test_suite>
{
public:
	DEFINE_SUITE_HELPERS

public:
	void	set_resources_path	(pcstr resources_path)	{ m_resources_path.assign_with_conversion(resources_path); }
	pcstr	get_resources_path	() const				{ return m_resources_path.c_str(); }

private:
	fs_new::native_path_string		m_resources_path;
}; // class core_test_suite

} // namespace vostok

#endif // #ifndef VOSTOK_CORE_TEST_SUITE_H_INCLUDED