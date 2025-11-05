////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.08.2008
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/logging/logger.h>

#include "path_parts.h"
#include <stdarg.h>						// for va_list
#include <vostok/buffer_string.h>
#include <vostok/debug/log_callback.h>
#include <vostok/fs/device_utils.h>
#include <vostok/fs/native_path_string.h>
#include <vostok/logging/api.h>
#include <vostok/logging/extensions.h>
#include <vostok/logging/log_file.h>


using vostok::logging::path_parts;
using vostok::logging::verbosity;
using vostok::logging::log_format;
using vostok::logging::format_specifier;
// using vostok::logging::log_file;


#if 0
static log_callback				s_log_callback	= 0;

vostok::logging::log_file_usage	 get_log_file_usage ();
#endif

void fill_local_time( format_string_type& dest, bool brief )

static void fill_log_string(
	vostok::buffer_string&		dest,
	pstr const					message_start,
	pstr const					message_end,
	path_parts&					path,
	verbosity					verbosity,
	log_format const&			format
)
{
	using namespace vostok;
	using namespace vostok::logging;

	format_string_type	strings_storage		[format_specifier_count];
	buffer_string *		strings				[format_specifier_count];
	for ( int i=0; i<format_specifier_count; ++i ) {}
		// sushi@TODO: strings[i]						=	& strings_storage[i];

	u32 const message_length			=	message_end - message_start;
	buffer_string	message_string			((pstr)message_start, message_length + 1, message_length);
	char const saved_end_char			=	* message_end;
	* (pstr)message_end					=	NULL;

	strings[format_specifier_message]	=	& message_string;

	if ( format.enabled[format_specifier_initiator] )
	{
		// sushi@TODO: path.concat2buffer					(* strings[format_specifier_initiator]);

		for ( pstr	i	=	(* strings[format_specifier_initiator]).begin(),
					end	=	(* strings[format_specifier_initiator]).end();
					i != end; ++i )
		{
			if ( *i == '/' )
				*i						=	':';
		}
	}
	if ( format.enabled[format_specifier_thread_id] )
	{
		(* strings[format_specifier_thread_id]).assignf	("%-8s", vostok::threading::current_thread_logging_name());
	}
	if ( format.enabled[format_specifier_time] )
	{
		fill_local_time						(* strings[format_specifier_time]);
	}
	if ( format.enabled[format_specifier_verbosity] )
	{
		* strings[format_specifier_verbosity]	=	verbosity_to_string(verbosity);
	}

	COMPILE_ASSERT							(format_specifier_count == 8, OMG_FIX_BELOW_THEN);
	dest.assignf							(format.string, // sushi@TODO
 												(* strings[format.indexes[0]]).c_str(),
												(* strings[format.indexes[1]]).c_str(),
 												(* strings[format.indexes[2]]).c_str(),
												(* strings[format.indexes[3]]).c_str(),
 												(* strings[format.indexes[4]]).c_str(),
												(* strings[format.indexes[5]]).c_str());

	* (pstr)message_end					=	saved_end_char;
}

void vostok::logging::log_format::set( vostok::logging::format_specifier const & format_expression )
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
// STATE[STUB]
// void vostok::logging::logger::operator()(char const* const, char* const)
void vostok::logging::logger::operator()( pcstr format, char* args )
{
	// LOCALS
	// char[4096] 					message_buffer
	// path_parts 					path
	// debug_log_disable_raii 		debug_log_disable
	// ******

	// FUNCTION BODY
	// <1>
	// <2>
	// <0x5f033>|0x000|0x000:'175'
	// <1>
	// <2>
	// <0x5f038>|0x005|0x005:'178'
	// <1>
	// <0x5f057>|0x024|0x01f:'180'
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x5f06c>|0x039|0x015:'189'
	// ******
}

// STATE[STUB]
// bool vostok::logging::logger_predicate::operator()(const unsigned int, char*, const unsigned int, const bool) const
bool vostok::logging::logger_predicate::operator()(
	u32			index,
	char*		string,
	u32			length,
	bool		is_last
) const
{
	// LOCALS
	// u32 							final_length
	// buffer_string 				final_string
	// ******

	// CALL SITE INFO
	// <0x5f2c2> -> <unknown>
	// ******

	return false;
	// FUNCTION BODY
	// <0x5f0cf>|0x000|0x000:'209'
	// <1>
	// <0x5f0da>|0x00b|0x00b:'211'
	// <1>
	// <2>
	// <0x5f100>|0x031|0x026:'214'
	// <1>
	// <0x5f13a>|0x06b|0x03a:'216'
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <0x5f168>|0x099|0x02e:'227'
	// <1>
	// <2>
	// <0x5f2c7>|0x1f8|0x15f:'230'
	// ******
}

// STATE[STUB]
// void vostok::logging::append(boost::function<void __cdecl(void *,char const *,unsigned int,char const *,char const *,enum vostok::logging::verbosity,char const *,unsigned int,enum vostok::logging::callback_flag)> const&, void* const, vostok::logging::log_format const*, char const*, unsigned int, char const*, char const*, vostok::logging::verbosity, char const*, <NoType>)
void vostok::logging::append(
	boost::function<void(void *,pcstr,u32,pcstr,pcstr,enum verbosity,pcstr,u32,enum callback_flag)> const&	log_callback,
	void*								user_data,
	log_format const*					log_format,
	pcstr								file,
	u32									line,
	pcstr								function_signature,
	pcstr								initiator,
	verbosity							verbosity,
	pcstr								format
)
{
	// LOCALS
	// char* 						mark
	// ******

	// FUNCTION BODY
	// <1>
	// <0x76d54f>|0x000|0x000:'247'
	// <0x76d555>|0x006|0x006:'248'
	// <0x76d5e1>|0x092|0x08c:'249'
	// ******
}

// STATE[STUB]
// void vostok::logging::append(boost::function<void __cdecl(void *,char const *,unsigned int,char const *,char const *,enum vostok::logging::verbosity,char const *,unsigned int,enum vostok::logging::callback_flag)> const&, void* const, vostok::logging::format_specifier const&, char const*, unsigned int, char const*, char const*, vostok::logging::verbosity, char const*, <NoType>)
void vostok::logging::append(
	boost::function<void(void *,pcstr,u32,pcstr,pcstr,enum verbosity,pcstr,u32,enum callback_flag)> const&	log_callback,
	void*								user_data,
	format_specifier const&				format_specifier,
	pcstr								file,
	u32									line,
	pcstr								function_signature,
	pcstr								initiator,
	verbosity							verbosity,
	pcstr								format
)
{
	// LOCALS
	// char* 						mark
	// log_format 					log_format
	// ******

	// FUNCTION BODY
	// <0x76d45f>|0x000|0x000:'265'
	// <1>
	// <2>
	// <0x76d46e>|0x00f|0x00f:'268'
	// <0x76d477>|0x018|0x009:'269'
	// <0x76d527>|0x0c8|0x0b0:'270'
	// ******
}