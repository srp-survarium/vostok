////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STATS_GRAPH_H_INCLUDED
#define STATS_GRAPH_H_INCLUDED

// PDB spells the render param vostok::render::base_scene_view_ptr - the same
// resource_ptr type our render tree typedefs as scene_view_ptr
#include <vostok/render/engine/base_classes.h>

namespace vostok {
namespace render {
namespace ui {
	class renderer;
} // namespace ui
} // namespace render
} // namespace vostok

namespace survarium {

class stats_graph : public boost::noncopyable {
public:
	struct stats_value {

	public:
		/* 0x0000 */	stats_value*	next;
		/* 0x0004 */	stats_value*	previous;
		/* 0x0008 */	float			time;
		/* 0x000c */	float			value;
	}; // struct stats_value

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
							render::ui::renderer&			renderer,
							render::scene_view_ptr const&	scene_view,
							const u32						top_margin,
							u32								height,
							u32								arg_4,
							u32								arg_5,
							u32								arg_6
						);

			void		stop_rendering		( );

	inline	u32			cumulative_count	( ) const { /* no source */ return m_count; }
			float		cumulative_time		( ) const;
	inline	float		cumulative_value	( ) const { /* no source */ return m_cumulative_value; }
			float		average_value		( ) const;

private:
			void		adjust_time_interval( );

	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	stats_value*		m_newest_value;
	/* 0x0004 */	stats_value*		m_values_pool;
	/* 0x0008 */	float				m_time_interval;
	/* 0x000c */	const float			m_invalid_value;
	/* 0x0010 */	const float			m_important_value0;
	/* 0x0014 */	const float			m_important_value1;
	/* 0x0018 */	float				m_cumulative_value;
	/* 0x001c */	float				m_weighted_average_value;
	/* 0x0020 */	u32					m_count;
	/* 0x0024 */	u32					m_color;
}; // class stats_graph

STATIC_SIZE_ASSERT(stats_graph, 0x28);
STATIC_SIZE_ASSERT(stats_graph::stats_value, 0x10);

} // namespace survarium

#endif // #ifndef STATS_GRAPH_H_INCLUDED
