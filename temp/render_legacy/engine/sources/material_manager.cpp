////////////////////////////////////////////////////////////////////////////
//	Created		: 17.05.2010
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "material_manager.h"
#include <vostok/fs_watcher.h>
#include "render_model.h"
#include "render_particle_emitter_instance.h"
#include <vostok/particle/world.h>
#include <vostok/render/core/effect_manager.h>
#include <vostok/render/core/destroy_data_helper.h>
#include <vostok/render/core/effect_options_descriptor.h>
#include "post_process_parameters.h"
#include <vostok/render/core/resource_manager.h>
#include "material_effects.h"
#include <vostok/render/core/res_effect.h>
#include <vostok/render/core/res_geometry.h>
#include <vostok/render/core/custom_config.h>
#include <vostok/render/core/untyped_buffer.h>

#include <vostok/render/facade/material_effects_instance_cook_data.h>

namespace vostok {
namespace render {


class material_shader_base: public effect_material_base
{
	material_shader_base(enum_vertex_input_type /*vertex_input_type*/, material_ptr /*mtl*/)
	{
		
	}
	u32				m_vertex_type;
	material_ptr	m_mtl;
	ref_effect		m_shader;
};



static pcstr resources_materials_string						=	"resources/materials";
static pcstr resources_material_instances_string			=	"resources/material_instances";
static pcstr resources_materials_string_with_slash			=	"resources/materials/";
static pcstr resources_material_instances_string_with_slash =	"resources/material_instances/";


pcstr material_manager::get_materials_path() const
{
	return resources_materials_string;
}

pcstr material_manager::get_material_instances_path() const
{
	return resources_material_instances_string;
}


void material_manager::get_output_text(fs_new::virtual_path_string * out_text, u32& num_lines)
{
	num_lines = 0;
	
	for (material_effects_entries_type::iterator it = m_material_effects.begin(); 
												 it != m_material_effects.end(); 
												 ++it)
	{
		out_text->assignf("[%d] %s", it->m_material_effects_instance_ptr->c_ptr(), it->m_material_name.c_str());
		
		out_text++;
		num_lines++;
		if (num_lines==100)
			break;
	}
}

#ifndef MASTER_GOLD
static vostok::fs_new::virtual_path_string get_short_material_name(pcstr name)
{
	vostok::fs_new::virtual_path_string result = name;
	result.replace	(resources_materials_string_with_slash, "");
	result.replace	(resources_material_instances_string_with_slash, "");
	
	vostok::fs_new::virtual_path_string::size_type pos = result.rfind('.');
	if (pos!=vostok::fs_new::virtual_path_string::npos)
		result.set_length(pos);
	
	return result;
}
#endif // #ifndef MASTER_GOLD

void material_manager::on_material_effects_instance_loaded(vostok::resources::queries_result& data, material_effects_instance_ptr* mtl_effects_instance_ptr)
{
	if (!data[0].is_successful())
		return;
	
	//pcstr requested_path		= data[0].get_requested_path();
	*mtl_effects_instance_ptr	= static_cast_resource_ptr<material_effects_instance_ptr>(data[0].get_unmanaged_resource());
	
	//mtl_effects_instance_ptr->set_material_effects		(new_material->get_material_effects());
}

void material_manager::on_material_loaded(vostok::resources::queries_result& data, 
										  post_process_parameters* in_out_post_process_parameters)
{
	if (!data[0].is_successful())
		return;
	
	initialize_post_process_parameters(
		in_out_post_process_parameters, 
		static_cast_resource_ptr<material_ptr>(data[0].get_unmanaged_resource()),
		false
	);
}

#ifndef MASTER_GOLD

void material_manager::update_material_from_config(fs_new::virtual_path_string const& material_name,  vostok::configs::lua_config_value const& config_value)
{
	on_material_source_changed(material_name, create_material(material_name, config_value));
}

material_ptr material_manager::create_material(fs_new::virtual_path_string const& material_name, vostok::configs::lua_config_value const& config_value)
{
	material_ptr								out_material_ptr;
	
	resources::user_data_variant				user_data;
	
	configs::binary_config_ptr binary_config	= vostok::configs::create_binary_config(config_value);
	user_data.set								(binary_config);
	
	query_resource_and_wait						(
		material_name.c_str(), 
		resources::material_class, 
		boost::bind(
			&material_manager::on_material_created, 
			this, 
			_1, 
			&out_material_ptr
		),
		render::g_allocator,
		&user_data
	);
	
	return out_material_ptr;
}

void material_manager::on_material_created(vostok::resources::queries_result& data, material_ptr* out_material_ptr)
{
	if (data[0].is_successful())
	{
		*out_material_ptr = static_cast_resource_ptr<material_ptr>(data[0].get_unmanaged_resource());
	}
	else
	{
		*out_material_ptr = NULL;
	}
}


void material_manager::on_material_source_changed(fs_new::virtual_path_string const& material_name, material_ptr const& in_material)
{
	bool found_in_effects = false;
	for (material_effects_entries_type::iterator it = m_material_effects.begin(); 
												 it != m_material_effects.end(); 
												 ++it)
	{
		if (it->m_material_name == material_name)
		{
			resources::user_data_variant user_data;
			user_data.set					(
				NEW(material_effects_instance_cook_data)(
					(*it->m_material_effects_instance_ptr)->get_material_effects().get_vertex_input_type(),
					in_material.c_ptr() ? static_cast_resource_ptr<resources::unmanaged_resource_ptr>(in_material) : NULL
				)
			);
			
			query_resource_and_wait			(
 				material_name.c_str(), 
 				resources::material_effects_instance_class, 
 				boost::bind(
					&material_manager::on_material_effects_instance_loaded, 
					this, 
					_1, 
					it->m_material_effects_instance_ptr
				),
 				render::g_allocator,
				&user_data
 			);
			
			found_in_effects				= true;
		}
	}
	
	if (!found_in_effects)
	{
		post_process_parameters_type::iterator it_begin = m_post_process_parameters.begin();
		post_process_parameters_type::iterator it_end	= m_post_process_parameters.end();
		post_process_parameters_type::iterator it		= it_begin;
		
		for ( ; it != it_end; ++it)
		{
			if (it->second != material_name)
				continue;
			
			if (!in_material.c_ptr())
			{
				query_resource_and_wait			(
					material_name.c_str(), 
					resources::material_class, 
					boost::bind(
						&material_manager::on_material_loaded, 
						this, 
						_1, 
						it->first
					),
					render::g_allocator
				);
			}
			else
			{
				initialize_post_process_parameters(
					it->first, 
					in_material,
					false
				);
			}
		}
	}
}

// FIX: on_material_source_changed calling twice
void material_manager::on_material_source_changed(vostok::vfs::vfs_notification const & info)
{
	if(info.type!=vostok::vfs::vfs_notification::type_modified)
		return;
	
	vostok::fs_new::virtual_path_string short_file_name	= get_short_material_name(info.virtual_path);
	on_material_source_changed				(short_file_name);
}
#endif // #ifndef MASTER_GOLD

void material_manager::remove_post_process_parameters(post_process_parameters* in_post_process_parameters)
{
	post_process_parameters_type::iterator found		= m_post_process_parameters.find(in_post_process_parameters);
	
	if (found != m_post_process_parameters.end())
		m_post_process_parameters.erase(found);
}




} // namespace render 
} // namespace vostok 
