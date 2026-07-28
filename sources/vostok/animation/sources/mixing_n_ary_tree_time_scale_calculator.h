////////////////////////////////////////////////////////////////////////////
//	Created		: 27.09.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef MIXING_N_ARY_TREE_TIME_SCALE_CALCULATOR_H_INCLUDED
#define MIXING_N_ARY_TREE_TIME_SCALE_CALCULATOR_H_INCLUDED

#include "mixing_n_ary_tree_visitor.h"
#include "mixing_n_ary_tree_animation_node.h"

namespace vostok {
namespace animation {
namespace mixing {

class n_ary_tree_time_scale_calculator :
	private boost::noncopyable,
	public n_ary_tree_visitor
{
public:
	enum transitions_destroying_enum {
		forbid_transitions_destroying,
		permit_transitions_destroying,
	}; // enum transitions_destroying_enum

public:
					n_ary_tree_time_scale_calculator(
						u32 current_time_in_ms,
						float previous_animation_time,
						u32 previous_time_in_ms,
						n_ary_tree_animation_node* animation = 0
					);


	inline	n_ary_tree_base_node*		result				( ) const;
	inline	float						time_scale			( ) const;
	inline	u32							transitions_count	( ) const;
	inline	base_interpolator const*	interpolator		( ) const;

	virtual	void	visit							( n_ary_tree_animation_node& node );
	virtual	void	visit							( n_ary_tree_weight_transition_node& node );
	virtual	void	visit							( n_ary_tree_time_scale_transition_node& node );
	virtual	void	visit							( n_ary_tree_weight_node& node );
	virtual	void	visit							( n_ary_tree_time_scale_node& node );
	virtual	void	visit							( n_ary_tree_addition_node& node );
	virtual	void	visit							( n_ary_tree_subtraction_node& node );
	virtual	void	visit							( n_ary_tree_multiplication_node& node );

private:
			void	remove_transition				( n_ary_tree_time_scale_transition_node& node );

	n_ary_tree_animation_node* const		m_animation;
	n_ary_tree_base_node*				m_result;
	base_interpolator const*			m_interpolator;
	u32 const							m_current_time_in_ms;
	float const							m_previous_animation_time;
	u32 const							m_previous_time_in_ms;
	float								m_time_scale;
	u32									m_transitions_count;
	u32									m_recursion_level;
}; // class n_ary_tree_time_scale_calculator

STATIC_SIZE_ASSERT(n_ary_tree_time_scale_calculator, 0x28);

} // namespace mixing
} // namespace animation
} // namespace vostok

#include "mixing_n_ary_tree_time_scale_calculator_inline.h"

#endif // #ifndef MIXING_N_ARY_TREE_TIME_SCALE_CALCULATOR_H_INCLUDED
