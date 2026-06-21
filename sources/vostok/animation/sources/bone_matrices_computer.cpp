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
using vostok::math::quaternion;
using vostok::animation::animation_types_enum;

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

// STATE[STUB]
// claude@NOTE: init list matched; body is a loop over m_animations selecting those whose
// event_iterator animation node targets m_animated_object, accumulating m_layers_count via
// math::max and pinning each cubic_spline_skeleton_animation. Needs n_ary_tree_animation_node
// + cubic_spline animation accessors (esi=*(anim+0xAC), cmp [esi+24h]==m_animated_object,
// max(m_layers_count,[esi+48h])); large pinned_ptr/event-iterator reconstruction - parked.
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
	// LOCALS
	// mixing::animation_state* 		e
	// ******

	// FUNCTION BODY
	// <0x6edf40>|0x000|+0x03b:'92'	{
	// <0x6edf7b>|0x03b|-0x02b:'92'
	// <0>
	// <0x6edf50>|0x010|+0x030:'94'
	// <0x6edf80>|0x040|+0x003:'95'
	// <0x6edf83>|0x043|+0x00c:'96'
	// <0>
	// <1>
	// <0x6edf8f>|0x04f|+0x00c:'99'
	// <0x6edf9b>|0x05b|+0x00e:'100'
	// <0x6edfa9>|0x069|+0x0c1:'101'
	// <0>
	// <1>
	// <2>
	// <0x6ee06a>|0x12a|      :'105'	}
	// ******
}

bone_matrices_computer::~bone_matrices_computer( )
{
	for ( mixing::animation_state* i = m_animations, * const e = m_animations + m_animations_count; i != e; ++i )
		i->bone_matrices_computer.pinned_animation	= cubic_spline_skeleton_animation_pinned( 0 );
}

float3 mix_translations( buffer_vector< std::pair< float3, float > > const& transforms )
{
	float3	result( 0.f, 0.f, 0.f );
	for ( std::pair< float3, float > const* i = transforms.begin(), * const e = transforms.end(); i != e; ++i )
		result	+= i->first * i->second;

	return	result;
}

// STATE[STUB]
// claude@NOTE: ~0x1f9-byte weighted quaternion blend (axis-angle log/exp accumulation with
// total_weight + optional final normalization). Large FP reconstruction; reachable once
// computed_local_bone_transform / get_object_transform are bodied (its only callers) - parked.
math::quaternion mix_rotations(
	buffer_vector< std::pair< float3, float > >&	arg_0 /* vostok::buffer_vector< std::pair< float3, float > >& transforms */,
	const bool		do_normalization
)
{
	// LOCALS
	// float 							angle
	// float3 							direction
	// vostok::math::quaternion 		mix
	// float 							angle
	// const float 						total_weight
	// float3 							direction
	// ******

	// TYPEDEFS
	// typedef
	// 	vostok::buffer_vector< std::pair< vostok::math::quaternion, float > >
	// 	rotations_type;

	// ******

	return vostok::math::quaternion();

	// FUNCTION BODY
	// <0x6eca10>|0x000|+0x008:'200'	{
	// <0>
	// <1>
	// <0x6eca18>|0x008|+0x00d:'203'
	// <0x6eca25>|0x015|+0x03f:'204'
	// <0>
	// <1>
	// <0x6eca64>|0x054|+0x01a:'207'
	// <0>
	// <1>
	// <0x6eca7e>|0x06e|+0x004:'210'
	// <0x6eca82>|0x072|+0x028:'211'
	// <0x6ecaaa>|0x09a|+0x04b:'212'
	// <0>
	// <0x6ecaf5>|0x0e5|+0x002:'214'
	// <0x6ecaf7>|0x0e7|+0x006:'215'
	// <0>
	// <0x6ecafd>|0x0ed|+0x01a:'217'
	// <0x6ecb17>|0x107|+0x004:'218'
	// <0x6ecb1b>|0x10b|+0x021:'219'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6ecb3c>|0x12c|+0x01e:'224'
	// <0x6ecb5a>|0x14a|+0x02a:'225'
	// <0>
	// <1>
	// <0x6ecb84>|0x174|+0x005:'228'
	// <0x6ecb89>|0x179|+0x00e:'229'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6ecb97>|0x187|+0x01d:'235'
	// <0>
	// <0x6ecbb4>|0x1a4|+0x006:'237'
	// <0x6ecbba>|0x1aa|+0x020:'238'
	// <0>
	// <1>
	// <2>
	// <0x6ecbda>|0x1ca|+0x00f:'242'
	// <0x6ecbe9>|0x1d9|-0x071:'243'
	// <0>
	// <1>
	// <0x6ecb78>|0x168|+0x080:'246'
	// <0x6ecbf8>|0x1e8|-0x19e:'246'
	// <0x6eca5a>|0x04a|+0x0d8:'247'
	// <0x6ecb32>|0x122|+0x048:'247'
	// <0x6ecb7a>|0x16a|+0x056:'247'
	// <0x6ecbd0>|0x1c0|+0x039:'247'
	// <0x6ecc09>|0x1f9|      :'247'	}
	// ******
}

float3 mix_scales( buffer_vector< std::pair< float3, float > > const& transforms )
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

// STATE[STUB]
// claude@NOTE: 27-stmt 0x41c-byte per-bone mixer: allocates rotations/scales buffer_vectors,
// loops m_animations sampling each bone_frame at frame_position for animation_layer_id,
// accumulates weighted (translation,weight)/(rotation,weight)/(scale,weight), then returns a
// bone_transform from mix_translations/mix_rotations/mix_scales. Large; parked.
bone_transform bone_matrices_computer::computed_local_bone_transform( skeleton_bone const& bone, const u32 bone_mask, const u32 animation_layer_id ) const
{
	// LOCALS
	// buffer_vector< std::pair< float3, float > > rotations
	// buffer_vector< std::pair< float3, float > > scales
	// mixing::animation_state* 		e
	// current_frame_position 			frame_position
	// ******

	// FUNCTION BODY
	// <0x6ed2b9>|0x009|+0x017:'275'
	// <0x6ed2d0>|0x020|+0x012:'276'
	// <0x6ed2e2>|0x032|+0x007:'277'
	// <0>
	// <0x6ed2e9>|0x039|+0x027:'279'
	// <0x6ed310>|0x060|+0x006:'280'
	// <0x6ed316>|0x066|+0x00b:'281'
	// <0>
	// <1>
	// <0x6ed321>|0x071|+0x012:'284'
	// <0>
	// <1>
	// <0x6ed333>|0x083|+0x00c:'287'
	// <0>
	// <1>
	// <0x6ed33f>|0x08f|+0x00c:'290'
	// <0>
	// <1>
	// <2>
	// <0x6ed34b>|0x09b|+0x0e0:'294'
	// <0>
	// <1>
	// <0x6ed42b>|0x17b|+0x057:'297'
	// <0x6ed482>|0x1d2|+0x041:'298'
	// <0x6ed4c3>|0x213|+0x061:'299'
	// <0>
	// <1>
	// <0x6ed524>|0x274|+0x003:'302'
	// <0>
	// <0x6ed527>|0x277|+0x010:'304'
	// <0>
	// <0x6ed537>|0x287|+0x009:'306'
	// <0x6ed540>|0x290|+0x013:'307'
	// <0>
	// <1>
	// <2>
	// <0x6ed553>|0x2a3|+0x02e:'311'
	// <0>
	// <1>
	// <0x6ed581>|0x2d1|+0x00f:'314'
	// <0x6ed590>|0x2e0|+0x015:'315'
	// <0>
	// <0x6ed5a5>|0x2f5|+0x003:'317'
	// <0x6ed5a8>|0x2f8|+0x008:'318'
	// <0x6ed5b0>|0x300|+0x013:'319'
	// <0>
	// <1>
	// <2>
	// <0x6ed5c3>|0x313|+0x029:'323'
	// <0x6ed5ec>|0x33c|+0x004:'324'
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
	// <0x6ed5f0>|0x340|+0x0cb:'348'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6ed6bb>|0x40b|+0x002:'353'
	// ******
}

// STATE[STUB]
// claude@NOTE: builds a buffer_vector<bone_transform>(bone.mask()) via
// computed_local_bone_transform per layer, reduces with bone_transform::apply (translation+=,
// rotation*=, scale*=), then returns mul4x3( create_scale(scale)*quat_to_matrix(rotation),
// create_translation(translation) ). Load-bearing for compute_skeleton_branch[_local] but they
// pair via the real call even while this is a stub; matrix reduction reconstruction parked.
float4x4 bone_matrices_computer::computed_local_bone_matrix( skeleton_bone const& bone, const u32 bone_mask ) const
{
	// LOCALS
	// bone_transform 					result
	// ******

	// TYPEDEFS
	// typedef
	// 	buffer_vector< bone_transform >
	// 	bone_transforms_type;

	// ******

	return vostok::math::float4x4();

	// FUNCTION BODY
	// <0>
	// <0x6ed6dd>|0x00d|+0x00f:'359'
	// <0x6ed6ec>|0x01c|+0x062:'360'
	// <0x6ed74e>|0x07e|-0x056:'360'
	// <0x6ed6f8>|0x028|+0x059:'361'
	// <0>
	// <1>
	// <0x6ed751>|0x081|+0x042:'364'
	// <0x6ed793>|0x0c3|-0x013:'364'
	// <0x6ed780>|0x0b0|+0x00b:'365'
	// <0>
	// <0x6ed78b>|0x0bb|+0x00b:'367'
	// <0x6ed796>|0x0c6|+0x16c:'368'
	// <0>
	// <0x6ed902>|0x232|+0x1cc:'370'
	// <0>
	// <0x6edace>|0x3fe|+0x002:'372'
	// ******
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

	skeleton_bone const* const e	= bone.children_end();
	for ( skeleton_bone const* i = bone.children_begin(); i != e; ++i )
		convert_skeleton_branch( *i, result + ( i - &bone ), *result );
}

void bone_matrices_computer::compute_bones_matrices( float4x4* const begin, float4x4* const end, u32 const* const bones_masks ) const
{
	skeleton_bone const* const roots_begin	= &m_skeleton->get_root();
	skeleton_bone const* const roots_end	= roots_begin->children_begin();
	for ( skeleton_bone const* i = roots_begin; i != roots_end; ++i ) {
		skeleton_bone const* const children_end	= i->children_end();
		for ( skeleton_bone const* j = i->children_begin(); j != children_end; ++j )
			compute_skeleton_branch(
				*j,
				begin + ( j - i->children_begin() ),
				float4x4( ).identity( ),
				bones_masks ? bones_masks + ( j - roots_begin ) : 0,
				bones_masks ? bones_masks + ( j - i->children_begin() ) : 0
			);
	}
}

void bone_matrices_computer::compute_bones_local_matrices( float4x4* const begin, float4x4* const end, u32 const* const bones_masks ) const
{
	skeleton_bone const* const roots_begin	= &m_skeleton->get_root();
	skeleton_bone const* const roots_end	= roots_begin->children_begin();
	for ( skeleton_bone const* i = roots_begin; i != roots_end; ++i ) {
		skeleton_bone const* const children_end	= i->children_end();
		for ( skeleton_bone const* j = i->children_begin(); j != children_end; ++j )
			compute_skeleton_branch_local(
				*j,
				begin + ( j - i->children_begin() ),
				bones_masks ? bones_masks + ( j - roots_begin ) : 0,
				bones_masks ? bones_masks + ( j - i->children_begin() ) : 0
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

// STATE[STUB]
// claude@NOTE: 19-stmt 0x689-byte object-movement mixer: allocates translations/rotations/scales
// buffer_vectors, loops m_animations accumulating each weighted accumulated_object_movement, then
// composes create_scale*quat_to_matrix*create_translation from mix_* results. Large; parked.
float4x4 bone_matrices_computer::get_object_transform( ) const
{
	// LOCALS
	// buffer_vector< std::pair< float3, float > > rotations
	// buffer_vector< std::pair< float3, float > > translations
	// buffer_vector< std::pair< float3, float > > scales
	// mixing::animation_state* 		e
	// mixing::animation_state* 		i
	// current_frame_position 			frame_position
	// ******

	return vostok::math::float4x4();

	// FUNCTION BODY
	// <0x6ecc29>|0x009|+0x05a:'492'
	// <0x6ecc83>|0x063|-0x043:'492'
	// <0x6ecc40>|0x020|+0x015:'493'
	// <0x6ecc55>|0x035|+0x007:'494'
	// <0>
	// <0x6ecc5c>|0x03c|+0x02a:'496'
	// <0x6ecc86>|0x066|+0x011:'497'
	// <0>
	// <1>
	// <0x6ecc97>|0x077|+0x010:'500'
	// <0>
	// <1>
	// <0x6ecca7>|0x087|+0x00d:'503'
	// <0>
	// <1>
	// <0x6eccb4>|0x094|+0x009:'506'
	// <0x6eccbd>|0x09d|+0x01d:'507'
	// <0x6eccda>|0x0ba|+0x05d:'508'
	// <0x6ecd37>|0x117|+0x02a:'509'
	// <0>
	// <0x6ecd61>|0x141|+0x005:'511'
	// <0>
	// <0x6ecd66>|0x146|+0x070:'513'
	// <0>
	// <1>
	// <2>
	// <0x6ecdd6>|0x1b6|+0x00b:'517'
	// <0x6ecde1>|0x1c1|+0x053:'518'
	// <0x6ece34>|0x214|+0x144:'519'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x6ecf78>|0x358|+0x1e6:'529'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6ed15e>|0x53e|+0x13c:'536'
	// ******
}

} // namespace animation
} // namespace vostok
