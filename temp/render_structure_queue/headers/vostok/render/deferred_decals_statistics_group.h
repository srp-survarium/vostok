////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_DEFERRED_DECALS_STATISTICS_GROUP_H_INCLUDED
#define RENDER_DEFERRED_DECALS_STATISTICS_GROUP_H_INCLUDED

/* INCLUDES */
struct vostok::render::statistics_float;
struct vostok::render::statistics_group;
struct vostok::render::statistics_int;

namespace vostok {
namespace render {

struct deferred_decals_statistics_group : public statistics_group {
			explicit	deferred_decals_statistics_group	( pcstr group_name );
	inline				~deferred_decals_statistics_group	( ) { /* no source */ }

	/* 0x0000 */	/* statistics_group */
	/* 0x0098 */	statistics_float	execute_time;
	/* 0x0170 */	statistics_int		num_decals;
	/* 0x022c */	statistics_int		num_decal_draw_calls;
}; // struct deferred_decals_statistics_group

STATIC_SIZE_ASSERT(deferred_decals_statistics_group, 0x2E8);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_DEFERRED_DECALS_STATISTICS_GROUP_H_INCLUDED
