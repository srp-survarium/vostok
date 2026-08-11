////////////////////////////////////////////////////////////////////////////
//	Created		: 23.11.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef MIXING_N_ARY_TREE_TRANSITION_TREE_CONSTRUCTOR_IMPL_H_INCLUDED
#define MIXING_N_ARY_TREE_TRANSITION_TREE_CONSTRUCTOR_IMPL_H_INCLUDED

#include "mixing_n_ary_tree_transition_to_one_detector.h"
#include "mixing_animation_state.h"

namespace vostok {
namespace animation {
namespace mixing {

std::pair< u32, u32 > computed_operands_count				( n_ary_tree_animation_node& from, n_ary_tree_animation_node& to );

inline bool is_asynchronous_animation						( n_ary_tree_animation_node& animation )
{
	return										animation.time_synchronization_group_id() == u32( -1 ); // sushi@TODO
}

inline n_ary_tree_animation_node* find_animation			(
		n_ary_tree_animation_node* const begin,
		n_ary_tree_animation_node* const end,
		n_ary_tree_animation_node& animation_to_find
	)
{
	animation_comparer_equal_predicate equal_predicate( false, false );
	for ( n_ary_tree_animation_node* i = begin; i != end; i = i->m_next_weight_animation )
		if ( equal_predicate( *i, animation_to_find ) )
			return								i;

	return										0;
}

inline n_ary_tree_animation_node* synchronization_group_end	( n_ary_tree_animation_node* const driving_animation )
{
	R_ASSERT									( driving_animation );
	n_ary_tree_animation_node* i			= driving_animation;
	u32 const current_synchronization_group_id	= driving_animation->time_synchronization_group_id(); // sushi@TODO
	for (; i && (*i).time_synchronization_group_id() == current_synchronization_group_id; i = i->m_next_time_animation ) ; // sushi@TODO
	return										i;
}

inline void get_next_synchronization_group		(
		n_ary_tree_animation_node*& begin,
		n_ary_tree_animation_node*& end
	)
{
	begin										= end;
	if ( begin )
		end										= synchronization_group_end( begin );
}

inline comparison_result_enum compare_animation_intervals	( n_ary_tree_animation_node const& left, n_ary_tree_animation_node const& right )
{
	if ( left.animation_intervals_count() < right.animation_intervals_count() )
		return									less;

	if ( right.animation_intervals_count() < left.animation_intervals_count() )
		return									more;

	if ( left.start_cycle_animation_interval_id() < right.start_cycle_animation_interval_id() )
		return									less;

	if ( right.start_cycle_animation_interval_id() < left.start_cycle_animation_interval_id() )
		return									more;

	animation_interval const* i					= left.animation_intervals();
	animation_interval const* const e			= left.animation_intervals() + left.animation_intervals_count();
	animation_interval const* j					= right.animation_intervals();
	for ( ; i != e; ++i, ++j ) {
		if ( *i < *j )
			return								less;

		if ( *i > *j )
			return								more;
	}

	return										equal;
}

} // namespace mixing
} // namespace animation
} // namespace vostok

#endif // #ifndef MIXING_N_ARY_TREE_TRANSITION_TREE_CONSTRUCTOR_IMPL_H_INCLUDED
