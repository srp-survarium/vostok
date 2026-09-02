// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/ik_processor.h>
#include <vostok/game_core/ik_utils.h>		// get_bone_matrix_in_object_space decl (VOSTOK_GAME_CORE_API)

#include <vostok/animation/api.h>
#include <vostok/animation/skeleton.h>

namespace survarium {

ik_processor::ik_processor( )
	: m_skeleton		( NULL )
	, m_last_time_in_ms	( 0 )
{
}

void ik_processor::activate( animation::skeleton const& skeleton )
{
	m_skeleton = &skeleton;
}

float4x4 get_bone_matrix_in_object_space_impl( animation::skeleton_bone const& bone, float4x4 const* matrices, animation::skeleton_bone const* first_non_root_bone )
{
	u32 const matrix_index	= u32( &bone - first_non_root_bone );
	return bone.parent( ) ?
		matrices[matrix_index] * get_bone_matrix_in_object_space_impl( *bone.parent( ), matrices, first_non_root_bone ) :
		float4x4( ).identity( );
}

float4x4 get_bone_matrix_in_object_space( animation::skeleton_bone const& bone, animation::skeleton const& skeleton, float4x4 const* matrices )
{
	animation::skeleton_bone const* const first_non_root_bone	= &skeleton.get_root( ) + skeleton.get_root_bones_count( );
	return get_bone_matrix_in_object_space_impl( bone, matrices, first_non_root_bone );
}

} // namespace survarium
