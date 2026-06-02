////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/ik_processor.h>

#include <vostok/animation/api.h>
#include <vostok/animation/skeleton.h>

namespace survarium {

// STATE[INPROGRESS]: written, NOT diffed - /nix store was 100% full, rebuild blocked.
// Next: free /nix and `python3 scripts/rebuild.py`, then score from report.json.
// claude@MATCH: the target's pre-store `call ...finalize_impl` (rva 0x03f210) is the
// COMDAT-folded core::noncopyable base ctor (noncopyable::noncopyable is ALSO 0x03f210),
// emitted out-of-line because noncopyable declares its own ctor - NOT an ASSERT (an
// ASSERT would emit `mov byte[ebp-N],0; lea eax; call`, which the target lacks).
// survarium::ik_processor::ik_processor()
ik_processor::ik_processor( )
	: m_skeleton		( NULL )
	, m_last_time_in_ms	( 0 )
{
	// FUNCTION BODY
	// <0x58eba0>|0x000|+0x022:'16'	{	: m_skeleton(0)/m_last_time_in_ms(0) (after base ctor)
	// <0x58ebc2>|0x022|      :'17'	}
	// ******
}

// STATE[INPROGRESS]: written, NOT diffed - /nix store was 100% full, rebuild blocked.
// Next: free /nix and `python3 scripts/rebuild.py`, then score from report.json.
void ik_processor::activate( animation::skeleton const& skeleton )
{
	m_skeleton = &skeleton;

	// FUNCTION BODY
	// <0x58eb87>|0x007|+0x008:'21'	m_skeleton = &skeleton;
	// ******
}

// STATE[100%|DONE]
float4x4 get_bone_matrix_in_object_space_impl( animation::skeleton_bone const& bone, float4x4 const* matrices, animation::skeleton_bone const* first_non_root_bone )
{
	u32 const matrix_index	= u32( &bone - first_non_root_bone );
	return bone.parent( ) ?
		matrices[matrix_index] * get_bone_matrix_in_object_space_impl( *bone.parent( ), matrices, first_non_root_bone ) :
		float4x4( ).identity( );
}

// STATE[100%|DONE]
float4x4 get_bone_matrix_in_object_space( animation::skeleton_bone const& bone, animation::skeleton const& skeleton, float4x4 const* matrices )
{
	animation::skeleton_bone const* const first_non_root_bone	= &skeleton.get_root( ) + skeleton.get_root_bones_count( );
	return get_bone_matrix_in_object_space_impl( bone, matrices, first_non_root_bone );
}

} // namespace survarium
