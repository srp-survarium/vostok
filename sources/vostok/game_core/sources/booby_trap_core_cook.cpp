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
// STRUCTURE DIFF[target 0x751ee0 | base 0x458280]: target 16 / base 16 stmts
// .. same ..
// 0x01b <0x1b> | 0x01b <0x1d> | if ( !parent.user_data( )->try_get( config ) )   SIZE
// .. same ..
// 0x0cf <0x1a> | 0x0d1 <0x15> | { aabb_path.c_str( ), resources::binary_config_class_impl },   SIZE
// .. same ..
// 0x0e9 <0x90> | 0x0e6 <0x96> | );   SIZE
// ; aligned 13, size-diffs 3, quantity-diffs 0
// VERDICT: STRUCTURE MATCH (shape ok) - 3 SIZE diffs are LTCG call-boundary lowering: user_data() result passed in reg vs slot to ->try_get; the request aggregate built into a temp slot + boost::bind/query_resources closure-temp frame inflation. create_request() form tested -> regressed 90.16->85.97, aggregate-init confirmed correct. Non-steerable. trail: booby_trap_core_cook.md
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
}

// STATE[94.31%|DONE]: LTCG resource_ptr-temp inline only
// STRUCTURE DIFF[target 0x751e00 | base 0x4581a0]: target 10 / base 10 stmts
// .. same ..
// 0x022 <0x22> | 0x022 <0x1d> | configs::binary_config_ptr aabb = static_cast_resource_ptr< configs::binary_config_ptr >( data[0].get_unmanaged_resource( ) );   SIZE
// .. same ..
// ; aligned 9, size-diffs 1, quantity-diffs 0
// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE diff is the LTCG resource_ptr-temp inline-vs-temp choice in static_cast_resource_ptr (target passes get_unmanaged_resource() prvalue directly; base materializes an extra resource_ptr copy-construct), same class as sibling booby_trap_set_core_cook::on_config_ready. Non-steerable. trail: booby_trap_core_cook.md
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