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
// claude@NOTE: base DOES emit its dynamic init/atexit dtor (??__Es_log_callback / ??__Fs_log_callback,
// byte-equivalent modulo LTCG conv); the report shows them unmatched only because objdiff never
// demangles ??__E/??__F base symbols to pair with the target's pretty names - tooling, see README.
static vostok::logging::log_callback_boost	s_log_callback;

namespace vostok {
namespace logging {
	void fill_local_time( format_string_type& dest, bool brief ); // sushi@NOTE: logger_win_xbox360.cpp doesn't have any headers
} // namespace logging
} // namespace vostok

// claude@MATCH: the '/'->':' fixup in fill_log_string is ONE 0x43-byte record on ONE line ('81',
// next stmt '83', the if-block's '}' line gets no bytes) and the target PDB records NO char* loop
// local for it (its only 'i' is the s32 of the '101' loop - the PDB does keep same-named block
// locals, see log_format::set's two u32 i) - so the loop reached fill_log_string INLINED from a
// one-line helper call (VC8 attributes inlined bytes to the call-site line and drops inlinee
// locals), confirming sushi's review hypothesis (PR #286).
// sushi@TODO: helper name/home unknowable in v0.100b (fully inlined, no symbol survives); a different
// Survarium build might emit it out-of-line and reveal the real name - check via vostok-versions.
static inline void convert_slashes_to_colons( pstr const string )
{
	for ( pstr i = string; *i; ++i )
		if ( *i == '/' )
			*i							=	':';
}

// STATE[97.5%|DONE]: core strings::copy<512> inline-vs-call. sushi@TODO: Ghidra script does not handle static functions
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

	if ( format.enabled[format_specifier_initiator] )																				// <0x76d28f>|0x011|0x006:'78'
	{
		path.concat2buffer( strings_storage[format_specifier_initiator] );															// <0x76d29d>|0x01f|0x00e:'80'
		convert_slashes_to_colons( strings_storage[format_specifier_initiator] );													// <0x76d2ac>|0x02e|0x00f:'81'
	}

	if ( format.enabled[format_specifier_thread_id] )																				// <0x76d2ef>|0x071|0x043:'83'
	{
		vostok::sprintf( strings_storage[format_specifier_thread_id], "%-8s", vostok::threading::current_thread_logging_name( ) );	// <0x76d2fd>|0x07f|0x00e:'85'
	}

	if ( format.enabled[format_specifier_time_brief] )																				// <0x76d316>|0x098|0x019:'87'
	{
		fill_local_time( strings_storage[format_specifier_time_brief], true );														// <0x76d324>|0x0a6|0x00e:'89'
	}
	if ( format.enabled[format_specifier_time] )																					// <0x76d335>|0x0b7|0x011:'91'
	{
		fill_local_time( strings_storage[format_specifier_time], false );															// <0x76d343>|0x0c5|0x00e:'93'
	}
	if ( format.enabled[format_specifier_verbosity] )																				// <0x76d354>|0x0d6|0x011:'95'
	{
		strings::copy( strings_storage[format_specifier_verbosity], verbosity_to_string( verbosity ) );								// <0x76d362>|0x0e4|0x00e:'97'
	}
	COMPILE_ASSERT( format_specifier_count == 8, OMG_FIX_BELOW_THEN );
	for ( int i = 0 ; i < format_specifier_count ; ++i )																			// <0x76d379>|0x0fb|0x017|[1]:'101'
		strings[i] = strings_storage[i];																							// <0x76d39d>|0x11f|0x024:'102'

	strings[format_specifier_message] = message_start;																				// <0x76d3bc>|0x13e|0x01f:'104'
	dest.assignf(
		format.string,
		strings[format.indexes[0]],
		strings[format.indexes[1]],
		strings[format.indexes[2]],
		strings[format.indexes[3]],
		strings[format.indexes[4]],
		strings[format.indexes[5]]
	);																																// <0x76d3c5>|0x147|0x009:'113'

	* (pstr)message_end					=	saved_end_char;																			// <0x76d43b>|0x1bd|0x076:'115'

	// STRUCTURE DIFF: target 18 stmts / base 18 stmts
	// SIZE +0x11 | 71 | strings::copy( strings_storage[format_specifier_verbosity], verbosity_to_string( verbosity ) );
	// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is core strings::copy<512> called out-of-line in target, inlined in base (core-side, flagged).
}

namespace vostok {
namespace logging {

// STATE[100%|DONE]
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
	} // <0x76d23c>|0x0b2|0x020:'132'
}

struct logger_predicate : public noncopyable {
public:
	inline			logger_predicate	( path_parts& path, logger const& helper ): m_path( path ), m_helper( helper ) { }

			bool	operator()			(
						u32			index,
						pstr		string,
						u32			length,
						bool		is_last
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


// STATE[80.6%|DONE]: core vostok::vsnprintf inline-vs-call + LTCG-materialized empty dtor call after iterate_items
void logger::operator()( pcstr const format, pstr const args )
{
	debug_log_disable_raii debug_log_disable;									// <0x5f033>|0x000|0x000:'175'

	string4096 message_buffer;
	vsnprintf( message_buffer, sizeof( message_buffer ) - 1, format, args );	// <0x5f038>|0x005|0x005:'178'

	path_parts path( m_initiator );												// <0x5f057>|0x024|0x01f:'180'

	strings::iterate_items(
		message_buffer,
		logger_predicate( path, *this ),
		'\n'
	);																			// <0x5f06c>|0x039|0x015:'189'

	// STRUCTURE DIFF: target 4 stmts / base 4 stmts
	// SIZE +0x2 | 151 | vsnprintf( message_buffer, sizeof( message_buffer ) - 1, format, args );
	// SIZE -0x5 | 159 | );
	// VERDICT: STRUCTURE MATCH (shape ok) - target calls core vostok::vsnprintf out-of-line (base inlines to __vsnprintf_s)
	// and calls an ICF-folded empty dtor for the logger_predicate temp (no symbol for it in the target unit, so an explicit
	// out-of-line ~logger_predicate would fabricate one - tried, reverted); both core/LTCG, banked.
}

// STATE[99.5%|DONE]: LTCG for `buffer_string`
bool logger_predicate::operator()(
	u32			index,
	pstr		string,
	u32			length,
	bool		is_last
) const
{
	u32 final_length = length + 128 + 1;																			// <0x5f0cf>|0x000|0x000:'209' // sushi@NOTE: Why 0x81? '\0'

	buffer_string final_string( ( pstr )ALLOCA( final_length ), final_length );										// <0x5f0da>|0x00b|0x00b:'211'


	fill_log_string( final_string, string, &string[length], m_path, m_helper.m_verbosity, m_helper.m_log_format );	// <0x5f100>|0x031|0x026:'214'

	if ( m_helper.m_log_callback )																					// <0x5f13a>|0x06b|0x03a:'216'
		m_helper.m_log_callback(
			m_helper.m_user_data,
			m_helper.m_file,
			m_helper.m_line,
			m_helper.m_function_signature,
			m_helper.m_initiator,
			m_helper.m_verbosity,
			final_string.c_str( ),
			final_string.length( ),
			index == 0 ? first : ( is_last ? last: (callback_flag)0 )												// <0x5f168>|0x099|0x02e:'227'
		);

	return true;																									// <0x5f2c7>|0x1f8|0x15f:'230'

	// STRUCTURE DIFF: target 6 stmts / base 6 stmts
	// SIZE +0x1 | 168 | buffer_string final_string( ( pstr )ALLOCA( final_length ), final_length );
	// VERDICT: STRUCTURE MATCH (shape ok) - 1 byte inside the buffer_string ctor call (LTCG conv), banked.
}

// STATE[93%|DONE]: target calls one more empty ctor while expanding logger's ctor (log_format()/noncopyable(), the format.h empty_stub COMDAT) - LTCG, see README
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
	va_start( mark, format );																								// <0x76d54f>|0x000|0x000:'247'
	logger( log_callback, user_data, log_format, initiator, line, file, function_signature, verbosity )( format, mark );	// <0x76d555>|0x006|0x006:'248'
	va_end( mark );																											// <0x76d5e1>|0x092|0x08c:'249'

	// STRUCTURE DIFF: target 3 stmts / base 3 stmts
	// SIZE -0xb | 205 | logger( log_callback, user_data, log_format, initiator, line, file, function_signature, verbosity )( format, mark );
	// VERDICT: STRUCTURE MATCH (shape ok) - target's inlined logger ctor calls TWO empty subobject ctors (ecx- and eax-conv),
	// base only one; the second is the un-expanded log_format(){}/noncopyable() COMDAT (format.h empty_stub) - LTCG, banked.
}

// STATE[93%|DONE]: same -0xb as the other append (one more empty subobject-ctor call in target) - LTCG, see README
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
	log_format log_format( format_specifier );																				// <0x76d45f>|0x000|0x000:'265'

	va_list mark;
	va_start( mark, format );																								// <0x76d46e>|0x00f|0x00f:'268'
	logger( log_callback, user_data, &log_format, initiator, line, file, function_signature, verbosity )( format, mark );	// <0x76d477>|0x018|0x009:'269'
	va_end( mark );																											// <0x76d527>|0x0c8|0x0b0:'270'

	// STRUCTURE DIFF: target 4 stmts / base 4 stmts
	// SIZE -0xb | 227 | logger( log_callback, user_data, &log_format, initiator, line, file, function_signature, verbosity )( format, mark );
	// VERDICT: STRUCTURE MATCH (shape ok) - same missing empty subobject-ctor call as the log_format-ptr append; LTCG, banked.
}

} // namespace logging
} // namespace vostok