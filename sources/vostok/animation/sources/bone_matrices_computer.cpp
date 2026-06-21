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

// STATE[STUB]
// claude@NOTE: ~0x1f9-byte weighted quaternion blend. STRUCTURE FULLY RECOVERED from target
// asm (0x6eca10), 26 stmts, lines 200-247; parked ONLY because both its callers
// (computed_local_bone_transform / get_object_transform) are still stubs so it DCE-strips and
// can't be scored. Ready-to-paste body (verify once a caller is bodied):
//   typedef buffer_vector< std::pair< quaternion, float > > rotations_type;
//   rotations_type rotations( ALLOCA( sizeof(rotations_type::value_type) * transforms.size() ), transforms.size() );
//   for ( pair<float3,float> const* i=transforms.begin(), * const e=transforms.end(); i!=e; ++i ) {
//       if ( math::abs( i->second ) < epsilon_5 ) continue;              // line 211: comiss epsilon,abs; ja skip
//       rotations.push_back( std::make_pair( quaternion( i->first ), i->second ) ); }   // 212
//   if ( rotations.empty() ) return quaternion( float4(0,0,0,1) );      // 214/215 -> .1 fills 0,0,0,clear_value
//   if ( rotations.size() == 1 ) {                                       // 217: count==1
//       if ( do_normalization ) return rotations.front().first;          // 218/219 (je .6: false->axis/angle)
//       float angle; float3 direction;
//       rotations.front().first.get_axis_and_angle( direction, angle );  // 224
//       return quaternion( direction, angle * rotations.front().second );} // 225 (.7 shared ctor tail)
//   if ( rotations.size() == 2 ) {                                       // 228: count==2
//       const float total_weight = rotations.front().second + rotations.back().second;          // 229
//       quaternion mix = slerp_optimized( rotations.front().first, rotations.back().first,
//                                         rotations.back().second / total_weight );              // 235
//       if ( do_normalization ) return mix;                              // 237/238
//       float angle; float3 direction; mix.get_axis_and_angle( direction, angle );               // 242
//       return quaternion( direction, angle * total_weight ); }          // 243 (jmp .7)
//   return extrapolated_slerp( rotations.begin(), rotations.end() );     // 246
// CROSS-UNIT DEP: extrapolated_slerp( pair<quaternion,float> const*, const* ) returns quaternion,
//   GLOBAL namespace, DEFINED (still a STUB) in core/sources/math_quaternion.cpp; NOT declared in
//   any header -> needs a file-scope forward decl here when un-stubbed.
math::quaternion mix_rotations(
	buffer_vector< std::pair< float3, float > >&	arg_0 /* vostok::buffer_vector< std::pair< float3, float > >& transforms */,
	const bool		do_normalization
)
{
	return vostok::math::quaternion();
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
// claude@NOTE: 27-stmt 0x41c per-bone mixer (target 0x6ed2b0, lines 275-353). STRUCTURE DECODED;
// parked - big FP reconstruction + a local-set puzzle that needs a build iteration to settle.
// Named locals (PDB, 4): rotations, scales, e (animation_state*), frame_position. NOTE only
// rotations + scales are buffers although the asm allocas THREE buffer_vectors and inline-mixes
// the third (translations) via the .16 loop at line 348 -> the translations buffer is UNNAMED
// (its mix_translations got inlined). Match the recorded 4-local set, do NOT add a named
// translations local.
// Shape:
//   buffer_vector<pair<float3,float>> rotations( ALLOCA, m_animations_count );   // 275
//   buffer_vector<pair<float3,float>> scales   ( ALLOCA, m_animations_count );   // 276 (+ inline translations buf)
//   for ( animation_state* i=m_animations, * const e=m_animations+m_animations_count; i!=e; ++i ) {  // 277/279
//       n_ary_tree_animation_node& node = i->event_iterator.animation();         // 280  ([edi+0xAC])
//       if ( i->weight == 0.f ) continue;                                        // 281  ([edi+0x68])
//       if ( node.additivity_priority() != animation_layer_id ) continue;        // 284  ([ecx+0x48] vs [ebp+0x18])
//       if ( !( node.bones_mask() & bone_mask ) ) continue;                      // 287  ([ecx+0x4C] & [ebp+0x14])
//       <evaluate frame>: idx = bone_names::bone_index( pinned_anim->bone_names, bone.id() );  // 290
//         if idx==-1 -> identity_frame(); else evaluate_frame( i->animation_time*default_fps,  // ([edi+0x6C])
//                       pinned_anim->bone(idx).channels, frame, frame_position );
//       translations.push_back( make_pair( frame.translation, weight ) );        // 297
//       rotations.push_back   ( make_pair( frame.rotation,    weight ) );        // 298
//       scales.push_back      ( make_pair( frame.scale,       weight ) ); }      // 299
//   <overweighting>: total = sum(rotations weights); if ( |total-1| > epsilon ) normalize-rotations // 302-311
//     then re-sum & if ( !m_overweighting_detected && |1-total| >= 0.5f ) m_overweighting_detected=true; // 314-324
//   return bone_transform( mix_translations(translations), mix_rotations(rotations,!m_overweighting_detected),  // 348
//                          mix_scales(scales) );                                  // visibility=true ([ecx+28h]=1) // 353
// (evaluate_frame is in bone_animation_inline.h; bone_names::bone_index, frame layout
//  translation/rotation/scale 0xC each + visibility @ anim_track_common.h.)
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
// claude@NOTE: 11-stmt 0x40f bone_transform->matrix reduction (target 0x6ed6d0, lines 359-372).
// STRUCTURE DECODED; parked - big inline FP (the create_matrix(quaternion,float3) expansion at
// line 370 is the 0x1cc-byte xmm block 0x232-0x3d7, ending in mul4x3+create_translation+mul4x3).
// Only named local (PDB): `result` (bone_transform). The bone_transforms buffer is UNNAMED.
// Shape:
//   buffer_vector<bone_transform> bone_transforms( ALLOCA(sizeof(bone_transform)*m_layers_count), m_layers_count ); // 359
//   for ( u32 i=0; i<m_layers_count; ++i )                                              // 360  (loop bound [this+0x10])
//       bone_transforms.push_back( computed_local_bone_transform( bone, bone_mask, i ) );// 361
//   bone_transform result = bone_transforms.front();                                    // 364/365 (copy first elem)
//   for ( bone_transform const* i=bone_transforms.begin()+1; i!=bone_transforms.end(); ++i ) // 367
//       result.apply( *i );                                                             // 368 (transl+=, rot*=, scale*=, vis&=)
//   return mul4x3( create_matrix( result.rotation, result.scale-as-scale ) ... ,        // 370 - actually:
//                  create_translation( result.translation ) );
//   => the 0x232 block builds create_matrix(result.rotation,float3(0)) scaled by result.scale,
//      via mul4x3( <quat-to-3x3 * scale>, create_translation(result.translation) ); confirm exact
//      create_scale/create_matrix nesting against math_float4x4_inline.h on the matching pass.
// ABI note: this thiscall returns float4x4 by value; [ebp+8] is reused as the buffer base after
// ebx=this is read; [ebp+0Ch]=sret. m_layers_count drives the layer loop (NOT bone.mask()).
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

	skeleton_bone const* i			= bone.children_begin();
	skeleton_bone const* const e	= bone.children_end();
	for ( ; i != e; ++i )
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
// claude@NOTE: 19-stmt 0x689 object-movement mixer (target 0x6ecc20, lines 492-536). STRUCTURE
// DECODED; parked - big FP (create_matrix/get_angles_xyz per anim @519 0x144, the final compose
// @529 0x1e6). Named locals (PDB, 6, in this order): rotations, translations, scales,
// e (animation_state*), i (animation_state*), frame_position. THREE buffers ALL named here
// (contrast computed_local_bone_transform where translations is unnamed).
// Shape:
//   buffer_vector<pair<float3,float>> rotations   ( ALLOCA, m_animations_count );  // 492 ([this+0x0C])
//   buffer_vector<pair<float3,float>> translations( ALLOCA, m_animations_count );  // 493
//   buffer_vector<pair<float3,float>> scales      ( ALLOCA, m_animations_count );  // 494
//   for ( animation_state* i=m_animations, * const e=m_animations+m_animations_count; i!=e; ++i ) { // 496
//       node = i->event_iterator.animation();                                      // ([i+0x78+...]=[ebx+0x78])
//       if ( node.animated_object() != m_animated_object ) continue;               // 497 ([edx+0x24] vs [this])
//       if ( pinned->animation_type() != 0 ) continue;                             // 500 (cubic_spline_skeleton_animation::animation_type, [ebx+0x20]=this)
//       if ( !( node.can_generate_events() ) ) continue;                           // 503 ([edx+0x4C]&1)  -- confirm accessor
//       if ( i->event_iterator.are_there_any_weight_transitions() ) {              // 506 ([ebx+0x40])
//           <use previous_object_movement>; translations/rotations/scales.push_back(...); }  // 507-509
//       else { <use accumulated_object_movement w/ create_matrix(rot, float3(0)) + get_angles_xyz>;  // 511-518
//           translations/rotations/scales.push_back( make_pair(component, i->weight) ); } }
//   return mul4x3/compose from mix_translations(translations) / mix_rotations(rotations,true) /
//          mix_scales(scales): create_scale * quat_to_matrix * create_translation.            // 529/536
// (object_movement layout @mixing_animation_state.h: rotation 0x10, translation 0x10, scale; the
//  bone_matrices_computer_data accumulated/previous_object_movement offsets drive [ebx+0x10..0x40].)
// Helpers: math::create_matrix(quaternion,float3) + float4x4::get_angles_xyz @math_float4x4*.h.
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
