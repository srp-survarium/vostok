////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SURFACE_STATS_H_INCLUDED
#define RENDER_SURFACE_STATS_H_INCLUDED

/* INCLUDES */
class vostok::fixed_string<260>;

namespace vostok {
namespace render {

struct surface_stats {
	inline		surface_stats	( ) { /* no source */ }

	/* 0x0000 */	u32						vcount;
	/* 0x0004 */	u32						tricount;
	/* 0x0008 */	fixed_string< 260 >		material;
}; // struct surface_stats

STATIC_SIZE_ASSERT(surface_stats, 0x118);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SURFACE_STATS_H_INCLUDED
