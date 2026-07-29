////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_COOK_INTERMEDIATE_DATA_H_INCLUDED
#define RENDER_COOK_INTERMEDIATE_DATA_H_INCLUDED

/* INCLUDES */
class vostok::fs_new::virtual_path_string;
class vostok::render::vector<char const *>;
class vostok::resources::query_result_for_cook;
struct vostok::render::model_asset;
typedef vostok::resources::resource_ptr<vostok::configs::binary_config,vostok::resources::unmanaged_intrusive_base>
	vostok::configs::binary_config_ptr;
typedef vostok::resources::resource_ptr<vostok::render::render_model,vostok::resources::unmanaged_intrusive_base>
	vostok::render::render_model_ptr;
class vostok::configs::binary_config;
class vostok::render::render_model;

/* FORWARD REFS */
class vostok::vfs::vfs_iterator;

namespace vostok {
namespace render {

struct cook_intermediate_data {
					cook_intermediate_data	(
						fs_new::virtual_path_string const&		in_resource_path,
						resources::query_result_for_cook*		in_query_result
					);

			s32		find_material_index		( pcstr surface_name );
			s32		find_surface_index		( pcstr surface_name );

			void	register_models			( vfs::vfs_iterator const& fs_it );

	inline			~cook_intermediate_data	( ) { /* no source */ }

	/* 0x0000 */	fs_new::virtual_path_string				root_model_path;
	/* 0x0114 */	resources::query_result_for_cook*		parent_query;
	/* 0x0118 */	bool									status_failed;
	/* 0x0119 */	bool									render_model_data_ready;
	/* 0x011a */	bool									material_data_ready;
	/* 0x011b */	bool									material_settings_valid;
	/* 0x011c */	render_model_ptr						result_model;
	/* 0x0120 */	model_asset*							assets;
	/* 0x0124 */	u8										m_num_render_models;
	/* 0x0128 */	configs::binary_config_ptr				model_settings_config;
	/* 0x012c */	vector< pcstr >							m_surface_materials;
}; // struct cook_intermediate_data

STATIC_SIZE_ASSERT(cook_intermediate_data, 0x138);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_COOK_INTERMEDIATE_DATA_H_INCLUDED
