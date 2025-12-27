////////////////////////////////////////////////////////////////////////////
//	Created		: 19.02.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef MIXING_BINARY_TREE_ANIMATION_NODE_INLINE_H_INCLUDED
#define MIXING_BINARY_TREE_ANIMATION_NODE_INLINE_H_INCLUDED

namespace vostok {
namespace animation {
namespace mixing {

inline binary_tree_animation_node::binary_tree_animation_node			( binary_tree_animation_node const& other ) :
	m_time_calculator					( other.m_time_calculator ),
	m_buffer							( other.m_buffer ),
#ifndef MASTER_GOLD
	m_identifier						( other.m_identifier ),
#endif // #ifndef MASTER_GOLD
	m_animation_intervals				( other.m_animation_intervals ),
	m_weight_interpolator				( other.m_weight_interpolator ),
	m_time_scale_interpolator			( other.m_time_scale_interpolator ),
	m_animated_object					( other.m_animated_object ),
	m_time_driving_animation			( other.m_time_driving_animation ),
	m_weight_driving_animation			( other.m_weight_driving_animation ),

	m_n_ary_animation					( other.m_n_ary_animation ),
	m_next_weight_animation				( other.m_next_weight_animation ),
	user_data							( other.user_data ),

	m_animation_intervals_count			( other.m_animation_intervals_count ),
	m_start_animation_interval_id		( other.m_start_animation_interval_id ),
	m_start_animation_interval_time		( other.m_start_animation_interval_time ),
	m_start_cycle_animation_interval_id	( other.m_start_cycle_animation_interval_id ),
	m_time_scale						( other.m_time_scale ),
	m_playback_type						( other.m_playback_type ),
	m_time_synchronization_group_id		( other.m_time_synchronization_group_id ),
	m_weight_synchronization_group_id	( other.m_weight_synchronization_group_id ),
	m_additivity_priority				( other.m_additivity_priority ),
	m_bones_mask						( other.m_bones_mask ),
	m_unique_animation_id				( other.m_unique_animation_id ),
	m_override_existing_animation		( other.m_override_existing_animation ),
	m_is_positive_event_direction		( other.m_is_positive_event_direction ),
	m_can_generate_user_defined_events	( other.m_can_generate_user_defined_events )
{
#ifndef MASTER_GOLD
	user_data							= other.user_data;
#endif // #ifndef MASTER_GOLD
}

inline float binary_tree_animation_node::time_scale						( ) const
{
	return								m_time_scale;
}

inline playback_enum binary_tree_animation_node::playback_type			( ) const
{
	return								m_playback_type;
}

inline u32 binary_tree_animation_node::time_synchronization_group_id	( ) const
{
	return								m_time_synchronization_group_id;
}

inline u32 binary_tree_animation_node::weight_synchronization_group_id	( ) const
{
	return								m_weight_synchronization_group_id;
}

inline u32 binary_tree_animation_node::additivity_priority				( ) const
{
	return								m_additivity_priority;
}

inline u32 binary_tree_animation_node::bones_mask						( ) const
{
	return								m_bones_mask;
}

inline	n_ary_tree_animation_node*		binary_tree_animation_node::n_ary_animation				( ) const
{
	return								m_n_ary_animation;
}

inline	binary_tree_animation_node_ptr	binary_tree_animation_node::time_driving_animation		( ) const
{
	return								m_time_driving_animation;
}

inline	binary_tree_animation_node_ptr	binary_tree_animation_node::weight_driving_animation	( ) const
{
	return								m_weight_driving_animation;
}

inline void binary_tree_animation_node::set_n_ary_animation					( n_ary_tree_animation_node& animation )
{
	// this is possible when single lexeme is used in several set_target function calls
//	R_ASSERT							( !m_n_ary_driving_animation );
//	R_ASSERT							( animation );
	m_n_ary_animation					= &animation;
}

inline void binary_tree_animation_node::set_time_driving_animation			( binary_tree_animation_node* time_driving_animation )
{
	// this is possible when single lexeme is used in several set_target function calls
//	R_ASSERT							( !m_time_driving_animation );
	R_ASSERT							( time_driving_animation );
	m_time_driving_animation			= time_driving_animation;
}

inline void binary_tree_animation_node::set_weight_driving_animation		( binary_tree_animation_node* weight_driving_animation )
{
	// this is possible when single lexeme is used in several set_target function calls
//	R_ASSERT							( !m_weight_driving_animation );
	R_ASSERT							( weight_driving_animation );
	m_weight_driving_animation			= weight_driving_animation;
}


inline void binary_tree_animation_node::assign_uninitialized_user_data	( )
{
#ifndef MASTER_GOLD
	user_data							= vostok::memory::uninitialized_value<u32>( );
#endif // #ifndef MASTER_GOLD
}


} // namespace mixing
} // namespace animation
} // namespace vostok

#endif // #ifndef MIXING_BINARY_TREE_ANIMATION_NODE_INLINE_H_INCLUDED