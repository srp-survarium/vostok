////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SKELETON_COMBINED_COOK_DATA_MODEL_DEF_H_INCLUDED
#define RENDER_SKELETON_COMBINED_COOK_DATA_MODEL_DEF_H_INCLUDED

/* INCLUDES */
class vostok::fs_new::virtual_path_string;
typedef vostok::resources::resource_ptr<vostok::configs::binary_config,vostok::resources::unmanaged_intrusive_base>
	vostok::configs::binary_config_ptr;
typedef vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base>
	vostok::resources::managed_resource_ptr;
typedef vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base>
	vostok::resources::unmanaged_resource_ptr;
class vostok::configs::binary_config;
class vostok::resources::managed_resource;
class vostok::resources::unmanaged_resource;

namespace vostok {
namespace render {

struct skeleton_combined_cook_data::model_def {
	inline		model_def	( ) { /* no source */ }
	inline		~model_def	( ) { /* no source */ }

	/* 0x0000 */	fs_new::virtual_path_string				base_model_name;
	/* 0x0114 */	fs_new::virtual_path_string				part_name;
	/* 0x0228 */	fs_new::virtual_path_string				material_name;
	/* 0x033c */	configs::binary_config_ptr				owner_model_config;
	/* 0x0340 */	configs::binary_config_ptr				export_properties_config;
	/* 0x0344 */	resources::unmanaged_resource_ptr		material_effects;
	/* 0x0348 */	resources::managed_resource_ptr			converted_model;
}; // struct skeleton_combined_cook_data::model_def

STATIC_SIZE_ASSERT(skeleton_combined_cook_data::model_def, 0x34C);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SKELETON_COMBINED_COOK_DATA_MODEL_DEF_H_INCLUDED
