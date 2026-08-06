#ifndef VOSTOK_RENDER_ENGINE_PORTAL_SECTOR_STRUCTURE_COOK_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_PORTAL_SECTOR_STRUCTURE_COOK_H_INCLUDED

#include <boost/noncopyable.hpp>
#include <vostok/resources_cook_classes.h>

namespace vostok {
namespace render {
namespace culling {

class portal_sector_structure_cook :
	public resources::translate_query_cook,
	public boost::noncopyable
{
public:
	portal_sector_structure_cook( );

	virtual void translate_query( resources::query_result_for_cook& parent );
	virtual void delete_resource( resources::resource_base* resource );
};

STATIC_SIZE_ASSERT( portal_sector_structure_cook, 0x20 );

} // namespace culling
} // namespace render
} // namespace vostok

#endif // VOSTOK_RENDER_ENGINE_PORTAL_SECTOR_STRUCTURE_COOK_H_INCLUDED
