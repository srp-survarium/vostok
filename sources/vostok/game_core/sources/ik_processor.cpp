////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/ik_processor.h>

#include <vostok/animation/api.h>
#include <vostok/animation/skeleton.h>

namespace survarium {

// STATE[STUB]
// survarium::ik_processor::ik_processor()
ik_processor::ik_processor( )
{
	// FUNCTION BODY
	// <0x58eba0>|0x000|+0x022:'16'	{
	// <0x58ebc2>|0x022|      :'17'	}
	// ******
}

// STATE[STUB]
// void survarium::ik_processor::activate(vostok::animation::skeleton const&)
void ik_processor::activate( animation::skeleton const& skeleton )
{
	// FUNCTION BODY
	// <0x58eb87>|0x007|+0x008:'21'
	// ******
}

// STATE[100%|DONE]: full match
// vostok::math::float4x4 survarium::get_bone_matrix_in_object_space_impl(vostok::animation::skeleton_bone const&, vostok::math::float4x4 const*, vostok::animation::skeleton_bone const*)
// claude@MATCH: matrix_index is a pointer subtraction; the recursion walks up the
// parent chain. The `: float4x4( ).identity( )` term is the parent==NULL leaf
// (default ctor + identity); operator*'s left operand is matrices[matrix_index]
// (push order in target asm), right operand is the recursive result.
float4x4 get_bone_matrix_in_object_space_impl( animation::skeleton_bone const& bone, float4x4 const* matrices, animation::skeleton_bone const* first_non_root_bone )
{
	u32 const matrix_index	= u32( &bone - first_non_root_bone );
	return bone.parent( ) ?
		matrices[matrix_index] * get_bone_matrix_in_object_space_impl( *bone.parent( ), matrices, first_non_root_bone ) :
		float4x4( ).identity( );
}

// STATE[100%|DONE]: full match
// vostok::math::float4x4 survarium::get_bone_matrix_in_object_space(vostok::animation::skeleton_bone const&, vostok::animation::skeleton const&, vostok::math::float4x4 const*)
float4x4 get_bone_matrix_in_object_space( animation::skeleton_bone const& bone, animation::skeleton const& skeleton, float4x4 const* matrices )
{
	animation::skeleton_bone const* const first_non_root_bone	= &skeleton.get_root( ) + skeleton.get_root_bones_count( );
	return get_bone_matrix_in_object_space_impl( bone, matrices, first_non_root_bone );
}

} // namespace survarium
