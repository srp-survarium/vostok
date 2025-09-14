////////////////////////////////////////////////////////////////////////////
//	Created 	: 26.08.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "fs_pack_application.h"

static vostok::uninitialized_reference<fs_pack::application>	s_application;

static int s_exit_code					= 0;

struct guard {
	inline	guard						( )
	{
		VOSTOK_CONSTRUCT_REFERENCE		( s_application, fs_pack::application );
	}

	inline	~guard						( )
	{
		s_exit_code						= s_application->get_exit_code( );
		VOSTOK_DESTROY_REFERENCE			( s_application );
	}
}; // struct guard

struct command_line_args
{
	u32			argc;
	pstr const*	argv;
};

static void	main_protected				( pvoid param )
{
	guard								guard;

	command_line_args* args				=	reinterpret_cast<command_line_args*>(param);

	s_application->initialize			( args->argc, args->argv );
	if ( s_application->get_exit_code( ) )
		return;
 
	s_application->execute				( );
	if ( s_application->get_exit_code( ) )
		return;

	s_application->finalize				( );
}

int   main								( int const argc, pstr const argv[ ], pstr const env[ ] )
{
	VOSTOK_UNREFERENCED_PARAMETER			( env );
	command_line_args args			=	{ argc, argv };
	vostok::debug::protected_call			( &main_protected, &args );
	return								( s_exit_code );
}