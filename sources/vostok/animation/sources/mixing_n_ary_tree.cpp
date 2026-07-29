////////////////////////////////////////////////////////////////////////////
//	Created		: 21.09.2011
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/animation/mixing_n_ary_tree.h>

#include "mixing_n_ary_tree_destroyer.h"
#include "mixing_n_ary_tree_animation_node.h"
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

	animation_state* const end	= m_animation_states + m_animations_count;
	for ( animation_state* i = m_animation_states; i != end; ++i )
		if ( i->event_iterator->event_type & time_event_weight_transitions_started )
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
	float4x4 result;
	if ( m_animation_states )
		result						=
			bone_matrices_computer( animated_object, 0, m_animation_states, m_animations_count )
				.get_object_transform( );
	if ( m_animation_states )
		result						= mul4x3(
			bone_matrices_computer( animated_object, 0, m_animation_states, m_animations_count )
				.get_object_transform( ),
			holder->transform
		);
	return							result;
}

void n_ary_tree::set_object_transform( n_ary_tree_animation_node& animation_node )
{
	animation_state& state					= animation_node.animation_state( );
	animation_interval const& interval		= animation_node.animation_intervals( )[ state.animation_interval_id ];
	current_frame_position frame_position;
	frame frame_transform;
	{
		cubic_spline_skeleton_animation_pinned pinned_animation( interval.animation( ) );
		pinned_animation->bone( u32( 0 ) ).get_frame(
			state.animation_time * default_fps,
			frame_transform,
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

// claude@NOTE: the three bone-matrix forwarders below are STRUCTURE-faithful (temporary
// bone_matrices_computer for convert_to_object_matrices/compute_bones_matrices = 1 stmt no
// local; named `computer` local for compute_bones_local_matrices = 2 stmts, per --view
// structure). They are unpaired/~20% only because bone_matrices_computer's ctor/dtor/methods
// are empty STUBs in bone_matrices_computer.cpp (other TU) -> the whole expression DCE-collapses
// to a 1-byte ret. Pairs once that TU is bodied; off-limits to touch here.
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
	animation_state& state					= animation_node.animation_state( );
	n_ary_tree_weight_calculator weight_calculator( time_in_ms, &animation_node );
	animation_node.accept					( weight_calculator );
	state.weight							= weight_calculator.weight( );
	state.animation_interval_time			= animation_interval_time;
	update_animation_time					( state );

	animation_interval const& interval		=
		animation_node.animation_intervals( )[ state.animation_interval_id ];
	cubic_spline_skeleton_animation_pinned pinned_animation( interval.animation( ) );
	current_frame_position frame_position;
	frame frame_transform;
	pinned_animation->bone( u32( 0 ) ).get_frame(
		state.animation_time * default_fps,
		frame_transform,
		frame_position
	);

	math::quaternion const frame_rotation	( frame_transform.rotation );
	object_movement& previous				= state.bone_matrices_computer.previous_object_movement;
	object_movement& accumulated			= state.bone_matrices_computer.accumulated_object_movement;

	accumulated.translation					+= frame_transform.translation - previous.translation;
	accumulated.rotation					=
		( frame_rotation * math::conjugate( previous.rotation ) ) * accumulated.rotation;
	accumulated.scale						*= frame_transform.scale / previous.scale;

	previous.translation					= frame_transform.translation;
	previous.rotation						= frame_rotation;
	previous.scale							= frame_transform.scale;
}

void n_ary_tree::update_synchronization_group_using_integration(
	n_ary_tree_animation_node&		animation_node,
	const u32						start_time_in_ms,
	const u32						target_time_in_ms
)
{
	animation_state& state					= animation_node.animation_state( );
	n_ary_tree_base_node* time_scale_node	=
		animation_node.operands_count( )
			? *animation_node.operands( sizeof( n_ary_tree_animation_node ) )
			: 0;
	bool is_time_scale_node					= time_scale_node && time_scale_node->is_time_scale( );
	float accumulated_animation_time		= state.animation_interval_time;
	animation_interval const& interval		=
		animation_node.animation_intervals( )[ state.animation_interval_id ];
	float const animation_interval_length	= interval.length( );

	u32 const integration_interval_length_in_ms	= 10;
	float const integration_interval_length		=
		float( integration_interval_length_in_ms ) / 1000.f;
	u32 const full_intervals_count				=
		( target_time_in_ms - start_time_in_ms ) / integration_interval_length_in_ms;

	for ( u32 i = 0; i <= full_intervals_count; ++i ) {
		u32 const previous_sample_time	=
			i < full_intervals_count
				? start_time_in_ms + ( i ? 2 * i - 1 : 0 ) * ( integration_interval_length_in_ms / 2 )
				: start_time_in_ms + full_intervals_count * integration_interval_length_in_ms;
		u32 const sample_time			=
			i < full_intervals_count
				? start_time_in_ms + ( 2 * i + 1 ) * ( integration_interval_length_in_ms / 2 )
				: target_time_in_ms;
		n_ary_tree_time_scale_calculator time_scale_calculator(
			sample_time,
			accumulated_animation_time,
			previous_sample_time,
			&animation_node
		);

		if ( is_time_scale_node ) {
			u32 const operands_count		= animation_node.operands_count( );
			time_scale_node->accept		( time_scale_calculator );
			if ( time_scale_calculator.result( ) )
				time_scale_node			= time_scale_calculator.result( );
			if ( operands_count != animation_node.operands_count( ) ) {
				time_scale_node			= 0;
				is_time_scale_node		= false;
			}
		}

		float const time_scale			=
			is_time_scale_node ? time_scale_calculator.time_scale( ) : 1.f;
		u32 const update_time			=
			i < full_intervals_count
				? start_time_in_ms + ( i + 1 ) * integration_interval_length_in_ms
				: target_time_in_ms;
		float const interval_fraction	=
			i < full_intervals_count
				? 1.f
				: float( ( target_time_in_ms - start_time_in_ms ) % integration_interval_length_in_ms )
					/ float( integration_interval_length_in_ms );
		accumulated_animation_time		+= time_scale * integration_interval_length * interval_fraction;

		if ( state.are_there_any_weight_transitions )
			accumulate_object_movement	( animation_node, accumulated_animation_time, update_time );

		u32 const group_id				= animation_node.time_synchronization_group_id( );
		if ( group_id == u32( -1 ) )
			continue;

		for (
			n_ary_tree_animation_node* current = animation_node.m_next_time_animation;
			current && current->time_synchronization_group_id( ) == group_id;
			current = current->m_next_time_animation
		) {
			animation_state& current_state	= current->animation_state( );
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

	animation_interval const& interval	=
		animation_state.event_iterator.animation( ).animation_intervals( )[ animation_state.animation_interval_id ];
	float const animation_time			= interval.start_time( ) + animation_state.animation_interval_time;
	float const animation_length		=
		cubic_spline_skeleton_animation_pinned( interval.animation( ) )->length_in_frames( ) / default_fps;
	animation_state.animation_time		= math::min(
		math::max( animation_time - animation_state.animation_time_threshold, 0.f ),
		animation_length
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

	if ( state.is_freezed )
		return;

	n_ary_tree_animation_node* const driving_animation	= animation_node.time_driving_animation( );
	if ( !driving_animation ) {
		state.animation_interval_time	=
			n_ary_tree_animation_time_calculator(
				animation_node,
				start_time_in_ms,
				state.animation_interval_time,
				target_time_in_ms,
				false
			).animation_time( );
		update_animation_time	( state );
		return;
	}

	animation_state const& driving_state	= driving_animation->animation_state( );
	animation_interval const& driving_interval	=
		driving_animation->animation_intervals( )[ driving_state.animation_interval_id ];
	animation_interval const& interval	=
		animation_node.animation_intervals( )[ state.animation_interval_id ];
	state.animation_interval_time	=
		interval.length( ) / driving_interval.length( ) * driving_state.animation_interval_time;
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
	for (
		;
		current && current->time_synchronization_group_id( ) == synchronization_group_id;
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
				std::find( m_animated_objects, m_animated_objects + m_animated_objects_count, i->animated_object( ) )->need_new_transform = true;
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

// STATE[STUB]
void n_ary_tree::process_event( n_ary_tree_animation_node& current_animation_node, const u32 event_types )
{
	// LOCALS
	// const u32 						event_type
	// n_ary_tree_time_scale_calculator time_scale_calculator
	// const u32 						event_time_in_ms
	// n_ary_tree_time_scale_calculator time_scale_calculator
	// const float 						animation_time
	// const float 						animation_length
	// animation_interval const& 		interval
	// const float 						animation_length
	// animation_interval const& 		interval
	// current_frame_position 			frame_position
	// resources::pinned_ptr_const< cubic_spline_skeleton_animation > pinned_animation
	// current_frame_position 			frame_position
	// n_ary_tree_weight_calculator 	weight_calculator
	// n_ary_tree_event_iterator 		event_iterator
	// resources::pinned_ptr_const< cubic_spline_skeleton_animation > pinned_animation
	// ******

	// CALL SITE INFO
	// <0x6efdcc> -> bool < unknown >()
	// <0x6efde1> -> void < unknown >( n_ary_tree_visitor& )
	// <0x6efdea> -> bool < unknown >()
	// <0x6efe59> -> void < unknown >( n_ary_tree_visitor& )
	// ******

	// FUNCTION BODY
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6efd2f>|0x00f|+0x003:'1244'
	// <0x6efd32>|0x012|+0x010:'1245'
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
	// <16>
	// <17>
	// <18>
	// <19>
	// <20>
	// <21>
	// <22>
	// <23>
	// <24>
	// <25>
	// <26>
	// <27>
	// <28>
	// <29>
	// <30>
	// <0x6efd42>|0x022|+0x019:'1277'
	// <0x6efd5b>|0x03b|+0x05e:'1278'
	// <0>
	// <0x6efdb9>|0x099|+0x046:'1280'
	// <0x6efdff>|0x0df|-0x03e:'1280'
	// <0x6efdc1>|0x0a1|+0x011:'1281'
	// <0x6efdd2>|0x0b2|+0x011:'1282'
	// <0x6efde3>|0x0c3|+0x01e:'1283'
	// <0x6efe01>|0x0e1|+0x008:'1283'
	// <0>
	// <1>
	// <2>
	// <0x6efe09>|0x0e9|+0x00c:'1287'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6efe15>|0x0f5|+0x010:'1292'
	// <0x6efe25>|0x105|+0x036:'1293'
	// <0x6efe5b>|0x13b|+0x006:'1294'
	// <0>
	// <1>
	// <0x6efe61>|0x141|+0x00d:'1297'
	// <0x6efe6e>|0x14e|+0x00c:'1298'
	// <0x6efe7a>|0x15a|+0x00a:'1299'
	// <0x6efe84>|0x164|+0x08e:'1300'
	// <0>
	// <0x6eff12>|0x1f2|+0x00f:'1302'
	// <0x6eff21>|0x201|+0x008:'1303'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6eff29>|0x209|+0x00b:'1309'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6eff34>|0x214|+0x00c:'1314'
	// <0x6eff40>|0x220|+0x006:'1315'
	// <0x6eff46>|0x226|+0x008:'1316'
	// <0x6eff4e>|0x22e|+0x008:'1317'
	// <0x6eff56>|0x236|+0x064:'1318'
	// <0>
	// <0x6effba>|0x29a|+0x005:'1320'
	// <0x6effbf>|0x29f|+0x011:'1321'
	// <0x6effd0>|0x2b0|+0x070:'1322'
	// <0x6f0040>|0x320|+0x006:'1323'
	// <0>
	// <0x6f0046>|0x326|+0x017:'1325'
	// <0x6f005d>|0x33d|+0x007:'1326'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6f0064>|0x344|+0x00b:'1331'
	// <0x6f006f>|0x34f|+0x004:'1332'
	// <0x6f0073>|0x353|+0x086:'1333'
	// <0x6f00f9>|0x3d9|+0x007:'1334'
	// <0x6f0100>|0x3e0|+0x003:'1335'
	// <0>
	// <0x6f0103>|0x3e3|+0x002:'1337'
	// <0>
	// <0x6f0105>|0x3e5|+0x008:'1339'
	// <0x6f010d>|0x3ed|+0x00c:'1340'
	// <0x6f0119>|0x3f9|-0x009:'1340'
	// <0x6f0110>|0x3f0|+0x005:'1341'
	// <0x6f0115>|0x3f5|+0x009:'1342'
	// <0>
	// <1>
	// <2>
	// <0x6f011e>|0x3fe|+0x00b:'1346'
	// <0x6f0129>|0x409|+0x00a:'1347'
	// <0x6f0133>|0x413|+0x003:'1348'
	// <0>
	// <0x6f0136>|0x416|+0x012:'1350'
	// <0>
	// <0x6f0148>|0x428|+0x010:'1352'
	// <0x6f0158>|0x438|+0x073:'1353'
	// <0x6f01cb>|0x4ab|+0x026:'1354'
	// <0x6f01f1>|0x4d1|+0x016:'1355'
	// <0x6f0207>|0x4e7|+0x006:'1356'
	// <0x6f020d>|0x4ed|+0x002:'1357'
	// <0>
	// <1>
	// <2>
	// <0x6f020f>|0x4ef|+0x003:'1361'
	// <0>
	// <0x6f0212>|0x4f2|+0x00e:'1363'
	// <0>
	// <1>
	// <0x6f0220>|0x500|+0x005:'1366'
	// <0>
	// <1>
	// <2>
	// <0x6f0225>|0x505|+0x044:'1370'
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
	// <16>
	// <17>
	// <0x6f0269>|0x549|+0x007:'1389'
	// <0x6f0270>|0x550|+0x005:'1390'
	// <0>
	// <1>
	// <2>
	// <0x6f0275>|0x555|+0x044:'1394'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x6f02b9>|0x599|+0x00b:'1402'
	// <0x6f02c4>|0x5a4|+0x009:'1403'
	// <0x6f02cd>|0x5ad|+0x026:'1404'
	// <0>
	// <1>
	// <0x6f02f3>|0x5d3|+0x046:'1407'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6f0339>|0x619|+0x02d:'1412'
	// <0x6f0366>|0x646|+0x007:'1413'
	// <0>
	// <1>
	// <0x6f036d>|0x64d|+0x042:'1416'
	// <0x6f03af>|0x68f|+0x158:'1417'
	// <0x6f0507>|0x7e7|+0x04b:'1418'
	// <0x6f0552>|0x832|+0x006:'1419'
	// <0>
	// <1>
	// <2>
	// <0x6f0558>|0x838|+0x00e:'1423'
	// <0>
	// <0x6f0566>|0x846|+0x00c:'1425'
	// <0x6f0572>|0x852|+0x032:'1426'
	// <0>
	// <1>
	// <0x6f05a4>|0x884|+0x046:'1429'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6f05ea>|0x8ca|+0x030:'1434'
	// <0x6f061a>|0x8fa|+0x007:'1435'
	// <0>
	// <1>
	// <0x6f0621>|0x901|+0x033:'1438'
	// <0x6f0654>|0x934|+0x044:'1439'
	// <0x6f0698>|0x978|+0x036:'1440'
	// <0>
	// <1>
	// <0x6f06ce>|0x9ae|+0x01b:'1443'
	// <0x6f06e9>|0x9c9|+0x026:'1444'
	// <0>
	// <1>
	// <0x6f070f>|0x9ef|+0x011:'1447'
	// <0x6f0720>|0xa00|+0x03a:'1448'
	// <0x6f075a>|0xa3a|+0x030:'1449'
	// <0>
	// <0x6f078a>|0xa6a|+0x057:'1451'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// ******
}

void n_ary_tree::process_events( const u32 target_time_in_ms, const u32 event_types )
{
	for ( n_ary_tree_animation_node* i = m_weight_root; i; i = i->m_next_weight_animation )
		if ( ( i->animation_state( ).event_iterator->event_time_in_ms == target_time_in_ms ) &&
			 ( event_types & i->animation_state( ).event_iterator->event_type ) )
			process_event			( *i, event_types );
}

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

// STATE[STUB]
bool n_ary_tree::dispatch_callbacks(
	callback_generator_info const* const	callback_generators_head,
	subscribed_channel*&	channels_head,
	const u32				current_time_in_ms,
	bool&					callbacks_are_actual
)
{
	// LOCALS
	// bool 							result
	// callback_generator_info const* 	generator
	// resources::pinned_ptr_const< cubic_spline_skeleton_animation > pinned_animation
	// animation_callback_params 		params
	// subscribed_channel const* 		subscribed_channel
	// u8 								domain_data
	// const u32 						knot_upper_id
	// animation_callback_params 		params
	// ******

	// CONSTANTS
	// const n_ary_tree::dispatch_callbacks::__l77::< unnamed-tag > partitions = 1;
	// const n_ary_tree::dispatch_callbacks::__l77::< unnamed-tag > intervals = 0;
	// const n_ary_tree::dispatch_callbacks::__l77::< unnamed-tag > events = 2;
	// ******

	// CALL SITE INFO
	// <0x6ee5a7> -> < unknown >
	// <0x6ee948> -> < unknown >
	// ******

	return false;

	// FUNCTION BODY
	// <0x6ee486>|0x016|+0x008:'1526'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6ee48e>|0x01e|+0x012:'1533'
	// <0x6ee4a0>|0x030|+0x00a:'1534'
	// <0x6ee4aa>|0x03a|+0x150:'1535'
	// <0x6ee5fa>|0x18a|-0x13a:'1535'
	// <0x6ee4c0>|0x050|+0x00c:'1536'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6ee4cc>|0x05c|+0x009:'1542'
	// <0x6ee4d5>|0x065|+0x005:'1543'
	// <0>
	// <0x6ee4da>|0x06a|+0x009:'1545'
	// <0x6ee4e3>|0x073|+0x005:'1546'
	// <0>
	// <0x6ee4e8>|0x078|+0x00d:'1548'
	// <0x6ee4f5>|0x085|+0x005:'1549'
	// <0>
	// <0x6ee4fa>|0x08a|+0x008:'1551'
	// <0>
	// <1>
	// <0x6ee502>|0x092|+0x0ed:'1554'
	// <0x6ee5ef>|0x17f|-0x0df:'1554'
	// <0x6ee510>|0x0a0|+0x00a:'1555'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6ee51a>|0x0aa|+0x018:'1560'
	// <0>
	// <1>
	// <0x6ee532>|0x0c2|+0x010:'1563'
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
	// <0x6ee542>|0x0d2|+0x06c:'1579'
	// <0x6ee5ae>|0x13e|+0x013:'1580'
	// <0x6ee5c1>|0x151|+0x013:'1581'
	// <0x6ee5d4>|0x164|+0x031:'1582'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6ee605>|0x195|+0x00a:'1587'
	// <0>
	// <1>
	// <0x6ee60f>|0x19f|+0x050:'1590'
	// <0x6ee65f>|0x1ef|+0x004:'1591'
	// <0x6ee663>|0x1f3|+0x006:'1592'
	// <0x6ee669>|0x1f9|+0x04c:'1593'
	// <0>
	// <0x6ee6b5>|0x245|+0x2eb:'1595'
	// <0x6ee9a0>|0x530|-0x2d0:'1595'
	// <0x6ee6d0>|0x260|+0x059:'1596'
	// <0x6ee729>|0x2b9|+0x009:'1597'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6ee732>|0x2c2|+0x011:'1602'
	// <0>
	// <1>
	// <0x6ee743>|0x2d3|+0x006:'1605'
	// <0x6ee749>|0x2d9|+0x009:'1606'
	// <0x6ee752>|0x2e2|+0x04c:'1607'
	// <0>
	// <0x6ee79e>|0x32e|+0x012:'1609'
	// <0x6ee7b0>|0x340|+0x012:'1610'
	// <0x6ee7c2>|0x352|+0x025:'1611'
	// <0x6ee7e7>|0x377|+0x03b:'1612'
	// <0>
	// <0x6ee822>|0x3b2|+0x02e:'1614'
	// <0x6ee850>|0x3e0|+0x008:'1615'
	// <0>
	// <1>
	// <0x6ee858>|0x3e8|+0x00e:'1618'
	// <0>
	// <0x6ee866>|0x3f6|+0x002:'1620'
	// <0x6ee868>|0x3f8|+0x002:'1621'
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
	// <0x6ee86a>|0x3fa|+0x004:'1638'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6ee86e>|0x3fe|+0x127:'1644'
	// <0x6ee995>|0x525|-0x0f1:'1644'
	// <0x6ee8a4>|0x434|+0x00a:'1645'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6ee8ae>|0x43e|+0x018:'1650'
	// <0>
	// <1>
	// <0x6ee8c6>|0x456|+0x00f:'1653'
	// <0>
	// <1>
	// <0x6ee8d5>|0x465|+0x010:'1656'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6ee8e5>|0x475|+0x07a:'1661'
	// <0x6ee95f>|0x4ef|+0x01c:'1662'
	// <0x6ee97b>|0x50b|+0x038:'1663'
	// <0>
	// <1>
	// <0x6ee9b3>|0x543|+0x061:'1666'
	// <0>
	// <0x6eea14>|0x5a4|+0x004:'1668'
	// ******
}

// STATE[STUB]
void n_ary_tree::remove_animations( const u32 target_time_in_ms )
{
	// LOCALS
	// animation_state* 				current_alive_animation_state
	// buffer_vector< pcvoid > 			animated_objects
	// animation_state* 				current_animation_state
	// n_ary_tree_animation_node* 		previous_animation
	// n_ary_tree_animation_node* 		current_animation
	// animated_object_holder* const 	e
	// animated_object_holder* 			j
	// ******

	// FUNCTION BODY
	// <0>
	// <0x6ef358>|0x008|+0x021:'1674'
	// <0>
	// <1>
	// <2>
	// <0x6ef379>|0x029|+0x02d:'1678'
	// <0>
	// <1>
	// <0x6ef3a6>|0x056|+0x00b:'1681'
	// <0x6ef3b1>|0x061|+0x009:'1682'
	// <0>
	// <0x6ef3ba>|0x06a|+0x038:'1684'
	// <0>
	// <1>
	// <0x6ef3f2>|0x0a2|+0x010:'1687'
	// <0x6ef402>|0x0b2|+0x005:'1688'
	// <0>
	// <1>
	// <2>
	// <0x6ef407>|0x0b7|+0x018:'1692'
	// <0x6ef41f>|0x0cf|+0x012:'1693'
	// <0>
	// <0x6ef431>|0x0e1|+0x00a:'1695'
	// <0x6ef43b>|0x0eb|+0x006:'1696'
	// <0x6ef441>|0x0f1|+0x003:'1697'
	// <0>
	// <1>
	// <2>
	// <0x6ef444>|0x0f4|+0x003:'1701'
	// <0x6ef447>|0x0f7|+0x01e:'1702'
	// <0>
	// <1>
	// <0x6ef465>|0x115|+0x00c:'1705'
	// <0x6ef471>|0x121|+0x01f:'1706'
	// <0x6ef490>|0x140|+0x00e:'1707'
	// <0>
	// <0x6ef49e>|0x14e|+0x0a8:'1709'
	// <0>
	// <1>
	// <0x6ef546>|0x1f6|-0x056:'1712'
	// <0x6ef4f0>|0x1a0|+0x059:'1713'
	// <0>
	// <0x6ef549>|0x1f9|+0x00f:'1715'
	// <0x6ef558>|0x208|+0x056:'1716'
	// <0x6ef5ae>|0x25e|-0x03e:'1716'
	// <0x6ef570>|0x220|+0x012:'1717'
	// <0x6ef582>|0x232|+0x007:'1718'
	// <0x6ef589>|0x239|+0x012:'1719'
	// <0>
	// <1>
	// <0x6ef59b>|0x24b|+0x016:'1722'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6ef5b1>|0x261|+0x00b:'1728'
	// <0>
	// ******
}

// STATE[STUB]
bool n_ary_tree::update_event_iterators_and_dispatch_callbacks(
	const u32				target_time_in_ms,
	subscribed_channel*&	channels_head,
	bool&					callbacks_are_actual
)
{
	// LOCALS
	// callback_generator_info const* 	callback_generators_head
	// callback_generator_info* 		previous_generator_info
	// animation_state const* const 	e
	// const u16 						event_type
	// const bool 						result
	// ******

	return false;

	// FUNCTION BODY
	// <0>
	// <1>
	// <2>
	// <0x6f08f8>|0x008|+0x0e8:'1741'
	// <0x6f09e0>|0x0f0|-0x0c0:'1741'
	// <0x6f0920>|0x030|+0x00f:'1742'
	// <0>
	// <1>
	// <0x6f092f>|0x03f|+0x010:'1745'
	// <0>
	// <1>
	// <0x6f093f>|0x04f|+0x015:'1748'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x6f0954>|0x064|+0x006:'1756'
	// <0>
	// <1>
	// <0x6f095a>|0x06a|+0x003:'1759'
	// <0x6f095d>|0x06d|+0x00d:'1760'
	// <0>
	// <1>
	// <0x6f096a>|0x07a|+0x015:'1763'
	// <0x6f097f>|0x08f|+0x003:'1764'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x6f0982>|0x092|+0x038:'1773'
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
	// <0x6f09ba>|0x0ca|+0x007:'1790'
	// <0x6f09c1>|0x0d1|+0x003:'1791'
	// <0x6f09c4>|0x0d4|+0x002:'1792'
	// <0x6f09c6>|0x0d6|+0x003:'1793'
	// <0>
	// <0x6f09c9>|0x0d9|+0x01a:'1795'
	// <0>
	// <1>
	// <0x6f09e3>|0x0f3|+0x00a:'1798'
	// <0x6f09ed>|0x0fd|+0x009:'1799'
	// <0x6f09f6>|0x106|+0x015:'1800'
	// <0>
	// <0x6f0a0b>|0x11b|+0x006:'1802'
	// <0x6f0a11>|0x121|+0x002:'1803'
	// <0x6f0a13>|0x123|+0x003:'1804'
	// <0x6f0a16>|0x126|+0x009:'1805'
	// <0>
	// <1>
	// <0x6f0a1f>|0x12f|+0x003:'1808'
	// ******
}

// STATE[STUB]
callback_generator_info* n_ary_tree::generate_animation_lexeme_end_events(
	n_ary_tree const&					previous_tree,
	n_ary_tree const&					new_tree,
	callback_generator_info*			callback_generators_buffer_begin,
	callback_generator_info* const		callback_generators_buffer_end,
	subscribed_channel* const			channels_head
)
{
	// LOCALS
	// callback_generator_info* 		callback_generators_head
	// callback_generator_info* 		previous_generator_info
	// ******

	return NULL;

	// FUNCTION BODY
	// <0>
	// <1>
	// <2>
	// <0x6ee307>|0x007|+0x150:'1822'
	// <0x6ee457>|0x157|-0x137:'1822'
	// <0x6ee320>|0x020|+0x02d:'1823'
	// <0x6ee34d>|0x04d|+0x008:'1823'
	// <0>
	// <1>
	// <2>
	// <0x6ee355>|0x055|+0x05f:'1827'
	// <0x6ee3b4>|0x0b4|-0x051:'1827'
	// <0x6ee363>|0x063|+0x007:'1828'
	// <0x6ee36a>|0x06a|+0x043:'1829'
	// <0x6ee3ad>|0x0ad|-0x03c:'1829'
	// <0x6ee371>|0x071|+0x006:'1830'
	// <0>
	// <1>
	// <0x6ee377>|0x077|+0x02a:'1833'
	// <0>
	// <1>
	// <0x6ee3a1>|0x0a1|-0x056:'1836'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6ee34b>|0x04b|+0x075:'1843'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x6ee3c0>|0x0c0|+0x004:'1852'
	// <0>
	// <1>
	// <2>
	// <0x6ee3c4>|0x0c4|+0x012:'1856'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x6ee3d6>|0x0d6|+0x06c:'1865'
	// <0>
	// <0x6ee442>|0x142|+0x008:'1867'
	// <0x6ee44a>|0x14a|+0x003:'1868'
	// <0x6ee44d>|0x14d|+0x002:'1869'
	// <0x6ee44f>|0x14f|+0x004:'1870'
	// <0>
	// <0x6ee453>|0x153|+0x00f:'1872'
	// <0>
	// <1>
	// <0x6ee462>|0x162|+0x004:'1875'
	// ******
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

// claude@NOTE: tick body is structure-faithful; residual is stubbed callees
// (process_events->process_event, update_animation_states->update_time_synchronization_group,
// update_event_iterators_and_dispatch_callbacks, set_object_transform(node&)) whose empty bodies
// let the optimizer DCE the loop effects. % rises as those callees get bodied.
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

	for ( u32 event_time_in_ms; ( event_time_in_ms = (*m_animation_events)->event_iterator->event_time_in_ms ) <= target_time_in_ms; ) {
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
		user_handled_callbacks		= update_event_iterators_and_dispatch_callbacks( event_time_in_ms, channels_head, callbacks_are_actual );
	}

	if ( m_weight_root && m_tree_actual_time_in_ms != target_time_in_ms )
		update_animation_states	( m_tree_actual_time_in_ms, target_time_in_ms );

	m_tree_actual_time_in_ms	= target_time_in_ms;

	return						user_handled_callbacks;
}

} // namespace mixing
} // namespace animation
} // namespace vostok
