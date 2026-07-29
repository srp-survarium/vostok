////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_CULLING_PORTAL_OBJECT_CLOSER_TO_POINT_H_INCLUDED
#define RENDER_CULLING_PORTAL_OBJECT_CLOSER_TO_POINT_H_INCLUDED

/* INCLUDES */
struct stlp_std::binary_function<vostok::collision::triangle_result const &,vostok::collision::triangle_result const &,bool>;

/* FORWARD REFS */
class vostok::collision::triangle_result;

namespace vostok {
namespace render {
namespace culling {

struct portal_object_closer_to_point : public std::binary_function< collision::triangle_result const&, collision::triangle_result const&, bool > {
	inline	explicit	portal_object_closer_to_point	( float3 const& arg_0 ) { /* no source */ }

	inline	bool		operator()						( collision::triangle_result const& arg_0, collision::triangle_result const& arg_1 ) const { /* no source */ }

	inline				~portal_object_closer_to_point	( ) { /* no source */ }

	/* 0x0000 */	/* std::binary_function< collision::triangle_result const&, collision::triangle_result const&, bool > */
private:
	/* 0x0000 */	float3		m_point;
}; // struct portal_object_closer_to_point

STATIC_SIZE_ASSERT(portal_object_closer_to_point, 0xC);

} // namespace culling
} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_CULLING_PORTAL_OBJECT_CLOSER_TO_POINT_H_INCLUDED
