////////////////////////////////////////////////////////////////////////////
//	Created		: 05.03.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef N_ARY_TREE_TRANSITION_TREE_CONSTRUCTOR_H_INCLUDED
#define N_ARY_TREE_TRANSITION_TREE_CONSTRUCTOR_H_INCLUDED

#include "mixing_n_ary_tree_cloner.h"

namespace vostok {

void anchor_animation_cloner( );

namespace animation {

struct base_interpolator;
struct subscribed_channel;

namespace mixing {

class n_ary_tree;
class n_ary_tree_intrusive_base;
struct animation_state;
class animation_clip;
class animated_object_holder;

enum interpolation_direction;

class n_ary_tree_transition_tree_constructor : private boost::noncopyable {
	friend void ::vostok::anchor_animation_cloner( );

public:
	typedef boost::function<float4x4(pcvoid)> transform_functor_type;

public:
											n_ary_tree_transition_tree_constructor(
												mutable_buffer&					buffer,
												n_ary_tree const&				from,
												n_ary_tree const&				to,
												u32 const						animations_count,
												u32 const						animated_objects_count,
												u32 const						current_time_in_ms,
												subscribed_channel*&			channels_head,
												transform_functor_type const&	get_transform_functor
											);

			n_ary_tree						computed_tree						( );

private:
			n_ary_tree_animation_node*		add_animation_node					(
												n_ary_tree_animation_node&		new_animation,
												animation_state const*			previous_animation_state,
												u32								animation_interval_id,
												float							animation_interval_time,
												bool							is_new_animation
											);

			n_ary_tree_animation_node*		new_animation						(
												n_ary_tree_animation_node&		to,
												n_ary_tree_animation_node&		from,
												n_ary_tree_animation_node*		weight_driving_animation,
												u32								weight_operands_count,
												u32&							time_scale_operands_count,
												u32&							operands_offset,
												u32&							animation_interval_id,
												float&							animation_interval_time,
												bool							is_transitting_to_zero,
												bool							can_be_time_driving_animation
											);

			n_ary_tree_base_node*			new_time_scale						( n_ary_tree_animation_node& new_time_driving_animation, u32& animation_interval_id, float& animation_interval_time );
			n_ary_tree_base_node*			new_time_scale_transition			(
												n_ary_tree_animation_node&		from_animation,
												n_ary_tree_animation_node&		to_animation,
												n_ary_tree_base_node&			from,
												n_ary_tree_base_node&			to
											);
			n_ary_tree_base_node*			new_time_scale_transition			( n_ary_tree_animation_node& from_animation, n_ary_tree_base_node& from, float to );
			n_ary_tree_base_node*			new_time_scale_transition			( float animation_time, float from, n_ary_tree_base_node& to );

			n_ary_tree_base_node*			new_weight_transition				( base_interpolator const& interpolator, float from, float to );
			n_ary_tree_base_node*			new_weight_transition				( n_ary_tree_base_node& from, n_ary_tree_base_node& to );
			n_ary_tree_base_node*			new_weight_transition				( base_interpolator const& from_animation_interpolator, n_ary_tree_base_node& from, float to );
			n_ary_tree_base_node*			new_weight_transition				( base_interpolator const& to_animation_interpolator, float from, n_ary_tree_base_node& to );

			void							add_operands						(
												n_ary_tree_animation_node&		from,
												n_ary_tree_animation_node&		to,
												n_ary_tree_base_node**			operands_begin,
												n_ary_tree_base_node**			operands_end,
												bool							skip_time_scale_node
											);

			n_ary_tree_animation_node*		add_animation						( n_ary_tree_animation_node& animation, n_ary_tree_animation_node* const weight_driving_animation );
			n_ary_tree_animation_node*		remove_animation					( n_ary_tree_animation_node& animation, n_ary_tree_animation_node* const weight_driving_animation, bool is_new_driving_animation );
			void							change_animation					(
												n_ary_tree_animation_node&		from,
												n_ary_tree_animation_node&		to,
												n_ary_tree_animation_node* const	weight_driving_animation,
												bool							is_new_driving_animation
											);

			void							merge_weight_asynchronous_groups	(
												n_ary_tree_animation_node* const	from_begin,
												n_ary_tree_animation_node* const	from_end,
												n_ary_tree_animation_node* const	to_begin,
												n_ary_tree_animation_node* const	to_end
											);
			void							merge_weight_synchronization_groups	(
												n_ary_tree_animation_node*		from_begin,
												n_ary_tree_animation_node*		from_end,
												n_ary_tree_animation_node*		to_begin,
												n_ary_tree_animation_node*		to_end,
												n_ary_tree_animation_node&		new_weight_driving_animation,
												bool							is_new_driving_animation
											);

			n_ary_tree_animation_node*		new_weight_driving_animation		( n_ary_tree_animation_node& animation );
			n_ary_tree_animation_node*		new_weight_driving_animation		( n_ary_tree_animation_node& new_weight_driving_animation, n_ary_tree_animation_node& new_driving_animation_in_previous_target );

			void							add_weight_synchronization_group	( n_ary_tree_animation_node* begin, n_ary_tree_animation_node* end );
			void							remove_weight_synchronization_group	( n_ary_tree_animation_node* begin, n_ary_tree_animation_node* end );
			void							change_weight_synchronization_group	(
												n_ary_tree_animation_node*		from_begin,
												n_ary_tree_animation_node*		from_end,
												n_ary_tree_animation_node*		to_begin,
												n_ary_tree_animation_node*		to_end
											);

			n_ary_tree_animation_node*		get_time_driving_animation			( u32 time_synchronization_group_id ) const;

			void							merge_trees							( n_ary_tree const& from, n_ary_tree const& to );

public:
	inline	void							advance_buffer						( u32 size ) { m_buffer += size; }

public:
	/* 0x0000 */	transform_functor_type			m_get_transform_functor;
	/* 0x0020 */	n_ary_tree_cloner				m_cloner;
	/* 0x0044 */	mutable_buffer&					m_buffer;
	/* 0x0048 */	n_ary_tree const&				m_from;
	/* 0x004c */	n_ary_tree const&				m_to;
	/* 0x0050 */	n_ary_tree_animation_node*		m_weight_root;
	/* 0x0054 */	n_ary_tree_animation_node*		m_time_root;
	/* 0x0058 */	n_ary_tree_animation_node**		m_time_driving_animations_begin;
	/* 0x005c */	n_ary_tree_animation_node**		m_time_driving_animations_end;
	/* 0x0060 */	animation_state*				m_animation_states;
	/* 0x0064 */	animation_state*				m_new_animation_state;
	/* 0x0068 */	animation_state**				m_animation_events;
	/* 0x006c */	animated_object_holder*			m_animated_objects;
	/* 0x0070 */	animated_object_holder*			m_new_animated_object;
	/* 0x0074 */	animation_state**				m_new_animation_event;
	/* 0x0078 */	n_ary_tree_intrusive_base*		m_reference_counter;
	/* 0x007c */	n_ary_tree_animation_node*		m_previous_animation;
	/* 0x0080 */	subscribed_channel*&			m_channels_head;
	/* 0x0084 */	u32								m_current_time_in_ms;
	/* 0x0088 */	u32								m_animations_count;
	/* 0x008c */	u32								m_animated_objects_count;
}; // class n_ary_tree_transition_tree_constructor

STATIC_SIZE_ASSERT(n_ary_tree_transition_tree_constructor, 0x90);

} // namespace mixing
} // namespace animation
} // namespace vostok

#endif // #ifndef N_ARY_TREE_TRANSITION_TREE_CONSTRUCTOR_H_INCLUDED
