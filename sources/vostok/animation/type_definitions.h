////////////////////////////////////////////////////////////////////////////
//	Created		: 01.02.2010
//	Author		: Konstantin Slipchenko
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_ANIMATION_TYPE_DEFINITIONS_H_INCLUDED
#define VOSTOK_ANIMATION_TYPE_DEFINITIONS_H_INCLUDED

namespace vostok {
namespace animation {

typedef	u32									bone_index_type;
typedef	FILE*								stream;
typedef	resources::managed_resource_ptr		skeleton_animation_ptr;

enum animation_types_enum {
	animation_type_full,
	animation_type_additive,
}; // enum animation_types_enum

enum playback_enum {
	play_cyclically				= 0x0000,
	play_once_and_freeze_at_end = 0x0001,
	play_once_and_remove_at_end = 0x0002,
}; // enum playback_enum // sushi@TODO

enum body_part_masks_enum {
	body_part_whole_body				= -1,
	body_part_hands_only				= 0x0002,
	body_part_whole_body_but_hands		= -3,
	bone_mask_whole_weapon				= -1,
	bone_mask_offset_only				= 0x0002,
	bone_mask_whole_weapon_but_offset	= -3,
}; // enum body_part_masks_enum // sushi@TODO

// sushi@TODO: Should be moved to a different place?
struct animation_playback_state {
	inline			animation_playback_state( ) { /* no source */ }
	inline	void	reset					( ) { /* no source */ }

public:
	/* 0x0000 */	u32			interval_id;
	/* 0x0004 */	float		interval_time;
}; // struct animation_playback_state

STATIC_SIZE_ASSERT(animation_playback_state, 0x8);

}  // namespace animation
}  // namespace vostok

#endif // #ifndef VOSTOK_ANIMATION_TYPE_DEFINITIONS_H_INCLUDED