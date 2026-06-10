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
	m_need_args = true;
}

// STATE[91%|DONE]: core strings::copy<512> called out-of-line in target (x2), inlined in base.
void logging_filters_console_command::execute( pcstr args )
{

	string512			initiator, verbosity;
	pcstr s				= strings::get_token( args, initiator, ' ' );

	if( s == NULL )
	{
		if ( strings::equal( initiator, "" ) )
		{
			on_invalid_syntax	( args );
			return;
		}
		else
		{
			strings::copy( verbosity, initiator );
			strings::copy( initiator, "" ) ;
		}
	}
	else
	{
		strings::get_token	( s, verbosity, ' ' );
	}

	vostok::logging::verbosity t_verb = string_to_verbosity( verbosity );
	if( t_verb == vostok::logging::invalid )
	{
		on_invalid_syntax	( args );
		return;
	}

	push_filter	( m_filter_tree, initiator, t_verb, u32(-1) );

	// STRUCTURE DIFF: target 15 stmts / base 15 stmts
	// SIZE +0x5 | 43 | strings::copy( verbosity, initiator );
	// SIZE +0x5 | 44 | strings::copy( initiator, "" ) ;
	// VERDICT: STRUCTURE MATCH (shape ok) - core strings::copy<512> inline-vs-call at both rows; core-side, banked.
}

} // namespace logging
} // namespace vostok
