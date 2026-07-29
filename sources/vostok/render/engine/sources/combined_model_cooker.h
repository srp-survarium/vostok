#ifndef VOSTOK_RENDER_ENGINE_COMBINED_MODEL_COOKER_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_COMBINED_MODEL_COOKER_H_INCLUDED

#include <vostok/animation/skeleton.h>
#include <vostok/configs_binary_config.h>
#include <vostok/fs/virtual_path_string.h>
#include <vostok/resources_cook_classes.h>
#include <vostok/resources_managed_resource.h>
#include <vostok/resources_unmanaged_resource.h>

namespace vostok {
namespace render {

struct skeleton_combined_cook_data {
	struct model_def {
		model_def( ) { }
		~model_def( ) { }

		fs_new::virtual_path_string			base_model_name;
		fs_new::virtual_path_string			part_name;
		fs_new::virtual_path_string			material_name;
		configs::binary_config_ptr			owner_model_config;
		configs::binary_config_ptr			export_properties_config;
		resources::unmanaged_resource_ptr	material_effects;
		resources::managed_resource_ptr		converted_model;
	};

	explicit skeleton_combined_cook_data( bool owner_cook ) :
		models_count	( 0 ),
		owner_is_cook	( owner_cook )
	{
	}

	~skeleton_combined_cook_data( ) { }

	fs_new::virtual_path_string		skeleton_name;
	animation::skeleton_ptr			skeleton;
	fs_new::virtual_path_string		bind_pose_name;
	resources::managed_resource_ptr	bind_pose;
	configs::binary_config_ptr		model_config;
	model_def						model_defs[8];
	u8								models_count;
	bool							owner_is_cook;
};

STATIC_SIZE_ASSERT( skeleton_combined_cook_data::model_def, 0x34C );
STATIC_SIZE_ASSERT( skeleton_combined_cook_data, 0x1C98 );

struct skeleton_combined_model_cook : public resources::translate_query_cook {
	skeleton_combined_model_cook( );
	virtual ~skeleton_combined_model_cook( ) { }

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
public:
	skeleton_combined_render_model_instance_cook( );
	virtual ~skeleton_combined_render_model_instance_cook( ) { }

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
	skeleton_combined_model_instance_cook( );
	virtual ~skeleton_combined_model_instance_cook( ) { }

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
