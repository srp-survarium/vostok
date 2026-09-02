// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 25.12.2025
////////////////////////////////////////////////////////////////////////////
#ifndef ANIMATION_BI_SPLINE_SKELETON_ANIMATION_BAKED_H_INCLUDED
#define ANIMATION_BI_SPLINE_SKELETON_ANIMATION_BAKED_H_INCLUDED

#include <vostok/animation/type_definitions.h>
#include "bi_spline_bone_animation_baked.h"
#include "bi_spline_event_channel_baked.h"
#include "bi_spline_skeleton_animation_baked_impl.h"

namespace vostok {
namespace animation {

class bi_spline_skeleton_animation_baked : public resources::unmanaged_resource {
public:
	inline	configs::binary_config_ptr						bones_names							( ) const { return m_bones_names; }
	inline	animation_types_enum							animation_type						( ) const
	{
		return static_cast< animation_types_enum >( impl().animation_type );
	}
	inline	u32												bones_count							( ) const { return impl().bones_count; }
	inline	bi_spline_bone_animation_baked const*			bones								( ) const
	{
		return reinterpret_cast< bi_spline_bone_animation_baked const* >( &impl() + 1 );
	}
	inline	u32												event_channel_count					( ) const { return impl().event_channels_count; }
	inline	bi_spline_event_channel_baked const*			event_channels						( ) const
	{
		return reinterpret_cast< bi_spline_event_channel_baked const* >(
			bones() + bones_count()
		);
	}
private:
	inline	bi_spline_skeleton_animation_baked_impl const&	impl								( ) const
	{
		return *reinterpret_cast< bi_spline_skeleton_animation_baked_impl const* >( this + 1 );
	}
private:
	/* 0x0000 */	/* resources::unmanaged_resource */
	/* 0x0108 */	configs::binary_config_ptr		m_bones_names;

	friend class bi_spline_skeleton_animation_impl_cook;
}; // class bi_spline_skeleton_animation_baked

STATIC_SIZE_ASSERT(bi_spline_skeleton_animation_baked, 0x110);

void create_baked_animation_in_place( void* const raw_buffer, const u32 buffer_size );

} // namespace animation
} // namespace vostok

#endif // #ifndef ANIMATION_BI_SPLINE_SKELETON_ANIMATION_BAKED_H_INCLUDED
