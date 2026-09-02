// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include "tracer_model_instance_cook.h"
#include "tracer_model_instance.h"
namespace vostok {
namespace render {

tracer_model_instance_cook::tracer_model_instance_cook( ) :
	resources::translate_query_cook(
		resources::tracer_model_instance_class,
		reuse_false,
		use_current_thread_id
	)
{
	resources::register_cook( this );
}

void tracer_model_instance_cook::translate_query( resources::query_result_for_cook& parent )
{
	pcstr const requested_path = parent.get_requested_path( );

	resources::query_resource(
		requested_path,
		resources::static_model_instance_class,
		boost::bind( &tracer_model_instance_cook::on_model_ready, this, _1 ),
		g_allocator,
		0,
		&parent
	);
}

void tracer_model_instance_cook::on_model_ready( resources::queries_result& data )
{
	resources::query_result_for_cook& parent = *data.get_parent_query( );
	if ( !data[0].is_successful( ) )
	{
		parent.finish_query( result_success );
		return;
	}

	tracer_model_instance* instance = NEW( tracer_model_instance );
	static_model_ptr static_model = static_cast_resource_ptr<static_model_ptr>(
		data[0].get_unmanaged_resource( )
	);
	instance->m_model = static_model->m_render_model;
	instance->m_transform = math::float4x4( ).identity( );
	instance->set_color( math::color( 0 ) );

	parent.set_unmanaged_resource( instance, resources::nocache_memory, sizeof( tracer_model_instance ) );
	parent.finish_query( result_success );
}

void tracer_model_instance_cook::delete_resource( resources::resource_base* resource )
{
	tracer_model_instance* instance = static_cast<tracer_model_instance*>( resource );
	DELETE( instance );
}

} // namespace render
} // namespace vostok
