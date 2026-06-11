////////////////////////////////////////////////////////////////////////////
//	Created 	: 03.11.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/collision_geometry.h>

#include <vostok/physics/ghost_object.h>

namespace survarium {

// STATE[88.12%|DONE]
collision_geometry::collision_geometry( ) :
	m_physics_world		( NULL ),
	m_ghost_object		( NULL ),
	m_group				( 0 ),
	m_mask				( 0 )
{
	// STRUCTURE DIFF: target 0 stmts / base 0 stmts (member-init list only) - no diverging rows
	// VERDICT: STRUCTURE MATCH (shape ok) - base INLINES the collision::game_object base ctor (out-of-line call in target; collision is an out-of-scope module), cascading frame 0x18 vs 0x24 + slot renames, non-steerable.
}

// STATE[99.74%|DONE]
collision_geometry::~collision_geometry( )
{
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION_T( m_ghost_object ) );
	destroy_ghost_object( );

	// STRUCTURE DIFF: target 3 stmts / base 3 stmts (0x4c both) - no diverging rows
	// VERDICT: STRUCTURE MATCH (shape ok) - residual is frame 0x3c vs 0x38 slot allocation + ASSERT empty-stub COMDAT-fold misname, non-steerable.
}

// STATE[100%|DONE]
void collision_geometry::destroy_ghost_object( )
{
	physics::destroy_ghost_object( m_ghost_object );
}

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

	// STRUCTURE DIFF: target 15 stmts / base 15 stmts
	// SIZE -0x6 | 69 | LOG_WARNING( "invalid collision_geometry" );
	// SIZE +0x1 | 76 | physics::bt_collision_shape_ptr shape	= physics::create_compound_shape( meshes, float3( 1.0f, 1.0f, 1.0f ), m_name.c_str( ) );
	// SIZE -0x1 | 77 | m_ghost_object							= physics::create_ghost_object( shape, transform );
	// SIZE -0x2 | 78 | shape->set_no_delete( );
	// VERDICT: STRUCTURE MATCH (shape ok) - SIZE rows are LOG_WARNING boost::function ctor inlining, create_* call-boundary arg registers, and the promoted intrusive_ptr::operator* convention (target mov ecx,eax vs base ecx-return), all whole-program LTCG/ICF, non-steerable.
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

// STATE[93.33%|DONE]
void collision_geometry::get_shapes_centers( vectora<float3>& centers_results ) const
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_ghost_object ) );
	m_ghost_object->non_compound_shapes_centers( centers_results );

	// STRUCTURE DIFF: target 2 stmts / base 2 stmts
	// SIZE -0x1 | 123 | m_ghost_object->non_compound_shapes_centers( centers_results );
	// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is the centers_results arg on the stack (push ecx) in target vs the LTCG-promoted ecx register convention in base, call-boundary arg passing, non-steerable.
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

// STATE[91.38%|DONE]
void collision_geometry::unsubscribe( collision_geometry_subscriber* subscriber )
{
	ASSERT( UNKNOWN_EXPRESSION );
	m_subscribers.erase( std::find( m_subscribers.begin( ), m_subscribers.end( ), subscriber ) );

	if ( m_subscribers.empty( ) )
		remove( );

	// STRUCTURE DIFF: target 4 stmts / base 4 stmts
	// SIZE -0x5 | 166 | m_subscribers.erase( std::find( m_subscribers.begin( ), m_subscribers.end( ), subscriber ) );
	// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is m_subscribers.end() out-of-line in target vs inlined member read in base, the vector end() inline-vs-call LTCG class, non-steerable.
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
