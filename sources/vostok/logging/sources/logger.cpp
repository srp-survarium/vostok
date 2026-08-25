////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.08.2008
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/logging/logger.h>

#include "path_parts.h"
#include <vostok/buffer_string.h>
#include <vostok/debug/log_callback.h>
#include <vostok/fs/device_utils.h>
#include <vostok/fs/native_path_string.h>
#include <vostok/logging/api.h>
#include <vostok/logging/extensions.h>
#include <vostok/logging/log_file.h>

#include <stdarg.h>						// for va_list

// sushi@NOTE: This static is unused. They forgot to remove it.
namespace vostok {
namespace logging {
	static log_callback_boost s_log_callback;

	void fill_local_time( format_string_type& dest, bool brief ); // sushi@NOTE: logger_win_xbox360.cpp doesn't have any headers
} // namespace logging
} // namespace vostok

// Target inlines this slash fixup into fill_log_string; no standalone target name survives.
static inline void convert_slashes_to_colons( pstr const string )
{
	for ( pstr i = string; *i; ++i )
		if ( *i == '/' )
			*i							=	':';
}

static void fill_log_string(
	vostok::buffer_string&				dest,
	pstr const							message_start,
	pstr const							message_end,
	vostok::logging::path_parts&		path,
	vostok::logging::verbosity const	verbosity,
	vostok::logging::log_format const&	format
)
{
	using namespace vostok;
	using namespace vostok::logging;

	char const saved_end_char			=	* message_end;
	* (pstr)message_end					=	NULL;

 	format_string_type strings_storage[format_specifier_count];
	pcstr	strings[format_specifier_count];

	if ( format.enabled[format_specifier_initiator] )
	{
		path.concat2buffer( strings_storage[format_specifier_initiator] );
		convert_slashes_to_colons( strings_storage[format_specifier_initiator] );
	}

	if ( format.enabled[format_specifier_thread_id] )
	{
		vostok::sprintf( strings_storage[format_specifier_thread_id], "%-8s", vostok::threading::current_thread_logging_name( ) );
	}

	if ( format.enabled[format_specifier_time_brief] )
	{
		fill_local_time( strings_storage[format_specifier_time_brief], true );
	}
	if ( format.enabled[format_specifier_time] )
	{
		fill_local_time( strings_storage[format_specifier_time], false );
	}
	if ( format.enabled[format_specifier_verbosity] )
	{
		strings::copy( strings_storage[format_specifier_verbosity], verbosity_to_string( verbosity ) );
	}
	COMPILE_ASSERT( format_specifier_count == 8, OMG_FIX_BELOW_THEN );
	for ( int i = 0 ; i < format_specifier_count ; ++i )
		strings[i] = strings_storage[i];

	strings[format_specifier_message] = message_start;
	dest.assignf(
		format.string,
		strings[format.indexes[0]],
		strings[format.indexes[1]],
		strings[format.indexes[2]],
		strings[format.indexes[3]],
		strings[format.indexes[4]],
		strings[format.indexes[5]]
	);

	* (pstr)message_end					=	saved_end_char;
}

namespace vostok {
namespace logging {

void log_format::set( format_specifier const & format_expression )
{
	format_specifier_list					specifiers;
	format_expression.fill_specifier_list	(specifiers, & string);
	for ( u32 i=0; i<format_specifier_count; ++i )
	{
		indexes[ i ]					=	0;
		enabled[ i ]					=	false;
	}

	for ( u32 i=0; i<specifiers.size(); ++i )
	{
		indexes[ i ]					=	(int)specifiers[i];
		enabled[ specifiers[i] ]		=	true;
	}
}

struct logger_predicate : private noncopyable {
public:
	inline			logger_predicate	( path_parts& path, logger const& helper ): m_path( path ), m_helper( helper ) { }

			bool	operator()			(
						u32 const	index,
						pstr		string,
						u32 const	length,
						bool const	is_last
					) const;

public:
	/* 0x0000 */	/* noncopyable */
	/* 0x0000 */	path_parts&			m_path;
	/* 0x0004 */	logger const&		m_helper;
}; // struct logger_predicate

STATIC_SIZE_ASSERT(logger_predicate, 0x8);

struct debug_log_disable_raii
{
	debug_log_disable_raii				()	{	debug::disable_log_callback	();	}
	~debug_log_disable_raii				()	{	debug::enable_log_callback	();	}
};


void logger::operator()( pcstr const format, pstr const args )
{
	debug_log_disable_raii debug_log_disable;

	string4096 message_buffer;
	vsnprintf( message_buffer, sizeof( message_buffer ) - 1, format, args );

	path_parts path( m_initiator );

	strings::iterate_items(
		message_buffer,
		logger_predicate( path, *this ),
		'\n'
	);
}

bool logger_predicate::operator()(
	u32 const	index,
	pstr		string,
	u32 const	length,
	bool const	is_last
) const
{
	u32 final_length = length + 128 + 1;																			// sushi@NOTE: Why 0x81? '\0'

	buffer_string final_string( ( pstr )ALLOCA( final_length ), final_length );


	fill_log_string( final_string, string, &string[length], m_path, m_helper.m_verbosity, m_helper.m_log_format );

	if ( m_helper.m_log_callback )
		m_helper.m_log_callback(
			m_helper.m_user_data,
			m_helper.m_file,
			m_helper.m_line,
			m_helper.m_function_signature,
			m_helper.m_initiator,
			m_helper.m_verbosity,
			final_string.c_str( ),
			final_string.length( ),
			index == 0 ? first : ( is_last ? last: (callback_flag)0 )
		);

	return true;
}

void append(
	log_callback_boost const&	log_callback,
	void* const					user_data,
	log_format const*			log_format,
	pcstr const					file,
	u32	const					line,
	pcstr const					function_signature,
	pcstr const					initiator,
	verbosity					verbosity,
	pcstr const					format,
	...
)
{
	va_list mark;
	va_start( mark, format );
	logger( log_callback, user_data, log_format, initiator, line, file, function_signature, verbosity )( format, mark );
	va_end( mark );
}

void append(
	log_callback_boost const&	log_callback,
	void* const					user_data,
	format_specifier const&		format_specifier,
	pcstr const					file,
	u32	const					line,
	pcstr const					function_signature,
	pcstr const					initiator,
	verbosity					verbosity,
	pcstr const					format,
	...
)
{
	log_format log_format( format_specifier );

	va_list mark;
	va_start( mark, format );
	logger( log_callback, user_data, &log_format, initiator, line, file, function_signature, verbosity )( format, mark );
	va_end( mark );
}

} // namespace logging
} // namespace vostok
