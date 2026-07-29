#include "pch.h"
#include "render_model_cooker.h"

namespace vostok {
namespace render {

user_mesh_cook::user_mesh_cook( ) :
	resources::unmanaged_cook(
		resources::user_mesh_class,
		reuse_true,
		use_current_thread_id,
		0,
		flag_create_allocates_destroy_deallocates
	)
{
	// STATE[STUB]
	// FUNCTION BODY[0x650df0]
}

mutable_buffer user_mesh_cook::allocate_resource(
	resources::query_result_for_cook&,
	const_buffer,
	bool
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x650c10]
	return mutable_buffer::zero( );
}

void user_mesh_cook::deallocate_resource( pvoid )
{
	// STATE[STUB]
	// FUNCTION BODY[0x650c00]
}

void user_mesh_cook::create_resource(
	resources::query_result_for_cook&,
	const_buffer,
	mutable_buffer
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x652200]
}

void user_mesh_cook::destroy_resource( resources::unmanaged_resource* resource )
{
	// STATE[STUB]
	// FUNCTION BODY[0x650f30]
	DELETE( resource );
}

static_render_model_instance_cook::static_render_model_instance_cook( ) :
	resources::translate_query_cook(
		resources::static_render_model_instance_class,
		reuse_false,
		use_current_thread_id
	)
{
	// STATE[STUB]
	// FUNCTION BODY[0x650d80]
}

void static_render_model_instance_cook::translate_query( resources::query_result_for_cook& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x652e20]
}

void static_render_model_instance_cook::on_sub_resources_loaded( resources::queries_result& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x651b20]
}

void static_render_model_instance_cook::delete_resource( resources::resource_base* resource )
{
	// STATE[STUB]
	// FUNCTION BODY[0x650ef0]
	DELETE( resource );
}

skeleton_render_model_instance_cook::skeleton_render_model_instance_cook( ) :
	resources::translate_query_cook(
		resources::skeleton_render_model_instance_class,
		reuse_false,
		use_current_thread_id
	)
{
	// STATE[STUB]
	// FUNCTION BODY[0x650d10]
}

void skeleton_render_model_instance_cook::translate_query( resources::query_result_for_cook& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x652d60]
}

void skeleton_render_model_instance_cook::on_sub_resources_loaded( resources::queries_result& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x651a10]
}

void skeleton_render_model_instance_cook::delete_resource( resources::resource_base* resource )
{
	// STATE[STUB]
	// FUNCTION BODY[0x650eb0]
	DELETE( resource );
}

cook_intermediate_data::cook_intermediate_data(
	fs_new::virtual_path_string const&	in_resource_path,
	resources::query_result_for_cook*	in_query_result
) :
	root_model_path			( in_resource_path ),
	parent_query			( in_query_result ),
	status_failed			( false ),
	render_model_data_ready	( false ),
	material_data_ready		( false ),
	material_settings_valid	( false ),
	assets					( 0 ),
	m_num_render_models		( 0 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x651980]
}

s32 cook_intermediate_data::find_surface_index( pcstr )
{
	// STATE[STUB]
	// FUNCTION BODY[0x650ca0]
	return 0;
}

s32 cook_intermediate_data::find_material_index( pcstr )
{
	// STATE[STUB]
	// FUNCTION BODY[0x650fb0]
	return 0;
}

void cook_intermediate_data::register_models( vfs::vfs_iterator const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x651830]
}

grass_render_model_cook::grass_render_model_cook( ) :
	render_model_cook( resources::grass_render_model_class )
{
	// STATE[STUB]
	// FUNCTION BODY[0x650c80]
}

void grass_render_model_cook::translate_query( resources::query_result_for_cook& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x653880]
}

render_model_cook::render_model_cook( resources::class_id_enum model_type ) :
	resources::translate_query_cook( model_type, reuse_true, use_current_thread_id )
{
	// STATE[STUB]
	// FUNCTION BODY[0x650c30]
}

void render_model_cook::translate_query( resources::query_result_for_cook& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6536f0]
}

void render_model_cook::on_fs_iterator_ready_submeshes(
	cook_intermediate_data*,
	vfs::vfs_locked_iterator const&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x653320]
}

void render_model_cook::on_model_settings_loaded(
	resources::queries_result&,
	cook_intermediate_data*
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x6528b0]
}

void render_model_cook::on_subresources_loaded(
	resources::queries_result&,
	cook_intermediate_data*
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x652ee0]
}

void render_model_cook::on_materials_loaded(
	resources::queries_result&,
	cook_intermediate_data*
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x652790]
}

enum_vertex_input_type mesh_type_to_vertex_input_type( mesh_type_enum )
{
	// STATE[STUB]
	// FUNCTION BODY[0x650ad0]
	return enum_vertex_input_type( 0 );
}

fs_new::virtual_path_string get_material_effects_instance_request_path(
	material_ptr,
	enum_vertex_input_type
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x6517b0]
	return fs_new::virtual_path_string( );
}

void render_model_cook::query_materail_effects( cook_intermediate_data* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x652300]
}

void arrange_surfaces_by_lod( cook_intermediate_data*, model_lods_descriptor*& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x651110]
}

void render_model_cook::finish_model_creation(
	resources::queries_result&,
	cook_intermediate_data*
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x651ca0]
}

void render_model_cook::delete_resource( resources::resource_base* resource )
{
	// STATE[STUB]
	// FUNCTION BODY[0x650e70]
	DELETE( resource );
}

} // namespace render
} // namespace vostok
