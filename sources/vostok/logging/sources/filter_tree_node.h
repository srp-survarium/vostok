////////////////////////////////////////////////////////////////////////////
//	Created 	: 01.09.2008
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef LOGGING_RULE_TREE_NODE_H_INCLUDED
#define LOGGING_RULE_TREE_NODE_H_INCLUDED

#include <vostok/logging/extensions.h>
#include <vostok/logging/api.h>

#include "path_parts.h"
#include "filter_tree_node_base.h"

namespace vostok {
namespace logging {

class path_parts;

class node : public node_base, private boost::noncopyable
{
public:
	inline	explicit	node			(pcstr const name, verbosity const filter) : node_base(name), m_verbosity(filter), m_thread_id(u32(-1)) {}
						~node			();
			void		set				(pcstr						initiator_path,
										 int						verbosity,
										 u32						thread_id,
										 memory::base_allocator *	allocator,
										 memory::base_allocator *	allocator_to_clean);
	inline	verbosity	get_verbosity	(path_parts* path) const { return get_verbosity(path, silent); }
private:
			verbosity	get_verbosity	(path_parts* path, verbosity inherited_verbosity) const;

public:
			void		clean			(memory::base_allocator * allocator);

private:
	nodes_tree_type						m_children;

	verbosity							m_verbosity;
	u32									m_thread_id;
}; // class node

STATIC_SIZE_ASSERT(node, 0x54);

} // namespace logging
} // namespace vostok

#endif // #ifndef LOGGING_RULE_TREE_NODE_H_INCLUDED
