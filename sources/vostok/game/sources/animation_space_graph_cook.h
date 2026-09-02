// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef ANIMATION_SPACE_GRAPH_COOK_H_INCLUDED
#define ANIMATION_SPACE_GRAPH_COOK_H_INCLUDED

#include <vostok/resources_cook_classes.h>

namespace vostok {
namespace ai {
namespace navigation {
	struct world;
} // namespace navigation
} // namespace ai
} // namespace vostok

namespace survarium {

class animation_space_graph;

// graph-build config folds (defined in animation_space_graph_cook.cpp; external
// linkage in the shipped binary, used by on_animations_loaded)
u32						get_animation_vertices_count	( configs::binary_config_value const& groups_config );
std::pair< u32, u32 >	get_animation_mixes_count		( configs::binary_config_value const& groups_config );

class animation_space_graph_cook : public resources::translate_query_cook , private boost::noncopyable {
public:
			explicit	animation_space_graph_cook	( ai::navigation::world& navigation_world );

	virtual	void		translate_request_path		( pcstr request, fs_new::virtual_path_string& new_request ) const override;
	virtual	void		translate_query				( resources::query_result_for_cook& parent ) override;

	virtual	void		delete_resource				( resources::resource_base* resource ) override;

private:
			void		on_options_received			( resources::queries_result& data );
			void		on_animations_loaded		( resources::queries_result& data, configs::binary_config_ptr config );

			void		generate_graph_edges		( animation_space_graph* graph );

private:
	/* 0x0000 */	/* resources::translate_query_cook */
	/* 0x0020 */	/* boost::noncopyable */
	/* 0x0020 */	ai::navigation::world&		m_navigation_world;
}; // class animation_space_graph_cook

STATIC_SIZE_ASSERT(animation_space_graph_cook, 0x24);

} // namespace survarium

#endif // #ifndef ANIMATION_SPACE_GRAPH_COOK_H_INCLUDED
