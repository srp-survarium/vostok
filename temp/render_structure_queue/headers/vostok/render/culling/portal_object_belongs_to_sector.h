////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_CULLING_PORTAL_OBJECT_BELONGS_TO_SECTOR_H_INCLUDED
#define RENDER_CULLING_PORTAL_OBJECT_BELONGS_TO_SECTOR_H_INCLUDED

/* INCLUDES */
struct stlp_std::unary_function<vostok::collision::triangle_result const &,bool>;

/* FORWARD REFS */
class vostok::collision::triangle_result;

namespace vostok {
namespace render {
namespace culling {

struct portal_object_belongs_to_sector : public std::unary_function< collision::triangle_result const&, bool > {
	inline	explicit	portal_object_belongs_to_sector	( u32 arg_0 ) { /* no source */ }

	inline	bool		operator()						( collision::triangle_result const& arg_0 ) const { /* no source */ }

	inline				~portal_object_belongs_to_sector( ) { /* no source */ }

	/* 0x0000 */	/* std::unary_function< collision::triangle_result const&, bool > */
private:
	/* 0x0000 */	u32		m_sector_id;
}; // struct portal_object_belongs_to_sector

STATIC_SIZE_ASSERT(portal_object_belongs_to_sector, 0x4);

} // namespace culling
} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_CULLING_PORTAL_OBJECT_BELONGS_TO_SECTOR_H_INCLUDED
