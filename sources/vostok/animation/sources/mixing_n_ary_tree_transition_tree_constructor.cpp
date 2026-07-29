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

#define VOSTOK_DEBUG_ANIMATIONS_COUNT			0

namespace vostok {
namespace animation {
namespace mixing {

struct time_animations_predicate {
	inline bool operator ( )(
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
};

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
	new_animation.accept				( weight_calculator );
	new_animation.set_animation_state	( *m_new_animation_state );

	animation_state const* const previous	=
		previous_animation_state
		&& (
			!previous_animation_state->is_freezed
			|| previous_animation_state->event_iterator.animation( ).is_transitting_to_zero( )
			|| new_animation.is_transitting_to_zero( )
		)
			? previous_animation_state
			: 0;
	new ( m_new_animation_state ) animation_state_params(
		initial_event_types,
		animation_interval_id,
		animation_interval_time,
		previous ? previous->animation_time_threshold : 0.f,
		weight_calculator.weight( ),
		previous
	);
	++m_new_animation_state;

	return								&new_animation;
}

// STATE[STUB]
// vostok::animation::mixing::n_ary_tree_base_node* vostok::animation::mixing::n_ary_tree_transition_tree_constructor::new_time_scale(vostok::animation::mixing::n_ary_tree_animation_node&, unsigned int&, float&)
n_ary_tree_base_node* n_ary_tree_transition_tree_constructor::new_time_scale( n_ary_tree_animation_node& new_time_driving_animation, u32& animation_interval_id, float& animation_interval_time )
{
	// LOCALS
	// float 							new_time_driving_animation_time_offset
	// n_ary_tree_time_scale_calculator time_scale_calculator
	// n_ary_tree_base_node* 			time_scale_node
	// float 							directional_time_scale_factor
	// float 							new_driving_animation_length
	// float 							time_scale_factor
	// bool 							is_target_time_scale_node
	// n_ary_tree_base_node* 			target_time_scale_node
	// float 							new_time_driving_animation_target_time_scale
	// ******

	// CALL SITE INFO
	// <0x6ea52f> -> bool < unknown >()
	// <0x6ea546> -> void < unknown >( n_ary_tree_visitor& )
	// <0x6ea57a> -> float < unknown >() const
	// <0x6ea598> -> bool < unknown >()
	// <0x6ea641> -> bool < unknown >()
	// <0x6ea783> -> bool < unknown >()
	// ******

	return NULL;

	// FUNCTION BODY
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6ea3e3>|0x003|+0x00d:'130'
	// <0x6ea3f0>|0x010|+0x01d:'131'
	// <0x6ea40d>|0x02d|-0x00b:'131'
	// <0x6ea402>|0x022|+0x01e:'132'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6ea420>|0x040|-0x00c:'138'
	// <0x6ea414>|0x034|+0x015:'139'
	// <0>
	// <0x6ea429>|0x049|+0x00a:'141'
	// <0x6ea433>|0x053|+0x00e:'142'
	// <0x6ea441>|0x061|+0x002:'143'
	// <0x6ea443>|0x063|+0x002:'144'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x6ea445>|0x065|+0x011:'154'
	// <0x6ea456>|0x076|+0x016:'155'
	// <0>
	// <1>
	// <2>
	// <0x6ea46c>|0x08c|+0x011:'159'
	// <0>
	// <1>
	// <0x6ea47d>|0x09d|+0x004:'162'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6ea481>|0x0a1|+0x022:'167'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x6ea4a3>|0x0c3|+0x02c:'175'
	// <0x6ea4cf>|0x0ef|+0x004:'176'
	// <0>
	// <1>
	// <0x6ea4d3>|0x0f3|+0x045:'179'
	// <0x6ea518>|0x138|+0x03c:'180'
	// <0x6ea554>|0x174|-0x030:'180'
	// <0x6ea524>|0x144|+0x011:'181'
	// <0>
	// <0x6ea535>|0x155|+0x013:'183'
	// <0x6ea548>|0x168|+0x010:'184'
	// <0x6ea558>|0x178|+0x00a:'184'
	// <0>
	// <1>
	// <2>
	// <0x6ea562>|0x182|+0x011:'188'
	// <0>
	// <0x6ea573>|0x193|+0x017:'190'
	// <0>
	// <1>
	// <0x6ea58a>|0x1aa|+0x017:'193'
	// <0x6ea5a1>|0x1c1|+0x036:'194'
	// <0>
	// <1>
	// <0x6ea5d7>|0x1f7|+0x046:'197'
	// <0>
	// <1>
	// <2>
	// <0x6ea61d>|0x23d|+0x02e:'201'
	// <0>
	// <0x6ea64b>|0x26b|+0x009:'203'
	// <0>
	// <0x6ea654>|0x274|+0x006:'205'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6ea65a>|0x27a|+0x037:'212'
	// <0x6ea691>|0x2b1|+0x029:'213'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x6ea6ba>|0x2da|+0x035:'222'
	// <0x6ea6ef>|0x30f|+0x009:'223'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x6ea6f8>|0x318|+0x03a:'231'
	// <0x6ea732>|0x352|+0x019:'232'
	// <0x6ea74b>|0x36b|+0x019:'232'
	// <0>
	// <1>
	// <0x6ea764>|0x384|+0x029:'235'
	// <0>
	// <0x6ea78d>|0x3ad|+0x01f:'237'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6ea7ac>|0x3cc|+0x013:'242'
	// <0x6ea7bf>|0x3df|+0x009:'243'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6ea7c8>|0x3e8|+0x031:'250'
	// <0x6ea7f9>|0x419|+0x01a:'251'
	// <0x6ea813>|0x433|-0x0d4:'251'
	// <0>
	// <1>
	// <2>
	// <0x6ea73f>|0x35f|+0x019:'255'
	// <0x6ea758>|0x378|+0x041:'255'
	// <0x6ea799>|0x3b9|+0x06e:'255'
	// <0x6ea807>|0x427|+0x01e:'255'
	// <0x6ea825>|0x445|-0x40f:'255'
	// <0x6ea416>|0x036|+0x32b:'256'
	// <0x6ea741>|0x361|+0x019:'256'
	// <0x6ea75a>|0x37a|+0x048:'256'
	// <0x6ea7a2>|0x3c2|+0x067:'256'
	// <0x6ea809>|0x429|+0x020:'256'
	// ******
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
	animation_interval_id			= ( to.override_existing_animation( ) || from.animation_state( ).is_freezed ? to : from ).animation_state( ).animation_interval_id;
	animation_interval_time			= ( to.override_existing_animation( ) || from.animation_state( ).is_freezed ? to : from ).animation_state( ).animation_interval_time;

	n_ary_tree_base_node* time_scale_node	= NULL;
	if ( can_be_time_driving_animation && !from.weight_synchronization_group_id( ) && from.time_synchronization_group_id( ) != u32( -1 ) ) {
		time_scale_node				= new_time_scale( from, animation_interval_id, animation_interval_time );
		if ( time_scale_node ) {
			++operands_offset;
			++time_scale_operands_count;
		}
	}

	animation_interval const* current_interval		= from.animation_intervals( );
	animation_interval const* const intervals_end	= current_interval + from.animation_intervals_count( );
	animation_interval const* const cloned_intervals_begin	= static_cast< animation_interval const* >( m_buffer.c_ptr( ) );
	for ( ; current_interval != intervals_end; ++current_interval, m_buffer += sizeof( animation_interval ) )
		new ( m_buffer.c_ptr( ) ) animation_interval(
			current_interval->animation( ),
			current_interval->start_time( ),
			current_interval->length( )
		);

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
		animation_interval_time		=
			from.animation_intervals( )[ animation_interval_id ].length( ) / from.time_driving_animation( )->animation_intervals( )[ animation_interval_id ].length( )
			* time_driving_animation_state.animation_interval_time;
	}
	else {
		animation_interval_id		= ( to.override_existing_animation( ) || from.animation_state( ).is_freezed ? to : from ).animation_state( ).animation_interval_id;
		animation_interval_time		= ( to.override_existing_animation( ) || from.animation_state( ).is_freezed ? to : from ).animation_state( ).animation_interval_time;
	}

	// claude@NOTE: TRGT_ONLY tail (target lines 382-392) is the animated_object dedup: __find
	// result->animated_object() in [m_animated_objects, m_new_animated_object), then in m_from's
	// animated_objects, else m_get_transform_functor(...) into a fresh animated_object_holder and
	// advance m_new_animated_object. Omitted: it reads n_ary_tree::m_animated_objects(_count) and
	// writes animated_object_holder::transform, both private to classes that do NOT befriend this
	// constructor here - reconstructing it needs friend decls I can't confirm from structure.
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

n_ary_tree_animation_node* n_ary_tree_transition_tree_constructor::remove_animation( n_ary_tree_animation_node& animation, n_ary_tree_animation_node* weight_driving_animation, bool is_new_driving_animation )
{
	base_interpolator const& interpolator	=
		( weight_driving_animation ? *weight_driving_animation : animation ).weight_interpolator( );
	if ( interpolator.transition_time( ) == 0.f )
		return							NULL;

	bool can_be_time_driving_animation	= true;
	u32 operands_offset					=
		animation.operands_count( )
		&& (*animation.operands( sizeof( n_ary_tree_animation_node ) ))->is_time_scale( )
			? 1
			: 0;

	if ( animation.time_synchronization_group_id( ) != u32( -1 ) ) {
		n_ary_tree_animation_node* i	= m_to.time_root( );
		for ( ; i && i->time_synchronization_group_id( ) != animation.time_synchronization_group_id( ); i = i->m_next_time_animation )
			;

		if ( i && i != &animation ) {
			can_be_time_driving_animation	= false;
			operands_offset					= 0;
		}
	}

	if ( animation.is_transitting_to_zero( ) && !is_new_driving_animation ) {
		u32 const operands_count			= animation.operands_count( ) - (
			animation.operands_count( )
			&& (*animation.operands( sizeof( n_ary_tree_animation_node ) ))->is_time_scale( )
				? 1
				: 0
		);
		u32 time_scale_operands_count	= 0;
		u32 animation_interval_id		= 0;
		float animation_interval_time	= 0.f;
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
			result->operands( sizeof( n_ary_tree_animation_node ) ) + time_scale_operands_count;
		m_buffer						+= ( time_scale_operands_count + operands_count ) * sizeof( n_ary_tree_base_node* );

		n_ary_tree_base_node** i			= animation.operands( sizeof( n_ary_tree_animation_node ) );
		n_ary_tree_base_node** const end	= i + animation.operands_count( );
		for ( ; i != end; ++i ) {
			if ( (*i)->is_time_scale( ) && time_scale_operands_count )
				continue;

			*new_operands++				= (*i)->is_time_scale( )
				? m_cloner.clone( **i, 1.f )
				: m_cloner.clone( **i );
		}

		return							add_animation_node(
			*result,
			&animation.animation_state( ),
			animation_interval_id,
			animation_interval_time,
			false
		);
	}

	operands_offset						=
		!animation.time_driving_animation( )
		&& animation.operands_count( )
		&& (*animation.operands( sizeof( n_ary_tree_animation_node ) ))->is_time_scale( )
			? 1
			: 0;

	u32 time_scale_operands_count		= 0;
	u32 animation_interval_id			= 0;
	float animation_interval_time		= 0.f;
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
		result->operands( sizeof( n_ary_tree_animation_node ) ) + time_scale_operands_count;
	m_buffer							+= ( time_scale_operands_count + 1 ) * sizeof( n_ary_tree_base_node* );

	if ( time_scale_operands_count < operands_offset )
		*new_operands++					= m_cloner.clone(
			**animation.operands( sizeof( n_ary_tree_animation_node ) ),
			1.f
		);

	n_ary_tree_weight_transition_node* const transition	=
		static_cast< n_ary_tree_weight_transition_node* >( m_buffer.c_ptr( ) );
	m_buffer							+= sizeof( n_ary_tree_weight_transition_node );

	u32 const weight_operands_offset	= animation.operands_count( )
		&& (*animation.operands( sizeof( n_ary_tree_animation_node ) ))->is_time_scale( )
			? 1
			: 0;
	u32 const multiplicands_count		= animation.operands_count( ) - weight_operands_offset;
	n_ary_tree_base_node* weight_from	= NULL;
	switch ( multiplicands_count ) {
		case 0:
			weight_from					= static_cast< n_ary_tree_base_node* >( m_buffer.c_ptr( ) );
			m_buffer					+= sizeof( n_ary_tree_weight_node );
			new ( weight_from ) n_ary_tree_weight_node( *m_cloner.clone( interpolator ), 1.f );
			break;

		case 1:
			weight_from					= m_cloner.clone(
				*animation.operands( sizeof( n_ary_tree_animation_node ) )[ weight_operands_offset ]
			);
			break;

		default: {
			n_ary_tree_multiplication_node* const multiplication	=
				new ( m_buffer.c_ptr( ) ) n_ary_tree_multiplication_node( multiplicands_count );
			m_buffer					+= sizeof( n_ary_tree_multiplication_node );
			n_ary_tree_base_node** operands	=
				multiplication->operands( sizeof( n_ary_tree_multiplication_node ) );
			m_buffer					+= multiplicands_count * sizeof( n_ary_tree_base_node* );

			n_ary_tree_base_node** i		=
				animation.operands( sizeof( n_ary_tree_animation_node ) ) + weight_operands_offset;
			n_ary_tree_base_node** const end	= i + multiplicands_count;
			for ( ; i != end; ++i )
				*operands++				= m_cloner.clone( **i );

			weight_from					= multiplication;
			break;
		}
	}

	n_ary_tree_weight_node* const weight_to	=
		new ( m_buffer.c_ptr( ) ) n_ary_tree_weight_node( *m_cloner.clone( interpolator ), 0.f );
	m_buffer							+= sizeof( n_ary_tree_weight_node );

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

// claude@NOTE: 22-stmt single-animation builder (target lines 618-701). Picks the weight
// interpolator (weight_driving_animation else animation), calls new_animation, then a per-operand
// merge that clones each operand (with a 1.f time-scale for time-scale nodes) until the first
// operand >= temp, emits a new_weight_transition there, then clones the rest, then an INLINED
// stlp_std::sort (introsort_loop + insertion_sort), then add_animation_node. This is the un-DCE
// driver for new_weight_transition(base_interpolator const&,float,float). Residual: the inlined
// sort + clone re-scheduling and the n_ary_tree_weight_node temp construction.
n_ary_tree_animation_node* n_ary_tree_transition_tree_constructor::add_animation( n_ary_tree_animation_node& animation, n_ary_tree_animation_node* weight_driving_animation )
{
	base_interpolator const& interpolator	= ( weight_driving_animation ? *weight_driving_animation : animation ).weight_interpolator( );

	u32 const to_operands_count		= animation.operands_count( ) - ( animation.operands_count( ) && (*animation.operands( sizeof( n_ary_tree_animation_node ) ))->is_time_scale( ) ? 1 : 0 );

	u32 operands_offset				= interpolator.transition_time( ) != 0.f ? 1 : 0;

	u32 time_scale_operands_count	= 0;
	u32 animation_interval_id		= 0;
	float animation_interval_time	= 0.f;
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

	n_ary_tree_base_node** new_operands	= result->operands( sizeof( n_ary_tree_animation_node ) );
	m_buffer						+= ( time_scale_operands_count + operands_offset ) * sizeof( n_ary_tree_base_node* );

	n_ary_tree_weight_node			temp( interpolator, 1.f );
	n_ary_tree_node_comparer		comparer;

	n_ary_tree_base_node** const	operands_end	= animation.operands( sizeof( n_ary_tree_animation_node ) ) + animation.operands_count( );
	n_ary_tree_base_node**			i				= animation.operands( sizeof( n_ary_tree_animation_node ) ) + ( has_weight_transition_been_added ? 1 : 0 );
	for ( ; i != operands_end; ++i ) {
		if ( comparer.compare( **i, temp ) == n_ary_tree_node_comparer::more ) {
			*new_operands++			= new_weight_transition( *m_cloner.clone( interpolator ), 0.f, 1.f );
			break;
		}

		*new_operands++				= (*i)->is_time_scale( ) ? m_cloner.clone( **i, 1.f ) : m_cloner.clone( **i );
	}

	for ( ; i != operands_end; ++i )
		*new_operands++				= m_cloner.clone( **i );

	if ( i == operands_end && interpolator.transition_time( ) == 0.f )
		*new_operands++				= new_weight_transition( *m_cloner.clone( interpolator ), 0.f, 1.f );

	stlp_std::sort					( result->operands( sizeof( n_ary_tree_animation_node ) ), new_operands, comparer );

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
	bool const has_time_scale				= animation.operands_count( )
		&& (*animation.operands( sizeof( n_ary_tree_animation_node ) ))->is_time_scale( );
	u32 const weight_operands_count			= animation.operands_count( ) - ( has_time_scale ? 1 : 0 )
		+ ( interpolator.transition_time( ) != 0.f ? 1 : 0 );

	u32 time_scale_operands_count			= 0;
	u32 operands_offset						= 0;
	u32 animation_interval_id				= 0;
	float animation_interval_time			= 0.f;
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
		result->operands( sizeof( n_ary_tree_animation_node ) ) + time_scale_operands_count;
	m_buffer								+= ( time_scale_operands_count + operands_offset )
		* sizeof( n_ary_tree_base_node* );

	n_ary_tree_base_node** const operands_end	=
		animation.operands( sizeof( n_ary_tree_animation_node ) ) + animation.operands_count( );
	n_ary_tree_base_node** i					=
		animation.operands( sizeof( n_ary_tree_animation_node ) )
			+ ( has_time_scale ? time_scale_operands_count : 0 );

	n_ary_tree_weight_node temp				( interpolator, 0.f );
	n_ary_tree_node_comparer comparer;
	for ( ; i != operands_end; ++i ) {
		if ( comparer.compare( **i, temp ) == n_ary_tree_node_comparer::more )
			break;

		*new_operands++						= (*i)->is_time_scale( )
			? m_cloner.clone( **i, 0.f )
			: m_cloner.clone( **i );
	}

	if ( interpolator.transition_time( ) != 0.f )
		*new_operands++						=
			new_weight_transition( *m_cloner.clone( interpolator ), 0.f, 1.f );

	for ( ; i != operands_end; ++i )
		*new_operands++						= m_cloner.clone( **i );

	stlp_std::sort(
		result->operands( sizeof( n_ary_tree_animation_node ) ),
		new_operands,
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

// claude@NOTE: structure-first reconstruction of the 4-arg time-scale-transition builder.
// The two single-node fast paths (equal-compare line 797, transition_time==0 line 801) each
// clone `from`/`to` with a fresh time scale via the 3-arg m_cloner.clone overload; residual is
// the inlined re-scheduled temp-node construction inside those clone calls.
n_ary_tree_base_node* n_ary_tree_transition_tree_constructor::new_time_scale_transition(
	n_ary_tree_animation_node&		from_animation,
	n_ary_tree_animation_node&		to_animation,
	n_ary_tree_base_node&			from,
	n_ary_tree_base_node&			to
)
{
	if ( n_ary_tree_node_comparer( ).compare( from, to ) == n_ary_tree_node_comparer::equal )
		return				m_cloner.clone(
								from,
								0.f,
								to_animation.override_existing_animation( ) ? to_animation.animation_state( ).animation_interval_time : from_animation.animation_state( ).animation_interval_time
							);

	n_ary_tree_interpolator_selector	interpolator_selector;
	from.accept				( interpolator_selector );

	if ( interpolator_selector.result( )->transition_time( ) == 0.f )
		return				m_cloner.clone(
								from,
								0.f,
								to_animation.override_existing_animation( ) ? to_animation.animation_state( ).animation_interval_time : from_animation.animation_state( ).animation_interval_time
							);

	n_ary_tree_base_node* const result	= (n_ary_tree_base_node*)m_buffer.c_ptr( );
	m_buffer				+= sizeof( n_ary_tree_time_scale_transition_node );

	n_ary_tree_base_node* const time_scale_from	= m_cloner.clone( from, 0.f );
	n_ary_tree_base_node* const time_scale_to	= m_cloner.clone( to, 0.f );

	time_scale_to->accept	( interpolator_selector );

	if ( result )
		new ( result ) n_ary_tree_time_scale_transition_node(
			*time_scale_from,
			*time_scale_to,
			*m_cloner.clone( *interpolator_selector.result() ),
			m_current_time_in_ms
		);

	return					result;
}

n_ary_tree_base_node* n_ary_tree_transition_tree_constructor::new_time_scale_transition( n_ary_tree_animation_node& from_animation, n_ary_tree_base_node& from, float to )
{
	n_ary_tree_interpolator_selector	interpolator_selector;
	from.accept				( interpolator_selector );

	if ( interpolator_selector.result()->transition_time() == 0.f )
		return				new ( m_buffer.c_ptr() ) n_ary_tree_time_scale_node(
								*m_cloner.clone( *interpolator_selector.result() ),
								0.f,
								from_animation.animation_state( ).animation_interval_time,
								m_current_time_in_ms
							);

	n_ary_tree_base_node* const result	= (n_ary_tree_base_node*)m_buffer.c_ptr( );
	m_buffer				+= sizeof( n_ary_tree_time_scale_transition_node );

	n_ary_tree_base_node* const time_scale_from	= m_cloner.clone( from );

	n_ary_tree_base_node* const time_scale_to	=
		new ( m_buffer.c_ptr() ) n_ary_tree_time_scale_node(
			*m_cloner.clone( *interpolator_selector.result() ),
			0.f,
			from_animation.animation_state( ).animation_interval_time,
			m_current_time_in_ms
		);
	m_buffer				+= sizeof( n_ary_tree_time_scale_node );

	new ( result ) n_ary_tree_time_scale_transition_node(
		*time_scale_from,
		*time_scale_to,
		*m_cloner.clone( *interpolator_selector.result() ),
		m_current_time_in_ms
	);

	return					result;
}

n_ary_tree_base_node* n_ary_tree_transition_tree_constructor::new_time_scale_transition( float animation_time, float from, n_ary_tree_base_node& to )
{
	n_ary_tree_interpolator_selector	interpolator_selector;
	to.accept				( interpolator_selector );

	if ( interpolator_selector.result()->transition_time() == 0.f )
		return				new ( m_buffer.c_ptr() ) n_ary_tree_time_scale_node(
								*m_cloner.clone( *interpolator_selector.result() ),
								0.f,
								animation_time,
								m_current_time_in_ms
							);

	n_ary_tree_base_node* const result	= (n_ary_tree_base_node*)m_buffer.c_ptr( );
	m_buffer				+= sizeof( n_ary_tree_time_scale_transition_node );

	n_ary_tree_base_node* const time_scale_from	=
		new ( m_buffer.c_ptr() ) n_ary_tree_time_scale_node(
			*m_cloner.clone( *interpolator_selector.result() ),
			0.f,
			from,
			m_current_time_in_ms
		);
	m_buffer				+= sizeof( n_ary_tree_time_scale_node );

	n_ary_tree_base_node* const cloned_to	= m_cloner.clone( to );

	new ( result ) n_ary_tree_time_scale_transition_node(
		*time_scale_from,
		*cloned_to,
		*m_cloner.clone( *interpolator_selector.result() ),
		m_current_time_in_ms
	);

	return					result;
}

n_ary_tree_base_node* n_ary_tree_transition_tree_constructor::new_weight_transition( n_ary_tree_base_node& from, n_ary_tree_base_node& to )
{
	n_ary_tree_interpolator_selector	interpolator_selector;
	to.accept				( interpolator_selector );

	if ( interpolator_selector.result()->transition_time() == 0.f )
		return				m_cloner.clone( to );

	n_ary_tree_base_node* const result	= (n_ary_tree_base_node*)m_buffer.c_ptr( );
	m_buffer				+= sizeof( n_ary_tree_weight_transition_node );

	n_ary_tree_base_node* const weight_from	= m_cloner.clone( from );

	n_ary_tree_base_node* const cloned_to	= m_cloner.clone( to );

	new ( result ) n_ary_tree_weight_transition_node(
		*weight_from,
		*cloned_to,
		*m_cloner.clone( *interpolator_selector.result() ),
		m_current_time_in_ms
	);

	return					result;
}

n_ary_tree_base_node* n_ary_tree_transition_tree_constructor::new_weight_transition( base_interpolator const& from_animation_interpolator, n_ary_tree_base_node& from, float to )
{
	if ( from_animation_interpolator.transition_time() == 0.f )
		return				new ( m_buffer.c_ptr() ) n_ary_tree_weight_node( *m_cloner.clone( from_animation_interpolator ), 0.f );

	n_ary_tree_weight_node	weight( from_animation_interpolator, to );
	if ( n_ary_tree_node_comparer( ).compare( from, weight ) == n_ary_tree_node_comparer::equal )
		return				m_cloner.clone( from );

	n_ary_tree_base_node* const result	= (n_ary_tree_base_node*)m_buffer.c_ptr( );
	m_buffer				+= sizeof( n_ary_tree_weight_transition_node );

	n_ary_tree_base_node* const cloned_from	= m_cloner.clone( from );

	n_ary_tree_base_node* const weight_to	=
		new ( m_buffer.c_ptr() ) n_ary_tree_weight_node( *m_cloner.clone( from_animation_interpolator ), 0.f );
	m_buffer				+= sizeof( n_ary_tree_weight_node );

	new ( result ) n_ary_tree_weight_transition_node(
		*cloned_from,
		*weight_to,
		*m_cloner.clone( from_animation_interpolator ),
		m_current_time_in_ms
	);

	return					result;
}

n_ary_tree_base_node* n_ary_tree_transition_tree_constructor::new_weight_transition( base_interpolator const& to_animation_interpolator, float from, n_ary_tree_base_node& to )
{
	if ( to_animation_interpolator.transition_time() == 0.f )
		return				m_cloner.clone( to );

	n_ary_tree_weight_node	weight( to_animation_interpolator, from );
	if ( n_ary_tree_node_comparer( ).compare( to, weight ) == n_ary_tree_node_comparer::equal )
		return				m_cloner.clone( to );

	n_ary_tree_base_node* const result	= (n_ary_tree_base_node*)m_buffer.c_ptr( );
	m_buffer				+= sizeof( n_ary_tree_weight_transition_node );

	n_ary_tree_base_node* const cloned_to	= m_cloner.clone( to );

	n_ary_tree_interpolator_selector	interpolator_selector;
	cloned_to->accept		( interpolator_selector );

	n_ary_tree_base_node* const weight_from	=
		new ( m_buffer.c_ptr() ) n_ary_tree_weight_node( *m_cloner.clone( *interpolator_selector.result() ), 0.f );
	m_buffer				+= sizeof( n_ary_tree_weight_node );

	new ( result ) n_ary_tree_weight_transition_node(
		*weight_from,
		*cloned_to,
		*m_cloner.clone( *interpolator_selector.result() ),
		m_current_time_in_ms
	);

	return					result;
}

void n_ary_tree_transition_tree_constructor::add_operands(
	n_ary_tree_animation_node&		from,
	n_ary_tree_animation_node&		to,
	n_ary_tree_base_node**			operands_begin,
	n_ary_tree_base_node**			operands_end,
	bool							skip_time_scale_node
)
{
	// claude@NOTE: time-scale prologue (lines 1003-1016) is collapsed vs the target's
	// nested form, and the merge cases each emit a couple of extra target statements -
	// residual structure work. Main 3-way merge + tails + sort match.
	n_ary_tree_node_comparer	comparer;
	n_ary_tree_base_node** const		i_e	= from.operands( sizeof( n_ary_tree_animation_node ) ) + from.operands_count( );
	n_ary_tree_base_node**				i	= from.operands( sizeof( n_ary_tree_animation_node ) );
	n_ary_tree_base_node** const		j_e	= to.operands( sizeof( n_ary_tree_animation_node ) ) + to.operands_count( );
	n_ary_tree_base_node**				j	= to.operands( sizeof( n_ary_tree_animation_node ) );

	if ( i != i_e && (*i)->is_time_scale( ) ) {
		if ( j != j_e && (*j)->is_time_scale( ) ) {
			if ( !skip_time_scale_node )
				*operands_begin++	= new_time_scale_transition( from, to, **i, **j );
			++i;
			++j;
		}
		else {
			if ( !skip_time_scale_node )
				*operands_begin++	= new_time_scale_transition( from, **i, to.weight_interpolator( ).transition_time( ) );
			++i;
		}
	}
	else if ( j != j_e && (*j)->is_time_scale( ) ) {
		if ( !skip_time_scale_node )
			*operands_begin++	= new_time_scale_transition( from.weight_interpolator( ).transition_time( ), to.weight_interpolator( ).transition_time( ), **j );
		++j;
	}

	n_ary_tree_interpolator_selector	interpolator_selector;
	for ( ; i != i_e; ) {
		if ( j == j_e )
			break;

		switch ( comparer.compare( **i, **j ) ) {
			case n_ary_tree_node_comparer::equal :
				*operands_begin++	= new_weight_transition( **j, **i );
				++i;
				++j;
				break;
			case n_ary_tree_node_comparer::less : {
				(*i)->accept		( interpolator_selector );
				base_interpolator const* const	i_interpolator	= interpolator_selector.result( );
				*operands_begin++	= new_weight_transition( *i_interpolator, **i, 0.f );
				++i;
				break;
			}
			case n_ary_tree_node_comparer::more : {
				(*j)->accept		( interpolator_selector );
				base_interpolator const* const	j_interpolator	= interpolator_selector.result( );
				*operands_begin++	= new_weight_transition( *j_interpolator, 0.f, **j );
				++j;
				break;
			}
			default : NODEFAULT( );
		}
	}

	for ( ; i != i_e; ++i )
		*operands_begin++	= new_weight_transition( from.weight_interpolator( ), **i, 0.f );

	for ( ; j != j_e; ++j )
		*operands_begin++	= new_weight_transition( to.weight_interpolator( ), 0.f, **j );

	if ( operands_begin != operands_end )
		stlp_std::sort		( operands_begin, operands_end, comparer );
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
	else if ( j != j_e && (*j)->is_time_scale( ) ) {
		time_scale_nodes_count	= 1;
		++j;
	}

	n_ary_tree_interpolator_selector	interpolator_selector;
	for ( ; i != i_e; ) {
		if ( j == j_e )
			break;

		if ( comparer.compare( **i, **j ) == n_ary_tree_node_comparer::equal ) {
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

	for ( ; i != i_e; ++i )
		++operands_count;

	for ( ; j != j_e; ++j )
		++operands_count;

	return						std::make_pair( operands_count, time_scale_nodes_count );
}

n_ary_tree_animation_node* n_ary_tree_transition_tree_constructor::new_weight_driving_animation( n_ary_tree_animation_node& new_weight_driving_animation, n_ary_tree_animation_node& new_driving_animation_in_previous_target )
{
	std::pair< u32, u32 > const operands_counts	=
		computed_operands_count( new_driving_animation_in_previous_target, new_weight_driving_animation );
	u32 time_scale_operands_count	= operands_counts.first;
	u32 operands_offset				= operands_counts.second;
	u32 animation_interval_id		= 0;
	float animation_interval_time	= 0.f;
	n_ary_tree_animation_node* const result	= new_animation(
		new_weight_driving_animation,
		new_driving_animation_in_previous_target,
		0,
		0,
		time_scale_operands_count,
		operands_offset,
		animation_interval_id,
		animation_interval_time,
		new_driving_animation_in_previous_target.is_transitting_to_zero( ),
		true
	);

	n_ary_tree_base_node** const operands	=
		result->operands( sizeof( n_ary_tree_animation_node ) ) + time_scale_operands_count;
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

// claude@NOTE: the two per-side multiplication-node operand-copy loops (clone every operand
// via m_cloner.clone, .15/.23 in the target) are reconstructed; structure now matches. The
// residual TRGT_ONLY is the call-arg setup for new_animation + add_animation_node, both still
// STUBs (return NULL) - their inlined-ctor/argument flow only lines up once they are bodied.
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
		n_ary_tree_base_node** const	to_end		= from.operands( sizeof( n_ary_tree_animation_node ) ) + from.operands_count( );

		u32								operands_offset	=
			( to.operands_count( ) && (*multiplicands)->is_time_scale( ) )
			|| ( from.operands_count( ) && (*from.operands( sizeof( n_ary_tree_animation_node ) ))->is_time_scale( ) )
			? 1 : 0;

		u32								animation_interval_id	= 0;
		float							animation_interval_time	= 0.f;
		u32								time_scale_operands_count	= 0;

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

		n_ary_tree_base_node** new_operands	= result->operands( sizeof( n_ary_tree_animation_node ) );
		m_buffer				+= ( time_scale_operands_count + operands_offset ) * sizeof( n_ary_tree_base_node* );

		if ( to.operands_count( ) && (*multiplicands)->is_time_scale( ) ) {
			if ( from.operands_count( ) && (*from.operands( sizeof( n_ary_tree_animation_node ) ))->is_time_scale( ) ) {
				*new_operands++	= new_time_scale_transition( to, from, **multiplicands, **from.operands( sizeof( n_ary_tree_animation_node ) ) );
				++multiplicands;
			}
			else
				*new_operands++	= new_time_scale_transition( to, **multiplicands, from.animation_state( ).animation_interval_time );
		}
		else if ( from.operands_count( ) && (*from.operands( sizeof( n_ary_tree_animation_node ) ))->is_time_scale( ) )
			*new_operands++	= new_time_scale_transition( animation_interval_time, animation_interval_time, **from.operands( sizeof( n_ary_tree_animation_node ) ) );

		u32								left_multiplicands_count	= u32( from_end - multiplicands );
		if ( left_multiplicands_count && (*multiplicands)->is_time_scale( ) )
			--left_multiplicands_count;

		n_ary_tree_base_node*			weight_from;
		if ( left_multiplicands_count >= 2 ) {
			n_ary_tree_base_node* const	left	= (n_ary_tree_base_node*)m_buffer.c_ptr( );
			m_buffer				+= sizeof( n_ary_tree_multiplication_node );
			n_ary_tree_multiplication_node* const	multiplication	= new ( left ) n_ary_tree_multiplication_node( left_multiplicands_count );

			n_ary_tree_base_node**	operands	= multiplication->operands( sizeof( n_ary_tree_multiplication_node ) );
			m_buffer				+= left_multiplicands_count * sizeof( n_ary_tree_base_node* );

			for ( n_ary_tree_base_node** i = multiplicands + ( operands_offset && (*multiplicands)->is_time_scale( ) ? 1 : 0 ); i != from_end; ++i )
				*operands++			= m_cloner.clone( **i );

			weight_from				= left;
		}
		else if ( left_multiplicands_count == 1 )
			weight_from				= m_cloner.clone( **(multiplicands - 1) );
		else {
			weight_from				= (n_ary_tree_base_node*)m_buffer.c_ptr( );
			m_buffer				+= sizeof( n_ary_tree_weight_node );
			new ( weight_from ) n_ary_tree_weight_node( result->weight_interpolator( ), 0.f );
		}

		u32								right_multiplicands_count	= u32( to_end - from.operands( sizeof( n_ary_tree_animation_node ) ) );
		if ( right_multiplicands_count && (*from.operands( sizeof( n_ary_tree_animation_node ) ))->is_time_scale( ) )
			--right_multiplicands_count;

		n_ary_tree_base_node*			weight_to;
		if ( right_multiplicands_count >= 2 ) {
			n_ary_tree_base_node* const	right	= (n_ary_tree_base_node*)m_buffer.c_ptr( );
			m_buffer				+= sizeof( n_ary_tree_multiplication_node );
			n_ary_tree_multiplication_node* const	multiplication	= new ( right ) n_ary_tree_multiplication_node( right_multiplicands_count );

			n_ary_tree_base_node**	operands	= multiplication->operands( sizeof( n_ary_tree_multiplication_node ) );
			m_buffer				+= right_multiplicands_count * sizeof( n_ary_tree_base_node* );

			for ( n_ary_tree_base_node** i = from.operands( sizeof( n_ary_tree_animation_node ) ) + ( operands_offset && (*from.operands( sizeof( n_ary_tree_animation_node ) ))->is_time_scale( ) ? 1 : 0 ); i != to_end; ++i )
				*operands++			= m_cloner.clone( **i );

			weight_to				= right;
		}
		else if ( right_multiplicands_count == 1 )
			weight_to				= m_cloner.clone( **(to_end - 1) );
		else {
			weight_to				= (n_ary_tree_base_node*)m_buffer.c_ptr( );
			m_buffer				+= sizeof( n_ary_tree_weight_node );
			new ( weight_to ) n_ary_tree_weight_node( result->weight_interpolator( ), 0.f );
		}

		*new_operands			= new_weight_transition( *weight_from, *weight_to );

		add_animation_node(
			*result,
			&to.animation_state( ),
			animation_interval_id,
			animation_interval_time,
			true
		);
		return;
	}

	std::pair< u32, u32 >	operands_counts	= computed_operands_count( from, to );

	u32						time_scale_operands_count	= operands_counts.first;
	u32						operands_offset				= operands_counts.second;
	u32						animation_interval_id		= 0;
	float					animation_interval_time		= 0.f;

	n_ary_tree_animation_node* const	result	= new_animation(
		to,
		from,
		weight_driving_animation,
		0,
		time_scale_operands_count,
		operands_offset,
		animation_interval_id,
		animation_interval_time,
		from.is_transitting_to_zero( ),
		true
	);

	n_ary_tree_base_node** const	new_operands	= result->operands( sizeof( n_ary_tree_animation_node ) );

	add_operands(
		from,
		to,
		new_operands,
		new_operands + time_scale_operands_count + operands_offset,
		weight_driving_animation != 0
	);

	add_animation_node(
		*result,
		&to.animation_state( ),
		animation_interval_id,
		animation_interval_time,
		true
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
			case equal :
				change_animation	( **animation++, *i, &new_weight_driving_animation, is_new_driving_animation );
				i				= i->m_next_weight_animation;
				break;
			case less :
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

void n_ary_tree_transition_tree_constructor::merge_weight_asynchronous_groups(
	n_ary_tree_animation_node*		from_begin,
	n_ary_tree_animation_node*		from_end,
	n_ary_tree_animation_node*		to_begin,
	n_ary_tree_animation_node*		to_end
)
{
	n_ary_tree_animation_node* i	= from_begin;
	n_ary_tree_animation_node* j	= to_begin;
	n_ary_tree_node_comparer comparer;
	while ( i && j ) {
		switch ( comparer.compare( *i, *j ) ) {
			case n_ary_tree_node_comparer::equal :
				change_animation	( *i, *j, 0, false );
				i				= i->m_next_weight_animation;
				j				= j->m_next_weight_animation;
				break;
			case n_ary_tree_node_comparer::less :
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
		animation_state_params const params		=
			*reinterpret_cast< animation_state_params const* >( &animation->animation_state( ) );

		u16 const initial_event_types			= params.initial_event_types;
		u32 animation_interval_id				= params.animation_interval_id;
		float animation_interval_time			= params.animation_interval_time;
		float animation_time_threshold			= params.animation_time_threshold;
		float const weight						= params.weight;
		animation_state const* const previous	= params.previous;
		bool const is_freezed					= previous ? previous->is_freezed : false;

		if ( animation->time_driving_animation( ) ) {
			n_ary_tree_animation_node& driver	= *animation->time_driving_animation( );
			animation_state const& driver_state	= driver.animation_state( );
			animation_interval_id				= driver_state.animation_interval_id;
			animation_interval const& driver_interval	=
				driver.animation_intervals( )[ animation_interval_id ];
			animation_interval_time				=
				animation->animation_intervals( )[ animation_interval_id ].length( )
				/ driver_interval.length( )
				* driver_state.animation_interval_time;
			animation_time_threshold			= 0.f;
		}

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
	u32								animations_count,
	u32								animated_objects_count,
	u32								current_time_in_ms,
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

// claude@NOTE: structure matches (single `return n_ary_tree( <11 members> )`, mirror of
// the matched sibling n_ary_tree_transition_constructor::computed_tree). Byte residual: the
// 11-arg n_ary_tree constructor is still an empty STUB (mixing_n_ary_tree.cpp), so the base
// INLINES it to nothing - only the implicit intrusive_ptr member-ctor survives, dropping all
// 11 arg pushes + the out-of-line `call`. Reappears once that constructor gets a real body.
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
