// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef ANIMATION_CONTROLLER_PARAMETERS_H_INCLUDED
#define ANIMATION_CONTROLLER_PARAMETERS_H_INCLUDED

// animation::animation_expression_emitter_ptr value members
#include <vostok/animation/animation_expression_emitter.h>

namespace survarium {

struct animation_controller_parameters {
	virtual	void	reset							( ) = 0;

}; // struct animation_controller_parameters

STATIC_SIZE_ASSERT(animation_controller_parameters, 0x4);

struct simple_animation_controller_parameters : public animation_controller_parameters {
	virtual	void	reset	( ) override;


public:
	/* 0x0000 */	/* animation_controller_parameters */
	/* 0x0004 */	animation::animation_expression_emitter_ptr	emitter;
}; // struct simple_animation_controller_parameters

STATIC_SIZE_ASSERT(simple_animation_controller_parameters, 0x8);

bool	operator==	( simple_animation_controller_parameters const& first, simple_animation_controller_parameters const& second );
bool	operator!=	( simple_animation_controller_parameters const& first, simple_animation_controller_parameters const& second );

struct movement_animation_controller_parameters : public animation_controller_parameters {
	virtual	void	reset	( ) override;


public:
	/* 0x0000 */	/* animation_controller_parameters */
	/* 0x0004 */	float3		position;
	/* 0x0010 */	float3		eyes_direction;
	/* 0x001c */	float3		velocity;
	/* 0x0028 */	animation::animation_expression_emitter_ptr	animation;
}; // struct movement_animation_controller_parameters

STATIC_SIZE_ASSERT(movement_animation_controller_parameters, 0x2C);

bool	operator==	( movement_animation_controller_parameters const& first, movement_animation_controller_parameters const& second );
bool	operator!=	( movement_animation_controller_parameters const& first, movement_animation_controller_parameters const& second );

} // namespace survarium

#endif // #ifndef ANIMATION_CONTROLLER_PARAMETERS_H_INCLUDED
