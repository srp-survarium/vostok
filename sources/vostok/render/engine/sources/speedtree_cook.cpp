// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include "speedtree_cook.h"
#include "speedtree_tree_component_billboard.h"
#include "speedtree_instance_impl.h"
#include "speedtree_tree.h"
namespace vostok {
namespace render {

speedtree_cook::speedtree_cook( ) :
	resources::translate_query_cook(
		resources::speedtree_class,
		reuse_true,
		use_current_thread_id
	)
{
}

void speedtree_cook::translate_query( resources::query_result_for_cook& parent )
{
 	fs_new::virtual_path_string		model_path;
	model_path.assignf				("resources/speedtree/%s.srt", parent.get_requested_path());

	fs_new::virtual_path_string		config_path;
	config_path.assignf				("resources/speedtree/%s.options", parent.get_requested_path());

	resources::request	requests[]={
		{model_path.c_str(), resources::raw_data_class},
		{config_path.c_str(), resources::binary_config_class}
	};
	speedtree_data* creation_data = NEW(speedtree_data)();
	creation_data->m_parent_query	= &parent;
	creation_data->m_failed			= false;

 	resources::query_resources(
		requests,
 		boost::bind	(&speedtree_cook::on_speedtree_raw_data_loaded, this, _1, creation_data ),
 		g_allocator,
		0,
		& parent
	);
}

void speedtree_cook::on_speedtree_raw_data_loaded( resources::queries_result& data, speedtree_data* creation_data )
{
 	if(!data[0].is_successful())
	{
		creation_data->m_parent_query->finish_query	( result_error );
		DELETE										( creation_data );
		return;
	}

	resources::pinned_ptr_const<u8> raw_data_ptr(data[0].get_managed_resource());

	speedtree_tree* st_tree			= NEW(speedtree_tree)(raw_data_ptr.c_ptr(), raw_data_ptr.size());

	creation_data->m_model			= st_tree;

 	if (!data[1].is_successful())
	{
		query_materail_effects		(creation_data);
		return;
	}

	configs::binary_config_ptr model_config = static_cast_resource_ptr<configs::binary_config_ptr>(data[1].get_unmanaged_resource());
	configs::binary_config_value const& root = model_config->get_root();

	resources::request materials_request[5]= {
		{strings::length(root["branch"]["material"]) ? pcstr(pcstr(root["branch"]["material"])) : "nomaterial", resources::material_class},
		{strings::length(root["frond"]["material"]) ? pcstr(pcstr(root["frond"]["material"])) : "nomaterial", resources::material_class},
		{strings::length(root["leafmesh"]["material"]) ? pcstr(pcstr(root["leafmesh"]["material"])) : "nomaterial", resources::material_class},
		{strings::length(root["leafcard"]["material"]) ? pcstr(pcstr(root["leafcard"]["material"])) : "nomaterial", resources::material_class},
		{strings::length(root["billboard"]["material"]) ? pcstr(pcstr(root["billboard"]["material"])) : "nomaterial", resources::material_class},
	};
	resources::query_resources(
		materials_request,
 		boost::bind	(&speedtree_cook::on_model_materials_loaded, this, _1, creation_data ),
 		g_allocator,
		0,
		creation_data->m_parent_query
	);
}

void speedtree_cook::on_model_materials_loaded( resources::queries_result& data, speedtree_data* d )
{
	bool has_valid_material			= false;
	for(int i=0; i<5; ++i)
	{
		if(data[i].is_successful())
		{
			d->m_materials[i]		= static_cast_resource_ptr<material_ptr>(data[i].get_unmanaged_resource());
			d->m_material_path[i]	= data[i].get_requested_path();
			has_valid_material		= true;
		}
	}

	if (has_valid_material)
		d->m_parent_query->finish_query	( result_error );
	else
		finish_model_creation		(d);
}

void speedtree_cook::query_materail_effects( speedtree_data* cook_data )
{
	cook_data->m_parent_query->finish_query( result_error );
}

void speedtree_cook::finish_model_creation( speedtree_data* d )
{
	if(d->m_failed)
	{
		d->m_parent_query->finish_query			( result_error );
	}else
	{
		speedtree_tree* st_tree = static_cast_checked<speedtree_tree*>(d->m_model.c_ptr());
		if( st_tree->m_branch_component )
			if(d->m_material_effects[0] && d->m_materials[0])
				st_tree->m_branch_component->set_material_effects(d->m_material_effects[0], d->m_materials[0]->get_material_name());
			else
				st_tree->m_branch_component->set_default_material();

		if( st_tree->m_frond_component )
			if(d->m_material_effects[1] && d->m_materials[1])
				st_tree->m_frond_component->set_material_effects(d->m_material_effects[1], d->m_materials[1]->get_material_name());
			else
				st_tree->m_frond_component->set_default_material();

		if( st_tree->m_leafmesh_component )
			if(d->m_material_effects[2] && d->m_materials[2])
				st_tree->m_leafmesh_component->set_material_effects(d->m_material_effects[2], d->m_materials[2]->get_material_name());
			else
				st_tree->m_leafmesh_component->set_default_material();

		if( st_tree->m_leafcard_component )
			if(d->m_material_effects[3] && d->m_materials[3])
				st_tree->m_leafcard_component->set_material_effects(d->m_material_effects[3], d->m_materials[3]->get_material_name());
			else
				st_tree->m_leafcard_component->set_default_material();

		if( st_tree->m_billboard_component )
			if(d->m_material_effects[4] && d->m_materials[4])
				st_tree->m_billboard_component->set_material_effects(d->m_material_effects[4], d->m_materials[4]->get_material_name());
			else
				st_tree->m_billboard_component->set_default_material();

		d->m_parent_query->set_unmanaged_resource( d->m_model.c_ptr(), resources::nocache_memory, sizeof(speedtree_tree));
		d->m_parent_query->finish_query			( result_success );
	}
	DELETE		( d );
}

void speedtree_cook::delete_resource( resources::resource_base* resource )
{
	speedtree_tree* tree = static_cast<speedtree_tree*>( resource );
	DELETE( tree );
}

speedtree_instance_cook::speedtree_instance_cook( ) :
	resources::translate_query_cook(
		resources::speedtree_instance_class,
		reuse_false,
		use_current_thread_id
	)
{
}

void speedtree_instance_cook::translate_query( resources::query_result_for_cook& parent )
{
 	resources::query_resource(
 		parent.get_requested_path(),
		resources::speedtree_class,
 		boost::bind	(&speedtree_instance_cook::on_speedtree_loaded, this, _1, &parent ),
 		vostok::render::g_allocator,
		0,
		& parent
	);
}

void speedtree_instance_cook::on_speedtree_loaded(
	resources::queries_result& data,
	resources::query_result_for_cook* parent_query
)
{
	if (!data[0].is_successful())
	{
		parent_query->finish_query				(result_error);
		return;
	}

	speedtree_tree_base_ptr st_tree_ptr			= static_cast_resource_ptr<speedtree_tree_base_ptr>(data[0].get_unmanaged_resource());

	speedtree_instance_ptr st_instance_ptr		= NEW(speedtree_instance_impl)(st_tree_ptr.c_ptr());

	// TODO: collision geometry

	parent_query->set_unmanaged_resource		(st_instance_ptr.c_ptr(), resources::nocache_memory, sizeof(speedtree_instance_impl));
	parent_query->finish_query					(result_success);
}

void speedtree_instance_cook::delete_resource( resources::resource_base* resource )
{
	speedtree_instance_impl* instance = static_cast<speedtree_instance_impl*>( resource );
	DELETE( instance );
}

} // namespace render
} // namespace vostok
