////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef PROJECT_COOKER_SIMPLE_H_INCLUDED
#define PROJECT_COOKER_SIMPLE_H_INCLUDED

#include <vostok/resources_cook_classes.h>
#include <vostok/configs.h>	// configs::binary_config_ptr (by-value param)

namespace survarium {

class game_object_;
class simple_game_project;

class project_cooker_simple : public resources::translate_query_cook , public boost::noncopyable {
	typedef	resources::translate_query_cook	super;

public:
			explicit	project_cooker_simple			( bool editor_present );

		virtual	void		translate_query					( resources::query_result_for_cook& parent ) override;

	virtual	void		delete_resource					( resources::resource_base* resource ) override;

private:
			void		on_game_project_loaded			( resources::queries_result& data, resources::query_result_for_cook* parent );
			void		on_object_loaded				(
							game_object_&							__formal,
							simple_game_project*					project,
							resources::query_result_for_cook*		parent_query
						);

			void		create_game_objects				(
							configs::binary_config_ptr				project_cfg,
							resources::query_result_for_cook*		parent_query
						);

			void		on_damage_zones_loaded			( simple_game_project* project, resources::queries_result& data );
			void		on_ladders_loaded				( simple_game_project* project, resources::queries_result& data );
			void		on_collision_and_visuals_loaded	( resources::queries_result& data, simple_game_project* project );

private:
	/* 0x0000 */	/* resources::translate_query_cook */
	/* 0x0020 */	/* boost::noncopyable */
	/* 0x0020 */	bool	m_editor_present;
}; // class project_cooker_simple

STATIC_SIZE_ASSERT(project_cooker_simple, 0x24);

} // namespace survarium

#endif // #ifndef PROJECT_COOKER_SIMPLE_H_INCLUDED
