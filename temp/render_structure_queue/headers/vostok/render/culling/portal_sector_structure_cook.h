////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_CULLING_PORTAL_SECTOR_STRUCTURE_COOK_H_INCLUDED
#define RENDER_CULLING_PORTAL_SECTOR_STRUCTURE_COOK_H_INCLUDED

/* INCLUDES */
class vostok::resources::translate_query_cook;

/* FORWARD REFS */
class vostok::resources::query_result_for_cook;
class vostok::resources::resource_base;

namespace vostok {
namespace render {
namespace culling {

class portal_sector_structure_cook : public resources::translate_query_cook , public boost::noncopyable {
public:
					portal_sector_structure_cook	( );

	virtual	void	translate_query					( resources::query_result_for_cook& parent ) override;

	virtual	void	delete_resource					( resources::resource_base* resource ) override;

	virtual			~portal_sector_structure_cook	( ) { /* no source */ }
}; // class portal_sector_structure_cook

STATIC_SIZE_ASSERT(portal_sector_structure_cook, 0x20);

} // namespace culling
} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_CULLING_PORTAL_SECTOR_STRUCTURE_COOK_H_INCLUDED
