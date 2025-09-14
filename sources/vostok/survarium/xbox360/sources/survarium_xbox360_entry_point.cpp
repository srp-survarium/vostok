////////////////////////////////////////////////////////////////////////////
//	Created 	: 26.08.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "survarium_xbox360_application.h"

static vostok::uninitialized_reference<survarium::application>	s_application;

static int s_exit_code					= 0;

struct guard {
	inline	guard						( )
	{
		VOSTOK_CONSTRUCT_REFERENCE		( s_application, survarium::application );
	}

	inline	~guard						( )
	{
		s_exit_code						= s_application->get_exit_code();
		VOSTOK_DESTROY_REFERENCE			( s_application );
	}
}; // struct guard

static void	main_protected				( pvoid )
{
	guard								guard;

	s_application->initialize			( );
	if ( s_application->get_exit_code() )
		return;

	s_application->execute				( );
	if ( s_application->get_exit_code() )
		return;

	s_application->finalize				( );
}

int main								( int const argc, pstr const argv[ ], pstr const env[ ] )
{
	VOSTOK_UNREFERENCED_PARAMETERS		( argc, argv, env );
	vostok::debug::protected_call			( &main_protected, 0 );
	return								( s_exit_code );
}