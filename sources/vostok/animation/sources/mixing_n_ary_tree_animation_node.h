////////////////////////////////////////////////////////////////////////////
//	Created		: 03.03.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef N_ARY_TREE_ANIMATION_NODE_H_INCLUDED
#define N_ARY_TREE_ANIMATION_NODE_H_INCLUDED

#include "mixing_n_ary_tree_n_ary_operation_node.h"
#include <vostok/animation/mixing.h>

namespace vostok {
namespace animation {

struct base_interpolator;

namespace mixing {

class animation_interval_base;
struct animation_state;
class animation_interval;

// sushi@TODO: Decide on location
typedef fastdelegate::FastDelegate< float( float, float, u32, u32, u32, float ) > time_calculator_fn;

class n_ary_tree_animation_node : public n_ary_tree_n_ary_operation_node {
public:

	// sushi@TODO: Document when one used instead of another
	inline									n_ary_tree_animation_node			(
#ifndef MASTER_GOLD
												pcstr const						identifier,
#endif // #ifndef MASTER_GOLD
												animation_interval const*		animation_intervals_begin,
												animation_interval const*		animation_intervals_end,
												u8								unique_animation_id,
												u32								start_cycle_animation_interval_id,
												base_interpolator const&		weight_interpolator,
												pcvoid const					animated_object,
												playback_enum					playback_type,
												time_calculator_fn const&		time_calculator,
												u32								time_synchronization_group_id,
												u32								weight_synchronization_group_id,
												bool							override_existing_animation,
												bool							is_positive_event_direction,
												bool							can_generate_user_defined_events,
												u32								additivity_priority,
												u32								bones_mask,
												u32								operands_count,
												bool							is_transitting_to_zero
											);

	inline									n_ary_tree_animation_node			(
#ifndef MASTER_GOLD
												pcstr const						identifier,
#endif // #ifndef MASTER_GOLD
												n_ary_tree_animation_node&		weight_driving_animation,
												animation_interval const*		animation_intervals_begin,
												animation_interval const*		animation_intervals_end,
												u8								unique_animation_id,
												u32								start_cycle_animation_interval_id,
												pcvoid const					animated_object,
												playback_enum					playback_type,
												time_calculator_fn const&		time_calculator,
												u32								time_synchronization_group_id,
												bool							override_existing_animation,
												bool							is_positive_event_direction,
												const bool						can_generate_user_defined_events,
												u32								additivity_priority,
												u32								bones_mask,
												u32								operands_count,
												bool							is_transitting_to_zero
											);


	// inline									n_ary_tree_animation_node			( n_ary_tree_animation_node const& other  );

	virtual	void							accept								( n_ary_tree_visitor& visitor );
	virtual void							accept								( n_ary_tree_double_dispatcher& dispatcher, n_ary_tree_base_node& node );

	//
	// setters
	//
	inline	void							set_animation_state					( mixing::animation_state& animation_state );
			void							set_time_driving_animation			( n_ary_tree_animation_node& time_driving_animation ); // sushi@TODO: In cpp impl

	//
	// accessors
	//
	inline	animation_state const*			get_animation_state					( ) const;
	inline	animation_state&				animation_state						( ) const;
	// declared unguarded to match the retail record; defined (and callable) only outside gold
	inline	pcstr							identifier							( ) const;
	inline	base_interpolator const&		weight_interpolator					( ) const;

	//
	// one-line accessors
	//
	inline	pcvoid							animated_object						( ) const { return m_animated_object				; }
	inline	animation_interval const*		animation_intervals					( ) const { return m_animation_intervals			; }
	inline	n_ary_tree_animation_node*		time_driving_animation				( ) const { return m_time_driving_animation			; }
	inline	n_ary_tree_animation_node*		weight_driving_animation			( ) const { return m_weight_driving_animation		; }
	inline	u32								time_synchronization_group_id		( ) const { return m_time_synchronization_group_id	; }
	inline	u32								weight_synchronization_group_id		( ) const { return m_weight_synchronization_group_id; }
	inline	u32								animation_intervals_count			( ) const { return m_animation_intervals_count		; }
	inline	u32								start_cycle_animation_interval_id	( ) const { return m_start_cycle_interval_id		; }
	inline	playback_enum					playback_type						( ) const { return m_playback_type					; }
	inline	time_calculator_fn const&		time_calculator						( ) const { return m_time_calculator				; }
	inline	u32								additivity_priority					( ) const { return m_additivity_priority			; }
	inline	u32								bones_mask							( ) const { return m_bones_mask						; }
	inline	u8								unique_animation_id					( ) const { return m_unique_animation_id			; }
	inline	bool							is_transitting_to_zero				( ) const { return m_is_transitting_to_zero			; }
	inline	bool							override_existing_animation			( ) const { return m_override_existing_animation	; }
	inline	bool							is_positive_event_direction			( ) const { return m_is_positive_event_direction	; }
	inline	bool							can_generate_events					( ) const { return m_can_generate_events			; }


private:
	virtual void							visit								( n_ary_tree_double_dispatcher& dispatcher, n_ary_tree_animation_node& node );
	virtual void							visit								( n_ary_tree_double_dispatcher& dispatcher, n_ary_tree_weight_node& node );
	virtual void							visit								( n_ary_tree_double_dispatcher& dispatcher, n_ary_tree_time_scale_node& node );
	virtual void							visit								( n_ary_tree_double_dispatcher& dispatcher, n_ary_tree_addition_node& node );
	virtual void							visit								( n_ary_tree_double_dispatcher& dispatcher, n_ary_tree_subtraction_node& node );
	virtual void							visit								( n_ary_tree_double_dispatcher& dispatcher, n_ary_tree_multiplication_node& node );
	virtual void							visit								( n_ary_tree_double_dispatcher& dispatcher, n_ary_tree_weight_transition_node& node );
	virtual void							visit								( n_ary_tree_double_dispatcher& dispatcher, n_ary_tree_time_scale_transition_node& node );

private:
	n_ary_tree_animation_node&				operator =							( n_ary_tree_animation_node const& other );

private:
	inline	void							assign_uninitialized_user_data		( );

private:
	/* 0x0000 */	/* n_ary_tree_n_ary_operation_node */
	/* 0x0008 */	const time_calculator_fn			m_time_calculator;
	/* 0x0010 */	animation_interval const* const		m_animation_intervals;
	/* 0x0014 */	n_ary_tree_animation_node*			m_time_driving_animation;
	/* 0x0018 */	n_ary_tree_animation_node* const	m_weight_driving_animation;
	/* 0x001c */	mixing::animation_state*			m_animation_state;
	/* 0x0020 */	base_interpolator const&			m_weight_interpolator;
#ifndef MASTER_GOLD
					pcstr const							m_identifier;
#endif // #ifndef MASTER_GOLD
	/* 0x0024 */	pcvoid const						m_animated_object;

public:
	/* 0x0028 */	n_ary_tree_animation_node*			m_next_weight_animation;
	/* 0x002c */	n_ary_tree_animation_node*			m_next_time_animation;
	/* 0x0030 */	u32									user_data;

private:
	/* 0x0034 */	u32									m_time_synchronization_group_id;
	/* 0x0038 */	const u32							m_weight_synchronization_group_id;
	/* 0x003c */	const u32							m_animation_intervals_count;
	/* 0x0040 */	const u32							m_start_cycle_interval_id;
	/* 0x0044 */	const playback_enum					m_playback_type;
	/* 0x0048 */	const u32							m_additivity_priority;
	/* 0x004c */	const u32							m_bones_mask;
	/* 0x0050 */	const u8							m_unique_animation_id;
	/* 0x0051 */	const bool							m_is_transitting_to_zero;
	/* 0x0052 */	const bool							m_override_existing_animation;
	/* 0x0053 */	const bool							m_is_positive_event_direction;
	/* 0x0054 */	const bool							m_can_generate_events;

}; // class n_ary_tree_animation_node

STATIC_SIZE_ASSERT(n_ary_tree_animation_node, 0x58);

} // namespace mixing
} // namespace animation
} // namespace vostok

#include "mixing_n_ary_tree_animation_node_inline.h"

#endif // #ifndef N_ARY_TREE_ANIMATION_NODE_H_INCLUDED
