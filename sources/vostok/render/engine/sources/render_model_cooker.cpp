#include "pch.h"
#include "render_model_cooker.h"
#include <vostok/render/facade/material_effects_instance_cook_data.h>
#include "material_effects_instance.h"
#include "model_manager.h"
#include "render_model_grass.h"
#include "render_model_skeleton.h"
#include "render_model_static.h"
#include "render_model_user.h"

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
	// FUNCTION BODY[0x650df0]
}

mutable_buffer user_mesh_cook::allocate_resource(
	resources::query_result_for_cook& in_query,
	const_buffer raw_file_data,
	bool file_exist
)
{
	// FUNCTION BODY[0x650c10]
	VOSTOK_UNREFERENCED_PARAMETERS	( in_query, &raw_file_data, file_exist );
	return mutable_buffer::zero		( );
}

void user_mesh_cook::deallocate_resource( pvoid buffer )
{
	// FUNCTION BODY[0x650c00]
	VOSTOK_UNREFERENCED_PARAMETERS	( buffer );
}

void user_mesh_cook::create_resource(
	resources::query_result_for_cook& in_out_query,
	const_buffer raw_file_data,
	mutable_buffer in_out_unmanaged_resource_buffer
)
{
	// FUNCTION BODY[0x652200]
	VOSTOK_UNREFERENCED_PARAMETER		(in_out_unmanaged_resource_buffer);
	memory::chunk_reader chunk		( (pcbyte)raw_file_data.c_ptr(), raw_file_data.size(), memory::chunk_reader::chunk_type_sequential );

	u16 type						= model_factory::get_model_type( chunk );

	user_render_surface* surface 	= static_cast_checked<user_render_surface*>(model_factory::create_render_surface( type ));
	R_ASSERT						( surface );
	surface->load_from_chunk_data	( chunk );

	user_render_model_instance* created_resource	= NEW(user_render_model_instance);
	created_resource->assign_surface				( surface );

	// TODO: pass correct sizeof, not sizeof of a base class
	in_out_query.set_unmanaged_resource			( created_resource, resources::nocache_memory, sizeof(user_render_model_instance) );
	in_out_query.finish_query					( result_success );
}

void user_mesh_cook::destroy_resource( resources::unmanaged_resource* resource )
{
	// FUNCTION BODY[0x650f30]
	user_render_model_instance* instance = static_cast_checked<user_render_model_instance*>( resource );
	R_ASSERT						( instance );

	model_factory::destroy_render_surface	( instance->m_surface );

	DELETE									( resource );
}

static_render_model_instance_cook::static_render_model_instance_cook( ) :
	resources::translate_query_cook(
		resources::static_render_model_instance_class,
		reuse_false,
		use_current_thread_id
	)
{
	// FUNCTION BODY[0x650d80]
}

void static_render_model_instance_cook::translate_query( resources::query_result_for_cook& parent )
{
	// FUNCTION BODY[0x652e20]
	fs_new::virtual_path_string render_path = parent.get_requested_path();
	resources::query_resource(	render_path.c_str(),
					resources::static_render_model_class,
					boost::bind(&static_render_model_instance_cook::on_sub_resources_loaded, this, _1),
					render::g_allocator,
					NULL,
					&parent);
}

void static_render_model_instance_cook::on_sub_resources_loaded( resources::queries_result& data )
{
	// FUNCTION BODY[0x651b20]
	resources::query_result_for_cook* parent = data.get_parent_query();

	if (!data.is_successful())
	{
		parent->finish_query( result_error );
		return;
	}

	// TODO: Iterate lods
	static_render_model_ptr v			= static_cast_resource_ptr<static_render_model_ptr>(data[0].get_unmanaged_resource());

	static_render_model_instance* created_resource = NEW(static_render_model_instance);
	created_resource->assign_original		( v );

	parent->set_unmanaged_resource			( created_resource, resources::nocache_memory, sizeof(static_render_model_instance) );
	parent->finish_query					( result_success );
}

void static_render_model_instance_cook::delete_resource( resources::resource_base* resource )
{
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
	// FUNCTION BODY[0x650d10]
}

void skeleton_render_model_instance_cook::translate_query( resources::query_result_for_cook& parent )
{
	// FUNCTION BODY[0x652d60]
	fs_new::virtual_path_string render_path = parent.get_requested_path();
	resources::query_resource(	render_path.c_str(),
					resources::skeleton_render_model_class,
					boost::bind(&skeleton_render_model_instance_cook::on_sub_resources_loaded, this, _1),
					render::g_allocator,
					NULL,
					&parent);
}

void skeleton_render_model_instance_cook::on_sub_resources_loaded( resources::queries_result& data )
{
	// FUNCTION BODY[0x651a10]
	resources::query_result_for_cook* parent	=	data.get_parent_query();

	if (!data.is_successful())
	{
		parent->finish_query( result_error );
		return;
	}

	skeleton_render_model_ptr v					= static_cast_resource_ptr<skeleton_render_model_ptr>(data[0].get_unmanaged_resource());
	skeleton_render_model_instance* created_resource = NEW(skeleton_render_model_instance);
	created_resource->assign_original			( v );

	parent->set_unmanaged_resource				( created_resource, resources::nocache_memory, sizeof(skeleton_render_model_instance) );
	parent->finish_query						( result_success );
}

void skeleton_render_model_instance_cook::delete_resource( resources::resource_base* resource )
{
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
	// FUNCTION BODY[0x651980]
}

s32 cook_intermediate_data::find_surface_index( pcstr )
{
	// claude@NOTE: no legacy ancestor - legacy cook_intermediate_data has only find_material_index/register_models; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x650ca0]
	return 0;
}

s32 cook_intermediate_data::find_material_index( pcstr surface_name )
{
	// FUNCTION BODY[0x650fb0]
	for(u32 idx=0; idx<m_surface_materials.size(); ++idx)
		if(strings::equal(m_surface_materials[idx], surface_name))
			return idx;

	LOG_WARNING("material not found %s", surface_name );
	return -1;
}

void cook_intermediate_data::register_models( vfs::vfs_iterator const& fs_it )
{
	// FUNCTION BODY[0x651830]
	R_ASSERT			( m_num_render_models==0 );
	R_ASSERT			( assets==NULL );

	u32 const fs_it_children_count	=	fs_it.get_children_count();
	if(fs_it.is_end() || 0==fs_it_children_count)
		return;

	vfs::vfs_iterator it	= fs_it.children_begin();
	vfs::vfs_iterator end	= fs_it.children_end();

	pcstr* snames = (pcstr*)ALLOCA( fs_it_children_count * sizeof(pcstr) );// max

	for( ; it != end; ++it)
	{
		if( it.is_folder() )
		{
			snames[m_num_render_models] = it.get_name();
			++m_num_render_models;
		}
	}

	assets				= NEW_ARRAY( model_asset, m_num_render_models );

	for(u32 i=0; i<m_num_render_models; ++i)
		assets[i].m_surface_name = snames[i];
}

grass_render_model_cook::grass_render_model_cook( ) :
	render_model_cook( resources::grass_render_model_class )
{
	// claude@NOTE: no legacy ancestor - legacy had no grass_render_model_cook class (grass was a class-id branch inside render_model_cook); matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x650c80]
}

void grass_render_model_cook::translate_query( resources::query_result_for_cook& )
{
	// claude@NOTE: no legacy ancestor - legacy had no grass_render_model_cook class; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x653880]
}

render_model_cook::render_model_cook( resources::class_id_enum model_type ) :
	resources::translate_query_cook( model_type, reuse_true, use_current_thread_id )
{
	// FUNCTION BODY[0x650c30]
}

void render_model_cook::translate_query( resources::query_result_for_cook& parent )
{
	// FUNCTION BODY[0x6536f0]
	fs_new::virtual_path_string model_path	= parent.get_requested_path();
	int idx						= model_path.find("/render");
	R_ASSERT					(-1!=idx);
	model_path.set_length		( idx );

	cook_intermediate_data* data	= NEW(cook_intermediate_data)(model_path, &parent);

	fs_new::virtual_path_string render_path;
	render_path.assignf("%s/render", model_path.c_str());

	resources::query_vfs_iterator(
		render_path.c_str(),
		boost::bind(&render_model_cook::on_fs_iterator_ready_submeshes, this, data, _1),
		g_allocator,
		resources::recursive_true,
		& parent
		);
}

void render_model_cook::on_fs_iterator_ready_submeshes(
	cook_intermediate_data* cook_data,
	vfs::vfs_locked_iterator const& fs_it
)
{
	// FUNCTION BODY[0x653320]
	fs_new::virtual_path_string	render_dir  = cook_data->root_model_path;
	render_dir.append					( "/render" );

	cook_data->register_models			( fs_it );

	if(0==cook_data->m_num_render_models)
	{
		cook_data->status_failed			= true;
		cook_data->render_model_data_ready	= true;

		query_materail_effects	( cook_data );

		return;
	}

	u32 requests_composite_model		= 1; // export_properties

	if( get_class_id() == resources::skeleton_render_model_class )
		++requests_composite_model; // skeleton also queries bind_pose

	const u32 requests_per_model		= 2;// converted_model+export_properties(tmp

	u32 num_requests	= cook_data->m_num_render_models * requests_per_model + requests_composite_model;

	resources::request*	requests	= NEW_ARRAY( resources::request, num_requests );
	fs_new::virtual_path_string* paths	= NEW_ARRAY( fs_new::virtual_path_string, num_requests );

	u32 request_index				= 0;
	u32 model_index					= 0;

	paths[request_index].assignf	( "%s/export_properties", render_dir.c_str() );
	requests[request_index].id		= resources::binary_config_class;
	requests[request_index].path	= paths[request_index].c_str();
	++request_index;

	vfs::vfs_iterator it			= fs_it.children_begin();
	vfs::vfs_iterator it_e			= fs_it.children_end();

	for ( ; it!=it_e; ++it )
	{
		if(!it.is_folder())
			continue;

		pcstr sname									= it.get_name();
		cook_data->assets[model_index].m_surface_name	= sname;

		paths[request_index].assignf	( "%s/%s/converted_model", render_dir.c_str(), sname );
		requests[request_index].id		= resources::converted_model_class;
		requests[request_index].path	= paths[request_index].c_str();
		++request_index;

		paths[request_index].assignf	( "%s/%s/export_properties", render_dir.c_str(), sname );
		requests[request_index].id		= resources::binary_config_class;
		requests[request_index].path	= paths[request_index].c_str();
		++request_index;
		++model_index;
	}

	if( get_class_id() == resources::skeleton_render_model_class )
	{
		paths[request_index].assignf	( "%s/bind_pose", render_dir.c_str(), model_index );
		requests[request_index].id		= resources::raw_data_class;
		requests[request_index].path	= paths[request_index].c_str();
		++request_index;
	}

	ASSERT( num_requests == request_index);

	resources::query_resources(
		requests,
		num_requests,
		boost::bind(&render_model_cook::on_subresources_loaded, this, _1, cook_data),
		vostok::render::g_allocator,
		0,
		cook_data->parent_query
		);

	DELETE_ARRAY		( paths );
	DELETE_ARRAY		( requests );
}

// // model editor created config
void render_model_cook::on_model_settings_loaded(
	resources::queries_result& data,
	cook_intermediate_data* cook_data
)
{
	// FUNCTION BODY[0x6528b0]
	// claude@NOTE: legacy name was on_material_settings_loaded; canonical renamed it.
	if( !data.is_successful() )
	{
		cook_data->material_data_ready	= true;
		if(cook_data->render_model_data_ready)
			query_materail_effects(cook_data);

		return;
	}

	configs::binary_config_ptr settings_config		= static_cast_resource_ptr<configs::binary_config_ptr>(data[0].get_unmanaged_resource());
	configs::binary_config_value root				= settings_config->get_root();

	u32 num_render_models = 0;
	bool bvalid = root.value_exists("material_settings");
	configs::binary_config_value msettings;
	if(bvalid)
	{
		msettings			= root["material_settings"];

		num_render_models	= msettings.size();
		bvalid				&= (cook_data->m_num_render_models==num_render_models);
	}

	if(bvalid)
	{
		for(u32 model_index=0; model_index<cook_data->m_num_render_models; ++model_index)
		{
			pcstr sname					= cook_data->assets[model_index].m_surface_name.c_str();
			if(!msettings.value_exists(sname))
			{
				bvalid = false;
				break;
			}
		}
	}

	if(!bvalid)
	{
		LOG_ERROR("Incompatible model settings for %s", cook_data->root_model_path.c_str());
		cook_data->material_settings_valid		= false;
		cook_data->material_data_ready			= true;

		if(cook_data->render_model_data_ready)
			query_materail_effects(cook_data);

		return;
	}
	cook_data->material_settings_valid			= true;

	resources::request*	mat_requests	= NEW_ARRAY( resources::request, num_render_models );

	for(u32 model_index=0; model_index<cook_data->m_num_render_models; ++model_index)
	{
		pcstr sname							= cook_data->assets[model_index].m_surface_name.c_str();
		cook_data->m_surface_materials.push_back(sname);
		pcstr material_name					= (pcstr)msettings[sname]["material_name"];
		bool has_mtl						= strings::length(material_name)>0;
		mat_requests[model_index].id		= has_mtl ? resources::material_class : resources::unknown_data_class;
		mat_requests[model_index].path		= has_mtl ? material_name : "a";
	}

	R_ASSERT(cook_data->m_surface_materials.size()==cook_data->m_num_render_models);

	resources::query_resources(
		mat_requests,
		num_render_models,
		boost::bind(&render_model_cook::on_materials_loaded, this, _1, cook_data),
		vostok::render::g_allocator,
		0,
		cook_data->parent_query
		);

	DELETE_ARRAY		( mat_requests );
}

// verts+indices
void render_model_cook::on_subresources_loaded(
	resources::queries_result& data,
	cook_intermediate_data* cook_data
)
{
	// FUNCTION BODY[0x652ee0]
	if (!data.is_successful())
	{
		cook_data->status_failed = true;
		query_materail_effects	( cook_data );
		return;
	}

	u32 num_models						= cook_data->m_num_render_models;

	// Requesting properties it must be the first request so we can get it by ind 0.
	u32 request_index					= 0;
	configs::binary_config_ptr			config_ptr			   = static_cast_checked<configs::binary_config *>( data[0].get_unmanaged_resource().c_ptr() );
	configs::binary_config_value const&	mesh_properties_config = config_ptr->get_root();
	++request_index;

	u16 model_type						= u16(mesh_properties_config["type"]);

	if (get_class_id() == resources::grass_render_model_class)
		model_type									= mt_grass_mesh;

	render_model* model					= model_factory::create_render_model( model_type );
	R_ASSERT							( model );

	model->load_properties				( mesh_properties_config );

	cook_data->result_model				= model;

	for(u32 model_index=0; model_index<num_models; model_index++)
	{
		memory::writer destination(render::g_allocator);

		cook_data->assets[model_index].converted_model_buffer = data[request_index].get_managed_resource( );
		++request_index;

		cook_data->assets[model_index].export_properties_config = static_cast_resource_ptr<configs::binary_config_ptr>( data[request_index].get_unmanaged_resource() );
		++request_index;
	}

	if( get_class_id() == resources::skeleton_render_model_class )
	{
		resources::pinned_ptr_const<u8> bind_pose_ptr(data[request_index].get_managed_resource( ));
		memory::reader bones_reader		( bind_pose_ptr.c_ptr(), bind_pose_ptr.size() );

		skeleton_render_model* skeleton_model = static_cast_checked<skeleton_render_model*>(model);
		skeleton_model->load_bones		( bones_reader );
	}

	cook_data->render_model_data_ready = true;

	if (cook_data->material_data_ready)
	{
		query_materail_effects			( cook_data );
	}

	fs_new::virtual_path_string material_settings_path;
	material_settings_path.assignf("%s/settings", cook_data->root_model_path.c_str() );

	resources::query_resource(
		material_settings_path.c_str(),
		resources::binary_config_class,
		boost::bind(&render_model_cook::on_model_settings_loaded, this, _1, cook_data),
		vostok::render::g_allocator,
		0,
		cook_data->parent_query
	);

}

void render_model_cook::on_materials_loaded(
	resources::queries_result& data,
	cook_intermediate_data* cook_data
)
{
	// FUNCTION BODY[0x652790]
	if(cook_data->status_failed)
	{
		query_materail_effects	( cook_data );
		return;
	}

	for (u32 i=0; i<data.size(); ++i)
	{
		if(data[i].is_successful())
			cook_data->assets[i].material = data[i].get_unmanaged_resource();
	}

	cook_data->material_data_ready	= true;

	if(cook_data->render_model_data_ready)
		query_materail_effects(cook_data);
}

enum_vertex_input_type mesh_type_to_vertex_input_type( mesh_type_enum type )
{
	// FUNCTION BODY[0x650ad0]
	switch (type)
	{
		case mt_static_mesh:
		case mt_static_submesh:
			return static_mesh_vertex_input_type;
		// claude@NOTE: legacy returned the retired single skeletal_mesh_vertex_input_type
		// for every skinned type; mapped onto the per-weight-count canonical enum. The
		// canonical mt_static_submesh_colored / mt_user_mesh_editable arms are unverified.
		case mt_static_submesh_colored:
			return static_mesh_vertex_colored_input_type;
		case mt_skinned_mesh:
		case mt_skinned_submesh_4w:
			return skeletal_4_bones_mesh_vertex_input_type;
		case mt_skinned_submesh_3w:
			return skeletal_3_bones_mesh_vertex_input_type;
		case mt_skinned_submesh_2w:
			return skeletal_2_bones_mesh_vertex_input_type;
		case mt_skinned_submesh_1w:
			return skeletal_1_bones_mesh_vertex_input_type;
		case mt_user_mesh_wire:
			return wires_vertex_input_type;
		case mt_grass_mesh:
			return grassmesh_vertex_input_type;
		default: NODEFAULT(return unknown_vertex_input_type);
	};
}

fs_new::virtual_path_string get_material_effects_instance_request_path(
	material_ptr mtl,
	enum_vertex_input_type vertex_input_type
)
{
	// FUNCTION BODY[0x6517b0]
	VOSTOK_UNREFERENCED_PARAMETER(vertex_input_type);
/*	fs_new::virtual_path_string					result;
	result.assignf					(
		"%s/%s",
		mtl ? mtl->get_material_name() : "",
		vertex_input_type_to_string(vertex_input_type)
	);
*/
	fs_new::virtual_path_string		result;
	result.assignf					(
		"%s",
		mtl ? mtl->get_material_name() : ""
	);
	return							result;
}

void render_model_cook::query_materail_effects( cook_intermediate_data* cook_data )
{
	// FUNCTION BODY[0x652300]
	resources::query_result_for_cook* parent_query = cook_data->parent_query;

	if (cook_data->status_failed)
	{
		parent_query->finish_query	(result_error);

		if (cook_data->assets)
			DELETE_ARRAY			(cook_data->assets);

		DELETE						(cook_data);

		return;
	}

	using namespace resources;

	u32 const num_requests								= cook_data->m_num_render_models;
	fs_new::virtual_path_string* pathes					= (fs_new::virtual_path_string*)	MALLOC(sizeof(fs_new::virtual_path_string) * num_requests, "");
	request* requests									= (request*)			MALLOC(sizeof(request) * num_requests, "");
	user_data_variant* user_data_variants				= (user_data_variant*)	MALLOC(sizeof(user_data_variant) * num_requests, "");
	user_data_variant** user_data_variants_ptrs			= (user_data_variant**)	MALLOC(sizeof(user_data_variant*) * num_requests, "");

	for (u32 model_index = 0; model_index < cook_data->m_num_render_models; ++model_index)
	{
		configs::binary_config_value const& properties	= cook_data->assets[model_index].export_properties_config->get_root();
		mesh_type_enum						model_type	= (mesh_type_enum)u16(properties["type"]);

		if (get_class_id() == resources::grass_render_model_class)
			model_type									= mt_grass_mesh;

		pcstr								sg_name		= cook_data->assets[model_index].m_surface_name.c_str();
		int									mtl_idx		= cook_data->find_material_index(sg_name);

		material_ptr						mtl			= 0;
		if (mtl_idx != -1)
			mtl											= static_cast_resource_ptr<material_ptr>(cook_data->assets[mtl_idx].material);

		*new(&pathes[model_index])fs_new::virtual_path_string		= get_material_effects_instance_request_path(
			mtl,
			mesh_type_to_vertex_input_type(model_type)
		);
		resources::request* request						= new(&requests[model_index])resources::request;
		request->path									= pathes[model_index].length() ? pathes[model_index].c_str() : "nomaterial";
		request->id										= material_effects_instance_class;
		user_data_variants_ptrs[model_index]			= new(&user_data_variants[model_index])user_data_variant;
		user_data_variants_ptrs[model_index]->set		(
			NEW(material_effects_instance_cook_data)(
				mesh_type_to_vertex_input_type(model_type),
				static_cast_resource_ptr<resources::unmanaged_resource_ptr>(mtl)
			)
		);
	}

	query_resources										(
		requests,
		num_requests,
		boost::bind(
			&render_model_cook::finish_model_creation,
			this,
			_1,
			cook_data
		),
		vostok::render::g_allocator,
		(resources::user_data_variant const**)user_data_variants_ptrs,
		cook_data->parent_query
	);
	FREE												(user_data_variants_ptrs);
	FREE												(user_data_variants);
	FREE												(requests);
	FREE												(pathes);
}

void arrange_surfaces_by_lod( cook_intermediate_data*, model_lods_descriptor*& )
{
	// claude@NOTE: no legacy ancestor - the model_lods_descriptor pipeline is new-in-target; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x651110]
}

// STATE[STUB]
// claude@NOTE: legacy body appended surfaces via render_model::append_surface, which the
// canonical tree replaced with set_children + arrange_surfaces_by_lod (both target-only);
// diverged - legacy seed kept in temp/render_legacy.
void render_model_cook::finish_model_creation(
	resources::queries_result&,
	cook_intermediate_data*
)
{
	// FUNCTION BODY[0x651ca0]
}

void render_model_cook::delete_resource( resources::resource_base* resource )
{
	// FUNCTION BODY[0x650e70]
	render_model* model					= static_cast_checked<render_model*>(resource);
	R_ASSERT							( model );
	model_factory::destroy_render_model	( model );
}

} // namespace render
} // namespace vostok
