////////////////////////////////////////////////////////////////////////////
//	Created 	: 25.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef ANIMATION_BI_SPLINE_BONE_ANIMATION_BAKED_H_INCLUDED
#define ANIMATION_BI_SPLINE_BONE_ANIMATION_BAKED_H_INCLUDED

/* INCLUDES */
class const vostok::animation::enum_channel_id;
struct vostok::platform_pointer_selector<vostok::animation::bi_spline_channel_animation_baked,1>::helper;

/* FORWARD REFS */
class vostok::animation::bi_spline_channel_animation_baked;

namespace vostok {
namespace animation {

class bi_spline_bone_animation_baked {
public:
	inline	bi_spline_channel_animation_baked const&	channel_animation				( const enum_channel_id arg_0 ) const { /* no source */ }

	inline		bi_spline_bone_animation_baked	( ) { /* no source */ }

private:
	/* 0x0000 */	platform_pointer_selector< bi_spline_channel_animation_baked, 1 >::helper	m_channel_animations[9];
}; // class bi_spline_bone_animation_baked

STATIC_SIZE_ASSERT(bi_spline_bone_animation_baked, 0x48);

} // namespace animation
} // namespace vostok

#endif // #ifndef ANIMATION_BI_SPLINE_BONE_ANIMATION_BAKED_H_INCLUDED
