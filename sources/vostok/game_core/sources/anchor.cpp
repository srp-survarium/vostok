#include "pch.h"
#include "anchor.h"

namespace survarium
{

IncludeAll::IncludeAll()
{
	// /OPT:REF reachability dispatcher: every use_* the old monolithic
	// temp_include_all.cpp called is still called here, one module entry point
	// per engine module (anchor_game_core.cpp, anchor_network_core.cpp, ...).
	vostok::anchor_game_core( );
	vostok::anchor_network_core( );
	vostok::anchor_physics( );
	vostok::anchor_collision( );
	vostok::anchor_logging( );

	Callback1 cb1;
	Callback2 cb2;
}


Callback1::Callback1()
{
	m_complete = boost::bind(&Callback1::complete, this, _1);
	m_partial  = boost::bind(&Callback1::partial , this, _1, 10);
}



void Callback1::complete(const char *name)
{
	m_complete = boost::bind(&Callback1::complete, this, _1);
}

void Callback1::partial(const char *name, int value)
{
	m_partial  = boost::bind(&Callback1::partial , this, _1, 10);
}



Callback2::Callback2()
{
	m_complete = boost::bind(&Callback2::complete, this, _1, _2);
	m_partial  = boost::bind(&Callback2::partial , this, _1, _2, 10.f);
}

void Callback2::complete(const char *name, int value)
{
	m_complete = boost::bind(&Callback2::complete, this, _1, _2);
}

void Callback2::partial(const char *name, int value, float precision)
{
	m_partial  = boost::bind(&Callback2::partial , this, _1, _2, 10.f);
}


}
