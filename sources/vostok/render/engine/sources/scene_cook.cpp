#include "pch.h"
#include "scene_cook.h"
#include "scene_manager.h"
#include "scene.h"
#include <vostok/render/facade/common_types.h>
#include <vostok/particle/world.h>
#include "material.h"

namespace vostok {
namespace render {

scene_cook::scene_cook( ) :
	resources::translate_query_cook(
		resources::scene_class,
		reuse_false,
		use_current_thread_id
	)
{
	// FUNCTION BODY[0x76af90]
	// ******
}

void scene_cook::translate_query( resources::query_result_for_cook& in_out_query )
{
	// LOCALS
	// scene_configuration 				renderer_configuration
	// variant< 32 > 					data
	// ******

	// FUNCTION BODY[0x76b140]: 26
	// <0>
	// <1>
	// <0x76b14a>|0x00a|+0x020:'30'
	// <0x76b16a>|0x02a|+0x00a:'31'
	// <0>
	// <0x76b174>|0x034|+0x026:'33'
	// <0x76b19a>|0x05a|+0x029:'34'
	// <0>
	// <0x76b1c3>|0x083|+0x00b:'36'
	// <0>
	// <0x76b1ce>|0x08e|+0x010:'38'
	// <0x76b1de>|0x09e|+0x00b:'39'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x76b1e9>|0x0a9|+0x0ee:'48'
	// <0x76b2d7>|0x197|+0x020:'49'
	// <0>
	// <0x76b2f7>|0x1b7|+0x020:'51'
	// <0x76b317>|0x1d7|-0x029:'52'
	// <0>
	// <0x76b2ee>|0x1ae|+0x036:'54'
	// ******
	vostok::render::scene_configuration renderer_configuration;

	if( in_out_query.user_data() )
		in_out_query.user_data()->try_get(renderer_configuration);

	scene* created_resource	= NEW(vostok::render::scene)(renderer_configuration);
	scene_manager::ref().add_scene(created_resource);

	if (renderer_configuration.m_create_particle_world)
	{
 		resources::user_data_variant data;
 		data.set( static_cast<particle::engine*>(&created_resource->m_particle_engine) );
 		resources::query_create_resource(
 			"",
			const_buffer("", 10*Mb),
 			resources::particle_world_class,
 			boost::bind(&scene_cook::on_particle_world_created, this, _1, created_resource, &in_out_query),
 			g_allocator,
 			&data,
			&in_out_query
 		);
 	}else
	{
		in_out_query.set_unmanaged_resource(created_resource, resources::nocache_memory, sizeof(vostok::render::scene));
		in_out_query.finish_query(result_success);
	}
}

void scene_cook::on_particle_world_created(
	resources::queries_result&				result,
	scene*									created_resource,
	resources::query_result_for_cook*		in_out_query
)
{
	// FUNCTION BODY[0x76b070]: 4
	// <0x76b071>|0x001|+0x089:'58'
	// <0x76b0fa>|0x08a|+0x006:'59'
	// <0x76b100>|0x090|+0x02a:'60'
	// <0x76b12a>|0x0ba|+0x00b:'61'
	// ******
	created_resource->m_particle_world	= static_cast_checked<vostok::particle::world*>(result[0].get_unmanaged_resource().c_ptr());
	created_resource->m_particle_engine.set_particle_world( *created_resource->m_particle_world );
	in_out_query->set_unmanaged_resource(created_resource, resources::nocache_memory, sizeof(vostok::render::scene));
	in_out_query->finish_query(result_success);
}

void scene_cook::delete_resource( resources::resource_base* resource )
{
	// CALL SITE INFO
	// <0x76b053> -> void* < unknown >( u32 )
	// ******

	// FUNCTION BODY[0x76aff0]: 5
	// <0>
	// <1>
	// <0x76aff0>|0x000|+0x045:'68'
	// <0>
	// <0x76b035>|0x045|+0x036:'70'
	// ******
 	scene* scn = static_cast_checked<scene*>( resource );

	scene_manager::ref().remove_scene(scn);

	DELETE(scn);
}
} // namespace render
} // namespace vostok
