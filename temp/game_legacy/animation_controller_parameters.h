////////////////////////////////////////////////////////////////////////////
//	Created		: 23.11.2011
//	Author		: Tetyana Meleshchenko
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef ANIMATION_CONTROLLER_PARAMETERS_H_INCLUDED
#define ANIMATION_CONTROLLER_PARAMETERS_H_INCLUDED

//#include <vostok/animation/skeleton_animation.h>
#include <vostok/animation/animation_expression_emitter.h>

namespace survarium {

struct animation_controller_parameters
{
}; // struct animation_controller_parameters

struct simple_animation_controller_parameters : public animation_controller_parameters
{
	animation::animation_expression_emitter_ptr	emitter;
}; // struct simple_animation_controller_parameters

struct movement_animation_controller_parameters : public animation_controller_parameters
{
	float3										position;
	float3										eyes_direction;
	float3										velocity;
	animation::animation_expression_emitter_ptr	animation;
}; // struct movement_animation_controller_parameters

} // namespace survarium

#endif // #ifndef ANIMATION_CONTROLLER_PARAMETERS_H_INCLUDED