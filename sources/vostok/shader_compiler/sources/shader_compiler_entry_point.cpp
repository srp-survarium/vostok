////////////////////////////////////////////////////////////////////////////
//	Created		: 18.05.2010
//	Author		: Armen Abroyan
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "shader_compiler_application.h"

#pragma comment( lib, "delayimp.lib" )

static vostok::uninitialized_reference<vostok::shader_compiler::application>	s_application;
static int s_exit_code			= 0;

struct guard {
	inline	guard				( )
	{
		VOSTOK_CONSTRUCT_REFERENCE( s_application, vostok::shader_compiler::application );
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

	// Needed for debugger attaching
//	MessageBox					( 0, "", "", 0 );
	
	vostok::debug::protected_call	( &main_protected, 0 );
	TerminateProcess			( GetCurrentProcess(), s_exit_code );
//	return						( s_exit_code );
}