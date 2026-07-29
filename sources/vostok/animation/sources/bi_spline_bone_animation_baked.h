////////////////////////////////////////////////////////////////////////////
//	Created 	: 25.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef ANIMATION_BI_SPLINE_BONE_ANIMATION_BAKED_H_INCLUDED
#define ANIMATION_BI_SPLINE_BONE_ANIMATION_BAKED_H_INCLUDED

#include <vostok/platform_pointer.h>
#include <vostok/animation/anim_track_common.h>
#include "bi_spline_channel_animation_baked.h"

namespace vostok {
namespace animation {

class bi_spline_bone_animation_baked {
public:
	inline	bi_spline_channel_animation_baked const&	channel_animation				( enum_channel_id const channel ) const
	{
		return *m_channel_animations[channel];
	}

	inline		bi_spline_bone_animation_baked	( ) { }

private:
	/* 0x0000 */	platform_pointer_selector<
						bi_spline_channel_animation_baked,
						platform_pointer_64bit
					>::helper	m_channel_animations[9];

	friend void create_baked_animation_in_place( void* const raw_buffer, const u32 buffer_size );
}; // class bi_spline_bone_animation_baked

STATIC_SIZE_ASSERT(bi_spline_bone_animation_baked, 0x48);

} // namespace animation
} // namespace vostok

#endif // #ifndef ANIMATION_BI_SPLINE_BONE_ANIMATION_BAKED_H_INCLUDED
