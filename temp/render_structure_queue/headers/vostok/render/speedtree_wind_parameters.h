////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SPEEDTREE_WIND_PARAMETERS_H_INCLUDED
#define RENDER_SPEEDTREE_WIND_PARAMETERS_H_INCLUDED

/* INCLUDES */
class vostok::render::shader_constant_host;

namespace vostok {
namespace render {

struct speedtree_wind_parameters {
			speedtree_wind_parameters	( );

	void	set							( SpeedTree::CWind const& wind );

	/* 0x0000 */	shader_constant_host*		m_wind_dir_parameter;
	/* 0x0004 */	shader_constant_host*		m_wind_times_parameter;
	/* 0x0008 */	shader_constant_host*		m_wind_distances_parameter;
	/* 0x000c */	shader_constant_host*		m_wind_leaves_parameter;
	/* 0x0010 */	shader_constant_host*		m_wind_gust_parameter;
	/* 0x0014 */	shader_constant_host*		m_wind_gust_hints_parameter;
	/* 0x0018 */	shader_constant_host*		m_wind_frond_ripple_parameter;
	/* 0x001c */	shader_constant_host*		m_wind_rolling_branches_parameter;
	/* 0x0020 */	shader_constant_host*		m_wind_rolling_leaves_parameter;
	/* 0x0024 */	shader_constant_host*		m_wind_twitching_leaves_parameter;
	/* 0x0028 */	shader_constant_host*		m_wind_tumbling_leaves_parameter;
}; // struct speedtree_wind_parameters

STATIC_SIZE_ASSERT(speedtree_wind_parameters, 0x2C);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SPEEDTREE_WIND_PARAMETERS_H_INCLUDED
