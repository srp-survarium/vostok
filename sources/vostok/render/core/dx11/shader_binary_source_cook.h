// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_CORE_DX11_SHADER_BINARY_SOURCE_COOK_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_SHADER_BINARY_SOURCE_COOK_H_INCLUDED
#include <vostok/fs/virtual_path_string.h>
#include <vostok/resources_cook_classes.h>

namespace vostok {

namespace resources {
class queries_result;
} // namespace resources

namespace tasks {
class task_type;
} // namespace tasks

namespace render {

struct binary_shader_cook_data;
struct binary_shader_source;

struct conveted_shader_loaded_data {
	conveted_shader_loaded_data(
		resources::query_result_for_cook* query,
		binary_shader_cook_data* cook_data,
		binary_shader_source* resource,
		fs_new::virtual_path_string converted_shader_path
	)
		: in_out_query( query ),
		  cook_data( cook_data ),
		  new_resource( resource ),
		  converted_shader_path( converted_shader_path )
	{
	}

	resources::query_result_for_cook* in_out_query;
	binary_shader_cook_data* cook_data;
	binary_shader_source* new_resource;
	fs_new::virtual_path_string converted_shader_path;
	pbyte data_to_save;
	u32 save_data_size;
};

struct shader_binary_source_cook : public resources::unmanaged_cook {
	shader_binary_source_cook( );
	virtual ~shader_binary_source_cook( );

	virtual mutable_buffer allocate_resource(
		resources::query_result_for_cook& query,
		const_buffer raw_file_data,
		bool file_exist
	);
	virtual void deallocate_resource( void* buffer );
	virtual void create_resource(
		resources::query_result_for_cook& query,
		const_buffer raw_file_data,
		mutable_buffer unmanaged_resource_buffer
	);
	virtual void destroy_resource( resources::unmanaged_resource* resource );

	void converted_shader_loaded(
		conveted_shader_loaded_data* data,
		resources::queries_result& result
	);

private:
	tasks::task_type* m_tasks_type;
	volatile long m_blob_creation_counter;
};

STATIC_SIZE_ASSERT( conveted_shader_loaded_data, 0x128 );
STATIC_SIZE_ASSERT( shader_binary_source_cook, 0x28 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_SHADER_BINARY_SOURCE_COOK_H_INCLUDED
