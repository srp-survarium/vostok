////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_GRASS_STATISTICS_GROUP_H_INCLUDED
#define RENDER_GRASS_STATISTICS_GROUP_H_INCLUDED

/* INCLUDES */
struct vostok::render::statistics_group;
struct vostok::render::statistics_int;

namespace vostok {
namespace render {

struct grass_statistics_group : public statistics_group {
			explicit	grass_statistics_group	( pcstr group_name );
	inline				~grass_statistics_group	( ) { /* no source */ }

	/* 0x0000 */	/* statistics_group */
	/* 0x0094 */	statistics_int		num_total_patches;
	/* 0x0150 */	statistics_int		num_rendered_patches;
	/* 0x020c */	statistics_int		num_visible_patches;
}; // struct grass_statistics_group

STATIC_SIZE_ASSERT(grass_statistics_group, 0x2C8);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_GRASS_STATISTICS_GROUP_H_INCLUDED
