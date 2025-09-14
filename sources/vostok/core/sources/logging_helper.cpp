////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.08.2008
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "logging_path_parts.h"
#include <vostok/logging_settings.h>
#include "logging.h"
#include <vostok/logging_log_file.h>
#include <vostok/buffer_string.h>
#include <stdarg.h>						// for va_list
#include <vostok/fs_utils.h>

using vostok::logging::helper;
using vostok::logging::helper_data;
using vostok::logging::settings;
using vostok::logging::path_parts;
using vostok::logging::log_callback;
using vostok::logging::log_file;

static log_callback				s_log_callback	= 0;

vostok::core::log_file_usage	 get_log_file_usage ();

namespace vostok {
namespace logging {

void fill_local_time( vostok::buffer_string& dest );

void set_log_callback			(log_callback callback)
{
	ASSERT						(callback);
	s_log_callback				= callback;
}

log_callback get_log_callback	()
{
	return						( s_log_callback );
}

int get_tree_verbosity			( path_parts* const path );
void log_thread_unsafe			( logging::settings const *, pcstr string, u32 string_length, logging::verbosity verbosity );

} // namespace logging
} // namespace vostok

helper_data::helper_data		(
		pcstr const file,
		pcstr const function_signature,
		int const line,
		pcstr const initiator,
		vostok::logging::verbosity const verbosity
	) :
	m_file					( file ),
	m_function_signature	( function_signature ),
	m_line					( line ),
	m_verbosity				( verbosity ),
	m_initiator				( initiator )
{
}

helper::helper					(
		pcstr const file,
		pcstr const function_signature,
		int const line,
		pcstr const initiator,
		vostok::logging::verbosity const verbosity
	) :
	helper_data				(
		file,
		function_signature,
		line,
		initiator,
		verbosity
	)
{
}

struct predicate {
	pcstr						m_initiator;
	pcstr						m_file;
	pcstr						m_function_signature;
	int							m_line;
	vostok::logging::verbosity	m_verbosity;
	vostok::logging::settings const *	m_settings;

	inline		predicate		(
			pcstr const 					initiator,
			pcstr const 					file,
			pcstr const 					function_signature,
			int const						line,
			vostok::logging::verbosity const	verbosity,
			vostok::logging::settings	const *	settings
		) :
		m_initiator				( initiator ),
		m_file					( file ),
		m_function_signature	( function_signature ),
		m_line					( line ),
		m_verbosity				( verbosity ),
		m_settings				( settings )
	{
	}

	inline bool	operator ( )	( u32 const index, pcstr string, u32 const length, bool const is_last ) const
	{
		// the underline code is the same as above
		// this is optimization for the debug mode :)
		// because debug::output works too slow for us

		u32 const	initiator_length	=	vostok::strings::length ( m_initiator );
		char const	end_line[]			=	"\r\n";
		u32 const	end_line_length		=	vostok::array_size(end_line) - 1; // counts trailing zero, so need to -1
		u32 const	final_length		=	initiator_length + length + end_line_length + 1;

		vostok::buffer_string	final_string	((pstr)ALLOCA(final_length*sizeof(char)), final_length);

		final_string.append					(m_initiator, m_initiator+initiator_length);
		final_string.append					(string, string+length);

		vostok::logging::get_log_mutex().lock	( );
		vostok::debug::output					( final_string.c_str() );
		vostok::debug::output					( "\n" );
		final_string.append					(end_line, end_line+end_line_length);
		vostok::logging::log_thread_unsafe	(m_settings, final_string.c_str(), final_string.length(), m_verbosity);

		if ( s_log_callback ) {
			s_log_callback		(
				m_file,
				m_function_signature,
				m_line,
				m_initiator,
				m_verbosity, 
				string,
				!index ? vostok::logging::first : ( is_last ? vostok::logging::last : vostok::logging::callback_flag( 0 ) )
			);
		}

		vostok::logging::get_log_mutex().unlock	( );

		return								( true );
	}
}; // struct predicate

static void fill_initiator		( pstr pointer, u32 const buffer_size, path_parts& path, int const verbosity )
{
	vostok::buffer_string	dest  ( pointer, buffer_size );

	dest.assignf				("{%-8s", vostok::threading::current_thread_logging_name());

	vostok::logging::fill_local_time	( dest );

	dest					+= "  <";
	switch ( verbosity ) {
		case vostok::logging::error : {
			dest			+= "ERROR";
			break;
		}
		case vostok::logging::warning : {
			dest			+= "Warning";
			break;
		}
		case vostok::logging::info : {
			dest			+= "info";
			break;
		}
		case vostok::logging::debug : {
			dest			+= "debug";
			break;
		}
		case vostok::logging::trace : {
			dest			+= "trace";
			break;
		}
		case vostok::logging::silent :
		default :			NODEFAULT();
	} // switch ( m_verbosity )

	dest					+= ">[";

	pstr const replace_start	= dest.c_str() + dest.length();
	path.concat2buffer		( dest );
	pstr const replace_end	= dest.c_str() + dest.length();

	for ( pstr i = replace_start; i != replace_end; ++i )
	{
		if ( *i == '/' )
			*i				= ':';
	}

	dest					+= "]    ";
}

void   generate_log_file_name (vostok::fs::path_string * out_result, pcstr extension);

static void backdoor_process	(
		helper_data & helper,
		settings const * const settings, 
		pcstr const initiator, 
		pcstr const format, 
		va_list const args
	)
{
	using namespace vostok::fs;

	VOSTOK_UNREFERENCED_PARAMETERS(initiator, settings);
	VOSTOK_UNREFERENCED_PARAMETER(helper);
	static bool s_in_backdoor	=	false;
	if ( s_in_backdoor )
		return;

	if ( get_log_file_usage() == vostok::core::no_log || get_log_file_usage() == vostok::core::uninitialized_log )
		return;

	s_in_backdoor		=	true;
	vostok::fixed_string4096	message_buffer;
	message_buffer.appendf_va_list	(format, args);
	message_buffer		+=	"\n";
	vostok::debug::output		(message_buffer.c_str());
	
	path_string	log_file_name;
	generate_log_file_name (& log_file_name, "log");
	path_string	log_file_directory;
	directory_part_from_path	(& log_file_directory, log_file_name.c_str());

	make_dir_r	(log_file_directory.c_str());

	file_type * file	=	NULL;
	if ( open_file (& file, open_file_create | open_file_write | open_file_append, 
							  log_file_name.c_str(), false) )
	{
		if ( file )
		{
			write_file		(file, message_buffer.c_str(), message_buffer.length());
			close_file		(file);
		}
	}

	s_in_backdoor		=	false;
}

static void process				(
		helper_data& helper,
		settings const* const settings, 
		pcstr const initiator, 
		pcstr const format, 
		va_list const args
	)
{
	if ( !vostok::logging::ready_for_use( ) )
	{
		backdoor_process	(helper, settings, initiator, format, args);
		return;
	}

	vostok::logging::ready_for_use	( false );

	path_parts				path;

	ASSERT					( helper.m_initiator );
	if ( *helper.m_initiator != vostok::logging::initiator_separator )
		path.add_part		( helper.m_initiator );
	
	if ( settings ) {
		pcstr const header	= settings->get_initiator_prefix( );
		if ( header && header[0] != NULL )
			path.add_part	( header );
	}

	if ( initiator )
		path.add_part		( initiator );

	path.add_part			(0);

	bool const force_verbosity	=	settings && (settings->flags & vostok::logging::settings::flags_force_verbosity);

	if ( !force_verbosity && get_tree_verbosity( &path ) < helper.m_verbosity ) {
		vostok::logging::ready_for_use	( true );
		return;
	}

	string256				initiator_buffer = "";
	if ( !settings || !(settings->flags & vostok::logging::settings::flags_log_only_user_string) ) {
		fill_initiator		( initiator_buffer, sizeof( initiator_buffer ), path, helper.m_verbosity );
	}

	string4096				message_buffer;
	vostok::vsnprintf			( message_buffer, sizeof( message_buffer ) - 1, format, args );

	vostok::strings::iterate_items	(
		message_buffer,
		predicate (
			initiator_buffer,
			helper.m_file,
			helper.m_function_signature,
			helper.m_line,
			helper.m_verbosity,
			settings
		),
		'\n'
	);

	vostok::logging::ready_for_use	( true );
}

void helper::operator ( )		( pcstr const format, ... )
{
	va_list	 				mark;
	va_start 				( mark, format );
	process	 				( *this, 0, 0, format, mark );
	va_end	 				( mark );
}

void helper::operator ( )		( settings const& log_settings, pcstr const format, ... )
{
	va_list	 				mark;
	va_start 				( mark, format );
	process	 				( *this, &log_settings, 0, format, mark );
	va_end	 				( mark );
}
