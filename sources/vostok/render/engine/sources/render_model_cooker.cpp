#include "pch.h"
#include "render_model_cooker.h"
#include <vostok/render/facade/material_effects_instance_cook_data.h>
#include "material_effects_instance.h"
#include "model_manager.h"
#include "render_model_grass.h"
#include "render_model_skeleton.h"
#include "render_model_static.h"
#include "render_model_user.h"
#include "render_surface.h"

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
	resources::register_cook( this );
}

mutable_buffer user_mesh_cook::allocate_resource(
	resources::query_result_for_cook& in_query,
	const_buffer raw_file_data,
	bool file_exist
)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( in_query, &raw_file_data, file_exist );
	return mutable_buffer::zero		( );
}

void user_mesh_cook::deallocate_resource( pvoid buffer )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( buffer );
}

void user_mesh_cook::create_resource(
	resources::query_result_for_cook& in_out_query,
	const_buffer raw_file_data,
	mutable_buffer in_out_unmanaged_resource_buffer
)
{
	VOSTOK_UNREFERENCED_PARAMETER		(in_out_unmanaged_resource_buffer);
	memory::chunk_reader chunk		( (pcbyte)raw_file_data.c_ptr(), raw_file_data.size(), memory::chunk_reader::chunk_type_sequential );

	u16 type						= model_factory::get_model_type( chunk );

	user_render_surface* surface 	= static_cast_checked<user_render_surface*>(model_factory::create_render_surface( type ));
	R_ASSERT						( surface );
	surface->load_from_chunk_data	( chunk );

	user_render_model_instance* created_resource	= NEW(user_render_model_instance);
	created_resource->assign_surface				( surface );

	in_out_query.set_unmanaged_resource			( created_resource, resources::nocache_memory, sizeof(user_render_model_instance) );
	in_out_query.finish_query					( result_success );
}

void user_mesh_cook::destroy_resource( resources::unmanaged_resource* resource )
{
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
	resources::register_cook( this );
}

void static_render_model_instance_cook::translate_query( resources::query_result_for_cook& parent )
{
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
	resources::query_result_for_cook* parent = data.get_parent_query();

	if (!data.is_successful())
	{
		parent->finish_query( result_error );
		return;
	}

	static_render_model_ptr v			= static_cast_resource_ptr<static_render_model_ptr>(data[0].get_unmanaged_resource());

	static_render_model_instance* created_resource = NEW(static_render_model_instance);
	if ( parent->user_data( ) )
	{
		configs::binary_config_value cfg;
		parent->user_data( )->try_get( cfg );
		created_resource->add_sectors_holder( cfg );
	}

	created_resource->assign_original		( v );

	parent->set_unmanaged_resource			( created_resource, resources::nocache_memory, sizeof(static_render_model_instance) );
	parent->finish_query					( result_success );
}

void static_render_model_instance_cook::delete_resource( resources::resource_base* resource )
{
	static_render_model_instance* instance = static_cast_checked<static_render_model_instance*>(resource);
	DELETE( instance );
}

skeleton_render_model_instance_cook::skeleton_render_model_instance_cook( ) :
	resources::translate_query_cook(
		resources::skeleton_render_model_instance_class,
		reuse_false,
		use_current_thread_id
)
{
	resources::register_cook( this );
}

void skeleton_render_model_instance_cook::translate_query( resources::query_result_for_cook& parent )
{
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
	skeleton_render_model_instance* instance = static_cast_checked<skeleton_render_model_instance*>(resource);
	DELETE( instance );
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
	assets					( 0 ),
	m_num_render_models		( 0 )
{
}

s32 cook_intermediate_data::find_surface_index( pcstr surface_name )
{
	for ( u32 idx = 0; idx < m_num_render_models; ++idx )
		if ( strings::equal( assets[idx].m_surface_name.c_str( ), surface_name ) )
			return idx;

	return -1;
}

s32 cook_intermediate_data::find_material_index( pcstr surface_name )
{
	for(u32 idx=0; idx<m_surface_materials.size(); ++idx)
		if(strings::equal(m_surface_materials[idx], surface_name))
			return idx;

#line 203
	LOG_WARNING("material not found %s", surface_name );
#line 202
	return -1;
}

void cook_intermediate_data::register_models( vfs::vfs_iterator const& fs_it )
{
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
}

void grass_render_model_cook::translate_query( resources::query_result_for_cook& parent )
{
	pcstr model_name = parent.get_requested_path( );

	fs_new::virtual_path_string model_path;
	model_path.assignf( "%s.model", model_name );

	cook_intermediate_data* data = NEW( cook_intermediate_data )( model_path, &parent );

	fs_new::virtual_path_string render_path;
	render_path.assignf( "resources/models/%s/render", model_path.c_str( ) );

	resources::query_vfs_iterator(
		render_path.c_str( ),
		boost::bind(
			&grass_render_model_cook::on_fs_iterator_ready_submeshes,
			this,
			data,
			_1
		),
		g_allocator,
		resources::recursive_true,
		&parent
	);
}

render_model_cook::render_model_cook( resources::class_id_enum model_type ) :
	resources::translate_query_cook( model_type, reuse_true, use_current_thread_id )
{
	resources::register_cook( this );
}

void render_model_cook::translate_query( resources::query_result_for_cook& parent )
{
	fs_new::virtual_path_string model_path	= parent.get_requested_path();
	int idx						= model_path.find("/render");
	R_ASSERT					(-1!=idx);
	model_path.set_length		( idx );

	cook_intermediate_data* data	= NEW(cook_intermediate_data)(model_path, &parent);

	fs_new::virtual_path_string render_path;
	render_path.assignf("resources/models/%s/render", model_path.c_str());

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

	paths[request_index].assignf	( "resources/models/%s/export_properties", render_dir.c_str() );
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

		paths[request_index].assignf	( "resources/models/%s/%s/converted_model", render_dir.c_str(), sname );
		requests[request_index].id		= resources::converted_model_class;
		requests[request_index].path	= paths[request_index].c_str();
		++request_index;

		paths[request_index].assignf	( "resources/models/%s/%s/export_properties", render_dir.c_str(), sname );
		requests[request_index].id		= resources::binary_config_class;
		requests[request_index].path	= paths[request_index].c_str();
		++request_index;
		++model_index;
	}

	if( get_class_id() == resources::skeleton_render_model_class )
	{
		paths[request_index].assignf	( "resources/models/%s/bind_pose", render_dir.c_str(), model_index );
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
	if( !data.is_successful() )
	{
		cook_data->material_data_ready	= true;
		if(cook_data->render_model_data_ready)
			query_materail_effects(cook_data);

		return;
	}

	cook_data->model_settings_config = static_cast_resource_ptr< configs::binary_config_ptr >(
		data[0].get_unmanaged_resource( )
	);
	configs::binary_config_value root = cook_data->model_settings_config->get_root( );

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
// Preserve target line metadata.
void render_model_cook::on_subresources_loaded(
	resources::queries_result& data,
	cook_intermediate_data* cook_data
)
#line 465
{
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
#line 497
		cook_data->assets[model_index].converted_model_buffer = data[request_index].get_managed_resource( );
		++request_index;

		cook_data->assets[model_index].export_properties_config = static_cast_resource_ptr<configs::binary_config_ptr>( data[request_index].get_unmanaged_resource() );
		++request_index;
	}

	if( get_class_id() == resources::skeleton_render_model_class )
	{
		resources::pinned_ptr_const<u8> bind_pose_ptr(data[request_index].get_managed_resource( ));
		memory::reader bones_reader		( bind_pose_ptr.c_ptr(), bind_pose_ptr.size() );

#line 509
		skeleton_render_model* skeleton_model = static_cast_checked<skeleton_render_model*>(model);
		skeleton_model->load_bones		( bones_reader );
	}

#line 515
	cook_data->render_model_data_ready = true;
	if (cook_data->material_data_ready)
		query_materail_effects			( cook_data );

#line 520
	fs_new::virtual_path_string material_settings_path;
	material_settings_path.assignf("resources/models/%s/settings", cook_data->root_model_path.c_str() );

	resources::query_resource(
		material_settings_path.c_str(),
		resources::binary_config_class,
		boost::bind(&render_model_cook::on_model_settings_loaded, this, _1, cook_data),
		vostok::render::g_allocator,
		0,
		cook_data->parent_query
	);

}

#line 533
void render_model_cook::on_materials_loaded(
	resources::queries_result& data,
	cook_intermediate_data* cook_data
)
{
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

static enum_vertex_input_type mesh_type_to_vertex_input_type( mesh_type_enum type )
{
	switch (type)
	{
		case mt_static_mesh:
		case mt_static_submesh:
			return static_mesh_vertex_input_type;
		case mt_static_submesh_colored:
			return static_mesh_vertex_colored_input_type;
		case mt_skinned_submesh_1w:
			return skeletal_1_bones_mesh_vertex_input_type;
		case mt_skinned_submesh_2w:
			return skeletal_2_bones_mesh_vertex_input_type;
		case mt_skinned_submesh_3w:
			return skeletal_3_bones_mesh_vertex_input_type;
		case mt_skinned_mesh:
		case mt_skinned_submesh_4w:
			return skeletal_4_bones_mesh_vertex_input_type;
		case mt_user_mesh_wire:
			return wires_vertex_input_type;
		case mt_grass_mesh:
			return grassmesh_vertex_input_type;
		default: NODEFAULT(return unknown_vertex_input_type);
	};
}

static fs_new::virtual_path_string get_material_effects_instance_request_path(
	material_ptr mtl,
	enum_vertex_input_type vertex_input_type
)
{
	VOSTOK_UNREFERENCED_PARAMETER(vertex_input_type);
	fs_new::virtual_path_string result;
	result.assignf("%s", mtl ? mtl->get_material_name() : "");
	return result;
}

void render_model_cook::query_materail_effects( cook_intermediate_data* cook_data )
{
	#line 597
	resources::query_result_for_cook* parent_query = cook_data->parent_query;
	if (cook_data->status_failed)
	{
		parent_query->finish_query	(result_error);

		if (cook_data->assets)
			DELETE_ARRAY			(cook_data->assets);

		DELETE						(cook_data);
		return;
	}
	// Three retail-only source lines precede the request construction.
	#line 610
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

		*new(&pathes[model_index])fs_new::virtual_path_string = get_material_effects_instance_request_path(mtl, mesh_type_to_vertex_input_type(model_type));
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

	query_resources(
		requests,
		num_requests,
		boost::bind(&render_model_cook::finish_model_creation, this, _1, cook_data),
		vostok::render::g_allocator,
		(resources::user_data_variant const**)user_data_variants_ptrs,
		cook_data->parent_query
	);
	FREE(user_data_variants_ptrs);
	FREE(user_data_variants);
	FREE(requests);
	FREE(pathes);
}

void arrange_surfaces_by_lod(
	cook_intermediate_data*	cook_data,
	model_lods_descriptor*&	lods_descriptor
)
{
	#line 669
	u8 result_lod_surfaces_count[3] = { 0 };
	#line 675
	pbyte result_lod_surfaces[3] = { 0 };

	if ( !cook_data->model_settings_config ) {
		lods_descriptor = NEW( model_lods_descriptor );
		#line 679
		for ( u8 lod_index = 0; lod_index < 3; ++lod_index )
		{
			lods_descriptor->m_lod_surfaces_count[lod_index] = 0;
			lods_descriptor->m_lod_surfaces[lod_index] = 0;
		}
	#line 681
		LOG_ERROR(
			"Incorrect model settings for %s",
			cook_data->parent_query->get_requested_path( )
		);
		return;
	}

	#line 689
	configs::binary_config_value t_root = cook_data->model_settings_config->get_root( );
	#line 690
	pcstr lods[3] = { "LOD0", "LOD1", "LOD2" };
	#line 690
	if ( !t_root.value_exists( "lod_hierrarchy" ) )
		return;
	#line 693
	configs::binary_config_value t_lods = t_root["lod_hierrarchy"];
	#line 695
	for ( u8 lod_index = 0; lod_index < 3; ++lod_index )
	{
		#line 698
		if ( !t_lods.value_exists( lods[lod_index] )
			 #line 699
			 || !t_lods[lods[lod_index]].value_exists( "surfaces" ) )
			continue;

		#line 702
		configs::binary_config_value t_surfaces = t_lods[lods[lod_index]]["surfaces"];
		#line 705
		configs::binary_config_value const* it = t_surfaces.begin( );
		configs::binary_config_value const* it_e = t_surfaces.end( );
		u8 surfaces_count = (u8)t_surfaces.size( );
		#line 710
		result_lod_surfaces[lod_index] = NEW_ARRAY( u8, surfaces_count );
		#line 712
		u8 i = 0;
		#line 714
		for ( ; it != it_e; ++it )
		{
			#line 716
			pcstr surface_name = t_surfaces.type == configs::t_table_indexed ? (pcstr)*it : it->key( );
			#line 718
			s32 index = cook_data->find_surface_index( surface_name );
			#line 719
			if ( index == -1 )
			{
				#line 722
				--surfaces_count;
				#line 719
				LOG_ERROR(
					"Incorrect model LOD settings for %s",
					cook_data->parent_query->get_requested_path( )
				);
			}
			#line 723
			else
				#line 725
				result_lod_surfaces[lod_index][i++] = (u8)index;
			#line 726
		}

		#line 729
		if ( surfaces_count )
			#line 732
			result_lod_surfaces_count[lod_index] = surfaces_count;
	}

	#line 740
	lods_descriptor = (model_lods_descriptor*)MALLOC( sizeof( model_lods_descriptor ) + result_lod_surfaces_count[0] + result_lod_surfaces_count[1] + result_lod_surfaces_count[2], "" );
	#line 741
	new ( lods_descriptor ) model_lods_descriptor;

	#line 743
	if ( t_root.value_exists( "lod_switching" ) )
	{
		#line 745
		configs::binary_config_value t_lod_switching = t_root["lod_switching"];
		#line 746
		lods_descriptor->m_lod_calc_type = (u8)t_root["lod_switching"]["type"];
		#line 747
		lods_descriptor->m_lod_params_default = (bool)t_root["lod_switching"]["default_params"];

		#line 748
		if ( !lods_descriptor->m_lod_params_default )
		{
			#line 750
			lods_descriptor->m_lod_custom_params[0] = (float)t_root["lod_switching"]["param0"];
			#line 751
			lods_descriptor->m_lod_custom_params[1] = (float)t_root["lod_switching"]["param1"];

			#line 752
			if ( t_root["lod_switching"].value_exists( "param2" ) )
				#line 753
				lods_descriptor->m_lod_custom_params[2] = (float)t_root["lod_switching"]["param2"];
			else
				#line 755
				lods_descriptor->m_lod_custom_params[2] = lods_descriptor->m_lod_calc_type == 0 ? 240.f : math::epsilon_3;
		}
	}

	#line 760
	u32 surfaces_shift = 0;
	for ( u8 lod_index = 0; lod_index < 3; ++lod_index )
	{
		#line 763
		lods_descriptor->m_lod_surfaces_count[lod_index] = result_lod_surfaces_count[lod_index];
		lods_descriptor->m_lod_surfaces[lod_index] =
			(pbyte)lods_descriptor + sizeof( model_lods_descriptor ) + surfaces_shift;
		#line 769
		memory::copy(
			lods_descriptor->m_lod_surfaces[lod_index],
			result_lod_surfaces_count[lod_index],
			result_lod_surfaces[lod_index],
			result_lod_surfaces_count[lod_index]
		);
		#line 771
		surfaces_shift += result_lod_surfaces_count[lod_index];
		#line 772
		DELETE_ARRAY( result_lod_surfaces[lod_index] );
	}
}

void render_model_cook::finish_model_creation(
	resources::queries_result&	data_material_effects,
	cook_intermediate_data*		cook_data
)
{
	resources::query_result_for_cook* parent_query = cook_data->parent_query;

	if ( cook_data->status_failed )
	{
		parent_query->finish_query( result_error );

		if ( cook_data->assets )
			DELETE_ARRAY( cook_data->assets );

		DELETE( cook_data );

		return;
	}

	render_surface** surfaces = ALLOC( render_surface*, cook_data->m_num_render_models );

	for ( u32 model_index = 0; model_index < cook_data->m_num_render_models; ++model_index ) {
		configs::binary_config_ptr prop_config_ptr = cook_data->assets[model_index].export_properties_config;
		configs::binary_config_value const& properties = prop_config_ptr->get_root( );
		mesh_type_enum model_type = (mesh_type_enum)(u16)properties["type"];

		if ( get_class_id( ) == resources::grass_render_model_class )

			model_type = mt_grass_mesh;
		resources::pinned_ptr_const< u8 > converted_model_ptr(
			cook_data->assets[model_index].converted_model_buffer
		);

		render_surface* surface = model_factory::create_render_surface( model_type );

		pcstr sg_name = cook_data->assets[model_index].m_surface_name.c_str( );
		surface->m_render_geometry.shading_group_name = sg_name;

		bool material_result = false;

		if ( cook_data->material_settings_valid )
		{
			s32 material_index = cook_data->find_material_index( sg_name );
			if ( material_index != -1 )
			{
				resources::unmanaged_resource_ptr m =
					cook_data->assets[material_index].material;
				if ( m )
				{
					material_result = data_material_effects[model_index].is_successful( );
					if ( material_result )
						surface->set_material_effects(
							static_cast_resource_ptr< material_effects_instance_ptr >(
								data_material_effects[model_index].get_unmanaged_resource( )
							),
							static_cast_resource_ptr< material_ptr >(
								m
							)->get_material_name( )
						);
				}
			}
		}

		if ( !material_result )
		{
			surface->set_default_material( );
			LOG_ERROR("material not loaded for %s : %s", cook_data->root_model_path.c_str( ), sg_name);
		}
		memory::chunk_reader model_reader(converted_model_ptr.c_ptr( ), converted_model_ptr.size( ), memory::chunk_reader::chunk_type_sequential);
		surface->load( properties, model_reader );
		surfaces[model_index] = surface;
	}

	model_lods_descriptor* lods_descriptor = 0; arrange_surfaces_by_lod( cook_data, lods_descriptor );
	cook_data->result_model->set_children(surfaces, cook_data->m_num_render_models, lods_descriptor);

	parent_query->set_unmanaged_resource(cook_data->result_model.c_ptr(), resources::nocache_memory, sizeof(cook_intermediate_data));
	parent_query->finish_query( result_success );

	DELETE_ARRAY( cook_data->assets );
	DELETE( cook_data );
}

void render_model_cook::delete_resource( resources::resource_base* resource )
{
	render_model* model					= static_cast_checked<render_model*>(resource);
	R_ASSERT							( model );
	model_factory::destroy_render_model	( model );
}

} // namespace render
} // namespace vostok
