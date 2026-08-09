////////////////////////////////////////////////////////////////////////////
//	Created		: 28.09.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef MIXING_N_ARY_TREE_TIME_SCALE_TRANSITION_NODE_H_INCLUDED
#define MIXING_N_ARY_TREE_TIME_SCALE_TRANSITION_NODE_H_INCLUDED

#include "mixing_n_ary_tree_weight_transition_node.h"

namespace vostok {
namespace animation {
namespace mixing {

class n_ary_tree_time_scale_transition_node : public n_ary_tree_weight_transition_node {
public:
	inline	n_ary_tree_time_scale_transition_node				(
						n_ary_tree_base_node& from,
						n_ary_tree_base_node& to,
						base_interpolator const& interpolator,
						u32 const current_time_in_ms
					);
	virtual			~n_ary_tree_time_scale_transition_node			( );

	virtual	void	accept										( n_ary_tree_visitor& visitor );

private:
	virtual	bool	is_time_scale								( ) { return true; }
	virtual	bool	is_weight									( ) { return false; }
	virtual bool	is_transition								( ) { return true; }

	virtual void	accept										( n_ary_tree_double_dispatcher& dispatcher, n_ary_tree_base_node& node );

	virtual void	visit										( n_ary_tree_double_dispatcher& dispatcher, n_ary_tree_animation_node& node );
	virtual void	visit										( n_ary_tree_double_dispatcher& dispatcher, n_ary_tree_weight_node& node );
	virtual void	visit										( n_ary_tree_double_dispatcher& dispatcher, n_ary_tree_time_scale_node& node );
	virtual void	visit										( n_ary_tree_double_dispatcher& dispatcher, n_ary_tree_addition_node& node );
	virtual void	visit										( n_ary_tree_double_dispatcher& dispatcher, n_ary_tree_subtraction_node& node );
	virtual void	visit										( n_ary_tree_double_dispatcher& dispatcher, n_ary_tree_multiplication_node& node );
	virtual void	visit										( n_ary_tree_double_dispatcher& dispatcher, n_ary_tree_weight_transition_node& node );
	virtual void	visit										( n_ary_tree_double_dispatcher& dispatcher, n_ary_tree_time_scale_transition_node& node );

private:
			void	check_consistency							( );

private:
	typedef n_ary_tree_weight_transition_node	super;
}; // class n_ary_tree_time_scale_transition_node

STATIC_SIZE_ASSERT(n_ary_tree_time_scale_transition_node, 0x14);

} // namespace mixing
} // namespace animation
} // namespace vostok

#include "mixing_n_ary_tree_time_scale_transition_node_inline.h"

#endif // #ifndef MIXING_N_ARY_TREE_TIME_SCALE_TRANSITION_NODE_H_INCLUDED
