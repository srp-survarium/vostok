#include "pch.h"
#include <vostok/render/core/resource_manager.h>
#include <vostok/render/core/shader_binary_source_cook.h>
#include <vostok/render/core/shader_defines.h>
#include <vostok/render/core/res_effect.h>

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
	// FUNCTION BODY[0x5588a0]
}

shader_binary_source_cook::~shader_binary_source_cook( )
{
	// FUNCTION BODY[0x558890]
//!	tasks::delete_task_type	( m_tasks_type );
}

// claude@NOTE: the legacy body branched on
// `!result[0].is_successful() || s_no_cache_shaders_key.is_set() || is_need_recompile(..)`
// into a spawn-compile_shader_task arm; the canonical class dropped that whole
// recompile machinery (is_need_recompile / compile_shader_task /
// all_tasks_finished / save_binary_shader are not members, and
// resource_manager::get_shader_sources() is gone), so only the legacy else-arm -
// load the precompiled blob - survives here. Its D3DCreateBlob + memory::copy
// pair collapses to a raw pointer into the pinned managed resource, which is what
// the canonical binary_shader_source models with
// m_shader_byte_code / m_shader_byte_code_size (and why it keeps shader_source
// alive); that collapse is also what fits the 0x18d-byte target body.
void shader_binary_source_cook::converted_shader_loaded(
	conveted_shader_loaded_data* data,
	resources::queries_result& result
)
{
	// FUNCTION BODY[0x5589e0]
	data->new_resource->shader_source			= result[0].get_managed_resource();

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

pcstr shader_type_to_ext( enum_shader_type const type )
{
	// FUNCTION BODY[0x558870]
	switch(type)
	{
		case enum_shader_type_vertex:	return "vs";
		case enum_shader_type_pixel:	return "ps";
		case enum_shader_type_geometry: return "gs";
		default: NODEFAULT(return "");
	}
}

static pcstr shader_type_to_compile_target( enum_shader_type const type )
{
	switch(type)
	{
		case enum_shader_type_vertex:	return "vs_4_0";
		case enum_shader_type_pixel:	return "ps_4_0";
		case enum_shader_type_geometry: return "gs_4_0";
		default: NODEFAULT(return "");
	}
}

// claude@NOTE: PARTIAL port of the legacy create_resource - the empty-name guard,
// the placement-new, the converted-shader base path, finish_query( result_postponed )
// and the query_resources( .. converted_shader_loaded .. ) tail are faithful. NOT
// ported: the per-define path-suffix loop between them, which needs
// resource_manager::get_shader_source_by_short_name (m_sources dropped from the
// canonical class) plus found_shader_declarated_macroses (shader_declarated_macros
// retired). shader_macros::merge_with_declared_macroses survives canonically, so the
// shipped generation still consumes declared macroses from some other source - that
// choice, and the resulting path spelling, need the 0x558b70 disassembly.
void shader_binary_source_cook::create_resource(
	resources::query_result_for_cook&	in_out_query,
	const_buffer						raw_file_data,
	mutable_buffer						in_out_unmanaged_resource_buffer
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x558b70]
	binary_shader_cook_data* user_data				= (binary_shader_cook_data*)raw_file_data.c_ptr();

	if (!user_data->shader_name.length())
	{
		in_out_query.finish_query					(result_error, in_out_query.assert_on_fail());
		return;
	}

	binary_shader_source* new_resource				= new(in_out_unmanaged_resource_buffer.c_ptr())binary_shader_source;

	fs_new::virtual_path_string converted_shader_path;
	converted_shader_path.assignf					(
		"%s/%s/%s/",
		resource_manager::ref().get_converted_shader_path(),
		user_data->shader_name.c_str(),
		shader_type_to_compile_target(user_data->shader_type)
	);

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

	params.assert_on_fail				=	assert_on_fail_false;

	resources::query_resources				(params);
}

void shader_binary_source_cook::destroy_resource(
	resources::unmanaged_resource* resource_to_destroy
)
{
	// FUNCTION BODY[0x558950]
	binary_shader_source* resource					= (binary_shader_source*)resource_to_destroy;

	resource->~binary_shader_source					();
}

mutable_buffer shader_binary_source_cook::allocate_resource(
	resources::query_result_for_cook& in_query,
	const_buffer raw_file_data,
	bool file_exist
)
{
	// FUNCTION BODY[0x5589b0]
	VOSTOK_UNREFERENCED_PARAMETERS					(&file_exist, &raw_file_data, &in_query);
	return											vostok::mutable_buffer(
		(pvoid)ALLOC(binary_shader_source, 1),
		sizeof(binary_shader_source)
	);
}

void shader_binary_source_cook::deallocate_resource( void* buffer )
{
	// FUNCTION BODY[0x558930]
	FREE											(buffer);
}

} // namespace render
} // namespace vostok
