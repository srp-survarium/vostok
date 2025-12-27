////////////////////////////////////////////////////////////////////////////
//	Unit		: n_ary_tree_weight_calculator.h
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_ANIMATION_N_ARY_TREE_WEIGHT_CALCULATOR_H_INCLUDED
#define VOSTOK_ANIMATION_N_ARY_TREE_WEIGHT_CALCULATOR_H_INCLUDED

#include "mixing_n_ary_tree_visitor.h"
#include "mixing_n_ary_tree_animation_node.h"

namespace vostok {
namespace animation {
namespace mixing {

class n_ary_tree_weight_calculator :
	public n_ary_tree_visitor,
	private boost::noncopyable
{
public:
	inline	n_ary_tree_weight_calculator( u32 const current_time_in_ms, n_ary_tree_animation_node* const animation );
	inline	float	weight				( ) const;
	inline	bool	null_weight_found	( ) const;
	inline	u32		weight_transition_ended_time_in_ms	( ) const;

	virtual	void	visit				( n_ary_tree_animation_node& node );
	virtual	void	visit				( n_ary_tree_weight_transition_node& node );
	virtual	void	visit				( n_ary_tree_time_scale_transition_node& node );
	virtual	void	visit				( n_ary_tree_weight_node& node );
	virtual	void	visit				( n_ary_tree_time_scale_node& node );
	virtual	void	visit				( n_ary_tree_addition_node& node );
	virtual	void	visit				( n_ary_tree_subtraction_node& node );
	virtual	void	visit				( n_ary_tree_multiplication_node& node );

private:
			void	remove_transition	( n_ary_tree_weight_transition_node& node );

private:
	/* 0x0004 */	n_ary_tree_animation_node* const	m_animation;
	/* 0x0008 */	n_ary_tree_base_node*				m_result;
	/* 0x000c */	u32									m_recursion_level;
	/* 0x0010 */	u32									m_current_time_in_ms;
	/* 0x0014 */	float								m_weight;
	/* 0x0018 */	u32									m_weight_transition_ended_time_in_ms;
	/* 0x001c */	bool								m_null_weight_found;
}; // class n_ary_tree_weight_calculator

STATIC_SIZE_ASSERT(n_ary_tree_weight_calculator, 0x20);

} // namespace mixing
} // namespace animation
} // namespace vostok

#include "mixing_n_ary_tree_weight_calculator_inline.h"

#endif // #ifndef VOSTOK_ANIMATION_N_ARY_TREE_WEIGHT_CALCULATOR_H_INCLUDED