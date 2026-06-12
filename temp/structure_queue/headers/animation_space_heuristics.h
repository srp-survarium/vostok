////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef ANIMATION_SPACE_HEURISTICS_H_INCLUDED
#define ANIMATION_SPACE_HEURISTICS_H_INCLUDED

/* INCLUDES */
class const survarium::animation_space_vertex_id;
struct survarium::animation_space_vertex_id;
class survarium::animation_space_vertex_id const* const;
typedef vostok::resources::resource_ptr<survarium::animation_space_graph,vostok::resources::unmanaged_intrusive_base>
	survarium::animation_space_graph_ptr;
class survarium::animation_space_graph;

namespace survarium {

class animation_space_heuristics : public boost::noncopyable {
public:
	inline										animation_space_heuristics	(
													animation_space_graph_ptr const&	graph,
													animation_space_vertex_id const&	target_vertex_id,
													const float							max_speed
												) { /* no source */ }

	inline	void								on_before_search			( ) { /* no source */ }
	inline	void								on_after_search				( const bool arg_0 ) { /* no source */ }
	inline	void								on_start_iteration			( const animation_space_vertex_id arg_0 ) { /* no source */ }

	inline	float								estimate					(
													animation_space_vertex_id const* const	current_vertex_id_ptr,
													animation_space_vertex_id const&	neighbour_vertex_id
												) const { /* no source */ }

	inline	animation_space_vertex_id const&	best_vertex_id				( ) const { /* no source */ }

	inline										~animation_space_heuristics	( ) { /* no source */ }

	static	inline	bool								metric_euclidian			( ) { /* no source */ }

private:
	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	animation_space_graph_ptr const&	m_graph;
	/* 0x0004 */	animation_space_vertex_id const&	m_target_vertex_id;
	/* 0x0008 */	animation_space_vertex_id			m_best_vertex_id;
	/* 0x0024 */	float								m_min_heuristics_value;
	/* 0x0028 */	const float							m_max_speed;
}; // class animation_space_heuristics

STATIC_SIZE_ASSERT(animation_space_heuristics, 0x2C);

} // namespace survarium

#endif // #ifndef ANIMATION_SPACE_HEURISTICS_H_INCLUDED
