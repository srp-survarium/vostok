#ifndef VOSTOK_RENDER_FACADE_SKELETON_COMBINED_COOK_DATA_H_INCLUDED
#define VOSTOK_RENDER_FACADE_SKELETON_COMBINED_COOK_DATA_H_INCLUDED

#include <vostok/animation/skeleton.h>
#include <vostok/configs_binary_config.h>
#include <vostok/fs/virtual_path_string.h>
#include <vostok/resources_managed_resource.h>
#include <vostok/resources_unmanaged_resource.h>

namespace vostok {
namespace render {

struct skeleton_combined_cook_data {
	explicit skeleton_combined_cook_data( bool owner_cook )
	{
		owner_is_cook = owner_cook;
	}

	fs_new::virtual_path_string		skeleton_name;
	animation::skeleton_ptr			skeleton;
	fs_new::virtual_path_string		bind_pose_name;
	resources::managed_resource_ptr	bind_pose;
	configs::binary_config_ptr		model_config;
	struct model_def {
		fs_new::virtual_path_string			base_model_name;
		fs_new::virtual_path_string			part_name;
		fs_new::virtual_path_string			material_name;
		configs::binary_config_ptr			owner_model_config;
		configs::binary_config_ptr			export_properties_config;
		resources::unmanaged_resource_ptr	material_effects;
		resources::managed_resource_ptr		converted_model;
	} model_defs[8];
	u8								models_count;
	bool							owner_is_cook;
};

STATIC_SIZE_ASSERT( skeleton_combined_cook_data::model_def, 0x34c );
STATIC_SIZE_ASSERT( skeleton_combined_cook_data, 0x1c98 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_FACADE_SKELETON_COMBINED_COOK_DATA_H_INCLUDED
