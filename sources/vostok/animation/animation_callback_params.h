////////////////////////////////////////////////////////////////////////////
//	Created		: 26.10.2011
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_ANIMATION_ANIMATION_CALLBACK_PARAMS_H_INCLUDED
#define VOSTOK_ANIMATION_ANIMATION_CALLBACK_PARAMS_H_INCLUDED

#include <vostok/animation/skeleton_animation.h>

namespace vostok {
namespace animation {

struct animation_callback_params : public boost::noncopyable {
	inline		animation_callback_params	(
					pcvoid							animated_object,
					skeleton_animation_ptr const&	animation,
					pcstr							channel_id,
					const u32						callback_time_in_ms,
					const u32						animation_user_data,
					const u8						domain_data,
					const u8						animation_interval_id
				) :
					 animated_object						( animated_object ),
					 animation								( animation ),
					 channel_id								( channel_id ),
					 callback_time_in_ms					( callback_time_in_ms ),
					 animation_user_data					( animation_user_data ),
					 domain_data							( domain_data ),
					 animation_interval_id					( animation_interval_id ),
					 interrupt_animation_player_tick		( false ) { }

public:
	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	pcvoid const					animated_object;
	/* 0x0004 */	skeleton_animation_ptr const&	animation;
	/* 0x0008 */	pcstr const						channel_id;
	/* 0x000c */	const u32						callback_time_in_ms;
	/* 0x0010 */	const u32						animation_user_data;
	/* 0x0014 */	const u8						domain_data;
	/* 0x0015 */	const u8						animation_interval_id;
	/* 0x0016 */	bool							interrupt_animation_player_tick;
}; // struct animation_callback_params

STATIC_SIZE_ASSERT(animation_callback_params, 0x18);

} // namespace animation
} // namespace vostok

#endif // #ifndef VOSTOK_ANIMATION_ANIMATION_CALLBACK_PARAMS_H_INCLUDED
