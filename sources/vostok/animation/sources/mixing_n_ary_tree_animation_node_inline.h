////////////////////////////////////////////////////////////////////////////
//	Created		: 26.03.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef N_ARY_TREE_ANIMATION_NODE_INLINE_H_INCLUDED
#define N_ARY_TREE_ANIMATION_NODE_INLINE_H_INCLUDED

#include <vostok/animation/mixing_animation_interval.h>
#include <vostok/animation/i_editor_mixer.h>

namespace vostok {
namespace animation {
namespace mixing {

inline n_ary_tree_animation_node::n_ary_tree_animation_node						(
#ifndef MASTER_GOLD
		pcstr const identifier,
#endif // #ifndef MASTER_GOLD
		animation_interval const* const animation_intervals_begin,
		animation_interval const* const animation_intervals_end,
		u8								unique_animation_id,
		u32 const						start_cycle_animation_interval_id,
		base_interpolator const&		weight_interpolator,
		pcvoid const					animated_object,
		playback_enum					playback_type,
		time_calculator_fn const&		time_calculator,
		u32								time_synchronization_group_id,
		u32								weight_synchronization_group_id,
		bool const						override_existing_animation,
		bool							is_positive_event_direction,
		bool							can_generate_user_defined_events,
		u32 const						additivity_priority,
		u32								bones_mask,
		u32 const						operands_count,
		bool const						is_transitting_to_zero
	) :
	n_ary_tree_n_ary_operation_node	( operands_count ),
	m_time_calculator				( time_calculator ),
	m_animation_intervals			( animation_intervals_begin ),
	m_animation_intervals_count		( u32 ( animation_intervals_end - animation_intervals_begin ) ),
	m_time_driving_animation		( NULL ),
	m_weight_driving_animation		( NULL ),
	m_animation_state				( NULL ),
	m_weight_interpolator			( weight_interpolator ),
#ifndef MASTER_GOLD
	m_identifier					( identifier ),
#endif // #ifndef MASTER_GOLD
	m_animated_object				( animated_object ),
	m_next_weight_animation			( NULL ),
	m_next_time_animation			( NULL ),
	m_time_synchronization_group_id	( time_synchronization_group_id ),
	m_weight_synchronization_group_id( weight_synchronization_group_id ),
	m_start_cycle_interval_id		( start_cycle_animation_interval_id ),
	m_playback_type					( playback_type ),
	m_additivity_priority			( additivity_priority ),
	m_bones_mask					( bones_mask ),
	m_unique_animation_id			( unique_animation_id ),
	m_is_transitting_to_zero		( is_transitting_to_zero ),
	m_override_existing_animation	( override_existing_animation ),
	m_is_positive_event_direction	( is_positive_event_direction ),
	m_can_generate_events			( can_generate_user_defined_events )
{
	assign_uninitialized_user_data	( );
}

inline n_ary_tree_animation_node::n_ary_tree_animation_node						(
#ifndef MASTER_GOLD
		pcstr const						identifier,
#endif // #ifndef MASTER_GOLD
		n_ary_tree_animation_node&		time_driving_animation,			// sushi@TODO: weight_driving_animation
		animation_interval const*		animation_intervals_begin,
		animation_interval const*		animation_intervals_end,
		u8								unique_animation_id,
		u32								start_cycle_animation_interval_id,
		pcvoid const					animated_object,
		playback_enum					playback_type,
		time_calculator_fn const&		time_calculator,
		u32								time_synchronization_group_id,	// sushi@TODO: weight_synchronization_group_id
		bool							override_existing_animation,
		bool							is_positive_event_direction,
		const bool						can_generate_user_defined_events,
		u32								additivity_priority,
		u32								bones_mask,
		u32								operands_count,
		bool							is_transitting_to_zero
	) :
/*
	m_synchronization_group_id		( driving_animation.synchronization_group_id() ),
	m_start_cycle_interval_id		( driving_animation.start_cycle_animation_interval_id() )
*/
	n_ary_tree_n_ary_operation_node	( operands_count ),
	m_time_calculator				( time_calculator ),
	m_animation_intervals			( animation_intervals_begin ),
	m_animation_intervals_count		( u32 ( animation_intervals_end - animation_intervals_begin ) ),
	m_time_driving_animation		( &time_driving_animation ),						// sushi@TODO
	m_weight_driving_animation		( NULL ),											// sushi@TODO
	m_animation_state				( NULL ),
	m_weight_interpolator			( time_driving_animation.m_weight_interpolator ),	// sushi@TODO
#ifndef MASTER_GOLD
	m_identifier					( identifier ),
#endif // #ifndef MASTER_GOLD
	m_animated_object				( animated_object ),
	m_next_weight_animation			( NULL ),
	m_next_time_animation			( NULL ),
	m_time_synchronization_group_id	( time_synchronization_group_id ),					// sushi@TODO
	m_weight_synchronization_group_id( 0 ),												// sushi@TODO
	m_start_cycle_interval_id		( start_cycle_animation_interval_id ),
	m_playback_type					( playback_type ),
	m_additivity_priority			( additivity_priority ),
	m_bones_mask					( bones_mask ),
	m_unique_animation_id			( unique_animation_id ),
	m_is_transitting_to_zero		( is_transitting_to_zero ),
	m_override_existing_animation	( override_existing_animation ),
	m_is_positive_event_direction	( is_positive_event_direction ),
	m_can_generate_events			( can_generate_user_defined_events )
{
	assign_uninitialized_user_data	( );
}

/* sushi@TODO: (tiresome)
inline n_ary_tree_animation_node::n_ary_tree_animation_node						( n_ary_tree_animation_node const& other ) :
	n_ary_tree_n_ary_operation_node	( other ),
#ifndef MASTER_GOLD
	m_identifier					( other.m_identifier ),
#endif // #ifndef MASTER_GOLD
	m_driving_animation				( other.m_driving_animation ),
	m_animation_intervals			( other.m_animation_intervals ),
	m_animation_intervals_count		( other.m_animation_intervals_count ),
	m_start_cycle_interval_id		( other.m_start_cycle_interval_id ),
	m_interpolator					( other.m_interpolator ),
	m_playing_type					( other.m_playing_type ),
	m_override_existing_animation	( other.m_override_existing_animation ),
	m_additivity_priority			( other.m_additivity_priority ),
	m_is_transitting_to_zero		( other.m_is_transitting_to_zero ),
	m_animation_state				( 0 ),
	m_next_animation				( 0 ),
	m_synchronization_group_id		( other.m_synchronization_group_id )
{
	user_data						= other.user_data;
}
*/

inline animation_state const* n_ary_tree_animation_node::get_animation_state	( ) const
{
	return							m_animation_state;
}

inline animation_state&	n_ary_tree_animation_node::animation_state				( ) const
{
	R_ASSERT						( m_animation_state );
	return							*m_animation_state;
}

#ifndef MASTER_GOLD
inline pcstr n_ary_tree_animation_node::identifier								( ) const
{
	R_ASSERT						( m_identifier );
	return							m_identifier;
}
#endif // #ifndef MASTER_GOLD

inline void n_ary_tree_animation_node::set_animation_state						( mixing::animation_state& animation_state )
{
	m_animation_state				= &animation_state;
}

inline void n_ary_tree_animation_node::assign_uninitialized_user_data			( )
{
	user_data						= memory::uninitialized_value<u32>( );
}

inline base_interpolator const& n_ary_tree_animation_node::weight_interpolator	( ) const
{
	return							m_weight_interpolator;
}

} // namespace mixing
} // namespace animation
} // namespace vostok

#endif // #ifndef N_ARY_TREE_ANIMATION_NODE_INLINE_H_INCLUDED
