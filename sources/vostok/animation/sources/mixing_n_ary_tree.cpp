////////////////////////////////////////////////////////////////////////////
//	Created		: 21.09.2011
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/animation/mixing_n_ary_tree.h>

#include "mixing_n_ary_tree_destroyer.h"
#include "mixing_n_ary_tree_animation_node.h"
#include "mixing_n_ary_tree_comparer.h"
#include "mixing_n_ary_tree_time_scale_node.h"
#include <vostok/animation/base_interpolator.h>
#include "mixing_n_ary_tree_dumper.h"
#include "bone_matrices_computer.h"
#include "mixing_n_ary_tree_event_iterator.h"
#include "mixing_n_ary_tree_animation_time_calculator.h"
#include "mixing_n_ary_tree_weight_calculator.h"
#include "mixing_n_ary_tree_time_scale_calculator.h"
#include "mixing_n_ary_tree_time_scale_start_time_modifier.h"
#include <vostok/animation/animation_callback.h>
#include "i_editor_mixer_inline.h"
#include "mixing_animation_state.h"
#include <vostok/animation/animation_player.h>
#include <vostok/animation/skeleton.h>
#include <vostok/animation/mixing_animated_object_holder.h>

namespace vostok {
namespace animation {
namespace mixing {

struct event_iterator_predicate {
	inline bool operator()( animation_state const* const left, animation_state const* const right ) const
	{
		return					left->event_iterator.is_less( right->event_iterator );
	}
};

 n_ary_tree::n_ary_tree(
	n_ary_tree_animation_node* const	weight_root,
	n_ary_tree_animation_node* const	time_root,
	base_interpolator const** const		interpolators,
	animation_state* const				animation_states,
	animation_state** const				animation_events,
	animated_object_holder* const		animated_objects,
	n_ary_tree_intrusive_base* const	reference_counter,
	const u32							animations_count,
	const u32							animated_objects_count,
	const u32							interpolators_count,
	const u32							current_time_in_ms
) :
	m_reference_counter		( reference_counter ),
	m_weight_root			( weight_root ),
	m_time_root				( time_root ),
	m_interpolators			( interpolators ),
	m_animation_states		( animation_states ),
	m_animation_events		( animation_events ),
	m_animated_objects		( animated_objects ),
	m_animations_count		( animations_count ),
	m_animated_objects_count( animated_objects_count ),
	m_interpolators_count	( interpolators_count ),
	m_tree_actual_time_in_ms( current_time_in_ms ),
	m_is_logging_enabled	( false )
{
	initialize					( );
}

void n_ary_tree::initialize( )
{
	stlp_std::sort				(
		m_animation_events,
		m_animation_events + m_animations_count,
		event_iterator_predicate( )
	);

	for ( animation_state* i = m_animation_states, * const end = i + m_animations_count; i != end; ++i )
		if ( (*i->event_iterator).event_type & time_event_weight_transitions_started )
			set_object_transform	( i->event_iterator.animation( ) );
}

n_ary_tree& n_ary_tree::operator=( n_ary_tree const& other )
{
	if ( this == &other )
		return *this;

	destroy						( );

	m_reference_counter			= other.m_reference_counter;
	m_weight_root				= other.m_weight_root;
	m_time_root					= other.m_time_root;
	m_interpolators				= other.m_interpolators;
	m_animation_states			= other.m_animation_states;
	m_animation_events			= other.m_animation_events;
	m_animated_objects			= other.m_animated_objects;
	m_animations_count			= other.m_animations_count;
	m_animated_objects_count	= other.m_animated_objects_count;
	m_interpolators_count		= other.m_interpolators_count;
	m_tree_actual_time_in_ms	= other.m_tree_actual_time_in_ms;
	m_is_logging_enabled		= other.m_is_logging_enabled;

	return						*this;
}

 n_ary_tree::~n_ary_tree( )
{
	destroy						( );
}

void n_ary_tree::destroy( )
{
	if ( !m_weight_root )
		return;

	if ( m_reference_counter->reference_count() > 1 ) {
		m_reference_counter		= 0;
		return;
	}

	n_ary_tree_destroyer		tree_destroyer;
	for ( n_ary_tree_animation_node* i = m_weight_root; i; i = i->m_next_weight_animation )
		i->accept				( tree_destroyer );

	m_weight_root				= 0,
	m_time_root					= 0;

	for ( base_interpolator const* const* i = m_interpolators, * const* const e = i + m_interpolators_count; i != e; ++i )
		(*i)->~base_interpolator( );

	animation_state* i			= m_animation_states;
	animation_state* const e	= m_animation_states + m_animations_count;
	for ( ; i != e; ++i )
		(*i).~animation_state	( );
}

float4x4 n_ary_tree::get_object_transform( pcvoid const animated_object ) const
{
	animated_object_holder const* const holder	=
		std::find( m_animated_objects, m_animated_objects + m_animated_objects_count, animated_object );
	if ( m_animation_states )
		bone_matrices_computer( animated_object, 0, m_animation_states, m_animations_count )
			.get_object_transform( );
	return m_animation_states ?
		bone_matrices_computer( animated_object, 0, m_animation_states, m_animations_count )
			.get_object_transform( ) * holder->transform : holder->transform;
}

void n_ary_tree::set_object_transform( n_ary_tree_animation_node& animation_node )
{
	animation_state& state					= animation_node.animation_state( );
	animation_interval const& interval		= animation_node.animation_intervals( )[ state.animation_interval_id ];
	current_frame_position frame_position;
	frame frame_transform;
	{
		cubic_spline_skeleton_animation_pinned pinned_animation( interval.animation( ) );
		frame_transform = pinned_animation->bone( u32( 0 ) ).bone_frame(
			state.animation_time * default_fps,
			frame_position
		);
	}

	object_movement& previous				= state.bone_matrices_computer.previous_object_movement;
	object_movement& accumulated			= state.bone_matrices_computer.accumulated_object_movement;
	if ( state.are_there_any_weight_transitions ) {
		previous.translation				= frame_transform.translation;
		previous.rotation					= math::quaternion( frame_transform.rotation );
		previous.scale						= frame_transform.scale;

		accumulated.translation				= float3( 0.f, 0.f, 0.f );
		accumulated.rotation				= math::quaternion( float3( 0.f, 0.f, 0.f ) );
		accumulated.scale					= float3( 1.f, 1.f, 1.f );
		return;
	}

	accumulated.translation					= frame_transform.translation;
	accumulated.rotation					= math::quaternion( frame_transform.rotation );
	accumulated.scale						= frame_transform.scale;
}

void n_ary_tree::set_object_transform( pcvoid const animated_object, float4x4 const& object_transform )
{
	for ( n_ary_tree_animation_node* i = m_weight_root; i; i = i->m_next_weight_animation )
		if ( i->animated_object( ) == animated_object )
			set_object_transform	( *i );

	animated_object_holder* const j = std::find( m_animated_objects, m_animated_objects + m_animated_objects_count, animated_object );
	j->transform				= object_transform;
}

void n_ary_tree::set_objects_transform( )
{
	for ( animated_object_holder* i = m_animated_objects, * const e = i + m_animated_objects_count; i != e; ++i )
		set_object_transform		( i->animated_object, get_object_transform( i->animated_object ) );
}

void n_ary_tree::compute_bones_matrices(
	pcvoid const		animated_object,
	skeleton const&		skeleton,
	float4x4* const		begin,
	float4x4* const		end,
	u32* const			bones_masks
) const
{
	bone_matrices_computer( animated_object, &skeleton, m_animation_states, m_animations_count ).compute_bones_matrices( begin, end, bones_masks );
}

void n_ary_tree::compute_bones_local_matrices(
	pcvoid				animated_object,
	skeleton const&		skeleton,
	float4x4* const		begin,
	float4x4* const		end,
	u32* const			bones_masks
) const
{
	bone_matrices_computer computer( animated_object, &skeleton, m_animation_states, m_animations_count );
	computer.compute_bones_local_matrices( begin, end, bones_masks );
}

void n_ary_tree::convert_to_object_matrices(
	pcvoid				animated_object,
	skeleton const&		skeleton,
	float4x4* const		begin,
	float4x4* const		end
) const
{
	bone_matrices_computer( animated_object, &skeleton, m_animation_states, m_animations_count ).convert_to_object_matrices( begin, end );
}

void n_ary_tree::accumulate_object_movement(
	n_ary_tree_animation_node&		animation_node,
	const float						animation_interval_time,
	const u32						time_in_ms
)
{
	animation_state& animation_state		= animation_node.animation_state( );
	n_ary_tree_weight_calculator weight_calculator( time_in_ms, &animation_node );
	animation_node.accept					( weight_calculator );
	float const weight						= weight_calculator.weight( );
	animation_state.weight					= weight;
	animation_state.animation_interval_time	= animation_interval_time;
	update_animation_time					( animation_state );

	animation_interval const* animation_interval	=
		animation_node.animation_intervals( ) + animation_state.animation_interval_id;
	cubic_spline_skeleton_animation_pinned pinned_animation	=
		cubic_spline_skeleton_animation_pinned( animation_interval->animation( ) );

	current_frame_position frame_position;
	frame const& frame_transform			=
		pinned_animation->bone( u32( 0 ) ).bone_frame(
		animation_state.animation_time * default_fps,
		frame_position
	);

	math::quaternion const& frame_transform_rotation	= math::quaternion( frame_transform.rotation );

	object_movement& previous_object_movement		=
		animation_state.bone_matrices_computer.previous_object_movement;
	object_movement& accumulated_object_movement	=
		animation_state.bone_matrices_computer.accumulated_object_movement;

	accumulated_object_movement.translation	+=
		frame_transform.translation - previous_object_movement.translation;
	accumulated_object_movement.rotation	=
		frame_transform_rotation * math::conjugate( previous_object_movement.rotation ) *
		accumulated_object_movement.rotation;
	accumulated_object_movement.scale		*=
		frame_transform.scale / previous_object_movement.scale;

	previous_object_movement.translation	= frame_transform.translation;
	previous_object_movement.rotation		= frame_transform_rotation;
	previous_object_movement.scale			= frame_transform.scale;
}

void n_ary_tree::update_synchronization_group_using_integration(
	n_ary_tree_animation_node&		animation_node,
	const u32						start_time_in_ms,
	const u32						target_time_in_ms
)
{
	animation_state& animation_state		= animation_node.animation_state( );
	n_ary_tree_base_node* time_scale_node	=
		animation_node.operands_count( )
			? *animation_node.operands( sizeof( n_ary_tree_animation_node ) )
			: 0;
	bool is_time_scale_node					= time_scale_node && time_scale_node->is_time_scale( );
	float accumulated_animation_time		= animation_state.animation_interval_time;
	animation_interval const& interval		=
		animation_node.animation_intervals( )[ animation_state.animation_interval_id ];
	float const animation_interval_length	= interval.length( );

	u32 const integration_interval_length_in_ms	= 10;
	u32 const full_intervals_count				=
		( target_time_in_ms - start_time_in_ms ) / integration_interval_length_in_ms;

	for ( u32 i = 0; i <= full_intervals_count; ++i ) {
		n_ary_tree_time_scale_calculator time_scale_calculator(
			i < full_intervals_count ? start_time_in_ms + ( 2*i + 1 )*( integration_interval_length_in_ms/2 ) : target_time_in_ms,
			accumulated_animation_time,
			i < full_intervals_count ? start_time_in_ms + ( i ? 2*i - 1 : 0 )*( integration_interval_length_in_ms/2 ) : start_time_in_ms + full_intervals_count*integration_interval_length_in_ms,
			&animation_node
		);

		if ( is_time_scale_node ) {
			u32 const operands_count		= animation_node.operands_count( );
			time_scale_node->accept		( time_scale_calculator );
			time_scale_node				= time_scale_calculator.result( ) ? time_scale_calculator.result( ) : time_scale_node;
			if ( operands_count != animation_node.operands_count( ) ) {
				time_scale_node			= 0;
				is_time_scale_node		= false;
			}
		}

		accumulated_animation_time		= computed_animation_time(
			animation_node,
			accumulated_animation_time,
			start_time_in_ms + i*integration_interval_length_in_ms,
			i < full_intervals_count ? start_time_in_ms + ( i + 1 )*integration_interval_length_in_ms : target_time_in_ms,
			start_time_in_ms + i*integration_interval_length_in_ms,
			is_time_scale_node ? time_scale_calculator.time_scale( ) : 1.f
		);
		accumulated_animation_time		=
			math::min( math::max( accumulated_animation_time, 0.f ), animation_interval_length );

		u32 const update_time			=
			i < full_intervals_count ? start_time_in_ms + ( i + 1 )*integration_interval_length_in_ms : target_time_in_ms;
		if ( animation_state.are_there_any_weight_transitions )
			accumulate_object_movement	( animation_node, accumulated_animation_time, update_time );

		u32 const time_synchronization_group_id	= animation_node.time_synchronization_group_id( );
		if ( time_synchronization_group_id == u32( -1 ) )
			continue;

		for (
			n_ary_tree_animation_node* current = animation_node.m_next_time_animation;
			current && current->time_synchronization_group_id( ) == time_synchronization_group_id;
			current = current->m_next_time_animation
		) {
			mixing::animation_state& current_state	= current->animation_state( );
			if ( !current_state.are_there_any_weight_transitions )
				continue;

			animation_interval const& current_interval	=
				current->animation_intervals( )[ current_state.animation_interval_id ];
			float const driven_time_scale	=
				current_interval.length( ) / animation_interval_length;
			accumulate_object_movement(
				*current,
				accumulated_animation_time * driven_time_scale,
				update_time
			);
		}
	}

}

void n_ary_tree::update_animation_time( animation_state& animation_state )
{
	if ( animation_state.is_freezed )
		return;

	animation_interval const& interval	= animation_state.event_iterator.animation( ).animation_intervals( )[ animation_state.animation_interval_id ];
	float const animation_time			= interval.start_time( ) + animation_state.animation_interval_time;
	float const animation_length		= cubic_spline_skeleton_animation_pinned( interval.animation( ) )->length_in_frames( ) / default_fps;

	animation_state.animation_time		= math::min(
		math::max( animation_time - animation_state.animation_time_threshold, 0.f ),
		animation_length
	);
}

float n_ary_tree::computed_animation_time(
	n_ary_tree_animation_node&	animation,
	const float					animation_time_before_scale_starts,
	const u32					time_scale_start_time_in_ms,
	const u32					current_time_in_ms,
	const u32					target_time_in_ms,
	const float					time_scale
) const
{
	if ( !animation.time_calculator() )
		return						animation_time_before_scale_starts + (current_time_in_ms - time_scale_start_time_in_ms)*time_scale/1000.f;

	return						animation.time_calculator()(
										animation_time_before_scale_starts,
										animation.animation_intervals()->length(),
										time_scale_start_time_in_ms,
										current_time_in_ms,
										target_time_in_ms,
										time_scale
									);
}

void n_ary_tree::update_animation_state(
	n_ary_tree_animation_node&		animation_node,
	const u32						start_time_in_ms,
	const u32						target_time_in_ms
)
{
	animation_state& state		= animation_node.animation_state( );
	n_ary_tree_weight_calculator weight_calculator( target_time_in_ms, &animation_node );
	weight_calculator.visit		( animation_node );
	state.weight				= weight_calculator.weight( );


	if ( state.is_freezed ) return;


	n_ary_tree_animation_node* const driving_animation	= animation_node.time_driving_animation( ); if ( !driving_animation ) {
		state.animation_interval_time	=
			n_ary_tree_animation_time_calculator(
				animation_node,
				start_time_in_ms,
				state.animation_interval_time,
				target_time_in_ms,
				false
			).animation_time( );
	}
	else {
		animation_state const& driving_state	= driving_animation->animation_state( );
		animation_interval const& driving_interval	= driving_animation->animation_intervals( )[ driving_state.animation_interval_id ];
		animation_interval const& interval	= animation_node.animation_intervals( )[ state.animation_interval_id ];
		state.animation_interval_time	= interval.length( ) / driving_interval.length( ) * driving_state.animation_interval_time;
	}

	update_animation_time		( state );
}

void n_ary_tree::update_time_synchronization_group(
	n_ary_tree_animation_node&		animation_node,
	const u32						start_time_in_ms,
	const u32						target_time_in_ms
)
{
	animation_state& state					= animation_node.animation_state( );
	bool are_there_any_weight_transitions	= state.are_there_any_weight_transitions;
	u32 const synchronization_group_id		= animation_node.time_synchronization_group_id( );
	if ( !are_there_any_weight_transitions && synchronization_group_id != u32( -1 ) ) {
		for (
			n_ary_tree_animation_node* current = animation_node.m_next_time_animation;
			current && current->time_synchronization_group_id( ) == synchronization_group_id;
			current = current->m_next_time_animation
		) {

			if ( !current->animation_state( ).are_there_any_weight_transitions )
				continue;

			are_there_any_weight_transitions	= true;
			break;
		}
	}

	if ( are_there_any_weight_transitions )
		update_synchronization_group_using_integration	( animation_node, start_time_in_ms, target_time_in_ms );
	else
		update_animation_state							( animation_node, start_time_in_ms, target_time_in_ms );

	if ( synchronization_group_id == u32( -1 ) )
		return;

	n_ary_tree_animation_node* current	=
		are_there_any_weight_transitions ? &animation_node : animation_node.m_next_time_animation;
	for ( ; current && current->time_synchronization_group_id( ) == synchronization_group_id;
		current = current->m_next_time_animation
	) {

		if ( current->animation_state( ).are_there_any_weight_transitions )
			continue;

		update_animation_state	( *current, start_time_in_ms, target_time_in_ms );
	}
}

void n_ary_tree::update_animation_states( const u32 start_time_in_ms, const u32 target_time_in_ms )
{
	for ( n_ary_tree_animation_node* i = m_time_root; i; i = i->m_next_time_animation )
		if ( !i->time_driving_animation( ) )
			update_time_synchronization_group( *i, start_time_in_ms, target_time_in_ms );
}

bool n_ary_tree::need_new_transform( const u32 target_time_in_ms ) const
{
	bool need_new_transform		= false;
	for ( n_ary_tree_animation_node* i = m_weight_root; i; i = i->m_next_weight_animation )
		if ( i->animation_state( ).event_iterator->event_time_in_ms == target_time_in_ms )
			if ( i->animation_state( ).event_iterator->event_type & time_event_need_new_object_transform ) {
				animated_object_holder* const animated_object = std::find( m_animated_objects, m_animated_objects + m_animated_objects_count, i->animated_object( ) );

				animated_object->need_new_transform = true;
				need_new_transform	= true;
			}

	return						need_new_transform;
}

void n_ary_tree::remove_animation( n_ary_tree_animation_node*& i, n_ary_tree_animation_node* j )
{
	n_ary_tree_animation_node** time_link	= &m_time_root;
	while ( *time_link && *time_link != i )
		time_link							= &( *time_link )->m_next_time_animation;
	if ( *time_link )
		*time_link							= ( *time_link )->m_next_time_animation;

	if ( j )
		j->m_next_weight_animation			= i->m_next_weight_animation;
	else
		m_weight_root						= i->m_next_weight_animation;

	n_ary_tree_destroyer destroyer;
	i->accept								( destroyer );
	i										= j ? j->m_next_weight_animation : m_weight_root;
	--m_animations_count;

}

void n_ary_tree::process_event( n_ary_tree_animation_node& current_animation_node, const u32 event_types )
{
	animation_state& state		= current_animation_node.animation_state( );
	u32 const event_type		= state.event_iterator->event_type & event_types;

	if ( event_type & time_event_animation_lexeme_started ) {
		u32 const event_time_in_ms	= state.event_iterator->event_time_in_ms;
		n_ary_tree_time_scale_calculator time_scale_calculator(
			event_time_in_ms,
			state.animation_interval_time,
			event_time_in_ms
		);
		n_ary_tree_animation_node* const driving_animation	=
			current_animation_node.time_driving_animation( )
				? current_animation_node.time_driving_animation( )
				: &current_animation_node;
		n_ary_tree_base_node* const time_scale_node	=
			driving_animation->operands_count( )
				? *driving_animation->operands( sizeof( n_ary_tree_animation_node ) )
				: 0;
		if ( time_scale_node && time_scale_node->is_time_scale( ) )
			time_scale_node->accept( time_scale_calculator );

		float time_scale	= time_scale_node && time_scale_node->is_time_scale( )
			? time_scale_calculator.time_scale( )
			: 1.f;
		if ( time_scale == 0.f ) {
			n_ary_tree_time_scale_calculator time_scale_calculator(
				event_time_in_ms + 1,
				state.animation_interval_time,
				event_time_in_ms + 1
			);
			time_scale_node->accept( time_scale_calculator );
			time_scale		= time_scale_calculator.time_scale( );
		}

		if ( time_scale >= 0.f ) {
			animation_interval const& interval	=
				current_animation_node.animation_intervals( )[ state.animation_interval_id ];
			float const animation_time	= interval.start_time( ) + state.animation_interval_time;
			float const animation_length	=
				cubic_spline_skeleton_animation_pinned( interval.animation( ) )->length_in_frames( ) / default_fps;
			if ( animation_time == animation_length )
				state.animation_time	= 0.f;
		}

		state.are_there_any_weight_transitions	= false;
		set_object_transform						( current_animation_node );
	}

	if ( event_type & time_event_animation_ended_in_positive_direction ) {
		if ( current_animation_node.playback_type( ) != play_once_and_freeze_at_end ) {
			state.animation_time	= 0.f;
			if ( !( event_type & time_event_animation_interval_ended ) ) {
				animation_interval const& interval	=
					current_animation_node.animation_intervals( )[ state.animation_interval_id ];
				state.animation_time_threshold	=
					cubic_spline_skeleton_animation_pinned( interval.animation( ) )->length_in_frames( ) / default_fps;
			}
		}
		else {
			animation_interval const& interval	=
				current_animation_node.animation_intervals( )[ current_animation_node.animation_intervals_count( ) - 1 ];
			float const animation_length	=
				cubic_spline_skeleton_animation_pinned( interval.animation( ) )->length_in_frames( ) / default_fps;
			state.animation_time			= animation_length;
			state.animation_time_threshold	= animation_length;
			state.animation_interval_time	= animation_length - interval.start_time( );
			state.is_freezed				= true;
		}
	}

	if ( event_type & time_event_animation_ended_in_negative_direction ) {
		animation_interval const& interval	=
			current_animation_node.animation_intervals( )[ state.animation_interval_id ];
		if ( current_animation_node.playback_type( ) != play_once_and_freeze_at_end ) {
			state.animation_time	=
				cubic_spline_skeleton_animation_pinned( interval.animation( ) )->length_in_frames( ) / default_fps;
			if ( !( event_type & time_event_animation_interval_ended ) )
				state.animation_time_threshold	= 0.f;
		}
		else {
			state.animation_time			= interval.start_time( );
			state.animation_interval_time	= 0.f;
			state.is_freezed				= true;
			state.animation_time_threshold	= 0.f;
		}
	}

	if ( event_type & time_event_animation_interval_ended ) {
		if ( !state.is_freezed ) {
			state.previous_animation_interval_id	= state.animation_interval_id;
			state.animation_interval_id			= state.event_iterator->animation_interval_id;
			state.animation_interval_time		= state.event_iterator->animation_interval_time;
			animation_interval const& interval	=
				current_animation_node.animation_intervals( )[ state.animation_interval_id ];
			float const animation_length	=
				cubic_spline_skeleton_animation_pinned( interval.animation( ) )->length_in_frames( ) / default_fps;
			state.animation_time_threshold	=
				animation_length < interval.start_time( ) + interval.length( ) &&
				animation_length < interval.start_time( ) + state.animation_interval_time
					? animation_length
					: 0.f;
			update_animation_time	( state );
		}

		if ( !current_animation_node.time_driving_animation( ) )
			n_ary_tree_time_scale_start_time_modifier(
				current_animation_node,
				state.event_iterator->event_time_in_ms,
				state.animation_interval_time
			);
	}

	if ( event_type & time_event_time_direction_changed ) {
		if ( !current_animation_node.time_driving_animation( ) )
			n_ary_tree_time_scale_start_time_modifier(
				current_animation_node,
				state.event_iterator->event_time_in_ms,
				state.animation_interval_time
			);
	}

	if ( event_type & time_event_weight_transitions_started ) {
		animation_interval const& interval	=
			current_animation_node.animation_intervals( )[ state.animation_interval_id ];
		current_frame_position frame_position;
		frame frame_transform;
		{
			cubic_spline_skeleton_animation_pinned pinned_animation =
				cubic_spline_skeleton_animation_pinned( interval.animation( ) );
			frame_transform = pinned_animation->bone( u32( 0 ) ).bone_frame(
				state.animation_time * default_fps,
				frame_position
			);
		}

		object_movement& accumulated	= state.bone_matrices_computer.accumulated_object_movement;
		accumulated.translation		= frame_transform.translation - accumulated.translation;
		accumulated.rotation		= math::quaternion( frame_transform.rotation ) * math::conjugate( accumulated.rotation );
		accumulated.scale			= frame_transform.scale / accumulated.scale;
		state.are_there_any_weight_transitions	= true;
	}

	if ( event_type & time_event_weight_transitions_ended ) {
		animation_interval const& interval	=
			current_animation_node.animation_intervals( )[ state.animation_interval_id ];
		current_frame_position frame_position;
		frame frame_transform;
		{
			cubic_spline_skeleton_animation_pinned pinned_animation =
				cubic_spline_skeleton_animation_pinned( interval.animation( ) );
			frame_transform = pinned_animation->bone( u32( 0 ) ).bone_frame(
				state.animation_time * default_fps,
				frame_position
			);
		}

		object_movement& accumulated	= state.bone_matrices_computer.accumulated_object_movement;
		accumulated.translation		= frame_transform.translation;
		accumulated.rotation		= math::quaternion( frame_transform.rotation );
		accumulated.scale			= frame_transform.scale;

		n_ary_tree_weight_calculator weight_calculator( state.event_iterator->event_time_in_ms, &current_animation_node );
		weight_calculator.visit		( current_animation_node );
		n_ary_tree_event_iterator event_iterator	= state.event_iterator;
		if ( event_types & time_event_animation_lexeme_started )
			event_iterator.m_animation_event_iterator.advance( 0 );
		if ( event_types & time_event_animation_lexeme_ended )
			++event_iterator.m_weight_event_iterator;
		event_iterator.select_state	( );
		state.are_there_any_weight_transitions	= event_iterator.are_there_any_weight_transitions( );

		n_ary_tree_time_scale_start_time_modifier(
			current_animation_node.time_driving_animation( )
				? *current_animation_node.time_driving_animation( )
				: current_animation_node,
			state.event_iterator->event_time_in_ms,
			(
				current_animation_node.time_driving_animation( )
					? *current_animation_node.time_driving_animation( )
					: current_animation_node
			).animation_state( ).animation_interval_time
		);
	}
}

void n_ary_tree::process_events( const u32 target_time_in_ms, const u32 event_types )
{
	for ( n_ary_tree_animation_node* i = m_weight_root; i; i = i->m_next_weight_animation )
		if ( i->animation_state( ).event_iterator->event_time_in_ms == target_time_in_ms )
			if ( event_types & i->animation_state( ).event_iterator->event_type )
				process_event			( *i, event_types );
}

// claude@NOTE: target passes the inlined iterator's this in eax to select_state; base retains it in edi.
void n_ary_tree::update_event_iterators( const u32 target_time_in_ms )
{
	while ( (*m_animation_events)->event_iterator->event_time_in_ms == target_time_in_ms ) {
		++(*m_animation_events)->event_iterator;

		animation_state** const found	= std::lower_bound(
			m_animation_events + 1,
			m_animation_events + m_animations_count,
			*m_animation_events,
			event_iterator_predicate( )
		);
		animation_state* const event	= *m_animation_events;
		std::copy						( m_animation_events + 1, found, m_animation_events );
		*( found - 1 )					= event;
	}
}

// claude@NOTE: target passes callback_generators_head on the stack; base uses EAX after restoring the target callback branches.
bool n_ary_tree::dispatch_callbacks(
	callback_generator_info const*		callback_generators_head,
	subscribed_channel*&	channels_head,
	const u32				current_time_in_ms,
	bool&					callbacks_are_actual
)
{
	bool result								= false;
	for ( callback_generator_info const* generator = callback_generators_head; generator; generator = generator->next )
	{
		if ( generator->event_type & (
				time_event_animation_lexeme_ended |
				time_event_animation_interval_ended |
				time_event_animation_ended_in_positive_direction |
				time_event_animation_ended_in_negative_direction
			) )
		{
			for ( subscribed_channel const* subscribed_channel = channels_head; subscribed_channel; subscribed_channel = subscribed_channel->next )
			{
				if ( subscribed_channel->channel_id[ 0 ] >= channel_id_max )
					continue;

				bool found						= false;
				if ( !found && ( generator->event_type & (
						time_event_animation_ended_in_positive_direction |
						time_event_animation_ended_in_negative_direction
					) ) )
					found							=
						subscribed_channel->channel_id[ 0 ] == channel_id_on_animation_end;
				if ( !found && ( generator->event_type & time_event_animation_interval_ended ) )
					found							=
						subscribed_channel->channel_id[ 0 ] == channel_id_on_animation_interval_end;
				if ( !found && ( generator->event_type & time_event_animation_lexeme_ended ) )
					found							=
						subscribed_channel->channel_id[ 0 ] == channel_id_on_animation_lexeme_end;
				if ( !found )
					continue;

				for ( animation_callback* callback = subscribed_channel->first_callback; callback; callback = callback->next )
				{
					if ( !callback->enabled )
						continue;
					if ( callback->animation &&
						 callback->animation.c_ptr( ) != generator->animation.c_ptr( ) )
						continue;
					if ( callback->animated_object &&
						 callback->animated_object != generator->animated_object )
						continue;

					animation_callback_params params(
						generator->animated_object,
						generator->animation,
						subscribed_channel->channel_id,
						current_time_in_ms,
						generator->user_data,
						u8( -1 ),
						generator->animation_interval_id
					);
					callback->enabled				=
						callback->callback( params ) == callback_return_type_call_me_again &&
						callback->enabled;
					callbacks_are_actual			= callbacks_are_actual && callback->enabled;
					result							= result || params.interrupt_animation_player_tick;
				}
			}
		}

		if ( !( generator->event_type & time_event_channel_callback_should_be_fired ) )
			continue;

		cubic_spline_skeleton_animation_pinned pinned_animation( generator->animation );
		animation_event_channels const& event_channels	= pinned_animation->event_channels( );
		if ( !event_channels.channels_count( ) )
			continue;

		for ( subscribed_channel const* subscribed_channel = channels_head; subscribed_channel; subscribed_channel = subscribed_channel->next )
		{
			u32 const channel_id			=
				event_channels.get_channel_id( subscribed_channel->channel_id );
			if ( channel_id == u32( -1 ) )
				continue;
			if ( ( generator->channel_ids & ( u8( 1 ) << channel_id ) ) !=
				 ( u8( 1 ) << channel_id ) )
				continue;

			event_channel const& channel	= event_channels.channel( channel_id );
			float const animation_time	= generator->animation_time * default_fps;
			float const* const knots		= channel.knots( );
			u32 const knots_count			= channel.knots_count( );
			float const* const upper		=
				std::lower_bound( knots, knots + knots_count, animation_time );
			u32 const knot_upper_id		= ( upper - knots ) % knots_count;
			u32 const knot_lower_id		=
				( knot_upper_id + knots_count - 1 ) % knots_count;
			float const animation_length	= pinned_animation->length_in_frames( );

			float const lower_time			=
				animation_time < channel.knot( knot_lower_id ) ?
				channel.knot( knot_lower_id ) - animation_length :
				channel.knot( knot_lower_id );
			float const upper_time			=
				channel.knot( knot_upper_id ) < animation_time ?
				channel.knot( knot_upper_id ) + animation_length :
				channel.knot( knot_upper_id );

			float const clamped_time		= math::min( animation_time, upper_time );
			u32 const domain_id			=
				clamped_time - lower_time <= upper_time - clamped_time ?
				knot_lower_id :
				knot_upper_id;
			u8 domain_data				= channel.domain( domain_id ).data;

			for ( animation_callback* callback = subscribed_channel->first_callback; callback; callback = callback->next )
			{
				if ( !callback->enabled )
					continue;
				if ( callback->animation &&
					 callback->animation.c_ptr( ) != generator->animation.c_ptr( ) )
					continue;
				if ( callback->event_type != u8( -1 ) &&
					 callback->event_type != domain_data )
					continue;
				if ( callback->animated_object &&
					 callback->animated_object != generator->animated_object )
					continue;

				animation_callback_params params(
					generator->animated_object,
					generator->animation,
					subscribed_channel->channel_id,
					current_time_in_ms,
					generator->user_data,
					domain_data,
					generator->animation_interval_id
				);
				callback->enabled				=
					callback->callback( params ) == callback_return_type_call_me_again &&
					callback->enabled;
				callbacks_are_actual			= callbacks_are_actual && callback->enabled;
				result							= result || params.interrupt_animation_player_tick;
			}
		}
	}

	return									result;
}

void n_ary_tree::remove_animations( const u32 target_time_in_ms )
{
	buffer_vector< pcvoid > animated_objects(
		ALLOCA( m_animated_objects_count * sizeof( pcvoid ) ),
		m_animated_objects_count
	);

	animation_state* current_animation_state		= m_animation_states;
	animation_state* current_alive_animation_state	= current_animation_state;
	n_ary_tree_animation_node* previous_animation	= 0;
	for ( n_ary_tree_animation_node* current_animation = m_weight_root; current_animation; ++current_animation_state ) {
		if ( current_animation_state->event_iterator->event_time_in_ms == target_time_in_ms &&
			 ( current_animation_state->event_iterator->event_type &
			   time_event_animation_lexeme_ended ) )
		{
			std::remove						(
				m_animation_events,
				m_animation_events + m_animations_count,
				&current_animation->animation_state( )
			);
			remove_animation				( current_animation, previous_animation );
			continue;
		}

		if ( std::find(
				animated_objects.begin( ),
				animated_objects.end( ),
				current_animation->animated_object( )
			) == animated_objects.end( ) )
			animated_objects.push_back	( current_animation->animated_object( ) );

		if ( current_animation_state != current_alive_animation_state ) {
			*current_alive_animation_state	= *current_animation_state;
			current_animation->set_animation_state( *current_alive_animation_state );
		}

		++current_alive_animation_state;
		previous_animation				= current_animation;
		current_animation				= current_animation->m_next_weight_animation;
	}

	if ( current_alive_animation_state != current_animation_state ) {
		for ( animation_state* i = m_animation_states, * const e = i + m_animations_count, **j = m_animation_events; i != e; ++i, ++j )
			*j						= i;

		stlp_std::sort					(
			m_animation_events,
			m_animation_events + m_animations_count,
			event_iterator_predicate( )
		);
	}

	for ( ; current_alive_animation_state != current_animation_state; ++current_alive_animation_state )
		current_alive_animation_state->~animation_state( );

	if ( m_animated_objects_count != animated_objects.size( ) ) {
		animated_object_holder* j			= m_animated_objects;
		for ( animated_object_holder* i = m_animated_objects, * const e = i + m_animated_objects_count; i != e; ++i ) {
			if ( std::find(
					animated_objects.begin( ),
					animated_objects.end( ),
					i->animated_object
				) == animated_objects.end( ) )
				continue;

			if ( j != i )
				*j						= *i;
			++j;
		}

		m_animated_objects_count		= animated_objects.size( );
	}
}

bool n_ary_tree::update_event_iterators_and_dispatch_callbacks(
	const u32				target_time_in_ms,
	subscribed_channel*&	channels_head,
	bool&					callbacks_are_actual
)
{
	callback_generator_info const* callback_generators_head	= 0;
	callback_generator_info* previous_generator_info			= 0;
	for ( animation_state const* i = m_animation_states, * const e = i + m_animations_count; i != e; ++i ) {
		if ( i->event_iterator->event_time_in_ms != target_time_in_ms )
			continue;

		u16 const event_type			= i->event_iterator->event_type;
		if ( !( event_type & (
			time_event_animation_lexeme_ended |
			time_event_animation_interval_ended |
			time_event_animation_ended_in_positive_direction |
			time_event_animation_ended_in_negative_direction |
			time_event_channel_callback_should_be_fired
		) ) )
			continue;

		n_ary_tree_animation_node const& animation	= i->event_iterator.animation( );
		if ( animation.is_transitting_to_zero( ) &&
			 !( event_type & time_event_animation_lexeme_ended ) )
			continue;
		if ( !animation.can_generate_events( ) )
			continue;

		callback_generator_info* const generator	=
			static_cast< callback_generator_info* >( ALLOCA( sizeof( callback_generator_info ) ) );
		u32 const animation_interval_id	=
			( event_type & time_event_animation_interval_ended ) ?
			i->previous_animation_interval_id :
			i->animation_interval_id;
		new ( generator ) callback_generator_info(
				animation.animated_object( ),
				animation.animation_intervals( )[ animation_interval_id ].animation( ),
				i->animation_time,
				event_type,
				i->event_iterator->channel_ids,
				animation.user_data,
				u8( animation_interval_id )
			);
		if ( previous_generator_info )
			previous_generator_info->next	= generator;
		else
			callback_generators_head			= generator;
		previous_generator_info				= generator;
	}

	remove_animations					( target_time_in_ms );
	update_event_iterators				( target_time_in_ms );
	bool const result					= dispatch_callbacks(
		callback_generators_head,
		channels_head,
		target_time_in_ms,
		callbacks_are_actual
	);

	for ( callback_generator_info const* i = callback_generators_head; i; ) {
		callback_generator_info const* const next	= i->next;
		i->~callback_generator_info			( );
		i								= next;
	}

	return								result;
}

callback_generator_info* n_ary_tree::generate_animation_lexeme_end_events(
	n_ary_tree const&					previous_tree,
	n_ary_tree const&					new_tree,
	callback_generator_info*			callback_generators_buffer_begin,
	callback_generator_info* const		callback_generators_buffer_end,
	subscribed_channel* const			channels_head
)
{
	callback_generator_info* callback_generators_head	= 0;
	callback_generator_info* previous_generator_info	= 0;
	for ( n_ary_tree_animation_node* previous_animation = previous_tree.m_weight_root;
		  previous_animation;
		  previous_animation = previous_animation->m_next_weight_animation )
	{
		n_ary_tree_animation_node* new_animation	= new_tree.m_weight_root;
		animation_comparer_predicate const predicate	( false, false );
		for ( ; new_animation; new_animation = new_animation->m_next_weight_animation )
			if ( predicate(
				*new_animation,
				*previous_animation
			) == equal )
				break;

		if ( new_animation )
			continue;

		for ( subscribed_channel const* channel = channels_head; channel; channel = channel->next ) {
			if ( u8( channel->channel_id[ 0 ] ) != channel_id_on_animation_lexeme_end )
				continue;

			animation_callback const* callback	= channel->first_callback;
			for ( ; callback;
				  callback = callback->next )
			{
				if ( !callback->enabled )
					continue;
				if ( callback->animation &&
					 callback->animation.c_ptr( ) != previous_animation->animation_intervals( )[ previous_animation->animation_state( ).animation_interval_id ].animation( ).c_ptr( ) )
					continue;
				if ( callback->animated_object &&
					 callback->animated_object != previous_animation->animated_object( ) )
					continue;

				break;
			}

			if ( !callback )
				continue;

			u32 const animation_interval_id	= previous_animation->animation_state( ).animation_interval_id;
			animation_interval const* const animation_intervals	= previous_animation->animation_intervals( );
			R_ASSERT_CMP					(
				callback_generators_buffer_begin,
				<,
				callback_generators_buffer_end
			);
			callback_generator_info* const generator	=
				new ( callback_generators_buffer_begin++ ) callback_generator_info(
					previous_animation->animated_object( ),
					animation_intervals[ animation_interval_id ].animation( ),
					previous_animation->animation_state( ).animation_time,
					time_event_animation_lexeme_ended,
					0,
					previous_animation->user_data,
					u8( animation_interval_id )
				);
			if ( previous_generator_info )
				previous_generator_info->next	= generator;
			else
				callback_generators_head			= generator;
			previous_generator_info				= generator;
			break;
		}
	}

	return							callback_generators_head;
}

u32 n_ary_tree::nearest_event_time_in_ms( ) const
{
	if ( m_weight_root ) return	(*m_animation_events)->event_iterator->event_time_in_ms;

	return						u32(-1);
}

void n_ary_tree::adjust_animation_events_times( n_ary_tree const& other )
{
	animation_state* source			= m_animation_states;
	animation_state* const end		= other.m_animation_states + other.m_animations_count;
	for ( animation_state* i = other.m_animation_states; i != end; ++i, ++source ) {
		n_ary_tree_animation_node* const animation	= i->event_iterator.m_animation_node;
		subscribed_channel** const channels_head	=
			i->event_iterator.m_animation_event_iterator.m_channels_head;

		*i								= *source;
		i->event_iterator.m_animation_node	=
			source->event_iterator.m_animation_node ? animation : 0;
		i->event_iterator.m_animation_event_iterator.m_channels_head	= channels_head;
		i->event_iterator.m_animation_event_iterator.m_animation	=
			source->event_iterator.m_animation_event_iterator.m_animation ? animation : 0;
		i->event_iterator.m_weight_event_iterator.m_animation	=
			source->event_iterator.m_weight_event_iterator.m_animation ? animation : 0;
	}

	stlp_std::sort					(
		other.m_animation_events,
		other.m_animation_events + other.m_animations_count,
		event_iterator_predicate( )
	);
}

bool n_ary_tree::tick(
	const u32				target_time_in_ms,
	subscribed_channel*&	channels_head,
	bool&					callbacks_are_actual
)
{
	bool user_handled_callbacks	= false;
	if ( !m_weight_root ) {
		m_tree_actual_time_in_ms	= target_time_in_ms;
		return						false;
	}

	for ( u32 event_time_in_ms; m_weight_root && ( event_time_in_ms = (*m_animation_events)->event_iterator->event_time_in_ms ) <= target_time_in_ms; ) {
		if ( m_tree_actual_time_in_ms < event_time_in_ms )
			update_animation_states	( m_tree_actual_time_in_ms, event_time_in_ms );

		if ( need_new_transform( event_time_in_ms ) ) {
			process_events			( event_time_in_ms, time_event_animation_lexeme_started );
			for ( animated_object_holder* i = m_animated_objects, * const e = i + m_animated_objects_count; i != e; ++i )
				if ( i->need_new_transform )
					i->new_transform	= get_object_transform( i->animated_object );

			process_events			( event_time_in_ms, time_event_all_events & ~time_event_animation_lexeme_started );
			for ( animated_object_holder* i = m_animated_objects, * const e = i + m_animated_objects_count; i != e; ++i )
				if ( i->need_new_transform ) {
					set_object_transform( i->animated_object, i->new_transform );
					i->need_new_transform = false;
				}
		}
		else
			process_events			( event_time_in_ms, time_event_all_events );

		m_tree_actual_time_in_ms	= event_time_in_ms;
		user_handled_callbacks		= update_event_iterators_and_dispatch_callbacks( event_time_in_ms, channels_head, callbacks_are_actual ) || user_handled_callbacks;
	}

	if ( m_weight_root && m_tree_actual_time_in_ms != target_time_in_ms )
		update_animation_states	( m_tree_actual_time_in_ms, target_time_in_ms );

	m_tree_actual_time_in_ms	= target_time_in_ms;

	return						user_handled_callbacks;
}

} // namespace mixing
} // namespace animation
} // namespace vostok
