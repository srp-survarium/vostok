// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ANIMATION_SPACE_VERTEX_H_INCLUDED
#define ANIMATION_SPACE_VERTEX_H_INCLUDED

#include <vostok/resources_managed_resource.h>

namespace survarium {

struct animation_space_vertex {
				animation_space_vertex	( resources::managed_resource_ptr const& animation_vertex, pcstr animation_caption );

public:
	/* 0x0000 */	resources::managed_resource_ptr		animation;
	/* 0x0004 */	fs_new::virtual_path_string			caption;
	/* 0x0118 */	float								length;
	/* 0x011c */	u32									group_id;
	/* 0x0120 */	u32									intervals_count;
}; // struct animation_space_vertex

STATIC_SIZE_ASSERT(animation_space_vertex, 0x124);

} // namespace survarium

#endif // #ifndef ANIMATION_SPACE_VERTEX_H_INCLUDED
