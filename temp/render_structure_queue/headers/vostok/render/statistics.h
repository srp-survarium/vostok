////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STATISTICS_H_INCLUDED
#define RENDER_STATISTICS_H_INCLUDED

/* INCLUDES */
class vostok::quasi_singleton<vostok::render::statistics>;
struct vostok::render::cascaded_sun_shadow_statistics_group;
struct vostok::render::debug_statistics_group;
struct vostok::render::deferred_decals_statistics_group;
struct vostok::render::distortion_pass_statistics_group;
struct vostok::render::forward_decals_statistics_group;
struct vostok::render::forward_stage_statistics_group;
struct vostok::render::gbuffer_statistics_group;
struct vostok::render::general_statistics_group;
struct vostok::render::grass_statistics_group;
struct vostok::render::lights_statistics_group;
struct vostok::render::lpv_statistics_group;
struct vostok::render::particles_statistics_group;
struct vostok::render::postprocess_statistics_group;
struct vostok::render::sky_statistics_group;
struct vostok::render::speedtree_statistics_group;
struct vostok::render::ssao_statistics_group;
struct vostok::render::statistics_group;
struct vostok::render::visibility_statistics_group;

/* FORWARD REFS */
class vostok::ui::world;

namespace vostok {
namespace render {

class statistics : public quasi_singleton< statistics > {
public:
					statistics			( );
					~statistics			( );

			void	start				( );

	inline	void	render				( ui::world& arg_0, u32 arg_1, u32 arg_2 ) { /* no source */ }

	inline	void	clear				( ) { /* no source */ }

	inline	u32		get_num_text_lines	( ) const { /* no source */ }

	/* 0x0000 */	/* quasi_singleton< statistics > */
	/* 0x0000 */	statistics_group*					first_group;
	/* 0x0008 */	postprocess_statistics_group		postprocess_stat_group;
	/* 0x02e8 */	ssao_statistics_group				ssao_stat_group;
	/* 0x0810 */	distortion_pass_statistics_group	distortion_pass_stat_group;
	/* 0x0d38 */	particles_statistics_group			particles_stat_group;
	/* 0x17b0 */	sky_statistics_group				sky_stat_group;
	/* 0x1a90 */	speedtree_statistics_group			speedtree_stat_group;
	/* 0x1f08 */	forward_stage_statistics_group		forward_stage_stat_group;
	/* 0x21e8 */	cascaded_sun_shadow_statistics_group	cascaded_sun_shadow_stat_group;
	/* 0x30c8 */	grass_statistics_group				grass_stat_group;
	/* 0x3390 */	gbuffer_statistics_group			gbuffer_stat_group;
	/* 0x38b8 */	forward_decals_statistics_group		forward_decals_stat_group;
	/* 0x3ba0 */	lpv_statistics_group				lpv_stat_group;
	/* 0x4d98 */	deferred_decals_statistics_group	deferred_decals_stat_group;
	/* 0x5080 */	lights_statistics_group				lights_stat_group;
	/* 0x57f0 */	visibility_statistics_group			visibility_stat_group;
	/* 0x6908 */	general_statistics_group			general_stat_group;
	/* 0x6fd0 */	debug_statistics_group				debug_stat_group;
private:
	/* 0x7eb0 */	u32									m_max_string_width;
	/* 0x7eb4 */	u32									m_max_string_height;
}; // class statistics

STATIC_SIZE_ASSERT(statistics, 0x7EB8);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STATISTICS_H_INCLUDED
