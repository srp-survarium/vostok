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
	inline	void	pop_filter				( ) { /* no source */ }

			bool	filter_is_overwritten	( initiator_filter* filter ) const;

			void	build_tree				( );


public:
	/* 0x0000 */	threading::reader_writer_lock		lock;
	/* 0x0008 */	node*								initiator_tree;
	/* 0x000c */	memory::base_allocator&				allocator;
	/* 0x0010 */	filter_stack						filter_stack;
}; // class filter_tree

STATIC_SIZE_ASSERT(filter_tree, 0x38);

VOSTOK_LOGGING_API filter_tree*	new_filter_tree		( memory::base_allocator& allocator );
VOSTOK_LOGGING_API void			delete_filter_tree	( filter_tree*& filter_tree );
VOSTOK_LOGGING_API void			push_filter			(
									filter_tree&	tree,
									pcstr			initiator,
									verbosity		verbosity,
									u32				thread_id
								);
VOSTOK_LOGGING_API bool			has_passed_filters	( filter_tree const& tree, pcstr initiator, verbosity verbosity );

} // namespace logging
} // namespace vostok

#endif // #ifndef LOGGING_FILTER_TREE_H_INCLUDED