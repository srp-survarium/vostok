////////////////////////////////////////////////////////////////////////////
//	Created		: 19.01.2010
//	Author		: Konstantin Slipchenko
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/animation/bone_animation.h>
#include "bi_spline_bone_animation_baked.h"

using namespace vostok::math;

namespace vostok {
namespace animation {
u32 bone_animation::count_internal_memory_size( bi_spline_bone_animation_baked const& data )
{
	u32 size = 0;

	for ( u32 channel = channel_translation_x; channel < channel_max; ++channel )
		size += animation_curve_type::count_internal_memory_size(
			data.channel_animation( enum_channel_id( channel ) )
		);

	return size;
}

void bone_animation::create_internals_in_place(
	bi_spline_bone_animation_baked const& data,
	void* memory
)
{
	for ( u32 channel = channel_translation_x; channel < channel_max; ++channel ) {
		bi_spline_channel_animation_baked const& spline =
			data.channel_animation( enum_channel_id( channel ) );

		this->channel( enum_channel_id( channel ) ).create_in_place_internals(
			spline,
			memory
		);

		memory = get_shift_ptr(
			memory,
			animation_curve_type::count_internal_memory_size( spline )
		);
	}
}

frame	bone_animation::bone_frame	( float const time, current_frame_position &frame_position ) const
{
	frame					result;
	evaluate_frame			( time, m_channels, result, frame_position );
	return					result;
}

void	bone_animation::bone_matrix( float time, const float3 &parent_scale_, float4x4 &matrix, float3 &scale_, current_frame_position &frame_pos )const
{
	frame f;
	evaluate_frame( time, m_channels, f, frame_pos );
	frame_matrix( f, matrix, parent_scale_, scale_ );
}

} // namespace animation
} // namespace vostok
