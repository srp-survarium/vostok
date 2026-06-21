////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "empty_hands_cook.h"
#include "empty_hands.h"

namespace survarium {

empty_hands_cook::empty_hands_cook( ) :
	resources::translate_query_cook( resources::empty_hands_class, reuse_false, use_current_thread_id )
{
	resources::register_cook( this );
}

// claude@NOTE: parked - target delete_resource is a CUSTOM 3-step delete of the
// combined empty_hands+animations block, not VOSTOK_DELETE_IMPL: (1) call the
// resource's virtual dtor via vtable[0] with deallocate=0, (2) loop destroying
// m_animations[i] (~managed_resource_ptr) over [resource+0x14C]/[+0x150] (the trailing
// array), (3) __RTCastToVoid + vostok_mspace_free. The animations array lives in the
// trailing region of the same malloc (see on_empty_hands_animations_loaded), so the
// cook destroys it explicitly. NEXT: spell the manual virtual-dtor + animations
// destruction loop + free (the empty_hands dtor itself does NOT own m_animations).
void empty_hands_cook::delete_resource( resources::resource_base* resource )
{
	VOSTOK_DELETE_IMPL( g_allocator, resource );
}

// claude@NOTE: parked - structure close but the combined-buffer allocation idiom isn't
// exact. Target mallocs sizeof(empty_hands)+count*4 as ONE block, places the
// managed_resource_ptr array in the trailing region ([hands+0x158]), copies each
// result's managed resource (refcounted), then placement-news empty_hands(animations,
// count) guarded by a non-null check, and set_unmanaged_resource(block, nocache,
// buffer_size). NEXT: reconcile the named-local set (buffer_size/parent/animations_count)
// and the malloc-then-guarded-placement-new shape against the asm at 0x766900.
void empty_hands_cook::on_empty_hands_animations_loaded( resources::queries_result& data )
{
	const u32 animations_count	= data.size( );
	const u32 buffer_size		= sizeof( empty_hands ) + animations_count * sizeof( resources::managed_resource_ptr );

	empty_hands* hands			= (empty_hands*)VOSTOK_MALLOC_IMPL( g_allocator, buffer_size, "empty_hands" );

	resources::managed_resource_ptr* animations = (resources::managed_resource_ptr*)( hands + 1 );
	for ( u32 i = 0 ; i < animations_count ; ++i )
		animations[i] = static_cast_resource_ptr< resources::managed_resource_ptr >( data[i].get_managed_resource( ) );

	new ( hands ) empty_hands( animations, animations_count );

	resources::query_result_for_cook* const parent = data.get_parent_query( );
	parent->set_unmanaged_resource( hands, resources::nocache_memory, buffer_size );
	parent->finish_query( result_success );
}

// claude@NOTE: parked - structure diverges on the request-array build. Target hoists
// the ["user_animations"] node and request count differently and the per-element loop
// (alloca'd request[count], each {(pcstr)node[i], animation_class}) does not match the
// target's statement granularity (lines 38/42/43/47/48/49 vs my hoisted node + alloca +
// loop). Named locals per carcass: animations_node + config. NEXT: match the exact VLA
// (_alloca_probe_16) request build + the divide-by-sizeof(binary_config_value) count
// computation seen at 0x766ac0.
void empty_hands_cook::on_empty_hands_config_loaded( resources::queries_result& data )
{
	configs::binary_config_ptr config					= static_cast_resource_ptr< configs::binary_config_ptr >( data[0].get_unmanaged_resource( ) );
	configs::binary_config_value const& animations_node	= config->get_root( )["user_animations"];

	u32 const animations_count = animations_node.size( );
	resources::request* requests = (resources::request*)alloca( animations_count * sizeof( resources::request ) );

	for ( u32 i = 0 ; i < animations_count ; ++i )
		requests[i].set( (pcstr)animations_node[i], resources::animation_class );

	resources::query_resources(
		requests,
		animations_count,
		boost::bind( &empty_hands_cook::on_empty_hands_animations_loaded, this, _1 ),
		g_allocator,
		NULL,
		data.get_parent_query( )
	);
}

void empty_hands_cook::translate_query( resources::query_result_for_cook& parent )
{
	fs_new::virtual_path_string empty_hands_config_name;
	empty_hands_config_name.assignf( "resources/%s", parent.get_requested_path( ) );

	resources::query_resource(
		empty_hands_config_name.c_str( ),
		resources::binary_config_class_impl,
		boost::bind( &empty_hands_cook::on_empty_hands_config_loaded, this, _1 ),
		g_allocator,
		NULL,
		&parent
	);
}

} // namespace survarium
