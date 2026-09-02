// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 26.10.2025
////////////////////////////////////////////////////////////////////////////
#ifndef LOGGING_FILTER_TREE_H_INCLUDED
#define LOGGING_FILTER_TREE_H_INCLUDED

#include "filter.h"
#include <vostok/logging/api.h>
#include <vostok/threading_reader_writer_lock.h>

namespace vostok {
namespace logging {

class node;

class filter_tree {
public:
			filter_tree				( memory::base_allocator& allocator );
			~filter_tree			( );

			bool	has_passed_filters		( pcstr initiator, verbosity verbosity ) const;

			void	push_filter				( pcstr initiator, verbosity verbosity, u32 thread_id );
			void	pop_filter				( ) { /* sushi@NOTE: Wasn't used in target executable, can be recovered if needed */ }

private:
			bool	filter_is_overwritten	( initiator_filter* filter ) const;
			void	build_tree				( );

	// save_to iterates the private stack, so retail must have granted the
	// console command friendship (friends leave no trace in the pdb records).
	friend class logging_filters_console_command;

	/* 0x0000 */	threading::reader_writer_lock		lock;
	/* 0x0008 */	node*								initiator_tree;
public:
	/* 0x000c */	memory::base_allocator&				allocator;
private:
	/* 0x0010 */	filter_stack						filter_stack;
}; // class filter_tree

STATIC_SIZE_ASSERT(filter_tree, 0x38);

} // namespace logging
} // namespace vostok

#endif // #ifndef LOGGING_FILTER_TREE_H_INCLUDED
