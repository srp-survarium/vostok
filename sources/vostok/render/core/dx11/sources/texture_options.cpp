#include "pch.h"
#include <vostok/fs/virtual_path_string.h>
#include <vostok/render/core/texture_options.h>
#include <vostok/render/core/texture_options_cooker.h>

namespace vostok {
namespace render {

texture_options_cooker::texture_options_cooker(
	resources::class_id_enum resource_class,
	resources::cook_base::reuse_enum reuse_type,
	u32 translate_query_thread,
	enum_flags<resources::cook_base::flags_enum> flags
)
	: super( resource_class, reuse_type, translate_query_thread, flags )
{
}

void texture_options_cooker::delete_resource( resources::resource_base* )
{
}

texture_options_binary_cooker::texture_options_binary_cooker( )
	: super(
		resources::texture_options_binary_class,
		reuse_true,
		use_resource_manager_thread_id
	)
{
}

void texture_options_binary_cooker::translate_query(
	resources::query_result_for_cook& parent
)
{
	pcstr path = parent.get_requested_path();

	resources::query_resource(
					path,
					resources::binary_config_class,
					boost::bind(&texture_options_binary_cooker::on_binary_config_loaded, this, _1),
					resources::unmanaged_allocator(),
					NULL,
					&parent);
}

void texture_options_binary_cooker::on_binary_config_loaded(
	resources::queries_result& result
)
{
	resources::query_result_for_cook* parent	= result.get_parent_query();

	if( result[0].is_successful())
	{
		// result[0] == binary config
		resources::unmanaged_resource_ptr	resource =	result[0].get_unmanaged_resource( );

		parent->set_unmanaged_resource	( resource, resource->memory_usage( ) );
		parent->finish_query			( result_success);
	}else
	{
		fs_new::virtual_path_string path;
		path.assign_replace		( parent->get_requested_path(), "resources/", "resources.sources/" );

		resources::query_resource(
						path.c_str(),
						resources::texture_options_lua_class,
						boost::bind(&texture_options_binary_cooker::on_lua_options_loaded, this, _1),
						resources::unmanaged_allocator(),
						NULL,
						parent);

	}
}

void texture_options_binary_cooker::on_lua_options_loaded(
	resources::queries_result& result
)
{
	resources::query_result_for_cook* parent	= result.get_parent_query();

	R_ASSERT						( result[0].is_successful() );
	parent->finish_query			( result_requery );
}

} // namespace render
} // namespace vostok
