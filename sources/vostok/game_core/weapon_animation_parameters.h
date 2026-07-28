////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_ANIMATION_PARAMETERS_H_INCLUDED
#define WEAPON_ANIMATION_PARAMETERS_H_INCLUDED

#include <vostok/animation/api.h> // sushi@TODO: for body_part_masks_enum


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
				) :
					recoil_horizontal	( arg_0 ),
					recoil_vertical		( arg_1 ),
					recoil_backward		( arg_2 ),
					body_part_mask		( arg_3 ),
					is_aimed			( arg_4 ),
					is_firing			( arg_5 )
				{ }

	inline		~weapon_animation_parameters( ) { }


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
