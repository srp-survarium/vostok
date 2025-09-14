////////////////////////////////////////////////////////////////////////////
//	Created		: 03.08.2010
//	Author		: Konstantin Slipchenko
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_ENGINE_TEST_SUITE_H_INCLUDED
#define VOSTOK_ENGINE_TEST_SUITE_H_INCLUDED

#include <vostok/testing.h>
#include <vostok/linkage_helper.h>
#include <vostok/engine/api.h>

namespace vostok {

class VOSTOK_CORE_API engine_test_suite : public testing::suite_base<engine_test_suite>
{
public:
	DEFINE_SUITE_HELPERS

}; // class engine_test_suite

} // namespace vostok

#endif // #ifndef VOSTOK_ENGINE_TEST_SUITE_H_INCLUDED