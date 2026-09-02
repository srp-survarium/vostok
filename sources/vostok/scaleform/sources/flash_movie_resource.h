// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef FLASH_MOVIE_RESOURCE_H_INCLUDED
#define FLASH_MOVIE_RESOURCE_H_INCLUDED

#include <vostok/resources_unmanaged_resource.h>

namespace survarium {

struct flash_movie;

struct flash_movie_resource : public vostok::resources::unmanaged_resource {

public:
	/* 0x0000 */	/* resources::unmanaged_resource */
	/* 0x0108 */	flash_movie*	movie;
}; // struct flash_movie_resource

STATIC_SIZE_ASSERT(flash_movie_resource, 0x110);

// canonical UDT alias (resource_ptr< flash_movie_resource, unmanaged_intrusive_base >),
// held by value across the menu/options/chat classes
typedef vostok::resources::resource_ptr<
	flash_movie_resource,
	vostok::resources::unmanaged_intrusive_base
> flash_movie_resource_ptr;

} // namespace survarium

#endif // #ifndef FLASH_MOVIE_RESOURCE_H_INCLUDED
