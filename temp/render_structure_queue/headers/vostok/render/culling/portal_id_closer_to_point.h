////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_CULLING_PORTAL_ID_CLOSER_TO_POINT_H_INCLUDED
#define RENDER_CULLING_PORTAL_ID_CLOSER_TO_POINT_H_INCLUDED

/* INCLUDES */
struct stlp_std::binary_function<unsigned int,unsigned int,bool>;

namespace vostok {
namespace render {
namespace culling {

struct portal_id_closer_to_point : public std::binary_function< u32, u32, bool > {
	inline	explicit	portal_id_closer_to_point	( float const* arg_0 ) { /* no source */ }

	inline	bool		operator()					( u32 arg_0, u32 arg_1 ) const { /* no source */ }

	inline				~portal_id_closer_to_point	( ) { /* no source */ }

	/* 0x0000 */	/* std::binary_function< u32, u32, bool > */
private:
	/* 0x0000 */	float const*	m_distances;
}; // struct portal_id_closer_to_point

STATIC_SIZE_ASSERT(portal_id_closer_to_point, 0x4);

} // namespace culling
} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_CULLING_PORTAL_ID_CLOSER_TO_POINT_H_INCLUDED
