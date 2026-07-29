////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STAGE_STAT_H_INCLUDED
#define RENDER_STAGE_STAT_H_INCLUDED

/* INCLUDES */
class vostok::render::stage;

namespace vostok {
namespace render {

struct stage_stat {
	inline	double		average_time( bool arg_0 ) const { /* no source */ }
	inline	u32			average_dips( ) const { /* no source */ }

	/* 0x0000 */	double		elapsed_gpu_msec[1];
	/* 0x0008 */	double		elapsed_cpu_msec[1];
	/* 0x0010 */	u32			dips[1];
	/* 0x0014 */	stage*		stg;
}; // struct stage_stat

STATIC_SIZE_ASSERT(stage_stat, 0x18);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STAGE_STAT_H_INCLUDED
