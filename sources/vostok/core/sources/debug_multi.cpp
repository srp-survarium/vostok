////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "debug_platform.h"
#include "debug.h"

#pragma message( VOSTOK_TODO("PS3: core dump generation") )
#pragma message( VOSTOK_TODO("PS3: save debug data") )

bool vostok::core::debug::platform::error_after_dialog	( )
{
	return						( false );
}

void vostok::debug::dump_call_stack					( pcstr						initiator, 
													  logging::verbosity const	verbosity, 
													  u32 						num_first_to_ignore, 
													  u32 						num_last_to_ignore, 
													  _EXCEPTION_POINTERS*		pointers, 
													  void*						callstack[] )
{
	VOSTOK_UNREFERENCED_PARAMETERS( initiator, verbosity, num_first_to_ignore, num_last_to_ignore, pointers, callstack );
}

void vostok::core::debug::platform::on_error				( bool* do_debug_break, char* const message, u32 message_size, bool* ignore_always, _EXCEPTION_POINTERS* const exception_information, error_type_enum error_type )
{
	VOSTOK_UNREFERENCED_PARAMETERS( do_debug_break, ignore_always, exception_information, error_type, message_size );
	vostok::core::debug::on_error	( message );
}

void vostok::core::debug::platform::on_error_message_box	( bool* do_debug_break, char* const message, u32 message_size, bool* ignore_always, _EXCEPTION_POINTERS* const exception_information, error_type_enum error_type )
{
	VOSTOK_UNREFERENCED_PARAMETERS( do_debug_break, ignore_always, exception_information, error_type, message_size );
	vostok::core::debug::on_error	( message );
}

pstr vostok::core::debug::platform::fill_format_message	( int const error_code )
{
	VOSTOK_UNREFERENCED_PARAMETER	( error_code );
	return						( "" );
}

void vostok::core::debug::platform::free_format_message	( pstr const buffer )
{
	VOSTOK_UNREFERENCED_PARAMETER	( buffer );
}

void vostok::core::debug::platform::terminate				( pcstr message, int const error_code )
{
	VOSTOK_UNREFERENCED_PARAMETER	( message );
	::exit						( error_code );
}