#include "pch.h"
#include <vostok/render/core/shader_binary_source_cook.h>
#include <vostok/render/core/shader_defines.h>
#include <vostok/render/core/dx11/binary_shader_source.h>

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

// claude@NOTE: legacy body split on is_need_recompile + spawned
// compile_shader_task; the canonical class dropped the whole recompile path
// (MASTER_GOLD loads precompiled shaders only) and binary_shader_source
// stores raw pointer+size instead of a D3D blob - reconstruct from 0x5589e0.
void shader_binary_source_cook::converted_shader_loaded(
	conveted_shader_loaded_data*,
	resources::queries_result&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x5589e0]
}

pcstr shader_type_to_ext( enum_shader_type )
{
	// STATE[STUB]
	// FUNCTION BODY[0x558870]
	return 0;
}

// claude@NOTE: legacy body scanned shader source text for declared macroses
// (found_shader_declarated_macroses - subsystem retired) to build the
// converted-shader path; the shipped path derivation differs - reconstruct
// from 0x558b70. Legacy remainder holds the ancestor.
void shader_binary_source_cook::create_resource(
	resources::query_result_for_cook&,
	const_buffer,
	mutable_buffer
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x558b70]
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
