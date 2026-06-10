////////////////////////////////////////////////////////////////////////////
//	Created 	: 01.09.2008
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"


#include "filter_tree_node.h"
#include "path_parts.h"

using vostok::logging::node;
using vostok::logging::node_base;
using vostok::logging::compare_nodes;
using vostok::logging::path_parts;
using vostok::logging::verbosity;
using vostok::logging::invalid;

enum verbosity_recursion
{
	recurse_all	=	0,
	recurse_0	=	256,
};

// STATE[60%|PARTIAL]: boost-internal inline-vs-call - target calls ~set_member_hook out-of-line (eax conv),
// base inlines it down to destructor_impl + bool temp. boost headers, cross-unit; banked.
node::~node				()
{
	// STRUCTURE DIFF: target 0 stmts / base 0 stmts (0x2b vs 0x39 bytes)
	// VERDICT: STRUCTURE MATCH (shape ok) - rbtree clear matches; tail diverges on boost hook dtor inline-vs-call.
}

// STATE[91%|DONE]: LTCG for `buffer_string::append`, `base_allocator::malloc_impl` and `fixed_string::fixed_string`.
void node::set			(pcstr const							initiator_path,
						 int const								verbosity,
						 u32 const								thread_id,
						 vostok::memory::base_allocator * const	allocator,
						 vostok::memory::base_allocator * const	allocator_to_clean)
{
	if ( !initiator_path || !*initiator_path )
	{
		m_verbosity 					=	(vostok::logging::verbosity)(verbosity & ~recurse_0);	// <0x65b1f0>|0x010|0x010:'43'
		m_thread_id						=	thread_id;												// <0x65b202>|0x022|0x012:'44'
		if ( !(verbosity & recurse_0) )																// <0x65b20e>|0x02e|0x00c:'45'
			clean							(allocator_to_clean);									// <0x65b218>|0x038|0x00a:'46'
		return;																						// <0x65b227>|0x047|0x00f:'47'
	}

	pcstr const next_path_portion		=	strchr(initiator_path, initiator_separator);

	node_name_type	path_portion;
	if ( next_path_portion )
		path_portion.assign					(initiator_path, next_path_portion);
	else
		path_portion					=	initiator_path;

	nodes_tree_type::iterator	it		=	m_children.find(path_portion.c_str(), m_children.key_comp());
 	node * child						=	NULL;
 	if ( it != m_children.end() )
 	{
 		child							=	static_cast<node *>(& * it);										// <0x65b30e>|0x12e|0x03c:'62'
 	}
 	else																										// <0x65b319>|0x139|0x00b:'64'
 	{
 		child							=	VOSTOK_NEW_IMPL(allocator, node)(path_portion.c_str(), invalid);	// <0x65b31b>|0x13b|0x002:'66'
		m_children.insert					(* child);															// <0x65b384>|0x1a4|0x069:'67'
 	}

	child->set								(next_path_portion ? next_path_portion + 1 : NULL,
											 verbosity, thread_id, allocator, allocator_to_clean);

	// STRUCTURE DIFF: target 20 stmts / base 20 stmts
	// SIZE +0x1  | 53 | path_portion					=	initiator_path;
	// SIZE -0xe  | 57 | if ( it != m_children.end() )
	// SIZE -0x17 | 63 | child							=	VOSTOK_NEW_IMPL(allocator, node)(path_portion.c_str(), invalid);
	// VERDICT: STRUCTURE MATCH (shape ok) - 53: fixed_string operator= overload/conv (core); 57: boost end()/iterator
	// compare inline-vs-call; 63: memory::strip_pointer + new_helper called out-of-line in target (core). Banked.
}

// STATE[92%|DONE]: target calls memory::strip_pointer (identity fn, folds to empty) before delete_helper_impl - core, banked.
void node::clean						(vostok::memory::base_allocator * allocator)
{
	while ( node * dying = static_cast<node *>(m_children.unlink_leftmost_without_rebalance()) )	// <0x65b169>|0x000|0x000|[1]:'76'
	{
		ASSERT								(allocator);
		dying->clean						(allocator);
		VOSTOK_DELETE_IMPL					(allocator, dying);
	}

	m_children.clear						();

	// STRUCTURE DIFF: target 6 stmts / base 6 stmts
	// SIZE -0xb | 78 | VOSTOK_DELETE_IMPL					(allocator, dying);
	// VERDICT: STRUCTURE MATCH (shape ok) - target's strip_pointer(allocator) is an out-of-line call (eax conv,
	// identity -> folds with empty stubs); base inlines it away. Core macro chain, banked.
}

// STATE[78%|PARTIAL]: boost rbtree find/iterator inline-vs-call (target expands the rbtree_impl::find wrapper and
// calls tree_algorithms::find; base calls the wrapper out-of-line). boost headers, cross-unit; banked.
verbosity node::get_verbosity					(path_parts * path, verbosity inherited_verbosity) const
{
	vostok::logging::verbosity verbosity =
		(m_thread_id != u32(-1) && m_thread_id != threading::current_thread_id()) ?
		silent :
		(
			m_verbosity != invalid ?
			m_verbosity :
			inherited_verbosity
		);																								// <0x65b009>|0x000|0x000:'95'

	pcstr cur_part						=	path->get_current_element();								// <0x65b04e>|0x045|0x045:'97'
	if ( !cur_part || cur_part[0] == NULL )																// <0x65b05d>|0x054|0x00f:'98'
	{
		// last element in path
		return								verbosity;													// <0x65b06d>|0x064|0x010:'101'
	}

 	nodes_tree_type::const_iterator const it	=	m_children.find(cur_part, m_children.key_comp());	// <0x65b075>|0x06c|0x008:'104'

 	if ( it == m_children.end() ) // no special rule?													// <0x65b0bb>|0x0b2|0x046:'106'
 		return								verbosity;													// <0x65b0f5>|0x0ec|0x03a:'107'

	const node * const child					=	static_cast<const node *>(& * it);					// <0x65b0fa>|0x0f1|0x005:'109'

 	path->to_next_element					();															// <0x65b105>|0x0fc|0x00b:'111'
 	return									child->get_verbosity(path, verbosity);						// <0x65b10d>|0x104|0x008:'112'

	// STRUCTURE DIFF: target 10 stmts / base 10 stmts
	// SIZE -0x3  | 100 | return								verbosity;
	// SIZE -0x28 | 103 | nodes_tree_type::const_iterator const it	=	m_children.find(cur_part, m_children.key_comp());
	// SIZE -0x13 | 105 | if ( it == m_children.end() )
	// VERDICT: STRUCTURE MATCH (shape ok) - 103/105: boost find-wrapper/end() inline-vs-call; 100: jmp rel32 vs rel8
	// to the epilogue, a cascade of the function being 0x3e bigger in target. All boost/LTCG, banked.
}

static inline bool is_terminal_character( char character )
{
	return character == '\0' || character == vostok::logging::initiator_separator;				// <0x65ae44>|0x000|0x000:'117'
}

// STATE[9x%|DONE]: sushi@TODO: Ghidra script does not handle static functions. https://decomp.me/scratch/aXNEg
static bool   compare_parts				(pcstr s1, pcstr s2)
{
    for ( ;; ++s1, ++s2 ) {						// <0x65ae76>|0x000|0x000:'123'
    	if ( is_terminal_character(*s1) )		// <0x65ae8a>|0x014|0x014:'124'
        	if ( is_terminal_character(*s2) )	// <0x65aebb>|0x045|0x031:'125'
            	return false;					// <0x65aeec>|0x076|0x031:'126'
		    else								// <0x65aef0>|0x07a|0x004:'127'
            	return true;					// <0x65aef2>|0x07c|0x002:'128' // sushi@NOTE: While assembly matches, this structure doesn't
												// <0x65aef6>|0x080|0x004:'129'
        else if ( is_terminal_character(*s2) )	// <0x65aef8>|0x082|0x002:'130'
			return false;						// <0x65af29>|0x0b3|0x031:'131'


        if ( *s1 != *s2 )						// <0x65af2d>|0x0b7|0x004:'134'
            return *s1 < *s2;					// <0x65af3d>|0x0c7|0x010:'135'
    }											// <0x65af54>|0x0de|0x017:'136'
}

// STATE[71%|PARTIAL]: target inlines fixed_string operator< (calling c_str x2 + detail::strcmp_s == -1 out-of-line);
// base calls the unmarked template vostok::operator< instead. Core buffer_string header, cross-unit; banked.
bool   compare_nodes::operator ()		(node_base const & left, node_base const & right) const
{
	return									left.name < right.name;

	// STRUCTURE DIFF: target 1 stmt / base 1 stmt
	// SIZE -0x14 | 141 | return									left.name < right.name;
	// VERDICT: STRUCTURE MISMATCH (size) - core operator< inline-vs-call (direction reversed: target expands it); blocked on core.
}

// STATE[100%|DONE]
bool   compare_nodes::operator ()		(pcstr const left, node_base const & right) const
{
	return									compare_parts(left, right.name.c_str());
}

// STATE[100%|DONE]
bool   compare_nodes::operator ()		(node_base const & left, pcstr const right) const
{
	return									compare_parts(left.name.c_str(), right);
}