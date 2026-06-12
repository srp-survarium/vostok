////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef FLASH_MOVIE_RESOURCE_H_INCLUDED
#define FLASH_MOVIE_RESOURCE_H_INCLUDED

/* INCLUDES */
class vostok::resources::unmanaged_resource;
struct survarium::flash_movie;

namespace survarium {

struct flash_movie_resource : public resources::unmanaged_resource {
	inline		flash_movie_resource	( ) { /* no source */ }
	virtual		~flash_movie_resource	( ) { /* no source */ }

public:
	/* 0x0000 */	/* resources::unmanaged_resource */
	/* 0x0108 */	flash_movie*	movie;
}; // struct flash_movie_resource

STATIC_SIZE_ASSERT(flash_movie_resource, 0x110);

} // namespace survarium

#endif // #ifndef FLASH_MOVIE_RESOURCE_H_INCLUDED
