// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ANIMATION_MIXING_N_ARY_TREE_NULL_WEIGHT_DETECTOR_H_INCLUDED
#define ANIMATION_MIXING_N_ARY_TREE_NULL_WEIGHT_DETECTOR_H_INCLUDED

#include "mixing_n_ary_tree_visitor.h"
#include "mixing_n_ary_tree_animation_node.h"
#include "mixing_n_ary_tree_weight_transition_node.h"
#include "mixing_n_ary_tree_weight_node.h"
#include "mixing_n_ary_tree_addition_node.h"
#include "mixing_n_ary_tree_subtraction_node.h"
#include "mixing_n_ary_tree_multiplication_node.h"

namespace vostok {
namespace animation {
namespace mixing {

class n_ary_tree_null_weight_detector : public n_ary_tree_visitor {
public:
	// sushi@TODO: confirm the transition/compound-node rules against an editor target.
	inline	explicit	n_ary_tree_null_weight_detector	( n_ary_tree_animation_node& animation ) : m_result( false ) { animation.accept( *this ); }
	inline	bool		result							( ) const { return m_result; }

private:
	virtual	void		visit							( n_ary_tree_animation_node& node ) override
	{
		if ( node.weight_driving_animation( ) ) {
			node.weight_driving_animation( )->accept( *this );
			return;
		}
		m_result = false;
		n_ary_tree_base_node** i = node.operands( sizeof( n_ary_tree_animation_node ) );
		for ( n_ary_tree_base_node** const e = i + node.operands_count( ); i != e; ++i ) {
			if ( (*i)->is_time_scale( ) )
				continue;
			(*i)->accept( *this );
			if ( m_result )
				return;
		}
	}
	virtual	void		visit							( n_ary_tree_weight_transition_node& node ) override
	{
		node.from( ).accept( *this );
		if ( m_result )
			node.to( ).accept( *this );
	}
	virtual	void		visit							( n_ary_tree_time_scale_transition_node& ) override { m_result = false; }
	virtual	void		visit							( n_ary_tree_weight_node& node ) override { m_result = node.weight( ) == 0.f; }
	virtual	void		visit							( n_ary_tree_time_scale_node& ) override { m_result = false; }
	virtual	void		visit							( n_ary_tree_addition_node& node ) override
	{
		m_result = true;
		n_ary_tree_base_node** i = node.operands( sizeof( n_ary_tree_addition_node ) );
		for ( n_ary_tree_base_node** const e = i + node.operands_count( ); i != e; ++i ) {
			(*i)->accept( *this );
			if ( !m_result )
				return;
		}
	}
	virtual	void		visit							( n_ary_tree_subtraction_node& node ) override
	{
		m_result = true;
		n_ary_tree_base_node** i = node.operands( sizeof( n_ary_tree_subtraction_node ) );
		for ( n_ary_tree_base_node** const e = i + node.operands_count( ); i != e; ++i ) {
			(*i)->accept( *this );
			if ( !m_result )
				return;
		}
	}
	virtual	void		visit							( n_ary_tree_multiplication_node& node ) override
	{
		m_result = false;
		n_ary_tree_base_node** i = node.operands( sizeof( n_ary_tree_multiplication_node ) );
		for ( n_ary_tree_base_node** const e = i + node.operands_count( ); i != e; ++i ) {
			(*i)->accept( *this );
			if ( m_result )
				return;
		}
	}

private:
	/* 0x0000 */	/* n_ary_tree_visitor */
	/* 0x0004 */	bool	m_result;
}; // class n_ary_tree_null_weight_detector

STATIC_SIZE_ASSERT(n_ary_tree_null_weight_detector, 0x8);

} // namespace mixing
} // namespace animation
} // namespace vostok

#endif // #ifndef ANIMATION_MIXING_N_ARY_TREE_NULL_WEIGHT_DETECTOR_H_INCLUDED
