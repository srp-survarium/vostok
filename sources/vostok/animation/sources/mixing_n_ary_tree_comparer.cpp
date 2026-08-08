////////////////////////////////////////////////////////////////////////////
//	Created		: 04.03.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "mixing_n_ary_tree_comparer.h"
#include "mixing_n_ary_tree_animation_node.h"
#include "mixing_n_ary_tree_weight_node.h"
#include "mixing_n_ary_tree_time_scale_node.h"
#include "mixing_n_ary_tree_weight_transition_node.h"
#include "mixing_n_ary_tree_time_scale_transition_node.h"
#include "mixing_n_ary_tree_multiplication_node.h"
#include <vostok/animation/mixing_n_ary_tree.h>
#include "mixing_n_ary_tree_node_comparer.h"
#include "mixing_n_ary_tree_size_calculator.h"
#include "interpolator_size_calculator.h"
#include "mixing_n_ary_tree_interpolator_selector.h"
#include "mixing_n_ary_tree_time_scale_calculator.h"
#include "mixing_n_ary_tree_transition_tree_constructor_impl.h"
#include "mixing_n_ary_tree_target_time_scale_calculator.h"
#include "mixing_animation_state.h"
#include "interpolator_comparer.h"

#include <vostok/animation/mixing_animated_object_holder.h>

namespace vostok {
namespace animation {
namespace mixing {

void n_ary_tree_comparer::increase_buffer_size( n_ary_tree_base_node& node )
{
	n_ary_tree_size_calculator	calculator( this );
	node.accept					( calculator );
}

bool n_ary_tree_comparer::equal( ) const
{
	return						m_equal;
}

u32 n_ary_tree_comparer::needed_buffer_size( ) const
{
	return						m_needed_buffer_size;
}

comparison_result_enum animation_comparer_predicate::operator()(
	n_ary_tree_animation_node const& left,
	n_ary_tree_animation_node const& right
) const
{
	if ( left.weight_synchronization_group_id() > right.weight_synchronization_group_id() )
		return									less;
	if ( left.weight_synchronization_group_id() < right.weight_synchronization_group_id() )
		return									more;

	if ( left.time_synchronization_group_id() > right.time_synchronization_group_id() )
		return									less;
	if ( left.time_synchronization_group_id() < right.time_synchronization_group_id() )
		return									more;

	if ( left.animated_object() > right.animated_object() )
		return									less;
	if ( left.animated_object() < right.animated_object() )
		return									more;

	if ( left.bones_mask() > right.bones_mask() )
		return									less;
	if ( left.bones_mask() < right.bones_mask() )
		return									more;

	if ( left.playback_type() > right.playback_type() )
		return									less;
	if ( left.playback_type() < right.playback_type() )
		return									more;

	if ( m_use_synchronized_animations && ( left.weight_driving_animation() || right.weight_driving_animation() ) ) {
		switch ( ( *this )(
			right.weight_driving_animation() ? *right.weight_driving_animation() : right,
			left.weight_driving_animation() ? *left.weight_driving_animation() : left
		) ) {
			case less:	return				less;
			case more:	return				more;
		}
	}

	if ( left.unique_animation_id() > right.unique_animation_id() )
		return									less;
	if ( left.unique_animation_id() < right.unique_animation_id() )
		return									more;

	if ( left.can_generate_events() != right.can_generate_events() )
		return									left.can_generate_events() ? less : more;

	switch ( compare_animation_intervals( left, right ) ) {
		case less:	return					less;
		case more:	return					more;
	}

	if ( m_use_overriding_animations && left.override_existing_animation() != right.override_existing_animation() )
		return									left.override_existing_animation() ? less : more;

	return										equal;
}

void n_ary_tree_comparer::process_interpolators( n_ary_tree const& from, n_ary_tree const& to )
{
	u32 const from_interpolators_count				= from.interpolators_count( );
	base_interpolator const* const* const to_interpolators_begin	= to.interpolators( );
	base_interpolator const** const interpolators	= static_cast< base_interpolator const** >(
		ALLOCA( sizeof( base_interpolator const* ) * ( from_interpolators_count + to.interpolators_count( ) ) )
	);
	base_interpolator const** const merged_end		= std::merge(
		from.interpolators( ),
		from.interpolators( ) + from_interpolators_count,
		to_interpolators_begin,
		to_interpolators_begin + to.interpolators_count( ),
		interpolators,
		merge_interpolators_predicate( )
	);
	base_interpolator const** const end				= std::unique(
		interpolators,
		merged_end,
		unique_interpolators_predicate( )
	);

	u32 const interpolators_count					= end - interpolators;
	m_equal											= m_equal && from_interpolators_count == interpolators_count;

	interpolator_size_calculator size_calculator	( this );
	for ( base_interpolator const** i = interpolators; i != end; ++i )
		(*i)->accept									( size_calculator );

	m_needed_buffer_size							+= interpolators_count * sizeof( base_interpolator const* );
}

bool n_ary_tree_comparer::new_time_scale( n_ary_tree_animation_node& new_time_driving_animation )
{
	n_ary_tree_animation_node* previous_time_driving_animation	= m_from.time_root( );
	for ( ; previous_time_driving_animation; previous_time_driving_animation = previous_time_driving_animation->m_next_weight_animation ) {
		if ( previous_time_driving_animation->time_driving_animation( ) )
			continue;
		if ( previous_time_driving_animation->time_synchronization_group_id( ) == new_time_driving_animation.time_synchronization_group_id( ) )
			break;
	}

	if ( !previous_time_driving_animation || previous_time_driving_animation->animation_state( ).is_freezed )
		return								false;

	n_ary_tree_animation_node* target_time_driving_animation	= m_to.time_root( );
	for ( ; target_time_driving_animation; target_time_driving_animation = target_time_driving_animation->m_next_time_animation )
		if ( target_time_driving_animation->time_synchronization_group_id( ) == new_time_driving_animation.time_synchronization_group_id( ) )
			break;
	if ( target_time_driving_animation && target_time_driving_animation != &new_time_driving_animation )
		return								false;

	u32 const animation_interval_id		=
		previous_time_driving_animation->animation_state( ).animation_interval_id;
	float const new_driving_animation_length	=
		new_time_driving_animation.animation_intervals( )[ animation_interval_id ].length( )
		/ previous_time_driving_animation->animation_intervals( )[ animation_interval_id ].length( );
	float const directional_time_scale_factor	=
		new_time_driving_animation.is_positive_event_direction( )
			== previous_time_driving_animation->is_positive_event_direction( )
				? new_driving_animation_length
				: -new_driving_animation_length;

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

	float const time_scale_factor			=
		target_time_scale_node ? time_scale_calculator.time_scale( ) : 1.f;
	base_interpolator const* const time_scale_interpolator	=
		target_time_scale_node
			? time_scale_calculator.interpolator( )
			: &new_time_driving_animation.weight_interpolator( );
	if ( !target_time_scale_node && time_scale_interpolator->transition_time( ) <= 0.f )
		return								false;

	n_ary_tree_base_node* previous_time_scale_node	= NULL;
	if ( previous_time_driving_animation->operands_count( ) ) {
		n_ary_tree_base_node* const node	=
			*previous_time_driving_animation->operands( sizeof( n_ary_tree_animation_node ) );
		if ( node && node->is_time_scale( ) ) {
			previous_time_scale_node		= node;
			increase_buffer_size			( *node );
		}
	}

	n_ary_tree_target_time_scale_calculator target_time_scale_calculator( *previous_time_driving_animation );
	float const previous_target_time_scale	=
		target_time_scale_calculator.result( ) * directional_time_scale_factor;
	if ( time_scale_factor == previous_target_time_scale
		&& ( !target_time_scale_node || !target_time_scale_node->is_transition( ) ) )
	{
		if ( !previous_time_scale_node )
			m_needed_buffer_size			+= sizeof( n_ary_tree_time_scale_node );
		m_needed_buffer_size				+= sizeof( n_ary_tree_time_scale_node )
			+ sizeof( n_ary_tree_time_scale_transition_node );
		m_equal							= false;
		return							true;
	}

	return									previous_time_scale_node != NULL;
}

void n_ary_tree_comparer::new_animation(
	n_ary_tree_animation_node&		animation,
	u32&							time_scale_operands_count,
	u32&							operands_offset
)
{
	time_scale_operands_count	= 0;
	if ( !animation.time_driving_animation( ) && animation.time_synchronization_group_id( ) != u32( -1 ) ) {
		if ( new_time_scale( animation ) ) {
			operands_offset				= 1;
			time_scale_operands_count	= 1;
		}
	}

	++m_animations_count;
	m_needed_buffer_size	+= sizeof( n_ary_tree_animation_node )
		+ animation.animation_intervals_count( ) * sizeof( animation_interval )
		+ sizeof( animation_state* );

	animated_object_holder* const i	= std::find( m_animated_objects, m_animated_objects_end, animation.animated_object( ) );
	if ( i == m_animated_objects_end )
		new ( m_animated_objects_end++ ) animated_object_holder( animation.animated_object( ) );
}

void n_ary_tree_comparer::new_weight_transition( float from, float to )
{
	m_needed_buffer_size		+= sizeof( n_ary_tree_weight_transition_node ) + 2 * sizeof( n_ary_tree_weight_node );
	m_equal						= false;
}

void n_ary_tree_comparer::remove_animation(
	n_ary_tree_animation_node&			animation,
	n_ary_tree_animation_node const*	weight_driving_animation,
	bool								is_new_driving_animation
)
{
	base_interpolator const& interpolator	= ( weight_driving_animation ? *weight_driving_animation : animation ).weight_interpolator( );
	if ( interpolator.transition_time( ) == 0.f )
		return;

	if ( animation.is_transitting_to_zero( ) && !is_new_driving_animation ) {
		u32 operands_offset	= animation.operands_count( ) && (*animation.operands( sizeof( n_ary_tree_animation_node ) ))->is_time_scale( ) ? 1 : 0;
		u32 operands_count	= animation.operands_count( ) - operands_offset;

		if ( animation.time_synchronization_group_id( ) != u32( -1 ) ) {
			n_ary_tree_animation_node* i	= m_to.time_root( );
			for ( ; i && i->time_synchronization_group_id( ) != animation.time_synchronization_group_id( ); i = i->m_next_time_animation )
				;

			if ( i && i != &animation )
				operands_offset	= 0;
		}

		u32 time_scale_operands_count;
		new_animation			( animation, time_scale_operands_count, operands_offset );
		operands_count			+= operands_offset;

		n_ary_tree_base_node* const* const end	= animation.operands( sizeof( n_ary_tree_animation_node ) ) + animation.operands_count( );
		for ( n_ary_tree_base_node* const* i = animation.operands( sizeof( n_ary_tree_animation_node ) ); i != end; ++i ) {
			if ( !(*i)->is_time_scale( ) || !time_scale_operands_count )
				increase_buffer_size	( **i );
		}

		m_needed_buffer_size	+= operands_count * sizeof( n_ary_tree_base_node* );
		return;
	}

	m_equal						= false;
	u32 operands_offset			= !animation.time_driving_animation( )
		&& animation.operands_count( )
		&& (*animation.operands( sizeof( n_ary_tree_animation_node ) ))->is_time_scale( ) ? 1 : 0;

	if ( animation.time_synchronization_group_id( ) != u32( -1 ) ) {
		n_ary_tree_animation_node* i	= m_to.time_root( );
		for ( ; i && i->time_synchronization_group_id( ) != animation.time_synchronization_group_id( ); i = i->m_next_time_animation )
			;

		if ( i && i != &animation )
			operands_offset		= 0;
	}

	u32 time_scale_operands_count;
	new_animation				( animation, time_scale_operands_count, operands_offset );
	m_needed_buffer_size		+= ( operands_offset + 1 ) * sizeof( n_ary_tree_base_node* );

	if ( time_scale_operands_count < operands_offset )
		increase_buffer_size	( **animation.operands( sizeof( n_ary_tree_animation_node ) ) );

	m_needed_buffer_size		+= sizeof( n_ary_tree_weight_transition_node );

	u32 const operands_count	= animation.operands_count( ) - (
		animation.operands_count( )
		&& (*animation.operands( sizeof( n_ary_tree_animation_node ) ))->is_time_scale( ) ? 1 : 0
	);

	switch ( operands_count ) {
		case 0:
			m_needed_buffer_size	+= sizeof( n_ary_tree_weight_node );
			break;
		case 1:
			increase_buffer_size	( *animation.operands( sizeof( n_ary_tree_animation_node ) )[
				animation.operands_count( ) && (*animation.operands( sizeof( n_ary_tree_animation_node ) ))->is_time_scale( ) ? 1 : 0
			] );
			break;
		default: {
			m_needed_buffer_size	+= sizeof( n_ary_tree_multiplication_node ) + operands_count * sizeof( n_ary_tree_base_node* );
			n_ary_tree_base_node* const* const end	= animation.operands( sizeof( n_ary_tree_animation_node ) ) + animation.operands_count( );
			n_ary_tree_base_node* const* i			= animation.operands( sizeof( n_ary_tree_animation_node ) )
				+ ( (*animation.operands( sizeof( n_ary_tree_animation_node ) ))->is_time_scale( ) ? 1 : 0 );
			for ( ; i != end; ++i )
				increase_buffer_size	( **i );
			break;
		}
	}

	m_needed_buffer_size		+= sizeof( n_ary_tree_weight_node );
}

void n_ary_tree_comparer::remove_weight_synchronization_group( n_ary_tree_animation_node* begin, n_ary_tree_animation_node* end )
{
	if ( begin->weight_interpolator( ).transition_time( ) == 0.f ) {
		m_equal					= false;
		return;
	}

	remove_animation			( *begin, 0, !begin->is_transitting_to_zero( ) );

	n_ary_tree_animation_node const* const weight_driving_animation	=
		begin->weight_synchronization_group_id( ) == u32( -1 ) ? 0 : begin;
	for ( n_ary_tree_animation_node* i = begin->m_next_weight_animation; i != end; i = i->m_next_weight_animation )
		remove_animation		( *i, weight_driving_animation, !i->is_transitting_to_zero( ) );
}

void n_ary_tree_comparer::add_animation(
	n_ary_tree_animation_node&			animation,
	n_ary_tree_animation_node* const	weight_driving_animation
)
{
	m_equal								= false;
	base_interpolator const& interpolator	= ( weight_driving_animation ? *weight_driving_animation : animation ).weight_interpolator( );
	u32 operands_count						= animation.operands_count( ) + ( interpolator.transition_time( ) != 0.f ? 1 : 0 );

	u32 time_scale_operands_count;
	u32 operands_offset					= 0;
	new_animation						( animation, time_scale_operands_count, operands_offset );
	if ( operands_count && operands_offset && (*animation.operands( sizeof( n_ary_tree_animation_node ) ))->is_time_scale( ) && !time_scale_operands_count )
		--operands_count;

	m_needed_buffer_size				+= operands_count * sizeof( n_ary_tree_base_node* );

	n_ary_tree_base_node* const* const end	= animation.operands( sizeof( n_ary_tree_animation_node ) ) + animation.operands_count( );
	n_ary_tree_base_node* const* i			= animation.operands( sizeof( n_ary_tree_animation_node ) ) + operands_offset;
	for ( ; i != end; ++i )
		increase_buffer_size				( **i );

	if ( interpolator.transition_time( ) != 0.f )
		new_weight_transition				( 0.f, 1.f );
}

void n_ary_tree_comparer::add_weight_synchronization_group( n_ary_tree_animation_node* begin, n_ary_tree_animation_node* end )
{
	m_equal						= false;
	n_ary_tree_animation_node* const weight_driving_animation	= ( begin->weight_synchronization_group_id() == u32( -1 ) ) ? 0 : begin;
	for ( n_ary_tree_animation_node* i = begin; i != end; i = i->m_next_weight_animation )
		add_animation			( *i, weight_driving_animation );
}

void n_ary_tree_comparer::new_weight_driving_animation( n_ary_tree_animation_node& animation )
{
	m_equal								= false;
	bool const has_time_scale				= animation.operands_count( )
		&& (*animation.operands( sizeof( n_ary_tree_animation_node ) ))->is_time_scale( );
	base_interpolator const& interpolator	= animation.weight_interpolator( );
	u32 const weight_operands_count			= animation.operands_count( ) - ( has_time_scale ? 1 : 0 )
		+ ( interpolator.transition_time( ) != 0.f ? 1 : 0 );

	u32 time_scale_operands_count;
	u32 operands_offset						= has_time_scale ? 1 : 0;
	new_animation							( animation, time_scale_operands_count, operands_offset );
	m_needed_buffer_size					+= ( weight_operands_count + time_scale_operands_count )
		* sizeof( n_ary_tree_base_node* );

	n_ary_tree_base_node* const* const operands_end	=
		animation.operands( sizeof( n_ary_tree_animation_node ) ) + animation.operands_count( );
	n_ary_tree_base_node* const* i					=
		animation.operands( sizeof( n_ary_tree_animation_node ) )
			+ ( has_time_scale ? time_scale_operands_count : 0 );

	n_ary_tree_weight_node temp				( interpolator, 0.f );
	n_ary_tree_node_comparer comparer;
	for ( ; i != operands_end; ++i ) {
		if ( comparer.compare( **i, temp ) == vostok::animation::more )
			break;

		increase_buffer_size					( **i );
	}

	if ( interpolator.transition_time( ) != 0.f )
		m_needed_buffer_size				+= sizeof( n_ary_tree_weight_transition_node )
			+ sizeof( n_ary_tree_weight_node );

	for ( ; i != operands_end; ++i )
		increase_buffer_size					( **i );
}

void n_ary_tree_comparer::new_time_scale_transition( n_ary_tree_base_node& from, n_ary_tree_base_node& to )
{
	n_ary_tree_node_comparer comparer;
	if ( comparer.compare( from, to ) == vostok::animation::equal ) {
		increase_buffer_size		( from );
		return;
	}

	m_equal						= false;
	base_interpolator const& interpolator	= static_cast< n_ary_tree_time_scale_node& >( to ).interpolator( );
	if ( interpolator.transition_time( ) == 0.f ) {
		increase_buffer_size		( to );
		return;
	}

	increase_buffer_size		( from );
	increase_buffer_size		( to );
	m_needed_buffer_size		+= sizeof( n_ary_tree_time_scale_transition_node );
}

void n_ary_tree_comparer::new_time_scale_transition( n_ary_tree_base_node& from, float to )
{
	n_ary_tree_interpolator_selector interpolator_selector;
	from.accept				( interpolator_selector );
	if ( interpolator_selector.result( )->transition_time( ) == 0.f ) {
		m_needed_buffer_size	+= sizeof( n_ary_tree_time_scale_node );
		return;
	}

	m_equal					= false;
	increase_buffer_size	( from );
	m_needed_buffer_size	+= sizeof( n_ary_tree_time_scale_node ) + sizeof( n_ary_tree_time_scale_transition_node );
}

void n_ary_tree_comparer::new_time_scale_transition( float from, n_ary_tree_base_node& to )
{
	m_equal						= false;
	base_interpolator const& interpolator	= static_cast< n_ary_tree_time_scale_node& >( to ).interpolator( );
	if ( interpolator.transition_time( ) == 0.f ) {
		increase_buffer_size		( to );
		return;
	}

	m_needed_buffer_size		+= sizeof( n_ary_tree_time_scale_node );
	increase_buffer_size		( to );
	m_needed_buffer_size		+= sizeof( n_ary_tree_time_scale_transition_node );
}

void n_ary_tree_comparer::new_weight_transition( n_ary_tree_base_node& from, n_ary_tree_base_node& to )
{
	m_equal						= false;
	base_interpolator const& interpolator	= static_cast< n_ary_tree_weight_node& >( to ).interpolator( );
	if ( interpolator.transition_time( ) == 0.f ) {
		increase_buffer_size		( to );
		return;
	}

	increase_buffer_size		( from );
	increase_buffer_size		( to );
	m_needed_buffer_size		+= sizeof( n_ary_tree_weight_transition_node );
}

void n_ary_tree_comparer::new_weight_transition(
	base_interpolator const&	from_animation_interpolator,
	n_ary_tree_base_node&		from,
	float						to
)
{
	if ( from_animation_interpolator.transition_time( ) == 0.f ) {
		m_needed_buffer_size	+= sizeof( n_ary_tree_weight_node );
		m_equal					= false;
		return;
	}

	increase_buffer_size		( from );
	n_ary_tree_weight_node weight( from_animation_interpolator, to );
	n_ary_tree_node_comparer comparer;
	if ( comparer.compare( from, weight ) == vostok::animation::equal )
		return;

	m_needed_buffer_size		+= sizeof( n_ary_tree_weight_transition_node ) + sizeof( n_ary_tree_weight_node );
	m_equal						= false;
}

void n_ary_tree_comparer::new_weight_transition(
	base_interpolator const&	to_animation_interpolator,
	float						from,
	n_ary_tree_base_node&		to
)
{
	increase_buffer_size		( to );
	n_ary_tree_weight_node weight( to_animation_interpolator, from );
	n_ary_tree_node_comparer comparer;
	if ( comparer.compare( weight, to ) == vostok::animation::equal )
		return;

	m_equal						= false;
	if ( to_animation_interpolator.transition_time( ) != 0.f )
		m_needed_buffer_size	+= sizeof( n_ary_tree_weight_transition_node ) + sizeof( n_ary_tree_weight_node );
}

void n_ary_tree_comparer::add_operands(
	n_ary_tree_animation_node&		from,
	n_ary_tree_animation_node&		to,
	const bool						skip_time_scale_node
)
{
	n_ary_tree_node_comparer			comparer;
	n_ary_tree_base_node** const		i_e	= from.operands( sizeof( n_ary_tree_animation_node ) ) + from.operands_count( );
	n_ary_tree_base_node**				i	= from.operands( sizeof( n_ary_tree_animation_node ) );
	n_ary_tree_base_node** const		j_e	= to.operands( sizeof( n_ary_tree_animation_node ) ) + to.operands_count( );
	n_ary_tree_base_node**				j	= to.operands( sizeof( n_ary_tree_animation_node ) );

	if ( i != i_e && (*i)->is_time_scale( ) ) {
		if ( j != j_e && (*j)->is_time_scale( ) ) {
			if ( !skip_time_scale_node )
				new_time_scale_transition	( **i, **j );
			++i;
			++j;
		}
		else {
			if ( !skip_time_scale_node )
				new_time_scale_transition	( **i, to.weight_interpolator( ).transition_time( ) );
			++i;
		}
	}
	else if ( j != j_e && (*j)->is_time_scale( ) ) {
		if ( !skip_time_scale_node )
			new_time_scale_transition		( from.weight_interpolator( ).transition_time( ), **j );
		++j;
	}

	n_ary_tree_interpolator_selector	interpolator_selector;
	for ( ; i != i_e; ) {
		if ( j == j_e )
			break;

		switch ( comparer.compare( **i, **j ) ) {
			case vostok::animation::equal :
				new_weight_transition			( **i, **j );
				++i;
				++j;
				break;
			case vostok::animation::less :
				(*i)->accept					( interpolator_selector );
				new_weight_transition			( *interpolator_selector.result( ), **i, 0.f );
				++i;
				break;
			case vostok::animation::more :
				(*j)->accept					( interpolator_selector );
				new_weight_transition			( *interpolator_selector.result( ), 0.f, **j );
				++j;
				break;
			default : NODEFAULT( );
		}
	}

	for ( ; i != i_e; ++i )
		new_weight_transition				( from.weight_interpolator( ), **i, 0.f );

	for ( ; j != j_e; ++j )
		new_weight_transition				( to.weight_interpolator( ), 0.f, **j );
}

void n_ary_tree_comparer::new_weight_driving_animation(
	n_ary_tree_animation_node&		new_weight_driving_animation,
	n_ary_tree_animation_node&		new_driving_animation_in_previous_target
)
{
	if ( new_driving_animation_in_previous_target.weight_driving_animation( ) )
		m_equal					= false;

	if ( new_weight_driving_animation.override_existing_animation( )
		&& new_weight_driving_animation.animation_state( ).animation_time
			!= new_driving_animation_in_previous_target.animation_state( ).animation_time )
		m_equal					= false;

	interpolator_comparer comparer;
	new_driving_animation_in_previous_target.weight_interpolator( ).accept(
		comparer,
		new_weight_driving_animation.weight_interpolator( )
	);
	m_equal						= m_equal && comparer.result == animation::equal;

	if ( new_weight_driving_animation.override_existing_animation( ) )
		m_equal					= m_equal
			&& new_driving_animation_in_previous_target.animation_state( ).animation_interval_id
				== new_weight_driving_animation.animation_state( ).animation_interval_id
			&& new_driving_animation_in_previous_target.animation_state( ).animation_interval_time
				== new_weight_driving_animation.animation_state( ).animation_interval_time;

	std::pair< u32, u32 > const operands_counts	=
		computed_operands_count( new_driving_animation_in_previous_target, new_weight_driving_animation );
	u32 const operands_count		= operands_counts.first;
	u32 operands_offset				= operands_counts.second;
	u32 time_scale_operands_count;
	new_animation					( new_weight_driving_animation, time_scale_operands_count, operands_offset );
	m_needed_buffer_size			+= ( operands_count + operands_offset ) * sizeof( n_ary_tree_base_node* );
	add_operands(
		new_driving_animation_in_previous_target,
		new_weight_driving_animation,
		operands_offset != 0
	);
}

void n_ary_tree_comparer::change_animation(
	n_ary_tree_animation_node&		from,
	n_ary_tree_animation_node&		to,
	bool							is_new_driving_animation
)
{
	if ( ( to.is_transitting_to_zero( ) && !from.is_transitting_to_zero( ) )
		|| ( is_new_driving_animation && to.animation_state( ).are_there_any_weight_transitions ) )
	{
		m_equal						= false;

		n_ary_tree_base_node**		to_begin	= to.operands( sizeof( n_ary_tree_animation_node ) );
		n_ary_tree_base_node** const	to_end		= to_begin + to.operands_count( );
		n_ary_tree_base_node**		from_begin	= from.operands( sizeof( n_ary_tree_animation_node ) );
		n_ary_tree_base_node** const	from_end	= from_begin + from.operands_count( );

		u32 operands_offset			=
			( to_begin != to_end && (*to_begin)->is_time_scale( ) )
			|| ( from_begin != from_end && (*from_begin)->is_time_scale( ) )
				? 1
				: 0;
		u32 time_scale_operands_count;
		new_animation				( from, time_scale_operands_count, operands_offset );
		m_needed_buffer_size		+= ( operands_offset + 1 ) * sizeof( n_ary_tree_base_node* );

		if ( !time_scale_operands_count ) {
			if ( to_begin != to_end && (*to_begin)->is_time_scale( ) ) {
				if ( from_begin != from_end && (*from_begin)->is_time_scale( ) ) {
					new_time_scale_transition	( **to_begin, **from_begin );
					++from_begin;
				}
				else
					new_time_scale_transition	( **to_begin, 0.f );
				++to_begin;
			}
			else if ( from_begin != from_end && (*from_begin)->is_time_scale( ) ) {
				new_time_scale_transition		( 0.f, **from_begin );
				++from_begin;
			}
		}

		u32 const left_multiplicands_count	= u32( to_end - to_begin );
		bool left_is_pure_weight				= false;
		float left_weight					= math::float_max;
		switch ( left_multiplicands_count ) {
			case 0 :
				m_needed_buffer_size			+= sizeof( n_ary_tree_weight_node );
				left_is_pure_weight			= true;
				left_weight					= 0.f;
				break;
			case 1 :
				increase_buffer_size			( **to_begin );
				if ( (*to_begin)->is_weight( ) && !(*to_begin)->is_transition( ) ) {
					left_is_pure_weight		= true;
					left_weight				= static_cast< n_ary_tree_weight_node& >( **to_begin ).weight( );
				}
				break;
			default :
				m_needed_buffer_size			+= sizeof( n_ary_tree_multiplication_node )
					+ left_multiplicands_count * sizeof( n_ary_tree_base_node* );
				for ( ; to_begin != to_end; ++to_begin )
					increase_buffer_size		( **to_begin );
				break;
		}

		u32 const right_multiplicands_count	= u32( from_end - from_begin );
		bool right_is_pure_weight			= false;
		float right_weight					= math::float_max;
		switch ( right_multiplicands_count ) {
			case 0 :
				m_needed_buffer_size			+= sizeof( n_ary_tree_weight_node );
				right_is_pure_weight			= true;
				right_weight				= 0.f;
				break;
			case 1 :
				increase_buffer_size			( **from_begin );
				if ( (*from_begin)->is_weight( ) && !(*from_begin)->is_transition( ) ) {
					right_is_pure_weight		= true;
					right_weight			= static_cast< n_ary_tree_weight_node& >( **from_begin ).weight( );
				}
				break;
			default :
				m_needed_buffer_size			+= sizeof( n_ary_tree_multiplication_node )
					+ right_multiplicands_count * sizeof( n_ary_tree_base_node* );
				for ( ; from_begin != from_end; ++from_begin )
					increase_buffer_size		( **from_begin );
				break;
		}

		if ( left_is_pure_weight && right_is_pure_weight && left_weight == right_weight )
			m_needed_buffer_size			+= sizeof( n_ary_tree_weight_node );
		else
			m_needed_buffer_size			+= sizeof( n_ary_tree_weight_transition_node );
		return;
	}

	interpolator_comparer animation_interpolator_comparer;
	from.weight_interpolator( ).accept(
		animation_interpolator_comparer,
		to.weight_interpolator( )
	);
	m_equal						= m_equal
		&& animation_interpolator_comparer.result == animation::equal;

	if ( to.override_existing_animation( ) )
		m_equal					= m_equal
			&& from.animation_state( ).animation_interval_id
				== to.animation_state( ).animation_interval_id
			&& from.animation_state( ).animation_interval_time
				== to.animation_state( ).animation_interval_time;

	std::pair< u32, u32 > const operands_counts	= computed_operands_count( from, to );
	u32 const operands_count		= operands_counts.first;
	u32 operands_offset				= operands_counts.second;
	u32 time_scale_operands_count;
	new_animation					( from, time_scale_operands_count, operands_offset );
	m_needed_buffer_size			+= ( operands_count + operands_offset ) * sizeof( n_ary_tree_base_node* );
	add_operands					( from, to, operands_offset != 0 );
}

void n_ary_tree_comparer::merge_weight_synchronization_groups(
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
		(n_ary_tree_animation_node**)ALLOCA( animations_count * sizeof( n_ary_tree_animation_node* ) );
	n_ary_tree_animation_node** animation	= animations;
	bool new_driving_animation_in_old_target_found	= false;
	animation_comparer_equal_predicate equal_predicate( false, true );
	for ( n_ary_tree_animation_node* i = from_begin; i != from_end; i = i->m_next_weight_animation ) {
		if ( !equal_predicate( *i, new_weight_driving_animation ) )
			*animation++			= i;
		else
			new_driving_animation_in_old_target_found	= true;
	}

	if ( new_driving_animation_in_old_target_found )
		--animations_count;

	n_ary_tree_animation_node** const animations_end	= animations + animations_count;
	stlp_std::sort				( animations, animations_end, animation_comparer_less_predicate( false, true ) );

	animation					= animations;
	n_ary_tree_animation_node* i	= to_begin;
	animation_comparer_predicate comparer( false, true );
	while ( animation != animations_end && i != to_end ) {
		switch ( comparer( **animation, *i ) ) {
			case animation::equal :
				change_animation	( **animation++, *i, is_new_driving_animation );
				i				= i->m_next_weight_animation;
				break;
			case animation::less :
				remove_animation	( **animation++, &new_weight_driving_animation, is_new_driving_animation );
				break;
			default :
				add_animation	( *i, &new_weight_driving_animation );
				i				= i->m_next_weight_animation;
				break;
		}
	}

	for ( ; animation != animations_end; ++animation )
		remove_animation			( **animation, &new_weight_driving_animation, is_new_driving_animation );

	for ( ; i != to_end; i = i->m_next_weight_animation )
		add_animation				( *i, &new_weight_driving_animation );
}

void n_ary_tree_comparer::merge_weight_asynchronous_groups(
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
				change_animation	( *i, *j, false );
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

void n_ary_tree_comparer::change_weight_synchronization_group(
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
	if ( previous_weight_driving_animation )
		new_weight_driving_animation		( *to_begin, *previous_weight_driving_animation );
	else
		new_weight_driving_animation		( *to_begin );

	merge_weight_synchronization_groups(
		from_begin,
		from_end,
		to_begin,
		to_end,
		*to_begin,
		is_new_driving_animation
	);
}

void n_ary_tree_comparer::merge_trees( n_ary_tree const& from, n_ary_tree const& to )
{
	n_ary_tree_animation_node* i_begin		= from.weight_root();
	n_ary_tree_animation_node* i_end		= synchronization_group_end( i_begin );
	n_ary_tree_animation_node* j_begin		= to.weight_root();
	n_ary_tree_animation_node* j_end		= synchronization_group_end( j_begin );
	while ( i_begin && j_begin ) {
		if ( (*i_begin).weight_synchronization_group_id() < (*j_begin).weight_synchronization_group_id() ) {
			m_equal							= false;
			remove_weight_synchronization_group	( i_begin, i_end );
			get_next_synchronization_group		( i_begin, i_end );
			continue;
		}

		if ( (*i_begin).weight_synchronization_group_id() > (*j_begin).weight_synchronization_group_id() ) {
			m_equal							= false;
			add_weight_synchronization_group	( j_begin, j_end );
			get_next_synchronization_group		( j_begin, j_end );
			continue;
		}

		change_weight_synchronization_group	( i_begin, i_end, j_begin, j_end );
		get_next_synchronization_group		( i_begin, i_end );
		get_next_synchronization_group		( j_begin, j_end );
	}

	while ( i_begin ) {
		m_equal								= false;
		remove_weight_synchronization_group	( i_begin, i_end );
		get_next_synchronization_group		( i_begin, i_end );
	}

	while ( j_begin ) {
		m_equal								= false;
		add_weight_synchronization_group	( j_begin, j_end );
		get_next_synchronization_group		( j_begin, j_end );
	}
}

 n_ary_tree_comparer::n_ary_tree_comparer( n_ary_tree const& from, n_ary_tree const& to, u32 current_time_in_ms ) :
	m_animations_count		( 0 ),
	m_animated_objects_count( 0 ),
	m_current_time_in_ms	( current_time_in_ms ),
	m_from					( from ),
	m_to					( to ),
	m_equal					( true ),
	m_needed_buffer_size	( 4 )
{

	process_interpolators( from, to );

	m_animated_objects = m_animated_objects_end = ( animated_object_holder* )ALLOCA( sizeof( animated_object_holder ) * ( from.animated_objects_count() + to.animated_objects_count() ) );
	merge_trees( from, to );

	m_needed_buffer_size += m_animations_count * sizeof( animation_state ) + ( m_animated_objects_count = m_animated_objects_end - m_animated_objects ) * sizeof( animated_object_holder );
}

} // namespace mixing
} // namespace animation
} // namespace vostok
