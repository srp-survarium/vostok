#ifndef VOSTOK_RENDER_ENGINE_COMBINED_MODEL_COOKER_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_COMBINED_MODEL_COOKER_H_INCLUDED

#include <vostok/render/facade/skeleton_combined_cook_data.h>
#include <vostok/resources_cook_classes.h>

namespace vostok {
namespace render {

struct skeleton_combined_model_cook : public resources::translate_query_cook {
	typedef resources::translate_query_cook super;

	skeleton_combined_model_cook( );

	virtual void translate_query( resources::query_result_for_cook& parent );
	virtual void delete_resource( resources::resource_base* resource );

private:
	void query_resources_by_data(
		resources::query_result_for_cook*	parent,
		skeleton_combined_cook_data*		cook_data
	);

	void on_config_loaded(
		resources::queries_result&			result,
		resources::query_result_for_cook*	parent
	);

	void on_resources_loaded(
		resources::queries_result&			data,
		resources::query_result_for_cook*	parent,
		skeleton_combined_cook_data*		cook_data
	);

	void on_materials_loaded(
		resources::queries_result&,
		resources::query_result_for_cook*,
		skeleton_combined_cook_data*
	)
	{
	}

	void on_material_effects_loaded(
		resources::queries_result&			data,
		resources::query_result_for_cook*	parent,
		skeleton_combined_cook_data*		cook_data
	);
};

STATIC_SIZE_ASSERT( skeleton_combined_model_cook, 0x20 );

class skeleton_combined_render_model_instance_cook :
	public resources::translate_query_cook
{
	typedef resources::translate_query_cook super;

public:
	skeleton_combined_render_model_instance_cook( );

	virtual void translate_query( resources::query_result_for_cook& parent );
	virtual void delete_resource( resources::resource_base* resource );

private:
	void on_resources_loaded(
		resources::queries_result&			data,
		resources::query_result_for_cook*	parent
	);
};

STATIC_SIZE_ASSERT( skeleton_combined_render_model_instance_cook, 0x20 );

struct skeleton_combined_model_instance_cook : public resources::translate_query_cook {
	typedef resources::translate_query_cook super;

	skeleton_combined_model_instance_cook( );

	virtual void translate_query( resources::query_result_for_cook& parent );
	virtual void delete_resource( resources::resource_base* resource );

private:
	void on_resources_loaded(
		resources::queries_result&			data,
		resources::query_result_for_cook*	parent_query
	);
};

STATIC_SIZE_ASSERT( skeleton_combined_model_instance_cook, 0x20 );

void build_from_config(
	configs::binary_config_ptr&	cfg,
	skeleton_combined_cook_data*	cook_data
);

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_COMBINED_MODEL_COOKER_H_INCLUDED
