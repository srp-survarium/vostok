////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/artefact_container_core.h>

#include <vostok/game_core/collision_user.h>
#include <vostok/game_core/generic_anomaly_core.h>
#include <vostok/game_core/inventory_holder.h>

namespace survarium {

// STATE[100%|DONE]
artefact_container_core::artefact_container_core( ) : m_owner( NULL )
{
	// FUNCTION BODY
	// <0x73dfe0>|0x000|      :'17'	{
	// ******
}

// STATE[100%|DONE]
void artefact_container_core::load( configs::binary_config_value const& cfg )
{
	usable_object::load( cfg );
	m_artefact_search_time_ms = math::floor( (float)cfg["artefacts_search_time_sec"] * 1000.0f );
}

// STATE[100%|DONE]
void artefact_container_core::activate( generic_anomaly_core* owner, physics::world* world, scheduler& __formal )
{
	m_owner = owner;
	usable_object::insert( world );

	// FUNCTION BODY
	// <0x73deb0>|0x000|+0x007:'26'	{
	// <0x73deb7>|0x007|+0x009:'27'
	// <0x73dec0>|0x010|+0x00c:'28'
	// <0x73decc>|0x01c|      :'29'	}
	// ******
}

// STATE[100%|DONE]
void artefact_container_core::deactivate( )
{
	usable_object::remove( );
	m_owner = NULL;

	// FUNCTION BODY
	// <0x73de90>|0x000|+0x007:'32'	{
	// <0x73de97>|0x007|+0x008:'33'
	// <0x73de9f>|0x00f|+0x00a:'34'
	// <0x73dea9>|0x019|      :'35'	}
	// ******
}

// STATE[100%|DONE]
bool artefact_container_core::use_initialize( usable_object_user_data* user )
{
	if ( !m_usable_object_users.empty( ) )
		return false;

	ASSERT( UNKNOWN_EXPRESSION_T( user ) );
	m_usable_object_users.push_front( user );
	user->current_object = this;
	user->start_using_time_ms = user->current_time_ms;

	return true;

	// FUNCTION BODY
	// <0x73de30>|0x000|+0x009:'38'	{
	// <0x73de39>|0x009|+0x013:'39'
	// <0x73de4c>|0x01c|+0x004:'40'
	// <0>
	// <0x73de50>|0x020|+0x00c:'42'
	// <0x73de5c>|0x02c|+0x011:'43'
	// <0x73de6d>|0x03d|+0x009:'44'
	// <0x73de76>|0x046|+0x00c:'45'
	// <0x73de82>|0x052|+0x002:'46'
	// <0x73de84>|0x054|      :'47'	}
	// ******
}

// STATE[92.86%|PARTIAL]: intrusive_ptr unspecified_bool conversion (if m_artefact) + transfer_artefact arg reg, non-steerable
bool artefact_container_core::use_execute( usable_object_user_data* user )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_usable_object_users[0] == user ) );
	ASSERT( UNKNOWN_EXPRESSION_T( user ) );															// sushi@NOTE: Stupid assert. Instead we should verify that the state at the point of "execution" is correct. Something like m_owner and m_artefact exist, since they should! Though ifs down there don't make sense any longer.
	u32 left_ms					= user->current_time_ms - user->start_using_time_ms;				// sushi@NOTE: Misnomer! elapsed_ms would be a better name.

	float artsearch_time		= m_artefact_search_time_ms * user->booster_artcont_time_factor;
	u32 artefact_search_time_ms = artsearch_time > 0.0f ? (u32)artsearch_time : 0;					// sushi@NOTE: Just use min. Also how can this ever be negative. And this can result in div by zero, which is not good!

	user->current_progress		= math::floor( left_ms / float(artefact_search_time_ms) * 100.0f ); // sushi@NOTE: This can get higher than 100%!

	if ( left_ms >= artefact_search_time_ms ) // if ( search_finished )
	{
		if ( m_owner )
			m_owner->on_artefact_container_use( this );

		user->start_using_time_ms = user->current_time_ms;
		if ( m_artefact	)	// sushi@NOTE: In target this calls `unspecified_bool_type` conversion. For some reason it doesn't in base.
			transfer_artefact( user->owner->cast_to_inventory_holder( ) );
	}

	return true;

	// STRUCTURE DIFF[target 0x72e090 | base 0x54a040]: target 20 / base 19 stmts
	// <0>         | --          |    EMPTY only target
	// .. same ..
	// 0x0f0 <0x29> | 0x0f0 <0x1f> | if ( m_artefact	)   SIZE
	// 0x119 <0x1c> | 0x10f <0x1a> | transfer_artefact( user->owner->cast_to_inventory_holder( ) );   SIZE
	// .. same ..
	// ; aligned 17, size-diffs 2, quantity-diffs 1
	// VERDICT: STRUCTURE MATCH (shape ok) - if(m_artefact): target calls intrusive_ptr unspecified_bool conversion, base inlines setne/movzx/test; transfer_artefact arg+this in different regs (LTCG). non-steerable. trail: artefact_container_core.md
}

// STATE[100%|DONE]
bool artefact_container_core::use_finalize( usable_object_user_data* user )
{
	ASSERT( UNKNOWN_EXPRESSION_T( user ) );
	ASSERT( UNKNOWN_EXPRESSION_T( m_usable_object_users.contains_object( user ) ) );
	ASSERT( UNKNOWN_EXPRESSION_T( user->current_object == this ) );

	user->current_object = NULL;
	user->current_progress = u32(-1);
	m_usable_object_users.erase( user );
	return true;

	// FUNCTION BODY
	// <0x73ddd0>|0x000|+0x009:'74'	{
	// <0x73ddd9>|0x009|+0x00c:'75'
	// <0x73dde5>|0x015|+0x00c:'76'
	// <0>
	// <0x73ddf1>|0x021|+0x00c:'78'
	// <0x73ddfd>|0x02d|+0x00a:'79'
	// <0x73de07>|0x037|+0x00a:'80'
	// <0x73de11>|0x041|+0x00f:'81'
	// <0x73de20>|0x050|+0x002:'82'
	// <0x73de22>|0x052|      :'83'	}
	// ******
}

// STATE[74.74%|PARTIAL]: resource_ptr conversion materializes more temporaries in target + set_amount arg reg, non-steerable
void artefact_container_core::artefact_spawned( resources::queries_result& data )
{
	ASSERT( UNKNOWN_EXPRESSION );
	m_artefact = static_cast_resource_ptr<artefact_base_ptr>( data[0].get_unmanaged_resource( ) );
	m_artefact->set_amount( 1 );

	// STRUCTURE DIFF[target 0x72df30 | base 0x549f80]: target 3 / base 3 stmts
	// .. same ..
	// 0x016 <0x6c> | 0x016 <0x80> | m_artefact = static_cast_resource_ptr<artefact_base_ptr>( data[0].get_unmanaged_resource( ) );   SIZE
	// 0x082 <0x22> | 0x096 <0x1f> | m_artefact->set_amount( 1 );   SIZE
	// ; aligned 1, size-diffs 2, quantity-diffs 0
	// VERDICT: STRUCTURE MATCH (shape ok) - assignment line: target/base materialize a different number of intrusive_ptr temporaries (extra ~dec dtors) for the resource_ptr conversion; set_amount(1): target pushes 1 on stack, base passes in ecx (LTCG arg). non-steerable. trail: artefact_container_core.md
}

// STATE[100%|DONE]
void artefact_container_core::spawn_artefact( )
{
	variant<32> ud;
	ud.set( u16(0x39) ); // sushi@TODO: What does that mean

	resources::query_resource(
		"gameplay/items/artefacts/lifebone",
		resources::item_class,
		boost::bind( &artefact_container_core::artefact_spawned, this, _1 ),
		g_allocator,
		&ud,
		NULL
	);
}

// STATE[59.28%|PARTIAL]: resource_ptr conversion inlines to a different shape (target: read c_ptr + intrusive_ptr::set; base: out-of-line static_cast_resource_ptr + typecheck copy), non-steerable
void artefact_container_core::transfer_artefact( inventory_holder* holder )
{
	holder->take_inventory_item( static_cast_resource_ptr< inventory_item_ptr >( m_artefact ) );
	m_artefact = NULL;

	// STRUCTURE DIFF[target 0x72e030 | base 0x549ec0]: target 2 / base 2 stmts
	// 0x009 <0x35> | 0x009 <0x8f> | holder->take_inventory_item( static_cast_resource_ptr< inventory_item_ptr >( m_artefact ) );   SIZE
	// .. same ..
	// ; aligned 1, size-diffs 1, quantity-diffs 0
	// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE on the conversion: target inlines static_cast_resource_ptr to a bare c_ptr read + intrusive_ptr::set (0x35 bytes); base emits an out-of-line static_cast_resource_ptr call plus an inlined intrusive_ptr copy-with-vtable-typecheck (0x8f bytes). same source, different template-inline decision under LTCG. non-steerable. (report.json leaves this symbol unpaired -> shows ~0%.) trail: artefact_container_core.md
}

} // namespace survarium
