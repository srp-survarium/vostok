////////////////////////////////////////////////////////////////////////////
//	Created		: 24.03.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef N_ARY_TREE_H_INCLUDED
#define N_ARY_TREE_H_INCLUDED

//#include "mixing_n_ary_tree_animation_node.h"
#include "mixing_n_ary_tree_intrusive_base.h"
#include "mixing_animation_state.h"

namespace vostok {
namespace animation {

struct base_interpolator;

namespace mixing {

class n_ary_tree_animation_node;
class animated_object_holder;

class n_ary_tree {
public:
	typedef	intrusive_ptr	<
				n_ary_tree_intrusive_base,
				n_ary_tree_intrusive_base,
				threading::single_threading_policy
			>						n_ary_tree_ptr;

public:
	inline							n_ary_tree				( );
	inline							n_ary_tree				(
										n_ary_tree_animation_node* const root,
										memory::base_allocator* const allocator,
										base_interpolator const** const interpolators,
										animation_state* const animations_states,
										n_ary_tree_intrusive_base* const reference_counter,
										u32 const animations_count,
										u32 const interpolators_count,
										u32 const transitions_count
									);
	inline							n_ary_tree				( n_ary_tree const& other );
	inline	n_ary_tree&				operator =				( n_ary_tree const& other );
									~n_ary_tree				( );
			void					tick					( u32 const current_time_in_ms );
			void					dump					( ) const;
	inline	animation_state const*	animation_states		( ) const;
	inline	u32				significant_animations_count	( ) const;
	inline	n_ary_tree_animation_node* root					( ) const;
	inline	base_interpolator const* const*	interpolators	( ) const;
	inline	u32						interpolators_count		( ) const;

private:
	inline	void					assign					( n_ary_tree const& other );
			void					destroy					( );

private:
	/* 0x0000 */	n_ary_tree_intrusive_base_ptr		m_reference_counter;
	/* 0x0004 */	n_ary_tree_animation_node*			m_weight_root;
	/* 0x0008 */	n_ary_tree_animation_node*			m_time_root;
	/* 0x000c */	base_interpolator const**			m_interpolators;
	/* 0x0010 */	animation_state*					m_animation_states;
	/* 0x0014 */	animation_state**					m_animation_events;
	/* 0x0018 */	animated_object_holder*				m_animated_objects;
	/* 0x001c */	u32									m_animations_count;
	/* 0x0020 */	u32									m_animated_objects_count;
	/* 0x0024 */	u32									m_interpolators_count;
	/* 0x0028 */	u32									m_tree_actual_time_in_ms;
	/* 0x002c */	bool								m_is_logging_enabled;
}; // class n_ary_tree

STATIC_SIZE_ASSERT(n_ary_tree, 0x30);

} // namespace mixing
} // namespace animation
} // namespace vostok

#include "mixing_n_ary_tree_inline.h"

#endif // #ifndef N_ARY_TREE_H_INCLUDED
