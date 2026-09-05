////////////////////////////////////////////////////////////////////////////
//	Created		: 26.10.2011
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_ANIMATION_ANIMATION_CALLBACK_H_INCLUDED
#define VOSTOK_ANIMATION_ANIMATION_CALLBACK_H_INCLUDED

#include <vostok/animation/skeleton_animation.h>

namespace vostok {
namespace animation {

enum callback_return_type_enum {
	callback_return_type_call_me_again,
	callback_return_type_dont_call_me_anymore,
	callback_return_type_hasnt_called,
}; // enum callback_return_type_enum

typedef boost::function< callback_return_type_enum ( skeleton_animation_ptr const& animation, pcstr channel_id, u32 callback_time_in_ms, u8 domain_data ) >	callback_type;

struct animation_callback_params;
typedef boost::function< enum callback_return_type_enum( animation_callback_params& ) > new_callback_type; // sushi@TODO: Once animations are recovered, rename this to callback_type

struct animation_callback : private boost::noncopyable {
	inline	explicit	animation_callback	(
					new_callback_type const&		callback,
					pcvoid const					callback_uid,
					skeleton_animation_ptr const&	animation,
					const u8						event_type,
					pcvoid const					animated_object
				) :
		callback				( callback ),
		animation				( animation ),
		animated_object			( animated_object ),
		next					( 0 ),
		callback_uid			( callback_uid ),
		event_type				( event_type ),
		enabled					( true )
	{
		R_ASSERT				( callback );
	}

	public:
	/* 0x0000 */	new_callback_type			callback;
	/* 0x0020 */	skeleton_animation_ptr		animation;
	/* 0x0024 */	pcvoid						animated_object;
	/* 0x0028 */	animation_callback*			next;
	/* 0x002c */	pcvoid						callback_uid;
	/* 0x0030 */	u8							event_type;
	/* 0x0031 */	bool						enabled;
}; // struct animation_callback

STATIC_SIZE_ASSERT(animation_callback, 0x38);

struct subscribed_channel {
	pcstr						channel_id;
	subscribed_channel*			next;
	animation_callback*			first_callback;
}; // struct subscribed_channel

STATIC_SIZE_ASSERT(subscribed_channel, 0xC);

} // namespace animation
} // namespace vostok

#include <vostok/animation/animation_callback_params.h>

#endif // #ifndef VOSTOK_ANIMATION_ANIMATION_CALLBACK_H_INCLUDED
