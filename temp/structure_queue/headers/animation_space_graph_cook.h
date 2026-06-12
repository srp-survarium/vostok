////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef ANIMATION_SPACE_GRAPH_COOK_H_INCLUDED
#define ANIMATION_SPACE_GRAPH_COOK_H_INCLUDED

/* INCLUDES */
class vostok::resources::translate_query_cook;
struct vostok::ai::navigation::world;
class vostok::configs::binary_config;

/* FORWARD REFS */
class vostok::fs_new::virtual_path_string;
class vostok::resources::queries_result;
class vostok::resources::query_result_for_cook;
class vostok::resources::resource_base;
class survarium::animation_space_graph;

namespace survarium {

class animation_space_graph_cook : public resources::translate_query_cook , public boost::noncopyable {
public:
			explicit	animation_space_graph_cook	( ai::navigation::world& navigation_world );

	virtual	void		translate_request_path		( pcstr request, fs_new::virtual_path_string& new_request ) const override;
	virtual	void		translate_query				( resources::query_result_for_cook& parent ) override;

	virtual	void		delete_resource				( resources::resource_base* resource ) override;

			void		on_options_received			( resources::queries_result& data );
			void		on_animations_loaded		( resources::queries_result& data, configs::binary_config_ptr config );

			void		generate_graph_edges		( animation_space_graph* graph );

	virtual				~animation_space_graph_cook	( ) { /* no source */ }

private:
	/* 0x0000 */	/* resources::translate_query_cook */
	/* 0x0020 */	/* boost::noncopyable */
	/* 0x0020 */	ai::navigation::world&		m_navigation_world;
}; // class animation_space_graph_cook

STATIC_SIZE_ASSERT(animation_space_graph_cook, 0x24);

} // namespace survarium

#endif // #ifndef ANIMATION_SPACE_GRAPH_COOK_H_INCLUDED
