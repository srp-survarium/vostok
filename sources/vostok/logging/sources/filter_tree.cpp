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

// STATE[99.75%|DONE]: LTCG conv inside the VOSTOK_NEW_IMPL chain.
filter_tree::filter_tree( memory::base_allocator& allocator ) :
	lock			( ),
	initiator_tree	( VOSTOK_NEW_IMPL( allocator, node )( "", trace ) ),
	allocator		( allocator ),
	filter_stack	( )
{
	// STRUCTURE DIFF: target 0 stmts / base 0 stmts (sizes equal, 0xc2)
	// VERDICT: STRUCTURE MATCH - residual is reloc/conv noise inside the new_helper chain, LTCG; banked.
}

// STATE[100%|DONE]
filter_tree::~filter_tree( )
{
	initiator_tree->clean( &allocator );
	VOSTOK_DELETE_IMPL( allocator, initiator_tree );
}


// STATE[100%|DONE]
bool filter_tree::has_passed_filters( pcstr initiator, verbosity verbosity ) const
{
	path_parts path( initiator );
	threading::reader_writer_lock::mutex_raii raii( lock, threading::lock_type_read );
	vostok::logging::verbosity allowed_verbosity = (vostok::logging::verbosity)initiator_tree->get_verbosity( &path );
	return allowed_verbosity >= verbosity;
}


// STATE[91%|DONE]: LTCG for `malloc` and `fixed_string` constructor/operator=.
void filter_tree::push_filter( pcstr initiator, verbosity verbosity, u32 thread_id )
{
	if ( !initiator )
		initiator = "";

	initiator_filter * const filter			=	(initiator_filter *)VOSTOK_NEW_IMPL( allocator, initiator_filter );

	filter->initiator						=	initiator;
	filter->verbosity						=	verbosity;
	filter->thread_id						=	thread_id;

	threading::reader_writer_lock_raii	raii	(lock, threading::lock_type_write);

	filter_stack.push_back					(filter);

	build_tree								();

	// STRUCTURE DIFF: target 9 stmts / base 9 stmts
	// SIZE +0x1 | 56 | filter->initiator						=	initiator;
	// VERDICT: STRUCTURE MATCH (shape ok) - target calls fixed_string operator=(pcstr by-value, esi/ecx conv),
	// base the template operator=<pcstr>(const&) - core header overload set + LTCG conv, banked.
}

// STATE[31%|PARTIAL]: target calls core buffer_string::find + fixed_string::c_str out-of-line; base inlines
// the whole find (strstr + pointer math, +0x36). Cross-unit (vostok core headers) - flagged, not edited here.
bool filter_tree::filter_is_overwritten( initiator_filter* filter ) const
{

	for ( initiator_filter *	it		=	filter_stack.get_next_of_object(filter);	// sushi@NOTE: Why is the first filter skipped, is this a bug?
								it		!=	NULL;
								it		=	filter_stack.get_next_of_object(it) )
	{
		if ( filter->initiator.find(it->initiator.c_str()) == 0 )
			return							true;
	}

	return									false;

	// STRUCTURE DIFF: target 7 stmts / base 7 stmts
	// SIZE +0x36 | 75 | if ( filter->initiator.find(it->initiator.c_str()) == 0 )
	// VERDICT: STRUCTURE MISMATCH (size) - core buffer_string::find/c_str inline-vs-call (target calls both
	// out-of-line); needs the core-side out-of-line definitions, blocked from logging.
}

// STATE[100%|DONE]: LTCG for `malloc`.
void filter_tree::build_tree( )
{
	initiator_tree->clean( &allocator );

	for ( initiator_filter *	it		=	filter_stack.front();
								it		!=	NULL;
								it		=	filter_stack.get_next_of_object(it)	)
	{
		if ( !filter_is_overwritten( it ) )
			initiator_tree->set( it->initiator.c_str(), it->verbosity, it->thread_id, &allocator, NULL );
	}
}

// STATE[91%|DONE]
filter_tree* new_filter_tree( memory::base_allocator& allocator )
{
	return VOSTOK_NEW_IMPL( allocator, filter_tree )( allocator );
}

// STATE[100%|DONE]
void delete_filter_tree( filter_tree*& filter_tree )
{
	VOSTOK_DELETE_IMPL( filter_tree->allocator, filter_tree );
}

// STATE[100%|DONE]
void push_filter(
	filter_tree&	tree,
	pcstr			initiator,
	verbosity		verbosity,
	u32				thread_id
)
{
	tree.push_filter( initiator, verbosity, thread_id );
}

// STATE[100%|DONE]
bool has_passed_filters( filter_tree const& tree, pcstr initiator, verbosity verbosity )
{
	return tree.has_passed_filters( initiator, verbosity );
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

// STATE[80%|DONE]: core strings::compare_insensitive called out-of-line in target (ecx/eax conv), inlined to __stricmp in base.
verbosity string_to_verbosity( pcstr in_verbosity )
{
	vostok::logging::verbosity verbosities[6] = { silent, error, warning, info, debug, trace };
	for ( u32 i = 0; i < 6; ++i	)
		if ( !strings::compare_insensitive( verbosity_to_str[verbosities[i]], in_verbosity ) )
			  return verbosities[i];

	return invalid;

	// STRUCTURE DIFF: target 5 stmts / base 5 stmts
	// SIZE +0xb | 141 | if ( !strings::compare_insensitive( verbosity_to_str[verbosities[i]], in_verbosity ) )
	// VERDICT: STRUCTURE MATCH (shape ok) - core compare_insensitive inline-vs-call; core-side, banked.
}

// STATE[100%|DONE]
pcstr verbosity_to_string( verbosity verbosity )
{
	switch ( verbosity )
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

STATIC_SIZE_ASSERT(filter_name_eq, // sushi@);

// STATE[97.2%|DONE]: LTCG conv in the vectora ctor / strings::join / add_line calls (-0x2/-0x1/-0x1).
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

	// STRUCTURE DIFF: target 29 stmts / base 29 stmts
	// SIZE -0x2 | 181 | filters_vec								uniq(a);
	// SIZE -0x1 | 215 | strings::join						( out_str, buffer_size, name(), " ", filter_str, " ", verbosity_str );
	// SIZE -0x1 | 216 | f.add_line							( out_str );
	// VERDICT: STRUCTURE MATCH (shape ok) - 4 bytes of LTCG arg/conv noise across three calls; banked.
}
} // namespace logging
} // namespace vostok