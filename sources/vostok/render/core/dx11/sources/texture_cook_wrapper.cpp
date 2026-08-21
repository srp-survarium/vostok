#include "pch.h"
#include <vostok/render/core/texture_cook_wrapper.h>
#include <vostok/command_line_extensions.h>
#include <vostok/fs/device_utils.h>
#include <vostok/resources_fs.h>

namespace vostok {
namespace render {

using namespace resources;
using namespace fs_new;

static pcstr converted_texture_extension			=	"dds";

static pcstr resources_textures_converted_string	=	"resources.converted/textures/";
static pcstr resources_textures_string				=	"resources/textures/";

#line 42
static command_line::key s_gpu_compressor( "gpu_compressor", "", "", "use gpu compressor" );

texture_cook_wrapper::texture_cook_wrapper( )
	: resources::translate_query_cook(
		resources::texture_wrapper_class,
#ifndef MASTER_GOLD
		reuse_false,
#else // #ifndef MASTER_GOLD
		reuse_true,
#endif // #ifndef MASTER_GOLD
		use_resource_manager_thread_id
	)
{
}

void texture_cook_wrapper::translate_query(
	resources::query_result_for_cook& query
)
{
	query_converted_texture					(& query);
}

void texture_cook_wrapper::query_converted_texture(
	resources::query_result_for_cook* parent
)
{
	virtual_path_string						converted_texture_path	(parent->get_requested_path());
	set_extension_for_path					(& converted_texture_path, converted_texture_extension);

	query_resource							(converted_texture_path.c_str(),
											 texture_class,
											 boost::bind(&texture_cook_wrapper::on_texture_loaded, this, _1),
											 & memory::g_mt_allocator,
											 NULL,
											 parent);
}

void texture_cook_wrapper::on_texture_loaded( resources::queries_result& result )
{
	query_result_for_cook * const	parent	=	result.get_parent_query();

	if ( !result.is_successful() )
	{
		parent->finish_query			(result_error);
		return;
	}

	parent->set_managed_resource	(result[0].get_managed_resource());
	parent->finish_query			(result_success);
}

void texture_cook_wrapper::delete_resource( resources::resource_base* )
{
#line 325
	CURE_ASSERT						(identity(false), "should not end up here! Call Lain");
}

} // namespace render
} // namespace vostok
