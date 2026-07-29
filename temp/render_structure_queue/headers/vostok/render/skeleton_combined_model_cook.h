////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SKELETON_COMBINED_MODEL_COOK_H_INCLUDED
#define RENDER_SKELETON_COMBINED_MODEL_COOK_H_INCLUDED

/* INCLUDES */
class vostok::resources::translate_query_cook;

/* FORWARD REFS */
class vostok::render::skeleton_combined_cook_data;
class vostok::resources::queries_result;
class vostok::resources::query_result_for_cook;
class vostok::resources::resource_base;

namespace vostok {
namespace render {

struct skeleton_combined_model_cook : public resources::translate_query_cook {
					skeleton_combined_model_cook	( );

	virtual	void	translate_query					( resources::query_result_for_cook& parent ) override;

	virtual	void	delete_resource					( resources::resource_base* resource ) override;

private:
			void	query_resources_by_data			( resources::query_result_for_cook* parent, skeleton_combined_cook_data* cook_data );

			void	on_config_loaded				( resources::queries_result& result, resources::query_result_for_cook* parent );
			void	on_resources_loaded				(
						resources::queries_result&				data,
						resources::query_result_for_cook*		parent,
						skeleton_combined_cook_data*			cook_data
					);
	inline	void	on_materials_loaded				(
						resources::queries_result&				arg_0,
						resources::query_result_for_cook*		arg_1,
						skeleton_combined_cook_data*			arg_2
					) { /* no source */ }
			void	on_material_effects_loaded		(
						resources::queries_result&				data,
						resources::query_result_for_cook*		parent,
						skeleton_combined_cook_data*			cook_data
					);

public:
	virtual			~skeleton_combined_model_cook	( ) { /* no source */ }
}; // struct skeleton_combined_model_cook

STATIC_SIZE_ASSERT(skeleton_combined_model_cook, 0x20);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SKELETON_COMBINED_MODEL_COOK_H_INCLUDED
