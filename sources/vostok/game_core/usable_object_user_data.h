// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef USABLE_OBJECT_USER_DATA_H_INCLUDED
#define USABLE_OBJECT_USER_DATA_H_INCLUDED

namespace survarium {

class collision_user;
class usable_object;
struct usable_object_user_data;

struct usable_object_user_data {
	// body in collision_user.h: the target attributes the ctor's COMDAT to
	// collision_user.h (the struct lived there in the original), so defining it
	// here would put the base COMDAT in the wrong objdiff unit and never pair.
	inline	explicit	usable_object_user_data	( );

	/* 0x0000 */	collision_user*				owner;
	/* 0x0004 */	usable_object*				current_object;
	/* 0x0008 */	u32							start_using_time_ms;
	/* 0x000c */	u32							current_time_ms;
	/* 0x0010 */	u32							current_progress;
	/* 0x0014 */	float						booster_artcont_time_factor;
	/* 0x0018 */	float						booster_engineer_use_time_factor;
	/* 0x001c */	usable_object_user_data*	next;
}; // struct usable_object_user_data

STATIC_SIZE_ASSERT(usable_object_user_data, 0x20);

typedef intrusive_list<
	usable_object_user_data,
	usable_object_user_data *,
	&usable_object_user_data::next,
	threading::single_threading_policy,
	size_policy,
	no_debug_policy > usable_object_user_data_list;


} // namespace survarium

#endif // #ifndef USABLE_OBJECT_USER_DATA_H_INCLUDED
