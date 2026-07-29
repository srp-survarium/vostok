////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SKELETON_COMBINED_COOK_DATA_H_INCLUDED
#define RENDER_SKELETON_COMBINED_COOK_DATA_H_INCLUDED

/* INCLUDES */
class vostok::fs_new::virtual_path_string;
struct vostok::render::skeleton_combined_cook_data::model_def;
typedef vostok::resources::resource_ptr<vostok::animation::skeleton,vostok::resources::unmanaged_intrusive_base>
	vostok::animation::skeleton_ptr;
typedef vostok::resources::resource_ptr<vostok::configs::binary_config,vostok::resources::unmanaged_intrusive_base>
	vostok::configs::binary_config_ptr;
typedef vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base>
	vostok::resources::managed_resource_ptr;
class vostok::animation::skeleton;
class vostok::configs::binary_config;
class vostok::resources::managed_resource;

namespace vostok {
namespace render {

struct skeleton_combined_cook_data {
	inline	explicit	skeleton_combined_cook_data	( bool owner_cook ) { /* no source */ }
	inline				~skeleton_combined_cook_data( ) { /* no source */ }

	/* 0x0000 */	fs_new::virtual_path_string			skeleton_name;
	/* 0x0114 */	animation::skeleton_ptr				skeleton;
	/* 0x0118 */	fs_new::virtual_path_string			bind_pose_name;
	/* 0x022c */	resources::managed_resource_ptr		bind_pose;
	/* 0x0230 */	configs::binary_config_ptr			model_config;
	/* 0x0234 */	skeleton_combined_cook_data::model_def	model_defs[8];
	/* 0x1c94 */	u8									models_count;
	/* 0x1c95 */	bool								owner_is_cook;
}; // struct skeleton_combined_cook_data

STATIC_SIZE_ASSERT(skeleton_combined_cook_data, 0x1C98);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SKELETON_COMBINED_COOK_DATA_H_INCLUDED
