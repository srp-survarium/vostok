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

node::~node				()
{
}

void node::set			(pcstr const							initiator_path,
						 int const								verbosity,
						 u32 const								thread_id,
						 vostok::memory::base_allocator * const	allocator,
						 vostok::memory::base_allocator * const	allocator_to_clean)
{
	if ( !initiator_path || !*initiator_path )
	{
		m_verbosity 					=	(vostok::logging::verbosity)(verbosity & ~recurse_0);
		m_thread_id						=	thread_id;
		if ( !(verbosity & recurse_0) )
			clean							(allocator_to_clean);
		return;
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
 		child							=	static_cast<node *>(& * it);
 	}
 	else
 	{
 		child							=	VOSTOK_NEW_IMPL(allocator, node)(path_portion.c_str(), invalid);
		m_children.insert					(* child);
 	}

	child->set								(next_path_portion ? next_path_portion + 1 : NULL,
											 verbosity, thread_id, allocator, allocator_to_clean);
}

void node::clean						(vostok::memory::base_allocator * allocator)
{
	while ( node * dying = static_cast<node *>(m_children.unlink_leftmost_without_rebalance()) )
	{
		ASSERT								(allocator);
		dying->clean						(allocator);
		VOSTOK_DELETE_IMPL					(allocator, dying);
	}

	m_children.clear						();
}

verbosity node::get_verbosity					(path_parts * path, verbosity inherited_verbosity) const
{
	vostok::logging::verbosity verbosity =
		(m_thread_id != u32(-1) && m_thread_id != threading::current_thread_id()) ?
		silent :
		(
			m_verbosity != invalid ?
			m_verbosity :
			inherited_verbosity
		);

	pcstr cur_part						=	path->get_current_element();
	if ( !cur_part || cur_part[0] == NULL )
	{
		// last element in path
		return								verbosity;
	}

 	nodes_tree_type::const_iterator const it	=	m_children.find(cur_part, m_children.key_comp());

 	if ( it == m_children.end() ) // no special rule?
 		return								verbosity;

	const node * const child					=	static_cast<const node *>(& * it);

 	path->to_next_element					();
 	return									child->get_verbosity(path, verbosity);
}

static inline bool is_terminal_character( char character )
{
	return character == '\0' || character == vostok::logging::initiator_separator;
}

static bool   compare_parts				(pcstr s1, pcstr s2)
{
    for ( ;; ++s1, ++s2 ) {
    	if ( is_terminal_character(*s1) )
        	if ( is_terminal_character(*s2) )
            	return false;
		    else
            {
            	return true;
            }
        else if ( is_terminal_character(*s2) )
			return false;


        if ( *s1 != *s2 )
            return *s1 < *s2;
    }
}

// Target inlines buffer_string::operator<; this context keeps the shared
// operator as a call. The intrusive operations below hit the same boundary.
bool   compare_nodes::operator ()		(node_base const & left, node_base const & right) const
{
	return									left.name < right.name;
}

bool   compare_nodes::operator ()		(pcstr const left, node_base const & right) const
{
	return									compare_parts(left, right.name.c_str());
}

bool   compare_nodes::operator ()		(node_base const & left, pcstr const right) const
{
	return									compare_parts(left.name.c_str(), right);
}
