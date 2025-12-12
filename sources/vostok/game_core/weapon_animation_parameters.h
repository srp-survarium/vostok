////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_ANIMATION_PARAMETERS_H_INCLUDED
#define WEAPON_ANIMATION_PARAMETERS_H_INCLUDED

namespace vostok {
namespace animation {
	// sushi@TODO: Match animations! Also strange values
	enum body_part_masks_enum {
		body_part_whole_body				= -1,
		body_part_hands_only				= 0x0002,
		body_part_whole_body_but_hands		= -3,
		bone_mask_whole_weapon				= -1,
		bone_mask_offset_only				= 0x0002,
		bone_mask_whole_weapon_but_offset	= -3,
	};
}
}

namespace survarium {

struct weapon_animation_parameters : public boost::noncopyable {
public:
	inline		weapon_animation_parameters	(
					float								arg_0,
					float								arg_1,
					float								arg_2,
					animation::body_part_masks_enum		arg_3,
					bool								arg_4,
					bool								arg_5
				) { /* no source */ }

	inline		~weapon_animation_parameters( ) { /* no source */ }


public:
	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	float								recoil_horizontal;
	/* 0x0004 */	float								recoil_vertical;
	/* 0x0008 */	float								recoil_backward;
	/* 0x000c */	animation::body_part_masks_enum		body_part_mask;
	/* 0x0010 */	bool								is_aimed;
	/* 0x0011 */	bool								is_firing;
}; // struct weapon_animation_parameters

STATIC_SIZE_ASSERT(weapon_animation_parameters, 0x14);

} // namespace survarium

#endif // #ifndef WEAPON_ANIMATION_PARAMETERS_H_INCLUDED
