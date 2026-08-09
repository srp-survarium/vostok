////////////////////////////////////////////////////////////////////////////
//	Created		: 05.03.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "mixing_n_ary_tree_transition_tree_constructor.h"

#include "mixing_n_ary_tree_animation_node.h"
#include "mixing_n_ary_tree_weight_node.h"
#include "mixing_n_ary_tree_time_scale_node.h"
#include "mixing_n_ary_tree_weight_transition_node.h"
#include "mixing_n_ary_tree_time_scale_transition_node.h"
#include "mixing_n_ary_tree_multiplication_node.h"
#include <vostok/animation/mixing_n_ary_tree.h>
#include "mixing_n_ary_tree_node_comparer.h"
#include "mixing_n_ary_tree_time_scale_calculator.h"
#include "mixing_n_ary_tree_node_comparer.h"
#include "mixing_n_ary_tree_interpolator_selector.h"
#include "mixing_n_ary_tree_transition_tree_constructor_impl.h"
#include "mixing_n_ary_tree_comparer.h"
#include "i_editor_mixer_inline.h"
#include "mixing_n_ary_tree_target_time_scale_calculator.h"
#include "mixing_n_ary_tree_weight_calculator.h"
#include <vostok/animation/mixing_animated_object_holder.h>
#include <vostok/animation/mixing_time_animations_predicate.h>

#define VOSTOK_DEBUG_ANIMATIONS_COUNT			0

namespace vostok {
namespace animation {
namespace mixing {

inline bool time_animations_predicate::operator ( )(
		n_ary_tree_animation_node const* const left,
		n_ary_tree_animation_node const* const right
	) const
	{
		if ( left->time_synchronization_group_id( ) != right->time_synchronization_group_id( ) )
			return left->time_synchronization_group_id( ) < right->time_synchronization_group_id( );

		if ( !left->time_driving_animation( ) && right->time_driving_animation( ) )
			return true;
		if ( left->time_driving_animation( ) && !right->time_driving_animation( ) )
			return false;

		return left < right;
	}

n_ary_tree_animation_node* n_ary_tree_transition_tree_constructor::add_animation_node(
	n_ary_tree_animation_node&		new_animation,
	animation_state const*			previous_animation_state,
	u32								animation_interval_id,
	float							animation_interval_time,
	bool							is_new_animation
)
{
	*m_new_animation_event++			= m_new_animation_state;

	u16 initial_event_types				= 0;
	if ( is_new_animation )
		initial_event_types				= time_event_animation_lexeme_started
			| time_event_weight_transitions_started;
	else if ( previous_animation_state && !previous_animation_state->are_there_any_weight_transitions )
		initial_event_types				= time_event_weight_transitions_started;

	n_ary_tree_weight_calculator weight_calculator( m_current_time_in_ms, 0 );
	weight_calculator.visit				( new_animation );
	new_animation.set_animation_state	( *m_new_animation_state );
	n_ary_tree_base_node** i				= new_animation.operands( sizeof( n_ary_tree_animation_node ) );
	n_ary_tree_base_node** const end		= i + new_animation.operands_count( );
	for ( ; i != end; ++i )
		(*i)->is_time_scale				( ); // Target discards the predicate result.

	bool const use_previous_animation_state	=
		previous_animation_state
		&& (
			!previous_animation_state->event_iterator.animation( ).is_transitting_to_zero( )
			|| new_animation.is_transitting_to_zero( )
			|| !previous_animation_state->is_freezed
		)
		;
	new ( m_new_animation_state ) animation_state_params(
		initial_event_types,
		animation_interval_id,
		animation_interval_time,
		use_previous_animation_state ? previous_animation_state->animation_time_threshold : 0.f,
		weight_calculator.weight( ),
		use_previous_animation_state ? previous_animation_state : 0
	);
	++m_new_animation_state;

	return								&new_animation;
}

n_ary_tree_base_node* n_ary_tree_transition_tree_constructor::new_time_scale( n_ary_tree_animation_node& new_time_driving_animation, u32& animation_interval_id, float& animation_interval_time )
{
	n_ary_tree_animation_node* previous_time_driving_animation	= m_from.time_root( );
	for ( ; previous_time_driving_animation; previous_time_driving_animation = previous_time_driving_animation->m_next_weight_animation ) {
		if ( previous_time_driving_animation->time_driving_animation( ) )
			continue;
		if ( previous_time_driving_animation->time_synchronization_group_id( ) == new_time_driving_animation.time_synchronization_group_id( ) )
			break;
	}

	if ( !previous_time_driving_animation || previous_time_driving_animation->animation_state( ).is_freezed )
		return								NULL;

	n_ary_tree_animation_node* target_time_driving_animation	= m_to.time_root( );
	for ( ; target_time_driving_animation; target_time_driving_animation = target_time_driving_animation->m_next_time_animation )
		if ( target_time_driving_animation->time_synchronization_group_id( ) == new_time_driving_animation.time_synchronization_group_id( ) )
			break;
	if ( target_time_driving_animation && target_time_driving_animation != &new_time_driving_animation )
		return								NULL;

	animation_interval_id					=
		( new_time_driving_animation.override_existing_animation( )
			? new_time_driving_animation
			: *previous_time_driving_animation
		).animation_state( ).animation_interval_id;
	float const new_driving_animation_length	=
		new_time_driving_animation.animation_intervals( )[ animation_interval_id ].length( )
		/ previous_time_driving_animation->animation_intervals( )[ animation_interval_id ].length( );
	float const directional_time_scale_factor	=
		new_time_driving_animation.is_positive_event_direction( )
			== previous_time_driving_animation->is_positive_event_direction( )
				? new_driving_animation_length
				: -new_driving_animation_length;
	animation_interval_time					=
		new_time_driving_animation.override_existing_animation( )
			? new_time_driving_animation.animation_state( ).animation_interval_time
			: previous_time_driving_animation->animation_state( ).animation_interval_time
				* new_driving_animation_length;

	n_ary_tree_time_scale_calculator time_scale_calculator(
		m_current_time_in_ms,
		0.f,
		m_current_time_in_ms
	);
	n_ary_tree_base_node* target_time_scale_node	= NULL;
	if ( new_time_driving_animation.operands_count( ) ) {
		n_ary_tree_base_node* const node	=
			*new_time_driving_animation.operands( sizeof( n_ary_tree_animation_node ) );
		if ( node && node->is_time_scale( ) ) {
			target_time_scale_node			= node;
			node->accept					( time_scale_calculator );
		}
	}

	float const time_scale_factor				=
		target_time_scale_node ? time_scale_calculator.time_scale( ) : 1.f;
	base_interpolator const* time_scale_interpolator	=
		target_time_scale_node
			? time_scale_calculator.interpolator( )
			: &new_time_driving_animation.weight_interpolator( );
	if ( !target_time_scale_node && time_scale_interpolator->transition_time( ) <= 0.f )
		return								NULL;

	n_ary_tree_base_node* previous_time_scale_node	= NULL;
	if ( previous_time_driving_animation->operands_count( ) ) {
		n_ary_tree_base_node* const node	=
			*previous_time_driving_animation->operands( sizeof( n_ary_tree_animation_node ) );
		if ( node && node->is_time_scale( ) )
			previous_time_scale_node		= m_cloner.clone( *node, directional_time_scale_factor );
	}

	n_ary_tree_target_time_scale_calculator target_time_scale_calculator( *previous_time_driving_animation );
	float const previous_target_time_scale	=
		target_time_scale_calculator.result( ) * directional_time_scale_factor;
	if ( time_scale_factor == previous_target_time_scale
		&& ( !target_time_scale_node || !target_time_scale_node->is_transition( ) ) )
	{
		base_interpolator const& interpolator	= *m_cloner.clone( *time_scale_interpolator );
		n_ary_tree_base_node* const from	=
			previous_time_scale_node
				? previous_time_scale_node
				: new ( m_buffer.c_ptr( ) ) n_ary_tree_time_scale_node(
					interpolator,
					1.f,
					animation_interval_time,
					m_current_time_in_ms
				);
		if ( !previous_time_scale_node )
			m_buffer					+= sizeof( n_ary_tree_time_scale_node );

		n_ary_tree_base_node* const to	=
			new ( m_buffer.c_ptr( ) ) n_ary_tree_time_scale_node(
				interpolator,
				time_scale_factor,
				animation_interval_time,
				m_current_time_in_ms
			);
		m_buffer						+= sizeof( n_ary_tree_time_scale_node );

		n_ary_tree_time_scale_transition_node* const result	=
			new ( m_buffer.c_ptr( ) ) n_ary_tree_time_scale_transition_node(
				*from,
				*to,
				interpolator,
				m_current_time_in_ms
			);
		m_buffer						+= sizeof( n_ary_tree_time_scale_transition_node );
		return							result;
	}

	if ( previous_time_scale_node ) {
		if ( time_scale_factor == 0.f && !previous_time_scale_node->is_transition( ) ) {
			n_ary_tree_time_scale_node& node	=
				static_cast_checked< n_ary_tree_time_scale_node& >( *previous_time_scale_node );
			node.set_time_scale_start_time(
				node.time_scale_start_time_in_ms( ),
				new_time_driving_animation.animation_state( ).animation_time
			);
		}
		return							previous_time_scale_node;
	}

	if ( time_scale_factor == 1.f )
		return								NULL;

	n_ary_tree_time_scale_node* const result	=
		new ( m_buffer.c_ptr( ) ) n_ary_tree_time_scale_node(
			*m_cloner.clone( *time_scale_interpolator ),
			time_scale_factor,
			animation_interval_time,
			m_current_time_in_ms
		);
	m_buffer							+= sizeof( n_ary_tree_time_scale_node );
	return								result;
}

n_ary_tree_animation_node* n_ary_tree_transition_tree_constructor::new_animation(
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
)
{
	animation_interval_id			= ( from.override_existing_animation( ) || to.animation_state( ).is_freezed ? from : to ).animation_state( ).animation_interval_id;
	animation_interval_time			= ( from.override_existing_animation( ) || to.animation_state( ).is_freezed ? from : to ).animation_state( ).animation_interval_time;

	time_scale_operands_count		= 0;
	n_ary_tree_base_node* time_scale_node	= NULL;
	if ( can_be_time_driving_animation && !from.time_driving_animation( ) && from.time_synchronization_group_id( ) != u32( -1 ) ) {
		time_scale_node				= new_time_scale( from, animation_interval_id, animation_interval_time );
		if ( time_scale_node ) {
			operands_offset			= 1;
			time_scale_operands_count	= 1;
		}
	}

	animation_interval const* current_interval		= from.animation_intervals( );
	animation_interval const* const intervals_end	= current_interval + from.animation_intervals_count( );
	animation_interval const* const cloned_intervals_begin	= static_cast< animation_interval const* >( m_buffer.c_ptr( ) );
	for ( ; current_interval != intervals_end; ++current_interval ) {
		new ( m_buffer.c_ptr( ) ) animation_interval(
			current_interval->animation( ),
			current_interval->start_time( ),
			current_interval->length( )
		);
		m_buffer					+= sizeof( animation_interval );
	}

	n_ary_tree_animation_node* const result	= static_cast< n_ary_tree_animation_node* >( m_buffer.c_ptr( ) );
	if ( !weight_driving_animation )
		new ( result ) n_ary_tree_animation_node(
			cloned_intervals_begin,
			cloned_intervals_begin + from.animation_intervals_count( ),
			from.unique_animation_id( ),
			from.start_cycle_animation_interval_id( ),
			*m_cloner.clone( from.weight_interpolator( ) ),
			from.animated_object( ),
			from.playback_type( ),
			from.time_calculator( ),
			from.time_synchronization_group_id( ),
			from.weight_synchronization_group_id( ),
			from.override_existing_animation( ),
			from.is_positive_event_direction( ),
			from.can_generate_events( ),
			from.additivity_priority( ),
			from.bones_mask( ),
			weight_operands_count + time_scale_operands_count + operands_offset,
			is_transitting_to_zero
		);
	else
		new ( result ) n_ary_tree_animation_node(
			*weight_driving_animation,
			cloned_intervals_begin,
			cloned_intervals_begin + from.animation_intervals_count( ),
			from.unique_animation_id( ),
			from.start_cycle_animation_interval_id( ),
			from.animated_object( ),
			from.playback_type( ),
			from.time_calculator( ),
			from.time_synchronization_group_id( ),
			from.override_existing_animation( ),
			from.is_positive_event_direction( ),
			from.can_generate_events( ),
			from.additivity_priority( ),
			from.bones_mask( ),
			weight_operands_count + time_scale_operands_count + operands_offset,
			is_transitting_to_zero
		);
	m_buffer						+= sizeof( n_ary_tree_animation_node );

	result->user_data				= from.user_data;

	if ( !m_weight_root )
		m_weight_root				= result;
	else
		m_previous_animation->m_next_weight_animation	= result;

	if ( can_be_time_driving_animation && !from.time_driving_animation( ) && from.time_synchronization_group_id( ) != u32( -1 ) ) {
		*m_time_driving_animations_end++	= result;
		if ( time_scale_node )
			*result->operands( sizeof( n_ary_tree_animation_node ) )	= time_scale_node;
	}
	else if ( from.time_driving_animation( ) ) {
		animation_state const& time_driving_animation_state	= from.time_driving_animation( )->animation_state( );
		animation_interval_id		= time_driving_animation_state.animation_interval_id;
		animation_interval const* const time_driving_animation_interval	=
			from.time_driving_animation( )->animation_intervals( ) + animation_interval_id;
		animation_interval const* const animation_interval	=
			from.animation_intervals( ) + animation_interval_id;
		animation_interval_time		=
			animation_interval->length( ) / time_driving_animation_interval->length( )
			* time_driving_animation_state.animation_interval_time;
	}
	else {
		animation_interval_id		= ( from.override_existing_animation( ) || to.animation_state( ).is_freezed ? from : to ).animation_state( ).animation_interval_id;
		animation_interval_time		= ( from.override_existing_animation( ) || to.animation_state( ).is_freezed ? from : to ).animation_state( ).animation_interval_time;
	}

	animated_object_holder* const existing	=
		std::find( m_animated_objects, m_new_animated_object, result->animated_object( ) );
	if ( existing == m_new_animated_object ) {
		new ( m_new_animated_object ) animated_object_holder( result->animated_object( ) );

		animated_object_holder const* const from_objects	= m_from.animated_objects( );
		animated_object_holder const* const from_end		=
			from_objects + m_from.animated_objects_count( );
		animated_object_holder const* const from_holder	=
			std::find( from_objects, from_end, result->animated_object( ) );

		m_new_animated_object->transform	=
			from_holder != from_end ?
			from_holder->transform :
			m_get_transform_functor( result->animated_object( ) );
		++m_new_animated_object;
	}

	m_previous_animation			= result;
	return							result;
}

n_ary_tree_base_node* n_ary_tree_transition_tree_constructor::new_weight_transition( base_interpolator const& interpolator, float from, float to )
{
	if ( interpolator.transition_time() == 0.f ) {
		n_ary_tree_base_node* const result	= (n_ary_tree_base_node*)m_buffer.c_ptr( );
		m_buffer				+= sizeof( n_ary_tree_weight_node );
		new ( result ) n_ary_tree_weight_node( *m_cloner.clone( interpolator ), 0.f );
		return					result;
	}

	n_ary_tree_base_node* const result	= (n_ary_tree_base_node*)m_buffer.c_ptr( );
	m_buffer				+= sizeof( n_ary_tree_weight_transition_node );

	n_ary_tree_base_node* const weight_from	= (n_ary_tree_base_node*)m_buffer.c_ptr( );
	m_buffer				+= sizeof( n_ary_tree_weight_node );
	new ( weight_from ) n_ary_tree_weight_node( *m_cloner.clone( interpolator ), from );

	n_ary_tree_base_node* const weight_to	= (n_ary_tree_base_node*)m_buffer.c_ptr( );
	m_buffer				+= sizeof( n_ary_tree_weight_node );
	new ( weight_to ) n_ary_tree_weight_node( *m_cloner.clone( interpolator ), to );

	new ( result ) n_ary_tree_weight_transition_node(
		*weight_from,
		*weight_to,
		*m_cloner.clone( interpolator ),
		m_current_time_in_ms
	);

	return					result;
}

n_ary_tree_animation_node* n_ary_tree_transition_tree_constructor::remove_animation( n_ary_tree_animation_node& animation, n_ary_tree_animation_node* const weight_driving_animation, bool is_new_driving_animation )
{
	base_interpolator const& interpolator	=
		weight_driving_animation ? weight_driving_animation->weight_interpolator( ) : animation.weight_interpolator( );
	if ( interpolator.transition_time( ) == 0.f )
		return							NULL;

	if ( animation.is_transitting_to_zero( ) && !is_new_driving_animation ) {
		u32 operands_offset				=
			animation.operands_count( )
			&& (*animation.operands( sizeof( n_ary_tree_animation_node ) ))->is_time_scale( )
				? 1
				: 0;
		u32 const operands_count			= animation.operands_count( ) - operands_offset;

		bool can_be_time_driving_animation	= true;
		if ( animation.time_synchronization_group_id( ) != u32( -1 ) ) {
			n_ary_tree_animation_node* i	= m_to.time_root( );
			for ( ; i && i->time_synchronization_group_id( ) != animation.time_synchronization_group_id( ); i = i->m_next_time_animation )
				;

			if ( i && i != &animation ) {
				can_be_time_driving_animation	= false;
				operands_offset					= 0;
			}
		}

		u32 time_scale_operands_count;
		u32 animation_interval_id;
		float animation_interval_time;
		n_ary_tree_animation_node* const result	= new_animation(
			animation,
			animation,
			weight_driving_animation,
			operands_count,
			time_scale_operands_count,
			operands_offset,
			animation_interval_id,
			animation_interval_time,
			true,
			can_be_time_driving_animation
		);

		n_ary_tree_base_node** new_operands	=
			static_cast< n_ary_tree_base_node** >( m_buffer.c_ptr( ) ) + time_scale_operands_count;
		m_buffer						+= ( time_scale_operands_count + operands_count ) * sizeof( n_ary_tree_base_node* );

		n_ary_tree_base_node** i			= animation.operands( sizeof( n_ary_tree_animation_node ) );
		n_ary_tree_base_node** const operands_end	= i + animation.operands_count( );
		for ( ; i != operands_end; ++i, ++new_operands ) {
			if ( (*i)->is_time_scale( ) ) {
				if ( time_scale_operands_count ) {
					--new_operands;
					continue;
				}

				*new_operands				= m_cloner.clone( **i, 1.f );
			}
			else
				*new_operands				= m_cloner.clone( **i );
		}

		return							add_animation_node(
			*result,
			&animation.animation_state( ),
			animation_interval_id,
			animation_interval_time,
			false
		);
	}

	u32 operands_offset					=
		!animation.time_driving_animation( )
		&& animation.operands_count( )
		&& (*animation.operands( sizeof( n_ary_tree_animation_node ) ))->is_time_scale( )
			? 1
			: 0;
	bool can_be_time_driving_animation	= true;
	if ( animation.time_synchronization_group_id( ) != u32( -1 ) ) {
		n_ary_tree_animation_node* i	= m_to.time_root( );
		for ( ; i && i->time_synchronization_group_id( ) != animation.time_synchronization_group_id( ); i = i->m_next_time_animation )
			;

		if ( i && i != &animation ) {
			can_be_time_driving_animation	= false;
			operands_offset					= 0;
		}
	}

	u32 time_scale_operands_count;
	u32 animation_interval_id;
	float animation_interval_time;
	n_ary_tree_animation_node* const result	= new_animation(
		animation,
		animation,
		weight_driving_animation,
		1,
		time_scale_operands_count,
		operands_offset,
		animation_interval_id,
		animation_interval_time,
		true,
		can_be_time_driving_animation
	);

	n_ary_tree_base_node** new_operands	=
		static_cast< n_ary_tree_base_node** >( m_buffer.c_ptr( ) ) + time_scale_operands_count;
	m_buffer							+= ( time_scale_operands_count + 1 ) * sizeof( n_ary_tree_base_node* );

	if ( time_scale_operands_count < operands_offset )
		*new_operands++					= m_cloner.clone(
			**animation.operands( sizeof( n_ary_tree_animation_node ) ),
			1.f
		);

	n_ary_tree_weight_transition_node* const transition	=
		static_cast< n_ary_tree_weight_transition_node* >( m_buffer.c_ptr( ) );
	m_buffer							+= sizeof( n_ary_tree_weight_transition_node );

	u32 const multiplicands_count		= animation.operands_count( ) - (
		animation.operands_count( )
		&& (*animation.operands( sizeof( n_ary_tree_animation_node ) ))->is_time_scale( )
			? 1
			: 0
	);
	n_ary_tree_base_node* weight_from	= NULL;
	switch ( multiplicands_count ) {
		case 0:
			weight_from					= static_cast< n_ary_tree_base_node* >( m_buffer.c_ptr( ) );
			m_buffer					+= sizeof( n_ary_tree_weight_node );
			new ( weight_from ) n_ary_tree_weight_node( result->weight_interpolator( ), 1.f );
			break;

		case 1:
			weight_from					= m_cloner.clone(
				*animation.operands( sizeof( n_ary_tree_animation_node ) )[
					(*animation.operands( sizeof( n_ary_tree_animation_node ) ))->is_time_scale( ) ? 1 : 0
				]
			);
			break;

		default: {
			weight_from					= static_cast< n_ary_tree_base_node* >( m_buffer.c_ptr( ) );
			m_buffer					+= sizeof( n_ary_tree_multiplication_node );
			new ( weight_from ) n_ary_tree_multiplication_node( multiplicands_count );

			new_operands					= (n_ary_tree_base_node**)m_buffer.c_ptr( );
			m_buffer					+= multiplicands_count * sizeof( n_ary_tree_base_node* );

			n_ary_tree_base_node** multiplicands	= animation.operands( sizeof( n_ary_tree_animation_node ) )
				+ ( (*animation.operands( sizeof( n_ary_tree_animation_node ) ))->is_time_scale( ) ? 1 : 0 );
			for ( n_ary_tree_base_node** e = multiplicands + multiplicands_count; multiplicands != e; ++multiplicands )
				*new_operands++			= m_cloner.clone( **multiplicands );

			break;
		}
	}

	n_ary_tree_weight_node* const weight_to	=
		static_cast< n_ary_tree_weight_node* >( m_buffer.c_ptr( ) );
	m_buffer							+= sizeof( n_ary_tree_weight_node );
	new ( weight_to ) n_ary_tree_weight_node( *m_cloner.clone( interpolator ), 0.f );

	new ( transition ) n_ary_tree_weight_transition_node(
		*weight_from,
		*weight_to,
		*m_cloner.clone( interpolator ),
		m_current_time_in_ms
	);
	*new_operands						= transition;

	return								add_animation_node(
		*result,
		&animation.animation_state( ),
		animation_interval_id,
		animation_interval_time,
		false
	);
}

void n_ary_tree_transition_tree_constructor::remove_weight_synchronization_group( n_ary_tree_animation_node* begin, n_ary_tree_animation_node* end )
{
	n_ary_tree_animation_node* const new_animation	= remove_animation( *begin, 0, !begin->is_transitting_to_zero( ) );
	if ( !new_animation )
		return;

	n_ary_tree_animation_node* const weight_driving_animation	=
		new_animation->weight_synchronization_group_id( ) == u32( -1 ) ? 0 : new_animation;
	for ( n_ary_tree_animation_node* i = begin->m_next_weight_animation; i != end; i = i->m_next_weight_animation )
		remove_animation		( *i, weight_driving_animation, !begin->is_transitting_to_zero( ) );
}

n_ary_tree_animation_node* n_ary_tree_transition_tree_constructor::add_animation( n_ary_tree_animation_node& animation, n_ary_tree_animation_node* const weight_driving_animation )
{
	base_interpolator const& interpolator	= ( weight_driving_animation ? *weight_driving_animation : animation ).weight_interpolator( );

	u32 const to_operands_count		= animation.operands_count( ) - ( animation.operands_count( ) && (*animation.operands( sizeof( n_ary_tree_animation_node ) ))->is_time_scale( ) ? 1 : 0 );

	u32 operands_offset				= interpolator.transition_time( ) != 0.f ? 1 : 0;

	u32 time_scale_operands_count;
	u32 animation_interval_id;
	float animation_interval_time;
	n_ary_tree_animation_node* const result	= new_animation(
		animation,
		animation,
		weight_driving_animation,
		operands_offset + to_operands_count,
		time_scale_operands_count,
		operands_offset,
		animation_interval_id,
		animation_interval_time,
		false,
		true
	);

	bool has_weight_transition_been_added	= weight_driving_animation == 0
		&& animation.operands_count( ) && (*animation.operands( sizeof( n_ary_tree_animation_node ) ))->is_time_scale( );

	n_ary_tree_base_node** new_operands	= static_cast< n_ary_tree_base_node** >( m_buffer.c_ptr( ) );
	m_buffer						+= ( time_scale_operands_count + operands_offset ) * sizeof( n_ary_tree_base_node* );

	n_ary_tree_weight_node			temp( interpolator, 1.f );
	n_ary_tree_node_comparer		comparer;

	n_ary_tree_base_node** const	operands_end	= animation.operands( sizeof( n_ary_tree_animation_node ) ) + animation.operands_count( );
	n_ary_tree_base_node**			i				= animation.operands( sizeof( n_ary_tree_animation_node ) ) + ( has_weight_transition_been_added ? 1 : 0 );
	for ( ; i != operands_end; ++i ) {
		if ( comparer.compare( **i, temp ) == vostok::animation::more )
			break;

		*new_operands++				= (*i)->is_time_scale( ) ? m_cloner.clone( **i, 1.f ) : m_cloner.clone( **i );
	}

	if ( i != operands_end ) {
		if ( interpolator.transition_time( ) == 0.f )
			*new_operands++			= new_weight_transition( *m_cloner.clone( interpolator ), 0.f, 1.f );

		for ( ; i != operands_end; ++i )
			*new_operands++			= m_cloner.clone( **i );
	}
	else if ( interpolator.transition_time( ) == 0.f )
		*new_operands++				= new_weight_transition( *m_cloner.clone( interpolator ), 0.f, 1.f );

	stlp_std::sort					( animation.operands( sizeof( n_ary_tree_animation_node ) ), operands_end, comparer );

	return							add_animation_node( *result, &animation.animation_state( ), animation_interval_id, animation_interval_time, true );
}

void n_ary_tree_transition_tree_constructor::add_weight_synchronization_group( n_ary_tree_animation_node* begin, n_ary_tree_animation_node* end )
{
	n_ary_tree_animation_node* const new_animation	= add_animation( *begin, 0 );
	n_ary_tree_animation_node* const weight_driving_animation	=
		new_animation->weight_synchronization_group_id( ) == u32( -1 ) ? 0 : new_animation;
	for ( n_ary_tree_animation_node* i = begin->m_next_weight_animation; i != end; i = i->m_next_weight_animation )
		add_animation			( *i, weight_driving_animation );
}

n_ary_tree_animation_node* n_ary_tree_transition_tree_constructor::new_weight_driving_animation( n_ary_tree_animation_node& animation )
{
	base_interpolator const& interpolator	= animation.weight_interpolator( );
	n_ary_tree_base_node** operands_begin	= animation.operands( sizeof( n_ary_tree_animation_node ) );
	bool const has_time_scale				= animation.operands_count( )
		? (*operands_begin)->is_time_scale( )
		: false;
	u32 const weight_operands_count			= animation.operands_count( ) - ( has_time_scale ? 1 : 0 )
		+ ( interpolator.transition_time( ) != 0.f ? 1 : 0 );

	u32 time_scale_operands_count;
	u32 operands_offset						= has_time_scale;
	u32 animation_interval_id;
	float animation_interval_time;
	n_ary_tree_animation_node* const result	= new_animation(
		animation,
		animation,
		0,
		weight_operands_count,
		time_scale_operands_count,
		operands_offset,
		animation_interval_id,
		animation_interval_time,
		false,
		true
	);

	n_ary_tree_base_node** new_operands		=
		static_cast< n_ary_tree_base_node** >( m_buffer.c_ptr( ) ) + time_scale_operands_count;
	m_buffer								+= ( time_scale_operands_count + operands_offset )
		* sizeof( n_ary_tree_base_node* );

	n_ary_tree_base_node** const operands_end	=
		operands_begin + animation.operands_count( );
	n_ary_tree_base_node** i					=
		operands_begin + ( has_time_scale ? time_scale_operands_count : 0 );

	n_ary_tree_weight_node temp				( interpolator, 0.f );
	n_ary_tree_node_comparer comparer;
	for ( ; i != operands_end; ++i ) {
		if ( comparer.compare( **i, temp ) == vostok::animation::more )
			break;

		if ( (*i)->is_time_scale( ) )
			*new_operands++					= m_cloner.clone( **i, 0.f );
		else
			*new_operands++					= m_cloner.clone( **i );
	}

	if ( interpolator.transition_time( ) != 0.f )
		*new_operands++						=
			new_weight_transition( *m_cloner.clone( interpolator ), 0.f, 1.f );

	for ( ; i != operands_end; ++i )
		*new_operands++						= m_cloner.clone( **i );

	stlp_std::sort(
		operands_begin,
		operands_end,
		comparer
	);

	return									add_animation_node(
		*result,
		0,
		animation_interval_id,
		animation_interval_time,
		true
	);
}

n_ary_tree_base_node* n_ary_tree_transition_tree_constructor::new_time_scale_transition(
	n_ary_tree_animation_node&		from_animation,
	n_ary_tree_animation_node&		to_animation,
	n_ary_tree_base_node&			from,
	n_ary_tree_base_node&			to
)
{
	if ( n_ary_tree_node_comparer( ).compare( from, to ) == vostok::animation::equal )
		return				m_cloner.clone(
								from,
								0.f,
								to_animation.override_existing_animation( ) ? to_animation.animation_state( ).animation_interval_time : from_animation.animation_state( ).animation_interval_time
							);

	n_ary_tree_time_scale_node&	time_scale_to_node	= static_cast< n_ary_tree_time_scale_node& >( to );
	if ( time_scale_to_node.interpolator( ).transition_time( ) == 0.f )
		return				m_cloner.clone(
								to,
								0.f,
								to_animation.override_existing_animation( ) ? to_animation.animation_state( ).animation_interval_time : from_animation.animation_state( ).animation_interval_time
							);

	n_ary_tree_base_node* const result	= (n_ary_tree_base_node*)m_buffer.c_ptr( );
	m_buffer				+= sizeof( n_ary_tree_time_scale_transition_node );

	n_ary_tree_base_node* const time_scale_from	= m_cloner.clone( from, 1.f );
	n_ary_tree_base_node* const time_scale_to	= m_cloner.clone( to, 1.f );

	n_ary_tree_interpolator_selector	interpolator_selector;
	time_scale_to->accept	( interpolator_selector );
	base_interpolator const* const cloned_interpolator	= m_cloner.clone( *interpolator_selector.result() );

	if ( result )
		new ( result ) n_ary_tree_time_scale_transition_node(
			*time_scale_from,
			*time_scale_to,
			*cloned_interpolator,
			m_current_time_in_ms
		);

	return					result;
}

n_ary_tree_base_node* n_ary_tree_transition_tree_constructor::new_time_scale_transition( n_ary_tree_animation_node& from_animation, n_ary_tree_base_node& from, float to )
{
	n_ary_tree_interpolator_selector	interpolator_selector;
	from.accept				( interpolator_selector );

	if ( interpolator_selector.result()->transition_time() == 0.f ) {
		base_interpolator const* const cloned_interpolator	= m_cloner.clone( *interpolator_selector.result() );
		n_ary_tree_base_node* const result	= (n_ary_tree_base_node*)m_buffer.c_ptr( );
		m_buffer				+= sizeof( n_ary_tree_time_scale_node );
		new ( result ) n_ary_tree_time_scale_node(
			*cloned_interpolator,
			0.f,
			from_animation.animation_state( ).animation_interval_time,
			m_current_time_in_ms
		);

		return				result;
	}

	n_ary_tree_base_node* const result	= (n_ary_tree_base_node*)m_buffer.c_ptr( );
	m_buffer				+= sizeof( n_ary_tree_time_scale_transition_node );

	n_ary_tree_base_node* const time_scale_from	= m_cloner.clone( from, 1.f );
	base_interpolator const* const cloned_interpolator	= m_cloner.clone( *interpolator_selector.result() );

	n_ary_tree_base_node* const time_scale_to	= (n_ary_tree_base_node*)m_buffer.c_ptr( );
	m_buffer				+= sizeof( n_ary_tree_time_scale_node );
	new ( time_scale_to ) n_ary_tree_time_scale_node(
			*cloned_interpolator,
			0.f,
			from_animation.animation_state( ).animation_interval_time,
			m_current_time_in_ms
		);

	new ( result ) n_ary_tree_time_scale_transition_node(
		*time_scale_from,
		*time_scale_to,
		*cloned_interpolator,
		m_current_time_in_ms
	);

	return					result;
}

n_ary_tree_base_node* n_ary_tree_transition_tree_constructor::new_time_scale_transition( float const animation_time, float from, n_ary_tree_base_node& to )
{
	n_ary_tree_time_scale_node&	time_scale_to	= static_cast< n_ary_tree_time_scale_node& >( to );
	if ( time_scale_to.interpolator( ).transition_time( ) == 0.f )
		return				m_cloner.clone( to, 0.f, animation_time );

	n_ary_tree_base_node* const result	= (n_ary_tree_base_node*)m_buffer.c_ptr( );
	m_buffer				+= sizeof( n_ary_tree_time_scale_transition_node );

	n_ary_tree_base_node* const cloned_to	= m_cloner.clone( to, 1.f );

	n_ary_tree_interpolator_selector	interpolator_selector;
	cloned_to->accept		( interpolator_selector );

	base_interpolator const* const cloned_interpolator	= m_cloner.clone( *interpolator_selector.result() );

	n_ary_tree_base_node* const time_scale_from	= (n_ary_tree_base_node*)m_buffer.c_ptr( );
	m_buffer				+= sizeof( n_ary_tree_time_scale_node );
	new ( time_scale_from ) n_ary_tree_time_scale_node(
			*cloned_interpolator,
			0.f,
			from,
			m_current_time_in_ms
		);

	new ( result ) n_ary_tree_time_scale_transition_node(
		*time_scale_from,
		*cloned_to,
		*cloned_interpolator,
		m_current_time_in_ms
	);

	return					result;
}

n_ary_tree_base_node* n_ary_tree_transition_tree_constructor::new_weight_transition( n_ary_tree_base_node& from, n_ary_tree_base_node& to )
{
	if ( static_cast< n_ary_tree_weight_node& >( to ).interpolator( ).transition_time( ) == 0.f )
		return				m_cloner.clone( to );

	n_ary_tree_base_node* const result	= (n_ary_tree_base_node*)m_buffer.c_ptr( );
	m_buffer				+= sizeof( n_ary_tree_weight_transition_node );

	n_ary_tree_base_node* const weight_from	= m_cloner.clone( from );

	n_ary_tree_base_node* const cloned_to	= m_cloner.clone( to );
	n_ary_tree_interpolator_selector	interpolator_selector;
	cloned_to->accept			( interpolator_selector );
	base_interpolator const* const cloned_interpolator	= m_cloner.clone( *interpolator_selector.result( ) );

	new ( result ) n_ary_tree_weight_transition_node(
		*weight_from,
		*cloned_to,
		*cloned_interpolator,
		m_current_time_in_ms
	);

	return					result;
}

n_ary_tree_base_node* n_ary_tree_transition_tree_constructor::new_weight_transition( base_interpolator const& from_animation_interpolator, n_ary_tree_base_node& from, float to )
{
	if ( from_animation_interpolator.transition_time() == 0.f ) {
		base_interpolator const* const cloned_interpolator	= m_cloner.clone( from_animation_interpolator );
		n_ary_tree_base_node* const result	= (n_ary_tree_base_node*)m_buffer.c_ptr( );
		m_buffer				+= sizeof( n_ary_tree_weight_node );
		new ( result ) n_ary_tree_weight_node( *cloned_interpolator, 0.f );

		return				result;
	}

	n_ary_tree_weight_node	weight( from_animation_interpolator, to );
	if ( n_ary_tree_node_comparer( ).compare( from, weight ) == vostok::animation::equal )
		return				m_cloner.clone( from );

	n_ary_tree_base_node* const result	= (n_ary_tree_base_node*)m_buffer.c_ptr( );
	m_buffer				+= sizeof( n_ary_tree_weight_transition_node );

	n_ary_tree_base_node* const cloned_from	= m_cloner.clone( from );
	base_interpolator const* const cloned_interpolator	= m_cloner.clone( from_animation_interpolator );

	n_ary_tree_base_node* const weight_to	= (n_ary_tree_base_node*)m_buffer.c_ptr( );
	m_buffer				+= sizeof( n_ary_tree_weight_node );
	new ( weight_to ) n_ary_tree_weight_node( *cloned_interpolator, 0.f );

	new ( result ) n_ary_tree_weight_transition_node(
		*cloned_from,
		*weight_to,
		*cloned_interpolator,
		m_current_time_in_ms
	);

	return					result;
}

n_ary_tree_base_node* n_ary_tree_transition_tree_constructor::new_weight_transition( base_interpolator const& to_animation_interpolator, float from, n_ary_tree_base_node& to )
{
	n_ary_tree_weight_node&	weight_to_node	= static_cast< n_ary_tree_weight_node& >( to );
	if ( weight_to_node.interpolator( ).transition_time( ) == 0.f )
		return				m_cloner.clone( to );

	n_ary_tree_weight_node	weight( to_animation_interpolator, 0.f );
	if ( n_ary_tree_node_comparer( ).compare( weight_to_node, weight ) == vostok::animation::equal )
		return				m_cloner.clone( to );

	n_ary_tree_base_node* const result	= (n_ary_tree_base_node*)m_buffer.c_ptr( );
	m_buffer				+= sizeof( n_ary_tree_weight_transition_node );

	n_ary_tree_base_node* const cloned_to	= m_cloner.clone( to );

	n_ary_tree_interpolator_selector	interpolator_selector;
	cloned_to->accept		( interpolator_selector );
	base_interpolator const* const cloned_interpolator	= m_cloner.clone( *interpolator_selector.result() );

	n_ary_tree_base_node* const weight_from	= (n_ary_tree_base_node*)m_buffer.c_ptr( );
	m_buffer				+= sizeof( n_ary_tree_weight_node );
	new ( weight_from ) n_ary_tree_weight_node( *cloned_interpolator, 0.f );

	new ( result ) n_ary_tree_weight_transition_node(
		*weight_from,
		*cloned_to,
		*cloned_interpolator,
		m_current_time_in_ms
	);

	return					result;
}

void n_ary_tree_transition_tree_constructor::add_operands(
	n_ary_tree_animation_node&		from,
	n_ary_tree_animation_node&		to,
	n_ary_tree_base_node**			operands_begin,
	n_ary_tree_base_node**			operands_end,
	bool const						skip_time_scale_node
)
{
	n_ary_tree_base_node**				operands			= operands_begin;
	base_interpolator const&			from_interpolator	= from.weight_interpolator( );
	n_ary_tree_node_comparer	comparer;
	n_ary_tree_base_node** const		i_e	= from.operands( sizeof( n_ary_tree_animation_node ) ) + from.operands_count( );
	n_ary_tree_base_node**				i	= from.operands( sizeof( n_ary_tree_animation_node ) );
	n_ary_tree_base_node** const		j_e	= to.operands( sizeof( n_ary_tree_animation_node ) ) + to.operands_count( );
	n_ary_tree_base_node**				j	= to.operands( sizeof( n_ary_tree_animation_node ) );

	if ( i != i_e && (*i)->is_time_scale( ) ) {
		if ( j != j_e && (*j)->is_time_scale( ) ) {
			if ( !skip_time_scale_node )
				*operands++			= new_time_scale_transition( from, to, **i, **j );
			++i;
			++j;
		}
		else {
			if ( !skip_time_scale_node )
				*operands++			= new_time_scale_transition( from, **i, 1.f );
			++i;
		}
	}
	else {
		if ( j != j_e && (*j)->is_time_scale( ) ) {
			if ( !skip_time_scale_node )
				*operands++		= new_time_scale_transition( from.animation_state( ).animation_interval_time, 1.f, **j );
			++j;
		}
	}

	n_ary_tree_interpolator_selector	interpolator_selector;
	for ( ; i != i_e; ) {
		if ( j == j_e )
			break;

		if ( comparer.compare( **i, **j ) == vostok::animation::equal ) {
			*operands++			= m_cloner.clone( **i, &from_interpolator );
			++i;
			++j;
			continue;
		}

		(*i)->accept			( interpolator_selector );
		base_interpolator const* const i_interpolator	= interpolator_selector.result( );
		(*j)->accept			( interpolator_selector );
		base_interpolator const* const j_interpolator	= interpolator_selector.result( );
		switch ( vostok::animation::compare( *i_interpolator, *j_interpolator ) ) {
			case vostok::animation::equal :
				*operands++			= new_weight_transition( **j, **i );
				++i;
				++j;
				break;
			case vostok::animation::less :
				*operands++			= new_weight_transition( *i_interpolator, **i, 0.f );
				++i;
				break;
			case vostok::animation::more :
				*operands++			= new_weight_transition( *j_interpolator, 0.f, **j );
				++j;
				break;
			default : NODEFAULT( );
		}
	}

	for ( ; i != i_e; ++i )
		*operands++			= new_weight_transition( from_interpolator, **i, 0.f );

	for ( ; j != j_e; )
		*operands++			= new_weight_transition( to.weight_interpolator( ), 0.f, **j++ );

	if ( operands == operands_begin )
		return;

	stlp_std::sort			( operands_begin, operands_end, comparer );
}

std::pair< u32, u32 > computed_operands_count( n_ary_tree_animation_node& from, n_ary_tree_animation_node& to )
{
	n_ary_tree_base_node** const		i_e	= from.operands( sizeof( n_ary_tree_animation_node ) ) + from.operands_count( );
	n_ary_tree_base_node**				i	= from.operands( sizeof( n_ary_tree_animation_node ) );
	n_ary_tree_base_node** const		j_e	= to.operands( sizeof( n_ary_tree_animation_node ) ) + to.operands_count( );
	n_ary_tree_base_node**				j	= to.operands( sizeof( n_ary_tree_animation_node ) );

	u32									operands_count			= 0;
	u32									time_scale_nodes_count	= 0;
	n_ary_tree_node_comparer			comparer;

	if ( i != i_e && (*i)->is_time_scale( ) ) {
		++i;
		time_scale_nodes_count	= 1;
		if ( j != j_e && (*j)->is_time_scale( ) )
			++j;
	}
	else {
		if ( j != j_e && (*j)->is_time_scale( ) ) {
			time_scale_nodes_count	= 1;
			++j;
		}
	}

	n_ary_tree_interpolator_selector	interpolator_selector;
	while ( i != i_e && j != j_e ) {
		if ( comparer.compare( **i, **j ) == vostok::animation::equal ) {
			++operands_count;
			++i;
			++j;
			continue;
		}

		(*i)->accept			( interpolator_selector );
		base_interpolator const* const	i_interpolator	= interpolator_selector.result( );
		(*j)->accept			( interpolator_selector );
		switch ( compare( *i_interpolator, *interpolator_selector.result( ) ) ) {
			case animation::equal :
				++operands_count;
				++i;
				++j;
				break;
			case animation::less :
				++operands_count;
				++i;
				break;
			default :
				++operands_count;
				++j;
				break;
		}
	}

	for ( ; i != i_e; ) {
		++i;
		++operands_count;
	}

	for ( ; j != j_e; ) {
		++j;
		++operands_count;
	}

	return						std::make_pair( operands_count, time_scale_nodes_count );
}

n_ary_tree_animation_node* n_ary_tree_transition_tree_constructor::new_weight_driving_animation( n_ary_tree_animation_node& new_weight_driving_animation, n_ary_tree_animation_node& new_driving_animation_in_previous_target )
{
	std::pair< u32, u32 > const operands_counts	=
		computed_operands_count( new_driving_animation_in_previous_target, new_weight_driving_animation );
	u32 time_scale_operands_count;
	u32 operands_offset				= operands_counts.second;
	u32 animation_interval_id;
	float animation_interval_time;
	n_ary_tree_animation_node* const result	= new_animation(
		new_weight_driving_animation,
		new_driving_animation_in_previous_target,
		0,
		operands_counts.first,
		time_scale_operands_count,
		operands_offset,
		animation_interval_id,
		animation_interval_time,
		new_driving_animation_in_previous_target.is_transitting_to_zero( ),
		true
	);

	n_ary_tree_base_node** const operands	=
		static_cast< n_ary_tree_base_node** >( m_buffer.c_ptr( ) ) + time_scale_operands_count;
	m_buffer					+= ( time_scale_operands_count + operands_offset ) * sizeof( n_ary_tree_base_node* );
	add_operands(
		new_driving_animation_in_previous_target,
		new_weight_driving_animation,
		operands,
		operands + operands_offset,
		time_scale_operands_count != 0
	);

	return						add_animation_node(
		*result,
		&new_weight_driving_animation.animation_state( ),
		animation_interval_id,
		animation_interval_time,
		false
	);
}

void n_ary_tree_transition_tree_constructor::change_animation(
	n_ary_tree_animation_node&		from,
	n_ary_tree_animation_node&		to,
	n_ary_tree_animation_node* const	weight_driving_animation,
	bool							is_new_driving_animation
)
{
	if ( ( to.is_transitting_to_zero( ) && !from.is_transitting_to_zero( ) )
		|| ( is_new_driving_animation && to.animation_state( ).are_there_any_weight_transitions ) )
	{
		n_ary_tree_base_node** const	from_end	= to.operands( sizeof( n_ary_tree_animation_node ) ) + to.operands_count( );
		n_ary_tree_base_node**			multiplicands	= to.operands( sizeof( n_ary_tree_animation_node ) );
		n_ary_tree_base_node**			to_begin	= from.operands( sizeof( n_ary_tree_animation_node ) );
		n_ary_tree_base_node** const	to_end		= to_begin + from.operands_count( );

		u32								operands_offset	=
			( to.operands_count( ) && (*multiplicands)->is_time_scale( ) )
			|| ( from.operands_count( ) && (*to_begin)->is_time_scale( ) )
			? 1 : 0;

		u32								animation_interval_id;
		float							animation_interval_time;
		u32								time_scale_operands_count;

		n_ary_tree_animation_node* const	result	= new_animation(
			to,
			from,
			weight_driving_animation,
			1,
			time_scale_operands_count,
			operands_offset,
			animation_interval_id,
			animation_interval_time,
			from.is_transitting_to_zero( ),
			true
		);

		n_ary_tree_base_node** new_operands	=
			static_cast< n_ary_tree_base_node** >( m_buffer.c_ptr( ) ) + time_scale_operands_count;
		m_buffer				+= ( time_scale_operands_count + 1 ) * sizeof( n_ary_tree_base_node* );

		if ( !time_scale_operands_count ) {
			if ( to.operands_count( ) && (*multiplicands)->is_time_scale( ) ) {
				if ( from.operands_count( ) && (*to_begin)->is_time_scale( ) )
					*new_operands++	= new_time_scale_transition( to, from, **multiplicands++, **to_begin++ );
				else
					*new_operands++	= new_time_scale_transition( to, **multiplicands++, 1.f );
			}
			else {
				if ( from.operands_count( ) && (*to_begin)->is_time_scale( ) )
					*new_operands++	= new_time_scale_transition( to.animation_state( ).animation_interval_time, 1.f, **to_begin++ );
			}
		}

		u32								left_multiplicands_count	= u32( from_end - multiplicands );
		if ( left_multiplicands_count && (*multiplicands)->is_time_scale( ) )
			--left_multiplicands_count;

		n_ary_tree_base_node*			weight_from;
		switch ( left_multiplicands_count ) {
			case 0 :
				weight_from				= (n_ary_tree_base_node*)m_buffer.c_ptr( );
				m_buffer				+= sizeof( n_ary_tree_weight_node );
				new ( weight_from ) n_ary_tree_weight_node( result->weight_interpolator( ), 0.f );
				break;
			case 1 :
				weight_from				= m_cloner.clone( **(from_end - 1) );
				break;
			default : {
			weight_from				= (n_ary_tree_base_node*)m_buffer.c_ptr( );
			m_buffer				+= sizeof( n_ary_tree_multiplication_node );
			new ( weight_from ) n_ary_tree_multiplication_node( left_multiplicands_count );

			n_ary_tree_base_node**	operands	= (n_ary_tree_base_node**)m_buffer.c_ptr( );
			m_buffer				+= left_multiplicands_count * sizeof( n_ary_tree_base_node* );

			for ( n_ary_tree_base_node** i = multiplicands + ( operands_offset && (*multiplicands)->is_time_scale( ) ? 1 : 0 ); i != from_end; ++i )
				*operands++			= m_cloner.clone( **i );

			break;
		}
		}

		u32								right_multiplicands_count	= u32( to_end - to_begin );
		if ( right_multiplicands_count && (*to_begin)->is_time_scale( ) )
			--right_multiplicands_count;

		n_ary_tree_base_node*			weight_to;
		switch ( right_multiplicands_count ) {
			case 0 :
				weight_to				= (n_ary_tree_base_node*)m_buffer.c_ptr( );
				m_buffer				+= sizeof( n_ary_tree_weight_node );
				new ( weight_to ) n_ary_tree_weight_node( result->weight_interpolator( ), 0.f );
				break;
			case 1 :
				weight_to				= m_cloner.clone( **(to_end - 1) );
				break;
			default : {
			weight_to				= (n_ary_tree_base_node*)m_buffer.c_ptr( );
			m_buffer				+= sizeof( n_ary_tree_multiplication_node );
			new ( weight_to ) n_ary_tree_multiplication_node( right_multiplicands_count );

			n_ary_tree_base_node**	operands	= (n_ary_tree_base_node**)m_buffer.c_ptr( );
			m_buffer				+= right_multiplicands_count * sizeof( n_ary_tree_base_node* );

			for ( n_ary_tree_base_node** i = to_begin + ( operands_offset && (*to_begin)->is_time_scale( ) ? 1 : 0 ); i != to_end; ++i )
				*operands++			= m_cloner.clone( **i );

			break;
		}
		}

		n_ary_tree_base_node*	weight;
		if (
				left_multiplicands_count < 2 &&
				right_multiplicands_count < 2 &&
				!weight_from->is_transition( ) &&
				static_cast< n_ary_tree_weight_node& >( *weight_from ).weight( ) == static_cast< n_ary_tree_weight_node& >( *weight_to ).weight( )
			)
		{
			weight					= (n_ary_tree_base_node*)m_buffer.c_ptr( );
			m_buffer				+= sizeof( n_ary_tree_weight_node );
			base_interpolator const& interpolator	= ( weight_driving_animation ? *weight_driving_animation : to ).weight_interpolator( );
			new ( weight ) n_ary_tree_weight_node(
				*m_cloner.clone( interpolator ),
				static_cast< n_ary_tree_weight_node& >( *weight_to ).weight( )
			);
		}
		else {
			weight					= (n_ary_tree_base_node*)m_buffer.c_ptr( );
			m_buffer				+= sizeof( n_ary_tree_weight_transition_node );
			base_interpolator const& interpolator	= ( weight_driving_animation ? *weight_driving_animation : to ).weight_interpolator( );
			new ( weight ) n_ary_tree_weight_transition_node(
				*weight_from,
				*weight_to,
				*m_cloner.clone( interpolator ),
				m_current_time_in_ms
			);
		}

		*new_operands			= weight;

		add_animation_node(
			*result,
			&to.animation_state( ),
			animation_interval_id,
			animation_interval_time,
			false
		);
		return;
	}

	std::pair< u32, u32 > const operands_counts	= computed_operands_count( from, to );

	u32						time_scale_operands_count;
	u32						operands_offset				= operands_counts.second;
	u32						animation_interval_id;
	float					animation_interval_time;

	n_ary_tree_animation_node* const	result	= new_animation(
		to,
		from,
		weight_driving_animation,
		operands_counts.first,
		time_scale_operands_count,
		operands_offset,
		animation_interval_id,
		animation_interval_time,
		from.is_transitting_to_zero( ),
		true
	);

	n_ary_tree_base_node** const new_operands	=
		static_cast< n_ary_tree_base_node** >( m_buffer.c_ptr( ) ) + time_scale_operands_count;
	m_buffer					+= ( operands_counts.first + operands_offset ) * sizeof( n_ary_tree_base_node* );

	add_operands(
		from,
		to,
		new_operands,
		new_operands + operands_counts.first + operands_offset - time_scale_operands_count,
		time_scale_operands_count != 0
	);

	add_animation_node(
		*result,
		&to.animation_state( ),
		animation_interval_id,
		animation_interval_time,
		false
	);
}

void n_ary_tree_transition_tree_constructor::merge_weight_synchronization_groups(
	n_ary_tree_animation_node*		from_begin,
	n_ary_tree_animation_node*		from_end,
	n_ary_tree_animation_node*		to_begin,
	n_ary_tree_animation_node*		to_end,
	n_ary_tree_animation_node&		new_weight_driving_animation,
	bool							is_new_driving_animation
)
{
	u32 animations_count			= 0;
	for ( n_ary_tree_animation_node* i = from_begin; i != from_end; i = i->m_next_weight_animation )
		++animations_count;

	n_ary_tree_animation_node** const animations	=
		static_cast< n_ary_tree_animation_node** >(
			ALLOCA( animations_count * sizeof( n_ary_tree_animation_node* ) )
		);
	n_ary_tree_animation_node** i	= animations;
	bool new_driving_animation_in_old_target_found	= false;
	animation_comparer_equal_predicate equal_predicate( false, true );
	for ( n_ary_tree_animation_node* animation = from_begin; animation != from_end; animation = animation->m_next_weight_animation )
		if ( !equal_predicate( *animation, new_weight_driving_animation ) )
			*i++					= animation;
		else
			new_driving_animation_in_old_target_found	= true;

	if ( new_driving_animation_in_old_target_found )
		--animations_count;

	n_ary_tree_animation_node** const animations_end	= animations + animations_count;
	stlp_std::sort				( animations, animations_end, animation_comparer_less_predicate( false, true ) );

	i							= animations;
	n_ary_tree_animation_node* j	= to_begin;
	animation_comparer_predicate comparer( false, true );
	while ( i != animations_end && j != to_end ) {
		switch ( comparer( **i, *j ) ) {
			case equal :
				change_animation	( **i++, *j, &new_weight_driving_animation, is_new_driving_animation );
				j				= j->m_next_weight_animation;
				break;
			case less :
				remove_animation	( **i++, &new_weight_driving_animation, is_new_driving_animation );
				break;
			default :
				add_animation	( *j, &new_weight_driving_animation );
				j				= j->m_next_weight_animation;
				break;
		}
	}

	for ( ; i != animations_end; ++i )
		remove_animation			( **i, &new_weight_driving_animation, is_new_driving_animation );

	for ( ; j != to_end; j = j->m_next_weight_animation )
		add_animation				( *j, &new_weight_driving_animation );
}

void n_ary_tree_transition_tree_constructor::merge_weight_asynchronous_groups(
	n_ary_tree_animation_node* const	from_begin,
	n_ary_tree_animation_node* const	from_end,
	n_ary_tree_animation_node* const	to_begin,
	n_ary_tree_animation_node* const	to_end
)
{
	n_ary_tree_animation_node* i	= from_begin;
	n_ary_tree_animation_node* j	= to_begin;
	n_ary_tree_node_comparer comparer;
	while ( i && j ) {
		switch ( comparer.compare( *i, *j ) ) {
			case vostok::animation::equal :
				change_animation	( *i, *j, 0, false );
				i				= i->m_next_weight_animation;
				j				= j->m_next_weight_animation;
				break;
			case vostok::animation::less :
				remove_animation	( *i, 0, false );
				i				= i->m_next_weight_animation;
				break;
			default :
				add_animation	( *j, 0 );
				j				= j->m_next_weight_animation;
				break;
		}
	}

	for ( ; i; i = i->m_next_weight_animation )
		remove_animation			( *i, 0, false );

	for ( ; j; j = j->m_next_weight_animation )
		add_animation				( *j, 0 );
}

void n_ary_tree_transition_tree_constructor::change_weight_synchronization_group(
	n_ary_tree_animation_node*		from_begin,
	n_ary_tree_animation_node*		from_end,
	n_ary_tree_animation_node*		to_begin,
	n_ary_tree_animation_node*		to_end
)
{
	if ( to_begin->weight_synchronization_group_id( ) == u32( -1 ) ) {
		merge_weight_asynchronous_groups	( from_begin, from_end, to_begin, to_end );
		return;
	}

	n_ary_tree_animation_node* const previous_weight_driving_animation	=
		find_animation( from_begin, from_end, *to_begin );
	bool const is_new_driving_animation	= !previous_weight_driving_animation
		|| previous_weight_driving_animation->is_transitting_to_zero( );
	n_ary_tree_animation_node* const new_weight_driving_animation_node	=
		previous_weight_driving_animation
			? new_weight_driving_animation( *to_begin, *previous_weight_driving_animation )
			: new_weight_driving_animation( *to_begin );

	merge_weight_synchronization_groups(
		from_begin,
		from_end,
		to_begin,
		to_end,
		*new_weight_driving_animation_node,
		is_new_driving_animation
	);

}

n_ary_tree_animation_node* n_ary_tree_transition_tree_constructor::get_time_driving_animation( u32 time_synchronization_group_id ) const
{
	for ( n_ary_tree_animation_node** i = m_time_driving_animations_begin; i != m_time_driving_animations_end; ++i )
		if ( (*i)->time_synchronization_group_id( ) == time_synchronization_group_id )
			return				*i;

	return						NULL;
}

void n_ary_tree_transition_tree_constructor::merge_trees( n_ary_tree const& from, n_ary_tree const& to )
{
	n_ary_tree_animation_node* i_begin	= from.weight_root( );
	n_ary_tree_animation_node* i_end	= synchronization_group_end( i_begin );
	n_ary_tree_animation_node* j_begin	= to.weight_root( );
	n_ary_tree_animation_node* j_end	= synchronization_group_end( j_begin );
	while ( i_begin && j_begin ) {
		if ( i_begin->weight_synchronization_group_id( ) < j_begin->weight_synchronization_group_id( ) ) {
			remove_weight_synchronization_group	( i_begin, i_end );
			get_next_synchronization_group			( i_begin, i_end );
			continue;
		}

		if ( i_begin->weight_synchronization_group_id( ) > j_begin->weight_synchronization_group_id( ) ) {
			add_weight_synchronization_group		( j_begin, j_end );
			get_next_synchronization_group			( j_begin, j_end );
			continue;
		}

		change_weight_synchronization_group		( i_begin, i_end, j_begin, j_end );
		get_next_synchronization_group				( i_begin, i_end );
		get_next_synchronization_group				( j_begin, j_end );
	}

	while ( j_begin ) {
		add_weight_synchronization_group			( j_begin, j_end );
		get_next_synchronization_group				( j_begin, j_end );
	}

	while ( i_begin ) {
		remove_weight_synchronization_group		( i_begin, i_end );
		get_next_synchronization_group				( i_begin, i_end );
	}

	for ( n_ary_tree_animation_node* i = m_weight_root; i; i = i->m_next_weight_animation ) {
		u32 const group_id						= i->time_synchronization_group_id( );
		if ( group_id == u32( -1 ) )
			continue;

		n_ary_tree_animation_node* const driver	= get_time_driving_animation( group_id );
		if ( driver && driver != i )
			i->set_time_driving_animation			( *driver );
	}

	n_ary_tree_animation_node** const animations	=
		static_cast< n_ary_tree_animation_node** >(
			ALLOCA( m_animations_count * sizeof( n_ary_tree_animation_node* ) )
		);
	n_ary_tree_animation_node** animations_end	= animations;
	for ( n_ary_tree_animation_node* i = m_weight_root; i; i = i->m_next_weight_animation )
		*animations_end++						= i;

	stlp_std::sort								( animations, animations_end, time_animations_predicate( ) );
	if ( animations != animations_end ) {
		m_time_root								= *animations;
		for ( n_ary_tree_animation_node** i = animations + 1, **previous = animations; i != animations_end; ++i, ++previous )
			(*previous)->m_next_time_animation	= *i;
	}

	for ( n_ary_tree_animation_node* animation = m_time_root; animation; animation = animation->m_next_time_animation ) {
		animation_state_params const& params		=
			*reinterpret_cast< animation_state_params const* >( animation->get_animation_state( ) );

		u16 const initial_event_types			= params.initial_event_types;
		u32 animation_interval_id;
		float animation_interval_time;
		float animation_time_threshold;

		if ( animation->time_driving_animation( ) ) {
			n_ary_tree_animation_node& driver	= *animation->time_driving_animation( );
			animation_state const& driver_state	= driver.animation_state( );
			animation_interval_id				= driver_state.animation_interval_id;
			animation_interval const* const time_driving_animation_interval	=
				driver.animation_intervals( ) + animation_interval_id;
			animation_interval_time				=
				animation->animation_intervals( )[ animation_interval_id ].length( )
				/ time_driving_animation_interval->length( )
				* driver_state.animation_interval_time;
			animation_time_threshold			= 0.f;
		}
		else {
			animation_interval_id				= params.animation_interval_id;
			animation_interval_time				= params.animation_interval_time;
			animation_time_threshold			= params.animation_time_threshold;
		}

		float const weight						= params.weight;
		animation_state const* const previous	= params.previous;
		bool const is_freezed					= previous ? previous->is_freezed : false;

		animation_state* const state				= &animation->animation_state( );
		new ( state ) animation_state(
			*animation,
			m_current_time_in_ms,
			initial_event_types,
			animation_interval_id,
			animation_interval_id,
			animation_interval_time,
			animation_time_threshold,
			weight,
			m_channels_head,
			is_freezed
		);
		state->are_there_any_weight_transitions	= state->event_iterator.are_there_any_weight_transitions( );

		if ( previous )
			state->bone_matrices_computer			= previous->bone_matrices_computer;
	}

	m_time_driving_animations_begin				= NULL;
	m_time_driving_animations_end				= NULL;
}

n_ary_tree_transition_tree_constructor::n_ary_tree_transition_tree_constructor(
	mutable_buffer&					buffer,
	n_ary_tree const&				from,
	n_ary_tree const&				to,
	u32 const						animations_count,
	u32 const						animated_objects_count,
	u32 const						current_time_in_ms,
	subscribed_channel*&			channels_head,
	transform_functor_type const&	get_transform_functor
) :
	m_get_transform_functor			( get_transform_functor ),
	m_cloner						( *this, current_time_in_ms ),
	m_buffer						( buffer ),
	m_from							( from ),
	m_to							( to ),
	m_weight_root					( NULL ),
	m_time_root						( NULL ),
	m_time_driving_animations_begin	( NULL ),
	m_time_driving_animations_end	( NULL ),
	m_animation_states				( NULL ),
	m_new_animation_state			( NULL ),
	m_animation_events				( NULL ),
	m_animated_objects				( NULL ),
	m_new_animated_object			( NULL ),
	m_new_animation_event			( NULL ),
	m_reference_counter				( NULL ),
	m_previous_animation			( NULL ),
	m_channels_head					( channels_head ),
	m_current_time_in_ms			( current_time_in_ms ),
	m_animations_count				( animations_count ),
	m_animated_objects_count		( animated_objects_count )
{
	m_reference_counter				= static_cast< n_ary_tree_intrusive_base* >( m_buffer.c_ptr( ) );
	m_buffer						+= sizeof( n_ary_tree_intrusive_base );
	new ( m_reference_counter ) n_ary_tree_intrusive_base( );

	m_cloner.initialize				( from, to );

	m_animation_states				= static_cast< animation_state* >( m_buffer.c_ptr( ) );
	m_buffer						+= animations_count * sizeof( animation_state );
	m_new_animation_state			= m_animation_states;

	m_animation_events				= static_cast< animation_state** >( m_buffer.c_ptr( ) );
	m_buffer						+= animations_count * sizeof( animation_state* );
	m_new_animation_event			= m_animation_events;

	m_animated_objects				= static_cast< animated_object_holder* >( m_buffer.c_ptr( ) );
	m_buffer						+= animated_objects_count * sizeof( animated_object_holder );
	m_new_animated_object			= m_animated_objects;

	m_time_driving_animations_begin	=
		static_cast< n_ary_tree_animation_node** >(
			ALLOCA( animations_count * sizeof( n_ary_tree_animation_node* ) )
		);
	m_time_driving_animations_end	= m_time_driving_animations_begin;

	merge_trees						( from, to );
}

n_ary_tree n_ary_tree_transition_tree_constructor::computed_tree( )
{
	return
		n_ary_tree(
			m_weight_root,
			m_time_root,
			m_cloner.interpolators(),
			m_animation_states,
			m_animation_events,
			m_animated_objects,
			m_reference_counter,
			m_animations_count,
			m_animated_objects_count,
			m_cloner.interpolators_count(),
			m_current_time_in_ms
		);
}

} // namespace mixing
} // namespace animation
} // namespace vostok
