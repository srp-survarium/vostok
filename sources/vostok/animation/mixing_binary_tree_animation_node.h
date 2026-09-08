////////////////////////////////////////////////////////////////////////////
//	Created		: 19.02.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef MIXING_BINARY_TREE_ANIMATION_NODE_H_INCLUDED
#define MIXING_BINARY_TREE_ANIMATION_NODE_H_INCLUDED

#include <vostok/animation/mixing_binary_tree_base_node.h>
#include <vostok/animation/mixing.h>
#include <vostok/animation/instant_interpolator.h>
#include <vostok/animation/mixing_animation_interval.h>

namespace vostok {
namespace animation {
namespace mixing {

class channel_event_callback;
class binary_tree_animation_node;
class n_ary_tree_animation_node;

} // namespace mixing
} // namespace animation

template class VOSTOK_ANIMATION_API
	intrusive_ptr<
		animation::mixing::binary_tree_animation_node,
		animation::mixing::binary_tree_base_node,
		threading::single_threading_policy
	>;

namespace animation {
namespace mixing {

class animation_lexeme_parameters;
class animation_interval;
class binary_tree_initializer;
class n_ary_tree_converter;

typedef intrusive_ptr<
	binary_tree_animation_node,
	binary_tree_base_node,
	threading::single_threading_policy
> binary_tree_animation_node_ptr;

class VOSTOK_ANIMATION_API binary_tree_animation_node : public binary_tree_base_node {

public:
			explicit							binary_tree_animation_node	( animation_lexeme_parameters const& parameters );
	inline										binary_tree_animation_node	( binary_tree_animation_node const& other );


	inline	mutable_buffer&						buffer								( ) const { return *m_buffer; }
	inline	float								time_scale							( ) const;
	inline	playback_enum						playback_type						( ) const;
	inline	u32									time_synchronization_group_id		( ) const;
	inline	u32									weight_synchronization_group_id		( ) const;
	inline	u32									additivity_priority					( ) const;
	inline	u32									bones_mask							( ) const;

	inline	n_ary_tree_animation_node*			n_ary_animation						( ) const;
	inline	binary_tree_animation_node_ptr		time_driving_animation				( ) const;
	inline	binary_tree_animation_node_ptr		weight_driving_animation			( ) const;
	inline	void								set_n_ary_animation					( n_ary_tree_animation_node& animation );
	inline	void								set_time_driving_animation			( binary_tree_animation_node* animation_to_syynchronize_with );
	inline	void								set_weight_driving_animation		( binary_tree_animation_node* animation_to_syynchronize_with );

	inline	base_interpolator const*			weight_interpolator					( ) const { R_ASSERT(m_weight_interpolator); return m_weight_interpolator; }
	inline	base_interpolator const*			time_scale_interpolator				( ) const { return m_time_scale_interpolator; }

	inline	pcvoid								animated_object						( ) const { return m_animated_object; }

#ifndef MASTER_GOLD
	inline	pcstr								identifier							( ) const { return m_identifier; }
#endif // #ifndef MASTER_GOLD

	inline	u32									start_animation_interval_id			( ) const { return m_start_animation_interval_id; }
	inline	float								start_animation_interval_time		( ) const { return m_start_animation_interval_time; }
	inline	bool								override_existing_animation			( ) const { return m_override_existing_animation; }

	inline	bool								is_positive_event_direction			( ) const { return m_is_positive_event_direction; }
	inline	bool								can_generate_user_defined_events	( ) const { return m_can_generate_user_defined_events; }


public:
	inline	animation_interval const*			animation_intervals_begin			( ) const { return m_animation_intervals; }
	inline	animation_interval const*			animation_intervals_end				( ) const { return m_animation_intervals + m_animation_intervals_count; }
	inline	u32									animation_intervals_count			( ) const { return m_animation_intervals_count; }

	inline	u8									unique_animation_id					( ) const { return m_unique_animation_id; }
	inline	u32									start_cycle_animation_interval_id	( ) const { return m_start_cycle_animation_interval_id; }
	// sushi@TODO: typedef
	inline	fastdelegate::FastDelegate< float( float, float, u32, u32, u32, float ) > const&	time_calculator						( ) const { return m_time_calculator; }

public:
	virtual	void					accept						( binary_tree_visitor& visitor );

protected:
			void					destroy_animation_intervals	( );

private:
	friend class binary_tree_initializer;
	friend class n_ary_tree_converter;

	inline	binary_tree_animation_node& operator=				( binary_tree_animation_node const& other );
	inline	void				assign_uninitialized_user_data	( );

private:
	// sushi@TODO
	// sushi@TODO: Moved before clone for whatever reason
	static	u32									animation_intervals_count			( skeleton_animation_ptr const& animation );

	static	animation_interval const*			clone								(
													mutable_buffer&					buffer,
													animation_interval const*		animation_intervals_begin,
													animation_interval const*		animation_intervals_end
												);
	static	animation_interval const*			clone								(
													mutable_buffer& arg_0,
													skeleton_animation_ptr const& animation
												);
#ifndef MASTER_GOLD
	static	pcstr								clone						(
													mutable_buffer& buffer,
													pcstr identifier
												);
#endif // #ifndef MASTER_GOLD

	static	u32									get_animation_interval_id			( skeleton_animation_ptr const& animation, float start_animation_time );
	static	float								get_animation_interval_time			( skeleton_animation_ptr const& animation, u32 start_interval_id, float start_animation_time );

private:
	// sushi@TODO: Add typedef
	/* 0x0014 */	fastdelegate::FastDelegate< float( float, float, u32, u32, u32, float ) >	m_time_calculator;
	/* 0x001c */	mutable_buffer* const				m_buffer;
#ifndef MASTER_GOLD
	pcstr const											m_identifier;
#endif // #ifndef MASTER_GOLD
	/* 0x0020 */	animation_interval const* const		m_animation_intervals;
	/* 0x0024 */	base_interpolator const*			m_weight_interpolator;
	/* 0x0028 */	base_interpolator const*			m_time_scale_interpolator;
	/* 0x002c */	pcvoid const						m_animated_object;
	/* 0x0030 */	binary_tree_animation_node*			m_time_driving_animation;
	/* 0x0034 */	binary_tree_animation_node*			m_weight_driving_animation;
	/* 0x0038 */	n_ary_tree_animation_node*			m_n_ary_animation;

public:
	/* 0x003c */	binary_tree_animation_node_ptr		m_next_weight_animation;
	/* 0x0040 */	u32									m_unique_weights_count;
	/* 0x0044 */	u32									user_data;

private:
	/* 0x0048 */	const u32							m_animation_intervals_count;
	/* 0x004c */	const u32							m_start_animation_interval_id;
	/* 0x0050 */	const float							m_start_animation_interval_time;
	/* 0x0054 */	const u32							m_start_cycle_animation_interval_id;
	/* 0x0058 */	const float							m_time_scale;
	/* 0x005c */	const playback_enum					m_playback_type;
	/* 0x0060 */	const u32							m_time_synchronization_group_id;
	/* 0x0064 */	const u32							m_weight_synchronization_group_id;
	/* 0x0068 */	const u32							m_additivity_priority;
	/* 0x006c */	const u32							m_bones_mask;
	/* 0x0070 */	const u8							m_unique_animation_id;
	/* 0x0071 */	const bool							m_override_existing_animation;
	/* 0x0072 */	const bool							m_is_positive_event_direction;
	/* 0x0073 */	const bool							m_can_generate_user_defined_events;

public:
	/* 0x0074 */	bool								m_null_weight_found;
}; // class binary_tree_animation_node

STATIC_SIZE_ASSERT(binary_tree_animation_node, 0x78);

} // namespace mixing
} // namespace animation
} // namespace vostok

#include <vostok/animation/mixing_binary_tree_animation_node_inline.h>

#endif // #ifndef MIXING_BINARY_TREE_ANIMATION_NODE_H_INCLUDED
