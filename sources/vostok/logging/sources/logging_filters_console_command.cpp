////////////////////////////////////////////////////////////////////////////
//	Created 	: 24.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/logging/logging_filters_console_command.h>

#include "./filter_tree.h"

namespace vostok {
namespace logging {

// STATE[100%|DONE]
logging_filters_console_command::logging_filters_console_command(
	filter_tree&						filter_tree,
	pcstr const							name,
	bool								serializable,
	console_commands::command_type		command_type,
	console_commands::execution_filter	execution_filter
	):
	console_commands::console_command	( name, serializable, command_type, execution_filter ),
	m_filter_tree						( filter_tree )
{
	m_need_args = true; // <0x76d06d>|0x000|0x000:'32'
}

// STATE[91%|DONE]: core strings::copy<512> called out-of-line in target (x2), inlined in base.
void logging_filters_console_command::execute( pcstr args )
{

	string512			initiator, verbosity;
	pcstr s				= strings::get_token( args, initiator, ' ' );		// <0x76d090>|0x000|0x000:'38'

	if( s == NULL )															// <0x76d0b0>|0x020|0x020:'40'
	{
		if ( strings::equal( initiator, "" ) )								// <0x76d0b9>|0x029|0x009:'42'
		{
			on_invalid_syntax	( args );									// <0x76d0d0>|0x040|0x017:'44'
			return;															// <0x76d0df>|0x04f|0x00f:'45'
		}
		else																// <0x76d0e4>|0x054|0x005:'47'
		{
			strings::copy( verbosity, initiator );							// <0x76d0e6>|0x056|0x002:'49'
			strings::copy( initiator, "" ) ;								// <0x76d0f7>|0x067|0x011:'50'
		}
	}
	else																	// <0x76d107>|0x077|0x010:'53'
	{
		strings::get_token	( s, verbosity, ' ' );							// <0x76d109>|0x079|0x002:'55'
	}

	vostok::logging::verbosity t_verb = string_to_verbosity( verbosity );	// <0x76d126>|0x096|0x01d:'58'
	if( t_verb == vostok::logging::invalid )								// <0x76d13b>|0x0ab|0x015:'59'
	{
		on_invalid_syntax	( args );										// <0x76d144>|0x0b4|0x009:'61'
		return;																// <0x76d153>|0x0c3|0x00f:'62'
	}

	push_filter	( m_filter_tree, initiator, t_verb, u32(-1) );				// <0x76d155>|0x0c5|0x002:'65'

	// STRUCTURE DIFF: target 15 stmts / base 15 stmts
	// SIZE +0x5 | 43 | strings::copy( verbosity, initiator );
	// SIZE +0x5 | 44 | strings::copy( initiator, "" ) ;
	// VERDICT: STRUCTURE MATCH (shape ok) - core strings::copy<512> inline-vs-call at both rows; core-side, banked.
}

} // namespace logging
} // namespace vostok
