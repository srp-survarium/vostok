////////////////////////////////////////////////////////////////////////////
//	Created		: 19.02.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef BINARY_TREE_BASE_NODE_H_INCLUDED
#define BINARY_TREE_BASE_NODE_H_INCLUDED

#include <vostok/animation/mixing_debug.h>
#include <vostok/animation/api.h>

namespace vostok {
namespace animation {

struct base_interpolator;

namespace mixing {

struct binary_tree_visitor;
class binary_tree_weight_node;

#ifdef VOSTOK_ANIMATION_CHECK_CONSTRUCTORS
extern volatile tree_stats VOSTOK_ANIMATION_API g_binary_tree_stats;
#endif

struct binary_tree_base_node;

typedef intrusive_ptr<
			binary_tree_base_node,
			binary_tree_base_node,
			threading::single_threading_policy
		> binary_tree_base_node_ptr;

struct VOSTOK_ANIMATION_API VOSTOK_NOVTABLE binary_tree_base_node {
	inline					binary_tree_base_node	( );
	virtual					~binary_tree_base_node	( ) = 0;

#ifdef VOSTOK_ANIMATION_CHECK_CONSTRUCTORS
	inline					binary_tree_base_node	( binary_tree_base_node const& other );
#endif // #ifdef VOSTOK_ANIMATION_CHECK_CONSTRUCTORS

	virtual	void			accept					( binary_tree_visitor& visitor ) = 0;
	virtual	binary_tree_weight_node* cast_weight	( ) { return 0; }
//	virtual	base_interpolator const* interpolator	( ) const { return 0; }

	template < typename T >
	static inline void	destroy						( T* object );

	binary_tree_base_node*	m_next_weight;
	binary_tree_base_node*	m_same_weight;
	binary_tree_base_node*	m_next_unique_interpolator;

private:
	friend class threading::single_threading_policy;
	inline	u32				reference_count			( ) const { return m_reference_count; }

private:
	friend class threading::single_threading_policy;
	u32						m_reference_count;
}; // struct binary_tree_base_node

} // namespace mixing
} // namespace animation

template class VOSTOK_ANIMATION_API
	intrusive_ptr <
		vostok::animation::mixing::binary_tree_base_node,
		vostok::animation::mixing::binary_tree_base_node,
		vostok::threading::single_threading_policy
	>;

} // namespace vostok

#include <vostok/animation/mixing_binary_tree_base_node_inline.h>

#endif // #ifndef BINARY_TREE_BASE_NODE_H_INCLUDED