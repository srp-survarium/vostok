// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include <vostok/render/core/resource_manager.h>
#include <vostok/render/core/shader_binary_source_cook.h>
#include <vostok/render/core/shader_defines.h>
#include <vostok/render/core/shader_macros.h>
#include <vostok/render/core/res_effect.h>
#include <vostok/command_line_extensions.h>
static vostok::command_line::key s_no_cache_shaders_key(
	"no_cache_shaders",
	"",
	"",
	"disable binary shaders caching"
);

namespace vostok {
namespace render {

struct shader_file_info {
	shader_file_info( ) { }

	char name[260];
	u32 time;
};

shader_binary_source_cook::shader_binary_source_cook( )
	: resources::unmanaged_cook(
		resources::shader_binary_source_class,
		reuse_false,
		use_current_thread_id,
		use_current_thread_id
	),
	  m_tasks_type( tasks::create_new_task_type("compile_shader_task", 0) ),
	  m_blob_creation_counter( 0 )
{
	// claude@NOTE: the task parameter constructor is inlined only in the target.
}

shader_binary_source_cook::~shader_binary_source_cook( )
{
//!	tasks::delete_task_type	( m_tasks_type );
}

void shader_binary_source_cook::converted_shader_loaded(
	conveted_shader_loaded_data* data,
	resources::queries_result& result
)
{
	data->new_resource->shader_source			= result[0].get_managed_resource();
	if (data->new_resource->shader_source == 0)
		debug::debug_message_box				("data->new_resource->shader_source == 0");

	resources::pinned_ptr_const<u8>	ptr_managed	(result[0].get_managed_resource());

	u32		num_saved_infos						= ((u32*)ptr_managed.c_ptr())[0];
	u32		additional_data_size				= sizeof(u32) + num_saved_infos * sizeof(shader_file_info);
	u32		binary_shader_size					= ptr_managed.size() - additional_data_size;
	pvoid	binary_shader_byte_code				= (pbyte)ptr_managed.c_ptr() + additional_data_size;

	data->new_resource->m_shader_byte_code		= binary_shader_byte_code;
	data->new_resource->m_shader_byte_code_size	= binary_shader_size;

	data->new_resource->configuration			= data->cook_data->configuration;
	data->new_resource->shader_name				= data->cook_data->shader_name;
	data->new_resource->shader_type				= data->cook_data->shader_type;

	data->in_out_query->set_unmanaged_resource	(data->new_resource, resources::nocache_memory, sizeof(binary_shader_source));
	data->in_out_query->finish_query			(result_success);

	DELETE										(data->cook_data);
	DELETE										(data);
}

static pcstr shader_type_to_ext( enum_shader_type type )
{
	switch(type)
	{
		case enum_shader_type_vertex:	return "vs";
		case enum_shader_type_pixel:	return "ps";
		case enum_shader_type_geometry: return "gs";
		default: NODEFAULT(return "");
	}
}

void shader_binary_source_cook::create_resource(
	resources::query_result_for_cook&	in_out_query,
	const_buffer						raw_file_data,
	mutable_buffer						in_out_unmanaged_resource_buffer
)
{
	binary_shader_cook_data* user_data				= (binary_shader_cook_data*)raw_file_data.c_ptr();

	if (!user_data->shader_name.length())
	{
		in_out_query.finish_query					(resources::query_result_for_user::error_type_cook_failed, assert_on_fail_false);

		// claude@MATCH: qualification keeps the target's direct deallocation path.
		shader_binary_source_cook::deallocate_resource(in_out_unmanaged_resource_buffer.c_ptr());
		DELETE										(user_data);
		return;
	}

	fs_new::virtual_path_string converted_shader_path;
	converted_shader_path.assignf					(
		"%s/%s.%s/",
		"resources/shaders/sm_4_0",
		user_data->shader_name.c_str(),
		shader_type_to_ext(user_data->shader_type)
	);

	binary_shader_source* new_resource				= new(in_out_unmanaged_resource_buffer.c_ptr())binary_shader_source;

	shader_defines_list working_defines_list;
	shader_macros::ref().fill_shader_macro_list		(working_defines_list, user_data->configuration);

	configs::binary_config_value const& masks		= resource_manager::ref().shader_name_to_mask_config->get_root();

	if (masks.value_exists(user_data->shader_name.c_str())
		&& masks[user_data->shader_name.c_str()].value_exists(shader_type_to_ext(user_data->shader_type)))
	{
		configs::binary_config_value const& mask_values	= masks[user_data->shader_name.c_str()][shader_type_to_ext(user_data->shader_type)];

		shader_defines_list::const_iterator define		= working_defines_list.begin();
		shader_defines_list::const_iterator define_end	= working_defines_list.end();

		for ( ; define != define_end; ++define)
		{
			configs::binary_config_value::const_iterator mask_end	= mask_values.end();

			if (!define->definition.length())
				continue;

			bool found									= false;
			for (configs::binary_config_value::const_iterator mask = mask_values.begin(); mask != mask_end; ++mask)
			{
				if (strings::compare(define->name.c_str(), *mask) == 0)
				{
					converted_shader_path				+= define->definition.c_str();
					found								= true;
					break;
				}
			}

			if (!found)
				converted_shader_path					+= "_";
		}
	}
	else
	{
		shader_defines_list::const_iterator define		= working_defines_list.begin();
		shader_defines_list::const_iterator define_end	= working_defines_list.end();

		for ( ; define != define_end; ++define)
			converted_shader_path						+= "_";
	}

	in_out_query.finish_query						(result_postponed);

	resources::request	request			=	{ converted_shader_path.c_str(), resources::raw_data_class };
	resources::query_resource_params	params(
		& request,
		NULL,
		1,
		boost::bind(
			&shader_binary_source_cook::converted_shader_loaded,
			this,
			NEW(conveted_shader_loaded_data)(
				&in_out_query,
				user_data,
				new_resource,
				converted_shader_path
			),
			_1
		),
		in_out_query.get_user_allocator(),
		NULL,
		NULL,
		NULL,
		&in_out_query
	);

	resources::query_resources				(params);
}

void shader_binary_source_cook::destroy_resource( resources::unmanaged_resource* resource_to_destroy )
{
	binary_shader_source* resource					= (binary_shader_source*)resource_to_destroy;

	if (resource->m_compiled_shader_byte_code)
	{
		MT_FREE										(resource->m_compiled_shader_byte_code);
		resource->m_compiled_shader_byte_code		= 0;
	}

	if (resource->error_code)
		resource->error_code->Release				();

	resource->~binary_shader_source					();
}

mutable_buffer shader_binary_source_cook::allocate_resource(
	resources::query_result_for_cook& in_query,
	const_buffer raw_file_data,
	bool file_exist
)
{
	// claude@NOTE: mutable_buffer construction is out-of-line only in the target.
	VOSTOK_UNREFERENCED_PARAMETERS					(&file_exist, &raw_file_data, &in_query);
	return											vostok::mutable_buffer(
		(pvoid)ALLOC(binary_shader_source, 1),
		sizeof(binary_shader_source)
	);
}

void shader_binary_source_cook::deallocate_resource( void* buffer )
{
	FREE											(buffer);
}

} // namespace render
} // namespace vostok
