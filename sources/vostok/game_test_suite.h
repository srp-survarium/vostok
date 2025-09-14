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
	game_test_suite(): m_rtp_world( 0 ) {}

	void			set_rtp_world	( rtp::world * rtp_world )	{  m_rtp_world = rtp_world; }

	
	rtp::world *	rtp_world		( )const					{ return m_rtp_world; }
	//rtp::world *	rtp_world		( )							{ return m_rtp_world; }

public:
	DEFINE_SUITE_HELPERS

private:
	rtp::world *m_rtp_world;

}; // class game_test_suite

} // namespace vostok

#endif // #ifndef VOSTOK_GAME_TEST_SUITE_H_INCLUDED