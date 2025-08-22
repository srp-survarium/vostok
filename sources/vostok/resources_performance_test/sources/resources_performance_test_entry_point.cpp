////////////////////////////////////////////////////////////////////////////
//	Created		: 07.11.2011
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "resources_performance_test_application.h"

#pragma comment( lib, "delayimp.lib" )

static vostok::uninitialized_reference<vostok::resources_performance_test::application>	s_application;

static int s_exit_code			= 0;

struct guard {
	inline	guard				( )
	{
		VOSTOK_CONSTRUCT_REFERENCE( s_application, vostok::resources_performance_test::application );
		s_application->initialize ( );
	}

	inline	~guard				( )
	{
		s_exit_code				= s_application->get_exit_code();
		s_application->finalize	( );
		VOSTOK_DESTROY_REFERENCE	( s_application );
	}
}; // struct guard

static void	main_protected		( pvoid )
{
	guard						guard;

	if ( s_application->get_exit_code() )
		return;

	s_application->execute		( );
	if ( s_application->get_exit_code() )
		return;

}

int main						( int const argc, pstr const argv[ ], pstr const env[ ] )
{
	VOSTOK_UNREFERENCED_PARAMETERS( argc, argv, env );

	vostok::debug::protected_call	( &main_protected, 0 );
	return						( s_exit_code );
}