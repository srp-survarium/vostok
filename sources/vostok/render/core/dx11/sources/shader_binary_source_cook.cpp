#include "pch.h"
#include <vostok/render/core/shader_binary_source_cook.h>
#include <vostok/render/core/shader_defines.h>

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
	  m_tasks_type( 0 ),
	  m_blob_creation_counter( 0 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5588a0]
}

shader_binary_source_cook::~shader_binary_source_cook( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x558890]
}

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
	resources::unmanaged_resource*
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x558950]
}

mutable_buffer shader_binary_source_cook::allocate_resource(
	resources::query_result_for_cook&,
	const_buffer,
	bool
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x5589b0]
	return mutable_buffer( );
}

void shader_binary_source_cook::deallocate_resource( void* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x558930]
}

} // namespace render
} // namespace vostok
