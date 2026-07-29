#ifndef VOSTOK_RENDER_ENGINE_RENDER_MODEL_COOKER_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_RENDER_MODEL_COOKER_H_INCLUDED

#include <vostok/configs_binary_config.h>
#include <vostok/fs/virtual_path_string.h>
#include <vostok/render/core/memory.h>
#include <vostok/render/engine/model_format.h>
#include <vostok/resources_cook_classes.h>
#include "material.h"
#include "render_model.h"

namespace vostok {

namespace vfs {

class vfs_iterator;
class vfs_locked_iterator;

} // namespace vfs

namespace render {

struct model_asset {
	model_asset( ) { }
	~model_asset( ) { }

	resources::managed_resource_ptr		converted_model_buffer;
	resources::unmanaged_resource_ptr	material;
	configs::binary_config_ptr			export_properties_config;
	fs_new::virtual_path_string			m_surface_name;
};

STATIC_SIZE_ASSERT( model_asset, 0x120 );

struct cook_intermediate_data {
	cook_intermediate_data(
		fs_new::virtual_path_string const&	in_resource_path,
		resources::query_result_for_cook*	in_query_result
	);

	s32 find_material_index( pcstr surface_name );
	s32 find_surface_index( pcstr surface_name );
	void register_models( vfs::vfs_iterator const& fs_it );

	fs_new::virtual_path_string			root_model_path;
	resources::query_result_for_cook*	parent_query;
	bool								status_failed;
	bool								render_model_data_ready;
	bool								material_data_ready;
	bool								material_settings_valid;
	render_model_ptr					result_model;
	model_asset*						assets;
	u8									m_num_render_models;
	configs::binary_config_ptr			model_settings_config;
	vector< pcstr >						m_surface_materials;
};

STATIC_SIZE_ASSERT( cook_intermediate_data, 0x138 );

struct user_mesh_cook : public resources::unmanaged_cook {
	user_mesh_cook( );
	virtual ~user_mesh_cook( ) { }

	virtual mutable_buffer allocate_resource(
		resources::query_result_for_cook&	in_query,
		const_buffer						raw_file_data,
		bool								file_exist
	);

	virtual void deallocate_resource( pvoid buffer );

	virtual void create_resource(
		resources::query_result_for_cook&	in_out_query,
		const_buffer						raw_file_data,
		mutable_buffer						in_out_unmanaged_resource_buffer
	);

	virtual void destroy_resource( resources::unmanaged_resource* resource );
};

STATIC_SIZE_ASSERT( user_mesh_cook, 0x20 );

class static_render_model_instance_cook : public resources::translate_query_cook {
public:
	static_render_model_instance_cook( );
	virtual ~static_render_model_instance_cook( ) { }

	virtual void translate_query( resources::query_result_for_cook& parent );
	virtual void delete_resource( resources::resource_base* resource );

private:
	void on_sub_resources_loaded( resources::queries_result& data );
};

STATIC_SIZE_ASSERT( static_render_model_instance_cook, 0x20 );

class skeleton_render_model_instance_cook : public resources::translate_query_cook {
public:
	skeleton_render_model_instance_cook( );
	virtual ~skeleton_render_model_instance_cook( ) { }

	virtual void translate_query( resources::query_result_for_cook& parent );
	virtual void delete_resource( resources::resource_base* resource );

private:
	void on_sub_resources_loaded( resources::queries_result& data );
};

STATIC_SIZE_ASSERT( skeleton_render_model_instance_cook, 0x20 );

struct render_model_cook : public resources::translate_query_cook {
	explicit render_model_cook( resources::class_id_enum model_type );
	virtual ~render_model_cook( ) { }

	virtual void translate_query( resources::query_result_for_cook& parent );
	virtual void delete_resource( resources::resource_base* resource );

protected:
	void on_subresources_loaded( resources::queries_result& data, cook_intermediate_data* cook_data );
	void on_materials_loaded( resources::queries_result& data, cook_intermediate_data* cook_data );
	void on_model_settings_loaded( resources::queries_result& data, cook_intermediate_data* cook_data );
	void finish_model_creation( resources::queries_result& data, cook_intermediate_data* cook_data );
	void query_materail_effects( cook_intermediate_data* cook_data );
	void on_fs_iterator_ready_submeshes(
		cook_intermediate_data*			cook_data,
		vfs::vfs_locked_iterator const&	fs_it
	);

	u32 count_models( vfs::vfs_iterator const& )
	{
		return 0;
	}
};

STATIC_SIZE_ASSERT( render_model_cook, 0x20 );

struct grass_render_model_cook : public render_model_cook {
	grass_render_model_cook( );
	virtual ~grass_render_model_cook( ) { }

	virtual void translate_query( resources::query_result_for_cook& parent );
};

STATIC_SIZE_ASSERT( grass_render_model_cook, 0x20 );

enum_vertex_input_type mesh_type_to_vertex_input_type( mesh_type_enum type );
fs_new::virtual_path_string get_material_effects_instance_request_path(
	material_ptr			material,
	enum_vertex_input_type	vertex_input_type
);
void arrange_surfaces_by_lod(
	cook_intermediate_data*	cook_data,
	model_lods_descriptor*&	lods_descriptor
);

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_RENDER_MODEL_COOKER_H_INCLUDED
