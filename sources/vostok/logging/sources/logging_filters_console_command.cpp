////////////////////////////////////////////////////////////////////////////
//	Created 	: 24.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/logging/logging_filters_console_command.h>

#include "./filter_tree.h"

namespace vostok {
namespace logging {

logging_filters_console_command::logging_filters_console_command(
	filter_tree&						filter_tree,
	pcstr const							name,
	bool const							serializable,
	console_commands::command_type const	command_type,
	console_commands::execution_filter const	execution_filter
	):
	console_commands::console_command	( name, serializable, command_type, execution_filter ),
	m_filter_tree						( filter_tree )
{
	m_need_args = true;
}

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
}

} // namespace logging
} // namespace vostok
