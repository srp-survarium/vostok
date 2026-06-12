////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STATS_GRAPH_H_INCLUDED
#define STATS_GRAPH_H_INCLUDED

/* INCLUDES */
struct survarium::stats_graph::stats_value;
class vostok::render::base_scene_view;

/* FORWARD REFS */
class vostok::render::ui::renderer;

namespace survarium {

class stats_graph : public boost::noncopyable {
public:
						stats_graph			(
							float		time_interval,
							float		invalid_value,
							float		important_value0,
							float		important_value1,
							u32			color
						);
						~stats_graph		( );

			void		add_value			( float time, float value );

			void		set_time_interval	( float new_time_interval );

			void		render				(
							render::ui::renderer&					renderer,
							render::base_scene_view_ptr const&		scene_view,
							const u32								top_margin,
							u32										height,
							u32										arg_4,
							u32										arg_5,
							u32										arg_6
						);

			void		stop_rendering		( );

	inline	u32			cumulative_count	( ) const { /* no source */ }
			float		cumulative_time		( ) const;
	inline	float		cumulative_value	( ) const { /* no source */ }
			float		average_value		( ) const;

			void		adjust_time_interval( );

private:
	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	stats_graph::stats_value*		m_newest_value;
	/* 0x0004 */	stats_graph::stats_value*		m_values_pool;
	/* 0x0008 */	float							m_time_interval;
	/* 0x000c */	const float						m_invalid_value;
	/* 0x0010 */	const float						m_important_value0;
	/* 0x0014 */	const float						m_important_value1;
	/* 0x0018 */	float							m_cumulative_value;
	/* 0x001c */	float							m_weighted_average_value;
	/* 0x0020 */	u32								m_count;
	/* 0x0024 */	u32								m_color;
}; // class stats_graph

STATIC_SIZE_ASSERT(stats_graph, 0x28);

} // namespace survarium

#endif // #ifndef STATS_GRAPH_H_INCLUDED
