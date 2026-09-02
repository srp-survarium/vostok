// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ANIMATION_BI_SPLINE_SKELETON_ANIMATION_BAKED_IMPL_H_INCLUDED
#define ANIMATION_BI_SPLINE_SKELETON_ANIMATION_BAKED_IMPL_H_INCLUDED

namespace vostok {
namespace animation {

struct bi_spline_skeleton_animation_baked_impl {

public:
	/* 0x0000 */	u16		bones_count;
	/* 0x0002 */	u8		event_channels_count;
	/* 0x0003 */	u8		animation_type;
}; // struct bi_spline_skeleton_animation_baked_impl

STATIC_SIZE_ASSERT(bi_spline_skeleton_animation_baked_impl, 0x4);

} // namespace animation
} // namespace vostok

#endif // #ifndef ANIMATION_BI_SPLINE_SKELETON_ANIMATION_BAKED_IMPL_H_INCLUDED
