////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_FIND_BY_ID_PREDICATE_H_INCLUDED
#define RENDER_FIND_BY_ID_PREDICATE_H_INCLUDED

/* INCLUDES */
class vostok::render::sky_ambient_occlusion const* const;

namespace vostok {
namespace render {

struct find_by_id_predicate< sky_ambient_occlusion > {
	inline	explicit	find_by_id_predicate<vostok::render::sky_ambient_occlusion>( const u32 arg_0 ) { /* no source */ }

	inline	bool		operator()	( sky_ambient_occlusion const* const arg_0 ) { /* no source */ }

	/* 0x0000 */	u32		m_id;
}; // struct find_by_id_predicate< sky_ambient_occlusion >

STATIC_SIZE_ASSERT(find_by_id_predicate< sky_ambient_occlusion >, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_FIND_BY_ID_PREDICATE_H_INCLUDED
