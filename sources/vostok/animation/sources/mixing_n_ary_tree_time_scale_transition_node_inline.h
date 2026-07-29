////////////////////////////////////////////////////////////////////////////
//	Created		: 28.09.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef MIXING_N_ARY_TREE_TIME_SCALE_TRANSITION_NODE_INLINE_H_INCLUDED
#define MIXING_N_ARY_TREE_TIME_SCALE_TRANSITION_NODE_INLINE_H_INCLUDED

namespace vostok {
namespace animation {
namespace mixing {

inline n_ary_tree_time_scale_transition_node::n_ary_tree_time_scale_transition_node	(
		n_ary_tree_base_node& from,
		n_ary_tree_base_node& to,
		base_interpolator const& interpolator,
		u32 const current_time_in_ms
	) :
	super										( from, to, interpolator, current_time_in_ms )
{
	R_ASSERT_CMP								( m_last_integration_interval_value, >=, 0.f );
	check_consistency							( );
}

} // namespace mixing
} // namespace animation
} // namespace vostok

#endif // #ifndef MIXING_N_ARY_TREE_TIME_SCALE_TRANSITION_NODE_INLINE_H_INCLUDED
