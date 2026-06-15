////////////////////////////////////////////////////////////////////////////
//	Created 	: 26.08.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "survarium_pc_application.h"

#pragma comment( lib, "delayimp.lib" )

static vostok::uninitialized_reference<survarium::application>	s_application;

static int s_exit_code					= 0;

struct guard {
	inline	guard						( )
	{
//		R_ASSERT						( vostok::memory::g_crt_allocations_are_enabled );
//		vostok::memory::g_crt_allocations_are_enabled	= false;
		// claude@NOTE: QUANTITY divergence (target 2 stmts / base 1). Target's first stmt is
		// vostok::debug::set_support_email( "game_crash_reports@survarium.com" ); we can't add
		// it - the symbol exists in the target exe but is NOT implemented/linked in our build
		// (0 occurrences in base index), so calling it is an unresolved external. Recover once
		// vostok::debug::set_support_email is provided by the debug module. Do not fabricate it.
		VOSTOK_CONSTRUCT_REFERENCE		( s_application, survarium::application );
		s_application->initialize		( );
	}

	inline	~guard						( )
	{
		s_exit_code						= s_application->get_exit_code();
		s_application->finalize			( );
		VOSTOK_DESTROY_REFERENCE			( s_application );
//		R_ASSERT						( !vostok::memory::g_crt_allocations_are_enabled );
//		vostok::memory::g_crt_allocations_are_enabled	= true;
	}
}; // struct guard

static void	main_protected				( pvoid )
{
	guard								guard;

	if ( !s_application->get_exit_code() )
		s_application->execute			( );
}

struct HINSTANCE__;
typedef HINSTANCE__* HINSTANCE;

bool	check_presence_mutex			( );
void	destroy_presence_mutex			( );

int __stdcall WinMain					(
		HINSTANCE hInstance,
		HINSTANCE hPrevInstance,
		pstr	  lpCmdLine,
		int       nCmdShow
	)
{
	VOSTOK_UNREFERENCED_PARAMETERS		( hInstance, hPrevInstance, nCmdShow, lpCmdLine );

	if ( !check_presence_mutex( ) )
		return							( 0 );

	vostok::debug::protected_call			( &main_protected, 0 );

	destroy_presence_mutex				( );
	return								( s_exit_code );
}