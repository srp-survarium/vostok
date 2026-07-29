////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_FIND_BY_ID_PREDICATE_H_INCLUDED
#define RENDER_FIND_BY_ID_PREDICATE_H_INCLUDED

/* INCLUDES */
class vostok::render::ambient_volume const* const;

namespace vostok {
namespace render {

struct find_by_id_predicate< ambient_volume > {
	inline	explicit	find_by_id_predicate<vostok::render::ambient_volume>( const u32 arg_0 ) { /* no source */ }

	inline	bool		operator()	( ambient_volume const* const arg_0 ) { /* no source */ }

	/* 0x0000 */	u32		m_id;
}; // struct find_by_id_predicate< ambient_volume >

STATIC_SIZE_ASSERT(find_by_id_predicate< ambient_volume >, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_FIND_BY_ID_PREDICATE_H_INCLUDED
