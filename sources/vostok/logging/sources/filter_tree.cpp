////////////////////////////////////////////////////////////////////////////
//	Created		: 30.08.2011
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "filter_tree.h"

#include "filter_tree_node.h"
#include <vostok/console_command.h>
#include <vostok/intrusive_double_linked_list.h>
#include <vostok/logging/api.h>
#include <vostok/logging/logging_filters_console_command.h>
#include <vostok/memory_writer.h>
#include <vostok/threading_reader_writer_lock.h>

namespace vostok {
namespace logging {

// STATE[91%|DONE]: LTCG for `malloc_impl`.
filter_tree::filter_tree( memory::base_allocator& allocator ) :
	lock			( ),
	initiator_tree	( VOSTOK_NEW_IMPL( allocator, node )( "", trace ) ),
	allocator		( allocator ),
	filter_stack	( )
{
}

// STATE[100%|DONE]
filter_tree::~filter_tree( )
{
	initiator_tree->clean( &allocator );				// <0x65c399>|0x000|0x000:'26'
	VOSTOK_DELETE_IMPL( allocator, initiator_tree );	// <0x65c3ab>|0x012|0x012:'27'
}


// STATE[100%|DONE]
bool filter_tree::has_passed_filters( pcstr initiator, verbosity verbosity ) const
{
	path_parts path( initiator );																						// <0x65c629>|0x000|0x000:'32'
	threading::reader_writer_lock::mutex_raii raii( lock, threading::lock_type_read );									// <0x65c635>|0x00c|0x00c:'33'
	vostok::logging::verbosity allowed_verbosity = (vostok::logging::verbosity)initiator_tree->get_verbosity( &path );	// <0x65c658>|0x02f|0x023:'34'
	return allowed_verbosity >= verbosity;																				// <0x65c672>|0x049|0x01a:'35'
}


// STATE[87%|DONE]: LTCG for `malloc` and `fixed_string` constructor.
void filter_tree::push_filter( pcstr initiator, verbosity verbosity, u32 thread_id )
{
	if ( !initiator )																								// <0x65c54a>|0x000|0x000:'40'
		initiator = "";																								// <0x65c550>|0x006|0x006:'41'

	initiator_filter * const filter			=	(initiator_filter *)VOSTOK_NEW_IMPL( allocator, initiator_filter );	// <0x65c557>|0x00d|0x007:'43'

	filter->initiator						=	initiator;															// <0x65c5a9>|0x05f|0x052:'45'
	filter->verbosity						=	verbosity;															// <0x65c5b7>|0x06d|0x00e:'46'
	filter->thread_id						=	thread_id;															// <0x65c5c0>|0x076|0x009:'47'

	threading::reader_writer_lock_raii	raii	(lock, threading::lock_type_write);									// <0x65c5c9>|0x07f|0x009:'49'

	filter_stack.push_back					(filter);																// <0x65c5ec>|0x0a2|0x023:'51'

	build_tree								();																		// <0x65c5fd>|0x0b3|0x011:'53'
}

// STATE[100%|DONE]
bool filter_tree::filter_is_overwritten( initiator_filter* filter ) const
{

	for ( initiator_filter *	it		=	filter_stack.get_next_of_object(filter);	// <0x65c0aa>|0x000|0x000|[1]:'72' // sushi@NOTE: Why is the first filter skipped, is this a bug?
								it		!=	NULL;										// <0x65c0b2>|0x008|0x008:'73'
								it		=	filter_stack.get_next_of_object(it) )		// <0x65c0b4>|0x00a|0x002:'74'
	{
		if ( filter->initiator.find(it->initiator.c_str()) == 0 )						// <0x65c0c2>|0x018|0x00e:'76'
			return							true;										// <0x65c0de>|0x034|0x01c:'77'
	}																					// <0x65c0e2>|0x038|0x004:'78'

	return									false;										// <0x65c0e4>|0x03a|0x002:'80'
}

// STATE[100%|DONE]: LTCG for `malloc`.
void filter_tree::build_tree( )
{
	initiator_tree->clean( &allocator );																	// <0x65c319>|0x000|0x000:'85'

	for ( initiator_filter *	it		=	filter_stack.front();											// <0x65c32b>|0x012|0x012|[1]:'87'
								it		!=	NULL;															// <0x65c334>|0x01b|0x009:'88'
								it		=	filter_stack.get_next_of_object(it)	)							// <0x65c336>|0x01d|0x002:'89'
	{
		if ( !filter_is_overwritten( it ) )																	// <0x65c344>|0x02b|0x00e:'91'
			initiator_tree->set( it->initiator.c_str(), it->verbosity, it->thread_id, &allocator, NULL );	// <0x65c357>|0x03e|0x013:'92'
	}																										// <0x65c385>|0x06c|0x02e:'93'
}

// STATE[91%|DONE]
filter_tree* new_filter_tree( memory::base_allocator& allocator )
{
	return VOSTOK_NEW_IMPL( allocator, filter_tree )( allocator );	// <0x65c4e6>|0x000|0x000:'109'
}

// STATE[100%|DONE]
void delete_filter_tree( filter_tree*& filter_tree )
{
	VOSTOK_DELETE_IMPL( filter_tree->allocator, filter_tree );	// <0x65c4b6>|0x000|0x000:'114'
}

// STATE[100%|DONE]
void push_filter(
	filter_tree&	tree,
	pcstr			initiator,
	verbosity		verbosity,
	u32				thread_id
)
{
	tree.push_filter( initiator, verbosity, thread_id );	// <0x65c6c3>|0x000|0x000:'124'
}

// STATE[100%|DONE]
bool has_passed_filters( filter_tree const& tree, pcstr initiator, verbosity verbosity )
{
	return tree.has_passed_filters( initiator, verbosity );	// <0x65c6a3>|0x000|0x000:'134'
}

pcstr verbosity_to_str[] ={
	"", // reserved
	"silent",
	"error",
	"warning",
	"info",
	"debug",
	"trace",
	NULL,
};

// STATE[99%|DONE]: Target allocated 4 more bytes.
verbosity string_to_verbosity( pcstr in_verbosity )
{
	vostok::logging::verbosity verbosities[6] = { silent, error, warning, info, debug, trace };	// <0x65c026>|0x000|0x000:'139'
	for ( u32 i = 0; i < 6; ++i	)																// <0x65c050>|0x02a|0x02a|[1]:'140'
		if ( !strings::compare_insensitive( verbosity_to_str[verbosities[i]], in_verbosity ) )	// <0x65c068>|0x042|0x018:'141'
			  return verbosities[i];															// <0x65c082>|0x05c|0x01a:'142'

	return invalid;																				// <0x65c08b>|0x065|0x009:'144'
}

// STATE[100%|DONE]
pcstr verbosity_to_string( verbosity verbosity )
{
	switch ( verbosity )	// <0x65bfb4>|0x000|0x000:'149'
	{
	case silent:	return "silent";
	case error:		return "ERROR";
	case warning:	return "Warning";
	case info:		return "info";
	case debug:		return "debug";
	case trace:		return "trace";
	default:		NODEFAULT( );
	}
}

struct filter_name_eq
{
	filter_name_eq ( pcstr s ) : name(s)	{}

	bool operator ()					(initiator_filter const & item) const
	{
		return								item.initiator == name;
	}

private:
	pcstr									name;
};

STATIC_SIZE_ASSERT(filter_name_eq, 0x4);

// STATE[99%|DONE]: Target allocated 4 more bytes
void logging_filters_console_command::save_to( console_commands::save_storage& f, memory::base_allocator* a ) const
{
	typedef vectora<initiator_filter>		filters_vec;
	filters_vec								uniq(a);

	for ( initiator_filter *	it		=	m_filter_tree.filter_stack.front();
								it		!=	NULL;
								it		=	m_filter_tree.filter_stack.get_next_of_object(it) )
	{
		filters_vec::iterator found		=	std::find_if(uniq.begin(), uniq.end(), filter_name_eq(it->initiator.c_str()) );
		if ( found != uniq.end() )
		{
			initiator_filter & filter	=	* found;
			filter.verbosity			=	it->verbosity;
		}
		else
			uniq.push_back					(*it);
	}

	u32 max_length						=	0;
	filters_vec::const_iterator uit_b	=	uniq.begin();
	filters_vec::const_iterator uit_e	=	uniq.end();
	for ( filters_vec::const_iterator uit =	uit_b; uit != uit_e; ++uit )
	{
		pcstr filter_str				=	(*uit).initiator.c_str();
		pcstr verbosity_str 			=	verbosity_to_str[(*uit).verbosity];
		u32 const length_to_test		=	strings::length(filter_str) + strings::length(verbosity_str);
		if ( length_to_test > max_length )
			max_length					=	length_to_test;
	}

	u32 const buffer_size				=	(max_length + strings::length( name() ) + 3)*sizeof(char);
	pstr const out_str					=	static_cast<pstr>( ALLOCA( buffer_size ) );
	for ( filters_vec::const_iterator uit = uit_b; uit != uit_e; ++uit )
	{
		pcstr filter_str				=	(*uit).initiator.c_str();
		pcstr verbosity_str 			=	verbosity_to_str[(*uit).verbosity];
		strings::join						( out_str, buffer_size, name(), " ", filter_str, " ", verbosity_str );
		f.add_line							( out_str );
	}
}
} // namespace logging
} // namespace vostok