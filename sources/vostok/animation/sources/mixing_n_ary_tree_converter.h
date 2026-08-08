////////////////////////////////////////////////////////////////////////////
//	Created		: 26.02.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef N_ARY_TREE_CONVERTER_H_INCLUDED
#define N_ARY_TREE_CONVERTER_H_INCLUDED

#include <vostok/animation/mixing_n_ary_tree.h>

namespace vostok {
namespace animation {
namespace mixing {

class expression;
class n_ary_tree_animation_node;
class binary_tree_animation_node;
struct binary_tree_base_node;
class animated_object_holder;

typedef intrusive_ptr<
			binary_tree_base_node,
			binary_tree_base_node,
			threading::single_threading_policy
		> binary_tree_base_node_ptr;

class n_ary_tree_converter : private boost::noncopyable {
public:
				n_ary_tree_converter	( expression const& expression );
				~n_ary_tree_converter	( );
		u32		needed_buffer_size		( );
	n_ary_tree	constructed_n_ary_tree	(
					vostok::mutable_buffer& buffer,
					bool const is_final_tree,
					u32 const current_time_in_ms,
					subscribed_channel*& channels_head
				);

private:
	binary_tree_base_node* create_binary_multipliers	(
					mutable_buffer& buffer,
					binary_tree_base_node* const start_weight
				);
		void	sort_animations			( mutable_buffer& buffer );
		void	simplify_weights		( );
		void	compute_buffer_size		( );
		void	process_interpolators	( binary_tree_base_node* const interpolators_root, u32 const interpolators_count, vostok::mutable_buffer& buffer );
		void	fix_weight_driving_animations_with_null_weights ( expression const& expression );

private:
	/* 0x0000 */	binary_tree_base_node_ptr		m_root;
	/* 0x0004 */	binary_tree_animation_node*		m_animations_root;
	/* 0x0008 */	base_interpolator const**		m_binary_interpolators;
	/* 0x000c */	base_interpolator const**		m_interpolators;
	/* 0x0010 */	animation_state*				m_animation_states;
	/* 0x0014 */	animation_state**				m_animation_events;
	/* 0x0018 */	animated_object_holder*			m_animated_objects;
	/* 0x001c */	n_ary_tree_intrusive_base*		m_reference_counter;
	/* 0x0020 */	binary_tree_base_node*			m_nodes_to_destroy_manually;
	/* 0x0024 */	u32								m_animations_count;
	/* 0x0028 */	u32								m_animated_objects_count;
	/* 0x002c */	u32								m_interpolators_count;
	/* 0x0030 */	u32								m_buffer_size;
}; // struct n_ary_tree_converter

STATIC_SIZE_ASSERT(n_ary_tree_converter, 0x34);

} // namespace mixing
} // namespace animation
} // namespace vostok

#endif // #ifndef N_ARY_TREE_CONVERTER_H_INCLUDED
