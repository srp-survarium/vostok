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

// STATE[STUB]
// vostok::animation::mixing::n_ary_tree_animation_node* vostok::animation::mixing::n_ary_tree_transition_tree_constructor::remove_animation(vostok::animation::mixing::n_ary_tree_animation_node&, vostok::animation::mixing::n_ary_tree_animation_node* const, bool)
n_ary_tree_animation_node* n_ary_tree_transition_tree_constructor::remove_animation( n_ary_tree_animation_node& animation, n_ary_tree_animation_node* weight_driving_animation, bool is_new_driving_animation )
{
	// LOCALS
	// float 							animation_interval_time
	// bool 							can_be_time_driving_animation
	// n_ary_tree_animation_node* 		result
	// u32 								animation_interval_id
	// n_ary_tree_base_node** 			new_operands
	// n_ary_tree_weight_transition_node* transition
	// u32 								operands_offset
	// float 							animation_interval_time
	// n_ary_tree_animation_node* 		result
	// u32 								animation_interval_id
	// u32 								operands_offset
	// n_ary_tree_base_node** 			multiplicands
	// n_ary_tree_base_node** 			e
	// u32 								time_scale_operands_count
	// n_ary_tree_base_node* 			weight_from
	// bool 							can_be_time_driving_animation
	// u32 								time_scale_operands_count
	// n_ary_tree_base_node** 			new_operands
	// n_ary_tree_base_node** 			operands_end
	// ******

	// CALL SITE INFO
	// <0x6ebb6c> -> float < unknown >() const
	// <0x6ebbaa> -> bool < unknown >()
	// <0x6ebc67> -> bool < unknown >()
	// <0x6ebd25> -> bool < unknown >()
	// <0x6ebe13> -> bool < unknown >()
	// <0x6ebe75> -> bool < unknown >()
	// <0x6ebf60> -> bool < unknown >()
	// ******

	return NULL;

	// FUNCTION BODY
	// <0x6ebb4a>|0x00a|+0x01d:'436'
	// <0x6ebb67>|0x027|+0x015:'437'
	// <0x6ebb7c>|0x03c|+0x00c:'438'
	// <0>
	// <0x6ebb88>|0x048|+0x015:'440'
	// <0>
	// <0x6ebb9d>|0x05d|+0x01c:'442'
	// <0x6ebbb9>|0x079|+0x003:'443'
	// <0>
	// <1>
	// <0x6ebbbc>|0x07c|+0x013:'446'
	// <0x6ebbcf>|0x08f|+0x011:'447'
	// <0x6ebbe0>|0x0a0|+0x00e:'448'
	// <0x6ebbee>|0x0ae|+0x004:'449'
	// <0x6ebbf2>|0x0b2|+0x005:'450'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6ebbf7>|0x0b7|+0x008:'457'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <12>
	// <13>
	// <14>
	// <15>
	// <0x6ebbff>|0x0bf|+0x025:'474'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6ebc24>|0x0e4|+0x010:'479'
	// <0x6ebc34>|0x0f4|+0x014:'480'
	// <0>
	// <1>
	// <2>
	// <0x6ebc48>|0x108|+0x00d:'484'
	// <0>
	// <1>
	// <0x6ebc55>|0x115|+0x00b:'487'
	// <0x6ebc60>|0x120|+0x00d:'488'
	// <0x6ebc6d>|0x12d|+0x007:'489'
	// <0x6ebc74>|0x134|+0x005:'490'
	// <0>
	// <0x6ebc79>|0x139|+0x002:'492'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6ebc7b>|0x13b|+0x038:'498'
	// <0>
	// <0x6ebcb3>|0x173|+0x002:'500'
	// <0>
	// <0x6ebcb5>|0x175|+0x037:'502'
	// <0>
	// <1>
	// <2>
	// <0x6ebcec>|0x1ac|+0x027:'506'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6ebd13>|0x1d3|+0x024:'511'
	// <0>
	// <0x6ebd37>|0x1f7|+0x00d:'513'
	// <0x6ebd44>|0x204|-0x158:'514'
	// <0x6ebbec>|0x0ac|+0x164:'515'
	// <0x6ebd50>|0x210|+0x00e:'515'
	// <0x6ebd5e>|0x21e|+0x004:'516'
	// <0x6ebd62>|0x222|+0x005:'517'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6ebd67>|0x227|+0x004:'524'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <12>
	// <13>
	// <14>
	// <15>
	// <0x6ebd6b>|0x22b|+0x026:'541'
	// <0>
	// <1>
	// <0x6ebd91>|0x251|+0x004:'544'
	// <0x6ebd95>|0x255|+0x020:'545'
	// <0>
	// <0x6ebdb5>|0x275|+0x004:'547'
	// <0>
	// <1>
	// <0x6ebdb9>|0x279|+0x03c:'550'
	// <0>
	// <1>
	// <0x6ebdf5>|0x2b5|+0x005:'553'
	// <0x6ebdfa>|0x2ba|+0x007:'554'
	// <0>
	// <1>
	// <0x6ebe01>|0x2c1|+0x026:'557'
	// <0x6ebe27>|0x2e7|+0x16c:'558'
	// <0>
	// <0x6ebf93>|0x453|+0x005:'560'
	// <0x6ebf98>|0x458|+0x00b:'561'
	// <0x6ebfa3>|0x463|-0x04b:'562'
	// <0>
	// <1>
	// <2>
	// <0x6ebf58>|0x418|+0x036:'566'
	// <0x6ebf8e>|0x44e|-0x153:'567'
	// <0>
	// <1>
	// <0x6ebe3b>|0x2fb|+0x005:'570'
	// <0x6ebe40>|0x300|+0x00b:'571'
	// <0x6ebe4b>|0x30b|+0x00d:'572'
	// <0x6ebe58>|0x318|+0x005:'573'
	// <0x6ebe5d>|0x31d|+0x00c:'574'
	// <0x6ebe69>|0x329|+0x027:'575'
	// <0x6ebe90>|0x350|+0x033:'576'
	// <0>
	// <0x6ebec3>|0x383|+0x004:'578'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6ebec7>|0x387|+0x005:'584'
	// <0x6ebecc>|0x38c|+0x007:'585'
	// <0x6ebed3>|0x393|+0x023:'586'
	// <0>
	// <1>
	// <0x6ebef6>|0x3b6|+0x035:'589'
	// <0x6ebf2b>|0x3eb|-0x227:'590'
	// <0>
	// <0x6ebd04>|0x1c4|+0x22b:'592'
	// <0x6ebf2f>|0x3ef|-0x3b1:'592'
	// <0x6ebb7e>|0x03e|+0x18b:'593'
	// <0x6ebd09>|0x1c9|+0x245:'593'
	// ******
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

// STATE[STUB]
// void vostok::animation::mixing::n_ary_tree_transition_tree_constructor::merge_trees(vostok::animation::mixing::n_ary_tree const&, vostok::animation::mixing::n_ary_tree const&)
void n_ary_tree_transition_tree_constructor::merge_trees( n_ary_tree const& from, n_ary_tree const& to )
{
	// LOCALS
	// n_ary_tree_animation_node* 		i_begin
	// float 							animation_interval_time
	// animation_state_params const& 	params
	// u16 								initial_event_types
	// float 							animation_time_threshold
	// float 							weight
	// animation_interval const* 		time_driving_animation_interval
	// bool 							is_freezed
	// ******

	// FUNCTION BODY
	// <0x6ec366>|0x006|+0x003:'1557'
	// <0x6ec369>|0x009|+0x017:'1558'
	// <0x6ec380>|0x020|+0x006:'1559'
	// <0x6ec386>|0x026|+0x01d:'1560'
	// <0x6ec3a3>|0x043|+0x0ab:'1561'
	// <0x6ec44e>|0x0ee|-0x0a3:'1561'
	// <0x6ec3ab>|0x04b|+0x00a:'1562'
	// <0x6ec3b5>|0x055|+0x00b:'1563'
	// <0x6ec3c0>|0x060|+0x01e:'1564'
	// <0x6ec3de>|0x07e|+0x003:'1565'
	// <0>
	// <1>
	// <0x6ec3e1>|0x081|-0x001:'1568'
	// <0x6ec3e0>|0x080|+0x003:'1569'
	// <0x6ec3e3>|0x083|+0x00b:'1569'
	// <0x6ec3ee>|0x08e|+0x01e:'1570'
	// <0x6ec40c>|0x0ac|+0x002:'1571'
	// <0>
	// <1>
	// <0x6ec40e>|0x0ae|+0x00c:'1574'
	// <0x6ec41a>|0x0ba|+0x01a:'1575'
	// <0x6ec434>|0x0d4|+0x053:'1576'
	// <0>
	// <1>
	// <0x6ec487>|0x127|+0x009:'1579'
	// <0x6ec490>|0x130|+0x00b:'1580'
	// <0x6ec49b>|0x13b|-0x043:'1581'
	// <0>
	// <1>
	// <0x6ec458>|0x0f8|+0x02d:'1584'
	// <0x6ec485>|0x125|-0x025:'1584'
	// <0x6ec460>|0x100|+0x00c:'1585'
	// <0x6ec46c>|0x10c|+0x04e:'1586'
	// <0>
	// <1>
	// <0x6ec4ba>|0x15a|+0x038:'1589'
	// <0x6ec4f2>|0x192|-0x02e:'1589'
	// <0x6ec4c4>|0x164|+0x008:'1590'
	// <0x6ec4cc>|0x16c|+0x01b:'1591'
	// <0>
	// <0x6ec4e7>|0x187|+0x008:'1593'
	// <0x6ec4ef>|0x18f|+0x00a:'1594'
	// <0>
	// <1>
	// <2>
	// <0x6ec4f9>|0x199|+0x00f:'1598'
	// <0>
	// <0x6ec508>|0x1a8|+0x00b:'1600'
	// <0x6ec513>|0x1b3|+0x00c:'1601'
	// <0>
	// <0x6ec51f>|0x1bf|+0x041:'1603'
	// <0>
	// <0x6ec560>|0x200|+0x002:'1605'
	// <0x6ec562>|0x202|+0x00e:'1606'
	// <0x6ec570>|0x210|+0x00f:'1607'
	// <0>
	// <0x6ec57f>|0x21f|+0x011:'1609'
	// <0x6ec590>|0x230|+0x003:'1610'
	// <0x6ec593>|0x233|+0x007:'1611'
	// <0>
	// <1>
	// <2>
	// <0x6ec59a>|0x23a|+0x00a:'1615'
	// <0x6ec5a4>|0x244|+0x006:'1616'
	// <0x6ec5aa>|0x24a|+0x014:'1617'
	// <0x6ec5be>|0x25e|+0x009:'1618'
	// <0x6ec5c7>|0x267|+0x013:'1619'
	// <0x6ec5da>|0x27a|+0x003:'1620'
	// <0>
	// <0x6ec5dd>|0x27d|+0x008:'1622'
	// <0>
	// <0x6ec5e5>|0x285|+0x00c:'1624'
	// <0x6ec5f1>|0x291|+0x008:'1625'
	// <0>
	// <1>
	// <2>
	// <0x6ec5f9>|0x299|+0x022:'1629'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <12>
	// <0x6ec61b>|0x2bb|+0x042:'1643'
	// <0>
	// <0x6ec65d>|0x2fd|+0x030:'1645'
	// <0>
	// <0x6ec68d>|0x32d|+0x004:'1647'
	// <0x6ec691>|0x331|+0x015:'1648'
	// <0>
	// <1>
	// <0x6ec6a6>|0x346|+0x017:'1651'
	// <0x6ec6bd>|0x35d|-0x00f:'1651'
	// <0x6ec6ae>|0x34e|+0x012:'1652'
	// <0x6ec6c0>|0x360|-0x00f:'1652'
	// <0x6ec6b1>|0x351|+0x012:'1653'
	// ******
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
