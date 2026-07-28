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

// claude@NOTE: 7-stmt STRUCTURE MATCH (capped ~30%). Residual is inlining-environment only:
// target keeps a real `call vostok::math::max` + register-promotes m_layers_count to ebx
// (single write-back + ++), base inlines max branchless and read/writes the member each pass;
// target inlines the intrusive_ptr ref-count dance into the pinned_ptr assignment while base
// keeps pinned_ptr_const::operator=/dtor calls. Same statements, different inline decomposition.
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

float3 mix_translations( buffer_vector< std::pair< float3, float > > const& transforms )
{
	float3	result( 0.f, 0.f, 0.f );
	for ( std::pair< float3, float > const* i = transforms.begin(), * const e = transforms.end(); i != e; ++i )
		result	+= i->first * i->second;

	return	result;
}

// claude@NOTE: BODIED (faithful reconstruction of the 26-stmt target at va 0x6eca10) but currently
// DCE-stripped: its only callers (computed_local_bone_transform / get_object_transform below) are
// still STUBs, and those in turn are blocked on two missing engine symbols - the 4-arg
// evaluate_frame( float, animation_curve_type const*, frame&, current_frame_position& ) and
// bone_names::bone_index( char const* ) const (CRC binary-search over a bone_name_index /
// crc_compare_predicate our bone_names.h does not yet model). Pairs the moment that chain is bodied
// (same situation as the n_ary_tree forwarders). Callee math::extrapolated_slerp added to
// math_quaternion.{h,cpp}.
math::quaternion mix_rotations(
		buffer_vector< std::pair< float3, float > >&	transforms,
		const bool										do_normalization
	)
{
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
// claude@NOTE: 27-stmt 0x41c per-bone mixer (target 0x6ed2b0, lines 275-353). STRUCTURE FULLY
// DECODED below; kept a returning STUB (not bodied) because the inner frame-evaluation needs two
// engine symbols our source does NOT yet provide:
//   (1) void evaluate_frame( float, animation_curve_type const*, frame&, current_frame_position& )
//       - a 4-arg overload (target rva 0x10E0E0, lines 22-23 of bone_animation_inline.h). The
//         existing inline is only the 3-arg form; the 4-arg one threads current_frame_position via
//         time_channel::domain(t, current_domain) + an inline poly_curve Horner eval. Reconstructable.
//   (2) unsigned bone_names::bone_index( char const* ) const (target rva 0x559FB0) - a CRC binary
//       search (bone_name_index + crc_compare_predicate + __lower_bound). Our bone_names.h models
//       names as vector<fixed_string<64>> with only bone_index(string_type const&) - the
//       bone_name_index/crc infrastructure is ABSENT. This is the HARD blocker; do NOT fabricate it.
// Both are also needed by n_ary_tree::set_object_transform/get_object_transform. Decoded shape (the
// faithful body to paste once (1)+(2) exist):
//   buffer_vector<pair<float3,float>> translations(ALLOCA,m_animations_count); // 275 (unnamed-in-PDB; mix_translations inlines @348)
//   buffer_vector<pair<float3,float>> rotations   (ALLOCA,m_animations_count); // 276
//   buffer_vector<pair<float3,float>> scales      (ALLOCA,m_animations_count); // 277
//   for ( animation_state* i=m_animations, * const e=m_animations+m_animations_count; i!=e; ++i ) { // 279
//       n_ary_tree_animation_node& node = i->event_iterator.animation();        // 280 [edi+0xAC]
//       if ( node.animated_object() != m_animated_object ) continue;            // 281 [ecx+0x24] vs [this]
//       if ( i->weight == 0.f ) continue;                                       // 284 [edi+0x68]
//       if ( node.additivity_priority() != animation_layer_id ) continue;       // 287 [ecx+0x48] vs [ebp+0x18]
//       if ( !( node.bones_mask() & bone_mask ) ) continue;                      // 290 [ecx+0x4C] & [ebp+0x14]
//       frame f; current_frame_position frame_position;                          // 294: bone_index over pinned bone_names
//       u32 idx = i->bone_matrices_computer.pinned_animation->bone_names().bone_index( bone.id() );
//       if ( idx == u32(-1) ) f = identity_frame();
//       else evaluate_frame( i->animation_time*default_fps, pinned->bone(idx).channels, f, frame_position );
//       translations.push_back( make_pair(f.translation, i->weight) );           // 297
//       rotations.push_back   ( make_pair(f.rotation,    i->weight) );           // 298
//       scales.push_back      ( make_pair(f.scale,       i->weight) ); }         // 299
//   if ( animation_layer_id == 0 ) {                                             // 302 guard ([ebp+0x18] test; jne .15)
//       float total = sum( rotations[k].second );                               // 304/306/307
//       if ( math::abs( total - 1.f ) > epsilon_5 ) {                           // 311 (clear_value=1.f)
//           for k: rotations[k].second *= 1.f/total;                            // 314/315 normalize
//           float total2 = sum( rotations[k].second );                         // 317/318/319
//           if ( !m_overweighting_detected && math::abs(1.f - total2) >= 0.5f )  // 323 ([half])
//               m_overweighting_detected = true; } }                            // 324 store [edx+0x14]=1
//   return bone_transform( mix_translations(translations),                       // 348 (mix_scales+mix_rotations called)
//       mix_rotations(rotations, animation_layer_id < 2), mix_scales(scales), true ); // 353 (visibility byte [ecx+0x28]=1)
// sushi@TODO: blocked on bone_names::bone_index(char const*) - a CRC index (bone_name_index /
// crc_compare_predicate) absent from our bone_names.h. Returning stub until that is recovered.
bone_transform bone_matrices_computer::computed_local_bone_transform( skeleton_bone const& bone, const u32 bone_mask, const u32 animation_layer_id ) const
{
	return vostok::animation::bone_transform( );
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

	return					mul4x3( mul4x3( create_scale( result.scale ), create_rotation( result.rotation ) ), create_translation( result.translation ) );
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
// claude@NOTE: 19-stmt 0x689 object-movement mixer (target 0x6ecc20, lines 492-536). Structure
// decoded; STUB kept because the per-animation movement extraction (513-519) is dense FP that I
// could not pin without risk of fabrication. The two callee blockers are now CLEARED:
//   - the 4-arg evaluate_frame is added to bone_animation_inline.h (used by the FALSE branch @519);
//   - create_matrix / create_rotation / get_angles_xyz / conjugate / mul4x3 are all in math headers.
// What remains is purely the FP branch decode (no missing symbols). Named locals (PDB, 6, in order):
// rotations, translations, scales, e (animation_state*), i (animation_state*), frame_position
// (THREE buffers ALL named here, unlike computed_local_bone_transform). ebx = i+0x34 so:
//   accumulated_object_movement (state +0x28): rotation=ebx-0xC, translation=ebx+0x4, scale=ebx+0x10
//   previous_object_movement    (state +0x00): rotation=ebx-0x34, translation=ebx-0x24, scale=ebx-0x18
//   weight=[ebx+0x34]=i+0x68, node=[ebx+0x78]=i->event_iterator.animation(), pinned=[ebx+0x20]=i->bmc.pinned_animation
// Shape:
//   buffer_vector<pair<float3,float>> rotations   ( ALLOCA, m_animations_count );  // 492
//   buffer_vector<pair<float3,float>> translations( ALLOCA, m_animations_count );  // 493
//   buffer_vector<pair<float3,float>> scales      ( ALLOCA, m_animations_count );  // 494
//   for ( animation_state* i=m_animations, * const e=m_animations+m_animations_count; i!=e; ++i ) { // 496
//       n_ary_tree_animation_node& node = i->event_iterator.animation();
//       if ( node.animated_object() != m_animated_object ) continue;               // 497
//       if ( pinned->animation_type() != 0 ) continue;                             // 500 cubic_spline_skeleton_animation::animation_type
//       if ( !node.can_generate_events() ) continue;                               // 503 ([node+0x4C]&1)
//       if ( i->event_iterator.are_there_any_weight_transitions() ) {              // 506 ([i+0x74])
//           // TRUE branch (507-509): push accumulated_object_movement components straight (scale/translation/rotation)
//       } else {                                                                   // FALSE branch @511
//           // 513-519: evaluate_frame(...) + bone_transform ctor + create_matrix(rot,float3(0)) + get_angles_xyz + conjugate
//           // to derive the per-frame movement, then push translation/rotation/scale paired with i->weight
//       } }
//   return <compose: create_scale(mix_scales(scales)) * create_rotation(mix_rotations(rotations,true)) // 529
//          * create_translation(mix_translations(translations))>;                  // 536
float4x4 bone_matrices_computer::get_object_transform( ) const
{
	return vostok::math::float4x4();
}

} // namespace animation
} // namespace vostok
