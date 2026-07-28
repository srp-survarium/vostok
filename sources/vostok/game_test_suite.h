////////////////////////////////////////////////////////////////////////////
//	Created		: 04.08.2010
//	Author		: Konstantin Slipchenko
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////
#ifndef VOSTOK_GAME_TEST_SUITE_H_INCLUDED
#define VOSTOK_GAME_TEST_SUITE_H_INCLUDED

#include <vostok/testing.h>
#include <vostok/linkage_helper.h>



namespace vostok {

namespace animation
{
	struct world;
} // namespace animation

namespace rtp
{
	struct world;
}

class VOSTOK_CORE_API game_test_suite : public testing::suite_base<game_test_suite>
{

public:
	game_test_suite() {}

public:
	DEFINE_SUITE_HELPERS

}; // class game_test_suite

STATIC_SIZE_ASSERT(game_test_suite, 0x30);

} // namespace vostok

#endif // #ifndef VOSTOK_GAME_TEST_SUITE_H_INCLUDED