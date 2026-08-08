////////////////////////////////////////////////////////////////////////////
//	Created 	: 26.08.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "survarium_pc_application.h"

#pragma comment( lib, "delayimp.lib" )

namespace vostok { namespace debug { void set_support_email( pcstr support_email ); } }

static vostok::uninitialized_reference<survarium::application>	s_application;

static int s_exit_code					= 0;

struct guard {
	inline	guard						( )
	{
//		R_ASSERT						( vostok::memory::g_crt_allocations_are_enabled );
//		vostok::memory::g_crt_allocations_are_enabled	= false;
		VOSTOK_CONSTRUCT_REFERENCE		( s_application, survarium::application );
		vostok::debug::set_support_email	( "game_crash_reports@survarium.com" );
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
		// The shipped entry point reports presence-mutex rejection as failure.
		return							( 1 );

	vostok::debug::protected_call			( &main_protected, 0 );

	destroy_presence_mutex				( );
	return								( s_exit_code );
}
