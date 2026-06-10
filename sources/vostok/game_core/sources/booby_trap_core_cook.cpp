////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/booby_trap_core_cook.h>

namespace survarium {

// STATE[100%|DONE]
booby_trap_core_cook::booby_trap_core_cook( ) :
	resources::translate_query_cook( resources::booby_trap_class, reuse_false, use_current_thread_id )
{
	resources::register_cook( this );
}

// STATE[90.16%|DONE]: LTCG temp/closure lowering only
void booby_trap_core_cook::translate_query( resources::query_result_for_cook& parent )
{
	configs::binary_config_ptr config;
	if ( !parent.user_data( )->try_get( config ) )
	{
		ASSERT( UNKNOWN_EXPRESSION );
		parent.finish_query( result_error );
		return;
	}

	ASSERT( UNKNOWN_EXPRESSION );
	pcstr model = (pcstr)config->get_root( )["data"]["model_armed"];

	fixed_string< 260 > aabb_path;
	aabb_path.assignf( "resources/models/%s.model/render/export_properties", model );

	resources::request requests[1] = {
		{ aabb_path.c_str( ), resources::binary_config_class_impl },
	};

	resources::query_resources(
		requests,
		boost::bind( &booby_trap_core_cook::on_subresources_loaded, this, _1, config ),
		g_allocator,
		NULL,
		&parent,
		assert_on_fail_true
	);

	// STRUCTURE DIFF: target 11 stmts / base 11 stmts
	// SIZE +0x2 | 30 | if ( !parent.user_data( )->try_get( config ) )
	// SIZE -0x5 | 44 | { aabb_path.c_str( ), resources::binary_config_class_impl },
	// SIZE +0x6 | 54 | );
	// VERDICT: STRUCTURE MATCH (shape ok) - L30 is the LTCG promoted-convention register (try_get
	// takes user_data()'s result in eax in target, ecx in base, one mov); L44 is the documented
	// LTCG-promoted two-store request factory (target builds the aggregate via an ICF-folded
	// dest-in-eax helper; create_request form tested earlier and regressed); L54 is bind/
	// query_resources closure-temp frame inflation. Non-steerable.
}

// STATE[100%|DONE]
void booby_trap_core_cook::on_subresources_loaded( resources::queries_result& data, configs::binary_config_ptr config )
{
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );

	configs::binary_config_ptr aabb = static_cast_resource_ptr< configs::binary_config_ptr >( data[0].get_unmanaged_resource( ) );

	booby_trap_core* resource = new_derived_resource( );
	resource->load( config->get_root( )["data"] ); // sushi@TODO: Understand;
	resource->load_aabb( aabb->get_root( ) );

	query_for_derived_resources( data.get_parent_query( ), resource, config );
}

// STATE[100%|DONE]
void booby_trap_core_cook::finish_query( resources::query_result_for_cook* parent, booby_trap_core* resource )
{
	parent->set_unmanaged_resource(
		resources::unmanaged_resource_ptr( resource ),
		resources::memory_usage_type( resources::nocache_memory, get_derived_resource_size( ) )
	);
	parent->finish_query( result_success );
}

} // namespace survarium