////////////////////////////////////////////////////////////////////////////
//	Created		: 26.09.2011
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef MIXING_N_ARY_TREE_WEIGHT_TRANSITION_END_TIME_CALCULATOR_H_INCLUDED
#define MIXING_N_ARY_TREE_WEIGHT_TRANSITION_END_TIME_CALCULATOR_H_INCLUDED

#include "mixing_n_ary_tree_visitor.h"

namespace vostok {
namespace animation {
namespace mixing {

class n_ary_tree_animation_node;
class n_ary_tree_weight_transition_node;
class n_ary_tree_time_scale_transition_node;
class n_ary_tree_weight_node;
class n_ary_tree_time_scale_node;
class n_ary_tree_addition_node;
class n_ary_tree_subtraction_node;
class n_ary_tree_multiplication_node;

class n_ary_tree_weight_transition_end_time_calculator :
	public n_ary_tree_visitor,
	public boost::noncopyable
{
public:
	inline	n_ary_tree_weight_transition_end_time_calculator( ) : m_weight_transition_end_time_in_ms( u32(-1) ), m_event_type( 0 ) { }
	inline	u32		weight_transition_end_time				( ) const { return m_weight_transition_end_time_in_ms; }
	inline	u16		event_type								( ) const { return m_event_type; }

	virtual	void	visit									( n_ary_tree_animation_node& node );
	virtual	void	visit									( n_ary_tree_weight_transition_node& node );
	virtual	void	visit									( n_ary_tree_time_scale_transition_node& node );
	virtual	void	visit									( n_ary_tree_weight_node& node );
	virtual	void	visit									( n_ary_tree_time_scale_node& node )
	{
		VOSTOK_UNREFERENCED_PARAMETER		( node );
		NODEFAULT						( );
	}
	virtual	void	visit									( n_ary_tree_addition_node& node );
	virtual	void	visit									( n_ary_tree_subtraction_node& node )
	{
		VOSTOK_UNREFERENCED_PARAMETER		( node );
	}
	virtual	void	visit									( n_ary_tree_multiplication_node& node )
	{
		VOSTOK_UNREFERENCED_PARAMETER		( node );
	}

	virtual	~n_ary_tree_weight_transition_end_time_calculator( ) { }

private:
	u32		m_weight_transition_end_time_in_ms;
	float	m_min_weight;
	u16		m_event_type;
}; // class n_ary_tree_weight_transition_end_time_calculator

STATIC_SIZE_ASSERT(n_ary_tree_weight_transition_end_time_calculator, 0x10);

} // namespace mixing
} // namespace animation
} // namespace vostok

#endif // #ifndef MIXING_N_ARY_TREE_WEIGHT_TRANSITION_END_TIME_CALCULATOR_H_INCLUDED
