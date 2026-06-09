////////////////////////////////////////////////////////////////////////////
//	Created 	: 03.11.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/collision_geometry.h>

#include <vostok/physics/ghost_object.h>

namespace survarium {

// STRUCTURE DIFF[target 0x6eed10 | base 0x5139e0]: target 0 / base 0 stmts
// ; aligned 0, size-diffs 0, quantity-diffs 0
// VERDICT: STRUCTURE MATCH (shape ok) - base INLINES the collision::game_object base
// ctor (out-of-line `call game_object::game_object` in target, rva 0x9bbb0; absent from
// base index - collision is an optimized/out-of-scope module), cascading frame 0x18 vs
// 0x24 + slot renames. Non-steerable base-class inline-vs-call. trail: collision_geometry.md
// STATE[88.12%|DONE]
collision_geometry::collision_geometry( ) :
	m_physics_world		( NULL ),
	m_ghost_object		( NULL ),
	m_group				( 0 ),
	m_mask				( 0 )
{
}

// STRUCTURE DIFF[target 0x6ef180 | base 0x513b70]: target 3 / base 3 stmts
// .. same ..
// ; aligned 3, size-diffs 0, quantity-diffs 0
// VERDICT: STRUCTURE MATCH (shape ok) - sole diff is frame 0x3c vs 0x38 slot allocation
// (both ASSERTs present) + ASSERT empty-stub COMDAT-fold misname, non-steerable. trail: collision_geometry.md
// STATE[99.74%|DONE]
collision_geometry::~collision_geometry( )
{
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION_T( m_ghost_object ) );
	destroy_ghost_object( );
}

// STATE[100%|DONE]
void collision_geometry::destroy_ghost_object( )
{
	physics::destroy_ghost_object( m_ghost_object );
}

// STRUCTURE DIFF[target 0x6eeee0 | base 0x513bc0]: target 19 / base 19 stmts
// .. same ..
// 0x0e0 <0x89> | 0x0e0 <0x83> | LOG_WARNING( "invalid collision_geometry" );   SIZE
// .. same ..
// 0x1bc <0x40> | 0x1b6 <0x41> | physics::bt_collision_shape_ptr shape	= physics::create_compound_shape( meshes, float3( 1.0f, 1.0f, 1.0f ), m_name.c_str( ) );   SIZE
// 0x1fc <0x24> | 0x1f7 <0x23> | m_ghost_object							= physics::create_ghost_object( shape, transform );   SIZE
// 0x220 <0xf> | 0x21a <0xd> | shape->set_no_delete( );	// sushi@TODO: `unmanaged_intrusive_base::destroy` will not delete this resource. Understand for what reasons this was done.   SIZE
// .. same ..
// ; aligned 15, size-diffs 4, quantity-diffs 0
// VERDICT: STRUCTURE MATCH (shape ok) - SIZE diffs are LOG_WARNING boost::function ctor
// inlining + operator[]/c_ptr COMDAT-fold misnames + a push 38h/2Dh signature-string-len
// literal, all whole-program LTCG/ICF, non-steerable. trail: collision_geometry.md
// STATE[96.04%|DONE]
void collision_geometry::load( configs::binary_config_value const& cfg_val )
{
	m_name					= cfg_val["full_name"];
	float3 const& scale		= cfg_val["scale"];			// sushi@NOTE: Can reuse `read_transform` in base_project.cpp
	float3 const& rotation	= cfg_val["rotation"];
	float3 const& position	= cfg_val["position"];
	float4x4 transform = create_scale( scale ) * create_rotation( rotation ) * create_translation( position );

	if ( !cfg_val.value_exists( "meshes" ) )
	{
		LOG_WARNING( "invalid collision_geometry" );
		return;
	}
	

	m_mode									= (collision_geometry::collision_geometry_mode)(u32)cfg_val["mode"];
	configs::binary_config_value meshes		= cfg_val["meshes"];
	physics::bt_collision_shape_ptr shape	= physics::create_compound_shape( meshes, float3( 1.0f, 1.0f, 1.0f ), m_name.c_str( ) );
	m_ghost_object							= physics::create_ghost_object( shape, transform );
	shape->set_no_delete( );	// sushi@TODO: `unmanaged_intrusive_base::destroy` will not delete this resource. Understand for what reasons this was done.

	m_group									= cfg_val["filter_group"];
	m_mask									= cfg_val["filter_mask"];
}

// STATE[100%|DONE]
u32 collision_geometry::get_overlapping_objects_count( ) const
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_ghost_object ) );
	return m_ghost_object->get_overlapping_objects_count( );
}

// STATE[100%|DONE]
void collision_geometry::get_overlapping_objects(physics::base_physics_objects_type& result ) const
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_ghost_object ) );
	m_ghost_object->get_overlapping_objects( result );
}

// STATE[100%|DONE]
void collision_geometry::contact_test( physics::base_physics_object* object, physics::contact_test_predicate& predicate )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_ghost_object ) );
	m_ghost_object->contact_test( m_physics_world, object, predicate );
}

// STATE[100%|DONE]
bool collision_geometry::contact_test( )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_ghost_object ) );
	return m_ghost_object->contact_test( m_physics_world );
}

// STRUCTURE DIFF[target 0x6eee80 | base 0x513b20]: target 2 / base 2 stmts
// .. same ..
// 0x015 <0x12> | 0x015 <0x11> | m_ghost_object->non_compound_shapes_centers( centers_results );   SIZE
// ; aligned 1, size-diffs 1, quantity-diffs 0
// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is the centers_results arg passed on
// the stack (push) in target vs a register in base, an LTCG call-boundary arg-passing
// choice, non-steerable. trail: collision_geometry.md
// STATE[93.33%|DONE]
void collision_geometry::get_shapes_centers( vectora<float3>& centers_results ) const
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_ghost_object ) );
	m_ghost_object->non_compound_shapes_centers( centers_results );
}

// STATE[100%|DONE]
void collision_geometry::insert( physics::world* world )
{
	m_physics_world = world;
	ASSERT( UNKNOWN_EXPRESSION_T( m_ghost_object ) );
	m_ghost_object->user_data = this;
	m_ghost_object->insert( m_physics_world, m_group, m_mask );
}

// STATE[100%|DONE]
void collision_geometry::remove( )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_ghost_object ) );	// sushi@NOTE: Might also be `m_physics_world` check.
	m_ghost_object->remove( m_physics_world );			// sushi@NOTE: `user_data` was set in `insert`, but is not unset in `remove`.
	m_physics_world = NULL;
}

// STATE[100%|DONE]
void collision_geometry::subscribe( physics::world* world, collision_geometry_subscriber* subscriber )
{
	ASSERT( UNKNOWN_EXPRESSION );

	if ( m_subscribers.empty( ) )
		insert( world );

	m_subscribers.push_back( subscriber );
}

// STRUCTURE DIFF[target 0x6eebe0 | base 0x513850]: target 5 / base 5 stmts
// .. same ..
// 0x015 <0x55> | 0x015 <0x50> | m_subscribers.erase( std::find( m_subscribers.begin( ), m_subscribers.end( ), subscriber ) );   SIZE
// .. same ..
// ; aligned 4, size-diffs 1, quantity-diffs 0
// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is m_subscribers.end() emitted as an
// out-of-line accessor call in target vs inlined member read (mov [this+8]) in base, the
// vectora end() inline-vs-call LTCG class, non-steerable. trail: collision_geometry.md
// STATE[91.38%|DONE]
void collision_geometry::unsubscribe( collision_geometry_subscriber* subscriber )
{
	ASSERT( UNKNOWN_EXPRESSION );
	m_subscribers.erase( std::find( m_subscribers.begin( ), m_subscribers.end( ), subscriber ) );

	if ( m_subscribers.empty( ) )
		remove( );
}

// STATE[100%|DONE]
void collision_geometry::set_transform( float4x4 const& transform )
{
	m_ghost_object->set_transform( transform );
}

// STATE[100%|DONE]
float4x4 collision_geometry::get_transform( )
{
	return m_ghost_object->get_transform( );
}

} // namespace survarium
