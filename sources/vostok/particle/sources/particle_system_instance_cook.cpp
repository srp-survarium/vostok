////////////////////////////////////////////////////////////////////////////
//	Created		: 28.07.2010
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "particle_system_instance_cook.h"
#include "particle_system_instance.h"
#include "help_functions.h"
#include "particle_system.h"
#include "particle_emitter.h"
#include "particle_world.h"
#include "particle_system_instance_impl.h"
#include <vostok/particle/material_query_data.h>
#include <vostok/render/facade/material_effects_instance_cook_data.h>

namespace vostok {
namespace particle {

particle_system_instance_cook::particle_system_instance_cook() 
	: super(resources::particle_system_instance_class, reuse_false, use_current_thread_id)
{
}

void particle_system_instance_cook::translate_query( resources::query_result_for_cook& parent )
{
	query_resource(
		parent.get_requested_path(),
		resources::particle_system_class,
		boost::bind( &particle_system_instance_cook::on_sub_resources_loaded, this, _1),
		&::vostok::memory::g_mt_allocator,
		NULL,
		&parent
	);
}

void particle_system_instance_cook::on_sub_resources_loaded( resources::queries_result& data )
{
	R_ASSERT_U(data.is_successful());
	
	particle_system* part_system = static_cast_checked<particle_system*>( data[0].get_unmanaged_resource().c_ptr() );
	
	particle_system_instance_impl* created_resource = MT_NEW(particle_system_instance_impl)();
	u32 num_material_queries = 0;
	
	for (u32 lod_index=0; lod_index<part_system->m_num_lods; lod_index++)
	{
		for (u32 emitter_index=0; emitter_index<part_system->m_lods[lod_index].m_num_emitters; emitter_index++)
		{
			particle_emitter& emitter = part_system->m_lods[lod_index].m_emitters_array[emitter_index];
			
			if (emitter.m_event || !emitter.get_enable())
				continue;

			++num_material_queries;
		}
	}

	material_query_data* material_data = MT_ALLOC(material_query_data, num_material_queries);
	material_query_data* material_data_it = material_data;

	for (u32 lod_index=0; lod_index<part_system->m_num_lods; lod_index++)
	{
		created_resource->set_template(lod_index, part_system);

		for (u32 emitter_index=0; emitter_index<part_system->m_lods[lod_index].m_num_emitters; emitter_index++)
		{
			particle_emitter& emitter = part_system->m_lods[lod_index].m_emitters_array[emitter_index];

			if (emitter.m_event || !emitter.get_enable())
				continue;

			particle_emitter_instance* instance = particle_world::create_emitter_instance(emitter, false, false);
			pcstr const material_name = emitter.m_material_name[0] ? emitter.m_material_name : "default_particle";

			new(material_data_it)material_query_data;
			material_data_it->instance = instance;
			material_data_it->material_name = material_name;
			material_data_it->vertex_type = instance->get_vertex_input_type();
			++material_data_it;

			created_resource->add_emitter_instance(lod_index, instance);
		}
	}

	resources::request* material_requests = (resources::request*)ALLOCA(sizeof(resources::request) * num_material_queries);
	resources::user_data_variant* user_data_variants = (resources::user_data_variant*)ALLOCA(sizeof(resources::user_data_variant) * num_material_queries);
	resources::user_data_variant** user_data_variants_ptrs = (resources::user_data_variant**)ALLOCA(sizeof(resources::user_data_variant*) * num_material_queries);
	render::material_effects_instance_cook_data* cook_data = MT_ALLOC(render::material_effects_instance_cook_data, num_material_queries);

	for (u32 i=0; i<num_material_queries; i++)
	{
		user_data_variants_ptrs[i] = new(&user_data_variants[i])resources::user_data_variant;
		user_data_variants_ptrs[i]->set(new(&cook_data[i])render::material_effects_instance_cook_data(material_data[i].vertex_type, NULL, false));
	}

	for (u32 i=0; i<num_material_queries; i++)
	{
		material_requests[i].id = resources::material_effects_instance_class;
		material_requests[i].path = material_data[i].material_name.c_str();
	}

	resources::query_resources(
		material_requests,
		num_material_queries,
		boost::bind(&particle_system_instance_cook::on_materials_loaded, this, _1, created_resource, material_data, cook_data),
		&::vostok::memory::g_mt_allocator,
		(resources::user_data_variant const**)user_data_variants_ptrs,
		data.get_parent_query()
	);
	}

void particle_system_instance_cook::on_materials_loaded(
	resources::queries_result& data,
	particle_system_instance_impl* created_resource,
	material_query_data* material_data,
	render::material_effects_instance_cook_data* cook_data
)
{
	u32 const num_queries = data.size();

	for (u32 i=0; i<num_queries; i++)
	{
		if (data[i].is_successful())
			material_data[i].instance->change_material(data[i].get_unmanaged_resource());
	}
	
	particle_system_instance_ptr created_res	= created_resource;
	
	resources::query_result_for_cook* parent	= data.get_parent_query();
	parent->set_unmanaged_resource				( created_res, resources::nocache_memory, sizeof(particle_system_instance_impl) );
	parent->finish_query						( result_success );

	for (u32 i=0; i<num_queries; i++)
	{
		cook_data[i].~material_effects_instance_cook_data();
	}

	MT_FREE(material_data);
	MT_FREE(cook_data);
}

void particle_system_instance_cook::delete_resource(resources::resource_base* res)
{
	particle_system_instance* ps_instance = static_cast_checked<particle_system_instance*>(res);
	MT_DELETE(ps_instance);
}

vostok::uninitialized_reference<particle_system_instance_cook>	s_particle_cook_object;

void register_particles_cooker()
{
	static bool particles_cooker_registered = false;
	if (particles_cooker_registered) return;

	VOSTOK_CONSTRUCT_REFERENCE	( s_particle_cook_object, particle_system_instance_cook ) ();
	resources::register_cook	( s_particle_cook_object.c_ptr() );

	particles_cooker_registered = true;
}

void unregister_particles_cooker()
{
	static bool particles_cooker_unregistered = false;
	if (particles_cooker_unregistered) return;

	resources::unregister_cook	( resources::particle_system_instance_class );
	VOSTOK_DESTROY_REFERENCE		( s_particle_cook_object );

	particles_cooker_unregistered = true;
}

} // namespace particle
} // namespace vostok
