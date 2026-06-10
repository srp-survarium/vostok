////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_ammunition_cook.h>

#include <vostok/game_core/weapon_ammunition.h>

namespace survarium {

// STATE[100%|DONE]
weapon_ammunition_cook::weapon_ammunition_cook( ) :
	resources::translate_query_cook( resources::weapon_ammunition_class, reuse_false, use_current_thread_id )
{
	resources::register_cook( this );
}

// STATE[99.83%|DONE]: LTCG frame-size/slot residual (boost::bind functor temp); structure exact.
void weapon_ammunition_cook::translate_query( resources::query_result_for_cook& parent )
{
	fs_new::virtual_path_string config_name;
	config_name.assignf( "resources/%s", parent.get_requested_path( ) );

	resources::query_resource(
		config_name.c_str( ),
		resources::binary_config_class_impl,
		boost::bind( &weapon_ammunition_cook::on_config_ready, this, _1, &parent ),
		g_allocator,
		NULL,
		&parent
	);

	// STRUCTURE DIFF: target 3 stmts / base 3 stmts (no diverging rows, 0x15f bytes both)
	// VERDICT: STRUCTURE MATCH - byte sizes now equal; 99.83 residual is an ICF fold-name relocation only.
}

// STATE[31.00%|PARTIAL]: shared cook-base wall - delete_helper LTCG arg passing.
void weapon_ammunition_cook::delete_resource( resources::resource_base* resource )
{
	VOSTOK_DELETE_IMPL( g_allocator, resource );

	// STRUCTURE DIFF: target 1 stmts / base 1 stmts
	// SIZE -0x1 | 46 | VOSTOK_DELETE_IMPL( g_allocator, resource );
	// VERDICT: STRUCTURE MATCH (shape ok) - delete_helper LTCG arg-passing convention differs
	// (target stack arg vs base edi), shared wall across the 5 raw-param cook deleters, non-steerable.
}

// STATE[100%|DONE]
void weapon_ammunition_cook::on_config_ready( resources::queries_result& data, resources::query_result_for_cook* parent )
{
	ASSERT( UNKNOWN_EXPRESSION_T( data.size( ) == 1 ) );
	configs::binary_config_ptr	config	= static_cast_resource_ptr< configs::binary_config_ptr >( data[0].get_unmanaged_resource( ) );
	weapon_ammunition*			wa		= VOSTOK_NEW_IMPL( g_allocator, weapon_ammunition );
	wa->load( config->get_root( )["data"] );

	parent->set_unmanaged_resource( wa, resources::memory_usage_type( resources::nocache_memory, sizeof( weapon_ammunition ) ) );
	parent->finish_query( result_success );
}

} // namespace survarium
