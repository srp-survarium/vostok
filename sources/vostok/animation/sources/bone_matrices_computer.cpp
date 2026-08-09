////////////////////////////////////////////////////////////////////////////
//	Created		: 12.08.2011
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "bone_matrices_computer.h"
#include <vostok/animation/skeleton.h>
#include "mixing_animation_state.h"
#include "mixing_n_ary_tree_animation_node.h"

using vostok::animation::skeleton_bone;
using vostok::animation::skeleton;
using vostok::animation::bone_matrices_computer;
using vostok::math::float4x4;
using vostok::math::float3;
using vostok::math::float4;
using vostok::math::quaternion;
using vostok::animation::animation_types_enum;
using vostok::animation::frame;
using vostok::buffer_vector;

namespace math = vostok::math;

namespace vostok {
namespace animation {

struct bone_transform {
	inline bone_transform	( )
	{
	}

	inline bone_transform	(
			float3 const& translation,
			quaternion const& rotation,
			float3 const& scale,
			bool const visibility = true
		) :
		translation	( translation ),
		rotation	( rotation ),
		scale		( scale ),
		visibility	( visibility )
	{
	}

	inline explicit bone_transform	(
			frame const& frame,
			bool const visibility = true
		) :
		translation	( frame.translation ),
		rotation	( frame.rotation ),
		scale		( frame.scale ),
		visibility	( visibility )
	{
	}

	inline	void make_zero	( )
	{
		translation	= float3( 0.f, 0.f, 0.f ),
		rotation	= quaternion( float4(0.f, 0.f, 1.f, 0.f) ),
		scale		= float3( 0.f, 0.f, 0.f ),
		visibility	= true;
	}

	inline	void apply	( bone_transform const& transform )
	{
		translation	+= transform.translation;
		rotation	= rotation * transform.rotation;
		scale		*= transform.scale;
		visibility	&= transform.visibility;
	}

	float3			translation;
	quaternion		rotation;
	float3			scale;
	bool			visibility;
}; // struct bone_transform

STATIC_SIZE_ASSERT(bone_transform, 0x2C);

} // namespace animation
} // namespace vostok

namespace vostok {
namespace animation {

bone_matrices_computer::bone_matrices_computer(
	pcvoid const						animated_object,
	skeleton const*						skeleton,
	mixing::animation_state* const		animations,
	const u32							animations_count
) :
	m_animated_object			( animated_object ),
	m_skeleton					( skeleton ),
	m_animations				( animations ),
	m_animations_count			( animations_count ),
	m_layers_count				( 0 ),
	m_overweighting_detected	( false )
{
	for ( mixing::animation_state* i = animations, * const e = animations + animations_count; i != e; ++i ) {
		mixing::n_ary_tree_animation_node& animation_node	= i->event_iterator.animation();
		if ( animation_node.animated_object() != m_animated_object )
			continue;

		m_layers_count	= math::max( m_layers_count, animation_node.additivity_priority() );
		i->bone_matrices_computer.pinned_animation	= cubic_spline_skeleton_animation_pinned(
			( animation_node.animation_intervals() + i->animation_interval_id )->animation()
		);
	}

	++m_layers_count;
}

bone_matrices_computer::~bone_matrices_computer( )
{
	for ( mixing::animation_state* i = m_animations, * const e = m_animations + m_animations_count; i != e; ++i )
		i->bone_matrices_computer.pinned_animation	= cubic_spline_skeleton_animation_pinned( 0 );
}

} // namespace animation
} // namespace vostok

static float3 mix_translations( buffer_vector< std::pair< float3, float > > const& transforms )
{
	float3	result( 0.f, 0.f, 0.f );
	for ( std::pair< float3, float > const* i = transforms.begin(), * const e = transforms.end(); i != e; ++i )
		result	+= i->first * i->second;

	return	result;
}

static math::quaternion mix_rotations(
		buffer_vector< std::pair< float3, float > >&	transforms,
		const bool										do_normalization
	)
{
	if ( transforms.empty( ) )
		return				quaternion( float4( 0.f, 0.f, 0.f, 1.f ) );

	typedef buffer_vector< std::pair< quaternion, float > >	rotations_type;
	rotations_type	rotations( ALLOCA( sizeof( rotations_type::value_type ) * transforms.size( ) ), transforms.size( ) );
	for ( std::pair< float3, float > const* i = transforms.begin( ), * const e = transforms.end( ); i != e; ++i ) {
		if ( math::abs( i->second ) < math::epsilon_5 )
			continue;

		rotations.push_back	( std::make_pair( quaternion( i->first ), i->second ) );
	}

	if ( rotations.empty( ) )
		return				quaternion( float4( 0.f, 0.f, 0.f, 1.f ) );

	if ( rotations.size( ) == 1 ) {
		if ( do_normalization )
			return			rotations.front( ).first;

		float				angle;
		float3				direction;
		rotations.front( ).first.get_axis_and_angle	( direction, angle );
		return				quaternion( direction, angle * rotations.front( ).second );
	}

	if ( rotations.size( ) == 2 ) {
		const float			total_weight	= rotations.front( ).second + rotations.back( ).second;
		quaternion			mix				= math::slerp_optimized( rotations.front( ).first, rotations.back( ).first, rotations.back( ).second / total_weight );
		if ( do_normalization )
			return			mix;

		float				angle;
		float3				direction;
		mix.get_axis_and_angle	( direction, angle );
		return				quaternion( direction, angle * total_weight );
	}

	return					math::extrapolated_slerp( rotations.begin( ), rotations.end( ) );
}

static float3 mix_scales( buffer_vector< std::pair< float3, float > > const& transforms )
{
	float3	result( 1.f, 1.f, 1.f );
	for ( std::pair< float3, float > const* i = transforms.begin(), * const e = transforms.end(); i != e; ++i )
		result	*= math::pow( i->first, i->second );

	return	result;
}

frame identity_frame( )
{
	frame	result;
	result.translation	= float3( 0.f, 0.f, 0.f );
	result.rotation		= float3( 0.f, 0.f, 0.f );
	result.scale		= float3( 1.f, 1.f, 1.f );
	return	result;
}

namespace vostok {
namespace animation {

bone_transform bone_matrices_computer::computed_local_bone_transform( skeleton_bone const& bone, const u32 bone_mask, const u32 animation_layer_id ) const
{
	typedef std::pair< float3, float > weighted_transform;
	buffer_vector< weighted_transform > translations( ALLOCA( sizeof( weighted_transform ) * m_animations_count ), m_animations_count );
	buffer_vector< weighted_transform > rotations( ALLOCA( sizeof( weighted_transform ) * m_animations_count ), m_animations_count );
	buffer_vector< weighted_transform > scales( ALLOCA( sizeof( weighted_transform ) * m_animations_count ), m_animations_count );

	mixing::animation_state* const e	= m_animations + m_animations_count;
	for ( mixing::animation_state* i = m_animations; i != e; ++i ) {
		mixing::n_ary_tree_animation_node& animation_node	= i->event_iterator.animation( );
		if ( animation_node.animated_object( ) != m_animated_object )
			continue;
		if ( i->weight == 0.f )
			continue;
		if ( animation_node.additivity_priority( ) != animation_layer_id )
			continue;
		if ( !( animation_node.bones_mask( ) & bone_mask ) )
			continue;

		frame animation_frame;
		current_frame_position frame_position;
		cubic_spline_skeleton_animation const& animation	= *i->bone_matrices_computer.pinned_animation;
		if ( animation.get_bone_names( ).bone_index( bone.id( ) ) == u32(-1) ) animation_frame = identity_frame( ); else animation.bone( animation.get_bone_names( ).bone_index( bone.id( ) ) ).get_frame( i->animation_time * default_fps, animation_frame, frame_position );

		translations.push_back	( std::make_pair( animation_frame.translation, i->weight ) );
		rotations.push_back		( std::make_pair( animation_frame.rotation, i->weight ) );
		scales.push_back		( std::make_pair( animation_frame.scale, i->weight ) );
	}

	if ( animation_layer_id == 0 ) {
		float total_weight	= 0.f;
		for ( weighted_transform const* i = rotations.begin( ), * const end = rotations.end( ); i != end; ++i )
			total_weight	+= i->second;

		if ( math::abs( total_weight - 1.f ) >= math::epsilon_5 ) {
			float const inverted_total_weight	= 1.f / total_weight;
			for ( weighted_transform* i = rotations.begin( ), * const end = rotations.end( ); i != end; ++i )
				i->second	*= inverted_total_weight;

			float normalized_total_weight	= 0.f;
			for ( weighted_transform const* i = rotations.begin( ), * const end = rotations.end( ); i != end; ++i )
				normalized_total_weight	+= i->second;

			if ( !m_overweighting_detected && math::abs( 1.f - normalized_total_weight ) >= .5f )
				m_overweighting_detected	= true;
		}
	}

	return bone_transform(
		mix_translations( translations ),
		mix_rotations( rotations, animation_layer_id < 2 ),
		mix_scales( scales )
	);
}

float4x4 bone_matrices_computer::computed_local_bone_matrix( skeleton_bone const& bone, const u32 bone_mask ) const
{
	typedef buffer_vector< bone_transform >	bone_transforms_type;
	bone_transforms_type	bone_transforms( ALLOCA( sizeof( bone_transform ) * m_layers_count ), m_layers_count );
	for ( u32 i = 0; i < m_layers_count; ++i )
		bone_transforms.push_back	( computed_local_bone_transform( bone, bone_mask, i ) );


	bone_transform	result	= bone_transforms.front( );
	for ( bone_transform const* i = bone_transforms.begin( ) + 1; i != bone_transforms.end( ); ++i )
		result.apply			( *i );
	return					mul4x3(
		mul4x3(
			create_scale( result.scale ), create_rotation( result.rotation )
		),
		create_translation( result.translation )
	);
}

void bone_matrices_computer::compute_skeleton_branch(
	skeleton_bone const&	bone,
	float4x4* const			result,
	float4x4 const&			parent,
	u32 const* const		bone_mask,
	u32 const* const		result_masks
) const
{
	*result	= mul4x3( computed_local_bone_matrix( bone, bone_mask ? *bone_mask : bone.mask() ), parent );

	skeleton_bone const* const e	= bone.children_end();
	for ( skeleton_bone const* i = bone.children_begin(); i != e; ++i )
		compute_skeleton_branch(
			*i,
			result + ( i - &bone ),
			*result,
			result_masks ? result_masks + ( bone.children_begin() - &bone ) : 0,
			result_masks ? result_masks + ( i - &bone ) : 0
		);
}

void bone_matrices_computer::compute_skeleton_branch_local(
	skeleton_bone const&	bone,
	float4x4* const			result,
	u32 const* const		bone_mask,
	u32 const* const		result_masks
) const
{
	*result	= computed_local_bone_matrix( bone, bone_mask ? *bone_mask : bone.mask() );

	skeleton_bone const* const e	= bone.children_end();
	for ( skeleton_bone const* i = bone.children_begin(); i != e; ++i )
		compute_skeleton_branch_local(
			*i,
			result + ( i - &bone ),
			bone_mask ? bone_mask + ( bone.children_begin() - &bone ) : 0,
			bone_mask ? bone_mask + ( i - &bone ) : 0
		);
}

void bone_matrices_computer::convert_skeleton_branch( skeleton_bone const& bone, float4x4* const result, float4x4 const& parent ) const
{
	*result	= mul4x3( *result, parent );

	skeleton_bone const* i			= bone.children_begin();
	skeleton_bone const* const e	= bone.children_end();
	for ( ; i != e; ++i )
		convert_skeleton_branch( *i, result + ( i - &bone ), *result );
}

void bone_matrices_computer::compute_bones_matrices( float4x4* const begin, float4x4* const end, u32 const* const bones_masks ) const
{


	skeleton_bone const* const roots_begin	= &m_skeleton->get_root();
	skeleton_bone const* const roots_end	= roots_begin->children_begin();
	for ( skeleton_bone const* i = roots_begin; i != roots_end; ++i ) {
		skeleton_bone const* const children_begin	= i->children_begin(),
							* const children_end	= i->children_end();
		for ( skeleton_bone const* j = children_begin; j != children_end; ++j )
			compute_skeleton_branch(
				*j,
				begin + ( j - children_begin ),
				float4x4( ).identity( ),
				bones_masks ? bones_masks + ( j - i ) : 0,
				bones_masks ? bones_masks + ( j - children_begin ) : 0

			);
	}
}

void bone_matrices_computer::compute_bones_local_matrices( float4x4* const begin, float4x4* const end, u32 const* const bones_masks ) const
{

	skeleton_bone const* const roots_begin	= &m_skeleton->get_root();
	skeleton_bone const* const roots_end	= roots_begin->children_begin();
	for ( skeleton_bone const* i = roots_begin; i != roots_end; ++i )
	{
		skeleton_bone const* const children_begin	= i->children_begin(),
							* const children_end	= i->children_end();
		for ( skeleton_bone const* j = children_begin; j != children_end; ++j )
			compute_skeleton_branch_local(
				*j,
				begin + ( j - children_begin ),
				bones_masks ? bones_masks + ( j - i ) : 0,
				bones_masks ? bones_masks + ( j - children_begin ) : 0

			);
	}
}

void bone_matrices_computer::convert_to_object_matrices( float4x4* begin, float4x4* const end ) const
{
	skeleton_bone const* const roots_end	= m_skeleton->get_root().children_begin();
	for ( skeleton_bone const* i = &m_skeleton->get_root(); i != roots_end; ++i )
		for ( skeleton_bone const* j = i->children_begin(); j != i->children_end(); ++j, ++begin )
			convert_skeleton_branch( *j, begin, float4x4( ).identity( ) );
}

float4x4 bone_matrices_computer::get_object_transform( ) const
{
	typedef std::pair< float3, float > weighted_transform;
	buffer_vector< weighted_transform > rotations( ALLOCA( sizeof( weighted_transform ) * m_animations_count ), m_animations_count );
	buffer_vector< weighted_transform > translations( ALLOCA( sizeof( weighted_transform ) * m_animations_count ), m_animations_count );
	buffer_vector< weighted_transform > scales( ALLOCA( sizeof( weighted_transform ) * m_animations_count ), m_animations_count );

	mixing::animation_state* const e	= m_animations + m_animations_count;
	for ( mixing::animation_state* i = m_animations; i != e; ++i ) {
		mixing::n_ary_tree_animation_node& animation_node	= i->event_iterator.animation( );
		if ( animation_node.animated_object( ) != m_animated_object )
			continue;
		if ( i->bone_matrices_computer.pinned_animation->animation_type( ) != animation_type_full )
			continue;
		if ( !animation_node.can_generate_events( ) )
			continue;

		mixing::object_movement const& accumulated_movement	=
			i->bone_matrices_computer.accumulated_object_movement;
		if ( i->are_there_any_weight_transitions ) {
			scales.push_back				( std::make_pair( accumulated_movement.scale, i->weight ) );
			rotations.push_back				( std::make_pair(
				create_matrix( accumulated_movement.rotation, float3( 0.f, 0.f, 0.f ) ).get_angles_xyz( ),
				i->weight
			) );
			translations.push_back			( std::make_pair( accumulated_movement.translation, i->weight ) );
		}
		else {
			frame animation_frame;
			current_frame_position frame_position;
			i->bone_matrices_computer.pinned_animation->bone( u32( 0 ) ).get_frame(
				i->animation_time * default_fps,
				animation_frame,
				frame_position
			);
			bone_transform const movement	( animation_frame );
			scales.push_back				( std::make_pair(
				movement.scale / accumulated_movement.scale,
				i->weight
			) );
			rotations.push_back				( std::make_pair(
				create_matrix(
					movement.rotation * math::conjugate( accumulated_movement.rotation ),
					float3( 0.f, 0.f, 0.f )
				).get_angles_xyz( ),
				i->weight
			) );
			translations.push_back			( std::make_pair(
				(
					accumulated_movement.rotation *
					quaternion(
						float4(
							movement.translation - accumulated_movement.translation,
							0.f
						)
					) *
					math::conjugate( accumulated_movement.rotation )
				).vector.xyz( ),
				i->weight
			) );
		}
	}

	return mul4x3(
		mul4x3(
			create_scale( mix_scales( scales ) ),
			create_rotation( mix_rotations( rotations, true ) )
		),
		create_translation( mix_translations( translations ) )
	);
}

} // namespace animation
} // namespace vostok
