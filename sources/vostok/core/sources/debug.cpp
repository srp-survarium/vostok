////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "debug.h"
#include "logging.h"
#include "debug_platform.h"
#include <stdarg.h>
#include "debug_bugtrap.h"
#include "testing_impl.h"
#include "build_extensions.h"
#include <vostok/fixed_string.h>
#include "testing_impl.h"
#include <vostok/core/engine.h>

using vostok::threading::mutex;
using vostok::logging::try_lock_log_mutex;
using vostok::logging::try_unlock_log_mutex;

static vostok::uninitialized_reference<mutex>	s_mutex;
static vostok::core::engine * s_core_engine	=	NULL;

#ifndef VOSTOK_STATIC_LIBRARIES
void boost::throw_exception			( std::exception const& exception )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( exception );
	FATAL							( "boost::throw_exception: %s", exception.what() );
}
#endif // #ifndef VOSTOK_STATIC_LIBRARIES

vostok::core::engine *   vostok::core::debug::get_core_engine ( )
{
	return							s_core_engine;
}

void vostok::core::debug::initialize	( core::engine* engine )
{
	s_core_engine				=	engine;
	VOSTOK_CONSTRUCT_REFERENCE		( s_mutex, mutex );
	on_thread_spawn					( );
}

void vostok::core::debug::postinitialize	( )
{
	bugtrap::initialize				( );
}

void vostok::core::debug::finalize		( )
{
	bugtrap::finalize				( );
	VOSTOK_DESTROY_REFERENCE			( s_mutex );
}

void vostok::debug::log_call_stack		( pcstr header )
{
	LOG_INFO						( "%s", header );
	dump_call_stack					( "debug", logging::info, 1, 0, 0 );
}

static void on_error					( vostok::process_error_enum process_error, bool* do_debug_break, pstr const message, u32 const message_size, bool* ignore_always )
{
	vostok::logging::flush				( );

	if ( vostok::testing::run_tests_command_line() || 
		 vostok::build::print_build_id_command_line() )
	{
		if ( vostok::debug::is_debugger_present() )
			DEBUG_BREAK					( );
		vostok::debug::terminate			( "" );
	}

	if ( process_error == vostok::process_error_true )
		vostok::core::debug::platform::on_error	(  do_debug_break, message, message_size, ignore_always, NULL, vostok::core::debug::platform::error_type_assert);
	else if ( process_error == vostok::process_error_to_message_box )
		vostok::core::debug::platform::on_error_message_box	(  do_debug_break, message, message_size, ignore_always, NULL, vostok::core::debug::platform::error_type_assert);
	else
		UNREACHABLE_CODE();
}

static void output						( vostok::buffer_string & message, pcstr const format, ... )
{
	va_list					mark;
	va_start				( mark, format );

	u32 const old_length	=	message.length();
	message.appendf_va_list	(format, mark);

	if ( !vostok::testing::is_testing() ) {
		vostok::logging::helper	( __FILE__, __FUNCSIG__, __LINE__, "debug", vostok::logging::error )
								( "%s", message.c_str() + old_length);
	}

	message					+= "\r\n";
}

static void process						( bool* do_debug_break, vostok::process_error_enum process_error, bool* ignore_always, vostok::assert_enum assert_type, pcstr reason, pcstr expression, pcstr description, pcstr file, pcstr function, u32 line )
{
	if ( s_mutex.initialized() )
		s_mutex->lock		( );

	if ( !vostok::testing::is_testing() )
		vostok::debug::dump_call_stack		( "debug", vostok::logging::error, 2, 0, 0 );

	vostok::fixed_string8192	message;
	output					( message, "" );
	output					( message, "Error occured : %s", reason );
	output					( message, "Expression    : %s", expression );

	if (description)
		output				( message, "Description   : %s", description );

	output					( message, "File          : %s", file );
	output					( message, "Line          : %d", line );
	output					( message, "Function      : %s", function );

	if ( vostok::testing::is_testing() ) 
	{
		vostok::testing::on_exception	(assert_type, message.c_str(), 0, true);
	}
	else {
		if ( process_error != vostok::process_error_false )
		{
			if ( process_error == vostok::process_error_to_message_box )
				message		=	description;

			on_error		( process_error, do_debug_break, message.c_str(), message.max_length(), ignore_always );
		}
	}
		
	if ( s_mutex.initialized() )
		s_mutex->unlock		( );
}

void vostok::debug::on_error		( bool* do_debug_break, vostok::process_error_enum process_error, bool* ignore_always, vostok::assert_enum assert_type, pcstr reason, pcstr expression, pcstr file, pcstr function, u32 line, pcstr format, ... )
{
	va_list					mark;
	va_start				( mark, format );

	string4096				description;
	vsnprintf				( description, sizeof(description), sizeof(description) - 1, format, mark );

	process					( do_debug_break, process_error, ignore_always, assert_type, reason, expression, description, file, function, line );
}

void vostok::debug::on_error		( bool* do_debug_break, vostok::process_error_enum process_error, bool* ignore_always, vostok::assert_enum assert_type, pcstr reason, pcstr expression, pcstr file, pcstr function, u32 line )
{
	process					( do_debug_break, process_error, ignore_always, assert_type, reason, expression, 0, file, function, line );
}

void vostok::debug::on_error		( bool* do_debug_break, vostok::process_error_enum process_error, bool* ignore_always, int error_code, pcstr expression, pcstr file, pcstr function, u32 line, pcstr format, ... )
{
	va_list					mark;
	va_start				( mark, format );

	string4096				description;
	vsnprintf				( description, sizeof(description), sizeof(description) - 1, format, mark );

	pcstr const graphics	= core::debug::platform::get_graphics_api_error_description( error_code );
	if ( graphics ) {
		process				( do_debug_break, process_error, ignore_always, vostok::assert_untyped, graphics, expression, description, file, function, line );
		return;
	}

	pstr const reason		= core::debug::platform::fill_format_message ( error_code );
	process					( do_debug_break, process_error, ignore_always, vostok::assert_untyped, reason, expression, description, file, function, line );
	core::debug::platform::free_format_message	( reason );
}

void vostok::debug::on_error		( bool* do_debug_break, vostok::process_error_enum process_error, bool* ignore_always, int error_code, pcstr expression, pcstr file, pcstr function, u32 line )
{
	pstr const reason		= core::debug::platform::fill_format_message ( error_code );
	process					( do_debug_break, process_error, ignore_always, vostok::assert_untyped, reason, expression, 0, file, function, line );
	core::debug::platform::free_format_message	( reason );
}

void vostok::debug::terminate			( pcstr format, ... )
{
	va_list					mark;
	va_start				( mark, format );

	string4096				message;
	vsnprintf				( message, sizeof(message), sizeof(message) - 1, format, mark );

	logging::flush			( );
	core::engine * const  engine	=	vostok::core::debug::get_core_engine();
	core::debug::platform::terminate	( message, (engine ? engine->get_exit_code() : 1) + 1 );
}

void vostok::debug::printf			( pcstr format, ... )
{
	va_list	 					mark;
	va_start 					( mark, format );

	string4096					message_buffer;
	vsnprintf					( message_buffer, sizeof( message_buffer ) - 1, format, mark );

	try_lock_log_mutex			( );
	debug::output				( message_buffer );
	debug::output				( "\n" );
	try_unlock_log_mutex		( );

	va_end	 					( mark );
}

#define _WIN32_WINNT	0x0500
#include <vostok/os_include.h>

void vostok::debug::output								( pcstr message )
{
	VOSTOK_UNREFERENCED_PARAMETER	( message );

	try_lock_log_mutex			( );
	OUTPUT_DEBUG_STRING			( message );
	try_unlock_log_mutex		( );
}
